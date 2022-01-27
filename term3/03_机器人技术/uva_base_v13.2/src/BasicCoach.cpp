/*
Copyright (c) 2000-2003, Jelle Kok, University of Amsterdam
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

3. Neither the name of the University of Amsterdam nor the names of its
contributors may be used to endorse or promote products derived from this
software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/*! \file BasicCoach.cpp
<pre>
<b>File:</b>          BasicCoach.cpp
<b>Project:</b>       Robocup Soccer Simulation Team: UvA Trilearn
<b>Authors:</b>       Jelle Kok
<b>Created:</b>       03/03/2001
<b>Last Revision:</b> $ID$
<b>Contents:</b>      This file contains the class definitions for the
                      BasicCoach which contains the main structure for the
                      coach.
<hr size=2>
<h2><b>Changes</b></h2>
<b>Date</b>             <b>Author</b>          <b>Comment</b>
03/03/2001        Jelle Kok       Initial version created
</pre>
*/

#include"BasicCoach.h"
#include"Parse.h"
#ifdef WIN32
#include <windows.h>
#else
#include <sys/poll.h>
#endif

extern Logger Log; /*!< This is a reference to the Logger to write loginfo to*/

extern ActHandler *ACT;
/*! This function is executed when a SIGALARM singal arrives. The time this
    signal comes is defined by the SenseHandler (depending on incoming
    sense_body messages). When the signal arrives, the commands currently
    stored in the queue of the ActHandler are send to the server (using the
    method sendCommands).
\param i is ignored */
#ifdef WIN32
extern void CALLBACK sigalarmHandler ( UINT , UINT , DWORD , DWORD , DWORD )
#else
extern void sigalarmHandler ( int i )
#endif
{
    Log.logFromSignal ( 2, "alarm handler!!" );
    ACT->sendCommands( );
}

/*!This is the constructor for the BasicCoach class and contains the
   arguments that are used to initialize a coach.
   \param act ActHandler to which the actions can be sent
   \param wm WorldModel which information is used to determine action
   \param ss ServerSettings that contain parameters used by the server
   \param strTeamName team name of this player
   \param dVersion version this basiccoach corresponds to
   \param isTrainer indicates whether the coach is a trainer (offline coach)
          or an online coach (used during the match). */
BasicCoach::BasicCoach ( ActHandler* act, WorldModel *wm, ServerSettings *ss,
                         char* strTeamName, double dVersion, bool isTrainer )

{
    char str[MAX_MSG];

    ACT       = act;
    WM        = wm;
    SS        = ss;
    bContLoop = true;
    WM->setTeamName ( strTeamName );

    if ( !isTrainer )
        sprintf ( str, "(init %s (version %f))", strTeamName, dVersion );
    else
        sprintf ( str, "(init (version %f))", dVersion );

    ACT->sendMessage ( str );
}

BasicCoach::~BasicCoach( )
{
}

/*! This method is the main loop of the coach. All sequence of actions are
    located in this method. */
void BasicCoach::mainLoopNormal( )
{
#ifdef WIN32
    Sleep ( /*1000*/50 );
#else
    poll ( 0, 0, /*1000*/50 );
#endif

    bool bSubstituted   = false;
    ACT->sendMessageDirect ( "(eye on)" );

#ifdef WIN32
    Sleep ( /*1000*/50 );
#else
    poll ( 0, 0, /*1000*/50 );
#endif

    while ( WM->getPlayMode() != PM_TIME_OVER  && bContLoop )
    {
        Log.log ( 1, "in loop %d %d %f",
                  WM->getTimeLastSeeGlobalMessage().getTime(),
                  bSubstituted,
                  WM->isConfidenceGood ( OBJECT_TEAMMATE_11 ) ) ;
        if ( WM->waitForNewInformation() == false )
        {
            printf ( "Shutting down coach\n" );
            bContLoop = false;
        }
        else if ( WM->getTimeLastSeeGlobalMessage().getTime() == 0 &&
                  bSubstituted == false &&
                  WM->isConfidenceGood ( OBJECT_TEAMMATE_11 ) )
        {
            // read (and write) all player_type information
            for ( int i = 0 ; i < MAX_HETERO_PLAYERS; i ++ )
            {
                m_player_types[i] = WM->getInfoHeteroPlayer ( i );
//       cout << i << ": " ;
//       m_player_types[i].show( cout );
            }

            // just substitute some players (define your own methods to
            // determine which player types should be substituted )
//             substitutePlayer ( 2, 7 );  // substitute player 2 to type 1
//             substitutePlayer ( 3, 11 );
//             substitutePlayer ( 4, 14 );
//             substitutePlayer ( 5, 9 );
//             substitutePlayer ( 6, 2 );
//             substitutePlayer ( 7, 10 );
//             substitutePlayer ( 8, 15 );
//             substitutePlayer ( 9, 12 );
//             substitutePlayer ( 10, 5 );
//             substitutePlayer ( 11, 17 );
			//substitutePlayer ( 11, 17 );
            bSubstituted = true;
        }

        if ( Log.isInLogLevel ( 456 ) )
            WM->logObjectInformation ( 456, OBJECT_ILLEGAL );
        if ( SS->getSynchMode() == true )
            ACT->sendMessageDirect ( "(done)" );
    }

    return;
}


/*! This method substitutes one player to the given player type and sends
    this command (using the ActHandler) to the soccer server. */
void BasicCoach::substitutePlayer ( int iPlayer, int iPlayerType )
{
    SoccerCommand soc;
    soc.makeCommand ( CMD_CHANGEPLAYER, ( double ) iPlayer, ( double ) iPlayerType );
    ACT->sendCommandDirect ( soc );
    cerr << "coachmsg: changed player " << iPlayer << " to type " << iPlayerType
         << endl;
}


#ifdef WIN32
DWORD WINAPI stdin_callback ( LPVOID v )
#else
void* stdin_callback ( void * v )
#endif
{
    Log.log ( 1, "Starting to listen for user input" );
    BasicCoach* bc = ( BasicCoach* ) v;
    bc->handleStdin();
    return NULL;
}

/*!This method listens for input from the keyboard and when it receives this
   input it converts this input to the associated action. See
   showStringCommands for the possible options. This method is used together
   with the SenseHandler class that sends an alarm to indicate that a new
   command can be sent. This conflicts with the method in this method that
   listens for the user input (fgets) on Linux systems (on Solaris this isn't a
   problem). The only known method is to use the flag SA_RESTART with this
   alarm function, but that does not seem to work under Linux. If each time
   the alarm is sent, this gets function unblocks, it will cause major
   performance problems. This function should not be called when a whole match
   is played! */
void BasicCoach::handleStdin( )
{
    char buf[MAX_MSG];

    while ( bContLoop )
    {
#ifdef WIN32
        cin.getline ( buf, MAX_MSG );
#else
        fgets ( buf, MAX_MSG, stdin ); // does unblock with signal !!!!!
#endif
        printf ( "after fgets: %s\n", buf );
        executeStringCommand ( buf );
    }
}

/*!This method prints the possible commands that can be entered by the user.
   The whole name can be entered to perform the corresponding command, but
   normally only the first character is sufficient. This is indicated by
   putting brackets around the part of the command that is not needed.
   \param out output stream to which the possible commands are printed */
void BasicCoach::showStringCommands ( ostream& out )
{
    out << "Basic commands:"                                << endl <<
    " m(ove) player_nr x y"                          << endl <<
    " q(uit)"                                        << endl;
}

/*!This method executes the command that is entered by the user. For the
   possible command look at the method showStringCommands.
   \param str string that is entered by the user
   \return true when command could be executed, false otherwise */
bool BasicCoach::executeStringCommand ( char *str )
{
    switch ( str[0] )
    {
    case 'm':                               // move
        sprintf ( str, "(move %d %f %f)", Parse::parseFirstInt ( &str ),
                  Parse::parseFirstDouble ( &str ),
                  Parse::parseFirstDouble ( &str ) );
        break;
    case 'q':                             // quit
        bContLoop = false;
        return true;
    default:                             // default: send entered string
        ;
    }
    printf ( "send: %s\n", str );
    ACT->sendMessage ( str );
    return true;
}
