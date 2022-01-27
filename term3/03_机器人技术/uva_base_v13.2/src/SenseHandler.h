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

/*! \file SenseHandler.h
<pre>
<b>File:</b>          SenseHandler.h
<b>Project:</b>       Robocup Soccer Simulation Team: UvA Trilearn
<b>Authors:</b>       Jelle Kok
<b>Created:</b>       28/11/2000
<b>Last Revision:</b> $ID$
<b>Contents:</b>      This file contains the class SenseHandler that is used
               to process the information coming from the server.
<hr size=2>
<h2><b>Changes</b></h2>
<b>Date</b>             <b>Author</b>          <b>Comment</b>
28/11/2000      Jelle Kok       Initial version created
</pre>
*/

#ifndef _SENSEHANDLER_
#define _SENSEHANDLER_

#include "Connection.h"
#include "WorldModel.h"

extern Logger Log; /*!< This is a reference to the Logger to write  info to */

#ifdef WIN32
DWORD WINAPI sense_callback( LPVOID v );
#else
void* sense_callback( void *v );
#endif


using namespace std;

/*****************************************************************************/
/********************* CLASS SENSEHANDLER ************************************/
/*****************************************************************************/

/*! Class SenseHandler receives input from a (Robocup Simulation)
    server (through an instance of Connection). The class contains
    methods to parse the incoming messages and sends these to the
    WorldModel accordingly. After this class is instantiated, a
    specific thread must call the function sense_callback.  This
    thread will then automatically call handleMessagesFromServer()
    which loops forever. In this way all messages are received and
    parsed (since the receiveMessage from the Connection blocks till a
    message arrives). Other threads can think about the next action
    while the SenseHandler sends the new information to the
    WorldModel. */
class SenseHandler {
    WorldModel     *WM;        /*!< Worldmodel containing all data of the match*/
    ServerSettings *SS;        /*!< ServerSettings with all server settings    */
    PlayerSettings *PS;        /*!< PlayerSettings with all client settings    */
    Connection* connection;    /*!< Connection with server to receive messages */
    int         iTimeSignal;   /*!< Wait time (microsec) before sense calls act*/
    int         iTriCounter;   /*!< Indicates when see message will arive      */
    int         m_iSeeCounter; /*!<! Used to count number of see msg in 1 cycle*/
    int         iSimStep;      /*!< Length (microsec) of server cycles         */
#ifdef WIN32
    UINT        iTimer;        /*!< timer used to call sigalarmHandler function*/
    UINT        timerRes;      /*!< timer resolution for the application       */
#else
    struct      itimerval itv; /*!< timer used to set alarm to send action     */
#endif

    //!add by zhaoyibin
    //!beg
    timeval   m_tvRcvSenseBody; /*! local time recv send_body msg*/
    int       m_maxMsgDelay;    /*!<( micro second ) max delay from send_body msg to see msg*/
    //!end

public:
    SenseHandler( Connection* c, WorldModel* wm, ServerSettings *ss,
                  PlayerSettings *ps );

    // start the loop to handle the messages from the server
    void    handleMessagesFromServer      (              );

    // methods to determine when the next action should be sent to the server.
    void    setTimeSignal                 (                            );

    // method to analyze incoming messages
    bool    analyzeMessage                ( char *strMsg               );
    bool    analyzeSeeGlobalMessage       ( char *strMsg               );
    bool    analyzeFullStateMessage       ( char *strMsg               );
    bool    analyzeSeeMessage             ( char *strMsg               );
    bool    analyzeSenseMessage           ( char *strMsg               );
    bool    analyzeInitMessage            ( char *strMsg               );
    bool    analyzeHearMessage            ( char *strMsg               );
    bool    analyzePlayerMessage          ( int  iTime  , char *strMsg );
    bool    analyzeCoachMessage           ( char *strMsg               );
    bool    analyzeChangePlayerTypeMessage( char *strMsg               );
    bool    analyzeServerParamMessage     ( char *strMsg               );
    bool    analyzeCheckBall              ( char *strMsg               );
    bool    analyzePlayerTypeMessage      ( char *strMsg               );
    bool    analyzePlayerParamMessage     ( char *strMsg               );

    // utility functions
    bool    readServerParam               ( const  char *strParam,
                                            char *strMsg               );
};

#endif
