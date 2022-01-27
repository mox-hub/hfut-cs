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

/*! \file Player.h
<pre>
<b>File:</b>          Player.h
<b>Project:</b>       Robocup Soccer Simulation Team: UvA Trilearn
<b>Authors:</b>       Jelle Kok
<b>Created:</b>       03/03/2001
<b>Last Revision:</b> $ID$
<b>Contents:</b>      This file contains the declaration for the Player class,
               which is a superclass from BasicPlayer and contains the
               decision procedure to select the skills from the
               BasicPlayer.
<hr size=2>
<h2><b>Changes</b></h2>
<b>Date</b>             <b>Author</b>          <b>Comment</b>
03/03/2001       Jelle Kok       Initial version created
</pre>
*/

#ifndef PLAYER
#define PLAYER

#include "BasicPlayer.h"
#include "Formations.h"     // needed for Formations

#ifdef WIN32
DWORD WINAPI stdin_callback ( LPVOID v );
#else
void* stdin_callback ( void * v );
#endif

#ifdef WIN32
void CALLBACK sigalarmHandler ( UINT id, UINT msg, DWORD dwUser, DWORD dw1, DWORD dw2 );
#else
void sigalarmHandler ( int i );
#endif


/*! This class is a superclass from BasicPlayer and contains a more
    sophisticated decision procedure to determine the next action. */
class Player:public BasicPlayer
{
    Formations    *formations;             /*!< all formation information      */
    bool          bContLoop;               /*!< is server is alive             */

    Time          m_timeLastSay;           /*!< last time communicated         */
    ObjectT       m_objMarkOpp;            /*!< last marked opponent           */
    ObjectT       m_objPassOption;         /*!< passing option in kick         */

    int           m_iPenaltyNr;            /*!< number of current penalty      */

    ActionT       m_actionPrev;            /*!< previous action of this agent  */

    void          performPenalty ( );


    VecPosition   getDeadBallPosition ( );

    // methods associated with saying (defined in Player.C)
    bool          shallISaySomething ( SoccerCommand  soc );
    bool          amIAgentToSaySomething ( SoccerCommand  soc );
    void          sayOppAttackerStatus ( char *         str );
    void          sayBallStatus ( char *         str );
    void          makeBallInfo ( VecPosition    posBall,
                                 VecPosition    velBall,
                                 int            iDiff,
                                 char *         str );

public:
    Player ( ActHandler     *a,
             WorldModel     *wm,
             ServerSettings *ss,
             PlayerSettings *cs,
             Formations     *fs,
             char           *strTeamName,
             double         dVersion,
             int            iReconnect = -1 );

    void          mainLoop ( );


    // methods that deal with user input (from keyboard) to sent commands
    void          handleStdin ( );
    void          showStringCommands ( ostream& out );
    bool          executeStringCommand ( char *str );

    // simple teams (PlayerTeams.cpp)
    SoccerCommand deMeer5 ( );
    SoccerCommand deMeer5_goalie ( );
    SoccerCommand goalieMainLoop ( );
    SoccerCommand defenderMainLoop ( );
    SoccerCommand midfielderMainLoop ( );
    SoccerCommand attackerMainLoop ( );
};

#endif
