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

/*! \file WorldModelUpdate.cpp
<pre>
<b>File:</b>          WorldModelUpdate.cpp
<b>Project:</b>       Robocup Soccer Simulation Team: UvA Trilearn
<b>Authors:</b>       Jelle Kok
<b>Created:</b>       12/02/2001
<b>Last Revision:</b> $ID$
<b>Contents:</b>      class definitions of WorldModel. This class contains
               methods that process and update the information in the world
               model.
<hr size=2>
<h2><b>Changes</b></h2>
<b>Date</b>             <b>Author</b>          <b>Comment</b>
12/02/2001       Jelle Kok       Initial version created
</pre>
*/

#include "WorldModel.h"
#include "Parse.h"
#include <stdio.h>      // needed for sprintf
#include <list>         // needed for list

#include <sys/times.h>  // needed for times
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include <cstdlib>
#include <cstring>

/*****************************************************************************/
/********************* CLASS WORLDMODEL **************************************/
/*****************************************************************************/


/*! This method is called when new visual information about objects on
    the field is received from the see_global message (only the coach
    receives these messages). This method updates the information of the
    Object that corresponds to the ObjectT that is supplied as the first
    argument.
    \param o objectType that should be updated
    \param time time of the corresponding values
    \param pos global position of this object
    \param vel global velocity of this object
    \param angBody body angle of this object
    \param angNeck neck angle of this object */
void WorldModel::processSeeGlobalInfo( ObjectT o, Time time,
                                       VecPosition pos, VecPosition vel, AngDeg angBody, AngDeg angNeck)
{
    DynamicObject * dobj;
    PlayerObject * pobj;

    if ( o == OBJECT_ILLEGAL )
        return;
    if ( SoccerTypes::isPlayer( o ) )
    {
        pobj = (PlayerObject*)getObjectPtrFromType( o );
        pobj->setTimeLastSeen( time );
        pobj->setGlobalPositionLastSee( pos, time );
        pobj->setTimeChangeInformation( time );
        pobj->setGlobalPosition( pos, time );
        pobj->setGlobalVelocity( vel, time );
        pobj->setGlobalBodyAngle( angBody, time );
        pobj->setGlobalNeckAngle( VecPosition::normalizeAngle(angBody+angNeck),
                                  time );
        pobj->setIsKnownPlayer( true );

    }
    else if ( SoccerTypes::isBall( o ) )
    {
        dobj = (DynamicObject*)getObjectPtrFromType( o );
        dobj->setTimeLastSeen( time );
        dobj->setGlobalPosition( pos, time );
        dobj->setGlobalVelocity( vel, time );
    }
}

/*! This method is called when new visual information about the agent
    is received. It updates the information of the AgentObject stored in
    the WorldModel.
    \param vq new ViewQuality for the agent
    \param va new ViewAngle for the agent
    \param dStamina new stamina for the agent
    \param dEffort new effort for the agent
    \param dSpeed magnitude of the velocity
    \param angSpeed angle of the speed velocity (relative to neck)
    \param angHeadAngle new relative angle between body and neck
    \param iTackleExpire number of cycles before tackle expires
    \param iArmMovable indicates whether arm is movable
    \param iArmExpires number of cycles before arm expires
    \param posArm relative position to which arm is pointed
    \return bool indicating whether update was succesful */
bool WorldModel::processNewAgentInfo( ViewQualityT vq, ViewAngleT va,
                                      double dStamina, double dEffort, double dCapacity, double dSpeed, AngDeg angSpeed,
                                      AngDeg angHeadAngle, int iTackleExpires, int iArmMovable,
                                      int iArmExpires, VecPosition posArm )/*! misol.gao fixed for v13.2 */
{
    Stamina sta = agentObject.getStamina();

    sta.setStamina                   ( dStamina                              );
    sta.setEffort                    ( dEffort                               );
    sta.setCapacity( dCapacity );/*! misol.gao fixed for v13.2 */
    agentObject.setStamina           ( sta                                   );
    // this is already done when change_view is sent, and thus updated to the
    // predicted view angle, quality in the next cycle...
    if ( vq != VQ_ILLEGAL )
        agentObject.setViewQuality       ( vq                                  );
    if ( va != VA_ILLEGAL )
        agentObject.setViewAngle         ( va                                  );
    agentObject.setSpeedRelToNeck    ( VecPosition( dSpeed, angSpeed, POLAR) );
    agentObject.setBodyAngleRelToNeck( angHeadAngle                          );
    agentObject.setTackleExpires     ( iTackleExpires                        );
    agentObject.setArmMovable        ( iArmMovable == 0                      );
    agentObject.setArmExpires        ( iArmExpires                           );
    agentObject.setGlobalArmPosition ( getAgentGlobalPosition() + posArm     );

    return true;
}

/*! This method is called when new visual information about other
    objects on the field is received. It updates the information of
    the Object that corresponds to the ObjectT that is supplied as the
    first argument. If some of the arguments are not received with the
    visual information, they are passed with the value
    'UnknownDoubleValue'. Note that the objects are only updated with
    the (relative) information passed as arguments. To make sure all
    global information in the objects is synchronized with the
    relative information, the method updateAll() has to be called
    after all (relative) object information is updated.

    \param o objectType that should be updated
    \param time time of the corresponding values
    \param dDist distance from agent to this object o
    \param iDir angle from agent with this object o (relative to neck)
    \param dDistChange distance change between agent and this object o
    \param dDirChange angle change between agent and this object o
    \param angRelBodyAng angle between neck of agent and body of object o
    \param angRelNeckAng angle between neck of agent and neck of object o
    \param isGoalie bool indicating whether object is a goalie
    \param dPointDir pointing direction of arm of observed player
    \param isTackling bool indicating whether object is tackling
    \return bool indicating whether update was succesful */
void WorldModel::processNewObjectInfo( ObjectT o, Time time,
                                       double dDist, int iDir, double dDistChange, double dDirChange,
                                       AngDeg angRelBodyAng,   AngDeg angRelNeckAng, bool isGoalie,
                                       ObjectT objMin, ObjectT objMax, double dPointDir,  bool isTackling )
{
    if ( dDist == UnknownDoubleValue || o == OBJECT_ILLEGAL )
        return; // no sense to update when only angle is known.

    if ( SoccerTypes::isFlag( o ) )
    {

        Flags[SoccerTypes::getIndex(o)].setRelativePosition(
            dDist,(double)iDir,time);
        Flags[SoccerTypes::getIndex(o)].setTimeLastSeen    ( time                );
        Flags[SoccerTypes::getIndex(o)].setType            ( o                   );
    }
    else if ( SoccerTypes::isPlayer( o ) || SoccerTypes::isBall( o ) )
    {
        DynamicObject *d ;

        // if we do not have all information, update UnknownPlayer array
        if ( !( SoccerTypes::isKnownPlayer( o ) || SoccerTypes::isBall( o ) ) )
        {
            UnknownPlayers[iNrUnknownPlayers].setIsKnownPlayer( false );
            UnknownPlayers[iNrUnknownPlayers].setPossibleRange( objMin, objMax );
            d = &UnknownPlayers[iNrUnknownPlayers];
            iNrUnknownPlayers++;
        }
        else // else update the known object (teammate, opponent, ball)
        {
            d = (DynamicObject*)getObjectPtrFromType( o );
            if ( SoccerTypes::isPlayer( o ) )
                ((PlayerObject*)d)->setIsKnownPlayer( true );
        }

        if ( d != NULL ) // if object was known
        {
            // set all values for this dynamicobject
            d->setRelativePosition( dDist, (double)iDir, time );
            if ( dDistChange != UnknownDoubleValue )
                d->setRelativeDistanceChange( dDistChange, time );
            if ( dDirChange != UnknownDoubleValue )
                d->setRelativeAngleChange( dDirChange, time );
            if ( angRelBodyAng != UnknownAngleValue )
                ((PlayerObject*)d)->setRelativeBodyAngle( angRelBodyAng, time );
            if ( angRelNeckAng != UnknownAngleValue )
                ((PlayerObject*)d)->setRelativeNeckAngle( angRelNeckAng, time );
            if ( isGoalie == true && SoccerTypes::isPlayer( o ))
                ((PlayerObject*)d)->setIsGoalie( true );
            else if ( SoccerTypes::isPlayer( o ))
                ((PlayerObject*)d)->setIsGoalie( false );
            d->setType( o );
            d->setTimeLastSeen( time );

            if ( isTackling )
            {
                // if last observed tackle time has been passed.
                if ( ((PlayerObject*)d)->getTimeTackle() + SS->getTackleCycles()
                        < getCurrentTime() )
                    ((PlayerObject*)d)->setTimeTackle( getCurrentTime() - 1  );
            }
            else
                ((PlayerObject*)d)->setTimeTackle( Time(-1,0)  );

            if ( dPointDir != UnknownDoubleValue )
                ((PlayerObject*)d)->setGlobalArm( dPointDir, getCurrentTime() );

            // check if there wasn't an unknown player located in the worldmodel
            // that corresponded to the same player
            if ( SoccerTypes::isPlayer( o ) && SoccerTypes::isKnownPlayer( o )  )
            {
                int        iIndex;
                ObjectT    objMin = OBJECT_ILLEGAL;
                double     dMinDist = 1000.0, dTmp;
                ObjectSetT set = SoccerTypes::isOpponent( o )
                                 ? OBJECT_SET_OPPONENTS
                                 : OBJECT_SET_TEAMMATES;

                for ( ObjectT obj = iterateObjectStart( iIndex, set );
                        obj != OBJECT_ILLEGAL;
                        obj = iterateObjectNext ( iIndex, set ) )
                {
                    if ( obj != getAgentObjectType() && isKnownPlayer( obj ) == false )
                    {
                        dTmp=(getRelativePosition(obj)-d->getRelativePosition()
                             ).getMagnitude();
                        if ( dTmp < dMinDist )
                        {
                            objMin   = obj;
                            dMinDist = dTmp;
                        }
                    }
                }
                iterateObjectDone( iIndex );
                if ( objMin != OBJECT_ILLEGAL &&
                        dMinDist < PS->getPlayerDistTolerance() &&
                        dMinDist < getMaxTraveledDistance( objMin )  )
                {
                    PlayerObject *pob = (PlayerObject*) getObjectPtrFromType( objMin );
                    pob->setTimeLastSeen( -1 ); // delete the unknown player
                    Log.log( 464, "set time objMin %d to -1 mapped to  player %d %f %f",
                             objMin, o, dMinDist, getMaxTraveledDistance( objMin )  );
                }
                else
                    Log.log( 464, "don't set time objMin %d to -1  player %d dist %f",
                             objMin, o, dMinDist  );

            }
        }
    }
    else if ( SoccerTypes::isLine( o ) )
    {
        // angle returned is angle of neck angle with line, convert to angle
        // of neck with orthogonal to line
        iDir = ( iDir < 0 ) ? (90 + iDir ) : - (90 - iDir );

        Lines[SoccerTypes::getIndex(o)].setRelativePosition(
            dDist,(double)iDir,time);
        Lines[SoccerTypes::getIndex(o)].setTimeLastSeen( time );
        Lines[SoccerTypes::getIndex(o)].setType( o );
    }
}

/*! This method stores a message communicated by an other player in the world
    model. It is not processed yet. This is done by the updateAll method.
    \param iPlayer player who communicated
    \param strMsg string that was communicated
    \param iDiff difference between cycles message was communicated and current
    time
    \return bool indicating whether all values were stored successfully. */
bool WorldModel::storePlayerMessage( int iPlayer, char *strMsg, int iCycle )
{
    strcpy( m_strPlayerMsg, strMsg );
    m_iCycleInMsg = iCycle;
    m_timePlayerMsg  = getCurrentTime();
    m_iMessageSender = iPlayer;
    return true;
}

/*! This method processes a communication message from a teammate. */
bool WorldModel::processPlayerMessage( )
{
    static char strMessage[MAX_MSG];                      // location for message
    int         iDiff = getCurrentCycle() - m_iCycleInMsg;// time difference
    double      dDiff = (double)iDiff/100.0;              // conf difference
    char        *strMsg;
    strcpy( strMessage, m_strPlayerMsg );
    strMsg = strMessage;                                  // pointer to work with

    char cOffside = strMsg[1];                        // read offside information
    if ( cOffside >= 'a' && cOffside <= 'z' )         // a-z corresponds to 0-25
    {
        m_dCommOffsideX    = (double)(cOffside - 'a');
        m_timeCommOffsideX = getCurrentTime() - 1;
    }
    else if ( cOffside >= 'A' && cOffside <= 'Z' )    // A-Z corresponds to 26-52
    {
        m_dCommOffsideX = 26 + (double)(cOffside - 'A');
        m_timeCommOffsideX = getCurrentTime() - 1;
    }
    else                                              // wrong message
        return false;

    if ( strMsg[2] >= '0' && strMsg[2] <= '9' &&      // received ball info
            strlen( strMessage ) == 12)
    {
        // translate ball position nd velocity back to initial range.
        double dBallX =    (int)(strMsg[2]-'0')*10 +(int)(strMsg[3] - '0' ) - 48.0;
        double dBallY =    (int)(strMsg[4]-'0')*10 +(int)(strMsg[5] - '0' ) - 34.0;
        double dBallVelX = (int)(strMsg[6]-'0') + (int)(strMsg[7] - '0' )/10.0-2.7;
        double dBallVelY = (int)(strMsg[8]-'0') + (int)(strMsg[9] - '0' )/10.0-2.7;

        VecPosition pos( dBallX, dBallY );
        VecPosition vel( dBallVelX, dBallVelY );
        for ( int i = 0; i < iDiff ; i ++ )
        {
            pos += vel;
            vel *= SS->getBallDecay();
        }

        // if ball not seen or felt for three cycles, update ball information
        if ( getTimeLastSeen( OBJECT_BALL ) == -1 ||
                (
                    getTimeChangeInformation( OBJECT_BALL ) < getCurrentTime() - 3 &&
                    getRelativeDistance( OBJECT_BALL ) > SS->getVisibleDistance()
                ) ||
                (
                    getTimeChangeInformation( OBJECT_BALL ) < getCurrentTime() - iDiff &&
                    vel.getDistanceTo( getGlobalVelocity(OBJECT_BALL) ) > 0.3 &&
                    getRelativeDistance( OBJECT_BALL ) > SS->getVisibleDistance()
                )
           )
        {
            Log.log( 600,
                     "update ball comm. (%1.2f,%1.2f)(%1.2f,%1.2f) diff %d, last %d",
                     pos.getX(), pos.getY(), vel.getX(), vel.getY(), iDiff,
                     getTimeLastSeen( OBJECT_BALL ).getTime() );
            Log.log( 601, "update ball from comm (%1.2f,%1.2f)(%1.2f,%1.2f) diff %d",
                     pos.getX(), pos.getY(), vel.getX(), vel.getY(), iDiff );
            processPerfectHearInfoBall( pos, vel, 1.00 - dDiff - 0.01 )    ;
        }
        else
            Log.log( 600, "do not update ball time_change %d, now %d, diff %d, d %f",
                     getTimeChangeInformation( OBJECT_BALL ).getTime(),
                     getCurrentCycle(),
                     iDiff,
                     vel.getDistanceTo( getGlobalVelocity(OBJECT_BALL) ) );
    }
    else if ( strlen( strMessage ) == 12 &&           // received attacker info
              strMsg[2] >= 'a' && strMsg[2] <= 'a' + 10 &&
              strMsg[6] == ' ' )
    {
        ObjectT objOpp
        = SoccerTypes::getOpponentObjectFromIndex((int)(strMsg[2]-'a'));
        char   *str  = &strMsg[3];                      // get pointer to string
        double dOppX = -1*Parse::parseFirstInt( &str );
        dOppX /= 10.0;
        double dOppY = Parse::parseFirstInt( &str );
        dOppY /= 10.0;
        VecPosition posOpp( dOppX, dOppY );
        processPerfectHearInfo( objOpp, posOpp, 0.99, false )  ;
    }

    return true;
}

bool WorldModel::processRecvThink( bool b )
{
    m_bRecvThink = b;
    if ( b == true && SS->getSynchMode() == true )
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

/*! This method is called when new information about the ball is heard.
    It updates the information of the ball only when the confidence is higher
    than the information already available in the WorldModel.
    \param posGlobal global position of the ball
    \param vel global velocity of the ball
    \param dConf confidence in the ball information
    \return true when information is updated, false otherwise */
bool WorldModel::processPerfectHearInfoBall( VecPosition posGlobal,
        VecPosition vel, double dConf )
{
    Log.log( 501, "ball conf: %f %d", getConfidence( OBJECT_BALL ),
             getTimeLastSeen( OBJECT_BALL ).getTime() );
    if ( Ball.getConfidence( getCurrentTime() ) < dConf ||
            vel.getDistanceTo( getGlobalVelocity(OBJECT_BALL) ) > 0.3  )
    {
        Time time = getTimeFromConfidence( dConf );
        Ball.setGlobalPosition( posGlobal, time );
        Ball.setGlobalVelocity( vel,       time );
        Ball.setTimeLastSeen  (            time );
        updateObjectRelativeFromGlobal( OBJECT_BALL );
        setTimeLastHearMessage( getCurrentTime() );
        return true;
    }
    return false;
}

/*! This method is called when new information about an object is heard. But
    only when the player who said the message was completely sure about the
    object type (it would not give information about a player it does not know
    the player number of, then it would call processUnsureHearInfo).
    It updates the information of the specified object only when the confidence
    is higher than the information already available in the WorldModel.
    \param o object type to which this information relates to
    \param posGlobal global position of this object type.
    \param dConf confidence in the object information.
    \param bIsGoalie boolean indicating whether 'o' is a goalie
    \return true when information is updated, false otherwise */
bool WorldModel::processPerfectHearInfo( ObjectT o, VecPosition posGlobal,
        double dConf, bool bIsGoalie )
{
    if ( SoccerTypes::isBall( o ) || o == getAgentObjectType() )
        return false; // ball should be called with processPerfectHearInfoBall
    else if ( !SoccerTypes::isKnownPlayer( o ) )
        return processUnsureHearInfo( o, posGlobal, dConf );

    PlayerObject *object = (PlayerObject *)getObjectPtrFromType( o );
    if ( object == NULL )
        return false;

    Time time = getTimeFromConfidence( dConf ) ;

    // if we are not sure about the exact player number of this player in
    // the world model (getIsKnownPlayer() == false) we overwrite the
    // information of this player since the player who said this information
    // is sure about it (otherwise processUnsureHearInfo would be called instead
    // of processPERFECTHearInfo)
    if ( object->getConfidence( getCurrentTime() ) < dConf ||
            object->getIsKnownPlayer() == false  )
    {
        object->setGlobalPosition     ( posGlobal         , time );
        object->setTimeLastSeen       ( time                     );
        object->setGlobalVelocity     ( VecPosition( 0, 0), time );
        object->setIsKnownPlayer      ( true                     );
        object->setIsGoalie           ( bIsGoalie                );
        updateObjectRelativeFromGlobal( o                        );
        setTimeLastHearMessage( getCurrentTime() );
        return true;
    }
    return false;
}

/*! This method is called when new information about an object is heard. But
    only when the player who said the message was not sure about the
    object type. It is tried to map the given information to an object already
    in the WorldModel. If we cannot find such a player, we add the player info
    at the position of the first player who we don't have information about.
    The information of the specified object is only updated when the confidence
    is higher than the information already available in the WorldModel.
    \param o object type to which this information relates to
    \param pos global position of this object type.
    \param dConf confidence in the object information.
    \return true when information is updated, false otherwise */
bool WorldModel::processUnsureHearInfo( ObjectT o, VecPosition pos,
                                        double dConf )
{
    double     dMinDist;        // used to find closest player to pos
    ObjectT    objInitial = o;

    if ( o != OBJECT_TEAMMATE_UNKNOWN && o != OBJECT_OPPONENT_UNKNOWN )
        return false;

    // if o is a teammate find closest teammate to pos and store distance
    if ( SoccerTypes::isTeammate( o ) )
        o = getClosestInSetTo( OBJECT_SET_TEAMMATES, pos, &dMinDist);
    else if ( SoccerTypes::isOpponent( o ) ) // if o is an opponent, do the same
        o = getClosestInSetTo( OBJECT_SET_OPPONENTS, pos, &dMinDist);

    if ( o == getAgentObjectType() &&
            pos.getDistanceTo(getAgentGlobalPosition())<PS->getPlayerDistTolerance())
        return false;  // do not update my own position, localization is better

    // if opponent or teammate was found and distance lies in tolerance distance
    //  update this opponent or teammate with the specified information.
    // else put the information in the first player position of which we have
    //  no information.
    else if ( SoccerTypes::isKnownPlayer(o) &&
              dMinDist < PS->getPlayerDistTolerance())
    {
        processPerfectHearInfo( o, pos, dConf );
        return true;
    }

    if ( objInitial == OBJECT_TEAMMATE_UNKNOWN )
        o = getFirstEmptySpotInSet( OBJECT_SET_TEAMMATES );
    else if ( objInitial == OBJECT_OPPONENT_UNKNOWN )
        o = getFirstEmptySpotInSet( OBJECT_SET_OPPONENTS );
    else
        return false ;  // in case of OBJECT_PLAYER_UNKNOWN

    if ( o != OBJECT_ILLEGAL )  // can be the case that there is no empty spot
    {
        processPerfectHearInfo( o, pos, dConf );
        setIsKnownPlayer( o, false );
    }
    return true;
}

/*! This methods fills the heterogeneous player array (stored as 'pt') with the
    specified information. This information is directly parsed from the
    player_type message received from the server. This method is therefore
    only called from the SenseHandler. The information in this array can later
    be used (by the coach) to determine the best heterogeneous player for a
    specific position on the field and to update the parameters in the
    ServerSettings when the player type of the agent is changed.
    \param iIndex index of the heterogeneous player as indicated by the server
    \param dPlayerSpeedMax maximum speed of the player
    \param dStaminaIncMax maximum increase of stamina for the player
    \param dPlayerDecay amount of velocity decay of the player
    \param dIntertiaMoment indication how fast is turned when moving
    \param dDashPowerRate how is power in dash command converted into speed
    \param dPlayerSize what is the size of the player
    \param dKickableMarng in which area can player still kick the ball
    \param dKickRand how much noise is added to kick command
    \param dExtraStamina how much extra stamina does player receive
    \param dEffortMax what is the maximum effort (effective dash)
    \param dEffortMin what is the minimum effort
    \return bool indicating whether update was succesfull. */
bool WorldModel::processNewHeteroPlayer( int iIndex,    double dPlayerSpeedMax,
        double dStaminaIncMax, double dPlayerDecay, double dInertiaMoment,
        double dDashPowerRate, double dPlayerSize,  double dKickableMargin,
        double dKickRand,      double dExtraStamina,double dEffortMax,
        double dEffortMin )
{
    pt[iIndex].dPlayerSpeedMax  = dPlayerSpeedMax;
    pt[iIndex].dStaminaIncMax   = dStaminaIncMax;
    pt[iIndex].dPlayerDecay     = dPlayerDecay;
    pt[iIndex].dInertiaMoment   = dInertiaMoment;
    pt[iIndex].dDashPowerRate   = dDashPowerRate;
    pt[iIndex].dPlayerSize      = dPlayerSize;
    pt[iIndex].dKickableMargin  = dKickableMargin;
    pt[iIndex].dKickRand        = dKickRand;
    pt[iIndex].dExtraStamina    = dExtraStamina;
    pt[iIndex].dEffortMax       = dEffortMax;
    pt[iIndex].dEffortMin       = dEffortMin;
    pt[iIndex].dMaximalKickDist = dKickableMargin +
                                  dPlayerSize +
                                  SS->getBallSize();

    return true;
}

/*! This method sets the catch cycle time for the goalie. This is the
    cycle time that the ball is catched by the goalie. This
    information is said by the referee by indicating the side that
    catched the ball. This method checks whether our side catched the
    ball. If this is the case, the time is set and the goalie cannot
    catch the ball for a certain cycles (see ServerSettings).

    \param rm referee message to indicated side of goalie that catched ball
    \param iTime time the ball was catched.  */
void WorldModel::processCatchedBall( RefereeMessageT rm, Time time )
{
    if ( rm == REFC_GOALIE_CATCH_BALL_LEFT && sideSide == SIDE_LEFT )
        timeLastCatch = time;
    else if ( rm == REFC_GOALIE_CATCH_BALL_RIGHT && sideSide == SIDE_RIGHT )
        timeLastCatch = time;
    Ball.setGlobalVelocity( VecPosition(0,0), getCurrentTime() );
}

/*! This method sets the performed commands by the agent object. Using this
    information, the future world states can be calculated when an update is
    performed. In this method a timestamp of the current time cycle is added
    to the commands structure for later usage.
    This method is called by the ActHandler when it has sent these commands to
    the server.
    \param commands all the commands that were sent in this cycle
    \param iCommands number of commands that were sent in this cycle. */
void WorldModel::processQueuedCommands( SoccerCommand commands[],
                                        int iCommands )
{
    if ( iCommands > CMD_MAX_COMMANDS )
    {
        cerr << "(WorldModel::setQueuedCommands) queuedCommands array cannot "
             << "contain so many commands!\n";
        return;
    }

    // put all sent commands to the array which stores queued commands.
    for ( int i = 0 ; i < iCommands ; i ++ )
    {
        commands[i].time                             = getCurrentTime();
        queuedCommands[(int)commands[i].commandType] = commands[i];
    }
}

/*! This is called to update the WorldModel. It determines the the last
    message (see or sense) and updates the worlmodel accordingly. When see
    information is received ("perfect" information) all objects are updated
    with this received information. After sense message the information of
    the object is calculated for the new cycle using the last visual
    information.
    \return bool to indicate whether update succeeded. */
bool WorldModel::updateAll( )
{
    static Timing timer;
    double dTimeSense = 0.0, dTimeSee = 0.0, dTimeComm=0.0, dTimeFastest = 0.0;
    static struct tms times1, times2;

    bool        bReturn            = false, bUpdateAfterSee = false;
    bool        bUpdateAfterSense  = false, bDebug = false;
    static Time timeLastHoleRecorded;
    static Time timeBeginInterval;
    static Time timePlayersCounted;
    static int  iNrHolesLastTime   = 0;
    static Time timeLastSenseUpdate;
    static Time timeLastSeeUpdate;
    static Time timeLastSayUpdate;
    if ( bDebug )
    {
        timer.restartTime();
        times( &times1 );
    }

    // check if last update of agent was not more than one cycle ago
    if ( agentObject.getTimeGlobalPosition() < getCurrentTime() - 1  )
        Log.log( 3, "(WorldModel::updateAll) missed a sense??" );

    // call update method depending on last received message
    if ( isFullStateOn( ) == true )
    {
        Log.log( 4, "full state is on" );
        updateRelativeFromGlobal();
        timeLastSenseMessage = timeLastRecvSeeMessage;
        timeLastSeeMessage   = timeLastRecvSeeMessage;
        bUpdateAfterSee = bUpdateAfterSense = false;
        bReturn = true;
    }
    else
    {
        Log.log( 4, "full state is off" );
        if ( getTimeLastRecvSeeMessage() > timeLastSeeUpdate )
            bUpdateAfterSee = true;
        if ( getTimeLastRecvSenseMessage() > timeLastSenseUpdate )
            bUpdateAfterSense = true;
    }

    // rare situation: can occur that see arrives between sense and calling
    // updateAll or sense arrives between see and calling updateAll.
    if ( bUpdateAfterSee && bUpdateAfterSense )
    {
        Log.log( 3, "!!! Two updates !!! " );
        Log.log( 3, "see: %d sense: %d", getTimeLastRecvSeeMessage().getTime(),
                 getTimeLastRecvSenseMessage().getTime() );
        if ( getTimeLastRecvSeeMessage( ) == getTimeLastRecvSenseMessage() )
        {
            Log.log( 3, "update sense" );
            timeLastSenseMessage = timeLastRecvSenseMessage;
            bReturn  = updateAfterSenseMessage( );
            if ( bDebug ) dTimeSense = timer.getElapsedTime(1000);
            updateRelativeFromGlobal();
            Log.log( 3, "update see" );
            timeLastSeeMessage = timeLastRecvSeeMessage;
            bReturn &= updateAfterSeeMessage( );
            if ( bDebug ) dTimeSee = timer.getElapsedTime(1000) - dTimeSense;
        }
        else if ( getTimeLastRecvSeeMessage( ) < getTimeLastRecvSenseMessage() )
        {
            Log.log( 3, "update see" );
            timeLastSeeMessage = timeLastRecvSeeMessage;
            bReturn  = updateAfterSeeMessage( );
            if ( bDebug ) dTimeSee = timer.getElapsedTime(1000);
            Log.log( 3, "update sense" );
            timeLastSenseMessage = timeLastRecvSenseMessage;
            bReturn &= updateAfterSenseMessage( );
            updateRelativeFromGlobal();
            if ( bDebug ) dTimeSense = timer.getElapsedTime(1000) - dTimeSee;
        }
        timeLastSenseUpdate = getTimeLastSenseMessage();
        timeLastSeeUpdate   = getTimeLastSeeMessage();
    }
    else if ( bUpdateAfterSee )                       // process see message
    {
        Log.log( 3, "update see" );
        timeLastSeeMessage = timeLastRecvSeeMessage;
        bReturn           = updateAfterSeeMessage( );
        timeLastSeeUpdate = getTimeLastSeeMessage();
        if ( bDebug ) dTimeSee = timer.getElapsedTime(1000);
    }
    else if ( bUpdateAfterSense )                     // process sense message
    {
        Log.log( 3, "update sense" );
        timeLastSenseMessage = timeLastRecvSenseMessage;
        bReturn             = updateAfterSenseMessage( );
        timeLastSenseUpdate = getTimeLastSenseMessage();
        updateRelativeFromGlobal();
        if ( bDebug ) dTimeSense = timer.getElapsedTime(1000);
    }

    if ( timeLastSayUpdate != m_timePlayerMsg &&
            isFullStateOn() == false        )            // process communication msg
    {
        Log.log( 3, "update hear" );
        if ( bDebug ) dTimeComm = timer.getElapsedTime(1000);
        timeLastSayUpdate = m_timePlayerMsg;
        processPlayerMessage();
        if ( bDebug ) dTimeComm = timer.getElapsedTime(1000) - dTimeComm;
    }

    SoccerCommand soc = getChangeViewCommand( );
    if ( ! soc.isIllegal() )
    {
        setAgentViewAngle( soc.va );
        setAgentViewQuality( soc.vq );
    }

    // check for holes
    if ( isQueuedActionPerformed() == false &&
            timeLastHoleRecorded != getCurrentTime() &&
            isFullStateOn() == false )
    {
        Log.log( 2, "HOLE recorded" );
        timeLastHoleRecorded = getCurrentTime();
        iNrHoles++;
    }

    // determine number of holes in last time interval and act accordingly
    int    iTimeDiff = getCurrentTime() - timeBeginInterval;
    double dHolePerc = (double)(iNrHoles - iNrHolesLastTime)/iTimeDiff*100;
    if ( ! isLastMessageSee( ) && iTimeDiff % 400 == 0 && dHolePerc > 1.0 &&
            PS->getFractionWaitSeeEnd() > 0.70 )
    {
        PS->setFractionWaitSeeEnd( PS->getFractionWaitSeeEnd() - 0.05 );
        timeBeginInterval = getCurrentTime();
        cerr << getCurrentCycle() << ": lowered send time to " <<
             PS->getFractionWaitSeeEnd() << " for player number "   <<
             getPlayerNumber()           <<
             "; nr of holes is "<< dHolePerc << "%" << "( " << iNrHoles << ", "
             << iNrHolesLastTime << ")" << endl;
        iNrHolesLastTime   = iNrHoles;
    }

    // store some statistics about number of players seen each cycle
    if ( bUpdateAfterSense == true  && ! isTimeStopped() &&
            getCurrentTime() != timePlayersCounted )
    {
        iNrTeammatesSeen += getNrInSetInRectangle( OBJECT_SET_TEAMMATES );
        iNrOpponentsSeen += getNrInSetInRectangle( OBJECT_SET_OPPONENTS );
        timePlayersCounted = getCurrentTime();
    }

    // log specific information when log level is set
    if ( Log.isInLogLevel( 456 ) )
        logObjectInformation( 456, getAgentObjectType() );

    if ( bUpdateAfterSee == true )
        Log.logWithTime(3, "  finished update_all see; start determining action" );
    if ( bUpdateAfterSense == true )
        Log.logWithTime(3, "  finished update_all sense;start determining action");

    if ( Log.isInLogLevel( 459 ) )
    {
        Log.log( 459, "%s%s", strLastSeeMessage, strLastSenseMessage );
        show( OBJECT_BALL, Log.getOutputStream() );
        show( OBJECT_SET_PLAYERS, Log.getOutputStream() );
    }
    if ( ( Log.isInLogLevel( 101 ) && getRelativeDistance( OBJECT_BALL ) < 2.0 ) )
        show( OBJECT_BALL, Log.getOutputStream() );
    if ( Log.isInLogLevel( 556 ) &&
            getRelativeDistance( OBJECT_BALL ) < SS->getVisibleDistance() )
    {
        Log.log( 556, "%s", strLastSeeMessage );
        show( OBJECT_SET_PLAYERS, Log.getOutputStream() );
        show( OBJECT_BALL, Log.getOutputStream() );
    }
    if ( LogDraw.isInLogLevel( 460 ) )
    {
        int iCycles;
        dTimeFastest = timer.getElapsedTime( 1000 );
        ObjectT obj = getFastestInSetTo(OBJECT_SET_OPPONENTS,OBJECT_BALL,&iCycles);
        logCircle( 460, getGlobalPosition( obj ), 1.5 );
        logCircle( 460, predictPosAfterNrCycles( OBJECT_BALL, iCycles ), 0.5 );
        obj=getFastestInSetTo(OBJECT_SET_TEAMMATES_NO_GOALIE,OBJECT_BALL,&iCycles);
        logCircle( 460, getGlobalPosition( obj ), 1.5 );
        logCircle( 460, predictPosAfterNrCycles( OBJECT_BALL, iCycles ), 0.75 );
        dTimeFastest = timer.getElapsedTime( 1000 ) - dTimeFastest;
    }

    if ( LogDraw.isInLogLevel( 701 ) )
        drawCoordinationGraph( );

    logLine( 602, VecPosition( getOffsideX(), -PITCH_WIDTH/2.0),
             VecPosition( getOffsideX(),  PITCH_WIDTH/2.0) );
    if ( bDebug )
    {
        Log.logWithTime( 461, "time update all: %f\n time comm:        %1.5f\n\
     time see:       %1.5f\n time sense:    %1.5f\n time fastest:     %1.5f\n\
     time rest:      %1.5f\n utime:         %1.5f",
                         timer.getElapsedTime()*1000, dTimeComm, dTimeSee, dTimeSense,dTimeFastest,
                         timer.getElapsedTime()*1000-(dTimeComm+dTimeSee+dTimeSense+dTimeFastest ),
                         times2.tms_utime  - times1.tms_utime );
    }
    return bReturn;
}

/*****************************************************************************/
/*************** WORLDMODEL: SEE RELATED UPDATES *****************************/
/*****************************************************************************/

void  WorldModel::processLastSeeMessage( )
{
    ObjectT o;
    double  dDist, dDistChange,    dDirChange,    dPointDir,    dTemp;
    int     iDir;
    AngDeg  angBodyFacingDir, angHeadFacingDir;
    Time    time = getTimeLastSeeMessage();
    bool    isGoalie, isTackling;
    static char strTmp[MAX_MSG];
    char   *strMsg = strLastSeeMessage ;
    Parse::parseFirstInt( &strMsg );         // get the time

    ObjectT objMin     = OBJECT_ILLEGAL;
    ObjectT objMax     = OBJECT_ILLEGAL;

    while ( *strMsg != ')' )                         // " ((objname.." or ")"
    {
		//ofstream out ( "kk.txt", ios_base::app );
		//out << strMsg << getCurrentCycle()<<"$$$"<< endl;
        dDist = dDistChange = dDirChange = dTemp = dPointDir = UnknownDoubleValue;
        angBodyFacingDir = angHeadFacingDir =         UnknownAngleValue;
        strMsg += 2;          // get the object name
        isTackling = false;

        // get the object name from the first part of the string
//    cerr << "string: |" << strMsg << endl;
        o = SoccerTypes::getObjectFromStr( &strMsg, &isGoalie, getTeamName() );

        if ( o == OBJECT_ILLEGAL )
        {
            Log.log( 4, "Illegal object in: %s", strLastSeeMessage );
            Log.log( 4, "rest of message: %s", strMsg );
        }
        else if ( SoccerTypes::isKnownPlayer( o ) )   // we know the player
            objMin = o;
        else if ( SoccerTypes::isPlayer( o ) )        // and thus an unknown player
        {
            if ( SoccerTypes::isTeammate( o ) &&
                    ( objMin == OBJECT_ILLEGAL || SoccerTypes::isOpponent( objMin ) ) )
                objMin = OBJECT_TEAMMATE_1 ;
            else if ( SoccerTypes::isOpponent( o )  &&
                      ( objMin == OBJECT_ILLEGAL || SoccerTypes::isTeammate( objMin ) ) )
                objMin = OBJECT_OPPONENT_1 ;
            else if ( objMin == OBJECT_ILLEGAL )
                objMin = (getSide() == SIDE_LEFT ) ? OBJECT_TEAMMATE_1
                         : OBJECT_OPPONENT_1 ;
            else if ( objMin == OBJECT_TEAMMATE_11 )
                objMin = OBJECT_OPPONENT_1;
            else if ( objMin == OBJECT_OPPONENT_11 )
                objMin = OBJECT_TEAMMATE_1;
            else
                objMin = (ObjectT)((int)objMin + 1);

            if ( objMin == getAgentObjectType() )
            {
                if ( objMin == OBJECT_TEAMMATE_11 )
                    objMin = OBJECT_OPPONENT_1;
                else if ( objMin == OBJECT_OPPONENT_11 )
                    objMin = OBJECT_TEAMMATE_1;
                else
                    objMin = (ObjectT)((int)objMin + 1);
            }
            strcpy( strTmp, strMsg );                  // get the maximum object by
            objMax = getMaxRangeUnknownPlayer( objMin, strTmp ); // look to next obj
        }

		//out << strMsg << "%%%" << endl;
        dTemp = Parse::parseFirstDouble( &strMsg );   // parse first value
        if ( *strMsg == ')' )                         // if it was the only value
            iDir = (int)dTemp;                          // it was the direction
        else
        {
			//out << strMsg << "!!!" << endl;
            dDist = dTemp;                              // else it was distance
            iDir = Parse::parseFirstInt( &strMsg );     // and have to get direction

			//out << strMsg << "*******" << endl;
            double dValues[7];
            int    i = 0;
            if ( *strMsg == ' ' )                       // stil not finished
            {                                           // get dist and dir change
                while ( *strMsg != ')' && *(strMsg+1) != 't' && *(strMsg+1) != 'k' && i < 7 )/*! misol.gao fixed for v13.2 */
                {
					if ( 'a' <= *(strMsg+1) && *(strMsg+1) <= 'z' )/*! misol.gao fixed for v13.2 */
						cout << endl << "new param accer:" << *(strMsg+1) << endl;
                    dValues[i]  = Parse::parseFirstDouble( &strMsg );
                    i++;
                }
            }
			//out << strMsg <<"MMMMMMMMMMMMM!"<< endl;
            switch ( i )
            {
            case 0:
                break;
            case 1:
                dPointDir   = dValues[0];
                break;
            case 5:
                dPointDir   = dValues[4]; // fall through
            case 4:
                angBodyFacingDir = dValues[2];
                angHeadFacingDir = dValues[3]; // fall through
            case 2:                        // in case of ball
                dDistChange = dValues[0];
                dDirChange  = dValues[1];
                break;
            default:
                cerr << getCurrentCycle()<< "(WorldModelUpdate::analyzeSee) wrong param nr " << i << strMsg << endl;;

            }

            if ( *(strMsg+1) == 't')
                isTackling = true;
        }

        // go to end object information
        // skip ending bracket of object information.
        Parse::gotoFirstOccurenceOf( ')', &strMsg );
        strMsg++;

        if ( SoccerTypes::isPlayer( o ) && !SoccerTypes::isKnownPlayer( o ) )
        {
            if ( objMin != OBJECT_ILLEGAL )
            {
                if ( objMin == objMax )
                {
                    Log.log( 459, "range, only %d left", objMin );
                    o = objMin;
                }
                else if ( SoccerTypes::isTeammate( objMin ) &&
                          SoccerTypes::isTeammate( objMax ) )
                    o = OBJECT_TEAMMATE_UNKNOWN;
                else if ( SoccerTypes::isOpponent( objMin ) &&
                          SoccerTypes::isOpponent( objMax ) )
                    o = OBJECT_OPPONENT_UNKNOWN;
            }
        }

        // process the parsed information (unread values are Unknown...)
        processNewObjectInfo( o, time, dDist, iDir, dDistChange,
                              dDirChange, angBodyFacingDir, angHeadFacingDir,
                              isGoalie, objMin, objMax, dPointDir, isTackling );
    }
}

/*! This method is called to update the WorldModel after a see message.
    The update methods for the agent, all teammates, all opponents and the
    ball are called. But only when the last see message of these objects equals
    the time of the last received see message.
    \return bool to indicate whether update succeeded. */
bool WorldModel::updateAfterSeeMessage( )
{
    processLastSeeMessage( );

    // update the agent (global position and angle using flags and lines)
    if ( getCurrentTime().getTime() != -1 )
        updateAgentObjectAfterSee( );

    mapUnknownPlayers( getTimeLastSeeMessage() ); // map players with unknown nr

    // walk past all players on the field an when new information was perceived
    // (and put in the relative attributes) update this dynamic object. When it
    // was not seen, convert its global position (this is an estimate from the
    // sense message) to a relative position
    double dConfThr = PS->getPlayerConfThr();
    int    iIndex;

    for ( ObjectT o = iterateObjectStart( iIndex, OBJECT_SET_PLAYERS, dConfThr );
            o != OBJECT_ILLEGAL;
            o = iterateObjectNext ( iIndex, OBJECT_SET_PLAYERS, dConfThr ) )
    {
        if ( getTimeLastSeen( o ) == getTimeLastSeeMessage() &&
                o != getAgentObjectType()  )
            updateDynamicObjectAfterSee   ( o );
        else
            updateObjectRelativeFromGlobal( o );
    }
    iterateObjectDone( iIndex );

    // if ball was seen update him, otherwise make estimated global relative
    if ( getTimeLastSeen( OBJECT_BALL ) == getTimeLastSeeMessage() )
        updateDynamicObjectAfterSee   ( OBJECT_BALL );
    else
        updateObjectRelativeFromGlobal( OBJECT_BALL );

    // delete objects from wordmodel that should have been seen, but aren't
    if ( Log.isInLogLevel( 2 ) )
        show( OBJECT_BALL, Log.getOutputStream() );
    removeGhosts();
    if ( Log.isInLogLevel( 2 ) )
        show( OBJECT_BALL, Log.getOutputStream() );


    return true;
}

/*! This method updates an agent after a see message.
    The global position and the global neck angle are calculated using the
    visual information using the method calculateStateAgent.
    \return bool to indicate whether update succeeded. */
bool WorldModel::updateAgentObjectAfterSee(  )
{
    VecPosition posGlobal, velGlobal;
    AngDeg      angGlobal;

    // calculate the state of the agent
    calculateStateAgent( &posGlobal, &velGlobal, &angGlobal );

    // and set the needed attributes
    agentObject.setTimeLastSeen         ( getTimeLastSeeMessage() );
    // store difference with predicted global position to compensate for error
    // in global position when global velocity is calculated for other objects.
    agentObject.setPositionDifference(posGlobal-agentObject.getGlobalPosition());
    agentObject.setGlobalPosition       ( posGlobal, getTimeLastSeeMessage() );
    agentObject.setGlobalPositionLastSee( posGlobal, getTimeLastSeeMessage() );
    agentObject.setGlobalNeckAngle      ( angGlobal );
    agentObject.setGlobalVelocity       ( velGlobal, getTimeLastSeeMessage() );

    return true;
}


/*! This method updates a dynamic object after a see message.
    The global position and the velocity are calculated using the
    visual information. For the ball the method 'calculateStateBall'
    is called, for players the method 'calculateStatePlayer' is called.
    \param o object that should be updated after a see message */
bool WorldModel::updateDynamicObjectAfterSee( ObjectT o )
{
    VecPosition posGlobal, velGlobal;

    if ( o == OBJECT_BALL )
    {
        calculateStateBall           ( &posGlobal, &velGlobal              );
        Ball.setGlobalVelocity       ( velGlobal,  getTimeLastSeeMessage() );
        Ball.setGlobalPosition       ( posGlobal,  getTimeLastSeeMessage() );
        Ball.setGlobalPositionLastSee( posGlobal,  getTimeLastSeeMessage() );
        return true;
    }
    else if ( SoccerTypes::isKnownPlayer( o )  )
    {
        calculateStatePlayer( o, &posGlobal, &velGlobal );

        PlayerObject *pob = (PlayerObject*) getObjectPtrFromType( o );

        // the time of the velocity does not have to be set, since it equals
        // the time the last change information has been set.
        pob->setGlobalVelocity       ( velGlobal, getTimeLastSeeMessage() );
        pob->setGlobalPosition       ( posGlobal, getTimeLastSeeMessage() );
        pob->setGlobalPositionLastSee( posGlobal, getTimeLastSeeMessage() );

        if ( pob->getTimeRelativeAngles() == getTimeLastSeeMessage() )
        {
            AngDeg ang = getAgentGlobalNeckAngle() + pob->getRelativeBodyAngle();
            ang = VecPosition::normalizeAngle( ang );
            pob->setGlobalBodyAngle( ang, getTimeLastSeeMessage() );
            pob->setGlobalBodyAngleLastSee( ang );
            ang = getAgentGlobalNeckAngle() + pob->getRelativeNeckAngle();
            ang = VecPosition::normalizeAngle( ang );
            pob->setGlobalNeckAngle( ang, getTimeLastSeeMessage() );
            pob->setGlobalVelocityLastSee( velGlobal );
        }
        return true;
    }

    return false;
}


/*****************************************************************************/
/******************* SENSE RELATED UPDATES ***********************************/
/*****************************************************************************/

void WorldModel::processLastSenseMessage( )
{
    char   *strMsg = strLastSenseMessage ;

    Parse::parseFirstInt( &strMsg );// get time
    strMsg++;                                      // go to ( before view_mode

    Parse::gotoFirstOccurenceOf( ' ', &strMsg );   // skip view_mode
    strMsg++;                                      // skip space

    ViewQualityT vq = SoccerTypes::getViewQualityFromStr( strMsg );// get quality
    Parse::gotoFirstOccurenceOf( ' ', &strMsg );
    strMsg++;                                      // skip space; get view_angle
    ViewAngleT va = SoccerTypes::getViewAngleFromStr( strMsg );
    double dStamina = Parse::parseFirstDouble( &strMsg );  // get stamina
    double dEffort  = Parse::parseFirstDouble( &strMsg );  // get effort
    double dCapacity= Parse::parseFirstDouble( &strMsg ); /// get capacity /*! misol.gao fixed for v13.2 */

    double dSpeed   = Parse::parseFirstDouble( &strMsg );  // get speed
    AngDeg angSpeed = Parse::parseFirstDouble( &strMsg );  // get speed ang

    // minus sign since we store angle between neck and body and not vice versa
    int iHeadAngle = - Parse::parseFirstInt( &strMsg );    // get head_angle

    // set all number of performed commands
    setNrOfCommands( CMD_KICK       , Parse::parseFirstInt( &strMsg ) );
    setNrOfCommands( CMD_DASH       , Parse::parseFirstInt( &strMsg ) );
    setNrOfCommands( CMD_TURN       , Parse::parseFirstInt( &strMsg ) );
    setNrOfCommands( CMD_SAY        , Parse::parseFirstInt( &strMsg ) );
    setNrOfCommands( CMD_TURNNECK   , Parse::parseFirstInt( &strMsg ) );
    setNrOfCommands( CMD_CATCH      , Parse::parseFirstInt( &strMsg ) );
    setNrOfCommands( CMD_MOVE       , Parse::parseFirstInt( &strMsg ) );
    setNrOfCommands( CMD_CHANGEVIEW , Parse::parseFirstInt( &strMsg ) );
    int    iArmMovable = Parse::parseFirstInt( &strMsg );         // arm movable
    int    iArmExpires = Parse::parseFirstInt( &strMsg );         // arm expires
    double dArmDist    = Parse::parseFirstDouble( &strMsg );      // arm dist
    AngDeg angArm      = Parse::parseFirstDouble( &strMsg );       // arm dir

    setNrOfCommands( CMD_POINTTO    , Parse::parseFirstInt( &strMsg ) );// count

    // focus target can be none (no integer) so check this
    int i = Parse::parseFirstInt( &strMsg );
    char c = (i >= 10 ) ? *(strMsg-4) : *(strMsg-3);
    Log.log( 602, "focus %d |%c|", i, c );
    if ( c == 'l' || c == 'r' )   // target l or r
    {
        Log.log( 602, "set focus: %d",
                 SoccerTypes::getTeammateObjectFromIndex( -1 + i ) );
        setObjectFocus( SoccerTypes::getTeammateObjectFromIndex( -1 + i ) );
        i = Parse::parseFirstInt( &strMsg );
    }
    setNrOfCommands( CMD_ATTENTIONTO , i );

    int iTackleExpires = Parse::parseFirstInt( &strMsg );   // tackle expires
    setNrOfCommands( CMD_TACKLE , Parse::parseFirstInt( &strMsg ) );

    angArm = VecPosition::normalizeAngle( angArm );
    processNewAgentInfo( vq, va, dStamina, dEffort, dCapacity, dSpeed,/*! misol.gao fixed for v13.2 */
                         (AngDeg) angSpeed, (AngDeg)iHeadAngle, iTackleExpires,
                         iArmMovable, iArmExpires, VecPosition( dArmDist, angArm, POLAR ));
}

/*! This method is called to update the WorldModel after a sense message.
    The current information for the agent, all teammates, all opponents and the
    ball are calculated using the associated methods. This is done by updating
    the objects till the time of the information matches the current time of
    the sense message. After all global
    positions are determined, the relative information is updated using the new
    global information using the method determineRelativeFromGlobal().
    \return bool to indicate whether update succeeded. */
bool WorldModel::updateAfterSenseMessage( )
{
    processLastSenseMessage( );

    // update agent information
    updateAgentAndBallAfterSense( );

    // update all global information of players that have a good confidence
    double dConfThr = PS->getPlayerConfThr();
    int    iIndex;

    for ( ObjectT o = iterateObjectStart( iIndex, OBJECT_SET_PLAYERS, dConfThr );
            o != OBJECT_ILLEGAL;
            o = iterateObjectNext ( iIndex, OBJECT_SET_PLAYERS, dConfThr ) )
    {
        if ( o != getAgentObjectType() && getTimeGlobalPosition(o) > 0 &&
                ! isBeforeKickOff() )
        {
            updateDynamicObjectForNextCycle( o,
                                             getCurrentTime() - getTimeGlobalPosition(o) );
        }
    }
    iterateObjectDone( iIndex);

    // before the kick off all player information can be set to their strat. pos
    if ( isBeforeKickOff() )
    {
        for ( ObjectT o = iterateObjectStart( iIndex, OBJECT_SET_TEAMMATES, -2000 );
                o != OBJECT_ILLEGAL;
                o = iterateObjectNext ( iIndex, OBJECT_SET_TEAMMATES, -2000  ) )
        {
            if ( o == getAgentObjectType() )
                continue;
            PlayerObject *pob = (PlayerObject*) getObjectPtrFromType( o );

            VecPosition pos = getStrategicPosition( SoccerTypes::getIndex( o )  ) ;
            VecPosition vel( 0, 0 );

            pob->setGlobalVelocity       ( vel, getTimeLastSenseMessage() );
            pob->setGlobalPosition       ( pos, getTimeLastSenseMessage() );
            pob->setGlobalPositionLastSee( pos, getTimeLastSenseMessage() );
            pob->setTimeLastSeen         ( getTimeLastSenseMessage() - 2 );
            updateObjectRelativeFromGlobal( o );
            Log.log( 459, "set info %d (%f,%f)", SoccerTypes::getIndex( o ),
                     pos.getX(), pos.getY() );
        }
        Ball.setGlobalPosition( VecPosition(0,0), getTimeLastSenseMessage() );
        Ball.setGlobalVelocity( VecPosition(0,0), getTimeLastSenseMessage() );
    }
//    show( OBJECT_SET_PLAYERS, Log.getOutputStream() );
    iterateObjectDone( iIndex);

    return true;
}

/*! This method updates the agent and ball information after a sense message.
    This is done using the current known information and the action that is
    performed in the last server cycle as indicated by the attribute
    'PerformedCommands'
    \return bool to indicate whether update succeeded. */
bool WorldModel::updateAgentAndBallAfterSense( )
{
    // get info from commands from previous cycle (if timestopped current cycle)
    bool        bBallUpdated   = false;
    VecPosition pos            = getAgentGlobalPosition();
    AngDeg      angGlobalNeck  = getAgentGlobalNeckAngle();
    AngDeg      angGlobalBody  = getAgentGlobalBodyAngle();
    Stamina     sta            = getAgentStamina();
    VecPosition velNeck        = agentObject.getSpeedRelToNeck(); // !!
    VecPosition vel            = getAgentGlobalVelocity();
    Time        time           = getCurrentTime() - 1 ;

    // first update information based on performed actions without using
    // the received velocity information.
    for ( int i = 0; i < CMD_MAX_COMMANDS; i ++  )
    {
        if ( performedCommands[i] == true && // sense msg indicates we executed this
                ( queuedCommands[i].time.getTime() == time.getTime() ||      // no hole
                  queuedCommands[i].time.getTime() == time.getTime() - 1 ) )// hole
        {
            switch ( queuedCommands[i].commandType )
            {
            case CMD_TURN:
            case CMD_TURNNECK:
            case CMD_DASH:
            case CMD_TACKLE: // in case of tackle do not update ball -> we do not
                // know whether tackle succeeded
                predictStateAfterCommand( queuedCommands[i], &pos, &vel,
                                          &angGlobalBody, &angGlobalNeck,
                                          getAgentObjectType(), &sta );
                break;
            case CMD_KICK:
                updateBallAfterKick( queuedCommands[i] );
                bBallUpdated     = true;         // should not be updated later on.
                queuedCommands[i].time.updateTime( -1 );
                break;
            case CMD_MOVE:
                pos.setVecPosition( queuedCommands[i].dX, queuedCommands[i].dY );
                initParticlesAgent( pos );
                break;
            default:
                break;
            }
        }
    }

    // update the ball when not done yet
    if ( !bBallUpdated )
    {
        m_bPerformedKick = false;
        updateDynamicObjectForNextCycle( OBJECT_BALL, 1 );
    }
    else
        m_bPerformedKick = true;

    // now use velocity information to check whether a collision occured
    velNeck.rotate( angGlobalNeck ); // make relative info global
    double dDistBall = pos.getDistanceTo( getBallPos() );

    if ( velNeck.getMagnitude() < EPSILON &&
            vel.getMagnitude() > 0.01 &&
            dDistBall < 0.6 )
        Log.log( 102, "I assume collision after kick" );

    if ( (
                velNeck.getMagnitude() < EPSILON &&
                vel.getMagnitude() > 0.01 &&
                dDistBall < 0.8
            )
            ||
            ( dDistBall<SS->getPlayerSize()+SS->getBallSize() && // ball too close AND
              velNeck.getMagnitude( ) < EPSILON                 // vel. gives no info
            )
            ||                                                   // OR
            ( dDistBall < SS->getMaximalKickDist() + 0.5  &&     // ball close
              velNeck.getMagnitude( ) > EPSILON &&                // can compare vel
              ( sign( vel.getX() ) != sign( velNeck.getX() ) ||   // both signs have
                fabs( vel.getX() ) < 0.08 )                &&     // changed or are
              ( sign( vel.getY() ) != sign( velNeck.getY() ) ||   // so small it is in
                fabs( vel.getY() ) < 0.08 )                &&     // error range
              velNeck.getMagnitude() < 0.25 * vel.getMagnitude() ) ) // lessened a lot
    {
        m_bWasCollision = true;
        m_timeLastCollision = getCurrentTime();
        double dDistPlayer;
        getClosestInSetTo(
            OBJECT_SET_PLAYERS, getAgentObjectType(), &dDistPlayer );
        if ( dDistBall < dDistPlayer )
        {
            updateBallForCollision( pos );
            Log.log( 102, "COLLISION WITH BALL %f, vel(%f,%f) velNeck(%f,%f)",
                     dDistBall, vel.getX(), vel.getY(), velNeck.getX(), velNeck.getY() );
        }
        else
            Log.log( 102, "COLLISION WITH PLAYER vel(%f,%f) velNeck(%f,%f)",
                     vel.getX(), vel.getY(), velNeck.getX(), velNeck.getY() );

        // new vel. agent is the one received from sense message
        vel = velNeck;
    }
    else
    {
        if ( dDistBall < SS->getVisibleDistance() )
            Log.log( 102, "No collision: dist: %f, velNeck (%f,%f), vel (%f,%f)",
                     dDistBall, velNeck.getX(), velNeck.getY(), vel.getX(), vel.getY() );
        m_bWasCollision = false;
        // use better vel. estimate in sense message to update information
        pos            = getAgentGlobalPosition();
        angGlobalNeck  = getAgentGlobalNeckAngle();
        angGlobalBody  = getAgentGlobalBodyAngle();
        sta            = getAgentStamina();
        vel            = agentObject.getSpeedRelToNeck();

        // calculate velocity at end of previous cycle using velocity from
        // current cycle. Although we do not know direction yet (this is
        // relative to neck which is not yet known), we can use the
        // magnitude to determine traveled  distance (speed) of the agent
        // After neck angle is estimated, we can rotate velocity vector
        // to get actual velocity.
        vel.setMagnitude( vel.getMagnitude()/SS->getPlayerDecay() );

        for ( int i = 0; i < CMD_MAX_COMMANDS; i ++  )
        {
            if ( performedCommands[i] == true &&
                    ( queuedCommands[i].time.getTime() == time.getTime() ||
                      queuedCommands[i].time.getTime() == time.getTime() - 1 ) )
            {
                switch ( queuedCommands[i].commandType )
                {
                case CMD_TURN:
                case CMD_TURNNECK:
                    predictStateAfterCommand( queuedCommands[i], &pos, &vel,
                                              &angGlobalBody, &angGlobalNeck,
                                              getAgentObjectType(), &sta );
                    break;
                case CMD_MOVE:
                    pos.setVecPosition( queuedCommands[i].dX, queuedCommands[i].dY );
                    initParticlesAgent( pos );
                    break;
                case CMD_DASH:     // no action needed, since resulting
                    // vel. is already available from sense
                    // this vel can be used to update pos
                    break;
                case CMD_TACKLE:   // no action needed, we are never sure that tackle
                    // is executed
                    break;
                default:
                    break;
                }
            }
            queuedCommands[i].time.updateTime( -1 );            // processed
        }
        // reset pos and vel information to previous cycle (since can be
        // changed in predictStateAfterCommand)
        vel = agentObject.getSpeedRelToNeck();
        pos = getAgentGlobalPosition();
        vel.setMagnitude( vel.getMagnitude()/SS->getPlayerDecay() );
        vel.rotate( angGlobalNeck ); // rotate vel using information about neck

        // predict global position using the calculated vel at the end of
        // the previous cycle (power and direction can thus both be set
        // to zero). There is just little noise in this perception, so
        // gives a good estimate
        predictStateAfterDash( 0.0, &pos, &vel, &sta, 0, getAgentObjectType() );
    }

    // update particles that keep track of position of agent using vel
    updateParticlesAgent( vel, true );

    // body angle is not set since relative angle to neck is already
    // set after parsing sense_body message, same holds for stamina
    agentObject.setGlobalPosition ( pos,        getCurrentTime() );
    agentObject.setGlobalVelocity ( vel,        getCurrentTime() );
    agentObject.setGlobalNeckAngle( angGlobalNeck );

    return true;
}

/*! This methods updates the ball after a kick command. First it is checked
    whether the ball is indeed in the kickable range. If this is the case
    the accelaration of the ball is calculated and added to the current
    velocity. With this information the new global position of the ball
    is set.
    \param soc performed kick command
    \return bool indicating whether update was succesful */
bool WorldModel::updateBallAfterKick( SoccerCommand soc )
{
    if ( getRelativeDistance( OBJECT_BALL ) < SS->getMaximalKickDist() )
    {
        VecPosition posBall, velBall;
        predictBallInfoAfterCommand( soc, &posBall, &velBall );
        Ball.setGlobalPosition( posBall, getCurrentTime()  );
        Ball.setGlobalVelocity( velBall, getCurrentTime()  );
// updateParticlesBall( particlesPosBall, particlesVelBall,
//                        iNrParticlesBall, dPower, ang );
    }
    else
    {
        updateDynamicObjectForNextCycle( OBJECT_BALL, 1 );
//  updateParticlesBall( particlesPosBall, particlesVelBall,
//  iNrParticlesBall, 0, 0 );
#ifdef WIN32
        Log.log( 21, "(WorldModel::%s) KICK command, but ball not kickable (%f)",
                 "updateBallAfterKick", getRelativeDistance( OBJECT_BALL ) );
#else
        Log.log( 21, "(WorldModel::%s) KICK command, but ball not kickable (%f)",
                 __FUNCTION__, getRelativeDistance( OBJECT_BALL ) );
#endif
    }
    return true;
}

/*! This methods updates a dynamic object for 'iCycles' cycle. This is done by
    updating the global position with the velocity vector. After this
    the velocity vector is decreased with the associated decay.
    \param obj object that should be updated
    \param iCycles denotes for how many cycles dynamic object should be updated
    \return bool indicating whether update was succesful */
bool WorldModel::updateDynamicObjectForNextCycle( ObjectT obj, int iCycles)
{
    DynamicObject *o = (DynamicObject*) getObjectPtrFromType( obj );
    if ( o == NULL )
        return false;

    // get velocity and add it to current global position
    VecPosition vel = getGlobalVelocity( obj );
    VecPosition pos = getGlobalPosition( obj );
    VecPosition posBall = getBallPos();
    //  ObjectT objFastest =  getFastestInSetTo( OBJECT_SET_TEAMMATES,
    //                                       OBJECT_BALL );
    if ( SoccerTypes::isBall( obj ) )
    {
        for ( int i = 0; i < iCycles ; i++ )
        {
            pos += vel;
            vel *= SS->getBallDecay();
        }
        double  dDist;
        ObjectT objOpp =
            getClosestInSetTo( OBJECT_SET_OPPONENTS, OBJECT_BALL, &dDist );
        if ( objOpp != OBJECT_ILLEGAL &&
                pos.getDistanceTo( getGlobalPosition( objOpp ) )
                < getMaximalKickDist( objOpp ) )
        {
            Log.log( 556, "update dyn. obj; set ball velocity to 0, opp has it" );
            vel.setVecPosition(0,0);
        }
    }
    else if ( SoccerTypes::isTeammate( obj ) &&
              obj != OBJECT_TEAMMATE_1 && getPlayMode() == PM_PLAY_ON )
    {
        for ( int i = 0; i < iCycles ; i++ )
        {
            // no idea of intention opponent, just let him roll out
            pos += vel;
            vel *= SS->getPlayerDecay();
        }
    }
    else if ( SoccerTypes::isOpponent( obj ) )
    {
        VecPosition velBall = getGlobalVelocity( OBJECT_BALL );
        for ( int i = 0; i < iCycles ; i++ )
        {
            // no idea of intention opponent, just let him roll out
            if ( obj == getOppGoalieType() )
                ;
            // if it is the fastest opponent, move him towards ball
            else if ( obj == getFastestInSetTo( OBJECT_SET_OPPONENTS, OBJECT_BALL ) )
            {
                AngDeg ang = (getBallPos()-getGlobalPosition( obj )).getDirection();
                pos += VecPosition( 0.3, ang, POLAR );
                vel *= SS->getPlayerDecay();
                Log.log( 556, "update fastest opp to (%f,%f) cyc %d",
                         pos.getX(), pos.getY(), iCycles );
            }
            else if ( velBall.getX() > 0 && ! ( isDeadBallUs() || isDeadBallThem() )
                      && ! ( fabs( pos.getX() ) > PENALTY_X + 5 ) )
            {
                pos += VecPosition( (velBall.getX() > 1.0) ? 0.5 : 0.25, 0 );
                vel *= SS->getPlayerDecay();
            }
            else if ( ! ( isDeadBallUs() || isDeadBallThem() )
                      && ! ( fabs( pos.getX() ) > PENALTY_X + 5 ) )
            {
                //     pos += vel;
                pos -= VecPosition( (velBall.getX() < -1.0 ) ? 0.5 : 0.25, 0 );
                vel *= SS->getPlayerDecay();
            }
        }
    }

    o->setGlobalVelocity ( vel, getCurrentTime() );
    o->setGlobalPosition ( pos, getCurrentTime() );

    return true;
}


/*! This method checks updates the ball information when it overlaps with a
    player. If this is the case the player and the ball are moved to a position
    where they do not overlap any more. Both velocities are multiplied with 0.1
    \return true when collision occured, false otherwise */
bool WorldModel::updateBallForCollision( VecPosition posAgent )
{
    VecPosition posBall  = getGlobalPosition( OBJECT_BALL );
    VecPosition velBall  = getGlobalVelocity( OBJECT_BALL );

    // get the direction the ball was coming from and put it at small distance
    // and multiply the velocity with -0.1
    AngDeg ang = (posBall - posAgent).getDirection();
    Ball.setGlobalPosition( posAgent + VecPosition( 0.2, ang, POLAR ),
                            getCurrentTime() );
    Ball.setGlobalVelocity( velBall*-0.1, getCurrentTime() );
    Log.log( 102, "updateBallForCollision; vel from (%f,%f) to (%f,%f)",
             velBall.getX(), velBall.getY(), getGlobalVelocity(OBJECT_BALL).getX(),
             getGlobalVelocity(OBJECT_BALL).getY() );
    return true;
}

/*! This method uses the global position of the agent and the global
    position of all objects to update their relative information. When
    the new global position of the objects is updated after a sense message,
    their relative information is not up to date.
   \return bool indicating whether update was succesful */
bool WorldModel::updateRelativeFromGlobal()
{
    double dConfThr = PS->getPlayerConfThr();
    int    iIndex;

    // update all player objects
    for ( ObjectT o = iterateObjectStart( iIndex, OBJECT_SET_PLAYERS, dConfThr );
            o != OBJECT_ILLEGAL;
            o = iterateObjectNext ( iIndex, OBJECT_SET_PLAYERS, dConfThr ) )
    {
        if ( o != getAgentObjectType() )
            updateObjectRelativeFromGlobal( o );
    }
    iterateObjectDone( iIndex);

    // and also the ball
    if ( isConfidenceGood( OBJECT_BALL ) )
        updateObjectRelativeFromGlobal( OBJECT_BALL );

    // flags and lines are not updated, since they are not used except
    // immediately after see message when they're up to date.
    return true;
}

/*! This method updates the relative information for one object using the
    global information of this object and the global information of the
    agent.
    \param o object information that should be updated.
    \return bool indicating whether the update was succesfull. */
bool WorldModel::updateObjectRelativeFromGlobal( ObjectT o )
{
    Object *obj = (Object*) getObjectPtrFromType( o );
    if ( obj == NULL )
        return false;

    // get global position and translate and rotate it to agent neck
    VecPosition rel = obj->getGlobalPosition();
    rel.globalToRelative( getAgentGlobalPosition(), getAgentGlobalNeckAngle() );
    obj->setRelativePosition( rel, obj->getTimeGlobalPosition() );
    return true;
}

/*! This method calculates the different state information of the agent, that
    is the global position, global velocity and global neck angle using the
    available information in the world model. This method uses a particle
    filter to determine this information.
    \param posGlobal will be filled with global position of the agent
    \param velGlobal will be filled with global velocity of the agent
    \param angGlobal will be filled with global neck angle of the agent
    \return global position of the agent */
bool WorldModel::calculateStateAgent( VecPosition *posGlobal,
                                      VecPosition *velGlobal, AngDeg *angGlobal )
{
    int    iNrLeft;

    // first determine global neck angle
    ObjectT objLine = getFurthestRelativeInSet( OBJECT_SET_LINES );
    if ( objLine != OBJECT_ILLEGAL )
    {
        double angGlobalLine = getGlobalAngle  ( objLine );
        AngDeg angRelLine    = getRelativeAngle( objLine );
        *angGlobal           = angGlobalLine - angRelLine;
        *angGlobal           = VecPosition::normalizeAngle( *angGlobal );
    }
    else
    {
        Log.log( 21,
                 "(WorldModel::calculateStateAgent) no line in last see message" );
        *angGlobal           = getAgentGlobalNeckAngle();
    }

    // use global neck angle to determine estimate of current global velocity
    // neck angle is better estimate than after sense -> better estimate velocity
    // update all position particles of the agent with this velocity
    //  'false' denotes update after see message. Since global neck angle can
    //  be determined more precise after 'see' it is better to predict position
    //  again (although it was already done after sense)
    // and then check which particles are possible given current perceptions
    *velGlobal = agentObject.getSpeedRelToNeck().rotate( *angGlobal );
    velGlobal->setMagnitude( velGlobal->getMagnitude()/SS->getPlayerDecay() );

    updateParticlesAgent          ( *velGlobal, false );
    iNrLeft = checkParticlesAgent ( *angGlobal        );

    if ( iNrLeft == 0 ) // if no particles left, initialize all particles
    {
        // initialize particles (from random samples using closest flag)
        // check particles are then checked with other flags
        initParticlesAgent ( *angGlobal );
        iNrLeft = checkParticlesAgent( *angGlobal );
        if ( iNrLeft == 0 )
        {
            // not succeeded, use second method (weighted average flags)
            // and initialize all particles to this position
            calculateStateAgent2( posGlobal, velGlobal, angGlobal );
            initParticlesAgent( *posGlobal );
            return false;
        }
    }

    // determine global position (= average of all particles)
    // and resample all particles
    *posGlobal = averageParticles( particlesPosAgent, iNrLeft );
    resampleParticlesAgent( iNrLeft );

    // use the position to calculate better global neck angle of the agent
    // and recalculate global velocity with improved neck angle
    AngDeg ang = calculateAngleAgentWithPos( *posGlobal );
    if ( ang != UnknownAngleValue )
        *angGlobal = ang;

    *velGlobal = agentObject.getSpeedRelToNeck().rotate(*angGlobal);
    return true;
}

/*! This method initializes all particles that represent the global position
    of the agent. This is done using the closest perceived flag. Points are
    generated from the area that could generate the perceived information.
    The global neck angle of the agent 'angGlobal' is used to determine the
    global position of the agent based on the perceived relative information
    to the closest flag.
    \param angGlobal global neck angle of the agent */
void WorldModel::initParticlesAgent( AngDeg angGlobal )
{
    double  dDist, dMaxRadius, dMinRadius, dInput;
    AngDeg  ang;

    // get closest flag from which samples will be generated
    ObjectT objFlag = getClosestRelativeInSet( OBJECT_SET_FLAGS );

    if ( objFlag == OBJECT_ILLEGAL )
    {
        Log.log( 21, "(WorldModel::%s) no flag in last see message",
                 "initParticlesAgent" );
        return;
    }

    // get the distance to this flag and the possible range it was derived from.
    dInput = getRelativeDistance( objFlag );
    getMinMaxDistQuantizeValue( dInput, &dMinRadius, &dMaxRadius,
                                SS->getQuantizeStepL(), 0.1 ) ;

    // get the perceived angle to this flag (add 180 to get angle relative from
    // flag to agent ) and make it global by adding global neck angle agent.
    AngDeg angFlag   = getRelativeAngle( objFlag ) + 180 + angGlobal ;
    // for all particles
    for ( int i = 0 ; i < iNrParticlesAgent ; i++ )
    {
        // determine random point from distance range and
        // determine random point from the range it could be generated from
        // angles are rounded and since noise is in global neck angle and relative
        // angle flag, maximum error is in range [-0.5,0.5] + [-0.5,0.5] = [-1,1].
        dDist = dMinRadius + drand48()*(dMaxRadius-dMinRadius);
        ang   = VecPosition::normalizeAngle( angFlag - 1.0 + 2*drand48() );

        // create random point from possible interval
        particlesPosAgent[i] = getGlobalPosition( objFlag ) +
                               VecPosition( dDist, ang, POLAR );
    }
}

/*! This method initializes all particles that represent the global position
    of the agent. All points are initialized with the position 'posInitial'.
    This method can be used when you are completely sure of the position of
    the agent (for example after a 'move' command).
    \param posInitial global position of the agent */
void WorldModel::initParticlesAgent( VecPosition posInitial )
{
    for ( int i = 0 ; i < iNrParticlesAgent ; i++ )
        particlesPosAgent[i] = posInitial;
}

/*! This method checks all the particles that represent the global
    position of the agent using the flags in the last perceived see
    message. The global neck angle of the agent is used to make the
    perceived information global. Particles that are not possible are
    deleted and all legal particles will be shifted to the beginning
    of the array. The number of legal particles is returned.

    \param angGlobalNeck global neck of the angle
    \return number of legal particles */
int WorldModel::checkParticlesAgent( AngDeg angGlobalNeck  )
{
    double dMaxRadius, dMinRadius, dInput, dDist;
    AngDeg ang;
    int    iIndex, iNrLeft = iNrParticlesAgent, iLength = iNrParticlesAgent;

    // for all current perceived flags
    for ( ObjectT o = iterateObjectStart( iIndex, OBJECT_SET_FLAGS, 1.0 );
            o != OBJECT_ILLEGAL;
            o = iterateObjectNext ( iIndex, OBJECT_SET_FLAGS, 1.0 ) )
    {
        iNrLeft = 0;                        // reset number of correct particles
        dInput = getRelativeDistance( o );  // get possible distance range
        getMinMaxDistQuantizeValue( dInput, &dMinRadius, &dMaxRadius,
                                    SS->getQuantizeStepL(), 0.1 )  ;

        // find all "correct points"
        for ( int i = 0; i < iLength; i ++ )
        {
            // determine distance particle to flag
            // determine difference in direction between direction between global
            // flag and agent position and global perceived direction
            dDist = particlesPosAgent[i].getDistanceTo( getGlobalPosition( o ) );
            ang = (getGlobalPosition(o) - particlesPosAgent[i]).getDirection();
            ang = ang - ( getRelativeAngle( o ) + angGlobalNeck );

            // if in possible range, save it (maximum angle range is 0.5 for
            // neck angle and 0.5 for relative flag angle gives 1.0)
            if ( dDist > dMinRadius && dDist < dMaxRadius &&
                    fabs(VecPosition::normalizeAngle( ang )) <= 1.0 )
                particlesPosAgent[iNrLeft++] = particlesPosAgent[i];
        }
        // change maximum of correct particles
        iLength = iNrLeft;
    }
    return iNrLeft;
}

/*! This method updates all the particles that represent the global position
    of the agent.The velocity 'vel' is added to each particle to move to
    the next location. When the last boolean argument is true, the velocity
    is simple added. If this value equals true the previously added velocity
    is subtracted before the specified velocity 'vel' is added. This for
    instance occurs when a better velocity estimate (after a see message) is
    determined with which the particles should be updated.
    \param velocity that should be added to each particle
    \param bAfterSense bool denoting whether this update is done after sense
           or see. When true last added velocity is first subtracted. */
void WorldModel::updateParticlesAgent( VecPosition vel, bool bAfterSense )
{
    // used to denote last added velocity
    static VecPosition prev_vel;

    for ( int i = 0; i < iNrParticlesAgent  ; i ++ )
    {
        if ( bAfterSense == false ) // if after see, subtract last added 'vel'
        {
            particlesPosAgent[i].setX( particlesPosAgent[i].getX()-prev_vel.getX());
            particlesPosAgent[i].setY( particlesPosAgent[i].getY()-prev_vel.getY());
        }

        particlesPosAgent[i].setX( particlesPosAgent[i].getX( ) + vel.getX() );
        particlesPosAgent[i].setY( particlesPosAgent[i].getY( ) + vel.getY() );
    }
    prev_vel = vel;
}


/*! This method returns the average of the particles contained in 'posArray'.
    \param posArray containing all possible global positions of the agent
    \param iLength number of particles in posArray
    \return average position of all the particles */
VecPosition WorldModel::averageParticles( VecPosition posArray[], int iLength )
{
    if ( iLength == 0 )
        return VecPosition( UnknownDoubleValue, UnknownDoubleValue );

    // take average of particles
    double x = 0, y = 0;
    for ( int i = 0; i < iLength  ; i ++ )
    {
        x += posArray[ i ].getX( );
        y += posArray[ i ].getY( );
    }

    return VecPosition( x/iLength, y/iLength );
}

/*! This method resamples the particles that represent the global position of
    the agent. 'iLength' denotes the number of particles contained in
    'posArray' and 'iLeft' the points that do do not have to be resampled.
    A particle is resampled by taking a random particle from the first part
    of the array.
    \param iLeft number of particles that should be contained */
void WorldModel::resampleParticlesAgent( int iLeft )
{
    for ( int i = iLeft; i < iNrParticlesAgent; i ++ )
        particlesPosAgent[ i ] = particlesPosAgent[ (int)(drand48()*iLeft) ];
}

/*! This method calculates the different state information of the agent, that
    is the global position, global velocity and global neck angle using the
    available information in the world model. This method uses a weighted
    average of all currently perceived flags.
    \param posGlobal will be filled with global position of the agent
    \param velGlobal will be filled with global velocity of the agent
    \param angGlobal will be filled with global neck angle of the agent
    \return global position of the agent */
bool WorldModel::calculateStateAgent2( VecPosition *posGlobal,
                                       VecPosition *velGlobal, AngDeg *angGlobal)
{
    double      x=0.0, y=0.0, dMinRadius1, dMaxRadius1, dMinRadius2, dMaxRadius2;
    double      dTotalVar = UnknownDoubleValue, dVar, K;
    int         iIndex1, iIndex2;
    ObjectT     obj2;
    VecPosition pos;

    // for all flags that were perceived in last see message
    for ( ObjectT obj1 = iterateObjectStart( iIndex1, OBJECT_SET_FLAGS, 1.0 );
            obj1 != OBJECT_ILLEGAL;
            obj1 = iterateObjectNext ( iIndex1, OBJECT_SET_FLAGS, 1.0 ) )
    {
        // calculate global position with all other flags using two flags
        iIndex2 = iIndex1;
        for ( obj2 = iterateObjectNext ( iIndex2, OBJECT_SET_FLAGS, 1.0 ) ;
                obj2 != OBJECT_ILLEGAL;
                obj2 = iterateObjectNext ( iIndex2, OBJECT_SET_FLAGS, 1.0 ) )
        {
            // calculate the position using the two flags
            pos = calculatePosAgentWith2Flags( obj1, obj2 );

            // get distance range from which perceived value can originate from
            // calculate variance (=weighted factor) based on the distance to the
            // two flags, use variance corresponding to uniform distribution
            // this is not completely correct, better would be to use the
            // intersection area size of the two circle, but is too computational
            // intensive
            getMinMaxDistQuantizeValue(getRelativeDistance(obj1),
                                       &dMinRadius1, &dMaxRadius1, SS->getQuantizeStepL(), 0.1 )  ;
            getMinMaxDistQuantizeValue(getRelativeDistance(obj2),
                                       &dMinRadius2, &dMaxRadius2, SS->getQuantizeStepL(), 0.1 )  ;
            dVar =  (dMaxRadius1-dMinRadius1)*(dMaxRadius1-dMinRadius1)/12;
            dVar += (dMaxRadius2-dMinRadius2)*(dMaxRadius2-dMinRadius2)/12;

            if ( pos.getX() != UnknownDoubleValue &&
                    dTotalVar  == UnknownDoubleValue )
            {
                dTotalVar = dVar;                     // initialize the position
                x         = pos.getX();
                y         = pos.getY();
            }
            else if ( pos.getX() != UnknownDoubleValue )
            {
                K = dTotalVar / ( dVar + dTotalVar ); // otherwise use new position
                x += K*( pos.getX() - x );            // based on weighted variance
                y += K*( pos.getY() - y );
                dTotalVar -= K*dTotalVar;
            }
        }
        iterateObjectDone( iIndex2 );
    }
    iterateObjectDone( iIndex1 );
    posGlobal->setVecPosition( x, y );

    // now calculate global position (experiments show best results are obtained
    // when average with all perceived flags is taken).
    *angGlobal = calculateAngleAgentWithPos( *posGlobal );

    // update velocity since after 'see' we have a better estimate of neck angle
    *velGlobal = agentObject.getSpeedRelToNeck().rotate(*angGlobal);

    return true;
}

/*! This method calculates the different state information of the
    agent, that is the global position, global velocity and global
    neck angle using the available information in the world
    model. Only the closest two global flags are taken into account.

    \param posGlobal will be filled with global position of the agent
    \param velGlobal will be filled with global velocity of the agent
    \param angGlobal will be filled with global neck angle of the agent
    \return global position of the agent */
bool WorldModel::calculateStateAgent3( VecPosition *posGlobal,
                                       VecPosition *velGlobal, AngDeg *angGlobal )
{
    // first determine the two closest flags
    ObjectT objFlag1 = getClosestRelativeInSet( OBJECT_SET_FLAGS );
    ObjectT objFlag2 = getSecondClosestRelativeInSet( OBJECT_SET_FLAGS );
    ObjectT objLine = getFurthestRelativeInSet( OBJECT_SET_LINES );

    // first determine global neck angle with furthest line (max. error is 0.5)
    if ( objLine != OBJECT_ILLEGAL )
    {
        double angGlobalLine = getGlobalAngle  ( objLine );
        AngDeg angRelLine    = getRelativeAngle( objLine );
        *angGlobal           = angGlobalLine - angRelLine;
        *angGlobal           = VecPosition::normalizeAngle( *angGlobal );
    }

    // if two flags were seen in last see message
    //   calculate global position using two closest flags (Cosinus rule)
    //     except when rel angle flags is smaller than 8 (gives bad results)
    //       except when no line is seen
    if ( objFlag1 != OBJECT_ILLEGAL && objFlag2 != OBJECT_ILLEGAL &&
            (
                fabs(getRelativeAngle(objFlag1) - getRelativeAngle(objFlag2)) > 8.0
                || objLine == OBJECT_ILLEGAL
            ) )
    {
        *posGlobal = calculatePosAgentWith2Flags( objFlag1, objFlag2 );
    }
    else if ( objFlag1 != OBJECT_ILLEGAL && objLine != OBJECT_ILLEGAL )
    {
        // calculate global position as follows:
        //   - get the global position of the closest flag
        //   - get the rel vector to this flag using global neck angle
        //   - global position = global pos flag - relative vector

        VecPosition posGlobalFlag = getGlobalPosition( objFlag1 );
        VecPosition relPosFlag    = VecPosition::getVecPositionFromPolar(
                                        getRelativeDistance( objFlag1 ),
                                        *angGlobal + getRelativeAngle( objFlag1 ) );
        *posGlobal                = posGlobalFlag - relPosFlag;
    }
    else
    {
        cout << "(WorldModel::calculateStateAgent3) cannot determine pos in cycle "
             << getCurrentCycle() << endl;
        return false;
    }

    // calculate global velocity using the absolute neck angle
    *velGlobal = agentObject.getSpeedRelToNeck().rotate(*angGlobal);

    return true;
}

/*! This method calculates the global position of the agent using two flags.
    Using the perceived distance to the two flag, two circles are created with
    as center the global position of the flag. The intersection of these two
    circles is returned as the global position of the agent (note that the
    relative direction to both flags can be used to determine which of the
    two possible intersection points is the correct position).
    It is assumed that the relative information of the specified flags are
    from the last see message.
    \param objFlag1 object type of first flag
    \param objFlag2 object type of second flag
    \return global position of the agent */
VecPosition WorldModel::calculatePosAgentWith2Flags( ObjectT objFlag1,
        ObjectT objFlag2 )
{
    double      xA, yA, xB, yB, rA, rB;
    AngDeg      aA, aB;

    // get all information of the two flags
    xA = getGlobalPosition  ( objFlag1 ).getX();
    yA = getGlobalPosition  ( objFlag1 ).getY();
    rA = getRelativeDistance( objFlag1 );
    aA = getRelativeAngle   ( objFlag1 );
    xB = getGlobalPosition  ( objFlag2 ).getX();
    yB = getGlobalPosition  ( objFlag2 ).getY();
    rB = getRelativeDistance( objFlag2 );
    aB = getRelativeAngle   ( objFlag2 );

    double      L, dx, dy, d_par, d_orth;
    double      x, y;
    // Sign is like this 'because' y-axis increases from top to bottom
    double sign = ((aB - aA) > 0.0) ? 1.0 : -1.0;

    // From Cosinus rule: rB rB = L L + rA rA - 2 L rA cos(aB)
    // with:              rA cos(aB) = d_par
    // and:               d_par^2 + d_orth^2 = rA^2
    // Finally:
    // Position from landmark position and vectors parallel and orthogonal
    // to line from landmark A to B

    dx = xB - xA;
    dy = yB - yA;
    L = sqrt(dx*dx + dy*dy);                   // distance between two flags

    dx /= L;
    dy /= L;                          // normalize

    d_par = (L*L + rA*rA - rB*rB) / (2.0 * L); // dist from flag1 to orth proj
    double arg = rA*rA - d_par*d_par;
    d_orth = (arg > 0.0) ? sqrt(arg) : 0.0;

    x = xA + d_par * dx - sign * d_orth * dy;
    y = yA + d_par * dy + sign * d_orth * dx;

    return VecPosition( x, y );
}

/*! This method calculates the global neck angle of the agent using
    all available flag information and the estimation of the current
    global position of the agent 'pos'. For each perceived flag, the
    global direction is calculated using the known global position of
    the flag and the specified global position 'pos' of the
    agent. Then the global neck direction of the agent is determined
    using the relative direction to this flag.  The average neck angle
    of all perceived flags is returned.

    \param pos current global position of the agent
    \return global neck angle of the agent */
AngDeg WorldModel::calculateAngleAgentWithPos( VecPosition pos )
{
    int    iNrFlags = 0, iIndex;
    double dCosX=0, dSinX=0 ,dAngleNow, xA, yA, aA;

    for ( ObjectT obj = iterateObjectStart( iIndex, OBJECT_SET_FLAGS, 1.0 );
            obj != OBJECT_ILLEGAL;
            obj = iterateObjectNext ( iIndex, OBJECT_SET_FLAGS, 1.0 ) )
    {
        xA = getGlobalPosition( obj ).getX();
        yA = getGlobalPosition( obj ).getY();
        aA = getRelativeAngle( obj );

        // calculate global direction between flag and agent
        // calculate global neck angle by subtracting relative angle to flag
        dAngleNow = atan2Deg( yA - pos.getY(), xA - pos.getX() );
        dAngleNow = VecPosition::normalizeAngle( dAngleNow - aA );

        // add cosine part of angle and sine part separately; this to avoid
        // boundary problem when computing average angle (average of -176 and
        // 178 equals -179 and not 1).
        dCosX += cosDeg( dAngleNow );
        dSinX += sinDeg( dAngleNow );
        iNrFlags++;

    }
    iterateObjectDone( iIndex );

    // calculate average cosine and sine part and determine corresponding angle
    dCosX /= (double)iNrFlags;
    dSinX /= (double)iNrFlags;
    if ( iNrFlags == 0 )
        return UnknownAngleValue;

    return VecPosition::normalizeAngle( atan2Deg( dSinX, dCosX  ) )  ;
}


/*! This method returns the velocity from the object o given the perceptions
    from the see message. It uses the soccer server formula directly
    (thus assuming no noise).
    \param o object type for which velocity is determined
    \return global velocity of the ball */
VecPosition WorldModel::calculateVelocityDynamicObject( ObjectT o )
{
    DynamicObject * dobj = (DynamicObject*) getObjectPtrFromType( o );
    if ( dobj == NULL )
        return VecPosition( UnknownDoubleValue, UnknownDoubleValue );
    double dDistCh = dobj->getRelativeDistanceChange(   );
    double angCh   = dobj->getRelativeAngleChange   (   );
    double dDist   = getRelativeDistance            ( o );
    double ang     = getRelativeAngle               ( o );

    double velx = dDistCh * cosDeg(ang) - Deg2Rad(angCh) * dDist * sinDeg( ang );
    double vely = dDistCh * sinDeg(ang) + Deg2Rad(angCh) * dDist * cosDeg( ang );

    VecPosition vel( velx, vely );
    return vel.relativeToGlobal( getAgentGlobalVelocity(),
                                 getAgentGlobalNeckAngle() );
}


/*! This method calculates the global position and velocity of the ball using
    the newest visual information.
    \param *posGlobal will be filled with the global position
    \param *velGlobal will be filled with the global velocity
    \return true when calculations were succesful, false otherwise */
bool WorldModel::calculateStateBall( VecPosition *posGlobal,
                                     VecPosition *velGlobal )
{
    // set the global position of the ball as follows:
    //  - get the relative position from the agent to it in world-axis
    //  - add global position agent to this relative position
    VecPosition posRelWorld =
        VecPosition( getRelativeDistance( OBJECT_BALL ),
                     getRelativeAngle( OBJECT_BALL ) + getAgentGlobalNeckAngle(),
                     POLAR );
    *posGlobal = getAgentGlobalPosition() + posRelWorld;

    if ( isBeforeKickOff() )
    {
        posGlobal->setVecPosition( 0, 0 );
        velGlobal->setVecPosition( 0, 0 );
        return true;
    }

    *velGlobal = getGlobalVelocity(OBJECT_BALL);
    if ( Ball.getTimeChangeInformation( ) == getTimeLastSeen( OBJECT_BALL ) )
    {
        *velGlobal = calculateVelocityDynamicObject( OBJECT_BALL );;
        Log.log( 458, "vel based on change info: (%f,%f)", velGlobal->getX(),
                 velGlobal->getY() );
        // average result with predicted velocity from last see message
        // this has the best result for the ball: see program absvelocity.C
        // only use average when no big chance (kick,turn or dash) has occured
        if ( fabs(velGlobal->getX() - getGlobalVelocity(OBJECT_BALL).getX())
                <= 2*SS->getBallRand()*getBallSpeed() &&
                fabs(velGlobal->getY() - getGlobalVelocity(OBJECT_BALL).getY())
                <= 2*SS->getBallRand()*getBallSpeed() )
        {
            *velGlobal = (*velGlobal + getGlobalVelocity(OBJECT_BALL))/2.0;
            Log.log( 458, "average ball vel to (%f,%f)", velGlobal->getX(),
                     velGlobal->getY() );
        }
    }
    else if ( getRelativeDistance(OBJECT_BALL) < SS->getVisibleDistance() &&
              getTimeLastSeeMessage() - Ball.getTimeGlobalPosDerivedFromSee() < 3)
    {
        // no change information but have got feel info -> use position based vel.
        // get the difference with the previous known global position
        // and subtract the position difference (this difference is caused by
        // the fact that the global position calculation contains a lot of noise
        // now the noise is filtered, since we compare the velocity as if its
        // position was seen "with the noise" of the last cycle).

        VecPosition posGlobalDiff   = *posGlobal - Ball.getGlobalPositionLastSee()
                                      - agentObject.getPositionDifference();
        Log.log( 101, "2 pos: ball(%f,%f), ball_prev(%f,%f), agentdiff(%f,%f)",
                 posGlobal->getX(), posGlobal->getY(),
                 Ball.getGlobalPositionLastSee().getX(),
                 Ball.getGlobalPositionLastSee().getY(),
                 agentObject.getPositionDifference().getX(),
                 agentObject.getPositionDifference().getY() );

        // difference in global positions is distance traveled, we have to make
        // distinction whether this distance is traveled in one or two cycles.
        // 1 cycle:
        //   distance difference equals last velocity so only have to multiply with
        //   decay
        // 2 cycles:
        //   do not update, since kick can be performed causing large change
        //   of which we have no see information (so thus use sense update)

        if ( getTimeLastSeeMessage() - Ball.getTimeGlobalPosDerivedFromSee() == 1 &&
                m_bWasCollision == false )
        {
            *velGlobal = posGlobalDiff*SS->getBallDecay();
            Log.log( 458, "vel based on 2 pos: (%f,%f)", velGlobal->getX(),
                     velGlobal->getY() );
            Log.log( 101, "vel based on 2 pos: (%f,%f)", velGlobal->getX(),
                     velGlobal->getY() );
        }
        else if (getTimeLastSeeMessage()-Ball.getTimeGlobalPosDerivedFromSee()==2 &&
                 m_bWasCollision == false )
        {
            VecPosition velTmp, posTmp;
            double      dDist;
            // we have seen ball for the last time two cycles ago
            // name v velocity after first cycle, vel. now is then v*d^2.
            // v can be calculated as v + v * d = diff -> v = diff / ( 1 + d)

            // velTmp is now distance traveled in previous cycle.
            // get position in previous cycle.
            // if no opponent or I could have shot the ball, update velocity.
            velTmp = (posGlobalDiff/(1+SS->getBallDecay()))*SS->getBallDecay();
            posTmp = *posGlobal - velTmp;
            getClosestInSetTo( OBJECT_SET_PLAYERS, posTmp, &dDist );
            if ( dDist > SS->getMaximalKickDist() &&
                    m_bPerformedKick == false &&
                    (getCurrentTime() - m_timeLastCollision) > 3 )
            {
                *velGlobal = velTmp * SS->getBallDecay();
                Log.log( 458, "vel based on 3 pos: (%f,%f)", velGlobal->getX(),
                         velGlobal->getY() );
            }
        }
        else if ( getTimeLastSeeMessage()-Ball.getTimeGlobalPosDerivedFromSee() > 2)
        {
#ifdef WIN32
            Log.log( 20, "(WorldModel:%s) time difference too large" ,
                     "calculateStateBall" );
#else
            Log.log( 20, "(WorldModel:%s) time difference too large" ,__FUNCTION__ );
#endif

        }
    }
    else
        Log.log( 458, "vel ball not updated", velGlobal->getX(),velGlobal->getY());
    // object too far away do not estimate velocity, sense has updated it
    // already

    // change velocity when the ball
    //   has been catched or play mode is not play_on
    //   is in kickable distance opponent
    //   higher than max speed with added noise
    // change position when it is a kick_in: know y coordinate
    // change position when it is a back_pass_[rl]: know positions
    if ( getTimeSinceLastCatch() < 2 ||
            (getPlayMode() != PM_PLAY_ON && !isGoalKickUs() && !isGoalKickThem() &&
             !isPenaltyUs() && !isPenaltyThem() ))
        velGlobal->setMagnitude( 0.0 );
    else if ( getNrInSetInCircle( OBJECT_SET_OPPONENTS,
                                  Circle(*posGlobal,SS->getMaximalKickDist())) > 0
              && getRelativeDistance( OBJECT_BALL ) > SS->getMaximalKickDist() )
        velGlobal->setMagnitude( 0.0 );
    else if ( velGlobal->getMagnitude() >
              ( 1.0 + SS->getBallRand() )*SS->getBallSpeedMax() )
        velGlobal->setMagnitude( SS->getBallSpeedMax() );

    if ( isKickInUs() || isKickInThem() )
        posGlobal->setY( sign( posGlobal->getY() ) * PITCH_WIDTH/2.0 );
    else if ( isBackPassUs() )
        posGlobal->setVecPosition( - PENALTY_X,
                                   sign(posGlobal->getY())*PENALTY_AREA_WIDTH/2.0);
    else if ( isBackPassThem() )
        posGlobal->setVecPosition( + PENALTY_X,
                                   sign(posGlobal->getY())*PENALTY_AREA_WIDTH/2.0);

    Log.log( 458, "final ball vel: (%f,%f)",velGlobal->getX(),velGlobal->getY());
    if ( getRelativeDistance(OBJECT_BALL) < SS->getVisibleDistance() )
        Log.log( 101, "direction old: %f, new: %f",
                 ( getGlobalPosition( OBJECT_BALL ) -
                   getAgentGlobalPosition()).getDirection(),
                 ( *posGlobal - getAgentGlobalPosition()).getDirection() );
    return true;
}



/*! This method determines the current state of a player and is called after
    a see message has arrived.
    \param o object type of the player
    \param *posGlobal will be filled with global position of the player
    \param *velGlobal will be filled with global velocity of the player
    \return bool indicating whether calculations succeeded. */
bool WorldModel::calculateStatePlayer( ObjectT o, VecPosition *posGlobal,
                                       VecPosition *velGlobal )
{
    PlayerObject *pob = (PlayerObject*) getObjectPtrFromType( o );
    if ( pob == NULL )
        return false;

    // set the global position of this dynamic object as follows:
    //  - get the relative position from the agent to it in world-axis
    //  - add global position agent to this relative position
    VecPosition posRelWorld =
        VecPosition( getRelativeDistance( o ),
                     getRelativeAngle( o ) + agentObject.getGlobalNeckAngle(),
                     POLAR );
    *posGlobal = getAgentGlobalPosition() + posRelWorld;

    *velGlobal = getGlobalVelocity( o ) ;
    if ( pob->getTimeChangeInformation( ) == getTimeLastSeen( o ) )
    {
        // calculate the global velocity using the distance and angle change
        // with the formula from the soccermanual
        *velGlobal = calculateVelocityDynamicObject( o );
    }
    else
        ; // object too far away do not estimate velocity, sense has updated it
    // already and does not really matter then

    if ( velGlobal->getMagnitude() >=
            ( 1.0 + SS->getPlayerRand())*SS->getPlayerSpeedMax() )
        velGlobal->setMagnitude( SS->getPlayerSpeedMax() );

    return true;
}


/*! This method determines the minimum and maximum input values that
    will produce a quantized (noise used in the soccer server)
    distance of 'dOutput'.  With other words, this is the range of
    values that will have the same quantized value 'dOutput'. The
    quantized steps are defined by x1 and x2.  See the soccer server
    manual for details.

    \param dOutput resulting quantized value
    \param *dMin will be filled with minimum possible value
    \param *dMax will be filled with maximum possible value
    \param x1 value of inner quantize call-0.1 for player/ball, 0.01 for flags
    \param x2 value of outer quantize call (normally 0.1)
    \return bool indicating whether values were filled correctly */
bool WorldModel::getMinMaxDistQuantizeValue( double dOutput, double *dMin,
        double *dMax,   double x1, double x2 )
{
    // change output a little bit to circumvent boundaries
    // q = quantize(e^(quantize(ln(V),x1)),x2) with quantize(V,Q) = rint(V/Q)*Q
    // e^(quantize(ln(V),x1)_min = invQuantize( q, x2 )
    // quantize(ln(V),x1) = ln ( invQuantize( q, x2 ) )
    // ln(V) = invQuantize( ln ( invQuantize( q, x2 ) ), x1 )
    // V_min = e^( invQuantize( ln ( invQuantize( q, x2 ) ), x1 ) )
    // apply inverse quantize twice to get correct value
    dOutput -= 1.0e-10;
    if ( dOutput < 0 )
        *dMin = 0.0;
    else
        *dMin = exp( invQuantizeMin( log( invQuantizeMin(dOutput,x2) ), x1 ) );
    dOutput += 2.0e-10;
    *dMax = exp( invQuantizeMax( log( invQuantizeMax(dOutput,x2) ), x1 ) );
    return true;
}

/*! This method determines the minimum and maximum input values that will
    produce a quantized (noise used in the soccer server) for the direction
    change of 'dOutput'.
    See the soccer server manual for details.
    \param dOutput resulting quantized value for direction change
    \param *dMin will be filled with minimum possible value
    \param *dMax will be filled with maximum possible value
    \param x1 value of outer quantize call (normally 0.1)
    \return bool indicating whether values were filled correctly */
bool WorldModel::getMinMaxDirChange( double dOutput, double *dMin,
                                     double *dMax,   double x1     )
{
    *dMin = invQuantizeMin( dOutput, x1 ) ;
    *dMax = invQuantizeMax( dOutput, x1 ) ;
    return true;
}

/*! This method determines the minimum and maximum input values that will
    produce a quantized (noise used in the soccer server) for the distance
    change of 'dOutput'.
    See the soccer server manual for details.
    \param dOutput resulting quantized value for direction change
    \param dDist distance to the perceived object
    \param *dMin will be filled with minimum possible value
    \param *dMax will be filled with maximum possible value
    \param x1 quantize step for distance change
    \param xDist1 value of inner quantize call to determine quantize value
                  corresponding to distance
    \param xDist2 value of outer quantize call to determine quantize value
                  corresponding to distance
    \return bool indicating whether values were filled correctly */
bool WorldModel::getMinMaxDistChange( double dOutput, double dDist,
                                      double *dMin, double *dMax, double x1, double xDist1, double xDist2)
{
    // Q_dist = quantize(e^(quantize(ln(V),xDist1)),xDist2)
    // q = Q_dist * Quantize( distance_change/distance, x1 )
    // dOutput = q/Q_dist = Quantize( distance_change/distance, x1 )
    // (distance_change/distance)_min = invQmin(q/Q_dist, x1 )
    // real distance is not know so should take into account distance range
    double dMinDist, dMaxDist;
    getMinMaxDistQuantizeValue( dDist, &dMinDist, &dMaxDist, xDist1, xDist2 );
    dOutput = dOutput/dDist;
    double dMinCh = invQuantizeMin( dOutput, x1 ) ;
    double dMaxCh = invQuantizeMax( dOutput, x1 ) ;
    *dMin = min( dMinDist*dMinCh, dMaxDist*dMinCh );
    *dMax = max( dMinDist*dMaxCh, dMaxDist*dMaxCh );
    return true;
}

/*! This method returns the minimum value that generates dOutput as a quantized
    value when 'dQuantizeStep' is used as the quantized step.
    \param dOutput quantized output
    \param dQuantizeStep quantize step
    \return minimum value that when quantized produces 'dOutput' */
double WorldModel::invQuantizeMin( double dOutput, double dQuantizeStep )
{
    // q = quantize( V, Q ) = rint(V/Q)*Q -> q/Q = rint( V/Q)
    // min = rint(q/Q)-0.5 = V_min/Q -> V_min = (rint(q/Q)-0.5)*Q
    return max(1.0e-10,(rint( dOutput / dQuantizeStep )-0.5 )*dQuantizeStep);
}

/*! This method returns the maximum value that generates dOutput as a quantized
    value when 'dQuantizeStep' is used as the quantized step.
    \param dOutput quantized output
    \param dQuantizeStep quantize step
    \return maximum value that when quantized produces 'dOutput' */
double WorldModel::invQuantizeMax( double dOutput, double dQuantizeStep )
{
    // q = quantize( V, Q ) = rint(V/Q)*Q -> q/Q = rint( V/Q)
    // max = rint(q/Q)+0.5 = V_max/Q -> V_max = (rint(q/Q)-0.5)*Q
    return (rint( dOutput/dQuantizeStep) + 0.5 )*dQuantizeStep;
}

/*! This method maps the information in the array of unknown players
    (players of which we do not know the number and/or team) to the
    player information located in the WorldModel. This is done by
    comparing the predicted position of players we haven't seen this
    cycle and the information of players of which we do not have the
    number and/or teamname.If this difference in the distance is
    smaller than tolerated (see
    PlayerSettings::getPlayerDistTolerance()), this player information
    is updated with the specified information.

    \param time time of the current cycle */
void WorldModel::mapUnknownPlayers( Time time)
{
    double      dDist, dMinDist;
    VecPosition pos, posAgent = getAgentGlobalPosition();
    ObjectT     o, o_new, objTmp;
    int         index;

    // for all unknown players, try to map it to closest teammate or opponent
    for ( int j = 0; j < iNrUnknownPlayers; j ++ )
    {
        pos = posAgent + VecPosition( UnknownPlayers[j].getRelativeDistance(),
                                      VecPosition::normalizeAngle( getAgentGlobalNeckAngle() +
                                                                   UnknownPlayers[j].getRelativeAngle() ),
                                      POLAR );
        dMinDist = 1000.0;
        o     = UnknownPlayers[j].getType();
        o_new = OBJECT_ILLEGAL;
        Log.log( 464, "map unknown player: %d %f %f (%f,%f) neck %f",
                 o,
                 UnknownPlayers[j].getRelativeDistance(),
                 UnknownPlayers[j].getRelativeAngle(),
                 pos.getX(), pos.getY(),
                 getAgentGlobalNeckAngle() );
        if ( ! SoccerTypes::isOpponent( o ) ) // TEAMMATE_UNKNOWN or PLAYER_UNKNOWN
        {
            for ( int i = 0 ; i < MAX_TEAMMATES ; i++ )
            {
                objTmp = Teammates[i].getType();
                if ( isConfidenceGood( objTmp ) && getTimeLastSeen( objTmp ) != time &&
                        objTmp != getAgentObjectType()  )
                {
                    dDist = pos.getDistanceTo( Teammates[i].getGlobalPosition( ) );
                    Log.log( 464, "distance with %d %f (%f,%f)", objTmp, dDist,
                             Teammates[i].getRelativeDistance(),
                             Teammates[i].getRelativeAngle()  );
                    if ( dDist < dMinDist )
                    {
                        o_new    = objTmp;
                        dMinDist = dDist;
                    }
                }
                else
                    Log.log( 464, "not possible: distance with %d (%f,%f) conf %d  \
                         last_seen (%d,%d), now (%d,%d)",
                             objTmp,
                             Teammates[i].getRelativeDistance(),
                             Teammates[i].getRelativeAngle(),
                             isConfidenceGood( objTmp ),
                             getTimeLastSeen( objTmp ).getTime(),
                             getTimeLastSeen( objTmp ).getTimeStopped(),
                             time.getTime(),
                             time.getTimeStopped());

            }
        }
        if ( ! SoccerTypes::isTeammate( o ) ) // OPPONENT_UNKNOWN or PLAYER_UNKNOWN
        {
            for ( int i = 0 ; i < MAX_OPPONENTS ; i++ )
            {
                objTmp = Opponents[i].getType();
                if ( isConfidenceGood( objTmp ) && getTimeLastSeen( objTmp ) != time )
                {
                    dDist = pos.getDistanceTo( Opponents[i].getGlobalPosition( ) );
                    Log.log( 464, "distance with %d %f (%f,%f)", objTmp, dDist,
                             Opponents[i].getRelativeDistance(),
                             Opponents[i].getRelativeAngle()  );
                    if ( dDist < dMinDist )
                    {
                        o_new    = objTmp;
                        dMinDist = dDist;
                    }
                }
                else
                    Log.log( 464, "not possible: distance with %d (%f,%f) conf %d  \
                         last_seen (%d,%d), now (%d,%d)",
                             objTmp,
                             Opponents[i].getRelativeDistance(),
                             Opponents[i].getRelativeAngle(),
                             isConfidenceGood( objTmp ),
                             getTimeLastSeen( objTmp ).getTime(),
                             getTimeLastSeen( objTmp ).getTimeStopped(),
                             time.getTime(),
                             time.getTimeStopped());
            }
        }

        Log.log( 464, "closest obj %d, found %f, max_move %f rel_dist %f type %d",
                 o_new, dMinDist, getMaxTraveledDistance( o_new ),
                 UnknownPlayers[j].getRelativeDistance(),
                 UnknownPlayers[j].getType() );

        // if player found and in tolerated distance update player
        // information.  else if not mapped to player put information in
        // first player position of which we have no info.  if we do not
        // know it is a teammate or opponent only assume opponent when it
        // is very close since then it is probably an opponent trying to
        // get ball
        if ( SoccerTypes::isKnownPlayer(o_new)
                && dMinDist < PS->getPlayerDistTolerance()
                && dMinDist < getMaxTraveledDistance( o_new ) + 2 ) // 2 for the noise
        {
            UnknownPlayers[j].setType( o_new );
            if ( SoccerTypes::isTeammate( o_new ) )
            {
                index = SoccerTypes::getIndex(o_new);
                UnknownPlayers[j].setHeteroPlayerType(
                    Teammates[index].getHeteroPlayerType( ) );
                Teammates[index] = UnknownPlayers[j];
                Log.log( 464, "map to known teammate %d (%f,%f) conf %f", index + 1,
                         Teammates[index].getGlobalPosition().getX(),
                         Teammates[index].getGlobalPosition().getY(),
                         Teammates[index].getConfidence( getCurrentTime() ) );
            }
            else if ( SoccerTypes::isOpponent( o_new ) )
            {
                index = SoccerTypes::getIndex(o_new );
                UnknownPlayers[j].setHeteroPlayerType(
                    Opponents[index].getHeteroPlayerType( ) );
                Opponents[index] = UnknownPlayers[j];
                Log.log( 464, "map to known opponent %d (time %d)",
                         index + 1, UnknownPlayers[j].getTimeLastSeen().getTime() );
            }
        }
        else if ( UnknownPlayers[j].getType() == OBJECT_TEAMMATE_UNKNOWN )
        {
            o_new = getFirstEmptySpotInSet( OBJECT_SET_TEAMMATES, j );

            if ( o_new != OBJECT_ILLEGAL )
            {
                index = SoccerTypes::getIndex(o_new);
                UnknownPlayers[j].setHeteroPlayerType(
                    Teammates[index].getHeteroPlayerType( ) );
                UnknownPlayers[j].setType( o_new );
                Teammates[index] = UnknownPlayers[j];
                Log.log( 464, "map to unknown teammate %d", index + 1 );
            }
        }
        else if ( UnknownPlayers[j].getType() == OBJECT_OPPONENT_UNKNOWN )
        {                                      // could not map info to a player
            o_new = getFirstEmptySpotInSet( OBJECT_SET_OPPONENTS, j );
            Log.log( 464, "map unkown opponent to %d", o_new );
            if ( o_new != OBJECT_ILLEGAL )
            {
                index = SoccerTypes::getIndex(o_new);
                UnknownPlayers[j].setHeteroPlayerType(
                    Opponents[index].getHeteroPlayerType( ) );
                UnknownPlayers[j].setType( o_new );
                Opponents[index] = UnknownPlayers[j];
                Log.log( 464, "map to unknown opponent %d", index + 1 );
            }
            else
            {
                Log.log( 464, "couldn't find empty spot for unk. opponent" );
                if ( Log.isInLogLevel( 464 ) )
                    show( OBJECT_SET_PLAYERS, Log.getOutputStream() );
//        if( getRelativeDistance( OBJECT_BALL ) < 3.0 )
//        cerr << getPlayerNumber() << ", " <<
//          getCurrentCycle() << " couldn't find empty spot" << endl;
            }
        }
        else if ( UnknownPlayers[j].getType() == OBJECT_PLAYER_UNKNOWN &&
                  ( UnknownPlayers[j].getRelativeDistance() < SS->getVisibleDistance()
                    || ( dMinDist - getMaxTraveledDistance( o_new ) ) > 10   ) )
        {
            o_new = getFirstEmptySpotInSet( OBJECT_SET_OPPONENTS );
            Log.log( 464, "map unkown player to %d", o_new );
            if ( o_new != OBJECT_ILLEGAL )
            {
                index = SoccerTypes::getIndex(o_new);
                UnknownPlayers[j].setHeteroPlayerType(
                    Opponents[index].getHeteroPlayerType( ) );
                UnknownPlayers[j].setType( o_new );
                Opponents[index] = UnknownPlayers[j];
                Log.log( 464, "map to unknown close opponent %d", index + 1 );
            }
        }
    }
    Log.log( 464, "end map unknown player" );

    iNrUnknownPlayers  = 0;
}

/*! This method updates the ServerSettings of this agent using the player type
    information of the heterogeneous player at index 'iIndex'. This method is
    usually called when the player type of the agent is changed by the coach.
    It updates its parameters, such that all calculations to determine the
    next action are based on the correct parameters.
    \param iIndex index of the new player type
    \return bool indicating whether update was succesfull. */
bool WorldModel::updateSSToHeteroPlayerType( int iIndex )
{
    SS->setPlayerSpeedMax( pt[iIndex].dPlayerSpeedMax );
    SS->setStaminaIncMax ( pt[iIndex].dStaminaIncMax  );
    SS->setPlayerDecay   ( pt[iIndex].dPlayerDecay    );
    SS->setInertiaMoment ( pt[iIndex].dInertiaMoment  );
    SS->setDashPowerRate ( pt[iIndex].dDashPowerRate  );
    SS->setPlayerSize    ( pt[iIndex].dPlayerSize     );
    SS->setKickableMargin( pt[iIndex].dKickableMargin );
    SS->setKickRand      ( pt[iIndex].dKickRand       );
    SS->setExtraStamina  ( pt[iIndex].dExtraStamina   );
    SS->setEffortMax     ( pt[iIndex].dEffortMax      );
    SS->setEffortMin     ( pt[iIndex].dEffortMin      );

    return true;
}

/*! This methods resets all the object information stored in the worldmodel.
    This means that the last see time of these objects are set to UnknownTime.
    \return bool indicating whether update was succesfull */
bool WorldModel::resetTimeObjects( )
{
    Ball.setTimeLastSeen             ( Time( -1, 0) );
    for ( int i = 0 ; i < MAX_TEAMMATES ; i ++ )
        Teammates[i].setTimeLastSeen   ( Time( -1, 0) );
    for ( int i = 0 ; i < MAX_OPPONENTS ; i ++ )
        Opponents[i].setTimeLastSeen   ( Time( -1, 0) );
    for ( int i = 0 ; i < MAX_FLAGS     ; i ++ )
        Flags[i].setTimeLastSeen   ( Time( -1, 0) );
    for ( int i = 0 ; i < MAX_LINES     ; i ++ )
        Lines[i].setTimeLastSeen   ( Time( -1, 0) );
    agentObject.setTimeLastSeen      ( Time( -1, 0) );
    return true;
}

/*! This method removes ghosts from the WorldModel. Ghosts are objects that
    should have been seen in the last see message but aren't. Currently only
    the ball is removed from the WorldMOdel. */
void WorldModel::removeGhosts( )
{
    AngDeg dAngle=SoccerTypes::getHalfViewAngleValue(agentObject.getViewAngle());
    dAngle -= 0.35*dAngle; // make somewhat smaller for error
    VecPosition posAgent = getAgentGlobalPosition();

    if ( fabs( getRelativeAngle( OBJECT_BALL ) ) < dAngle
            && getTimeLastSeen( OBJECT_BALL ) != getTimeLastSeeMessage() )
    {
        double dDist;
        ObjectT objOpp=getClosestInSetTo(OBJECT_SET_OPPONENTS,OBJECT_BALL,&dDist);
        // when opp is very close and ball is not seen (and also not felt) set it
        // at the opponent position
        if ( dDist < 2.0 &&
                posAgent.getDistanceTo( getGlobalPosition( objOpp ) ) < 4.5  &&
                getTimeLastSeen( objOpp ) != getTimeLastSeeMessage() )
        {
            Log.log( 556, "ball not seen, but opp close, set ball to that pos" );
            Ball.setGlobalPosition( getGlobalPosition( objOpp ),
                                    Ball.getTimeGlobalPosition() );
        }
        else
        {
            Log.log( 556, "ball not in cone: set time ball at -1 %f %f",
                     fabs( getRelativeAngle( OBJECT_BALL ) ), dAngle );
            Ball.setTimeLastSeen( Time( -1, 0 ) );
        }
    }

    // ball should be "seen" when it is in visible distance. 0.9 is for noise
    if ( fabs( getRelativeDistance( OBJECT_BALL ) ) < 0.9*SS->getVisibleDistance()
            && getTimeLastSeen( OBJECT_BALL ) != getTimeLastSeeMessage() )
    {
        double dDist;
        ObjectT objOpp=getClosestInSetTo(OBJECT_SET_OPPONENTS,OBJECT_BALL,&dDist);
        if ( dDist < 2.0 )
        {
            Log.log( 556, "ball not seen, but opp close, set ball to that pos" );
            Ball.setGlobalPosition( getGlobalPosition( objOpp ),
                                    Ball.getTimeGlobalPosition() );
        }
        else
        {
            Log.log( 556, "ball not in vis. dist: set time ball at -1 %f %f",
                     fabs( getRelativeAngle( OBJECT_BALL ) ), dAngle );
            Ball.setTimeLastSeen( Time( -1, 0 ) );
        }
    }

    // now remove all opponents. If an opponent is not seen while you expect him
    // somewhere, he has moved a lot. It's no use keeping his current position
    // in the world model, since he would still be considered in passing
    // confidence, etc.
    int iIndex;
    for ( ObjectT o = iterateObjectStart( iIndex, OBJECT_SET_OPPONENTS );
            o != OBJECT_ILLEGAL;
            o = iterateObjectNext ( iIndex, OBJECT_SET_OPPONENTS ) )
    {
        if (  fabs( getRelativeAngle( o ) ) < dAngle
//          || getRelativeDistance( o ) < SS->getVisibleDistance() )
                && getTimeLastSeen( o ) != getTimeLastSeeMessage() &&
                o != getOppGoalieType() )
        {
            Log.log( 556, "opponent %d not in cone: (angle %f>%f) see (%d,%d)",
                     SoccerTypes::getIndex( o ) + 1,
                     fabs( getRelativeAngle( o ) ), dAngle,
                     getTimeLastSeeMessage().getTime(),getTimeLastSeen(o).getTime());

            AngDeg ang =
                SoccerTypes::getHalfViewAngleValue(agentObject.getViewAngle());
            AngDeg angOpp = (getGlobalPosition(o)-posAgent).getDirection();
            AngDeg angNeck = getAgentGlobalNeckAngle();
            // could have move both ways
            if ( fabs(VecPosition::normalizeAngle( angOpp - angNeck )) < 10 &&
                    getCurrentTime() - getTimeGlobalAngles(o) < 4 )
                angOpp = (getGlobalPosition(o)+
                          VecPosition(2.0, getGlobalBodyAngle(o), POLAR )-posAgent)
                         .getDirection();
            if ( VecPosition::normalizeAngle( angOpp - angNeck )  > 0 )
                angOpp = angNeck + ang;
            else
                angOpp = angNeck - ang;
            angOpp = VecPosition::normalizeAngle( angOpp );
            DynamicObject *obj = (DynamicObject*) getObjectPtrFromType( o );

            // if at edge change him otherwise delete
            if ( fabs( getRelativeAngle( o ) ) + 20 > dAngle )
            {
                VecPosition posNew = posAgent+
                                     VecPosition(getRelativeDistance(o),angOpp, POLAR);
                if ( posNew.getDistanceTo( getGlobalPosition( o ) ) <
                        getMaxTraveledDistance( o ) )
                {
                    obj->setGlobalPosition( posNew, getCurrentTime());
                    //          obj->setGlobalPosition( posNew, getTimeLastSeen(o));
                    Log.log( 556, "set opp at angle %f", angOpp );
                    updateObjectRelativeFromGlobal( o );
                }
                else
                    Log.log( 556, "do not reposition opp too far dist %f",
                             posNew.getDistanceTo( getGlobalPosition( o ) ));
            }
            else
            {
                setTimeLastSeen( o, Time( -1, 0 ) );
                Log.log( 556, "remove opponent not in cone at angle %f %f %f", angOpp,
                         fabs(getRelativeAngle(o)), dAngle  );
            }
        }
        if ( getRelativeDistance(o) < SS->getVisibleDistance( )
                && getTimeLastSeen( o ) != getTimeLastSeeMessage() )
        {
            Log.log( 556, "opp %d not felt, place him outside vis. dist", o );
            DynamicObject *obj = (DynamicObject*) getObjectPtrFromType( o );
            VecPosition posNew = posAgent + VecPosition( SS->getVisibleDistance(),
                                 (getGlobalPosition( o ) - getAgentGlobalPosition()).
                                 getDirection(), POLAR );
            obj->setGlobalPosition( posNew, getCurrentTime(  ));
            //      obj->setGlobalPosition( posNew, getTimeLastSeen( o ));
            updateObjectRelativeFromGlobal( o );
        }
    }
    iterateObjectDone( iIndex );

    // now remove all teammates. If a teammate is not seen while you expect him
    // somewhere, he has moved a lot. It's no use keeping his current position
    // in the world model, since he would still be considered in passing
    // confidence, etc.
    for ( ObjectT o = iterateObjectStart( iIndex, OBJECT_SET_TEAMMATES );
            o != OBJECT_ILLEGAL;
            o = iterateObjectNext ( iIndex, OBJECT_SET_TEAMMATES ) )
    {
        if ( fabs( getRelativeAngle( o ) ) < dAngle
//       ||getRelativeDistance( o ) < SS->getVisibleDistance() )
                && getTimeLastSeen( o ) != getTimeLastSeeMessage() )
        {
            Log.log( 556, "teammate %d not in cone: (angle %f>%f) see (%d,%d)",
                     SoccerTypes::getIndex( o ) + 1,
                     fabs( getRelativeAngle( o ) ), dAngle,
                     getTimeLastSeeMessage().getTime(), getTimeLastSeen( o ).getTime() );
            setTimeLastSeen( o, Time( -1, 0 ) );
        }
    }
    iterateObjectDone( iIndex );

}

/*! This method initializes all particles that represent the global position
    and global velocity of the ball. This is done by taking the last
    perceived information and generate particles within the range that can
    produce the perceived values. Note that each position is related to the
    velocity with the same index. The state of the ball is implicitly
    represented as a 4-tuple (pos_x, pos_y, vel_x, vel_y).
    \param posArray array that will be filled with the position particles
    \param velArray array that will be filled with the velocity particles
    \param iLength number of particles that have to be initialized. */
void WorldModel::initParticlesBall( VecPosition posArray[],
                                    VecPosition velArray[], int iLength )
{
    // declare a bunch of variables
    double dDistBall, dDistChange = UnknownDoubleValue;
    AngDeg angBall, angChange   = UnknownAngleValue;
    double dMinDist, dMaxDist, dMinCh, dMaxCh, dDistTmp, dDistChTmp, dVelX,dVelY;
    AngDeg angChMin, angChMax,     angTmp,   angChTmp;

    // no information  received -> no initialization
    if ( Ball.getTimeRelativePosition() != getTimeLastSeeMessage() )
        return;

    // get perceived values for distance and angle with ball
    dDistBall = getRelativeDistance( OBJECT_BALL );
    angBall   = getRelativeAngle( OBJECT_BALL );

    // get perceived values for distance and direction change
    if ( Ball.getTimeChangeInformation( ) == getTimeLastSeeMessage() )
    {
        dDistChange = Ball.getRelativeDistanceChange();
        angChange   = Ball.getRelativeAngleChange();
    }

    // get ranges from which values could originate
    getMinMaxDistQuantizeValue( dDistBall, &dMinDist, &dMaxDist, 0.1, 0.1 );
    getMinMaxDistChange( dDistChange, dDistBall, &dMinCh, &dMaxCh, 0.02, 0.1,0.1);
    getMinMaxDirChange ( angChange, &angChMin, &angChMax, 0.1 );

    for ( int i = 0; i < iLength; i ++ )
    {
        // make random distance and angle from range (angle is rounded)
        // and make pos
        dDistTmp   = dMinDist + drand48()*fabs(dMaxDist - dMinDist); // angle->sign
        angTmp     = angBall  + drand48() - 0.5;

        posArray[i].setVecPosition( dDistTmp, angTmp, POLAR );
        posArray[i].relativeToGlobal( getAgentGlobalPosition(),
                                      getAgentGlobalNeckAngle() );

        if ( dDistChange != UnknownDoubleValue )
        {
            // make random values for direction and distance change and make velocity
            angChTmp   = angChMin + drand48()*(angChMax-angChMin);
            dDistChTmp = dMinCh   + drand48()*(dMaxCh-dMinCh);

            dVelX=dDistChTmp*
                  cosDeg(angTmp)-Deg2Rad(angChTmp)*dDistTmp*sinDeg(angTmp);
            dVelY=dDistChTmp*
                  sinDeg(angTmp)+Deg2Rad(angChTmp)*dDistTmp*cosDeg(angTmp);

            velArray[i].setVecPosition( dVelX, dVelY );
            velArray[i].relativeToGlobal( getAgentGlobalVelocity(),
                                          getAgentGlobalNeckAngle() );
        }
        else
            velArray[i].setVecPosition( 0, 0 );
    }
}

/*! This method checks all particles that represent the global position
    and global velocity of the ball. This is done by using the last
    perceived information. Using the possible ranges from which these values
    coule originate from, it can be checked which particles are legal. These
    particles are moved to the beginning of the array. Afterwards the number of
    legal particles is stored in 'iNrParticlesLeft'.
    \param posArray array that contains the position particles
    \param velArray array that contains the velocity particles
    \param iLength number of particles that have to be checked.
    \param iNrParticlesLeft will contain the number of legal particles*/
void WorldModel::checkParticlesBall( VecPosition posArray[],
                                     VecPosition velArray[], int iLength, int *iNrParticlesLeft )
{
    bool   bIllegal;
    double dMinDist, dMaxDist, dMinCh, dMaxCh, dMag;
    double dDistBall, dDistChange = UnknownDoubleValue;
    AngDeg angBall,  angChange;
    double dDistChTmp;
    AngDeg angChTmp, angChMin, angChMax;
    VecPosition pos_rel, vel_rel;

    // no new perceptions, do not check
    if ( getTimeLastSeen( OBJECT_BALL ) != getTimeLastSeeMessage() )
        return;

    // initialize values distance, direction, distance change and
    // direction change and get the associated ranges
    dDistBall = getRelativeDistance( OBJECT_BALL );
    angBall   = getRelativeAngle( OBJECT_BALL );
    getMinMaxDistQuantizeValue( dDistBall, &dMinDist, &dMaxDist, 0.1, 0.1 );

    if ( getTimeLastSeen( OBJECT_BALL ) == Ball.getTimeChangeInformation( ) )
    {
        dDistChange = Ball.getRelativeDistanceChange();
        angChange   = Ball.getRelativeAngleChange();
        getMinMaxDirChange ( angChange, &angChMin, &angChMax, 0.1);
        getMinMaxDistChange( dDistChange,dDistBall, &dMinCh, &dMaxCh,0.02,0.1,0.1);

    }

    *iNrParticlesLeft = 0;
    for ( int i = 0; i < iLength; i ++ )
    {
        // get particles and make them relative to the agent to compare
        pos_rel = posArray[i];
        vel_rel = velArray[i];
        pos_rel.globalToRelative( getAgentGlobalPosition(),
                                  getAgentGlobalNeckAngle() );
        vel_rel.globalToRelative( getAgentGlobalVelocity(),
                                  getAgentGlobalNeckAngle() );

        bIllegal = false;

        dMag = pos_rel.getMagnitude();
        if ( dMag <  dMinDist || dMag > dMaxDist )
        {
            bIllegal = true;
        }
        if ( fabs( VecPosition::normalizeAngle(pos_rel.getDirection() - angBall) )
                > 0.5 )
        {
            bIllegal = true;
        }

        if ( dDistChange != UnknownDoubleValue )
        {
            dDistChTmp = (vel_rel.getX()*(pos_rel.getX()/dMag)) +
                         (vel_rel.getY()*(pos_rel.getY()/dMag));
            angChTmp   = Rad2Deg( ((vel_rel.getY()*pos_rel.getX()/dMag) -
                                   (vel_rel.getX()*pos_rel.getY()/dMag)))/dMag ;

            if ( angChTmp < angChMin || angChTmp > angChMax )
            {
                bIllegal = true;
            }
            if ( dDistChTmp < dMinCh || dDistChTmp > dMaxCh )
            {
                bIllegal = true;
            }
        }

        // if not illegal, save particles and raise iNrParticlesLeft
        if ( bIllegal == false )
        {
            posArray[*iNrParticlesLeft]     = posArray[i];
            velArray[(*iNrParticlesLeft)++] = velArray[i];
        }
    }
}

/*! This method updates all particles that represent the global position
    and global velocity of the ball to the next cycle. This is done using the
    same formula as the soccer server.
    \param posArray array that contains the position particles
    \param velArray array that contains the velocity particles
    \param iLength number of particles that have to be checked.
    \param dPower power with which the ball is accelerated.
    \param ang angle (relative to body) to which the ball is accelerated. */
void WorldModel::updateParticlesBall( VecPosition posArray[],
                                      VecPosition velArray[], int iLength, double dPower, AngDeg ang )
{
    double dRand = SS->getBallRand();
    double dMaxRand;

    for ( int i = 0; i < iLength; i ++ )
    {
        // if power supplied, assume ball (and thus particles) are kicked
        if ( dPower > EPSILON )
        {
            ang = VecPosition::normalizeAngle(ang + getAgentGlobalBodyAngle() );
            velArray[i] += VecPosition(getActualKickPowerRate()*dPower, ang, POLAR) ;
            if ( velArray[i].getMagnitude() > SS->getBallSpeedMax() )
                velArray[i].setMagnitude( SS->getBallSpeedMax() );
        }

        // add noise in same way server does.
        dMaxRand = dRand * velArray[i].getMagnitude();
        velArray[i] += VecPosition(
                           (-1 + 2*drand48())*dMaxRand,
                           (-1 + 2*drand48())*dMaxRand );
        posArray[i] += velArray[i];
        velArray[i] *= SS->getBallDecay();
    }
}


/*! This method resamples all particles that represent the global position
    and global velocity of the ball. This is by copying existing legal
    particles at random. Since noise will be added to each particle, they will
    blur each time the particles are updated. The first 'iLeft' particles are
    legal particles and copied to the position at the end of the array.
    \param posArray array that contains the position particles
    \param velArray array that contains the velocity particles
    \param iLength number of particles that have to be checked.
    \param iLeft number of particles that are legal */
void WorldModel::resampleParticlesBall( VecPosition posArray[],
                                        VecPosition velArray[], int iLength, int iLeft )
{
    int iRand ;
    for ( int i = iLeft; i < iLength; i ++ )
    {
        iRand = (int)(drand48()*iLeft);       // pick random particle
        posArray[ i ] = posArray[ iRand ];    // and copy contents
        velArray[ i ] = velArray[ iRand ];
    }
}

ObjectT WorldModel::getMaxRangeUnknownPlayer( ObjectT obj, char* strMsg )
{
    list<ObjectT> l;
    ObjectT o;
    bool    isGoalie, bCont = true;
    int     i, loop;
//  bool    isTeammate = SoccerTypes::isTeammate( obj );
    ObjectT objMax = (getSide()==SIDE_LEFT) ? OBJECT_OPPONENT_11
                     : OBJECT_TEAMMATE_11 ;

    while ( bCont == true )
    {
        i = Parse::gotoFirstOccurenceOf( '(', &strMsg );
        if ( i == -1 )
            bCont = false;                             // no more objects
        else
        {
            strMsg++;
            o = SoccerTypes::getObjectFromStr(&strMsg,&isGoalie,getTeamName());
            if ( SoccerTypes::isPlayer( o ) )
                l.push_back( o );
        }
    }
    Log.log( 459, "list size %d", l.size() );
    while ( ! l.empty() )
    {
        o = l.back();
        l.pop_back();
        if ( SoccerTypes::isKnownPlayer( o ) )     // max range is one lower
            objMax = o;
        else if ( SoccerTypes::isOpponent( o ) &&
                  SoccerTypes::isTeammate( objMax ) )
            objMax = OBJECT_OPPONENT_11;
        else if ( SoccerTypes::isTeammate( o ) &&
                  SoccerTypes::isOpponent( objMax ) )
            objMax = OBJECT_TEAMMATE_11;

        if ( objMax == getAgentObjectType() )
            loop = 2;
        else
            loop = 1;

        for ( int j = 0; j < loop; j ++ )
        {
            i = SoccerTypes::getIndex( objMax );
            if ( objMax == OBJECT_TEAMMATE_1 )
                objMax = OBJECT_OPPONENT_11;
            else if ( objMax == OBJECT_OPPONENT_1 )
                objMax = OBJECT_TEAMMATE_11;
            else if ( SoccerTypes::isTeammate( objMax ) )
                objMax = SoccerTypes::getTeammateObjectFromIndex( i - 1 );
            else if ( SoccerTypes::isOpponent( objMax ) )
                objMax = SoccerTypes::getOpponentObjectFromIndex( i - 1 );
        }
        Log.log( 459, "processs %d new obj_max: %d", o, objMax );

    }

    return objMax;
}
