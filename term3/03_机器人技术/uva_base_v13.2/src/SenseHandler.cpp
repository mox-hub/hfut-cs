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

/*! \file SenseHandler.cpp
<pre>
<b>File:</b>          SenseHandler.cpp
<b>Project:</b>       Robocup Soccer Simulation Team: UvA Trilearn
<b>Authors:</b>       Jelle Kok
<b>Created:</b>       28/11/2000
<b>Last Revision:</b> $ID$
<b>Contents:</b>      This file contains the class SenseHandler that is used
               to process the information coming from the server.
<hr size=2>
<h2><b>Changes</b></h2>
<b>Date</b>             <b>Author</b>          <b>Comment</b>
28/11/2000       Jelle Kok       Initial version created
</pre>
*/

#include "SenseHandler.h"
#include "ActHandler.h"  // sigalarmHandler
#include "Parse.h"

#include <signal.h>   // needed for SIGALARM
#include <string.h>   // needed for strlen
#include <stdio.h>    // needed for printf
#include <iostream>   // needed for cout

/*****************************************************************************/
/********************* CLASS SENSEHANDLER ************************************/
/*****************************************************************************/

/*! This function is needed to start the Sense Thread (thread that continually
    waits for input and parses the input). This function is needed since it is
    not possible to call a method from a class using a thread. So this function
    calls handleMessagesFromServer from the SenseHandler class.
\param v pointer to a SenseHandler class.*/
#ifdef WIN32
DWORD WINAPI sense_callback ( LPVOID v )
#else
void* sense_callback ( void *v )
#endif
{
    Log.log ( 1, "Starting to listen for server messages" );
    SenseHandler* s = ( SenseHandler* ) v;
    s->handleMessagesFromServer( );
    return NULL;
}


extern  void  sigalarmHandler ( int );
/*! Constructor for the SenseHandler. It needs a reference to a connection and
    a reference to a worldmodel.
    \param c Connection from which input is received
    \param wm WorldModel to which new information will be sent for processing
    \param ss ServerSettings that contain the parameters used by the server
    \param ps PlayerSettings that determine how to interact with messages. */
SenseHandler::SenseHandler ( Connection *c, WorldModel *wm, ServerSettings *ss,
                             PlayerSettings *ps )
{
    connection             = c;
    SS                     = ss;
    PS                     = ps;
    WM                     = wm;
    iSimStep               = SS->getSimulatorStep() *1000;
    iTimeSignal            = ( int ) ( iSimStep*0.85 );
    //!new add
    m_maxMsgDelay = 30000;

#ifdef WIN32
    TIMECAPS tc;
    UINT     resolution = 1;   // timer resolution for the application (ms)
    iTimer = NULL;

    // set the minimum timer resolution for an application
    if ( TIMERR_NOERROR == timeGetDevCaps ( &tc, sizeof ( TIMECAPS ) ) )
    {
        timerRes = min ( max ( tc.wPeriodMin, resolution ), tc.wPeriodMax );
        timeBeginPeriod ( timerRes );
    }
#else
    struct sigaction sigact;

    sigact.sa_flags = SA_RESTART; // do not unblock primitives (like recvfrom)
    sigact.sa_handler = ( void ( * ) ( int ) ) sigalarmHandler;
    sigaction ( SIGALRM, &sigact, NULL );

    // set timer signal to indicate when ActHandler should sent commands to the
    // server, this structure will later be filled with exact timing values
    itv.it_interval.tv_sec  = 0;
    itv.it_interval.tv_usec = 0;
    itv.it_value.tv_sec     = 0;
    itv.it_value.tv_usec    = 0;
#endif
}

/*! This is the main routine of this class. It loops forever (till the thread
    is destroyed) and receives and parses the incoming messages.   */
void SenseHandler::handleMessagesFromServer( )
{
    char strBuf[MAX_MSG];
    int i=0;

    while ( 1 )
    {
        strBuf[0]='\0';
        if ( i != -1 )                                        // if no error
            i = connection->receiveMessage ( strBuf, MAX_MSG ); // get message
        if ( strBuf[0] != '\0' )                              // if not empty
            analyzeMessage ( strBuf );                          // parse message
    }
}



/*! This method sets the time signal. This is the time that should be
    waited before the next action should be sent to the server. As
    soon as a sense message arrives this method is called. Using the
    information from the member variable 'iTriCounter' which denotes
    when the see message will arrive in this cycle (0=first half,
    1=2nd half, 2=no see, all for the default view frequency) the
    timer is set. The values that denote the fraction of the
    simulation step that is waited are all defined in PlayerSettings,
    such that they can be easily changed. */
void SenseHandler::setTimeSignal( )
{
    //!modify
//     if ( WM->getAgentViewFrequency() == 1.0 ) // VA_NORMAL AND VQ_HIGH (default)
//     {
//         if ( iTriCounter % 3 == 0 )            // see will arrive first half cycle
//         {
//             iTimeSignal = ( int ) ( iSimStep * PS->getFractionWaitSeeBegin() );
//             iTriCounter = 0;
//         }
//         else if ( iTriCounter % 3 == 1 )       // see will arrive 2nd half of cycle
//         {
//             iTimeSignal = ( int ) ( iSimStep * PS->getFractionWaitSeeEnd() );
//         }
//         else                                   // no see will arrive
//             iTimeSignal = ( int ) ( iSimStep * PS->getFractionWaitNoSee( ) );
//     }
//     else if ( WM->getAgentViewFrequency() == 2.0 ) // VA_WIDE AND VQ_HIGH
//     {
//         if ( iTriCounter % 3 == 0 )             // see will arrive
//         {
//             iTimeSignal = ( int ) ( iSimStep * PS->getFractionWaitSeeEnd() );
//             iTriCounter = 0;
//         }
//         else                                   // no see will arrive
//             iTimeSignal = ( int ) ( iSimStep * PS->getFractionWaitNoSee() );
//     }
//     else                                     // VA_NARROW AND VQ_HIGH
//         iTimeSignal = ( int ) ( iSimStep * PS->getFractionWaitSeeEnd() );
//
//     iTriCounter++;
// #ifdef WIN32
//     // kill the previous timer
//     if ( iTimer != NULL ) timeKillEvent ( iTimer );
//     // start a new one
//     iTimer = timeSetEvent ( iTimeSignal / 1000, timerRes,
//                             sigalarmHandler, ( DWORD ) 0, TIME_ONESHOT );
// #else
//     itv.it_value.tv_usec = iTimeSignal;
//     setitimer ( c, &itv, NULL );
// #endif
    itv.it_value.tv_usec = m_maxMsgDelay + 20000;
    setitimer( ITIMER_REAL, & itv, NULL);
}

/*! This method analyzes the type of the incoming message and calls the
    message that corresponds to this message.
    \param strMsg message that should be parsed.
    \return bool indicating whether the message was parsed or not */
bool SenseHandler::analyzeMessage ( char *strMsg )
{
    Log.log ( 1, strMsg );
    bool bReturn = false;

    switch ( strMsg[1] )
    {
    case 'c':
        if ( strMsg[2] == 'h' )
            return analyzeChangePlayerTypeMessage ( strMsg );     // ( c hange_
        else//!modify by zhaoyibin
            cout<<strMsg<<endl;                                   // (clang
        break;
    case 'f':
        return analyzeFullStateMessage ( strMsg );     // ( f ullstate_
    case 'o':                                                 // ( o k
        //!modify by zhoayibin
        if ( strlen ( strMsg ) > 14 && strMsg[4] == 'c' && strMsg[10] == 'b' )
            analyzeCheckBall ( strMsg );                        // (ok check_ball
        else if ( strlen ( strMsg ) >= 14 && strMsg[4] == 's' && strMsg[10] == 's' )
            WM->setSynchSeeMode();                                // (ok synch_see)
        return true;
    case 's':
    {
        switch ( strMsg[3] )
        {
        case 'e':
            if ( strMsg[5] == 'g' )
                return analyzeSeeGlobalMessage ( strMsg );  // (se e_g
            else if ( WM->isFullStateOn( ) == false )
                return analyzeSeeMessage ( strMsg );        // (se e
            break;
        case 'n':
            bReturn = analyzeSenseMessage ( strMsg );      // (se n se
            if ( WM->isFullStateOn( ) == true )
                WM->updateAfterSenseMessage( );
            return bReturn;
            break;
        case 'r':
            return analyzeServerParamMessage ( strMsg ); // (se r ver_param
        default :
            break;
        }
    }
    break;
    case 'i':
        return analyzeInitMessage ( strMsg );       // ( i nit
    case 'h':
        return analyzeHearMessage ( strMsg );       // ( h ear
    case 'p':
        return ( strMsg[8] == 't' )
               ? analyzePlayerTypeMessage ( strMsg )  // (player_ t ype
               : analyzePlayerParamMessage ( strMsg ); // (player_ p aram
    case 'e':
        printf ( "(%d,%d) %s\n", WM->getCurrentCycle(),
                 WM->getPlayerNumber(),strMsg );// ( error
        break;
    case 't':
		//cout << " @@@@  ";
        Log.logWithTime ( 2, " incoming think message" );
        WM->processRecvThink ( true );         // ( think
        break;
    default:
        cerr << "(" << WM->getCurrentTime() << ", " <<
             WM->getPlayerNumber()
             << ") (SenseHandler::analyzeMessage) " <<
             "ignored message: " << strMsg << "\n";
        return false;
    }
    return false;
}


/*! This method analyzes a see Message. It gets the time from the message and
    tries to synchronize with the server. Then the message is stored in the
    world model, which processes it when it performs an update.
    \return bool indicating whether the message was parsed correctly. */
bool SenseHandler::analyzeSeeMessage ( char *strMsg )
{
    strcpy ( WM->strLastSeeMessage, strMsg );

    Log.logWithTime ( 2, " %s",strMsg );

    if ( WM->getRelativeDistance ( OBJECT_BALL ) < SS->getVisibleDistance() )
        Log.logWithTime ( 560, "%s", WM->strLastSeeMessage );

    Time    time = WM->getTimeLastRecvSenseMessage();
    int iTime = Parse::parseFirstInt ( &strMsg );        // get the time
    if ( time.getTime() != iTime )
    {
        cerr << "(SenseHandler:analyzeSeeMessage) see and different time as sense:"
             << time.getTime() << " vs. " << iTime << endl;
        return false;
    }

    // count number of see message in this cycle
    if ( WM->getTimeLastSeeMessage() == time )
        m_iSeeCounter++;
    else
        m_iSeeCounter = 1;

    // do nothing with second see, since it adds nothings
    if ( m_iSeeCounter >= 2 )
    {
        Log.logWithTime ( 4, "second see message in cycle; do nothing " );
        return true;
    }

    // reset the send pattern when previous cycle(s) no see arrived
    if ( WM->getAgentViewFrequency() == 1.0 && // VA_NORMAL; previous cycle no see
            time.getTimeDifference ( WM->getTimeLastSeeMessage() ) == 2 )
        iTriCounter = 1;                // see will arive in 2nd half in next cycle
    else if ( WM->getAgentViewFrequency() == 2.0 && // VA_WIDE; two cycles no see
              time.getTimeDifference ( WM->getTimeLastSeeMessage() ) == 3 )
        iTriCounter = 1;                // no see will arrive next two cycles

    WM->setTimeLastSeeMessage ( time );  // set time of last see message

    //!add
    //!begin
    timeval     tvRcvSee;
    gettimeofday ( &tvRcvSee, NULL );
    int iDelay = ( tvRcvSee.tv_sec - m_tvRcvSenseBody.tv_sec ) *1000000
                 +tvRcvSee.tv_usec - m_tvRcvSenseBody.tv_usec;
    if ( iDelay > m_maxMsgDelay )
    {
        m_maxMsgDelay = iDelay;
        if ( m_maxMsgDelay > 60000 )
        {
            cout<<"player "<<SoccerTypes::getIndex ( WM->getAgentObjectType() )
                <<" , server idle: delays "<<m_maxMsgDelay<<" micro seconds"
                <<", set max msg delay to default"<<endl;
            m_maxMsgDelay = 40000;
        }
//     else
//       cout<<"player "<<SoccerTypes::getIndex ( WM->getAgentObjectType() )
//       <<" , adjust max msg delay to: "<<m_maxMsgDelay<<" micro seconds"<<endl;
    }
    //!end

    return true;
}

/*! This method analyzes a see Message. All information from the different
    objects that is stored in a see message is send to worldmodel.
    A see message looks like(see 0 ((g r) 64.1 13) ((f r t) 65.4 -16) ....
    \param strMsg message that should be parsed
    \return bool indicating whether the message was parsed correctly. */
///This function is called by Coach..mmm
bool SenseHandler::analyzeSeeGlobalMessage ( char *strMsg )
{
    Log.logWithTime ( 2, " incoming see global message" );
    strcpy ( WM->strLastSeeMessage, strMsg );

	//cout << WM->getPlayerNumber() << "!!!!!!!!!!!!!" << endl;
    ObjectT o;
    bool    isGoalie;
    double  dX, dY, dVelX, dVelY;
    int     iTime;
    AngDeg  angBody, angNeck;
    Time    time = WM->getCurrentTime();

    iTime = Parse::parseFirstInt ( &strMsg );        // get the time
    time.updateTime ( iTime );

    while ( *strMsg != ')' )                         // " ((objname.." or ")"
    {
        dVelX = dVelY = UnknownDoubleValue;
        angBody = angNeck = UnknownAngleValue;
        strMsg += 2;          // go the start of the object name

        // get the object type at the current position in the string
        o = SoccerTypes::getObjectFromStr ( &strMsg, &isGoalie, WM->getTeamName() );
        if ( o == OBJECT_ILLEGAL )
        {
            Log.log ( 4, "Illegal object" );
            Log.log ( 4, "total messages: %s", WM->strLastSeeMessage );
            Log.log ( 4, "rest of message: %s", strMsg );
        }

        dX = Parse::parseFirstDouble ( &strMsg );       // parse first value
        dY = Parse::parseFirstDouble ( &strMsg );       // parse second value
        if ( *strMsg != ')' )                           // if it was no flag
        {
            dVelX = Parse::parseFirstDouble ( &strMsg );  // parse delta x
            dVelY = Parse::parseFirstDouble ( &strMsg );  // parse delta y
            if ( *strMsg != ')' )                         // stil not finished
            {
                // get body and neck angle
                angBody = Parse::parseFirstDouble ( &strMsg );
                angNeck = Parse::parseFirstDouble ( &strMsg );
            }
        }
        // skip ending bracket of object information.
        Parse::gotoFirstOccurenceOf ( ')', &strMsg );
        strMsg++;

        // process the parsed information (unread values are Unknown...)
        WM->processSeeGlobalInfo ( o, time, VecPosition ( dX,dY ),
                                   VecPosition ( dVelX,dVelY ), angBody, angNeck );
    }
    WM->setTimeLastSeeGlobalMessage ( time ); // set time last see global message
    return true;
}

/*! This method parses a full state message. This message contains all
    information from the playing field without noise. It will not be used
    during real tournaments.
    此时我修改了processNewAgentInfo(add Capacity)但是这个函数在比赛中不使用，所以就不修改啦！在以后的版本，如果有需要，一定要改的！
    当前适应server版本V13.2*/
bool SenseHandler::analyzeFullStateMessage ( char *strMsg )
{
    Log.restartTimer( );
    Log.logWithTime ( 2, " incoming fullstate message" );
    Log.log ( 4, " fullstate message: %s", strMsg );
    strcpy ( WM->strLastSeeMessage, strMsg );

    ObjectT o;
    bool    isGoalie;
    double  dX, dY, dVelX, dVelY;
    int     iTime;
    AngDeg  angBody, angNeck;
    Time    time = WM->getCurrentTime();

    iTime = Parse::parseFirstInt ( &strMsg );        // get the time
    time.updateTime ( iTime );
    Log.log ( 4, "fullstate time: %d", time.getTime() );

    strMsg++;                                      // skip space
    Parse::gotoFirstOccurenceOf ( ' ', &strMsg );  // skip (pmode
    strMsg++;                                      // skip space

    Log.log ( 4, "fullstate parse ref: %s", strMsg );
    RefereeMessageT rm = SoccerTypes::getRefereeMessageFromStr ( strMsg );
    PlayModeT       pm = SoccerTypes::getPlayModeFromRefereeMessage ( rm );
    if ( pm != PM_ILLEGAL )
        WM->setPlayMode ( pm );

    Parse::gotoFirstOccurenceOf ( 'e', &strMsg );  // go to end of vmode
    strMsg++;                                      // skip 'e'
    strMsg++;                                      // skip space

    Log.log ( 4, "fullstate parse qua: %s", strMsg );
    ViewQualityT vq = SoccerTypes::getViewQualityFromStr ( strMsg );
    Parse::gotoFirstOccurenceOf ( ' ', &strMsg );  // go to end of quality
    strMsg++;
    Log.log ( 4, "fullstate parse ang: %s", strMsg );
    ViewAngleT   va = SoccerTypes::getViewAngleFromStr ( strMsg );

    Log.log ( 4, "fullstate parse count: %s", strMsg );
    WM->setNrOfCommands ( CMD_KICK       , Parse::parseFirstInt ( &strMsg ) );
    WM->setNrOfCommands ( CMD_DASH       , Parse::parseFirstInt ( &strMsg ) );
    WM->setNrOfCommands ( CMD_TURN       , Parse::parseFirstInt ( &strMsg ) );
    WM->setNrOfCommands ( CMD_CATCH      , Parse::parseFirstInt ( &strMsg ) );
    WM->setNrOfCommands ( CMD_MOVE       , Parse::parseFirstInt ( &strMsg ) );
    WM->setNrOfCommands ( CMD_TURNNECK   , Parse::parseFirstInt ( &strMsg ) );
    WM->setNrOfCommands ( CMD_CHANGEVIEW , Parse::parseFirstInt ( &strMsg ) );
    WM->setNrOfCommands ( CMD_SAY        , Parse::parseFirstInt ( &strMsg ) );

    int iArmMovable = Parse::parseFirstInt ( &strMsg );
    int iArmExpires = Parse::parseFirstInt ( &strMsg );
    Parse::parseFirstDouble ( &strMsg ); // skip pointto info, comes later
    Parse::parseFirstDouble ( &strMsg ); // skip pointto info, comes later
    WM->setNrOfCommands ( CMD_POINTTO    , Parse::parseFirstInt ( &strMsg ) );

    Parse::gotoFirstOccurenceOf ( 'b', &strMsg );  // go to ball position

    Log.log ( 4, "fullstate parse ball: %s", strMsg );
    dX    = Parse::parseFirstDouble ( &strMsg );   // parse first value
    dY    = Parse::parseFirstDouble ( &strMsg );   // parse second value
    dVelX = Parse::parseFirstDouble ( &strMsg );   // parse third value
    dVelY = Parse::parseFirstDouble ( &strMsg );   // parse fourth value
    if ( WM->isBeforeKickOff() )
        dX = dY = dVelX = dVelY = 0.0;
    if ( WM->getSide() == SIDE_RIGHT )
    {
        dX    *= -1;
        dY    *= -1;
        dVelX *= -1;
        dVelY *= -1;
    }
    WM->processSeeGlobalInfo ( OBJECT_BALL, time, VecPosition ( dX,dY ),
                               VecPosition ( dVelX,dVelY ), -1, -1 );
    strMsg++;
    Log.log ( 4, "fullstate ball: %f %f %f %f", dX, dY, dVelX, dVelY );

    while ( *strMsg != ')' )                         // " ((objname.." or ")"
    {
        dVelX = dVelY = UnknownDoubleValue;
        angBody = angNeck = UnknownAngleValue;
        strMsg += 2;          // go the start of the object name
        Log.log ( 4, "fullstate parse object: %s", strMsg );
        // get the object type at the current position in the string
        o = SoccerTypes::getObjectFromStr ( &strMsg, &isGoalie,
                                            ( WM->getSide() == SIDE_LEFT ) ? "l" : "r" );

        dX      = Parse::parseFirstDouble ( &strMsg );  // parse x position
        dY      = Parse::parseFirstDouble ( &strMsg );  // parse y position
        dVelX   = Parse::parseFirstDouble ( &strMsg );  // parse x velocity
        dVelY   = Parse::parseFirstDouble ( &strMsg );  // parse y velocity
        angBody = Parse::parseFirstDouble ( &strMsg );  // parse body angle
        angNeck = Parse::parseFirstDouble ( &strMsg );  // parse neck angle

        if ( WM->getSide() == SIDE_RIGHT )
        {
            dX    *= -1;
            dY    *= -1;
            dVelX *= -1;
            dVelY *= -1;
            angBody = VecPosition::normalizeAngle ( angBody + 180 );
        }

        double dStamina  = Parse::parseFirstDouble ( &strMsg ); // get stamina
        double dEffort   = Parse::parseFirstDouble ( &strMsg ); // get effort
        double dCapacity = Parse::parseFirstDouble ( &strMsg ); /// get Capacity /*! misol.gao fixed for v13.2 */
        Parse::parseFirstDouble ( &strMsg ); // skip recovery

        // skip ending bracket of stamina and then of object information.
        Parse::gotoFirstOccurenceOf ( ')', &strMsg );
        Parse::gotoFirstOccurenceOf ( ')', &strMsg );

        strMsg++;
        strMsg++;

        Log.log ( 1, "fullstate obj %d: %f %f %f %f %f %f", o, dX, dY, dVelX, dVelY,
                  angBody, angNeck );
        // process the parsed information
        if ( o == WM->getAgentObjectType() )
            WM->processNewAgentInfo ( vq, va, dStamina, dEffort, dCapacity, -1.0, -1.0, -angNeck,
                                      -1,iArmMovable, iArmExpires, VecPosition ( 0,0 ) );

        WM->processSeeGlobalInfo ( o, time, VecPosition ( dX,dY ),
                                   VecPosition ( dVelX,dVelY ), angBody, angNeck );

    }
    WM->setTimeLastSeeGlobalMessage ( time ); // set time last see global message
    WM->setTimeLastSenseMessage ( time );     // set time last see global message

    return true;
}

//!new add
bool bDecisionMade;
/*! This method analyzes a sense message. All information from the player is
    parsed and updated in the WorldModel.
    A sense message looks like (sense_body 0 (view_mode high normal)
    (stamina 2000 1) (speed 0 0) (head_angle 0) (kick 0) (dash 0)
    (turn 0) (say 0) (turn_neck 0) (catch 0) (move 0) (change_view 0))
    \param strMsg message that should be parsed
    \return bool indicating whether the message was parsed correctly. */
bool SenseHandler::analyzeSenseMessage ( char *strMsg )
{
    bDecisionMade = false;

    Log.log ( 999, "%s", strMsg );
    // cerr << strMsg << endl;
    // set the synchronization counter, this is a value [0..2] indicating the
    // section of the pattern this cycle is in. It gives an indication when new
    // visual information will arrive.

//     if ( SS->getSynchMode() == false )
//         setTimeSignal();                        // set signal when to send action
    strcpy ( WM->strLastSenseMessage, strMsg );

    if ( WM->getRelativeDistance ( OBJECT_BALL ) < SS->getVisibleDistance() )
        Log.logWithTime ( 560, "%s", WM->strLastSenseMessage );

    int iTime = Parse::parseFirstInt ( &strMsg );// get time
    Time timeOld = WM->getCurrentTime();
    Time timeNew = timeOld;
    timeNew.updateTime ( iTime );

    if ( timeNew.getTimeDifference ( timeOld ) > 1 )
        Log.log ( 1, "Missed a sense!!" );

    Log.logWithTime ( 2, "\n\nSENSE (%d, %d)", timeNew.getTime(),
                      timeNew.getTimeStopped() );
    Log.restartTimer( );
    iSimStep               = SS->getSimulatorStep() *1000;
    iTimeSignal            = ( int ) ( iSimStep*0.85 );
    Log.logWithTime ( 2, " alarm after %d", iTimeSignal );

    WM->setTimeLastSenseMessage ( timeNew ); // set the time

    if ( SS->getSynchMode() == false )  //!move and modify by zhaoyibin
        setTimeSignal();                        // set signal when to send action
    //!add by zhaoyibin
    //!begin
    gettimeofday ( &m_tvRcvSenseBody, NULL );
    //!end
//  Log.logWithTime( 2, " end analyzing sense" );
    return true;
}

/*! This method analyzes an init message. All information from the
    initialization is parsed and updated in the WorldModel.
    An init message looks like (init [l|r] 10 before_kick_off)
    \param strMsg message that should be parsed
    \return bool indicating whether the message was parsed correctly. */
bool SenseHandler::analyzeInitMessage ( char *strMsg )
{
    Log.log ( 999, "%s", strMsg );
    strMsg += 6;                                            // go to Side
    WM->setSide ( SoccerTypes::getSideFromStr ( strMsg ) ); // get and set Side
    int nr = Parse::parseFirstInt ( &strMsg );              // get and set number
    if ( nr == 0 )                                          // coach
    {
        WM->setPlayerNumber ( nr );
        cerr << strMsg  << endl;
        return true;
    }
    WM->setAgentObjectType ( SoccerTypes::getTeammateObjectFromIndex ( nr - 1 ) );
    WM->setPlayerNumber ( nr );
	//cout <<"Nomber:"<< nr << endl;
    strMsg++;                                               // skip space to pm
    WM->setPlayMode ( SoccerTypes::getPlayModeFromStr ( strMsg ) ); // get playmode
    return true;
}


extern  ActHandler  *ACT;
/*! This method analyzes a hear message. When the message is from the
    referee the message is parsed and the new play mode is set or the
    goal difference is adjusted. When the message comes from another
    player the method analyzePlayerMessage is called A hear message
    looks like (hear 0 self|referee|dir message)

    \param strMsg message that should be parsed
    \return bool indicating whether the message was parsed correctly. */
bool SenseHandler::analyzeHearMessage ( char *strMsg )
{
    RefereeMessageT rm;
    PlayModeT       pm;
    strcpy ( WM->strLastHearMessage, strMsg );

    int iTime = Parse::parseFirstInt ( &strMsg );             // ignore time
    Time time ( iTime );

    switch ( Parse::gotoFirstNonSpace ( &strMsg ) )
    {
    case 'r':                                               // referee
        Log.log ( 999, "%s", WM->strLastHearMessage );
        WM->setTimeLastRefereeMessage ( time );
        Parse::gotoFirstOccurenceOf ( ' ', &strMsg );         // go to start
        Parse::gotoFirstNonSpace ( &strMsg );                 // and first part
        rm = SoccerTypes::getRefereeMessageFromStr ( strMsg ); // get the ref msg
        Log.logWithTime ( 2, " referee message: %s %s",
                          SoccerTypes::getRefereeMessageStr ( rm ), WM->strLastHearMessage );
        pm = SoccerTypes::getPlayModeFromRefereeMessage ( rm );// get play mode
        if ( pm != PM_ILLEGAL )                               // from ref msg
            WM->setPlayMode ( pm );                             // if was pm, set

        switch ( rm )
        {
        case REFC_TIME_OVER:
            ACT->sendMessage ( "(bye)" );
            break;
        case REFC_GOAL_LEFT:                            // goal left
            if ( WM->getSide() == SIDE_LEFT )
                WM->addOneToGoalDiff();
            else
                WM->subtractOneFromGoalDiff();
            WM->processSeeGlobalInfo ( OBJECT_BALL, time, VecPosition ( 0, 0 ),
                                       VecPosition ( 0, 0 ), 0, 0 );
            break;
        case REFC_GOAL_RIGHT:                      // goal right
            if ( WM->getSide() == SIDE_RIGHT )
                WM->addOneToGoalDiff();
            else
                WM->subtractOneFromGoalDiff();
            WM->processSeeGlobalInfo ( OBJECT_BALL, time, VecPosition ( 0, 0 ),
                                       VecPosition ( 0, 0 ), 0, 0 );
            break;
        case REFC_GOALIE_CATCH_BALL_LEFT:         // catch ball
        case REFC_GOALIE_CATCH_BALL_RIGHT:
            WM->processCatchedBall ( rm, time );
            break;
        case REFC_PENALTY_ONFIELD_LEFT:
            WM->setSidePenalty ( SIDE_LEFT );
            break;
        case REFC_PENALTY_ONFIELD_RIGHT:
            WM->setSidePenalty ( SIDE_RIGHT );
            break;
        case REFC_PENALTY_MISS_LEFT:
        case REFC_PENALTY_SCORE_LEFT:
            WM->setPlayMode ( PM_FROZEN );
            break;
        case REFC_PENALTY_MISS_RIGHT:
        case REFC_PENALTY_SCORE_RIGHT:
            WM->setPlayMode ( PM_FROZEN );
            break;
        case REFC_PENALTY_FOUL_LEFT:
        case REFC_PENALTY_FOUL_RIGHT:
        default:
            break;
        }
        break;
    case 'o':                                               // online_coach_
        analyzeCoachMessage ( strMsg );
        break;
    case 's':                                               // self
        break;                                                // do nothing
    default:                                                // from direction
        Log.logWithTime ( 600, "incoming hear: %s", WM->strLastHearMessage );
        analyzePlayerMessage ( iTime, strMsg );   // from player
        break;
    }

    return true;
}

/*! This message analyzes an incoming communication message. Messages from
    opponents are discarded. First it is checked whether the message arrived
    from a teammate using a specific encoding string and then the contents
    are parsed and stored in the world model, which will process it when it
    updates the world model.
    \param iTime time from the message */
bool SenseHandler::analyzePlayerMessage ( int iTime, char *strMsg )
{
    Parse::gotoFirstNonSpace ( &strMsg );           // skip space

    if ( WM->getPlayerNumber() == 0 )               // if i am coach
        return false;                                 //   skip message
    if ( strlen ( strMsg ) < 2 || strMsg[0] == 'o' )  // skip message since no dir.
        return false;                                 // thus no content

    Parse::parseFirstInt ( &strMsg );               // skip direction
    Parse::gotoFirstNonSpace ( &strMsg );           // skip space
    if ( strlen ( strMsg ) < 2 || strMsg[1] == 'p' )  // skip message when from opp
        return false;

    int iPlayer = Parse::parseFirstInt ( &strMsg ); // get player number
    Parse::gotoFirstNonSpace ( &strMsg );           // skip space
    strMsg++;                                       // skip " (=quote)

    if ( strlen ( strMsg ) < 4 )            // < 2 + two ending charactres ")
    {
        Log.log ( 600, "communication string too small" );
        return false;
    }

    // get the cycle number from the encoding
    int iModCycle = ( int ) ( strMsg[0] - 'a' );

    if ( iModCycle < 0 || iModCycle > 9 )
    {
        Log.log ( 600, "communication cycle nr out of bounds: %d", iModCycle );
        return false;
    }

    // get the time difference between the current time and the send time
    int iDiff = ( iTime % 10 ) - iModCycle;
    if ( iDiff < 0 )
        iDiff += 10;

    // if it is too old; skip parsing; otherwise determine actual send time
    if ( iDiff > 2 )
    {
        Log.log ( 600, "communication string too old time %d mod %d diff %d",
                  iTime, iModCycle, iDiff );
        return false;
    }
    iTime -= iDiff;

    Log.log ( 600, "process comm msg, diff %d time %d, %s", iDiff, iTime, strMsg );
    WM->storePlayerMessage ( iPlayer, strMsg, iTime );
    return true;
}

///hear from Coach! mmm
bool SenseHandler::analyzeCoachMessage ( char *strMsg )
{
    Log.log ( 605, "received coach messages: %s" , strMsg );

    return true;
}
/*! This method analyzes the check_ball message that is only received by the
    coach. It sets the information in the Worldmodel what the status of the
    ball is.
    The format is as follows (check_ball <time> <status>).
    \param strMsg string that contains the check_ball message
    \return bool indicating whether update succeeded. */
bool SenseHandler::analyzeCheckBall ( char *strMsg )
{
    WM->setTimeCheckBall ( Parse::parseFirstInt ( &strMsg ) );
    strMsg++;
    WM->setCheckBallStatus ( SoccerTypes::getBallStatusFromStr ( strMsg ) );
    return true;
}

/*! This method analyzes the change player type message. This method checks
    whether the player that changed type equals the agent. When this is the
    case, it adjust the ServerSettings according to the values associated
    with this player type.
    \param strMsg string that contains the player type message.
    \return bool indicating whether player type of agent changed. */
bool SenseHandler::analyzeChangePlayerTypeMessage ( char *strMsg )
{
    Log.log ( 999, "%s", strMsg );
    int iPlayer = Parse::parseFirstInt ( &strMsg );
    if ( *strMsg != ')' ) // we are dealing with player of own team
    {
        int      iType = Parse::parseFirstInt ( &strMsg );
        ObjectT  obj   = SoccerTypes::getTeammateObjectFromIndex ( iPlayer - 1 );
        Log.log ( 605, "change player from message %d -> %d", obj, iType );
        WM->setHeteroPlayerType ( obj, iType );
        Log.log ( 605, "changed player from message %d -> %d", obj,
                  WM->getHeteroPlayerType ( obj ) );
        return true;
    }
    else
    {
        ObjectT  obj   = SoccerTypes::getOpponentObjectFromIndex ( iPlayer - 1 );
        return WM->setSubstitutedOpp ( obj );
    }


    return false;
}

/*! This method analyzes the server_param message. This message contains all
    the server parameters. All settings of the ServerSettings are changed
    according to the supplied values. This makes the reading from a server
    configuration file obsolete.
    \param strMsg string message with all the server parameters
    \return booli indicating whether string was parsed. */
bool SenseHandler::analyzeServerParamMessage ( char *strMsg )
{

    Log.log ( 4, "%s", strMsg );

    readServerParam ( "goal_width",               strMsg );
    readServerParam ( "player_size",              strMsg );
    readServerParam ( "player_decay",             strMsg );
    readServerParam ( "player_rand",              strMsg );
    readServerParam ( "player_weight",            strMsg );
    readServerParam ( "player_speed_max",         strMsg );
    readServerParam ( "player_accel_max",         strMsg );
    readServerParam ( "stamina_max",              strMsg );
    readServerParam ( "stamina_inc_max",          strMsg );
    readServerParam ( "recover_dec_thr",          strMsg );
    readServerParam ( "recover_min",              strMsg );
    readServerParam ( "recover_dec",              strMsg );
    readServerParam ( "effort_dec_thr",           strMsg );
    readServerParam ( "effort_min",               strMsg );
    readServerParam ( "effort_dec",               strMsg );
    readServerParam ( "effort_inc_thr",           strMsg );
    readServerParam ( "effort_inc",               strMsg );
    readServerParam ( "kick_rand",                strMsg );
    readServerParam ( "ball_size",                strMsg );
    readServerParam ( "ball_decay",               strMsg );
    readServerParam ( "ball_rand",                strMsg );
    readServerParam ( "ball_weight",              strMsg );
    readServerParam ( "ball_speed_max",           strMsg );
    readServerParam ( "ball_accel_max",           strMsg );
    readServerParam ( "dash_power_rate",          strMsg );
    readServerParam ( "kick_power_rate",          strMsg );
    readServerParam ( "kickable_margin",          strMsg );
    readServerParam ( "catch_probability",        strMsg );
    readServerParam ( "catchable_area_l",         strMsg );
    readServerParam ( "catchable_area_w",         strMsg );
    readServerParam ( "goalie_max_moves",         strMsg );
    readServerParam ( "maxpower",                 strMsg );
    readServerParam ( "minpower",                 strMsg );
    readServerParam ( "maxmoment",                strMsg );
    readServerParam ( "minmoment",                strMsg );
    readServerParam ( "maxneckmoment",            strMsg );
    readServerParam ( "minneckmoment",            strMsg );
    readServerParam ( "maxneckang",               strMsg );
    readServerParam ( "minneckang",               strMsg );
    readServerParam ( "visible_angle",            strMsg );
    readServerParam ( "visible_distance",         strMsg );
    readServerParam ( "audio_cut_dist",           strMsg );
    readServerParam ( "quantize_step",            strMsg );
    readServerParam ( "quantize_step_l",          strMsg );
    readServerParam ( "ckick_margin",             strMsg );
    readServerParam ( "wind_dir",                 strMsg );
    readServerParam ( "wind_force",               strMsg );
    readServerParam ( "wind_rand",                strMsg );
    readServerParam ( "wind_random",              strMsg );
    readServerParam ( "inertia_moment",           strMsg );
    readServerParam ( "half_time",                strMsg );
    readServerParam ( "drop_ball_time",           strMsg );
    readServerParam ( "port",                     strMsg );
    readServerParam ( "coach_port",               strMsg );
    readServerParam ( "olcoach_port",             strMsg );
    readServerParam ( "say_coach_cnt_max",        strMsg );
    readServerParam ( "say_coach_msg_size",       strMsg );
    readServerParam ( "simulator_step",           strMsg );
    readServerParam ( "send_step",                strMsg );
    readServerParam ( "recv_step",                strMsg );
    readServerParam ( "sense_body_step",          strMsg );
    readServerParam ( "say_msg_size",             strMsg );
    readServerParam ( "clang_win_size",           strMsg );
    readServerParam ( "clang_define_win",         strMsg );
    readServerParam ( "clang_meta_win",           strMsg );
    readServerParam ( "clang_advice_win",         strMsg );
    readServerParam ( "clang_info_win",           strMsg );
    readServerParam ( "clang_mess_delay",         strMsg );
    readServerParam ( "clang_mess_per_cycle",     strMsg );
    readServerParam ( "hear_max",                 strMsg );
    readServerParam ( "hear_inc",                 strMsg );
    readServerParam ( "hear_decay",               strMsg );
    readServerParam ( "catch_ban_cycle",          strMsg );
    readServerParam ( "send_vi_step",             strMsg );
    readServerParam ( "use_offside",              strMsg );
    readServerParam ( "offside_active_area_size", strMsg );
    readServerParam ( "forbid_kick_off_offside",  strMsg );
    readServerParam ( "verbose",                  strMsg );
    readServerParam ( "offside_kick_margin",      strMsg );
    readServerParam ( "slow_down_factor",         strMsg );
    readServerParam ( "synch_mode",               strMsg );
    readServerParam ( "fullstate_l",              strMsg );
    readServerParam ( "fullstate_r",              strMsg );
    readServerParam ( "pen_dist_x",               strMsg );
    readServerParam ( "pen_max_goalie_dist_x",    strMsg );
    readServerParam ( "pen_allow_mult_kicks",     strMsg );
    readServerParam ( "tackle_dist",              strMsg );
    readServerParam ( "tackle_back_dist",         strMsg );
    readServerParam ( "tackle_width",             strMsg );
    readServerParam ( "tackle_cycles",            strMsg );
    readServerParam ( "tackle_power_rate",        strMsg );
    readServerParam ( "tackle_exponent",          strMsg );

    SS->setMaximalKickDist ( SS->getKickableMargin() +
                             SS->getPlayerSize()     +
                             SS->getBallSize() );
// SS->show( cerr, ":" );
    return true;
}

bool SenseHandler::readServerParam ( const char *strParam, char *strMsg )
{
    char strFormat[128];
    char strValue[128] = "";
    sprintf ( strValue, "none" );

    sprintf ( strFormat, "%s ", strParam );       // add space after parameters
    char *str = strstr ( strMsg, strFormat );     // and find param definition
    sprintf ( strFormat, "%s %%[^)]", strParam ); // read till closing bracket

    if ( str == NULL )
    {
        cerr << "(SenseHandler::readServerParam) " << WM->getPlayerNumber() <<
             "  error finding " << strParam <<endl;
        return false;
    }
    int ret = sscanf ( str, strFormat, strValue ); // read in values

    if ( ret == 1 )
        SS->setValue ( strParam, strValue );
    else
        cerr << "(SenseHandler::readServerParam) error reading " <<strParam <<endl;
    return ( ret == 1 ) ? true : false ;
}


/*! This method analyze a player type message. This message contains the
    values associated with a specific heterogeneous player type. The values
    are parsed from the message and supplied to the WorldModel method
    processNewHeteroPlayer.
    \param strMsg string that contains the player type information
    \return bool indicating whether the message was parsed correctly. */
bool SenseHandler::analyzePlayerTypeMessage ( char *strMsg )
{
    Log.log ( 999, "%s", strMsg );
// cerr << strMsg << endl;

    // analyze all heterogeneous player information
    int    iIndex           = Parse::parseFirstInt ( &strMsg );
    double dPlayerSpeedMax  = Parse::parseFirstDouble ( &strMsg );
    double dStaminaIncMax   = Parse::parseFirstDouble ( &strMsg );
    double dPlayerDecay     = Parse::parseFirstDouble ( &strMsg );
    double dInertiaMoment   = Parse::parseFirstDouble ( &strMsg );
    double dDashPowerRate   = Parse::parseFirstDouble ( &strMsg );
    double dPlayerSize      = Parse::parseFirstDouble ( &strMsg );
    double dKickableMargin  = Parse::parseFirstDouble ( &strMsg );
    double dKickRand        = Parse::parseFirstDouble ( &strMsg );
    double dExtraStamina    = Parse::parseFirstDouble ( &strMsg );
    double dEffortMax       = Parse::parseFirstDouble ( &strMsg );
    double dEffortMin       = Parse::parseFirstDouble ( &strMsg );

    WM->processNewHeteroPlayer ( iIndex, dPlayerSpeedMax, dStaminaIncMax,
                                 dPlayerDecay,    dInertiaMoment, dDashPowerRate, dPlayerSize,
                                 dKickableMargin, dKickRand,      dExtraStamina,  dEffortMax,
                                 dEffortMin );
    return true;
}

/*! This method analyzes the player_param message that indicates the ranges of
    the possible values for the heterogeneous player types. Nothing is done
    with this information.
    \param strMsg string that contains the player_param message.
    \bool will always be true. */
bool SenseHandler::analyzePlayerParamMessage ( char *strMsg )
{
//  cout << strMsg << endl;
    Log.log ( 999, "%s", strMsg );
    readServerParam ( "player_types",                     strMsg );
    readServerParam ( "subs_max",                         strMsg );
    readServerParam ( "player_speed_max_delta_min",       strMsg );
    readServerParam ( "player_speed_max_delta_max",       strMsg );
    readServerParam ( "stamina_inc_max_delta_factor",     strMsg );
    readServerParam ( "player_decay_delta_min",           strMsg );
    readServerParam ( "player_decay_delta_max",           strMsg );
    readServerParam ( "inertia_moment_delta_factor",      strMsg );
    readServerParam ( "dash_power_rate_delta_min",        strMsg );
    readServerParam ( "dash_power_rate_delta_max",        strMsg );
    readServerParam ( "player_size_delta_factor",         strMsg );
    readServerParam ( "kickable_margin_delta_min",        strMsg );
    readServerParam ( "kickable_margin_delta_max",        strMsg );
    readServerParam ( "kick_rand_delta_factor",           strMsg );
    readServerParam ( "extra_stamina_delta_min",          strMsg );
    readServerParam ( "extra_stamina_delta_max",          strMsg );
    readServerParam ( "effort_max_delta_factor",          strMsg );
    readServerParam ( "effort_min_delta_factor",          strMsg );
    readServerParam ( "new_dash_power_rate_delta_min",    strMsg );
    readServerParam ( "new_dash_power_rate_delta_max",    strMsg );
    readServerParam ( "new_stamina_inc_max_delta_factor", strMsg );

    return true;
}

/*****************************************************************************/
/********************* TESTING PURPOSES **************************************/
/*****************************************************************************/

/*
int main( void )
{
  Connection c( "localhost", 6000 );
  WorldModel wm;
  SenseHandler i( &c,  &wm );
  i.analyzeMessage( "(see 0 ((g r) 64.1 13) ((f r t) 65.4 -16) ((f r b) 79 38) ((f p r t) 46.1 -6) ((f p r c) 48.4 18) ((f p r b) 58 37) ((f g r t) 62.8 7) ((f g r b) 66 19) ((f t r 20) 38.5 -38) ((f t r 30) 46.5 -30) ((f t r 40) 55.7 -25) ((f t r 50) 64.7 -21) ((f b r 50) 80.6 41) ((f r t 30) 69.4 -12) ((f r t 20) 67.4 -4) ((f r t 10) 67.4 4) ((f r 0) 69.4 12) ((f r b 10) 72.2 20) ((f r b 20) 75.9 27) ((f r b 30) 81.5 33) ((l r) 62.8 -89))" );
   cout << "2" << endl;
  i.analyzeMessage( "(see 0 ((g l) 49.9 -24) ((f l t) 50.9 14) ((f p l t) 31.5 1 0 0) ((f p l c) 34.5 -33) ((f g l t) 47.9 -17) ((f g l b) 52.5 -32) ((f t l 50) 50.9 20) ((f t l 40) 42.5 26) ((f t l 30) 34.8 36) ((f l t 30) 54.6 8) ((f l t 20) 53 -2) ((f l t 10) 53 -12) ((f l 0) 54.6 -23) ((f l b 10) 58 -32) ((f l b 20) 62.8 -41) ((p \"l\" 2) 5 -7 0 0 172 172) ((l l) 47.9 82))" );
  c.disconnect();
  cout << "exit" << endl ;

}

*/

