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

/*! \file ActHandler.cpp
<pre>
<b>File:</b>          ActHandler.cpp
<b>Project:</b>       Robocup Soccer Simulation Team: UvA Trilearn
<b>Authors:</b>       Jelle Kok
<b>Created:</b>       28/11/2000
<b>Last Revision:</b> $ID$
<b>Contents:</b>      This file contains the class definitions for the
               ActHandler that handles the outgoing messages to the
               server.
<hr size=2>
<h2><b>Changes</b></h2>
<b>Date</b>             <b>Author</b>          <b>Comment</b>
28/11/2000       Jelle Kok       Initial version created
</pre>
*/

#include "ActHandler.h"

#ifndef WIN32
#include <poll.h>     // poll
#include <sys/poll.h> // poll
#endif
#include <signal.h>     // SIGALARM
#include <cstring>

ActHandler* ACT; /*!< Pointer to ActHandler class needed by signal handler */

/*! This is the constructor for the ActHandler class. All the variables are
    initialized.
    \param c Connection that is connected with the soccerserver
    \param wm WorldModel, used to set performed commands
    \param ss ServerSettings in which server settings are defined */
ActHandler::ActHandler( Connection *c, WorldModel *wm, ServerSettings *ss )
{
    connection          = c;
    SS                  = ss;
    WM                  = wm;

    m_iMultipleCommands = 0;
    ACT                 = this; // needed to let signal call method from class
}

/*! This method empties the queue in which all the commands are stored. */
void ActHandler::emptyQueue( )
{
    m_queueOneCycleCommand.commandType = CMD_ILLEGAL;
    for ( int i = 0; i < CMD_MAX_COMMANDS - 1 ; i ++ )
        m_queueMultipleCommands[i].commandType = CMD_ILLEGAL;
    m_iMultipleCommands=0;
}

/*! This method returns whether the current queue contains no commands
    \return true when queue is empty, false otherwise */
bool ActHandler::isQueueEmpty()
{
    return m_queueOneCycleCommand.commandType == CMD_ILLEGAL &&
           m_iMultipleCommands                == 0;
}

/*! This method converts all commands in the queue to text strings and sends
    these text strings to the server (connected by Connection). When the server
    didn't execute the commands from the previous cycle (this information is
    stored in the WorldModel) the commands in the queue are not sent, since it
    is probably the case that these commands will be performed this cycle and
    we don't want a clash (two commands in one cycle). In this case false is
    returned.
    \return true when sending of messages succeeded, false otherwise */
bool ActHandler::sendCommands( )
{
    static Time timeLastSent = -1;
    bool        bNoOneCycle  = false;
    char        strCommand[MAX_MSG];
    strCommand[0]            = '\0';

    if ( WM->getCurrentTime() == timeLastSent )
    {
        Log.logFromSignal( 2, " already sent message; don't send" );
        return false;
    }

    if (  WM->isQueuedActionPerformed()    == false &&  // don't send action when
            m_queueOneCycleCommand.commandType != CMD_CATCH && // previous one is not
            WM->isFullStateOn() == false )                     // fullstate sense not
    {                                                     // not processed yet
        Log.logFromSignal( 2, " previous message not processed yet; don't send" );
        return false;                                    // except with catch since
    }                                                  // too important

    // make string of primary action and put it in 'strCommand' variable
    bool bReturn = m_queueOneCycleCommand.getCommandString( strCommand, SS );

    if ( bReturn == false )
        cerr << WM->getCurrentCycle() << ", " <<  WM->getPlayerNumber() << " "
             << "Acthandler::failed to create primary command string" << endl;

    if ( strCommand[0] == '\0' )
    {
        bNoOneCycle = true;
        Log.logFromSignal( 2, " no primary action in queue" );
    }

    // make string of all other commands and add them to the end of 'strCommand'
    for ( int i = 0; i < m_iMultipleCommands ; i ++ )
    {
        bReturn = m_queueMultipleCommands[i].getCommandString(
                      &strCommand[strlen(strCommand)], SS );
        if ( bReturn == false )
            cerr << WM->getCurrentCycle() << ", " <<  WM->getPlayerNumber() << " "
                 << "Acthandler::failed to create secondary command string " <<
                 m_queueMultipleCommands[i].commandType <<  endl;
    }

    char strComm[MAX_SAY_MSG];
    strcpy( strComm, WM->getCommunicationString() );
    if ( strlen( strComm ) != 0 )
    {
        sprintf( &strCommand[strlen(strCommand)], "(say \"%s\")", strComm );
        WM->setCommunicationString( "" );
    }

    // send the string to the server (example string: (dash 100)(turn_neck -19))
    if ( strCommand[0] != '\0' )
    {
        timeLastSent        = WM->getCurrentTime();
        connection->sendMessage( strCommand );
        Log.logFromSignal( 2, " send queued action to server: %s", strCommand);
    }
    else
    {
        Log.logFromSignal( 2, " no action in queue??" );
        return false;
    }

    if ( ! bNoOneCycle ) // if primary action was send, place it at end of array
        m_queueMultipleCommands[m_iMultipleCommands++] = m_queueOneCycleCommand;

    // let worldmodel know which commands were sent to the server
    WM->processQueuedCommands( m_queueMultipleCommands, m_iMultipleCommands );
    m_iMultipleCommands = 0;

    // decrease amount of times primary action still has to be sent, if 0 delete
    // it, furthermore set number of multiple commands to zero
    if ( --m_queueOneCycleCommand.iTimes  == 0 )
        m_queueOneCycleCommand.commandType = CMD_ILLEGAL;

    for ( int i = 0; i < CMD_MAX_COMMANDS; i++ )
        m_queueMultipleCommands[i].commandType = CMD_ILLEGAL;

    return true;
}


/*! This method returns the primary command that is currently stored in the
    queue. */
SoccerCommand ActHandler::getPrimaryCommand(  )
{
    return m_queueOneCycleCommand;
}

/*! This method puts a SoccerCommand in the queue. The last added command
    will be sent to the soccerserver when the method sendCommands is performed.
    Normally this is done when a signal set by the SenseHandler arrives.
    \param command SoccerCommand that should be put in the queue.
    \return true when command is added, false otherwise (queue is full) */
bool ActHandler::putCommandInQueue( SoccerCommand command )
{
    int i;
    bool bOverwritten = false;

    if ( command.commandType == CMD_ILLEGAL )
        return false;
    if ( SoccerTypes::isPrimaryCommand( command.commandType ) )
        m_queueOneCycleCommand = command;           // overwrite primary command
    else                                          // non-primary command
    {
        for ( i = 0; i < m_iMultipleCommands ; i ++ )
            if ( m_queueMultipleCommands[i].commandType == command.commandType )
            {
                m_queueMultipleCommands[i] = command;   // if command already in queue
                bOverwritten = true;                    // overwrite it
            }

        // 1 less to save space for primary command
        if ( bOverwritten == false && m_iMultipleCommands == CMD_MAX_COMMANDS-1 )
        {
            cerr << "(ActHandler::putCommandInQueue) too many commands" << endl;
            return false;
        }
        if ( bOverwritten == false  ) // add it when command was not yet in queue
            m_queueMultipleCommands[m_iMultipleCommands++] = command;
    }

    return true;
}

/*! This method sends a single command directly to the server. First a string
    is made from the SoccerCommand and afterwards this string is send to the
    server using the method sendMessage.
    \param soc SoccerCommand that should be send to the server.
    \return true when message was sent, false otherwise */
bool ActHandler::sendCommand( SoccerCommand soc )
{
    char strCommand[MAX_MSG];
    soc.getCommandString( strCommand, SS );
    return sendMessage( strCommand );
}

/*! This method sends a single string directly to the server. To make sure
    this message arrives, the time of one complete cycle is waited before and
    after the message is sent.
    \param str string that should be sent to the server
    \return true when message was sent, false otherwise */
bool ActHandler::sendMessage( const char * str )
{
    emptyQueue( );
#ifdef WIN32
    Sleep( SS->getSimulatorStep() );
#else
    poll( 0, 0, /*SS->getSimulatorStep()*/10 );
#endif

    bool bReturn = connection->sendMessage( str );
    Log.logFromSignal( 2, " send message to server and wait: %s", str);

#ifdef WIN32
    Sleep( SS->getSimulatorStep() );
#else
    poll( 0, 0, /*SS->getSimulatorStep()*/10 );
#endif
    return bReturn;
}

/*! This method sends a single command directly to the server. First a string
    is made from the SoccerCommand and afterwards this string is send to the
    server using the method sendMessageDirect.
    \param soc SoccerCommand that should be send to the server.
    \return true when message was sent, false otherwise */
bool ActHandler::sendCommandDirect( SoccerCommand soc )
{
    char strCommand[MAX_MSG];
    if ( soc.commandType == CMD_ILLEGAL )
        return false;
    soc.getCommandString( strCommand, SS );
    return sendMessageDirect( strCommand );
}

/*! This method sends a single string directly to the server.
    \param str string that should be sent to the server
    \return true when message was sent, false otherwise */
bool ActHandler::sendMessageDirect( const char * str )
{
    bool bReturn = connection->sendMessage( str );
    Log.logFromSignal( 2, " send message to server directly: %s", str);
    return bReturn;
}

