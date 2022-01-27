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

/*! \file BasicCoach.h
<pre>
<b>File:</b>          BasicCoach.h
<b>Project:</b>       Robocup Soccer Simulation Team: UvA Trilearn
<b>Authors:</b>       Jelle Kok
<b>Created:</b>       03/03/2001
<b>Last Revision:</b> $ID$
<b>Contents:</b>      This file contains the class declarations for the
                      BasicCoach which contains the main structure for the
                      case.
<hr size=2>
<h2><b>Changes</b></h2>
<b>Date</b>             <b>Author</b>          <b>Comment</b>
03/03/2001        Jelle Kok       Initial version created
</pre>
*/

#ifndef _BASICCOACH_
#define _BASICCOACH_

#include "ActHandler.h"

#ifdef WIN32
DWORD WINAPI stdin_callback( LPVOID v );
#else
void* stdin_callback( void * v );
#endif

#ifdef WIN32
void CALLBACK sigalarmHandler(UINT id, UINT msg, DWORD dwUser, DWORD dw1, DWORD dw2);
#else
void sigalarmHandler( int i );
#endif

/*! This class starts a simple coach, which actions are defined in the method
    mainLoop. It uses an ActHandler to send actions to the server and can
    receive information from the WorldModel. The declaration of the different
    methods are defined in BasicCoachTest.C and BasicCoach.C. */
class BasicCoach
{
protected:
    ActHandler     *ACT; /*!< ActHandler to which commands can be sent         */
    WorldModel     *WM;  /*!< WorldModel that contains information of world    */
    ServerSettings *SS;  /*!< All parameters used by the server                */

    HeteroPlayerSettings m_player_types[MAX_HETERO_PLAYERS];
    bool bContLoop;      /*!< bool to indicate whether to stop or continue     */

public:
    BasicCoach( ActHandler* a, WorldModel *wm, ServerSettings *ss,
                char* strTeamName, double dVersion, bool isTrainer );
    virtual ~BasicCoach( );

    virtual void mainLoopNormal   ( ); // virtual can be overwritten in subclass
    void         substitutePlayer( int iPlayer, int iPlayerType );

    // methods that deal with user input (from keyboard) to sent commands
    void          handleStdin               (                                  );
    void          showStringCommands        ( ostream& out                     );
    bool          executeStringCommand      ( char *str                        );

};

#endif
