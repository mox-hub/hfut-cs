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

/*! \file ActHandler.h
<pre>
<b>File:</b>          ActHandler.h
<b>Project:</b>       Robocup Soccer Simulation Team: UvA Trilearn
<b>Authors:</b>       Jelle Kok
<b>Created:</b>       28/11/2000
<b>Last Revision:</b> $ID$
<b>Contents:</b>      This file contains the class declarations for the
               ActHandler that handles the outgoing messages to the
               server.
<hr size=2>
<h2><b>Changes</b></h2>
<b>Date</b>             <b>Author</b>          <b>Comment</b>
28/11/2000       Jelle Kok       Initial version created
</pre>
*/

#ifndef _ACTHANDLER_
#define _ACTHANDLER_

#include "Connection.h"      // needed for Connection class
#include "WorldModel.h"      // needed for 'setPerformedActions'

void   sendChangeViewCommands( int iSyncCounter );

extern Logger Log; /*!< Reference to the Logger to write log info to*/

/*****************************************************************************/
/********************** CLASS ACTHANDLER *************************************/
/*****************************************************************************/

/*!The ActHandler Class is used in the RoboCup Soccer environment to send the
   commands to the soccerserver. The ActHandler contains a queue in which the
   commands are put. When a signal arrives (set by the SenseHandler depending
   on the time of the sense_body message) the commands that are currently in
   the queue are converted to text strings and send to the server. The sent
   commands are also passed to the WorldModel, such that the WorldModel can
   update its internal state based on the performed actions.
   It is possible to send more than one command to the server at each time
   step, but some type of (primary) commands can only be sent once (kick,dash,
   move, tackle, turn and catch). Therefore internally different queues are
   stored. One with only one element, namely the last entered primary command.
   One with all the change_view commands (since these have to be sent at
   special times for the synchronization). And finally a separate queue
   containing all the other commands. Each time a command is put into the queue
   that is already there, the command is updated with the new information.
   Furthermore it is also possible to directly send commands (or text strings)
   to the server. These methods can be used when an initialization or move
   command has to be sent to the server and you're sure this information
   is final, i.e. the message will not become better when new information
   arrives from the server. */
class ActHandler {

    Connection     *connection;  /*!< Connection with the server               */
    ServerSettings *SS;          /*!< ServerSettings with server parameters    */
    WorldModel     *WM;          /*!< needed to set performed actions          */

    SoccerCommand  m_queueOneCycleCommand; /*!< primary command is saved here  */
    SoccerCommand  m_queueMultipleCommands[CMD_MAX_COMMANDS];
    /*!< non primary commands*/
    int            m_iMultipleCommands;    /*!< number of non-primary commands */

public:
    ActHandler( Connection* c, WorldModel *wm, ServerSettings *ss);

    // methods related to putting and sending messages using the queue
    bool           putCommandInQueue( SoccerCommand command      );
    void           emptyQueue       (                            );
    bool           isQueueEmpty     (                            );
    bool           sendCommands     (                            );
    SoccerCommand  getPrimaryCommand(                            );

    // methods to send commands directly to the server
    bool           sendCommand      ( SoccerCommand soc          );
    bool           sendMessage      ( const  char     *str       );
    bool           sendCommandDirect( SoccerCommand soc          );
    bool           sendMessageDirect( const    char     *str     );

} ;
#endif
