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

/*! \file WorldModel.cpp
<pre>
<b>File:</b>          WorldModel.cpp
<b>Project:</b>       Robocup Soccer Simulation Team: UvA Trilearn
<b>Authors:</b>       Jelle Kok
<b>Created:</b>       12/02/2001
<b>Last Revision:</b> $ID$
<b>Contents:</b>      class definitions of WorldModel. This class contains
               the methods of the WorldModel that process the incoming
               information and analyze the current state of the world.
<hr size=2>
<h2><b>Changes</b></h2>
<b>Date</b>             <b>Author</b>          <b>Comment</b>
12/02/2001       Jelle Kok       Initial version created
</pre>
*/

#include<stdio.h>    // needed for printf
#include<errno.h>    // needed for ETIMEDOUT
#ifdef WIN32
#include<windows.h>
#else
#include<strings.h>  // needed for strcpy
#include<pthread.h>  // needed for pthread_mutex_init
#endif
#include<string.h>   // needed for strcpy
#include<math.h>     // needed for erf
#include<map>        // needed for map
#include"WorldModel.h"

/*****************************************************************************/
/********************** CLASS WORLDMODEL *************************************/
/*****************************************************************************/

/*! This constructor creates the worldmodel, all variables are initialized by
    default values
    \param ss reference to class in which all server parameters are stored
    \param ps reference to class in which all client parameters are stored
    \param fs reference to class in which all formation information is stored*/
WorldModel::WorldModel( ServerSettings *ss, PlayerSettings *ps,
                        Formations *fs):agentObject(  )
{
    dTotalVarVel   = 0.0;
    dTotalVarPos   = 0.0;
    SS             = ss;
    PS             = ps;
    formations     = fs;
    bNewInfo       = false;

    //!new add
    bSynchSeeMode = false;
    setSide         ( SIDE_ILLEGAL ); // is set by init message
    strTeamName[0] = '\0';
    setPlayMode     ( PM_BEFORE_KICK_OFF );
    iGoalDiff      = 0;
    m_sidePenalty = SIDE_ILLEGAL;

    int i;
    for ( i = 0; i < MAX_TEAMMATES ; i ++ )
        Teammates[i].setType( SoccerTypes::getTeammateObjectFromIndex( i ) );
    for ( i = 0; i < MAX_OPPONENTS ; i ++ )
        Opponents[i].setType( SoccerTypes::getOpponentObjectFromIndex( i ) );
    for ( i = 0; i < MAX_OPPONENTS + MAX_TEAMMATES ; i ++ )
        UnknownPlayers[i].setType( OBJECT_ILLEGAL );
    for ( i = 0; i < MAX_FLAGS; i ++ )
        Flags[i].setType( OBJECT_ILLEGAL );
    for ( i = 0; i < MAX_LINES; i ++ )
        Lines[i].setType( OBJECT_ILLEGAL );

    iNrUnknownPlayers     = 0;

    Ball.setType             ( OBJECT_BALL    );
    agentObject.setType      ( OBJECT_ILLEGAL );
    agentObject.setStamina   ( Stamina(SS->getStaminaMax(),1.0,1.0) );

    for ( i = 0 ; i < CMD_MAX_COMMANDS ; i ++ )
    {
        queuedCommands[i].commandType = (CommandT)i;
        performedCommands[i]          = false;
        iCommandCounters[i]           = 0;
    }

    iNrHoles              = 0;
    iNrOpponentsSeen      = 0;
    iNrTeammatesSeen      = 0;
    bsCheckBall           = BS_ILLEGAL;

    // initialize the mutex for bNewInfo
#ifdef WIN32
    InitializeCriticalSection( &mutex_newInfo );
    event_newInfo = CreateEvent( NULL, TRUE, FALSE, NULL );
#else
    pthread_mutex_init( &mutex_newInfo, NULL );
    pthread_cond_init ( &cond_newInfo,  NULL );
#endif
    m_bRecvThink          = false;
    timeLastSenseMessage  = Time( 0, 1 );

    strcpy( m_colorPlayers[0], "ffb0b0" );   // pink
    strcpy( m_colorPlayers[1], "ffb000" );   // orange
    strcpy( m_colorPlayers[2], "00b000" );   // darkgreen
    strcpy( m_colorPlayers[3], "00ffff" );   // lightblue
    strcpy( m_colorPlayers[4], "ff00ff" );   // purple
    strcpy( m_colorPlayers[5], "ffff00" );   // yellow
    strcpy( m_colorPlayers[6], "ffffff" );   // white
    strcpy( m_colorPlayers[7], "0000ff" );   // blue
    strcpy( m_colorPlayers[8], "00ff00" );   // green
    strcpy( m_colorPlayers[9], "ff0000" );   // red
    strcpy( m_colorPlayers[10],"aaaaaa" );   // gray

    for ( i = 0 ; i < MAX_FEATURES ; i ++ )
    {
        m_features[i].setTimeSee( Time( UnknownTime, 0 ) );
        m_features[i].setTimeSense( Time( UnknownTime, 0 ) );
    }
}

/*! Destructor */
WorldModel::~WorldModel()
{
#ifdef WIN32
    DeleteCriticalSection( &mutex_newInfo );
#endif
}

/*! This method returns a pointer to the Object information of the object
    type that is passed as the first argument.
    \param o ObjectType of which information should be returned
    \return pointer to object information of supplied ObjectT argument */
Object* WorldModel::getObjectPtrFromType( ObjectT o )
{
    Object *object = NULL;
    if ( o == OBJECT_ILLEGAL )
        return NULL;

    if ( SoccerTypes::isKnownPlayer( o ) )
    {
        if ( o == agentObject.getType() )
            object = &agentObject;
        else if ( SoccerTypes::isTeammate( o ) )
            object = &Teammates[SoccerTypes::getIndex(o)];
        else
            object = &Opponents[SoccerTypes::getIndex(o)];
    }
    else if ( SoccerTypes::isFlag( o ) )
        object = &Flags[SoccerTypes::getIndex(o)];
    else if ( SoccerTypes::isLine( o ) )
        object = &Lines[SoccerTypes::getIndex(o)];
    else if ( SoccerTypes::isBall( o ) )
        object = &Ball;
    else if ( o == OBJECT_OPPONENT_GOALIE )
        return getObjectPtrFromType( getOppGoalieType() );
    else if ( o == OBJECT_TEAMMATE_GOALIE )
        return getObjectPtrFromType( getOwnGoalieType() );
    return object;
}

/*! This method sets the time of the last catch cycle. This information is
    received by the SenseHandler when the referee has sent this message.
    After a catch, the goalie is not allowed to catch the ball for
    catch_ban_cycles (defined in ServerSettings).
    \param iTime time the ball was catched. */
void WorldModel::setTimeLastCatch( Time time )
{
    timeLastCatch = time;
}

/*! This method returns the number of cycles since the last catch.
    \return cycles since last catch. */
int WorldModel::getTimeSinceLastCatch()
{
    if ( timeLastCatch.getTime() == -1 )
        return 1000;
    return timeLastSenseMessage - timeLastCatch;
}

/*! This method sets the time of the last received referee message. This
    information is received by the SenseHandler.
    \param iTime time the referee sent the last message. */
bool WorldModel::setTimeLastRefereeMessage( Time time )
{
    timeLastRefMessage = time;
    return true;
}

/*! This method returns the time of the last received referee message.
    \return time of last received referee message. */
Time WorldModel::getTimeLastRefereeMessage( )
{
    return timeLastRefMessage;
}

/*! This method returns the current time. In case of a player this is the
    time of the last sense message, in case of the coach this is the time of
    the last see_global message.
    \return actual time */
Time WorldModel::getCurrentTime()
{
    if ( getPlayerNumber() == 0 )
        return getTimeLastSeeGlobalMessage();
    else
        return getTimeLastRecvSenseMessage();
}

/*! This method returns the current cycle number. In case of a player this is
    the cycle of the last sense message, in case of the coach this is the cycle
    of the last see_global message.
    \return actual time */
int WorldModel::getCurrentCycle()
{
    return getCurrentTime().getTime();
}


/*! This method returns whether the time of the server stands
    still. This occurs during non play-on modes (kick_in, kick_off,
    etc.).

    \return bool indicating whether time of the server stands still. */
bool WorldModel::isTimeStopped()
{
    return getCurrentTime().isStopped();
}

/*! This method returns whether the last received message was a see or not.
    \return bool indicating whether the last received message was a see.*/
bool WorldModel::isLastMessageSee() const
{
    return getTimeLastSeeMessage() == getTimeLastSenseMessage() ;
}

/*! This method returns the time of the last see global message.
    This message can only be received by the coach.
    \return time of last see_global message */
Time WorldModel::getTimeLastSeeGlobalMessage( ) const
{
    return getTimeLastRecvSeeMessage();
}

/*! This method sets the time of the last see_global message.
    \param time see message has arrived
    \return true when update was succesful */
bool WorldModel::setTimeLastSeeGlobalMessage( Time time )
{
	//cout << " A " << getCurrentTime() << getCurrentCycle() << endl;
    updateRelativeFromGlobal( );
	//cout << getPlayerNumber() << " " << getCurrentCycle() << endl;
	bNewInfo = true;/*!miso.gao fixed!这个地方必须设置，因为同步视觉下，其他地方对这个值都没有设置true*/
    return setTimeLastSeeMessage( time ); // set see message
}

/*! This method returns the time of the last see message
    \return time of last see message */
Time WorldModel::getTimeLastSeeMessage( ) const
{
    return timeLastSeeMessage;
}

/*! This method returns the time of the last received see message.
    The difference with getTimeLastSeeMessage is that that method returns
    the last see message that has been updated in the world model. In most
    cases these are equal.
    \return time of last received sense message */
Time WorldModel::getTimeLastRecvSeeMessage( ) const
{
    return timeLastRecvSeeMessage ;
}
/*! This method sets the time of the last see message. It also sends a
    condition signal to indicate that variable bNewInfo has
    changed. When main thread is stopped in waitForNewInformation, it
    is unblocked.

    \param time see message has arrived
    \return true when update was succesful */
bool WorldModel::setTimeLastSeeMessage( Time time )
{
    timeLastRecvSeeMessage  = time;
    if ( SS->getSynchMode() == false )
    {
#ifdef WIN32
        //EnterCriticalSection( &mutex_newInfo );
        bNewInfo            = true;
        SetEvent            (  event_newInfo );
        //LeaveCriticalSection( &mutex_newInfo );
#else
        pthread_mutex_lock  ( &mutex_newInfo );
        bNewInfo            = true;
        pthread_cond_signal ( &cond_newInfo );
        pthread_mutex_unlock( &mutex_newInfo );
#endif
    }

    return true;
}

/*! This method returns the time of the last sense message
    \return time of last sense message */
Time WorldModel::getTimeLastSenseMessage( ) const
{
    return timeLastSenseMessage ;
}

/*! This method returns the time of the last received sense message.
    The difference with getTimeLastSenseMessage is that that method returns
    the last sense message that has been updated in the world model. In most
    cases these are equal.
    \return time of last received sense message */
Time WorldModel::getTimeLastRecvSenseMessage( ) const
{
    return timeLastRecvSenseMessage ;
}


/*! This method sets the time of the last sense message. It also send a
    condition signal to indicate that variable bNewInfo has changed.
    When main thread is stopped in waitForNewInformation, it is
    unblocked.
    \param time sense message has arrived
    \return true when update was succesful */
bool WorldModel::setTimeLastSenseMessage( Time time )
{
    timeLastRecvSenseMessage = time;
    if ( SS->getSynchMode() == false )
    {
#ifdef WIN32
        //EnterCriticalSection( &mutex_newInfo );
        bNewInfo = true;
        SetEvent            (  event_newInfo );
        //LeaveCriticalSection( &mutex_newInfo );
#else
        pthread_mutex_lock  ( &mutex_newInfo );
        bNewInfo = true;
        pthread_cond_signal ( &cond_newInfo );
        pthread_mutex_unlock( &mutex_newInfo );
#endif
    }

    return true;
}

/*! This method returns the time of the last hear message
    \return time of last hear message */
Time WorldModel::getTimeLastHearMessage( ) const
{
    return timeLastHearMessage ;
}

/*! This method sets the time of the last hear message.
    \param time hear message has arrived
    \return true when update was succesful */
bool WorldModel::setTimeLastHearMessage( Time time )
{
    timeLastHearMessage = time;
    return true;
}

/*! This method returns the player number of the agent. The player number is
    the fixed number which is given by the server after initialization.
    \return player number of this player. */
int WorldModel::getPlayerNumber( ) const
{
    return iPlayerNumber;
}

/*! This method sets the player number of the agent. This value is available in
    the conformation message sent by the soccerserver after the initialization.
    \param i new player number of the agent.
    \return bool indicating whether player number was set. */
bool WorldModel::setPlayerNumber( int i )
{
    iPlayerNumber = i;
    return true;
}

/*! This method returns the side of the agent. Note that
    the side of the agent does not change after half time.
    \return side (SIDE_LEFT or SIDE_RIGHT) for agent */
SideT WorldModel::getSide( ) const
{
    return sideSide;
}

/*! This method sets the side of the agent
    \param s (SIDE_LEFT or SIDE_RIGHT) for agent
    \return bool indicating whether update was succesful.*/
bool WorldModel::setSide( SideT s )
{
    sideSide = s;
    m_iMultX = (getSide() == SIDE_LEFT ) ?  1 : -1 ;    // set the draw info
    m_iMultY = (getSide() == SIDE_LEFT ) ? -1 :  1 ;    // from Logger.C
    return true;
}

/*! This method returns the teamname of the agent in this worldmodel
    \return teamname for the agent in this worldmodel */
const char* WorldModel::getTeamName( ) const
{
    return strTeamName ;
}

/*! This method sets the teamname of the agent. The maximum team name is
    MAX_TEAM_NAME_LENGTH as defined in Soccertypes.h.
    \param str teamname for the agent in this worldmodel
    \return bool indicating whether update was succesful.*/
bool WorldModel::setTeamName( char * str )
{
    strcpy( strTeamName, str );
    return true;
}

/*! This method returns the current playmode. This playmode is passed
    through by the referee.

    \return current playmode (see SoccerTypes for possibilities) */
PlayModeT WorldModel::getPlayMode( ) const
{
    return playMode ;
}

/*! This method sets the play mode of the agent in this worldmodel
    \param pm for the agent in this worldmodel
    \return bool indicating whether update was succesful.*/
bool WorldModel::setPlayMode( PlayModeT pm )
{
    playMode = pm;
    if ( ( pm == PM_GOAL_KICK_LEFT  && getSide() == SIDE_LEFT  ) ||
            ( pm == PM_GOAL_KICK_RIGHT && getSide() == SIDE_RIGHT )    )
        setTimeLastCatch( getTimeLastSenseMessage() );
    return true;
}

/*! This method returns the goal difference. When this value is below zero,
    the team of agent is behind, 0 means that the score is currently the same
    for both teams and a value higher than zero means that you're winning!.
    \return goal difference */
int WorldModel::getGoalDiff( ) const
{
    return iGoalDiff;
}

/*!This method adds one goal to the goal difference. Call this method when your
   team has scored a goal
   \return new goal difference */
int WorldModel::addOneToGoalDiff( )
{
    return ++iGoalDiff;
}

/*!This method subtracts one from the goal difference. Call this method when
   you're team has conceided a goal
   \return new goal difference */
int WorldModel::subtractOneFromGoalDiff()
{
    return --iGoalDiff;
}

/*! This method returns the amount of commands c performed by the agent.
    This is supplied in the sense_body message.
    \param c CommandT of which number of commands should be returned.
    \return amount of commands c performed by the soccerserver. */
int WorldModel::getNrOfCommands( CommandT c ) const
{
    return iCommandCounters[ (int) c ];
}

/*! This method sets the number of commands c that were performed by the agent.
    This is supplied in the sense_body message and can be used to check
    whether an action is actually performed by the soccer server, since the
    corresponding counter should be one higher than the previous
    sense_body message. When this is the case the corresponding index of
    the PerformedCommands array is set to true.
    \param c CommandT of which the number of commands should be set.
    \param i number of commands that are performed of this command
    \return bool indicating whether update was performed. */
bool WorldModel::setNrOfCommands( CommandT c, int i )
{
    int iIndex = (int) c;

    // if counter is the same as before, no command is performed, otherwise it is
    performedCommands[iIndex] = ( iCommandCounters[iIndex] == i ) ? false : true;
    iCommandCounters [iIndex] = i;
    return true;
}

/*! This method returns the time the status of the ball was last checked
    (coach only).
    \return server cycle the status of the ball was last checked. */
Time WorldModel::getTimeCheckBall( ) const
{
    return timeCheckBall;
}

/*! This method sets the time the ball was checked for the last time (coach
    only).
    \param time server time ball was checked.
    \return bool indicating whether update was succesfull. */
bool WorldModel::setTimeCheckBall( Time time )
{
    timeCheckBall = time;
    return true;
}

/*! This method returns the status of the ball. This value is derived from
    the check_ball command that can only be used by the coach. The status of
    the ball corresponds to the server time returned by getTimeCheckBall.
    \return BallStatus status of the ball. */
BallStatusT WorldModel::getCheckBallStatus( ) const
{
    return bsCheckBall;
}

/*! This method sets the status of the ball. The status of the ball
    corresponds to the server time returned by getTimeCheckBall.  This method
    is only useful for the coach, since only he can sent a check_ball message.
    \return BallStatus status of the ball.  */
bool WorldModel::setCheckBallStatus( BallStatusT bs )
{
    bsCheckBall = bs;
    return true;
}

/*! This method returns a boolean indicating whether the synchronization method
    indicated we are ready. */
bool WorldModel::getRecvThink( )
{
    return m_bRecvThink;
}

/*! This method returns the string that we want communicate. It can be set
    during the creation of the action. When the ActHandler sends its actions,
    it sends this string to the server.
    \return communication string. */
char* WorldModel::getCommunicationString( )
{
    return m_strCommunicate;
}


/*! This methods returns the object type of the object that is currently
    listened to. This information is gathered from a sense message.
    When this agent says a message, we will definitely hear it. */
ObjectT WorldModel::getObjectFocus( )
{
    return m_objFocus;
}


/*! This methods sets the object type of the object that is currently listened
    to. This information is gathered from a sense message. When this agent says
    a message, we will definitely hear it. */
bool WorldModel::setObjectFocus( ObjectT obj )
{
    m_objFocus = obj;
    return true;
}


/*! This method sets the string that we want communicate. It can be set
    during the creation of the action. When the ActHandler sends its actions,
    it sends this string to the server.
    \return communication string. */
bool WorldModel::setCommunicationString( char *str )
{
    strncpy( m_strCommunicate, str, MAX_SAY_MSG );
    return true;
}


/*!This method starts an iteration over an object set g. This method will
   return the first ObjectT in an ObjectSetT that has a confidence higher than
   dConf. After this call use the method iterateObjectNext with the same
   argument to get the other objects that belong to this set.
   \param iIndex index that should be used in consecutive cycles.
   \param g ObjectSetT of which the ObjecT should be returned.
   \param dConf minimum confidence needed for ObjectT to be returned.
   \return ObjectT that is first in the set g. */
ObjectT WorldModel::iterateObjectStart(int& iIndex,ObjectSetT g,double dConf,
                                       bool bForward)
{
    iIndex = -1;
    return iterateObjectNext( iIndex, g, dConf, bForward );
}

/*!This method gets the next object in the iteration over an object set g.
   iterateObjectStart should have been called first. Only ObjectT with a
   confidence higher than dConf will be returned. When no more objects are
   available OBJECT_ILLEGAL is returned.
   \param iIndex same argument as was supplied to iterateObjectStart.
   \param g ObjectSetT of which the ObjecT should be returned.
   \param dConf minimum confidence needed for ObjectT to be returned.
   \return ObjectT that is next in the set g. */
ObjectT WorldModel::iterateObjectNext(int& iIndex,ObjectSetT g, double dConf,
                                      bool bForward)
{
    ObjectT o, objGoalie = OBJECT_TEAMMATE_1;
    bool bContinue = true;

    if ( g == OBJECT_SET_TEAMMATES_NO_GOALIE )
        objGoalie = getOwnGoalieType();

    if ( iIndex < 0 )
        iIndex = (bForward==false) ? OBJECT_MAX_OBJECTS : -1 ;

    // when dConf is not specified it has the default value of -1.0, in this
    // case set it to the confidence threshold defined in PlayerSetting, but
    // only do this for dynamic objexts, not for flags, lines, etc. since all
    // should be returend.
    if ( dConf == -1.0 && (g==OBJECT_SET_OPPONENTS || g==OBJECT_SET_PLAYERS ||
                           g==OBJECT_SET_TEAMMATES) )
        dConf = PS->getPlayerConfThr();

    int i = 0;
    if ( bForward == true )
        i = iIndex + 1;
    else
        i = iIndex - 1;
    bContinue = ( bForward == false ) ? ( i >= 0 ) : ( i < OBJECT_MAX_OBJECTS );

    while ( bContinue )
    {
        o = (ObjectT) i;
        if ( SoccerTypes::isInSet( o, g, objGoalie ) )
        {
            if ( getConfidence( o ) >= dConf )
            {
                iIndex = i;
                return o;
            }
            else if ( dConf == 1.0 && getTimeLastSeeMessage() == getTimeLastSeen( o ))
            {
                iIndex = i; // confidence of 1.0 can only be in same cycle as see
                return o;   // message. Therefore first test should succeed normally;
            }             // in cases where this method is called after see message,
            // but new sense has already arrived, confidence is lowered
        }               // but we want to return object that was seen in last see
        // message; this compensates for those cases.
        if ( bForward == true )
            i++;
        else
            i--;
        bContinue = ( bForward == false ) ? ( i >= 0 ) :( i < OBJECT_MAX_OBJECTS);
    }

    return OBJECT_ILLEGAL;
}

/*!This method finsished the iteration. It should be called after
   iterateObjectNext has returned OBJECT_ILLEGAL indicating no more objects are
   available that satisfy the constraints.
   \param iIndex index of iteration */
void WorldModel::iterateObjectDone( int &iIndex )
{
    iIndex = -1;
}

/*! This method returns the ObjectType of the agent. This
    is an ObjectT between OBJECT_TEAMMATE_1 and OBJECT_TEAMMATE_11
    \return ObjectT that represent the type of the agent  */
ObjectT WorldModel::getAgentObjectType( ) const
{
    return agentObject.getType();
}

int WorldModel::getAgentIndex( ) const
{
    return SoccerTypes::getIndex( getAgentObjectType() );
}

/*! This method sets the ObjectType of the agent. This
    is an objectT between OBJECT_TEAMMATE_1 and OBJECT_TEAMMATE_11
    \param ObjectT that represent the type of the agent
    \return bool indicating whether the update was succesful */
bool WorldModel::setAgentObjectType( ObjectT o )
{
    agentObject.setType( o );
    return true;
}

/*! This method returns the body angle relative to the neck of the agent.
    \return AngDeg representing the body angle relative to the neck */
AngDeg WorldModel::getAgentBodyAngleRelToNeck( ) const
{
    return agentObject.getBodyAngleRelToNeck();
}

/*! This method returns the global neck angle of the agent in the world.
    \return global neck angle agent */
AngDeg WorldModel::getAgentGlobalNeckAngle(  ) const
{
    return agentObject.getGlobalNeckAngle(  );
}

/*! This method returns the global body angle of the agent in the world.
    \return global body angle agent */
AngDeg WorldModel::getAgentGlobalBodyAngle(  )
{
    return agentObject.getGlobalBodyAngle(  );
}

/*! This method returns the stamina information of the agent
    \return Stamina stamina of the agent */
Stamina WorldModel::getAgentStamina( ) const
{
    return agentObject.getStamina();
}

/*! This method returns a TiredNessT value that indicates how tired the agent
   is.
	 \return TiredNessT value that indicates how tired the agent is. */
TiredNessT WorldModel::getAgentTiredNess( ) const
{
    return getAgentStamina().getTiredNess(
               SS->getRecoverDecThr(), SS->getStaminaMax() );
}

/*! This method returns the effort information of the agent
    \return double effort of the agent */
double WorldModel::getAgentEffort( ) const
{
    return agentObject.getStamina().getEffort();
}

/*! This method returns the global velocity information of the agent
    \return global velocity of the agent */
VecPosition WorldModel::getAgentGlobalVelocity( ) const
{
    return agentObject.getGlobalVelocity();
}

/*! This method returns the speed of the agent
    \return speed of the agent */
double WorldModel::getAgentSpeed( ) const
{
    return agentObject.getSpeed();
}

/*! This method returns the global position of the agent
    \return global position of the agent */
VecPosition WorldModel::getAgentGlobalPosition( ) const
{
    return agentObject.getGlobalPosition();
}

/*! This method sets the view angle of the agent.
    \return bool indicating whether update was successful.  */
bool WorldModel::setAgentViewAngle( ViewAngleT va )
{
    agentObject.setViewAngle( va );
    return true;
}


/*! This method returns the view angle of the agent.

    \return ViewAngleT view angle of the agent (VA_NARROW, VA_NORMAL,
    VA_WIDE)*/
ViewAngleT WorldModel::getAgentViewAngle( ) const
{
    return agentObject.getViewAngle();
}

/*! This method sets the view quality of the agent.
    \return bool indicating whether update was successful.  */
bool WorldModel::setAgentViewQuality( ViewQualityT vq )
{
    agentObject.setViewQuality( vq );
    return true;
}
/*! This method returns the view quality of the agent
    \return ViewQualityT of the agent (VA_LOW, VA_HIGH). */
ViewQualityT WorldModel::getAgentViewQuality( ) const
{
    return agentObject.getViewQuality();
}

/*! This method returns the view frequency of see messages for the agent
    relative to the time of the sense_body interval. So 0.5 means a see
    message arrives twice in every simulation cycle.
    \return double representing the view frequency */
double WorldModel::getAgentViewFrequency( ViewAngleT va, ViewQualityT vq )
{
    double dViewQualityFactor ;
    double dViewWidthFactor   ;

    if ( va == VA_ILLEGAL )
        va = getAgentViewAngle();
    if ( vq == VQ_ILLEGAL )
        vq = getAgentViewQuality();

    //!modify
//   switch( va )
//   {
//     case VA_NARROW:  dViewWidthFactor   = 0.5; break;
//     case VA_NORMAL:  dViewWidthFactor   = 1.0; break;
//     case VA_WIDE:    dViewWidthFactor   = 2.0; break;
//     case VA_ILLEGAL:
//     default:         dViewWidthFactor   = 0.0; break;
//   }

    switch ( va )
    {
    case VA_NARROW:
        dViewWidthFactor   = 1.0;
        break;
    case VA_NORMAL:
        dViewWidthFactor   = 2.0;
        break;
    case VA_WIDE:
        dViewWidthFactor   = 3.0;
        break;
    case VA_ILLEGAL:
    default:
        dViewWidthFactor   = 2.0;
        break;
    }

//   switch( vq )
//   {
//     case VQ_LOW:     dViewQualityFactor = 0.5; break;
//     case VQ_HIGH:    dViewQualityFactor = 1.0; break;
//     case VQ_ILLEGAL:
//     default:         dViewQualityFactor = 0.0; break;
//   }
    dViewQualityFactor = 1.0;
    return dViewQualityFactor*dViewWidthFactor;
}

/*! This method returns whether the arm of the agent can be moved. */
bool WorldModel::getAgentArmMovable( )
{
    return agentObject.getArmMovable();
}

/*! This method returns the current position the arm of the agent is
    pointing towards. */
VecPosition WorldModel::getAgentArmPosition( )
{
    return agentObject.getGlobalArmPosition();
}

/*! This method returns how many cycles it will last before the arm
    of the agent stops pointing. */
int WorldModel::getAgentArmExpires( )
{
    return agentObject.getArmExpires();
}

/*! This method returns the global position of the ball. The method
    getConfidence with as argument OBJECT_BALL should be called to check the
    confidence of this global position.
    \return global position bal. */
VecPosition  WorldModel::getBallPos()
{
    return getGlobalPosition( OBJECT_BALL );
}

/*! This method returns the current estimate of the speed of the ball.
    \return speed of the ball (magnitude of the global velocity vector). */
double WorldModel::getBallSpeed()
{
    return Ball.getGlobalVelocity().getMagnitude();
}

/*! This method returns the global direction of the ball velocity.
    \return global direction of the ball velocity */
AngDeg WorldModel::getBallDirection()
{
    return Ball.getGlobalVelocity().getDirection();
}

/*! This method returns the time of the global position
    of the specified object.
    \param ObjectT that represent the type of the object to check
    \return time corresponding to the global position of 'o' */
Time WorldModel::getTimeGlobalPosition( ObjectT o )
{
    PlayerObject *object = (PlayerObject*) getObjectPtrFromType( o );
    if ( object != NULL )
        return object->getTimeGlobalPosition();
    return UnknownTime;
}


/*! This method returns the global position of an objectType. This
    method is normally used for the objects on the field (player,
    opponents and the ball).  When the global position cannot be
    determined, a VecPosition with both the x and y coordinate are set
    to 'UnknownDoubleValue'.

    \param ObjectT that represent the type of the object to check
    \return VecPosition containing the global position. */
VecPosition WorldModel::getGlobalPosition( ObjectT o )
{
    Object *object = getObjectPtrFromType( o );
    if ( object != NULL )
    {
        if ( SoccerTypes::isFlag( o ) || SoccerTypes::isGoal( o ) )
            return SoccerTypes::getGlobalPositionFlag( o, getSide(),
                    SS->getGoalWidth() );
        else
            return object->getGlobalPosition();
    }
    return VecPosition( UnknownDoubleValue, UnknownDoubleValue);
}

/*! This method returns the time of the global velocity
    of the specified object.
    \param ObjectT that represent the type of the object to check
    \return time corresponding to the global velocity of 'o' */
Time WorldModel::getTimeGlobalVelocity( ObjectT o )
{
    PlayerObject *object = (PlayerObject*) getObjectPtrFromType( o );
    if ( object != NULL )
        return object->getTimeGlobalVelocity();
    return UnknownTime;
}


/*! This method returns the global velocity of an objectType. When the
    global position cannot be determined, a VecPosition is returned with
    both the x and y coordinate set to 'UnknownDoubleValue'.
    \param ObjectT that represent the type of the object to check
    \return VecPosition containing the global position. */
VecPosition WorldModel::getGlobalVelocity( ObjectT o )
{
    DynamicObject *object = (DynamicObject*)getObjectPtrFromType( o );
    if ( object != NULL )
        return object->getGlobalVelocity(  );
    return VecPosition( UnknownDoubleValue, UnknownDoubleValue );

}

/*! This method returns the relative distance between the agent and the object
    supplied as the first argument. No check is made whether this information
    is up to date (use isVisible or getConfidence for that).
    \param ObjectT that represent the type of the object to check
    \return relative distance to this object */
double WorldModel::getRelativeDistance( ObjectT o )
{
    Object *object = getObjectPtrFromType( o );
    if ( object != NULL )
        return object->getRelativeDistance();
    return UnknownDoubleValue;
}

/*! This method returns the relative position of the object to the agent.
    No check is made whether this information is up to date (use isVisible or
    getConfidence for that).
    \param ObjectT that represent the type of the object to check
    \return relative position to this object */
VecPosition  WorldModel::getRelativePosition( ObjectT o )
{
    Object *object = getObjectPtrFromType( o );
    if ( object != NULL )
        return object->getRelativePosition();
    return VecPosition(UnknownDoubleValue, UnknownDoubleValue);
}

/*! This method returns the relative angle between the agent and the object
    supplied as the first argument. No check is made whether this information
    is up to date (use isVisible or getConfidence for that).
    By default the returned angle is relative to the neck
    of the agent. When the second argument 'bWithBody' is set to true, the
    returned angle is relative to the body of the agent.
    \param ObjectT that represent the type of the object to check
    \param bWithBody when true angle is relative to body, otherwise to neck
           (default false)
    \return relative angle to this object */
AngDeg WorldModel::getRelativeAngle( ObjectT o, bool bWithBody )
{
    Object *object = getObjectPtrFromType( o );
    double dBody   = 0.0;

    if ( object != NULL )
    {
        if ( bWithBody == true )
            dBody = getAgentBodyAngleRelToNeck();
        return VecPosition::normalizeAngle( object->getRelativeAngle() - dBody );
    }
    return UnknownDoubleValue;
}

/*! This method returns the time of the global angles (both body and
    neck angle) of the specified object.

    \param ObjectT that represent the type of the object to check
    \return time corresponding to both the stored body and neck angle */
Time WorldModel::getTimeGlobalAngles( ObjectT o )
{
    PlayerObject *object = (PlayerObject*) getObjectPtrFromType( o );
    if ( object != NULL )
        return object->getTimeGlobalAngles();
    return Time( -1, 0);
}

/*! This method returns the global body angle of the specified object.
    No check is made whether this information is up to date (use
    getTimeGlobalAngles).
    \param ObjectT that represent the type of the object to check
    \return last known global body angle of this object */
AngDeg WorldModel::getGlobalBodyAngle( ObjectT o )
{
    PlayerObject *object = (PlayerObject*) getObjectPtrFromType( o );
    if ( object != NULL )
        return object->getGlobalBodyAngle();
    return UnknownAngleValue;
}

/*! This method returns the global neck angle of the specified object.
    No check is made whether this information is up to date (use
    getTimeGlobalAngles).
    \param ObjectT that represent the type of the object to check
    \return last known global neck angle of this object */
AngDeg WorldModel::getGlobalNeckAngle( ObjectT o )
{
    PlayerObject *object = (PlayerObject*) getObjectPtrFromType( o );
    if ( object != NULL )
        return object->getGlobalNeckAngle();
    return UnknownAngleValue;
}

/*! This method returns the global angle of the specified object (this object
    is normally a line).
    \param ObjectT that represent the type of the object to check
    \return global angle of this object in the field */
AngDeg WorldModel::getGlobalAngle( ObjectT o )
{
    if ( SoccerTypes::isLine( o ) )
        return SoccerTypes::getGlobalAngleLine( o, getSide() );
    return UnknownAngleValue;
}


/*! This method returns the confidence value of the object
    supplied as the first argument. The confidence is calculated using the
    current server cycle and the time the object was last seen.
    \param ObjectT that represent the type of the object to check
    \return confidence value [0.0, 1.0] that indicates the confidence value */
double WorldModel::getConfidence( ObjectT o)
{
    Object *object = getObjectPtrFromType( o );
    if ( object != NULL )
        return object->getConfidence( getCurrentTime() );

    return 0.0;
}

/*! This method returns wheter the specified object type is a known player.
    A known player is a player of which we know for certain that the player
    number is correct. If a player is seen without a number and it cannot be
    mapped to a player, it is put on the first empty position in the
    player list and the status of known player is set to false.
    \param o object type of player that should be checked
    \return bool indicating whether we are certain of number of player 'o'. */
bool WorldModel::isKnownPlayer( ObjectT o )
{
    PlayerObject *object = (PlayerObject *)getObjectPtrFromType( o );
    if ( object != NULL  )
        return object->getIsKnownPlayer();
    return false;

}

/*! This method returns the object type of the opponent
    goalkeeper. Which object type is the actual goalkeeper is checked
    in different ways. First of all this information is can be
    available in a see message.  When no player is stored in the world
    model of which this is is perceived, the opponent goalkeeper is
    assumed the player with the highest x coordinate, but only if this
    player stands very close in front of the goal

    \return ObjectT that represents the opponent goalkeeper, OBJECT_ILLEGAL if
    it cannot be determined which object type is the opponent goalkeeper. */
ObjectT WorldModel::getOppGoalieType()
{
    static ObjectT objGoalieType = OBJECT_ILLEGAL;

    if ( objGoalieType != OBJECT_ILLEGAL &&
            isConfidenceGood( objGoalieType ) &&
            isKnownPlayer( objGoalieType ) )
        return objGoalieType;

    ObjectT objOppMaxX = OBJECT_ILLEGAL;
    double  x = -100.0, y = UnknownDoubleValue;

    for ( int i = 0; i < MAX_OPPONENTS; i++ )
    {
        if ( isConfidenceGood( Opponents[i].getType( ) ) )
        {
            if ( Opponents[i].getIsGoalie() == true ) // &&
//           Opponents[i].getGlobalPosition().getX() > PENALTY_X - 2.0 )
            {
                objGoalieType = Opponents[i].getType();
                return Opponents[i].getType();
            }
            if ( Opponents[i].getGlobalPosition().getX() > x )
            {
                x          = Opponents[i].getGlobalPosition().getX();
                y          = Opponents[i].getGlobalPosition().getY();
                objOppMaxX = Opponents[i].getType();
            }
        }
    }

    // if opponent with highest x is nr 1, assume it is goalkeeper when standing
    // in own penalty area, otherwise assume goalkeeper closest player to goal.
    if ( (objOppMaxX == OBJECT_OPPONENT_1 && x > PENALTY_X + 4.0 ) ||
            (objOppMaxX != OBJECT_ILLEGAL    && x > PITCH_LENGTH/2.0 - 6.0 &&
             fabs( y ) < SS->getGoalWidth()/2.0 ))
        return objOppMaxX;
    return OBJECT_ILLEGAL;
}


/*! This method returns the object type of the own goalkeeper. Which object
    type is the actual goalkeeper is checked in different ways. First of all
    this information is available in the see, when (goalie) is behind the
    perceived object. When no player is stored in the world model of which
    this is perceived, the own goalkeeper is assumed the player with the lowest
    x coordinate, but only if this player stands close in front of the goal.
    \return ObjectT that represents the own goalkeeper, OBJECT_ILLEGAL if
    it cannot be determined which object type is the own goalkeeper. */
ObjectT WorldModel::getOwnGoalieType()
{
    ObjectT objOwnMinX = OBJECT_ILLEGAL;
    double x = 100.0, y = UnknownDoubleValue;
    for ( int i = 0; i < MAX_TEAMMATES; i++ )
    {
        if ( isConfidenceGood( Teammates[i].getType( ) ) )
        {
            if ( Teammates[i].getIsGoalie() == true )
                return Teammates[i].getType();
            if ( Teammates[i].getGlobalPosition().getX() < x )
            {
                x          = Teammates[i].getGlobalPosition().getX();
                y          = Teammates[i].getGlobalPosition().getY();
                objOwnMinX = Teammates[i].getType();
            }
        }
    }
    if ( ( objOwnMinX == OBJECT_TEAMMATE_1 && x < - ( PENALTY_X + 4.0 ) ) ||
            (objOwnMinX != OBJECT_ILLEGAL    && x < -  PITCH_LENGTH/2.0 + 6.0  &&
             fabs( y ) < SS->getGoalWidth()/2.0 ))
        return objOwnMinX;
    return OBJECT_ILLEGAL;
}


/*! This method returns the last server cycle the specified object has been
    seen.
    \param object type of object that should be checked
    \return server time this object was last seen (in a see message). */
Time WorldModel::getTimeLastSeen( ObjectT o )
{
    Object *object = getObjectPtrFromType( o );
    if ( object != NULL )
        return object->getTimeLastSeen(  );
    return Time( -1, 0);

}

/*! This method returns the last server cycle the relative distance change of
    the specified object has been reported.
    \param object type of object that should be checked
    \return server time relative distance change of this object was last
           seen (in a see message). */
Time WorldModel::getTimeChangeInformation( ObjectT o )
{
    DynamicObject *object = (DynamicObject*)getObjectPtrFromType( o );
    if ( object != NULL )
        return object->getTimeChangeInformation(  );
    return Time( -1, 0);

}

/*! This method returns the last global position derived from a see message.
    The time corresponds to the method 'getTimeGlobalPositionLastSee'.
    \param object type of object that should be checked
    \return global position related to the last see message. */
VecPosition WorldModel::getGlobalPositionLastSee( ObjectT o )
{
    DynamicObject *object = (DynamicObject*)getObjectPtrFromType( o );
    if ( object != NULL )
        return object->getGlobalPositionLastSee(  );
    return VecPosition( UnknownDoubleValue, UnknownDoubleValue);
}

/*! This method returns the last server cycle the global position
    the specified object has been calculated.
    \param object type of object that should be checked
    \return server time global position of this object was last
           derived (from a see message). */
Time WorldModel::getTimeGlobalPositionLastSee( ObjectT o )
{
    DynamicObject *object = (DynamicObject*)getObjectPtrFromType( o );
    if ( object != NULL )
        return object->getTimeGlobalPosDerivedFromSee(  );
    return UnknownTime;
}

/*! This method returns the last global velocity derived from a see message.
    The time corresponds to the method 'getTimeChangeInformation'.
    \param object type of object that should be checked
    \return global velocity related to the last see message. */
VecPosition WorldModel::getGlobalVelocityLastSee( ObjectT o )
{
    DynamicObject *object = (DynamicObject*)getObjectPtrFromType( o );
    if ( object != NULL )
        return object->getGlobalVelocityLastSee(  );
    return VecPosition( UnknownDoubleValue, UnknownDoubleValue);
}

/*! This method returns the last global body angle derived from a see message.
    The time corresponds to the method 'getTimeChangeInformation'.
    \param object type of object that should be checked
    \return global body angle related to the last see message. */
AngDeg WorldModel::getGlobalBodyAngleLastSee( ObjectT o )
{
    PlayerObject *object = (PlayerObject*)getObjectPtrFromType( o );
    if ( object != NULL )
        return object->getGlobalBodyAngleLastSee(  );
    return UnknownAngleValue;
}

/*! This method returns the number of cycles it will take the tackle to expire.
    In case of the argument OBJECT_ILLEGAL, the number of cycles for the
    agentObject is returned. */
int WorldModel::getTackleExpires( ObjectT o )
{
    if ( o == OBJECT_ILLEGAL || o == getAgentObjectType() )
        return agentObject.getTackleExpires();

    PlayerObject *object = (PlayerObject*)getObjectPtrFromType( o );
    if ( object == NULL )
        return 0;
    return max(0, object->getTimeTackle( ) -
               getCurrentTime()         +
               SS->getTackleCycles());
}

/*! This method returns the arm direction of object 'o'. It does not keep track
    of how relevant this information is. When the pointing agent stops pointing
    the angle is set to UnknownAngleValue. */
AngDeg WorldModel::getGlobalArmDirection( ObjectT o )
{
    PlayerObject *object = (PlayerObject*)getObjectPtrFromType( o );
    if ( object == NULL )
        return UnknownAngleValue;

    return object->getGlobalArm();
}

/*! This method returns the time related to the global arm direction of object
    'o'. When the pointing agent stops pointing the angle is set to
    UnknownAngleValue and the returned time is not relevant. */
Time  WorldModel::getTimeGlobalArmDirection( ObjectT o )
{
    PlayerObject *object = (PlayerObject*)getObjectPtrFromType( o );
    if ( object == NULL )
        return Time(-1,0);

    return object->getTimeGlobalArm();
}

/*! This method returns the probability that a tackle performed by object o
    will succeed. This probability depends on the relative distance to the
    ball in both the x and y direction and various server parameters. In the
    case that o equals OBJECT_ILLEGAL, the returned probability corresponds
    to that of the agent object. */
double WorldModel::getProbTackleSucceeds( ObjectT o, int iExtraCycles,
        VecPosition *pos)
{
    if ( o == OBJECT_ILLEGAL )
        o = getAgentObjectType();

    VecPosition posObject   = getGlobalPosition( o );
    VecPosition posBall     = (pos == NULL ) ? getBallPos() : *pos ;
    AngDeg      angBody     = getGlobalBodyAngle( o );
    int         iExtra      = 0;
    double      dTackleDist, dDist=0;

    // if opponent goalie is within 3 metres he can probably catch in next cycle
    // RC2003 HACK
    if ( o == getOppGoalieType() &&
            posBall.getDistanceTo( o ) < 3.0  )
        return 1.0;

    if ( o != getAgentObjectType() )
    {
        // get the number of cycles object was not seen and assume he moves 0.6
        // in every cycle. Only in case of bad body direction subtract one.
        // then move object position closer to the ball
        dDist  = posBall.getDistanceTo( posObject );
        iExtra = getCurrentTime() - getTimeLastSeen( o ) + iExtraCycles;
        AngDeg ang    = (posBall - posObject).getDirection();

        // if body angle ok,
        if ( getCurrentTime() - getTimeGlobalAngles( o ) < 2 )
        {
            if ( fabs( VecPosition::normalizeAngle( ang - angBody ) ) > 35 )
                iExtra --;
            if ( getGlobalVelocity( o ).getMagnitude() < 0.2 )
                iExtra --;
        }

        double dExtra = 0.7*max( 0, iExtra );

        // if object was not seen in last see message, he stood further away
        // then the visible_distance.
        if ( getTimeLastSeen( o ) != getTimeLastSeeMessage() &&
                getCurrentTime() == getTimeLastSeeMessage() &&
                dDist - dExtra < SS->getVisibleDistance() )
        {
            Log.log( 560, "prob tackle succeeds: opp not seen raise dExtra" );
            dExtra = dDist - SS->getVisibleDistance();
        }

        // now incorporate that to kick the ball we may need more cycles
        //    dExtra = max( 0, dExtra + iExtraCycles );

        // do not move object more than 4.0 metres.
        posObject += VecPosition( min(4.0,min(dDist - 0.2, dExtra )), ang, POLAR );

        // object is directed towards ball
        angBody = ang;
    }

    VecPosition posBallRel  = posBall - posObject;
    posBallRel.rotate( - angBody );
    if ( posBallRel.getX() > 0.0 )      // ball in front -> tackle_dist parameter
        dTackleDist = SS->getTackleDist();
    else
        dTackleDist = SS->getTackleBackDist();

    double dProb =
        pow(fabs(posBallRel.getX())/dTackleDist         ,SS->getTackleExponent())+
        pow(fabs(posBallRel.getY())/SS->getTackleWidth(),SS->getTackleExponent());

    Log.log( 556,
             "tackle relpos o %d: (%f,%f) dist %f body %f extra %d %d: prob %f",
             o, posBallRel.getX(),posBallRel.getY(), dDist, angBody, iExtra,
             iExtraCycles, max(0,1-dProb) );

    return max( 0, 1 - dProb );
}

/*! This method returns a list with all the opponents that are location within
  distance of 'dDist' of position 'pos'. */
list<ObjectT> WorldModel::getListCloseOpponents( VecPosition pos,double dDist )
{
    int iIndex;
    list<ObjectT> listOpp;

    for ( ObjectT o = iterateObjectStart( iIndex, OBJECT_SET_OPPONENTS );
            o != OBJECT_ILLEGAL;
            o = iterateObjectNext ( iIndex, OBJECT_SET_OPPONENTS  ) )
    {
        if ( getGlobalPosition( o ).getDistanceTo( pos ) < dDist )
            listOpp.push_back( o );
    }
    iterateObjectDone( iIndex );
    return listOpp;
}

/*! This method returns the tackle probability of the closest opponent to the
    ball. */
double WorldModel::getProbTackleClosestOpp( int iExtraCycles )
{
    ObjectT obj = getClosestInSetTo( OBJECT_SET_OPPONENTS, OBJECT_BALL );
    if ( obj == OBJECT_ILLEGAL )
        return -1.0;

    return getProbTackleSucceeds( obj, iExtraCycles );
}

/*! This method sets the value of the specified object to a known
    player or not.  A known player is a player of which the exact team
    name and player number are known. If the player number is not
    known, information about the object is stored at an empty position
    in the player array and the value of isKnownPlayer is set to
    'false'.

    \param o object type of which known player information should be set
    \param isKnownPlayer new known player value
    \return boolean indicating whether update was successful */
bool WorldModel::setIsKnownPlayer( ObjectT o, bool isKnownPlayer )
{
    PlayerObject *object = (PlayerObject*) getObjectPtrFromType( o );
    return object->setIsKnownPlayer( isKnownPlayer );
}

/*! This method sets the time the object 'o' has last been seen.
    \param o object of which the time should be changed
    \param time new time for this object
    \return bool indicating whether update was successful. */
bool WorldModel::setTimeLastSeen( ObjectT o, Time time )
{
    PlayerObject *object = (PlayerObject*) getObjectPtrFromType( o );
    return object->setTimeLastSeen( time );
}

/*! This method sets the heterogeneous player type of the object that is
    passed as the first argument.
    \param o object type of which the heterogeneous player type should be set
    \param iPlayer new heterogeneous player type of this object. */
bool WorldModel::setHeteroPlayerType( ObjectT  o, int iPlayerType )
{
    PlayerObject *object = (PlayerObject*) getObjectPtrFromType( o );
    if ( o == getAgentObjectType() )
        updateSSToHeteroPlayerType( iPlayerType );

    return object->setHeteroPlayerType( iPlayerType );
}

/*! This method returns the player type of the object 'o' in the
  current formation. */
PlayerT WorldModel::getPlayerType ( ObjectT o )
{
    if ( o == OBJECT_ILLEGAL )
        o = getAgentObjectType();
    return formations->getPlayerType( SoccerTypes::getIndex( o ) );
}

/*! This method returns whether the object 'o' is located in the set of
    player types 'ps'. */
bool WorldModel::isInPlayerSet( ObjectT o, PlayerSetT ps )
{
    return SoccerTypes::isPlayerTypeInSet( getPlayerType( o ), ps );
}


/*! This methods returns the heterogeneous player type of object 'obj'.
    Initially, the player types of all players are set to the type of the agent
    itself. After new information from the coach, they are set correctly. */
int WorldModel::getHeteroPlayerType( ObjectT obj )
{
    PlayerObject *object = (PlayerObject*) getObjectPtrFromType( obj );

    return object->getHeteroPlayerType( );

}


/*! This method returns the global position of the opponent goal.
    \return VecPosition containing the position of the opponent goal. */
VecPosition WorldModel::getPosOpponentGoal( )
{
    ObjectT objGoal = SoccerTypes::getGoalOpponent( getSide() );
    if ( isPenaltyUs() || isPenaltyThem() )
        objGoal = ( getSidePenalty() == SIDE_LEFT ) ?OBJECT_GOAL_L:OBJECT_GOAL_R ;
    return SoccerTypes::getGlobalPositionFlag(
               objGoal,
               getSide( ),
               SS->getGoalWidth() );
}

/*! This method returns the global position of the own goal.
    \return VecPosition containing the position of the own goal. */
VecPosition WorldModel::getPosOwnGoal( )
{
    SideT sideGoal = getSide();
    ObjectT objGoal = SoccerTypes::getOwnGoal( sideGoal );
    if ( isPenaltyUs() || isPenaltyThem() )
        objGoal = (getSidePenalty() == SIDE_LEFT ) ? OBJECT_GOAL_L : OBJECT_GOAL_R;

    return SoccerTypes::getGlobalPositionFlag(
               objGoal,
               getSide( ),
               SS->getGoalWidth() );
}

/*! This method returns the relative distance to the opponent goal
    \return relative distance from the agent to the opponent goal. */
double  WorldModel::getRelDistanceOpponentGoal()
{
    VecPosition posGoal = getPosOpponentGoal();
    return getAgentGlobalPosition().getDistanceTo( posGoal );

}

/*! This method returns the relative angle to the opponent goal. This
    relative angle is the relative angle between the opponent goal
    position and the agent position. The neck and body angle of the
    agent are NOT taken into account.

    \return relative angle between goal and agent position. */
double  WorldModel::getRelAngleOpponentGoal()
{
    VecPosition posGoal;
    if ( sideSide == SIDE_LEFT )
        posGoal = SoccerTypes::getGlobalPositionFlag( OBJECT_GOAL_R, sideSide );
    else
        posGoal = SoccerTypes::getGlobalPositionFlag( OBJECT_GOAL_L, sideSide );

    return ( posGoal - getAgentGlobalPosition()).getDirection() ;

}

/*! This method returns information about the heterogeneous player at index
    'iIndex'. This information consists of a subset of the ServerSettings
    values that fully specify the information of the heterogeneous player. */
HeteroPlayerSettings WorldModel::getInfoHeteroPlayer( int iIndex )
{
    return pt[iIndex];
}

/*! This method returns the heterogeneous player information of the object
    'obj'. This information consists of a subset of the ServerSettings
    values that fully specify the information of the heterogeneous player. */
HeteroPlayerSettings WorldModel::getHeteroInfoPlayer( ObjectT obj )
{
//  if( obj == getAgentObjectType() || SoccerTypes::isOpponent( obj ) )
//    return getInfoHeteroPlayer( agentObject.getHeteroPlayerType() );
    if ( ! SoccerTypes::isKnownPlayer( obj ) )
        obj = getAgentObjectType();

    PlayerObject *object = (PlayerObject*) getObjectPtrFromType( obj );
    int          iType   = object->getHeteroPlayerType();
    // default iType in Object = 0
    return getInfoHeteroPlayer( iType );
}


/*! This method inserts a substituted opponent player in the set of substituted
    opponent players. This can then be detected by the coach, who then can
    figure out to which new type this opponent player is changed. */
bool WorldModel::setSubstitutedOpp( ObjectT obj )
{
    m_setSubstitutedOpp.insert( obj );
    return true;
}

/*! This method returns the first substituted opponent player in the set of
    substituted opponent players and then erases this opponent from the set. */
ObjectT WorldModel::getSubstitutedOpp( )
{
    if ( m_setSubstitutedOpp.empty() == true )
        return OBJECT_ILLEGAL;

    ObjectT obj = *m_setSubstitutedOpp.begin();
    m_setSubstitutedOpp.erase( obj );
    return obj;
}


/*! This method returns the dash power rate of the object 'obj'. */
double WorldModel::getDashPowerRate( ObjectT obj )
{
    return getHeteroInfoPlayer( obj ).dDashPowerRate ;
}

/*! This method returns the maximum speed of the object 'obj'. */
double WorldModel::getPlayerSpeedMax( ObjectT obj )
{
    return getHeteroInfoPlayer( obj ).dPlayerSpeedMax ;
}

/*! This method returns the decay of the object 'obj'. */
double WorldModel::getPlayerDecay( ObjectT obj )
{
    return getHeteroInfoPlayer( obj ).dPlayerDecay ;
}

/*! This method returns the maximal kick distance of the object 'obj'. */
double WorldModel::getMaximalKickDist( ObjectT obj )
{
    return getHeteroInfoPlayer( obj ).dMaximalKickDist ;
}

/*! This method returns the stamina increase of the object 'obj'. */
double WorldModel::getStaminaIncMax( ObjectT obj )
{
    return getHeteroInfoPlayer( obj ).dStaminaIncMax ;
}

/*! This method returns the size of the object 'obj'. */
double WorldModel::getPlayerSize( ObjectT obj )
{
    return getHeteroInfoPlayer( obj ).dPlayerSize ;
}

/*! This method returns the inertia moment of the object 'obj'. */
double WorldModel::getInertiaMoment( ObjectT obj )
{
    return getHeteroInfoPlayer( obj ).dInertiaMoment;
}

/*! This method returns the maximum effort of the object 'obj'. */
double WorldModel::getEffortMax( ObjectT obj )
{
    return getHeteroInfoPlayer( obj ).dEffortMax;
}

/*! This method returns the effective max speed of the object 'obj'. */
double WorldModel::getEffectiveMaxSpeed( ObjectT obj, bool bWithNoise )
{
    double dSpeed = 0.0;
    HeteroPlayerSettings pt = getHeteroInfoPlayer( obj );

    for ( int j = 0; j < 15 ; j ++ )
    {
        dSpeed *= pt.dPlayerDecay;
        dSpeed += SS->getMaxPower()*pt.dEffortMax*pt.dDashPowerRate;
        if ( dSpeed > pt.dPlayerSpeedMax )
            dSpeed = pt.dPlayerSpeedMax;
        if ( bWithNoise )
            dSpeed += sqrt(2*(pow(dSpeed * SS->getPlayerRand(), 2)));
    }
    return dSpeed;
}

/*! This method checks whether a queued action is performed. The commands in
    QueuedCommands are the commands that are sent to the server by the
    ActHandler. The performedCommands array contains the commands that are
    performed in the last cycle (using the count information in the sense_body
    message). Using these two array it is possible to check whether a command
    is actually performed by the server. When there is an action that is sent
    to the server and not performed, this method returns false, true otherwise.
    \return true when all commands sent to the server are performed. */
bool WorldModel::isQueuedActionPerformed()
{
    // for all possible commands check if it is sent in previous cycle,
    // but not performed
    for ( int i = 0 ; i < CMD_MAX_COMMANDS ; i++ )
        if ( queuedCommands[i].time   == getTimeLastSenseMessage() - 1 &&
                performedCommands[i]     == false )
            return false;

    return true;
}

/*! This method checks whether the play mode indicates that we have
    a free kick. When the specified PlayModeT equals PM_ILLEGAL (default), the
    current play mode is used.
    \param pm play mode to check. In default case (PM_ILLEGAL) the current play
           mode is used.
    \return bool indicating whether we have a free kick. */
bool WorldModel::isFreeKickUs( PlayModeT pm )
{
    if ( pm == PM_ILLEGAL )
        pm = getPlayMode();

    bool bLeftKick = (pm==PM_FREE_KICK_LEFT  || pm==PM_INDIRECT_FREE_KICK_LEFT );
    bool bRightKick= (pm==PM_FREE_KICK_RIGHT || pm==PM_INDIRECT_FREE_KICK_RIGHT);

    return ( bLeftKick   && getSide() == SIDE_LEFT  ) ||
           ( bRightKick  && getSide() == SIDE_RIGHT ) ;
}

/*! This method checks whether the play mode indicates that the other
    team has a free kick. When the specified PlayModeT equals
    PM_ILLEGAL (default), the current play mode is used.
    \param pm play mode to check. In default case (PM_ILLEGAL) the current play
           mode is used.
    \return bool indicating whether the other team has a free kick. */
bool WorldModel::isFreeKickThem( PlayModeT pm )
{
    if ( pm == PM_ILLEGAL )
        pm = getPlayMode();

    bool bLeftKick = (pm==PM_FREE_KICK_LEFT  || pm==PM_INDIRECT_FREE_KICK_LEFT );
    bool bRightKick= (pm==PM_FREE_KICK_RIGHT || pm==PM_INDIRECT_FREE_KICK_RIGHT);

    return ( bRightKick  && getSide() == SIDE_LEFT  ) ||
           ( bLeftKick   && getSide() == SIDE_RIGHT ) ;
}

/*! This method checks whether the play mode indicates that there is (or
    will be) a before kick off situation. This is the case when the play mode
    equals PM_BEFORE_KICK_OFF or either PM_GOAL_LEFT or PM_GOAL_RIGHT since
    after the goal the play mode will go to PM_BEFORE_KICK_OFF.
    When the specified PlayModeT equals PM_ILLEGAL (default), the
    current play mode is used.
    \param pm play mode to check. In default case (PM_ILLEGAL) the current play
           mode is used.
    \return bool indicating whether there is a before kick off situation. */
bool WorldModel::isBeforeKickOff( PlayModeT pm )
{
    if ( pm == PM_ILLEGAL )
        pm = getPlayMode();

    return pm == PM_BEFORE_KICK_OFF  || pm == PM_GOAL_LEFT  ||
           pm == PM_GOAL_RIGHT || isKickOffUs( pm ) || isKickOffThem( pm );
}

/*! This method checks whether the play mode indicates that there is
    a dead ball situation and our team is allowed to perform the action.
    That is our team has either a free kick, kick in, corner kick or a kick in.
    When the specified PlayModeT equals PM_ILLEGAL (default), the
    current play mode is used.
    \param pm play mode to check. In default case (PM_ILLEGAL) the current play
           mode is used.
    \return bool indicating whether we have a dead ball situation. */
bool WorldModel::isDeadBallUs( PlayModeT pm )
{
    if ( pm == PM_ILLEGAL )
        pm = getPlayMode();

    return isKickInUs  ( pm ) || isFreeKickUs  ( pm ) || isCornerKickUs     ( pm)
           || isKickOffUs ( pm ) || isOffsideThem ( pm ) || isFreeKickFaultThem( pm)
           || isGoalKickUs( pm ) || isBackPassThem( pm ) ;
}


/*! This method checks whether the current play mode indicates that there is
    a dead ball situation and their team is allowed to perform the action.
    That is their team has either a free kick, kick in, corner kick or a kick
    in. When the specified PlayModeT equals PM_ILLEGAL (default), the
    current play mode is used.
    \param pm play mode to check. In default case (PM_ILLEGAL) the current play
           mode is used.
    \return bool indicating whether they have a dead ball situation. */
bool WorldModel::isDeadBallThem( PlayModeT pm )
{
    if ( pm == PM_ILLEGAL )
        pm = getPlayMode();

    return isFreeKickThem( pm ) || isKickInThem  ( pm ) || isCornerKickThem ( pm)
           ||  isKickOffThem ( pm ) || isGoalKickThem( pm ) || isFreeKickFaultUs( pm)
           || isOffsideUs    ( pm ) || isBackPassUs  ( pm ) ;
}

bool WorldModel::setChangeViewCommand( SoccerCommand soc )
{
    m_changeViewCommand = soc;
    return true;
}

SoccerCommand WorldModel::getChangeViewCommand( )
{
    return m_changeViewCommand;
}

/*! This method returns the side the penalty kick is taken. */
SideT WorldModel::getSidePenalty( )
{
    return m_sidePenalty;
}

/*! This method sets the side the penalty kick is taken. */
bool WorldModel::setSidePenalty( SideT side )
{
    m_sidePenalty = side;
    return true;
}


/*! This method checks whether the current play mode indicates that we have
    a corner kick. When the specified PlayModeT equals PM_ILLEGAL (default),
    the current play mode is used.
    \param pm play mode to check. In default case (PM_ILLEGAL) the current play
           mode is used.
    \return bool indicating whether we have a corner kick. */
bool WorldModel::isCornerKickUs( PlayModeT pm )
{
    if ( pm == PM_ILLEGAL )
        pm = getPlayMode();

    return ( pm == PM_CORNER_KICK_LEFT  && getSide() == SIDE_LEFT  ) ||
           ( pm == PM_CORNER_KICK_RIGHT && getSide() == SIDE_RIGHT ) ;
}

/*! This method checks whether the current play mode indicates that the other
    team has a corner kick. When the specified PlayModeT equals PM_ILLEGAL
    (default), the current play mode is used.
    \param pm play mode to check. In default case (PM_ILLEGAL) the current play
           mode is used.
    \return bool indicating whether the other team has a corner kick. */
bool WorldModel::isCornerKickThem( PlayModeT pm )
{
    if ( pm == PM_ILLEGAL )
        pm = getPlayMode();

    return ( pm == PM_CORNER_KICK_RIGHT  && getSide() == SIDE_LEFT  ) ||
           ( pm == PM_CORNER_KICK_LEFT   && getSide() == SIDE_RIGHT ) ;
}

/*! This method checks whether the current play mode indicates that we stood
    offside. When the specified PlayModeT equals PM_ILLEGAL (default), the
    current play mode is used.
    \param pm play mode to check. In default case (PM_ILLEGAL) the current play
           mode is used.
    \return bool indicating whether we stood offside. */
bool WorldModel::isOffsideUs( PlayModeT pm )
{
    if ( pm == PM_ILLEGAL )
        pm = getPlayMode();

    return ( pm == PM_OFFSIDE_RIGHT  && getSide() == SIDE_RIGHT ) ||
           ( pm == PM_OFFSIDE_LEFT   && getSide() == SIDE_LEFT );
}

/*! This method checks whether the current play mode indicates that the other
    team stood offside. When the specified PlayModeT equals PM_ILLEGAL
    (default), the current play mode is used.
    \param pm play mode to check. In default case (PM_ILLEGAL) the current play
           mode is used.
    \return bool indicating whether the other team stood offside. */
bool WorldModel::isOffsideThem( PlayModeT pm )
{
    if ( pm == PM_ILLEGAL )
        pm = getPlayMode();

    return ( pm == PM_OFFSIDE_LEFT  && getSide() == SIDE_RIGHT ) ||
           ( pm == PM_OFFSIDE_RIGHT && getSide() == SIDE_LEFT );
}

/*! This method checks whether the current play mode indicates that we have
    a kick in. When the specified PlayModeT equals PM_ILLEGAL (default), the
    current play mode is used.
    \param pm play mode to check. In default case (PM_ILLEGAL) the current play
           mode is used.
    \return bool indicating whether we have a kick in. */
bool WorldModel::isKickInUs( PlayModeT pm )
{
    if ( pm == PM_ILLEGAL )
        pm = getPlayMode();

    return ( pm == PM_KICK_IN_LEFT  && getSide() == SIDE_LEFT  ) ||
           ( pm == PM_KICK_IN_RIGHT && getSide() == SIDE_RIGHT ) ;
}

/*! This method checks whether the current play mode indicates that the other
    team has a kick in. When the specified PlayModeT equals PM_ILLEGAL
    (default), the current play mode is used.
    \param pm play mode to check. In default case (PM_ILLEGAL) the current play
           mode is used.
    \return bool indicating whether the other team has a kick in. */
bool WorldModel::isKickInThem( PlayModeT pm )
{
    if ( pm == PM_ILLEGAL )
        pm = getPlayMode();

    return ( pm == PM_KICK_IN_RIGHT  && getSide() == SIDE_LEFT  ) ||
           ( pm == PM_KICK_IN_LEFT   && getSide() == SIDE_RIGHT ) ;
}

/*! This method checks whether the current play mode indicates that we have
    made a free kick fault. This happens when a player kicks the ball twice
		after a free kick or a kick in.
		When the specified PlayModeT equals PM_ILLEGAL (default), the
    current play mode is used.
    \param pm play mode to check. In default case (PM_ILLEGAL) the current play
           mode is used.
    \return bool indicating whether we have made a free kick fault. */
bool WorldModel::isFreeKickFaultUs( PlayModeT pm )
{
    if ( pm == PM_ILLEGAL )
        pm = getPlayMode();

    return ( pm == PM_FREE_KICK_FAULT_LEFT  && getSide() == SIDE_LEFT  ) ||
           ( pm == PM_FREE_KICK_FAULT_RIGHT && getSide() == SIDE_RIGHT ) ;
}

/*! This method checks whether the current play mode indicates that the other
    team has made a free kick fault. This happens when a player kicks the ball
		twice after a free kick or a kick in.
		When the specified PlayModeT equals PM_ILLEGAL (default), the
    current play mode is used.
    \param pm play mode to check. In default case (PM_ILLEGAL) the current play
           mode is used.

    \return bool indicating whether the other team has made a free
    kick fault.*/
bool WorldModel::isFreeKickFaultThem( PlayModeT pm )
{
    if ( pm == PM_ILLEGAL )
        pm = getPlayMode();

    return ( pm == PM_FREE_KICK_FAULT_RIGHT  && getSide() == SIDE_LEFT  ) ||
           ( pm == PM_FREE_KICK_FAULT_LEFT   && getSide() == SIDE_RIGHT ) ;
}

/*! This method checks whether the current play mode indicates that we have
    a kick off. When the specified PlayModeT equals PM_ILLEGAL (default), the
    current play mode is used.
    \param pm play mode to check. In default case (PM_ILLEGAL) the current play
           mode is used.
    \return bool indicating whether we have a kick off. */
bool WorldModel::isKickOffUs( PlayModeT pm )
{
    if ( pm == PM_ILLEGAL )
        pm = getPlayMode();

    return ( pm == PM_KICK_OFF_LEFT  && getSide() == SIDE_LEFT  ) ||
           ( pm == PM_KICK_OFF_RIGHT && getSide() == SIDE_RIGHT ) ;
}

/*! This method checks whether the current play mode indicates that the other
    team has a kick off. When the specified PlayModeT equals PM_ILLEGAL
    (default), the current play mode is used. When the specified PlayModeT
    equals PM_ILLEGAL (default), the current play mode is used.
    \param pm play mode to check. In default case (PM_ILLEGAL) the current play
           mode is used.
    \return bool indicating whether the other team has a kick off. */
bool WorldModel::isKickOffThem( PlayModeT pm )
{
    if ( pm == PM_ILLEGAL )
        pm = getPlayMode();

    return ( pm == PM_KICK_OFF_RIGHT  && getSide() == SIDE_LEFT  ) ||
           ( pm == PM_KICK_OFF_LEFT   && getSide() == SIDE_RIGHT ) ;
}

/*! This method checks whether the current play mode indicates that we have
    made a back pass (which is not allowed). This occurs when a teamamte has
		passed the ball back to the goalkeeper and he catched it.
		When the specified PlayModeT equals PM_ILLEGAL (default), the
    current play mode is used.
    \param pm play mode to check. In default case (PM_ILLEGAL) the current play
           mode is used.
    \return bool indicating whether we have made a back pass. */
bool WorldModel::isBackPassUs( PlayModeT pm )
{
    if ( pm == PM_ILLEGAL )
        pm = getPlayMode();

    return ( pm == PM_BACK_PASS_LEFT  && getSide() == SIDE_LEFT  ) ||
           ( pm == PM_BACK_PASS_RIGHT && getSide() == SIDE_RIGHT ) ;
}

/*! This method checks whether the current play mode indicates that
    the other team has made a back pass (which is not allowed). This
    occurs when an opponent has passed the ball back to the goalkeeper
    and he catched it.  When the specified PlayModeT equals PM_ILLEGAL
    (default), the current play mode is used.

    \param pm play mode to check. In default case (PM_ILLEGAL) the current play
           mode is used.
    \return bool indicating whether the other team has made a back pass. */
bool WorldModel::isBackPassThem( PlayModeT pm )
{
    if ( pm == PM_ILLEGAL )
        pm = getPlayMode();

    return ( pm == PM_BACK_PASS_RIGHT && getSide() == SIDE_LEFT  ) ||
           ( pm == PM_BACK_PASS_LEFT  && getSide() == SIDE_RIGHT ) ;
}

/*! This method checks whether the current play mode indicates that we have
    a goal kick. When the specified PlayModeT equals PM_ILLEGAL (default), the
    current play mode is used.
    \param pm play mode to check. In default case (PM_ILLEGAL) the current play
           mode is used.
    \return bool indicating whether we have a goal kick. */
bool WorldModel::isGoalKickUs( PlayModeT pm )
{
    if ( pm == PM_ILLEGAL )
        pm = getPlayMode();

    return ( pm == PM_GOAL_KICK_LEFT  && getSide() == SIDE_LEFT  ) ||
           ( pm == PM_GOAL_KICK_RIGHT && getSide() == SIDE_RIGHT ) ;
}

/*! This method checks whether the current play mode indicates that the other
    team has a kick off. When the specified PlayModeT equals PM_ILLEGAL
    (default), the current play mode is used.
    \param pm play mode to check. In default case (PM_ILLEGAL) the current play
           mode is used.
    \return bool indicating whether the other team has a kick off. */
bool WorldModel::isGoalKickThem( PlayModeT pm )
{
    if ( pm == PM_ILLEGAL )
        pm = getPlayMode();

    return ( pm == PM_GOAL_KICK_RIGHT  && getSide() == SIDE_LEFT  ) ||
           ( pm == PM_GOAL_KICK_LEFT   && getSide() == SIDE_RIGHT ) ;
}

/*! This method checks whether the current play mode indicates that we have
    a penalty. When the specified PlayModeT equals PM_ILLEGAL (default), the
    current play mode is used.
    \param pm play mode to check. In default case (PM_ILLEGAL) the current play
           mode is used.
    \return bool indicating whether we have a penalty. */
bool WorldModel::isPenaltyUs( PlayModeT pm )
{
    if ( pm == PM_ILLEGAL )
        pm = getPlayMode();

    return ( (
                 ( pm == PM_PENALTY_SETUP_LEFT ||
                   pm == PM_PENALTY_READY_LEFT ||
                   pm == PM_PENALTY_TAKEN_LEFT ) && getSide() == SIDE_LEFT  )   ||
             (
                 ( pm == PM_PENALTY_SETUP_RIGHT ||
                   pm == PM_PENALTY_READY_RIGHT ||
                   pm == PM_PENALTY_TAKEN_RIGHT ) && getSide() == SIDE_RIGHT  ) );
}


/*! This method checks whether the current play mode indicates that the other
    team takes a penalty. When the specified PlayModeT equals PM_ILLEGAL
    (default), the current play mode is used.
    \param pm play mode to check. In default case (PM_ILLEGAL) the current play
           mode is used.
    \return bool indicating whether the other team has a penalty. */
bool WorldModel::isPenaltyThem( PlayModeT pm )
{
    if ( pm == PM_ILLEGAL )
        pm = getPlayMode();

    return ( (
                 ( pm == PM_PENALTY_SETUP_LEFT ||
                   pm == PM_PENALTY_READY_LEFT ||
                   pm == PM_PENALTY_TAKEN_LEFT ) && getSide() == SIDE_RIGHT  )   ||
             (
                 ( pm == PM_PENALTY_SETUP_RIGHT ||
                   pm == PM_PENALTY_READY_RIGHT ||
                   pm == PM_PENALTY_TAKEN_RIGHT ) && getSide() == SIDE_LEFT  ) );
}

bool WorldModel::isFullStateOn( SideT s )
{
    if ( s == SIDE_ILLEGAL )
        s = getSide();

    if ( s == SIDE_LEFT )
        return SS->getFullStateLeft();
    else if ( s == SIDE_RIGHT )
        return SS->getFullStateRight();
    else
        return false;
}

/*! This method prints all the objects and information of the agent to the
    specified outputstream. Only the information of the objects that are seen
    recently are printed.
    \param os output stream to which output is written (default cout). */
void WorldModel::show( ostream & os )
{
    int i;
    os << "Worldmodel (" << getCurrentTime() << ")\n" <<
    "========================\n";
    os << "Teamname: " << getTeamName() << "\n";
    if ( Ball.getTimeLastSeen( ).getTime() != -1 )
        Ball.show();
    os << "Teammates: " << "\n";
    for ( i = 0; i < MAX_TEAMMATES ; i++ )
        if ( isConfidenceGood( Teammates[i].getType() ) )
            Teammates[i].show( getTeamName() );
    os << "Opponents: " << "\n";
    for ( i = 0; i < MAX_OPPONENTS ; i++ )
        if ( isConfidenceGood( Opponents[i].getType() ) )
            Opponents[i].show( DEFAULT_OPPONENT_NAME );
    os << "Agent: " << "\n";
    agentObject.show( getTeamName() );

    os << "General Info: " << "\n" <<
    "side: "      << SoccerTypes::getSideStr( getSide() )    << "\n" <<
    "kicks: "     << getNrOfCommands( CMD_KICK )             << "\n" <<
    "turns: "     << getNrOfCommands( CMD_TURN )             << "\n" <<
    "dashes: "    << getNrOfCommands( CMD_DASH )             << "\n" <<
    "turnnecks: " << getNrOfCommands( CMD_TURNNECK )         << "\n" <<
    "says: "      << getNrOfCommands( CMD_SAY )              << "\n" <<
    "playmode: "  << SoccerTypes::getPlayModeStr( playMode ) << "\n" <<
    "===================================="                   << "\n";

}

/*! This method prints all the objects and information contained in the object
    set 'set' to specified outputstream.
    \param os output stream to which output is written (default cout). */
void WorldModel::show( ObjectSetT set, ostream & os )
{
    int iIndex;
    for ( ObjectT o = iterateObjectStart( iIndex, set );
            o != OBJECT_ILLEGAL;
            o = iterateObjectNext( iIndex, set ) )
        show( o, os );
    os << "\n";
}

/*! This method prints the queued commands - commands that are sent by the
    ActHandler to the server - to the specified output stream.
    \param os output stream to which information is printed (default cout)*/
void WorldModel::showQueuedCommands( ostream & os )
{
    os << "Commands in queue:\n" ;
    for ( int i = 0; i < CMD_MAX_COMMANDS; i++ )
        if ( queuedCommands[i].commandType != CMD_ILLEGAL )
            queuedCommands[i].show( os );
}

/*! This method prints the information about the Object o to the output stream
    os.
    \param o object of which information should be printed
    \param os output stream to which information is printed. */
void WorldModel::show( ObjectT o, ostream &os )
{
    Object *object = getObjectPtrFromType( o );
    if ( object != NULL )
    {
        if ( SoccerTypes::isPlayer( o ) )
        {
            PlayerObject *pobj = (PlayerObject*) object ;
            if ( SoccerTypes::isTeammate( o ) )
                pobj->show( getTeamName(), os );
            else
                pobj->show( "Opponent", os );
        }
        else
            object->show( os );
    }
}

/*! This method blocks till new information has arrived. Information is either
    a sense_body message or a see message. If there isn't received information
    from the server for longer than 3 seconds, server is assumed dead and
    false is returned.
    \return true when new info has arrived, false if server is dead */
bool WorldModel::waitForNewInformation( )
{
	//cout << "@"<< getCurrentTime() << getCurrentCycle() << endl;
    bool bReturn = true;
    if ( bNewInfo == false ) // there hasn't arrived any information yet
    {
		//cout << " False!!!!" << endl;
#ifdef WIN32
        DWORD waittime = PS->getServerTimeOut() * 1000;
        EnterCriticalSection( &mutex_newInfo );
        int ret;
        Log.logWithTime( 2, "go into conditional wait" );
        while ( (ret = WaitForSingleObject( event_newInfo,
                                            waittime) ) == WAIT_ABANDONED )
            printf("(WorldModel::waitForNewInformation) failure in loop!!\n");
        Log.logWithTime( 2, "go out of conditional wait" );
        if ( ret == WAIT_TIMEOUT ) // if no info was received but timer timed out
            bReturn = false;
        ResetEvent( event_newInfo );
        LeaveCriticalSection( &mutex_newInfo );
#else
        struct timeval now;
        struct timespec timeout;
        gettimeofday(&now, NULL);
        timeout.tv_sec = now.tv_sec + PS->getServerTimeOut();
        timeout.tv_nsec = now.tv_usec*1000;

        // lock mutex and wait till it is unlocked by Sense thread
        // this happens in setTimeLastSeeMessage, setTimeLastSenseMessage
        // or setTimeLastSeeGlobalMessage
        pthread_mutex_lock( &mutex_newInfo );
        int ret;
        Log.logWithTime( 2, "go into conditional wait" );
        while ( (ret = pthread_cond_timedwait( &cond_newInfo,
                                               &mutex_newInfo, &timeout) ) == EINTR )
            printf("(WorldModel::waitForNewInformation) failure in loop!!\n");
        Log.logWithTime( 2, "go out of conditional wait" );
        if ( ret == ETIMEDOUT ) // if no info was received but timer timed out
            bReturn = false;
        pthread_mutex_unlock( &mutex_newInfo );
#endif
    }
    else
        Log.logWithTime( 2, "already new info waiting" );

    // update the time of the see and the sense messages not yet with the time
    // from the last received messages. This is done to circumvent that a time
    // is updated while the main thread is still determining a new action and
    // the last see message is not yet updated in the world model. This is
    // therefore then in updateAll;

    // reset the indication of new visual information
    bNewInfo = false;
	//cout << bReturn ;
    return bReturn;
}

/*! This method logs all object information that is currently stored in the
    World Model. The output is formatted as follows. First the current time
    (cycle_nr,cycle_stopped) is printed, followed by the object information
    of the specified object 'obj'. The global x and global y position are first
    printed, followed by the global x and global y velocity. Finally the body
    and the neck angle are printed. Then the information of the ball
    "pos_x pos_y vel_x vel_y conf" is printed, followed by the information of
    all eleven teammates "nr pos_x pos_y vel_x vel_y conf" and the same
    information about the eleven opponents. Values that are currently not known
    are replaced by the value -10.0000. This method is normally used by a
    player to log every cycle all its information contained in the world model
    to a file. If the coach (with perfect information) does the same, these two
    files can be analyzed to calculate the error for the different values.
    \param iLogLevel loglevel for which information should be printed
    \param obj object of which information should be printed at start line
               of this object also the body and neck angle are printed. */
void WorldModel::logObjectInformation( int iLogLevel, ObjectT obj )
{

    static char   str[2048];
    double dConf     = PS->getPlayerConfThr();
    sprintf( str, "(%4d,%3d) ", getCurrentTime().getTime(),
             getCurrentTime().getTimeStopped() );
    if ( obj != OBJECT_ILLEGAL )
        sprintf( str, "%12.6f %12.6f %12.6f %12.6f %12.6f %12.6f",
                 getGlobalPosition(obj).getX(),
                 getGlobalPosition(obj).getY(),
                 getGlobalVelocity(obj).getX(),
                 getGlobalVelocity(obj).getY(),
                 getGlobalBodyAngle(obj),
                 getGlobalNeckAngle(obj) );

    if ( getConfidence      ( OBJECT_BALL ) > dConf &&
            getRelativeDistance( OBJECT_BALL ) < 20.0 )
        sprintf( &str[strlen(str)], " %12.6f %12.6f",
                 getGlobalPosition(OBJECT_BALL).getX(),
                 getGlobalPosition(OBJECT_BALL).getY() );
    else
        sprintf( &str[strlen(str)], " %12.6f %12.6f", -10.0, -10.0 );

    if ( getTimeGlobalVelocity( OBJECT_BALL ) > getTimeFromConfidence( dConf ) &&
            getRelativeDistance  ( OBJECT_BALL ) < 20.0 )
        sprintf( &str[strlen(str)], " %12.6f %12.6f",
                 getGlobalVelocity(OBJECT_BALL).getX(),
                 getGlobalVelocity(OBJECT_BALL).getY() );
    else
        sprintf( &str[strlen(str)], " %12.6f %12.6f", -10.0, -10.0 );
    sprintf( &str[strlen(str)], " %12.6f", getConfidence(OBJECT_BALL) );

    int    iIndex=-1;
    int    iIndexPlayer;
    for ( ObjectT o = iterateObjectStart( iIndex, OBJECT_SET_PLAYERS, 0.0 );
            o != OBJECT_ILLEGAL;
            o = iterateObjectNext( iIndex, OBJECT_SET_PLAYERS, 0.0 ) )
    {
        bool bPrint = false;

        iIndexPlayer = (SoccerTypes::isTeammate(o))
                       ? SoccerTypes::getIndex(o) + 1
                       : SoccerTypes::getIndex(o) + 12;
        sprintf( &str[strlen(str)], " %d", iIndexPlayer );
        if ( getConfidence      ( o ) > dConf && isKnownPlayer(o) &&
                getRelativeDistance( o ) < 20.0 )
        {
            sprintf( &str[strlen(str)], " %12.6f %12.6f",
                     getGlobalPosition(o).getX(),
                     getGlobalPosition(o).getY() );
            bPrint = true;
        }
        else
            sprintf( &str[strlen(str)], " %12.6f %12.6f", -10.0, -10.0 );

        if ( getTimeGlobalVelocity( o ) > getTimeFromConfidence( dConf ) &&
                getRelativeDistance  ( o ) < 20.0 && isKnownPlayer(o)  )
        {
            sprintf( &str[strlen(str)], " %12.6f %12.6f",
                     getGlobalVelocity(o).getX(),
                     getGlobalVelocity(o).getY() );
            bPrint = true;
        }
        else
            sprintf( &str[strlen(str)], " %12.6f %12.6f", -10.0, -10.0 );

        if ( bPrint )
            sprintf( &str[strlen(str)], " %12.6f", getConfidence(o) );
        else
            sprintf( &str[strlen(str)], " %12.6f", -10.0 );
    }
    if ( getCurrentCycle() != 3000  )
        Log.log( iLogLevel, str );
}

/*! This method logs drawing information to the log file LogDraw. The drawing
    information is written in the syntax that is understood by the soccer
    monitor. The contents of the created file can afterwards be read by an
    (adapted) logplayer to show the logged information. The information logged
    in this method is all the global position information of the players on the
    field.
    \param iLogLevel loglevel that has to be passed to the Logger.
*/
void WorldModel::logDrawInfo( int iLogLevel  )
{
    if ( getCurrentCycle() % ( SS->getHalfTime() * SS->getSimulatorStep()) == 0 )
        return ;

    bool   bReturn1 = false, bReturn2 = false;
    double dConfThr = PS->getPlayerConfThr();
    int    iIndex;
    char   strMsg1[MAX_MSG], strMsg2[MAX_MSG];
    char   strColorLeft[]  = "ffff00" ;
    char   strColorRight[] = "0000ff" ;
    if ( getSide() != SIDE_LEFT )
    {
        sprintf( strColorLeft, strColorRight );
        sprintf( strColorRight, "ffff00" );
    }


    sprintf( strMsg1, "_2D_ CIRCLE col=%s ", strColorLeft ) ;
    // first each players logs the positions of all the teammates
    for ( ObjectT o = iterateObjectStart( iIndex, OBJECT_SET_TEAMMATES, dConfThr );
            o != OBJECT_ILLEGAL;
            o = iterateObjectNext ( iIndex, OBJECT_SET_TEAMMATES, dConfThr ) )
    {
        bReturn1 = true;

        // add all the global positions to the string
        sprintf( &strMsg1[strlen(strMsg1)], "(%1.2f,%1.2f,1.3) ",
                 m_iMultX*getGlobalPosition( o ).getX(),
                 m_iMultY*getGlobalPosition( o ).getY() );
    }
    iterateObjectDone( iIndex );

    if ( bReturn1 == false )
        sprintf( strMsg1, "_2D_ " );
    else
        strcat( strMsg1, "; " );

    sprintf( strMsg2, "CIRCLE col=%s ", strColorRight ) ;
    for ( ObjectT o = iterateObjectStart( iIndex, OBJECT_SET_OPPONENTS, dConfThr);
            o != OBJECT_ILLEGAL;
            o = iterateObjectNext ( iIndex, OBJECT_SET_OPPONENTS, dConfThr ) )
    {
        bReturn2 = true;

        // add all the global positions to the string
        sprintf( &strMsg2[strlen(strMsg2)], "(%1.2f,%1.2f,1.3) ",
                 m_iMultX*getGlobalPosition( o ).getX(),
                 m_iMultY*getGlobalPosition( o ).getY() );
    }
    iterateObjectDone( iIndex );
    strcat( strMsg2, ";" );

    if ( bReturn2 == true )
        strcat( strMsg1, strMsg2 );
    if ( bReturn1 == true || bReturn2 == true)
        LogDraw.log( iLogLevel, strMsg1 );

    return ;
}

/*! This method logs drawing information to the log file LogDraw. The drawing
    information is written in the syntax that is understood by the soccer
    monitor. The contents of the created file can afterwards be read by an
    (adapted) logplayer to show the logged information. This method logs all
    the information about the coordination graphs.
    \param iLogLevel loglevel that has to be passed to the Logger.
*/
void WorldModel::logCoordInfo( int iLogLevel  )
{
    if ( getCurrentCycle() % ( SS->getHalfTime() * SS->getSimulatorStep()) == 0 )
        return ;

    bool   bReturn = false;
    int    iIndex;
    double dConfThr = PS->getPlayerConfThr();
    char   strMsg[MAX_MSG];

    map<double, ObjectT > mapDistToBall;
    VecPosition posBall = getGlobalPosition( OBJECT_BALL ), posFirst;
    int i = 0;

    for ( ObjectT o = iterateObjectStart( iIndex, OBJECT_SET_TEAMMATES, dConfThr);
            o != OBJECT_ILLEGAL;
            o = iterateObjectNext ( iIndex, OBJECT_SET_TEAMMATES, dConfThr ) )
    {
        // add all the global positions to the string
        mapDistToBall.insert(
            make_pair( getGlobalPosition( o ).getDistanceTo( posBall ), o ) );
    }
    iterateObjectDone( iIndex );

    sprintf( strMsg, "all_2D_ ");
    for (  map<double, ObjectT >::iterator iter = mapDistToBall.begin();
            iter != mapDistToBall.end()  ;
            iter ++ )
    {
        if ( i == 0 )
            posFirst = getGlobalPosition( iter->second );
        else if ( iter->second == getAgentObjectType() && i < 4 &&
                  iter->first < 20.0 )
        {
            bReturn = true;
            sprintf( &strMsg[strlen(strMsg)],
                     "LINE col=%s (%1.2f,%1.2f,%1.2f,%1.2f);",
                     m_colorPlayers[ SoccerTypes::getIndex( getAgentObjectType() ) ],
                     m_iMultX*posFirst.getX(),
                     m_iMultY*posFirst.getY(),
                     m_iMultX*getGlobalPosition( iter->second ).getX(),
                     m_iMultY*getGlobalPosition( iter->second ).getY() );
        }

        i++;
    }

    if ( bReturn == true )
        LogDraw.log( iLogLevel, strMsg );
}


/* This method logs a circle to the specified log level.
   \param iLogLevel log level to which the circle should be printed
   \param pos center position of the circle
   \param dRadius radius of the circle
   \param bAll indication whether it should be printed to specific drawlevel
          or not (default false). */
bool WorldModel::logCircle( int iLogLevel, VecPosition pos, double  dRadius,
                            bool  bAll                                       )
{
    if ( getCurrentCycle() % ( SS->getHalfTime() * SS->getSimulatorStep()) == 0 )
        return false;

    return LogDraw.log( iLogLevel, "%s_2D_ CIRCLE col=%s (%1.2f,%1.2f,%f) ;",
                        ( bAll == true ) ? "all" : "" ,
                        m_colorPlayers[ getAgentIndex() ],
                        m_iMultX*pos.getX(), m_iMultY*pos.getY(), dRadius );
}

/* This method logs a line to the specified log level.
   \param iLogLevel log level to which the circle should be printed
   \param pos1 first point of line
   \param pos2 second point of line
   \param bAll indication whether it should be printed to specific drawlevel
          or not (default false). */
bool WorldModel::logLine( int  iLogLevel, VecPosition pos1, VecPosition pos2,
                          bool bAll              )
{
    if ( getCurrentCycle() % ( SS->getHalfTime() * SS->getSimulatorStep()) == 0 )
        return false;

    return LogDraw.log( iLogLevel,
                        "%s_2D_ LINE col=%s (%1.2f,%1.2f,%1.2f,%1.2f) ;",
                        ( bAll == true ) ? "all" : "" ,
                        m_colorPlayers[ getAgentIndex() ],
                        m_iMultX*pos1.getX(), m_iMultY*pos1.getY(),
                        m_iMultX*pos2.getX(), m_iMultY*pos2.getY() );

}

/* This method logs the ball position and its velocity.

   \param iLogLevel log level to which the ball information should be
   printed.*/
bool WorldModel::logDrawBallInfo( int  iLogLevel )
{
    int  iPoints = 20;
    char strMsg[MAX_MSG];
    char *strColor = "ff0000";

    VecPosition vel     = getGlobalVelocity( OBJECT_BALL );
    VecPosition pos     = getBallPos(), pos1, pos2;
    AngDeg      angOrth = VecPosition::normalizeAngle( vel.getDirection( )+90 );
    VecPosition posBall = getBallPos();
    sprintf( strMsg, "_2D_ CIRCLE col=%s (%1.2f,%1.2f,0.3) ; ",
             strColor, m_iMultX*posBall.getX(), m_iMultY*posBall.getY() );

    for ( int i = 0; i < iPoints; i ++ )
    {
        // create all the orthogonal lines
        pos += vel;
        pos1 = pos + VecPosition( -0.5*vel.getMagnitude(), angOrth, POLAR );
        pos2 = pos + VecPosition(  0.5*vel.getMagnitude(), angOrth, POLAR );
        sprintf( &strMsg[strlen(strMsg)], "LINE col=%s (%1.2f,%1.2f,%1.2f,%1.2f);",
                 strColor, m_iMultX*pos1.getX(), m_iMultY*pos1.getY(),
                 m_iMultX*pos2.getX(), m_iMultY*pos2.getY() );
        vel *= SS->getBallDecay();
    }

    // finally create the line through all orthogonal lines
    sprintf( &strMsg[strlen(strMsg)], "LINE col=%s (%1.2f,%1.2f,%1.2f,%1.2f) ;",
             strColor, m_iMultX*getBallPos().getX(), m_iMultY*getBallPos().getY(),
             m_iMultX*pos.getX(), m_iMultY*pos.getY() );

    return LogDraw.log( iLogLevel, strMsg );
}

/*! This method checks whether the feature of type 'type' is relevant. This is
    done by comparing the time of the feature with the current time. When it
    is equal, the feature is assumed relevant, otherwise irrelevant.
    \param type feature that should be checked on relevance
    \return bool indicating whether the feature is relevant. */
bool WorldModel::isFeatureRelevant( FeatureT type )
{
    int  iIndex  = (int)type;
    bool bReturn = true;
    static Time  timeKickedUsed = -1;

#if 0
    Log.log( 460, "check feature (%d,%d,%d) relevance now (%d,%d,%d)",
             m_features[iIndex].getTimeSee().getTime(),
             m_features[iIndex].getTimeSense().getTime(),
             m_features[iIndex].getTimeHear().getTime(),
             getTimeLastSeeMessage().getTime(),
             getTimeLastSenseMessage().getTime(),
             getTimeLastHearMessage().getTime() );
#endif
    // feature is relevant when see and hear time is larger or equal than the
    // current time
    bReturn  = m_features[iIndex].getTimeSee()  >= getTimeLastSeeMessage() &&
               m_features[iIndex].getTimeHear() >= getTimeLastHearMessage();

    // if ball kicked, also recheck
    if ( timeKickedUsed != getTimeLastSenseMessage() )
    {
        bReturn &= ( m_bPerformedKick == false );
        timeKickedUsed = getTimeLastSenseMessage();
    }

    // in case of interception, also recheck after sense message
    if ( type ==  FEATURE_INTERCEPTION_POINT_BALL ||
            type ==  FEATURE_INTERCEPT_CLOSE        )
        bReturn &= m_features[iIndex].getTimeSense() >= getTimeLastSenseMessage();
    return bReturn;
}

/*! This method return the feature corresponding to the type 'type'.
    \param type type of this feature, e.g., FEATURE_FASTEST_PLAYER_TO_BALL.
    \return corresponding feature.*/
Feature WorldModel::getFeature( FeatureT type )
{
    return m_features[(int)type];
}

/*! This method updates the feature 'type' with the information stored in
    'feature'.
    \param type type of this feature, e.g., FEATURE_FASTEST_PLAYER_TO_BALL.
    \param feature information regarding this feature
    \return boolean indicating whether the update was successful.*/
bool WorldModel::setFeature( FeatureT type, Feature feature )
{
    m_features[(int)type] = feature;
    return true;
}

/*! This method draws a coordination graph between the relevant players
    and logs this information. */
void WorldModel::drawCoordinationGraph( )
{
    bool partOfGraph = false; // is the player part of the coordination graph?
    static bool bFirst = true;
    static ofstream ofile;
    char strTmp[128];
    if ( bFirst == true )
    {
        sprintf( strTmp, "logs/coord%d.txt", getPlayerNumber() );
        ofile.open( strTmp );
        bFirst = false;
    }

    ObjectT objAgent = getAgentObjectType();
    ObjectT objClosestToBall = getFastestInSetTo( OBJECT_SET_TEAMMATES,
                               OBJECT_BALL );
    ObjectT objTeam = OBJECT_ILLEGAL;

    VecPosition vpAgent = getAgentGlobalPosition();
    VecPosition vpTeam = getGlobalPosition( objTeam ); // -1000.0, 1000.0

#if 0
    if ( objClosestToBall == objAgent ) // Agent is closest to ball
        // find the closest player that has a x-coordinate greater than the agent's
    {
        partOfGraph = true;

        int iIndex;
        ObjectSetT set = OBJECT_SET_TEAMMATES;
        for ( ObjectT o = iterateObjectStart( iIndex, set );
                o != OBJECT_ILLEGAL;
                o = iterateObjectNext ( iIndex, set ) )
        {
            VecPosition vpNew = getGlobalPosition( o );
            Log.log( 701, "i am fastest, check %d, last_x %f, his_x %f  \
                           last_rel %f, his_rel  %f",
                     o, vpTeam.getX(), vpNew.getX(),
                     getRelativeDistance( objTeam ),
                     getRelativeDistance( o ) );
            if ( objTeam == OBJECT_ILLEGAL)
            {
                if ( vpNew.getX() > vpAgent.getX() - 5)
                {
                    objTeam = o;
                    vpTeam = vpNew;
                }
            }
            else if ( vpNew.getX() > vpTeam.getX() && o != objAgent &&
                      vpNew.getX() > vpAgent.getX() - 5 )
            {
                if ( getRelativeDistance( o ) < getRelativeDistance( objTeam ) )
                {
                    objTeam = o;
                    vpTeam = vpNew;
                }
            }
        }
        iterateObjectDone( iIndex );
    }
    else // Agent is not closest to ball
    {
    }
#else // Jelle
    objAgent = getAgentObjectType();

    objClosestToBall = getFastestInSetTo( OBJECT_SET_TEAMMATES, OBJECT_BALL );
    VecPosition posFastest   = getGlobalPosition( objClosestToBall );
    objTeam = OBJECT_ILLEGAL;

    vpAgent = getAgentGlobalPosition();
    vpTeam  = getGlobalPosition( objTeam ); // -1000.0, 1000.0

    // serach for closest teammate with x higher than 5 of guy with ball
    int iIndex;
    ObjectSetT set = OBJECT_SET_TEAMMATES;
    for ( ObjectT o = iterateObjectStart( iIndex, set );
            o != OBJECT_ILLEGAL;
            o = iterateObjectNext ( iIndex, set ) )
    {
        VecPosition vpNew = getGlobalPosition( o );
        Log.log( 701, "fastest %d, check %d, last_x %f, his_x %f dist_old %f dist_new %f last_rel %f, his_rel  %f",
                 objClosestToBall, o, vpTeam.getX(), vpNew.getX(),
                 posFastest.getDistanceTo( vpTeam ),
                 posFastest.getDistanceTo( vpNew ),
                 getRelativeDistance( objTeam ),
                 getRelativeDistance( o ) );
        if ( objTeam == OBJECT_ILLEGAL)
        {
            if ( vpNew.getX() > posFastest.getX() - 5 && o != objClosestToBall )
            {
                objTeam = o;
                vpTeam = vpNew;
            }
        }
        else if ( /*vpNew.getX() > vpTeam.getX() && */o != objClosestToBall &&
                vpNew.getX() > posFastest.getX() - 5)
        {
            if ( posFastest.getDistanceTo( vpNew ) <
                    posFastest.getDistanceTo( vpTeam ) )
            {
                objTeam = o;
                vpTeam = vpNew;
            }
        }
    }
    iterateObjectDone( iIndex );
    Log.log( 701, "team %d fastest %d me %d", objTeam, objClosestToBall, objAgent  );
    if ( objAgent == objTeam || objAgent == objClosestToBall )
        partOfGraph = true;
#endif
    if ( partOfGraph == true && getCurrentTime().isStopped() == false )
    {
        Log.log( 701,
                 "create line with me (%d) (%f,%f) and closest team %d (%f,%f)",
                 SoccerTypes::getIndex(getAgentObjectType()) +1,
                 posFastest.getX(),posFastest.getY(),
                 SoccerTypes::getIndex(objTeam) +1,
                 vpTeam.getX(),vpTeam.getY()          );
        logLine( 701, posFastest, vpTeam, true );
        ofile << getCurrentCycle() << ": " <<
        SoccerTypes::getObjectStr( strTmp, objTeam, getTeamName() ) << " "
        << SoccerTypes::getIndex( objTeam ) <<
        endl;
    }
}


//!new add
void   WorldModel::setSynchSeeMode()
{
    bSynchSeeMode = true;
}
bool   WorldModel::isSynchSeeMode()
{
    return  bSynchSeeMode;
}
