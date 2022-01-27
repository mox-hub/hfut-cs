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

/*! \file WorldModelPredict.cpp
<pre>
<b>File:</b>          WorldModelPredict.cpp
<b>ProWoct:</b>       Robocup Soccer Simulation Team: UvA Trilearn
<b>Authors:</b>       Jelle Kok
<b>Created:</b>       12/02/2001
<b>Last Revision:</b> $ID$
<b>Contents:</b>      class definitions of WorldModel. This class contains
               methods that give predictions about future states of the world.
<hr size=2>
<h2><b>Changes</b></h2>
<b>Date</b>             <b>Author</b>          <b>Comment</b>
12/02/2001       Jelle Kok       Initial version created
</pre>
*/

#include <stdio.h>
#include "WorldModel.h"

/*****************************************************************************/
/************************** PREDICTIONS **************************************/
/*****************************************************************************/

/*! This method predicts the state of an player after it performs a specific
    SoccerCommand. The current state of the agent is passed by giving the
    position, velocity and body and neck direction. These arguments are
    updated and after return of this method will contain the new values
    as if the command was performed.
    \param com SoccerCommand that will be performed
    \param pos current position of the object.
    \param vel current velocity of the object
    \param angGlobalBody global body angle
    \param angGlobalBody global neck angle
    \param current stamina information.
    \return boolean which indicates whether values were updated */
bool WorldModel::predictStateAfterCommand( SoccerCommand com,
        VecPosition *pos, VecPosition *vel, AngDeg *angGlobalBody,
        AngDeg *angGlobalNeck, ObjectT obj, Stamina *sta )
{
    switch ( com.commandType ) // based on kind of command, choose action
    {
    case CMD_DASH:
        predictStateAfterDash( com.dPower, pos, vel, sta, *angGlobalBody, obj );
        break;
    case CMD_TURN:
        predictStateAfterTurn( com.dAngle, pos, vel,
                               angGlobalBody, angGlobalNeck, obj, sta);
        break;
    case CMD_TURNNECK: // note that position and velocity are not updated
        *angGlobalNeck = VecPosition::normalizeAngle(*angGlobalNeck+com.dAngle);
        break;
    case CMD_KICK:
    case CMD_CATCH:
    case CMD_TACKLE:
        predictStateAfterDash( 0.0, pos, vel, sta, *angGlobalBody, obj );
        break;
    case CMD_MOVE:
        pos->setVecPosition( com.dX, com.dY );
        vel->setMagnitude( 0.0 );
        break;
    case CMD_ILLEGAL:
        predictStateAfterDash( 0.01, pos, vel, sta, *angGlobalBody, obj );
        break;
    default:
        return false;
    }
    return true;
}

/*! This method predicts the state of the agent after it performs a specific
    SoccerCommand. This method makes use of the method predictInfoAfterCommand.
    All arguments are initialized with the current values of the agent.
    \param com SoccerCommand that will be performed
    \param pos will be filled with updated position of agent
    \param vel will be filled with updated velocity of agent
    \param angGlobalBody will be filled with global body angle of agent
    \param angGlobalBody will be filled with global neck angle of agent
    \param current will be filled with stamina information of agent
    \return boolean which indicates whether values were updated */
bool WorldModel::predictAgentStateAfterCommand( SoccerCommand com,
        VecPosition *pos, VecPosition *vel, AngDeg *angGlobalBody,
        AngDeg *angGlobalNeck, Stamina *sta )
{
    *pos           = getAgentGlobalPosition();
    *vel           = getAgentGlobalVelocity();
    *angGlobalBody = getAgentGlobalBodyAngle();
    *angGlobalNeck = getAgentGlobalNeckAngle();
    *sta           = getAgentStamina();
    predictStateAfterCommand( com, pos, vel, angGlobalBody, angGlobalNeck,
                              getAgentObjectType(), sta );

    return true;
}

/*! This method predicts the state of the agent after it performs a specific
    SoccerCommand. This method makes use of the method predictInfoAfterCommand.
    All arguments are initialized with the current values of the agent.
    \param com SoccerCommand that will be performed
    \param pos will be filled with updated position of agent
    \param vel will be filled with updated velocity of agent
    \param angGlobalBody will be filled with global body angle of agent
    \param angGlobalBody will be filled with global neck angle of agent
    \param current will be filled with stamina information of agent
    \return boolean which indicates whether values were updated */
bool WorldModel::predictObjectStateAfterCommand( ObjectT obj,SoccerCommand com,
        VecPosition *pos, VecPosition *vel, AngDeg *angGlobalBody,
        AngDeg *angGlobalNeck, Stamina *sta )
{
    if ( obj == getAgentObjectType() )
        return predictAgentStateAfterCommand(
                   com, pos, vel, angGlobalBody, angGlobalNeck, sta );
    *pos           = getGlobalPosition( obj );
    *vel           = getGlobalVelocity( obj );
    *angGlobalBody = getGlobalBodyAngle( obj  );
    *angGlobalNeck = getGlobalNeckAngle(obj );
    predictStateAfterCommand( com, pos, vel, angGlobalBody, angGlobalNeck, obj );

    return true;
}

/*! This method returns the global position of the agent after the specified
    command is performed. This method makes use of the method
    'predictAgentInfoAfterCommand'
    \param com SoccerCommand that will be performed.
    \return VecPosition new global position of the agent. */
VecPosition WorldModel::predictAgentPosAfterCommand( SoccerCommand com )
{
    VecPosition p1, p2;
    AngDeg      a1, a2;
    Stamina     sta;
    predictAgentStateAfterCommand( com, &p1, &p2, &a1, &a2, &sta );
    return p1;
}

/*! This method determines the state of a player after a dash command is
    performed. The current state of the player is specified by the passed
    arguments. After this method returns, all arguments are updated.
    \param pos initial position, will be changed to the predicted position
    \param vel intital velocity, will be changed to the predicted velocity
    \param dActualPower actual power that is send with dash command
    \param sta pointer to stamina, when not NULL, effort is used and updated
    \param dDirection direction of dash */
void WorldModel::predictStateAfterDash( double dActualPower, VecPosition *pos,
                                        VecPosition *vel, Stamina *sta, double dDirection, ObjectT obj )
{
    // get acceleration associated with actualpower
    double dEffort = ( sta != NULL ) ? sta->getEffort() : getEffortMax( obj );
    double dAcc    = dActualPower * getDashPowerRate( obj ) * dEffort;

    // add it to the velocity; negative acceleration in backward direction
    if ( dAcc > 0 )
        *vel += VecPosition::getVecPositionFromPolar( dAcc, dDirection );
    else
        *vel += VecPosition::getVecPositionFromPolar( fabs(dAcc),
                VecPosition::normalizeAngle(dDirection+180));

    // check if velocity doesn't exceed maximum speed
    if ( vel->getMagnitude() > SS->getPlayerSpeedMax() )
        vel->setMagnitude( SS->getPlayerSpeedMax() );

    // add velocity to current global position and decrease velocity
    *pos += *vel;
    *vel *= getPlayerDecay(obj);
    if ( sta != NULL )
        predictStaminaAfterDash( dActualPower, sta );
}

/*! This method determines the state of a player after a turn command is
    performed. The global position is updated with the velocity and the
    velocity is updated. Then the actual turn angle is calculated taken the
    inertia into account. This actual turn angle is used to update both
    the global body and global neck direction.
    \param dSendAngle actual angle given in command
    \param pos initial position, will be changed to the predicted position
    \param vel intital velocity, will be changed to the predicted velocity
    \param angBody global body direction
    \param angNeck global neck direction
    \param sta Stamina of player can be NULL */
void WorldModel::predictStateAfterTurn( AngDeg dSendAngle, VecPosition *pos,
                                        VecPosition *vel, AngDeg *angBody, AngDeg *angNeck, ObjectT obj,
                                        Stamina *sta )
{
    // calculate effective turning angle and neck accordingly
    double dEffectiveAngle;
    dEffectiveAngle = getActualTurnAngle( dSendAngle, vel->getMagnitude(), obj );
    *angBody = VecPosition::normalizeAngle( *angBody + dEffectiveAngle );
    *angNeck = VecPosition::normalizeAngle( *angNeck + dEffectiveAngle );

    // update as if dashed with no power
    predictStateAfterDash( 0.0, pos, vel, sta, *angBody, obj );
    return;
}

void WorldModel::predictBallInfoAfterCommand( SoccerCommand soc,
        VecPosition *pos, VecPosition *vel )
{
    VecPosition posBall = getGlobalPosition( OBJECT_BALL ) ;
    VecPosition velBall = getGlobalVelocity( OBJECT_BALL );

    if ( soc.commandType == CMD_KICK )
    {
        int iAng   = (int)soc.dAngle;
        int iPower = (int)soc.dPower;

        // make angle relative to body
        // calculate added acceleration and add it to current velocity
        AngDeg ang = VecPosition::normalizeAngle(iAng+getAgentGlobalBodyAngle());
        velBall += VecPosition( getActualKickPowerRate()*iPower, ang, POLAR ) ;
        if ( velBall.getMagnitude() > SS->getBallSpeedMax() )
            velBall.setMagnitude( SS->getBallSpeedMax() );
        Log.log( 600, "ang: %f kick_rate %f", ang, getActualKickPowerRate() );
        Log.log( 600, "update for kick: %f %f", soc.dPower, soc.dAngle );
    }

    posBall += velBall;
    velBall *= SS->getBallDecay();

    if ( pos != NULL )
        *pos = posBall;
    if ( vel != NULL )
        *vel = velBall;
}

/*! This method determines the global position of the object o after iCycles
    When the object is the ball, only the decay of the ball is taken into
    account. When the object is a player it
    is assumed that the player dashes with 'iDashPower' every cycle.
    \param o objectT of which global position will be predicted
    \param iCycles pos is predicted after this number of cycles
    \param iDashPower dash power that is used every cycle in dash (default 100)
    \param vel will be filled with predicted global velocity after iCycles
    \return predicted global position after iCycles. */
VecPosition WorldModel::predictPosAfterNrCycles( ObjectT o, double dCycles,
        int iDashPower, VecPosition *posIn, VecPosition *velIn, bool bUpdate )
{
    VecPosition vel = ( velIn == NULL ) ? getGlobalVelocity( o ) : *velIn ;
    VecPosition pos = ( posIn == NULL ) ? getGlobalPosition( o ) : *posIn ;

    if ( o == OBJECT_BALL )
    {
        // get the speed and the distance it travels in iCycle's.
        // use this distance and direction it travels in, to calculate new pos
        // geom series is serie s=a+ar+..+ar^n...decay=r,iCycles=n,dSpeed=a
        double dDist = Geometry::getSumGeomSeries( vel.getMagnitude(),
                       SS->getBallDecay(),
                       dCycles);
        pos          += VecPosition( dDist, vel.getDirection(), POLAR );
        vel          *= pow( SS->getBallDecay(), dCycles );
    }
    else if ( SoccerTypes::isKnownPlayer( o )  )
    {
        double      dDirection = 0.0; // used when no info about global body
        Stamina     stamina;          // used when object is agent

        if ( getAgentObjectType() == o )
        {
            dDirection = getAgentGlobalBodyAngle();
            stamina    = getAgentStamina();
        }
        else if ( getTimeGlobalAngles(o) > getCurrentTime() - 2 )
            dDirection = getGlobalBodyAngle(o);

        for ( int i = 0; i < (int)dCycles ; i ++ )
            predictStateAfterDash( iDashPower, &pos, &vel, &stamina, dDirection, o );
    }

    if ( posIn != NULL && bUpdate )
        *posIn = pos;
    if ( velIn != NULL && bUpdate )
        *velIn = vel;

    return pos;
}

/*! This method predicts the position of the agent after 'iCycles' when every
    cycle is dashed with 'iDashPower'. The method
    'predictGlobalPosAfterNrCycles' is used to calculate this position.
    \param iCycles number of cycles
    \param iDashPower dash power that is passed
    \return VecPosition indicating predicted global position of agent. */
VecPosition WorldModel::predictAgentPos( int iCycles, int iDashPower )
{
    return predictPosAfterNrCycles( getAgentObjectType(), iCycles, iDashPower);
}

/*! This method predicts the final position of the agent when no commands
    are issued, but the agent just rolls out.
    \return VecPosition indicating predicted final global position of agent. */
VecPosition WorldModel::predictFinalAgentPos(VecPosition *pos,VecPosition *vel)
{
    VecPosition velAgent   = (vel==NULL) ? getAgentGlobalVelocity (): *vel;
    VecPosition posAgent   = (pos==NULL) ? getAgentGlobalPosition (): *pos;
    double      dDistExtra =
        Geometry::getSumInfGeomSeries(velAgent.getMagnitude(),SS->getPlayerDecay());
    return posAgent + VecPosition(dDistExtra,velAgent.getDirection(), POLAR );

}

/*! This method check how many cycles are needed for object 'o' to travel
    a distance 'dDist' when it currently has a speed 'dSpeed'. */
int WorldModel::predictNrCyclesForDistance ( ObjectT o, double dDist,
        double dSpeed )
{
    double dSpeedPrev = -1.0;
    int    iCycles    = 0;
    double dDecay     = getPlayerDecay( o );
    double dDashRate  = getDashPowerRate( o );
    double dMinDist   = getMaximalKickDist( o );

    // stop this loop when max speed is reached or the distance is traveled.
    while ( dDist > dMinDist &&
            (fabs(dSpeed - dSpeedPrev) > EPSILON || dSpeed < 0.3 ) &&
            iCycles < 40 ) // ignore standing still and turning
    {
        dSpeedPrev = dSpeed;
        dSpeed    += SS->getMaxPower()*dDashRate;
        if ( dSpeed > SS->getPlayerSpeedMax() )
            dSpeed = SS->getPlayerSpeedMax();
        dDist = max( 0, dDist - dSpeed );
        dSpeed *= dDecay;
        iCycles++;
    }
    dSpeed /= dDecay;

    // if distance not completely traveled yet, count the number of cycles to
    // travel the remaining distance with this speed.
    if ( dDist > dMinDist  )
        iCycles += (int)ceil(( dDist - dMinDist )/dSpeed);
    return max(0, iCycles ) ;
}


/*! This method gives an estimate for the number of cycles a player needs to
    reach a specific position. A position is reached when the player is located
    in the maximal kick distance of this position. When this is not the case
    a dash (or turn) is performed until the player is in the kickable distance.
    \param o objectT which wants to reach posTo
    \param posTo global position which has to be reached
    \param angToTurn angle to 'posTo' when is turned (instead of dashed)
    \return predicted nr of cycles for o to reach posTo  */
int WorldModel::predictNrCyclesToPoint( ObjectT o, VecPosition posTo )
{
    char          strBuf[128];
    VecPosition   posGlobal = getGlobalPositionLastSee( o ), posPred;
    VecPosition   vel;
    int           iCycles;
    AngDeg        angBody, angNeck = 0, ang;
    AngDeg        angDes = (posTo-posGlobal).getDirection();
    SoccerCommand soc;

    Log.log( 460, "predict steps for %s with dist %f (time %d) and body %f (%d)",
             SoccerTypes::getObjectStr( strBuf, o ),
             posTo.getDistanceTo( posGlobal ),
             getTimeGlobalPositionLastSee( o ).getTime(),
             getGlobalBodyAngle(o), getTimeChangeInformation(o).getTime() );

    // if already in kickable distance, return 0
    if ( posTo.getDistanceTo( posGlobal ) < getMaximalKickDist( o) )
    {
        Log.log( 460, "already close: 0" );
        return 0;
    }

    // first check how old the change info (and thus body and vel.) info is
    // if too old, assume information is perfect and set time to position info
    // otherwise update all information with stored information
    iCycles = getTimeChangeInformation(o).getTime() - getCurrentCycle();
    if ( o == getAgentObjectType() )
    {
        angBody = getAgentGlobalBodyAngle();
        vel     = getAgentGlobalVelocity( );
        posPred = getAgentGlobalPosition( );
        iCycles = 0;
    }
    else if ( iCycles < -3 )
    {
        angBody = angDes;
        vel.setVecPosition( 0.3, angDes, POLAR );
        if ( SoccerTypes::isOpponent( o ) )
            iCycles = -2; // otherwise too optimistic
        else
            iCycles = 0;
        posPred = getGlobalPositionLastSee( o );
    }
    else
    {
        angBody = getGlobalBodyAngleLastSee( o );
        vel     = getGlobalVelocityLastSee( o );
        posPred = getGlobalPositionLastSee( o );
    }

    Log.log( 460, "rel. time angle info (des %f,now %f,speed %f): %d (%d-%d)",
             angDes, angBody, vel.getMagnitude(), iCycles,
             getTimeChangeInformation(o).getTime(), getCurrentCycle() );

    if ( o != getAgentObjectType() &&
            getTimeGlobalPositionLastSee( o ) > getTimeChangeInformation(o) )
    {
        Log.log( 460, "update cycles to global pos. time: %d",
                 getTimeGlobalPositionLastSee( o ).getTime() );
        iCycles = max(iCycles,
                      getTimeGlobalPositionLastSee(o).getTime()-getCurrentCycle());
    }

    soc = predictCommandToMoveToPos(o,posTo,1,2.5,false,&posPred,&vel,&angBody );
    ang = VecPosition::normalizeAngle( angBody - angDes );

    // sometimes we dash to stand still and turn then
    while ( soc.commandType == CMD_TURN ||
            ( fabs( ang ) > 20 && soc.commandType == CMD_DASH && soc.dPower < 0 ))
    {
        iCycles++;
        predictStateAfterCommand( soc, &posPred, &vel, &angBody, &angNeck, o );
        if ( posTo.getDistanceTo( posPred ) < getMaximalKickDist( o ) )
        {
            Log.log( 460, "reached point during turning, vel %f: %d",
                     vel.getMagnitude(), iCycles );
            return iCycles;
        }
        soc=predictCommandToMoveToPos(o,posTo,1,2.5,false,&posPred,&vel,&angBody );
        ang = VecPosition::normalizeAngle( angBody - angDes );
    }
    Log.log( 460, "cycles after turning: %d (ang %f, %f) vel %f",
             iCycles, ang, angDes, vel.getMagnitude() );

    if ( o != getAgentObjectType() )
    {
        // iCycles++; // do not count last dash -> predictState not called
        double dVel = vel.rotate(-angBody).getX(); // get distance in direction
        iCycles += predictNrCyclesForDistance(o,posPred.getDistanceTo(posTo),dVel);
    }
    else
    {
        while ( posPred.getDistanceTo( posTo ) > getMaximalKickDist( o ) )
        {
            soc=predictCommandToMoveToPos(o,posTo,1,2.5,0,&posPred,&vel,&angBody);
            predictStateAfterCommand( soc, &posPred, &vel, &angBody, &angNeck, o );
            iCycles++;
        }
    }

    Log.log( 460, "total cycles: %d", iCycles );
    return iCycles;
}

/*! This method returns the number of cycles it will take the object 'objFrom'
    to reach the object 'objTo' (usually respectively the player and the ball).
    \param objFrom ObjectT that is the object that wants to move
    \param objTo ObjectT to which is moved
    \return number of cycles it will take objFrom to move to objTo */
int WorldModel::predictNrCyclesToObject( ObjectT objFrom, ObjectT objTo )
{
    VecPosition posPrev(UnknownDoubleValue,UnknownDoubleValue);

    if ( objFrom == OBJECT_ILLEGAL || objTo == OBJECT_ILLEGAL ||
            getGlobalPosition( objFrom ).getDistanceTo( getGlobalPosition( objTo )
                                                      ) > 40 )
        return 101;

    // this is part of the intercept
    if ( objFrom == getAgentObjectType() && objTo == OBJECT_BALL )
    {
        FeatureT  feature_type = FEATURE_INTERCEPT_CYCLES_ME;
        if ( isFeatureRelevant( feature_type ) )
        {
            return max(0,
                       ((int)getFeature( feature_type ).getInfo() - getCurrentCycle() ));
        }
        else
        {
            Log.log( 460, "create intercept features" );
            createInterceptFeatures( );
            Log.log( 460, "call predict again" );
            return predictNrCyclesToObject( objFrom, objTo );
        }
    }

    // in case of ball with no velocity, calculate cycles to point
    if ( objTo == OBJECT_BALL && getBallSpeed() < 0.01 )
        return predictNrCyclesToPoint( objFrom, getBallPos() );

    int         iCycles      = 0;
    int         iCyclesToObj = 100;
    VecPosition posObj(0,0);

    // continue calculating number of cycles to position until or we can get
    // earlier at object position, are past maximum allowed number of cycles or
    // the object does not move anymore.
    while ( iCycles <= iCyclesToObj && iCycles < PS->getPlayerWhenToIntercept() &&
            posObj.getDistanceTo( posPrev ) > EPSILON )
    {
        iCycles      = iCycles + 1  ;
        posPrev      = posObj;
        posObj       = predictPosAfterNrCycles( objTo,   iCycles );

        if (getGlobalPosition(objFrom).getDistanceTo(posObj)/SS->getPlayerSpeedMax()
                < iCycles + 1 )
        {
            Log.log( 460, "predictNrCyclesToPoint after %d cycles", iCycles );
            iCyclesToObj = predictNrCyclesToPoint ( objFrom, posObj );
        }
    }

    return iCyclesToObj;
}

/*! This method updates all the stamina variables using the calculations from
    the soccer manual. It is not really important since stamina is read from
    sense_body every cycle. That information is more up to date.
    \param power of last dash command
    \param stamina pointer to all stamina values, will change to new value
    \return stamina class will be updated to new stamina values */
void WorldModel::predictStaminaAfterDash( double dPower, Stamina *stamina )
{
    double sta = stamina->getStamina();
    double eff = stamina->getEffort();
    double rec = stamina->getRecovery();

    // double negative value when dashed backwards
    sta -= ( dPower > 0.0 ) ? dPower : -2*dPower ;
    if ( sta < 0 ) sta = 0;

    // stamina below recovery threshold, lower recovery
    if ( sta <= SS->getRecoverDecThr()*SS->getStaminaMax() &&
            rec > SS->getRecoverMin() )
        rec -= SS->getRecoverDec();

    // stamina below effort decrease threshold, lower effort
    if ( sta <= SS->getEffortDecThr()*SS->getStaminaMax() &&
            eff > SS->getEffortMin() )
        eff -= SS->getEffortDec();

    // stamina higher than effort incr threshold, raise effort and check maximum
    if ( sta >= SS->getEffortIncThr() * SS->getStaminaMax() &&
            eff < 1.0)
    {
        eff += SS->getEffortInc();
        if ( eff > 1.0 )
            eff = 1.0;
    }

    // increase stamina with (new) recovery value and check for maximum
    sta += rec*SS->getStaminaIncMax();
    if ( sta > SS->getStaminaMax() )
        sta = SS->getStaminaMax();

    stamina->setStamina ( sta );
    stamina->setEffort  ( eff );
    stamina->setRecovery( rec );
}

/*! This method returns the command for object 'obj' to turn towards a point
    'posTo' on the field when it has 'iCycles' to reach that point. If
    the point is within 'dDistBack' behind the object it will try to dash
    backwards. In the case that 'bMoveBack' is true, it will always try to
    move backwards. When posIn, velIn and angBodyIn are equal to NULL, the
    current agent information is used. */
SoccerCommand WorldModel::predictCommandTurnTowards( ObjectT obj, VecPosition
        posTo, int iCycles, double dDistBack, bool bMoveBack,
        VecPosition *posIn, VecPosition *velIn, AngDeg *angBodyIn )
{
    SoccerCommand soc, socFirst;
    VecPosition   pos, vel;
    AngDeg        angBody, ang, angNeck, angTo;
    Stamina       sta;
    bool          bFirst = true;

    // fill in all values
    angBody = ( angBodyIn == NULL ) ? getGlobalBodyAngle( obj ) : *angBodyIn;
    pos     = ( posIn     == NULL ) ? getGlobalPosition ( obj ) : *posIn;
    vel     = ( velIn     == NULL ) ? getGlobalVelocity ( obj ) : *velIn;
    angNeck = getGlobalNeckAngle( obj );

    // predict where we will finally stand when our current vel is propogated
    // and then check the orthogonal distance w.r.t. our body direction
    VecPosition posPred=predictPosAfterNrCycles( obj, min(iCycles,4),
                        0, &pos, &vel, false );
    Line        line   =Line::makeLineFromPositionAndAngle( posPred, angBody );
    double      dDist  =line.getDistanceWithPoint( posTo );

    // get the angle to this point
    angTo = (posTo - posPred).getDirection();
    angTo = VecPosition::normalizeAngle( angTo - angBody );

    // determine whether we want to turn based on orthogonal distance
    double dRatioTurn;
    if ( pos.getDistanceTo(posTo) > 30.0 )
        dRatioTurn = 4.0;
    if ( pos.getDistanceTo(posTo) > 20.0 )
        dRatioTurn = 3.0;
    else if ( pos.getDistanceTo(posTo) > 10 )
        dRatioTurn = 2.0;
    else
        dRatioTurn = 0.90 ;

    AngDeg angTmp = angTo + (bMoveBack) ? 180 : 0;
    angTmp = VecPosition::normalizeAngle( angTmp );

    // turn when:
    //  1. point lies outside our body range (forward and backwards)
    //  2. point lies outside distBack and behind us (forward move)
    //  3. point lies outside distBack and in front of us backwards move)
    int    turn = 0;
    while ( ( dDist > dRatioTurn*getMaximalKickDist( obj ) ||
              ( posPred.getDistanceTo( posTo ) > dDistBack &&
                ( ( fabs( angTo ) > 90 && bMoveBack == false  ) ||
                  ( fabs( angTo ) < 90 && bMoveBack == true ) ) ) )
            && turn < 5 && fabs( angTmp ) > PS->getPlayerWhenToTurnAngle() )
    {

        ang = (posTo - posPred).getDirection() + (( bMoveBack == true )?180:0);
        ang = VecPosition::normalizeAngle( ang - angBody );
        soc = SoccerCommand(CMD_TURN,getAngleForTurn(ang,vel.getMagnitude(),obj));
        Log.log( 468, "angTo %f, dDist %f, ang %f %d angBody %f soc %f vel %f %f",
                 angTo, dDist, ang, obj, angBody, soc.dAngle, vel.getMagnitude(),
                 getInertiaMoment( obj ));
        if ( bFirst == true )
            socFirst = soc;
        bFirst = false;
        predictStateAfterTurn(soc.dAngle, &pos, &vel, &angBody,&angNeck,obj,&sta);
        line = Line::makeLineFromPositionAndAngle( posPred, angBody );
        dDist = line.getDistanceWithPoint( posTo );
        angTo = (posTo - posPred).getDirection();
        angTo = VecPosition::normalizeAngle( angTo - angBody );
        turn++;
    }

    // if very close and have to turn a lot, it may be better to move with our
    // back to that point
    if ( turn > 1 && iCycles < 4 && posPred.getDistanceTo( posTo ) < dDistBack &&
            bMoveBack == false)
    {
        angBody = ( angBodyIn == NULL ) ? getGlobalBodyAngle( obj ) : *angBodyIn;
        pos     = ( posIn     == NULL ) ? getGlobalPosition ( obj ) : *posIn;
        vel     = ( velIn     == NULL ) ? getGlobalVelocity ( obj ) : *velIn;
        ang = (posTo - posPred).getDirection() + 180;
        ang = VecPosition::normalizeAngle( ang - angBody );
        soc = SoccerCommand(CMD_TURN,getAngleForTurn(ang,vel.getMagnitude(),obj));
        predictStateAfterTurn( soc.dAngle,&pos,&vel,&angBody,&angNeck,obj,&sta);
        line = Line::makeLineFromPositionAndAngle( posPred, angBody );
        dDist = line.getDistanceWithPoint( posTo );
        if ( dDist < 0.9*getMaximalKickDist( obj )  )
        {
            Log.log( 463, "turn around and intercept with back" );
            return soc;
        }
    }

    return socFirst;
}

/*! This method returns the command to move to a position, first it checks
    whether a turn is necessary. When this is the case, it performs the turn.
    Otherwise a dash command is generated to move in 'iCycles' cycles to
    the point 'posTo'. */
SoccerCommand WorldModel::predictCommandToMoveToPos( ObjectT obj,
        VecPosition posTo, int iCycles, double dDistBack,
        bool bMoveBack,VecPosition *posIn, VecPosition *velIn, AngDeg *angBodyIn)
{
    VecPosition   pos, vel;
    AngDeg        angBody;
    SoccerCommand soc;
    double        dPower;

    // fill in all values
    angBody = ( angBodyIn == NULL ) ? getGlobalBodyAngle( obj ) : *angBodyIn;
    pos     = ( posIn     == NULL ) ? getGlobalPosition ( obj ) : *posIn;
    vel     = ( velIn     == NULL ) ? getGlobalVelocity ( obj ) : *velIn;

    soc = predictCommandTurnTowards(obj, posTo, iCycles, dDistBack, bMoveBack,
                                    posIn, velIn,   angBodyIn);
    if ( ! soc.isIllegal() )
        return soc;

    dPower = getPowerForDash( posTo-pos, angBody, vel,getAgentEffort(),iCycles );
    return SoccerCommand( CMD_DASH, dPower );
}

/*! This command returns the command for object 'obj' to intercept the ball.
    It needs the command 'socClose' as the command to intercept a close ball
    (may be CMD_ILLEGAL). 'iCycles' will be filled with the number of cycles
    to get to this ball position and 'posIntercept' will be filled with the
    final interception point. When posIn, velInn, angBody are equal to NULL,
    the agent information is used in the calculations. */
SoccerCommand WorldModel::predictCommandToInterceptBall( ObjectT obj,
        SoccerCommand socClose, int *iCycles, VecPosition *posIntercept,
        VecPosition *posIn, VecPosition *velIn, AngDeg *angBodyIn )
{
    FeatureT feature_type = FEATURE_INTERCEPTION_POINT_BALL;

    // check whether we already have calculated this value
    if ( isFeatureRelevant( feature_type ) )
    {
        int i = max(0,((int)getFeature(feature_type).getInfo()-getCurrentCycle()));
        if ( iCycles != NULL )
            *iCycles = i;
        if ( posIntercept != NULL )
            *posIntercept = predictPosAfterNrCycles( OBJECT_BALL, i );

        Log.log( 463, "intercept, use old info, feature %d: %d", feature_type,
                 max(0,((int)getFeature( feature_type ).getInfo()-getCurrentCycle())));
        return getFeature( feature_type ).getCommand();
    }

    // declare all needed variables
    SoccerCommand soc;
    VecPosition   pos, vel, posPred, posBall(0,0), posBallTmp, velBall, posAgent;
    AngDeg        angBody, angNeck;
    int           iMinCyclesBall=100, iFirstBall=100;
    double        dMaxDist = getMaximalKickDist( obj );
    double        dBestX = UnknownDoubleValue;
    Stamina       sta;
    double        dMinOldIntercept = 100, dDistanceOfIntercept = 10.0;
    int           iOldIntercept = UnknownIntValue;
    static Time   timeLastIntercepted(-1,0);
    static VecPosition posOldIntercept;

    // didn't intercept ball in last two cycles -> reset old interception point
    if ( (getCurrentTime() - timeLastIntercepted) > 2 )
        posOldIntercept.setVecPosition( UnknownDoubleValue, UnknownDoubleValue);
    timeLastIntercepted = getCurrentTime();

    int iCyclesBall = 0;

    Log.log( 468, "old interception point: (%f,%f)", posOldIntercept.getX(),
             posOldIntercept.getY() );

    // for each new pos of the ball, check whether agent can reach ball
    // and update the best interception point
    while ( iCyclesBall <= PS->getPlayerWhenToIntercept() &&
            iCyclesBall <= iFirstBall + 20 &&
            isInField( posBall ) == true )
    {
        // re-initialize all variables
        angBody = ( angBodyIn == NULL ) ? getGlobalBodyAngle( obj ) : *angBodyIn;
        angNeck = getGlobalNeckAngle( obj );
        pos     = ( posIn     == NULL ) ? getGlobalPosition ( obj ) : *posIn;
        vel     = ( velIn     == NULL ) ? getGlobalVelocity ( obj ) : *velIn;
        sta     = getAgentStamina();
        soc.commandType = CMD_ILLEGAL;

        // predict the ball position after iCycles and from that its velocity
        posBallTmp = predictPosAfterNrCycles( OBJECT_BALL, iCyclesBall );
        if ( iCyclesBall == 0 )
            velBall = getGlobalVelocity( OBJECT_BALL );
        else
            velBall = posBallTmp - posBall;
        posBall   = posBallTmp;

        // predict the agent position
        posPred   = predictPosAfterNrCycles( obj, min(iCyclesBall,4), 0 );
        posAgent  = getGlobalPosition( obj );

        // if too far away, we can never reach it and try next cycle
        if ( posPred.getDistanceTo(posBall)/getPlayerSpeedMax( obj )
                > iCyclesBall + dMaxDist || isInField( posBall ) == false )
        {
            iCyclesBall++;
            continue;
        }

        // predict our position after the same nr of cycles when intercepting
        for ( int i = 0; i < iCyclesBall; i++ )
        {
            soc = predictCommandToMoveToPos( obj, posBall, iCyclesBall - i ,
                                             2.5, false, &pos, &vel, &angBody );
            predictStateAfterCommand( soc, &pos, &vel, &angBody, &angNeck, obj );
        }

        // if in kickable distance, we can reach the ball!
        if (pos.getDistanceTo( posBall ) < dMaxDist  )
        {
            Log.log( 468, "can intercept ball in %d cycles, dist %f, old %f obj %d",
                     iCyclesBall, pos.getDistanceTo( posBall ),
                     posBall.getDistanceTo( posOldIntercept ), obj  );

            if ( iMinCyclesBall == 100 ) // log first possible interception point
                iFirstBall = iMinCyclesBall = iCyclesBall;

            // too get some consistency in the interception point and avoid
            // too many turns, also keep track of the current possible
            // interception point.  This is the point close to the old
            // interception point. Two constraints are that the ball has to
            // have some speed (else it does not really matter where to
            // intercept) and the ball must be intercepted safely, that is
            // the ball is close to the body when intercepting.
            if ( posBall.getDistanceTo( posOldIntercept ) <
                    min( 1.0, dMinOldIntercept ) &&
                    pos.getDistanceTo( posBall ) < 0.70*getMaximalKickDist( obj ) &&
                    velBall.getMagnitude() > 0.6 )
            {
                Log.log( 468, "update old interception point %d", iCyclesBall );
                dBestX           = posBall.getX();
                iOldIntercept    = iCyclesBall;
                dDistanceOfIntercept = pos.getDistanceTo( posBall );
                dMinOldIntercept = posBall.getDistanceTo( posOldIntercept );
            }
            // determine the safest interception point. This point must be
            // better than the current intercept, the distance to ball must
            // be very small after interception and close to the previous
            // calculated interception point
            else if ( pos.getDistanceTo( posBall ) < dDistanceOfIntercept &&
                      dDistanceOfIntercept > 0.50*getMaximalKickDist( obj ) &&
                      ( iCyclesBall <= iMinCyclesBall + 3  ||
                        iCyclesBall <= iOldIntercept + 3 ) &&
                      fabs( posBall.getY() ) < 32.0 &&
                      fabs( posBall.getX() ) < 50.0 )
            {
                iMinCyclesBall = iCyclesBall;
                dDistanceOfIntercept = pos.getDistanceTo( posBall );
                Log.log( 468, "safer interception at %d", iMinCyclesBall );
                if ( iOldIntercept == iMinCyclesBall - 1 )
                {
                    Log.log( 468, "old interception point -> safer" );
                    iOldIntercept = iMinCyclesBall;
                }
            }
        }
        else
            Log.log( 468, "cannot intercept ball in %d cycles, dist %f, %f and %f",
                     iCyclesBall, pos.getDistanceTo(posBall), pos.getDistanceTo( posAgent ),
                     posBall.getDistanceTo( posAgent ) - dMaxDist);;

        iCyclesBall++;
    }

    Log.log( 463, "first interception point:        %d cycles", iFirstBall );
    Log.log( 463, "best interception point:         %d cycles", iMinCyclesBall );
    Log.log( 463, "old interception point           %d cycles", iOldIntercept );

    // check special situations where we move to special position.
    if ( !( iMinCyclesBall > iOldIntercept + 2 ) &&
            iOldIntercept != UnknownIntValue  )
    {
        Log.log( 463, "move to old interception point." );
        iMinCyclesBall = iOldIntercept;
    }
    else
    {
        Log.log( 463, "move to first intercept" );
        iMinCyclesBall = iFirstBall;
    }

    posBall = predictPosAfterNrCycles( OBJECT_BALL, iMinCyclesBall );
    Log.log( 463, "choose %d cycles", iMinCyclesBall );
    logCircle( 463, posBall, 1.0 );
    if ( iCycles != NULL )
        *iCycles = iMinCyclesBall;

    posOldIntercept = posBall;
    posPred = predictPosAfterNrCycles( obj, min(iMinCyclesBall,4), 0 );
    if ( posIntercept != NULL )
        *posIntercept = posBall;

    if ( iMinCyclesBall < 3 && ! socClose.isIllegal() )
    {
        Log.log( 463, "do close intercept" );
        iMinCyclesBall = 1;
        soc = socClose;
    }
    else if ( posPred.getDistanceTo( posBall ) < 0.5 )
    {
        Log.log( 463, "intercept: do not move already close" );
        soc = SoccerCommand( CMD_ILLEGAL );
    }
    else
    {
        Log.log( 463, "intercept: move to (%f,%f)", posBall.getX(),posBall.getY());
        Log.log( 560, "intercept: move to (%f,%f) in %d cycles",
                 posBall.getX(),posBall.getY(), iMinCyclesBall);
        if ( isDeadBallUs() && !isGoalKickUs())    // do not dash backwards
            soc = predictCommandToMoveToPos( obj, posBall, iMinCyclesBall, 0 );
        else
            soc = predictCommandToMoveToPos( obj, posBall, iMinCyclesBall );
    }

    // store the calculated action as a feature
    if ( obj == getAgentObjectType() )
        setFeature( feature_type,
                    Feature( getTimeLastSeeMessage(),
                             getTimeLastSenseMessage(),
                             getTimeLastHearMessage(),
                             OBJECT_ILLEGAL,
                             getTimeLastSeeMessage().getTime() + iMinCyclesBall,
                             soc ) );
    return soc;
}

/*! This method determines whether a dash command (supplied as the first
    argument) will result in collision with another player.
    This is checked by determing the global position after the command
    and then check whether the positions of one of the other players lies
    with the player size. Since it cannot be known what kind of action the
    other player takes in this cycle, it is also difficult to predict what the
    global position of the player will be in the next cycle. This method
    therefore assumes the other players have issued a dash with maximum power
    in the last cycle.
    \return bool indicating whether dash will result in a collision. */
bool WorldModel::isCollisionAfterCommand( SoccerCommand soc )
{
    VecPosition posPred, velPred;
    AngDeg ang1, ang2;
    Stamina sta;

    predictAgentStateAfterCommand( soc, &posPred, &velPred, &ang1,&ang2,&sta );
    velPred /= SS->getPlayerDecay();
    VecPosition posBall = predictPosAfterNrCycles( OBJECT_BALL, 1 );
    if ( soc.commandType == CMD_KICK )
        predictBallInfoAfterCommand( soc, &posBall );
    double  dDist   = posPred.getDistanceTo( posBall ) -
                      SS->getPlayerSize() - SS->getBallSize();
    Log.log( 510, "check collision dist %f, noise_ball %f noise_me %f",
             dDist, getBallSpeed()*SS->getBallRand(),
             velPred.getMagnitude()*SS->getPlayerRand() );

    // we could also take into account the error in player movement, but this
    // is very large, so we would in many cases get a dash
    if ( dDist < getBallSpeed()*SS->getBallRand() )
        return true;

    return false;
}

