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

/*! \file BasicPlayer.cpp
<pre>
<b>File:</b>          BasicPlayer.cpp
<b>Project:</b>       Robocup Soccer Simulation Team: UvA Trilearn
<b>Authors:</b>       Jelle Kok
<b>Created:</b>       10/12/2000
<b>Last Revision:</b> $ID$
<b>Contents:</b>      This file contains the class declaration for the
               BasicPlayer. The BasicPlayer is the class where the
               available skills for the agent are defined.
<hr size=2>
<h2><b>Changes</b></h2>
<b>Date</b>             <b>Author</b>          <b>Comment</b>:
10/12/2000       Jelle Kok       Initial version created
</pre>
*/

#include "BasicPlayer.h"
#include "Parse.h"        // parseFirstInt

/********************** LOW-LEVEL SKILLS *************************************/

/*! This skill enables an agent to align his neck with his body. It returns a
    turn neck command that takes the angle of the agent's body relative to his
    neck as its only argument.
   \return SoccerCommand turn_neck command that aligns neck with body */
SoccerCommand BasicPlayer::alignNeckWithBody( )
{
    return SoccerCommand( CMD_TURNNECK, WM->getAgentBodyAngleRelToNeck( ) );
}

/*! This skill enables an agent to turn his body towards a given point. It
    receives a global position 'pos' on the field and returns a turn command
    that will turn the agent's body towards this point. To this end the agent's
    global position in the next cycle is predicted based on his current
    velocity. This is done to compensate for the fact that the remaining
    velocity will move the agent to another position in the next cycle. The
    global angle between the given position and the predicted position is then
    determined after which the agent's global body direction is subtracted from
    this angle in order to make it relative to the agent's body. Finally,
    the resulting angle is normalized and adjusted to compensate for the
    inertia moment and speed of the agent. If it is impossible to turn towards
    the given position in a single cycle then the agent turns as far as
    possible.
    \param pos position to which body should be turned
    \param iCycles denotes the number of cycles that are used to update the
               the agent position. The resulting position is compared with
               'pos' to determine the desired turning angle.
    \return SoccerCommand turn command to turn body to the desired point */
SoccerCommand BasicPlayer::turnBodyToPoint( VecPosition pos, int iCycles )
{
    VecPosition posGlobal = WM->predictAgentPos(iCycles, 0);
    AngDeg angTurn        = (pos - posGlobal).getDirection();
    angTurn              -= WM->getAgentGlobalBodyAngle();
    angTurn               = VecPosition::normalizeAngle( angTurn );
    angTurn               = WM->getAngleForTurn( angTurn, WM->getAgentSpeed(),
                            WM->getAgentObjectType() );

    return SoccerCommand( CMD_TURN, angTurn );
}

/*! This skill enables an agent to turn his back towards a given point 'pos'.
    The only difference between this skill and turnBodyToPoint is that the
    angle between the given position and the predicted position of the agent
    in the next cycle is now made relative to the back of the agent by
    subtracting the agent's global back direction. This skill can for example
    be used by the goalkeeper in case he wants to move back to his goal while
    keeping sight of the rest of the field.
    \param pos position to which the agent's back should be turned
    \param iCycles denotes the number of cycles that are used to update the
               the agent position. The resulting position is compared with
               'pos' to determine the desired turning angle.
    \return SoccerCommand command to turn agent's back to the desired point */
SoccerCommand BasicPlayer::turnBackToPoint( VecPosition pos, int iCycles )
{
    VecPosition posGlobal = WM->predictAgentPos(iCycles, 0);
    AngDeg angTurn        = (pos - posGlobal).getDirection();
    angTurn              -= (WM->getAgentGlobalBodyAngle() + 180);
    angTurn               = VecPosition::normalizeAngle( angTurn );
    angTurn               = WM->getAngleForTurn( angTurn, WM->getAgentSpeed(),
                            WM->getAgentObjectType() );

    return SoccerCommand( CMD_TURN, angTurn );
}

/*! This skill enables an agent to turn his neck towards a given point. It
    receives a global position 'pos' on the field as well as a primary action
    command 'soc' that will be executed by the agent at the end of the current
    cycle and returns a turn neck command that will turn the agent's neck
    towards 'pos'. To this end the agent's global position and neck direction
    after executing the cmd command are predicted using methods from the world
    model. The global angle between the given position and the predicted
    position is then determined after which the predicted neck direction is
    subtracted from this angle in order to make it relative to the agent's
    neck. Finally, the resulting angle is normalized and directly passed as an
    argument to the turn neck command since the actual angle with which a
    player turns his neck is by definition equal to this argument.
    If the resulting turn angle causes the absolute angle between
    the agent's neck and body to exceed the maximum value, then the agent turns
    his neck as far as possible. Note that it is necessary to supply the
    selected primary command as an argument to this skill, since a turn neck
    command can be executed in the same cycle as a kick, dash, turn , move
    or catch command.
   \param pos position to which neck should be turned
   \param soc SoccerCommand that is executed in the same cycle
   \return SoccerCommand turn command to turn neck to the desired point */


SoccerCommand BasicPlayer::turnNeckToPoint(VecPosition pos, SoccerCommand soc)
{
    VecPosition posMe,   velMe;
    AngDeg      angBody, angNeck, angActual;
    Stamina     sta;

    // predict agent information after command 'soc' is performed
    // calculate the desired global angle of the neck
    // calculate the desired angle of the neck relative to the body
    WM->predictAgentStateAfterCommand(soc,&posMe,&velMe,&angBody,&angNeck,&sta);
    AngDeg angDesGlobNeck  = (pos - posMe).getDirection();
    AngDeg angNeckRelToBody= VecPosition::normalizeAngle(angDesGlobNeck-angBody);

    // calculate the current angle of the body relative to the neck
    // check if the desired neck angle relative to the body is possible:
    // if angle is smaller than the minimum or larger than the maximum neck angle
    //  turn neck to the minimum or maximum neck angle + the current neck angle
    // else calculate the desired angle relative to the body
    AngDeg angBodyRelToNeck = VecPosition::normalizeAngle(angBody-angNeck);
    if ( angNeckRelToBody < SS->getMinNeckAng() )
        angActual = SS->getMinNeckAng() + angBodyRelToNeck;
    else if ( angNeckRelToBody > SS->getMaxNeckAng() )
        angActual = SS->getMaxNeckAng() + angBodyRelToNeck;
    else
        angActual = angNeckRelToBody + angBodyRelToNeck;
    return SoccerCommand( CMD_TURNNECK, angActual );
}

/*! This skill enables an agent to search for the ball when he cannot see it.
    It returns a turn command that causes the agent to turn his body by an
    angle that equals the width of his current view cone (denoted by the
    ViewAngle attribute in the AgentObject class). In this way the agent will
    see an entirely different part of the field after the turn which maximizes
    the chance that he will see the ball in the next cycle. Note that the
    agent turns towards the direction in which the ball was last observed to
    avoid turning back and forth without ever seeing the ball. Furthermore
    the inertia moment of the agent is taken into account to compensate for
    the current speed of the agent.
    \return SoccerCommand that searches for the ball. */
SoccerCommand BasicPlayer::searchBall()
{
    static Time   timeLastSearch;
    static SoccerCommand soc;
    static int    iSign       = 1;
    VecPosition   posBall =WM->getBallPos();
    VecPosition   posAgent=WM->getAgentGlobalPosition();
    AngDeg        angBall =(posBall-WM->getAgentGlobalPosition()).getDirection();
    AngDeg        angBody =WM->getAgentGlobalBodyAngle();

    if ( WM->getCurrentTime().getTime() == timeLastSearch.getTime()  )
        return soc;

    if ( WM->getCurrentTime() - timeLastSearch > 3 )
        iSign = ( isAngInInterval( angBall, angBody,
                                   VecPosition::normalizeAngle(angBody+180) ) )
                ? 1
                : -1  ;

    //  if( iSign == -1 )
    // angBall = VecPosition::normalizeAngle( angBall + 180 );

    soc = turnBodyToPoint( posAgent + VecPosition(1.0,
                           VecPosition::normalizeAngle(angBody+60*iSign), POLAR ) );
    Log.log( 556, "search ball: turn to %f s %d t(%d %d) %f", angBall, iSign,
             WM->getCurrentTime().getTime(), timeLastSearch.getTime(),
             soc.dAngle );
    timeLastSearch = WM->getCurrentTime();
    return soc;
}


/*! This method can be called to create a SoccerCommand that dashes to
    a point.  This skill enables an agent to dash to a given point. It
    receives a global position 'pos' as its only argument and returns
    a dash command that causes the agent to come as close to this
    point as possible. Since the agent can only move forwards or
    backwards, the closest point to the target position that he can
    reach by dashing is the orthogonal projection of 'pos' onto the
    line that extends into the direction of his body (forwards and
    backwards).  The power that must be supplied to the dash command
    is computed using the 'getPowerForDash' method which takes the
    position of 'pos' relative to the agent as input and 'iCycles'
    which denotes in how many cycles we want to reach that point.
    \param pos global position to which the agent wants to dash
    \param iCycles desired number of cycles to reach point 'pos'
    \return SoccerCommand dash command to move closer to 'pos' */
SoccerCommand BasicPlayer::dashToPoint( VecPosition pos, int iCycles )
{
    double dDashPower = WM->getPowerForDash(
                            pos - WM->getAgentGlobalPosition(),
                            WM->getAgentGlobalBodyAngle(),
                            WM->getAgentGlobalVelocity(),
                            WM->getAgentEffort(),
                            iCycles                              );
    return SoccerCommand( CMD_DASH, dDashPower );
}

/*! This skill enables an agent to freeze a moving ball, i.e. it returns a kick
    command that stops the ball dead at its current position. Since ball
    movement in the soccer server is implemented as a vector addition, the ball
    will stop in the next cycle when it is kicked in such a way that the
    resulting acceleration vector has the same length and opposite direction to
    the current ball velocity. The desired speed that should be given to the
    ball on the kick thus equals the current ball speed. Furthermore, the
    direction of the kick should equal the direction of the current ball
    velocity plus 180 degrees. Note that this direction must be made relative
    to the agent's global body angle before it can be passed as an argument to
    the kick command.
    \return SoccerCommand to freeze the ball. */
SoccerCommand BasicPlayer::freezeBall( )
{
    // determine power needed to kick the ball to compensate for current speed
    // get opposite direction (current direction + 180) relative to body
    // and make the kick command.
    VecPosition posAgentPred = WM->predictAgentPos( 1, 0 );

    double dPower = WM->getKickPowerForSpeed( WM->getBallSpeed() );
    if ( dPower > SS->getMaxPower() )
    {
        Log.log( 552, "%d: freeze ball has to much power", WM->getCurrentCycle() );
        dPower = (double)SS->getMaxPower();
    }
    double dAngle = WM->getBallDirection() + 180 - WM->getAgentGlobalBodyAngle();
    dAngle        = VecPosition::normalizeAngle( dAngle );
    SoccerCommand soc( CMD_KICK, dPower, dAngle );
    VecPosition posBall, velBall;
    WM->predictBallInfoAfterCommand( soc, &posBall, &velBall );
    if ( posBall.getDistanceTo( posAgentPred ) < 0.8 * SS->getMaximalKickDist() )
        return soc;
    Log.log( 101, "freeze ball will end up oustide -> accelerate" );
    posBall = WM->getBallPos();
    // kick ball to position inside to compensate when agent is moving
    VecPosition posTo   = posAgentPred +
                          VecPosition( min( 0.7 * SS->getMaximalKickDist(),
                                            posBall.getDistanceTo( posAgentPred ) - 0.1 ),
                                       (posBall-posAgentPred).getDirection(),
                                       POLAR );
    VecPosition velDes( posTo - posBall );
    return accelerateBallToVelocity( velDes );
}

/*! This skill enables an agent to kick the ball close to his body. It receives
    an angle 'ang' as its only argument and returns a kick command that causes
    the ball to move to a point at a relative angle of 'ang' degrees and at a
    close distance (kickable margin/6 to be precise) from the agent's body.
    To this end the ball has to be kicked from its current position to the
    desired point relative to the predicted position of the agent in the next
    cycle. In general, this skill will be used when the agent wants to kick the
    ball to a certain position on the field which cannot be reached with a
    single kick. Since the efficiency of a kick is highest when the ball is
    positioned just in front of the agent's body, calling this skill with
    'ang = 0' will have the effect that the agent can kick the ball with more
    power after it is executed.
    Note that this skill will only be executed if it is possible to actually
    reach the desired ball position with a single kick. If the required power
    does exceed the maximum then the ball is frozen at its current position
    using the freezeBall skill. In general, it will then always be possible to
    shoot the motionless ball to the desired point in the next cycle.
    \param 'ang' relative angle to body to which the ball should be kicked
    \param relative ratio to which the ball is kicked
    \return SoccerCommand to kick the ball close to the body */
SoccerCommand BasicPlayer::kickBallCloseToBody( AngDeg ang, double dKickRatio )
{
    AngDeg      angBody    = WM->getAgentGlobalBodyAngle();
    VecPosition posAgent   = WM->predictAgentPos( 1, 0 );
    double      dDist      = SS->getPlayerSize() +
                             SS->getBallSize()   +
                             SS->getKickableMargin()*dKickRatio;
    AngDeg      angGlobal  = VecPosition::normalizeAngle( angBody + ang );
    VecPosition posDesBall = posAgent   + VecPosition( dDist, angGlobal, POLAR );
    if ( fabs( posDesBall.getY() ) > PITCH_WIDTH/2.0 ||
            fabs( posDesBall.getX() ) > PITCH_LENGTH/2.0 )
    {
        Line lineBody = Line::makeLineFromPositionAndAngle( posAgent, angGlobal );
        Line lineSide(0,0,0);
        if ( fabs( posDesBall.getY() ) > PITCH_WIDTH/2.0 )
            lineSide = Line::makeLineFromPositionAndAngle(
                           VecPosition( 0, sign(posDesBall.getY() )* PITCH_WIDTH/2.0 ), 0 );
        else
            lineSide = Line::makeLineFromPositionAndAngle(
                           VecPosition( 0, sign(posDesBall.getX() )* PITCH_LENGTH/2.0 ),  90 );
        VecPosition posIntersect = lineSide.getIntersection( lineBody );
        posDesBall = posAgent +
                     VecPosition( posIntersect.getDistanceTo( posAgent ) - 0.2,
                                  angGlobal, POLAR );
    }

    VecPosition vecDesired = posDesBall - WM->getBallPos();
    VecPosition vecShoot   = vecDesired - WM->getGlobalVelocity( OBJECT_BALL );
    double      dPower     = WM->getKickPowerForSpeed( vecShoot.getMagnitude() );
    AngDeg      angActual  = vecShoot.getDirection() - angBody;
    angActual  = VecPosition::normalizeAngle( angActual );

    if ( dPower > SS->getMaxPower() && WM->getBallSpeed() > 0.1 )
    {
        Log.log( 500, "kickBallCloseToBody: cannot compensate ball speed, freeze");
        Log.log( 101, "kickBallCloseToBody: cannot compensate ball speed, freeze");
        return freezeBall();
    }
    else if ( dPower > SS->getMaxPower() )
    {
        if ( WM->isDeadBallUs() )
        {
            if ( WM->getRelativeAngle( OBJECT_BALL ) > 25 )
            {
                Log.log( 101, "dead ball situation, turn to ball" );
                return turnBodyToObject( OBJECT_BALL );
            }
        }
        else
        {
            Log.log( 101, "kickBallCloseToBody: ball has no speed, but far away" );
            dPower = 100;
        }
    }
    else
        Log.log( 101, "(kick %f %f), vecDesired (%f,%f) %f posDes(%f,%f)",
                 dPower, angActual,vecDesired.getX(), vecDesired.getY(),ang,
                 posDesBall.getX(), posDesBall.getY() );
    return SoccerCommand( CMD_KICK, dPower, angActual );
}

/*! This skill enables an agent to accelerate the ball in such a way that it
    gets a certain velocity after the kick. It receives the desired velocity
    'vecDes' as its only argument and returns a kick command that causes the
    ball to be accelerated to this velocity. If the power that must be supplied
    to the kick command to get the desired result does not exceed the maximum
    kick power then the desired velocity can be realized with a single kick.
    The kick direction should then be equal to the direction of the
    acceleration vector relative to the agent's global body angle. However,
    if the desired velocity is too great or if the current ball velocity is
    too high then the required acceleration cannot be realized with a single
    kick. In this case, the ball is kicked in such a way that the acceleration
    vector has the maximum possible length and a direction that aligns the
    resulting ball movement with 'vecDes'. This means that after the kick the
    ball will move in the same direction as 'vecDes' but at a lower speed. To
    this end the acceleration vector has to compensate for the current ball
    velocity in the `wrong' direction (y-component).
    \param velDes desired ball velocity
    \return SoccerCommand that accelerates the ball to 'vecDes' */
SoccerCommand BasicPlayer::accelerateBallToVelocity( VecPosition velDes )
{
    AngDeg      angBody  = WM->getAgentGlobalBodyAngle();
    VecPosition velBall  = WM->getGlobalVelocity( OBJECT_BALL );
    VecPosition accDes   = velDes - velBall;
    double      dPower;
    double      angActual;

    // if acceleration can be reached, create shooting vector
    if ( accDes.getMagnitude() < SS->getBallAccelMax() )
    {
        dPower    = WM->getKickPowerForSpeed   ( accDes.getMagnitude() );
        angActual = VecPosition::normalizeAngle( accDes.getDirection() - angBody );
        if ( dPower <= SS->getMaxPower() )
            return SoccerCommand( CMD_KICK, dPower, angActual );
    }

    // else determine vector that is in direction 'velDes' (magnitude is lower)
    dPower    = SS->getMaxPower();
    double dSpeed    = WM->getActualKickPowerRate() * dPower;
    double tmp       = velBall.rotate(-velDes.getDirection()).getY();
    angActual = velDes.getDirection() - asinDeg( tmp / dSpeed );
    angActual = VecPosition::normalizeAngle( angActual - angBody );

    return SoccerCommand( CMD_KICK, dPower, angActual );
}

/*! This skill enables an agent to catch the ball and can only be executed
    when the agent is a goalkeeper. It returns a catch command that takes the
    angle of the ball relative to the body of the agent as its only argument.
    The correct value for this argument is computed by determining the global
    direction between the current ball position and the agent's current
    position and by making this direction relative to the agent's global body
    angle.
    \return SoccerCommand to catch the ball */
SoccerCommand BasicPlayer::catchBall()
{
    // true means returned angle is relative to body instead of neck
    return SoccerCommand( CMD_CATCH, WM->getRelativeAngle( OBJECT_BALL, true ));
}


/*! This skill enables an agent to communicate with other players on the field.
    It receives a string message as its only argument and returns a say command
    that causes the message to be broadcast to all players within a certain
    distance from the speaker.
    \return SoccerCommand to say the specified string 'str' */
SoccerCommand BasicPlayer::communicate( char *str )
{
    return SoccerCommand( CMD_SAY, str );
}

/*! This method returns a 'move' command to teleport the agent directly to the
    specified global position.
    \param pos global position to which should be moved.
    \return SoccerCommand to move directly to 'pos'. */
SoccerCommand BasicPlayer::teleportToPos( VecPosition pos )
{
    return SoccerCommand( CMD_MOVE, pos.getX(), pos.getY() );

}

/*! This method returns a 'attentionto' command to listen to the specified
    object. In most occasions this is a teammate. */
SoccerCommand BasicPlayer::listenTo( ObjectT obj )
{
    if ( !SoccerTypes::isKnownPlayer( obj ) )
        return SoccerCommand( CMD_ATTENTIONTO, -1.0, -1.0 );

    return SoccerCommand( CMD_ATTENTIONTO,
                          1.0,                                       // 1.0 denotes our team
                          SoccerTypes::getIndex( obj ) + 1 );
}

/*! This method returns the command to tackle the ball. */
SoccerCommand BasicPlayer::tackle( )
{
    return SoccerCommand( CMD_TACKLE, 100.0 );
}

/********************** INTERMEDIATE LEVEL SKILLS ****************************/

/*! This skill enables an agent to turn his body towards an object o which is
    supplied to it as an argument. To this end the object's global position
    o in the next cycle is predicted based on its current velocity.
    This predicted position is passed as an argument to the turnBodyToPoint
    skill which generates a turn command that causes the agent to turn his
    body towards the object.
    \param o object to which agent wants to turn
    \return SoccerCommand that turns to this object */
SoccerCommand BasicPlayer::turnBodyToObject( ObjectT o )
{
    return turnBodyToPoint( WM->predictPosAfterNrCycles(o, 1) );
}

/*! This skill enables an agent to turn his neck towards an object. It
    receives as arguments this object o as well as a primary action command
    'soc' that will be executed by the agent at the end of the current cycle.
    Turning the neck towards an object amounts to predicting the object's
    global position in the next cycle and passing this predicted position
    together with the 'soc' command as arguments to the turnNeckToPoint skill.
    This low-level skill will then generate a turn neck command that causes the
    agent to turn his neck towards the given object. Note that the 'soc'
    command is supplied as an argument for predicting the agent's global
    position and neck angle after executing the command. This is necessary
    because a turn neck command can be executed in the same cycle as a kick,
    dash, turn , move or catch command.
   \param o object to which the agent wants to turn his neck
   \param soc SoccerCommand that is performed in this cycle.
   \return SoccerCommand that turns the neck of the agent to this object */
SoccerCommand BasicPlayer::turnNeckToObject( ObjectT o, SoccerCommand soc )
{
    return turnNeckToPoint( WM->predictPosAfterNrCycles(o, 1), soc );
}

SoccerCommand BasicPlayer::directTowards( VecPosition posTurnTo,
        AngDeg angWhenToTurn, VecPosition *pos, VecPosition *vel, AngDeg *angBody  )
{
//  return turnBodyToPoint( posTurnTo );
    // copy values or initialize when not set
    VecPosition posAgent= (pos    ==NULL)?WM->getAgentGlobalPosition ():*pos;
    VecPosition velAgent= (vel    ==NULL)?WM->getAgentGlobalVelocity ():*vel;
    AngDeg  angBodyAgent= (angBody==NULL)?WM->getAgentGlobalBodyAngle():*angBody;

    // first predict what happens when the agents rolls out.
    VecPosition posPred    = WM->predictFinalAgentPos();
    AngDeg      angTo      = ( posTurnTo - posPred ).getDirection();
    AngDeg      ang        = VecPosition::normalizeAngle( angTo - angBodyAgent );
    AngDeg      angNeck = 0;

    int iTurn = 0;
    while ( fabs( ang ) > angWhenToTurn && iTurn < 5 )
    {
        iTurn++;
        WM->predictStateAfterTurn(
            WM->getAngleForTurn( ang, velAgent.getMagnitude() ),
            &posAgent,
            &velAgent,
            &angBodyAgent,
            &angNeck );
        ang = VecPosition::normalizeAngle( angTo - angBodyAgent );
    }
    Log.log( 509, "direct towards: %d turns", iTurn );
    posAgent = (pos    ==NULL)?WM->getAgentGlobalPosition ():*pos;
    velAgent = (vel    ==NULL)?WM->getAgentGlobalVelocity ():*vel;
    angBodyAgent = (angBody==NULL)?WM->getAgentGlobalBodyAngle():*angBody;

    switch ( iTurn )
    {
    case 0:
        cerr << "direct towards: 0 turns" ;
        return SoccerCommand( CMD_ILLEGAL );
    case 1:
    case 2:
        return turnBodyToPoint( posTurnTo, 2 );
    default:
        return dashToPoint(
                   (pos==NULL)?WM->getAgentGlobalPosition ():*pos   );  // stop
    }
}

/*! This skill enables an agent to move to a global position 'pos' on
    the field which is supplied to it as an argument. Since the agent
    can only move forwards or backwards into the direction of his
    body, the crucial decision in the execution of this skill is
    whether he should perform a turn or a dash. Turning has the
    advantage that in the next cycle the agent will be orientated
    correctly towards the point he wants to reach. However, it has the
    disadvantage that performing the turn will cost a cycle and will
    reduce the agent's velocity since no acceleration vector is added
    in that cycle.  Apart from the target position 'pos', this skill
    receives several additional arguments for determining whether a
    turn or dash should be performed in the current situation. If the
    target point is in front of the agent then a dash is performed
    when the relative angle to this point is smaller than a given
    angle 'angWhenToTurn'. However, if the target point is behind the
    agent then a dash is only performed if the distance to point is
    less than a given value 'dDistBack' and if the angle relative to
    the back direction of the agent is smaller than
    'angWhenToTurn'. In all other cases a turn is performed. Note that
    in the case of the goalkeeper it is sometimes desirable that he
    moves backwards towards his goal in order to keep sight of the
    rest of the field. To this end an additional boolean argument
    'bMoveBack' is supplied to this skill that indicates whether the
    agent should always move backwards to the target point. If this
    value equals true then the agent will turn his back towards the
    target point if the angle relative to his back direction is larger
    than 'angToTurn'. In all other cases he will perform a (backward)
    dash towards 'posTo' regardless of whether the distance to this
    point is larger than 'dDistBack'.

    \param posTo global target position to which the agent wants to move
    \param angWhenToTurn angle determining when turn command is returned
    \param dDistBack when posTo lies closer than this value to the back of
          the agent (and within angWhenToTurn) a backward dash is returned
    \param bMoveBack boolean determing whether to move backwards to 'posTo'
    \return SoccerCommand that determines next action to move to 'posTo' */
SoccerCommand BasicPlayer::moveToPos( VecPosition posTo, AngDeg angWhenToTurn,
                                      double dDistBack, bool bMoveBack, int iCycles )
{
// previously we only turned relative to position in next cycle, now take
// angle relative to position when you're totally rolled out...
//  VecPosition posPred   = WM->predictAgentPos( 1, 0 );
    VecPosition posAgent  = WM->getAgentGlobalPosition();
    VecPosition posPred   = WM->predictFinalAgentPos();

    AngDeg      angBody   = WM->getAgentGlobalBodyAngle();
    AngDeg      angTo     = ( posTo - posPred ).getDirection();
    angTo     = VecPosition::normalizeAngle( angTo - angBody );
    AngDeg      angBackTo = VecPosition::normalizeAngle( angTo + 180 );

    double      dDist     = posAgent.getDistanceTo( posTo );

    Log.log( 509, "moveToPos (%f,%f): body %f to %f diff %f now %f when %f",
             posTo.getX(), posTo.getY(), angBody,
             ( posTo - posPred ).getDirection(), angTo,
             ( posTo -  WM->predictAgentPos( 1, 0 )).getDirection(),
             angWhenToTurn );
    if ( bMoveBack )
    {
        if ( fabs( angBackTo ) < angWhenToTurn )
            return dashToPoint( posTo, iCycles );
        else
            return turnBackToPoint( posTo );
    }
    else if (  fabs( angTo     ) < angWhenToTurn ||
               (fabs( angBackTo ) < angWhenToTurn && dDist < dDistBack ) )
        return dashToPoint( posTo, iCycles );
    else
        return directTowards( posTo, angWhenToTurn );
//return turnBodyToPoint( posTo );
}


/*! This method returns a command that can be used to collide with the ball
    on purpose. When this is not possible. CMD_ILLEGAL is returned. Colliding
    with the ball may be useful when the player is turned with his back to the
    opponent goal and is intercepting a moving ball, by colliding both the ball
    and the player will loose all their velocity. Now the player can turn at
    once to the desired direction. Otherwise he first has to freeze the ball,
    freeze his own speed and then turn around. */
SoccerCommand BasicPlayer::collideWithBall( )
{
    SoccerCommand soc( CMD_ILLEGAL );
    if ( WM->getRelativeDistance( OBJECT_BALL ) >
            WM->getBallSpeed() + SS->getPlayerSpeedMax() )
        return soc;

    VecPosition posBallPred  = WM->predictPosAfterNrCycles( OBJECT_BALL, 1 );

    // first try turn
    soc                      = turnBodyToPoint( WM->getAgentGlobalPosition() +
                               VecPosition( 1, 0, POLAR ) );
    VecPosition posAgentPred = WM->predictAgentPosAfterCommand( soc );
    if ( posAgentPred.getDistanceTo( posBallPred ) <
            SS->getBallSize() + SS->getPlayerSize() )
    {
        Log.log( 511, "can collide with ball by turning" );
        return soc;
    }

    soc          = dashToPoint( posBallPred, 1 );
    posAgentPred = WM->predictAgentPosAfterCommand( soc );
    if ( posAgentPred.getDistanceTo( posBallPred ) <
            SS->getBallSize() + SS->getPlayerSize() )
    {
        Log.log( 511, "can collide with ball by dashing %f", soc.dPower );
        return soc;
    }

    return SoccerCommand( CMD_ILLEGAL );
}
/*! This skill enables an agent to intercept a ball which is close to him. The
    objective is to move in such a way that the ball will come within the
    kickable distance from the agent in one or two cycles. To this end the
    prediction methods from the world model are used to predict the ball
    position in the next cycle and two cycles into the future. It is then
    determined whether it is possible to move the agent within kickable
    distance from one of these positions using all logical combinations of
    turn and dash commands. If it is not possible to intercept the ball within
    two cycles then this skill returns an illegal command to indicate that
    it cannot be performed. First it is determined whether the agent can
    intercept the ball in one cycle. To this end the position of the ball in
    the next cycle is predicted and a calculation is performed to decide
    whether a single dash can move the agent within the kickable distance from
    this position. In order to be able to kick the ball efficiently after
    intercepting it, it is important that the agent moves to a good position
    relative to the ball (i.e. the ball must be in front of him). At the same
    time the agent must make sure that he does not collide with the ball when
    trying to intercept it. Let l be a line that runs forwards and backwards
    from the predicted position of the agent in the next cycle into the
    direction of his body. This line thus denotes the possible movement
    direction of the agent. Note that we have to use the agent's predicted
    position in the next cycle since his current velocity must be taken into
    account. In addition, let c be a circle which is centered on the predicted
    ball position and which has a radius equal to the sum of the radius of the
    agent, the radius of the ball and a small buffer (kickable margin/6). It
    is now determined whether the agent can intercept the ball in the next
    cycle by looking at the number of intersection points between l and c. If
    l and c have exactly one point in common then this point is the desired
    interception point for the next cycle. However, if the number of
    intersection points equals two then the desired point is the one for
    which the absolute angle of the ball relative to that point is the
    smallest. This amounts to the intersection point which is closest to
    the agent when the ball lies in front of him and to the furthest one when
    the ball is behind his back. As a result, the desired interception point
    will always be such that the agent has the ball in front of him in the
    next cycle. Then a dash command is generated
    that will bring the agent as close as possible to the desired point.
    Next, the position of the agent after executing this command is predicted
    and if it turns out that this predicted position lies within the kickable
    distance from the ball then the dash is performed. However, if the
    predicted position is not close enough to the ball or if l and c have no
    points in common then it is assumed that the ball cannot be intercepted
    with a single dash. In these cases, two alternatives are explored to see
    if the ball can be intercepted in two cycles.
    The first alternative is to determine whether the agent can intercept the
    ball by performing a turn followed by a dash. To this end the global
    position of the ball is predicted two cycles into the future and a turn
    command is generated that will turn
    the agent towards this point. The agent's position after executing this
    command is then predicted after which a dash command is generated
    that will bring the agent as close as possible to
    the predicted ball position in two cycles. If it turns out that the
    predicted position of the agent after the dash lies within kickable
    distance from the ball then the first command (i.e. the turn) in the
    sequence of two is performed. Otherwise, a second alternative is tried to
    determine whether the agent can intercept the ball by performing two dash
    commands. To this end two dash commands are generated to get closer to the
    predicted ball position after two cycles.
    If the predicted position of the agent after these two dashes lies within
    kickable distance from the ball then the first dash is performed.
    Otherwise, an illegal command is returned to indicate that the skill
    cannot be performed. The close interception procedure is heavily based on
    a similar method introduced in CMU'99 by Peter Stone.
    \return command to intercept ball in two cycles, CMD_ILLEGAL otherwise */
SoccerCommand BasicPlayer::interceptClose( )
{
    FeatureT feature_type = FEATURE_INTERCEPT_CLOSE;
    if ( WM->isFeatureRelevant( feature_type ) )
        return WM->getFeature( feature_type ).getCommand();

    SoccerCommand soc, socDash1, socFinal, socCollide, socTurn(CMD_ILLEGAL);
    double        dPower, dDist;
    AngDeg        ang,    ang2;
    VecPosition   s1,     s2;
    bool          bReady = false;

    // first determine whether the distance to the ball is not too large
    dDist = 3*SS->getPlayerSpeedMax()
            + (1.0 + SS->getBallDecay())*SS->getBallSpeedMax()
            + SS->getMaximalKickDist();
    if ( WM->getRelativeDistance( OBJECT_BALL ) > dDist )
    {
        bReady = true;
        socFinal = SoccerCommand( CMD_ILLEGAL ); // do not quit, but log feature
    }

    socCollide = collideWithBall( );
    // initialize all variables with information from the worldmodel.
    VecPosition   posAgent = WM->getAgentGlobalPosition( );
    VecPosition   posPred  = WM->predictAgentPos( 1, 0 ), posDash1;
    VecPosition   posBall  = WM->predictPosAfterNrCycles( OBJECT_BALL, 1 );
    VecPosition   velMe    = WM->getAgentGlobalVelocity( );
    Stamina       sta      = WM->getAgentStamina( );
    AngDeg        angBody  = WM->getAgentGlobalBodyAngle( ), angTurn, angNeck=0;
    double        dDesBody = 0.0;

    // our desired heading after the intercept is 0 degrees, only when we
    // are far up front we want to be headed toward the opponent goal
    if ( posAgent.getX() > PENALTY_X - 5.0 )
        dDesBody = (WM->getPosOpponentGoal()-posAgent).getDirection();

    // get the distance to the closest opponent
    double dDistOpp;
    ObjectT objOpp = WM->getClosestInSetTo( OBJECT_SET_OPPONENTS,
                                            WM->getAgentObjectType(), &dDistOpp, PS->getPlayerConfThr() );
    angTurn =VecPosition::normalizeAngle(dDesBody-WM->getAgentGlobalBodyAngle());

    // check the distance to the ball when we do not dash (e.g., and only turn)
    posBall = WM->predictPosAfterNrCycles( OBJECT_BALL, 1 );
    VecPosition posPred1 = WM->predictAgentPos( 1, 0 );
    double dDist1 = posPred1.getDistanceTo( posBall );
    posBall = WM->predictPosAfterNrCycles( OBJECT_BALL, 2 );
    VecPosition posPred2 = WM->predictAgentPos( 2, 0 );
    double dDist2 = posPred2.getDistanceTo( posBall );
    posBall = WM->predictPosAfterNrCycles( OBJECT_BALL, 3 );
    VecPosition posPred3 = WM->predictAgentPos( 3, 0 );
    double dDist3 = posPred3.getDistanceTo( posBall );
    Log.log( 508, "dist 1: %f, 2: %f 3: %f, 0.6: %f", dDist1, dDist2, dDist3,
             0.7*SS->getMaximalKickDist() );

    AngDeg angThreshold = 25;
    bool   bOppClose = ( objOpp != OBJECT_ILLEGAL && dDistOpp < 3.0 )  ;

    // make a line from center of body in next cycle with direction of body
    // use next cycle since current velocity is always propogated to position in
    // next cycle.  Make a circle around the ball with a radius equal to the
    // sum of your own body, the ball size and a small buffer. Then calculate
    // the intersection between the line and this circle. These are the (two)
    // points that denote the possible agent locations close to the ball
    // From these two points we take the point where the body direction of the
    // agent makes the smallest angle with the ball (with backward
    // dashing we sometime have to dash "over" the ball to face it up front)
    posAgent  = WM->getAgentGlobalPosition( );
    posBall   = WM->predictPosAfterNrCycles( OBJECT_BALL, 1 );
    angBody   = WM->getAgentGlobalBodyAngle();
    velMe     = WM->getAgentGlobalVelocity( );
    sta       = WM->getAgentStamina( );
    Line line = Line::makeLineFromPositionAndAngle(posPred1,angBody);
    dDist     = SS->getPlayerSize()+SS->getBallSize()+SS->getKickableMargin()/6;
    int  iSol = line.getCircleIntersectionPoints(
                    Circle(posBall,dDist), &s1, &s2);
    if (iSol > 0)                                          // if a solution
    {
        if (iSol == 2)                                       // take the best one
        {
            ang = VecPosition::normalizeAngle((posBall - s1).getDirection() -angBody);
            ang2= VecPosition::normalizeAngle((posBall - s2).getDirection() -angBody);
//     if ( fabs(ang2) < 90)
            if ( s2.getX() > s1.getX() ) // move as much forward as possible
                s1 = s2;                                          // and put it in s1
        }

        // try one dash
        // now we have the interception point we try to reach in one cycle. We
        // calculate the needed dash power from the current position to this point,
        // predict were we will stand if we execute this command and check whether
        // we are in the kickable distance
        dPower = WM->getPowerForDash(s1-posAgent, angBody, velMe,sta.getEffort() );
        posDash1 = WM->predictAgentPos( 1, (int)dPower);
        if ( posDash1.getDistanceTo( posBall ) < 0.95*SS->getMaximalKickDist() )
        {
            Log.log( 508, "dash 1x possible at s1" );
            socDash1 = SoccerCommand( CMD_DASH, dPower );
        }
        else
        {
            dPower=WM->getPowerForDash(s2-posAgent, angBody, velMe,sta.getEffort() );
            posDash1 = WM->predictAgentPos( 1, (int)dPower);
            if ( posDash1.getDistanceTo( posBall ) < 0.95*SS->getMaximalKickDist() )
            {
                Log.log( 508, "dash 1x possible at s2" );
                socDash1 = SoccerCommand( CMD_DASH, dPower );
            }
        }
    }

    // try one dash by getting close to ball
    // this handles situation where ball cannot be reached within distance
    // SS->getKickableMargin()/6
    if ( socDash1.commandType == CMD_ILLEGAL )
    {
        soc = dashToPoint( posBall );
        WM->predictAgentStateAfterCommand(soc,&posDash1,&velMe,
                                          &angBody,&ang,&sta );
        if ( posDash1.getDistanceTo( posBall ) < 0.95*SS->getMaximalKickDist() )
        {
            Log.log( 508, "dash 1x possible (special)" );
            socDash1 = soc;
        }
    }

    if ( bReady != true )
    {
        if ( bOppClose && ! socDash1.isIllegal() )
        {
            Log.log( 508, "do dash 1x, opponent close" );
            WM->logCircle( 508, posDash1, SS->getMaximalKickDist(), true );
            socFinal = socDash1;
        }
        else
        {
            soc = turnBodyToPoint( posPred1 + VecPosition(1,dDesBody, POLAR), 1 );
            WM->predictAgentStateAfterCommand(soc, &posPred, &velMe,
                                              &angBody, &ang, &sta);
            posBall = WM->predictPosAfterNrCycles( OBJECT_BALL, 1 );
            if ( posPred.getDistanceTo( posBall ) < 0.8*SS->getMaximalKickDist() )
            {
                socTurn = soc; // we can do turn and end up ok, but can maybe improve
                ang     = VecPosition::normalizeAngle(dDesBody-angBody);
                if ( fabs(ang) < angThreshold )
                {
                    socFinal = soc;
                    Log.log( 508, "turn 1x, dist %f, angle %f, opp %f",
                             dDist1, angTurn, dDistOpp );
                    WM->logCircle( 508, posPred1, SS->getMaximalKickDist(), true );
                }

            }
            if ( socFinal.isIllegal() )
            {
                ang     = VecPosition::normalizeAngle(dDesBody-angBody);
                WM->predictStateAfterTurn(
                    WM->getAngleForTurn(ang,velMe.getMagnitude()),
                    &posPred, &velMe, &angBody, &angNeck,
                    WM->getAgentObjectType(),
                    &sta             );
                posBall = WM->predictPosAfterNrCycles( OBJECT_BALL, 2 );
                if ( posPred.getDistanceTo( posBall ) < 0.8*SS->getMaximalKickDist() )
                {
                    socTurn = soc; // we can do turn and end up ok, but can maybe improve
                    ang     = VecPosition::normalizeAngle(dDesBody-angBody);
                    if ( fabs(ang) < angThreshold )
                    {
                        Log.log( 508, "turn 2x, dist %f, angle %f, opp %f",
                                 dDist2, angTurn, dDistOpp );
                        WM->logCircle( 508, posPred2, SS->getMaximalKickDist(), true );
                        socFinal = soc;
                    }
                }
            }
            if ( socFinal.isIllegal() && ! socCollide.isIllegal() &&
                    fabs( angTurn ) > angThreshold )
            {
                Log.log( 508, "collide with ball on purpose" );
                posBall = WM->predictPosAfterNrCycles( OBJECT_BALL, 1 );
                WM->logCircle( 508, posBall, SS->getMaximalKickDist(), true );
                socFinal = socCollide;
            }
            if ( socFinal.isIllegal() && fabs( angTurn ) > angThreshold )
            {
                posBall = WM->predictPosAfterNrCycles( OBJECT_BALL, 2 );
                soc = dashToPoint( posAgent );
                WM->predictAgentStateAfterCommand(soc,
                                                  &posPred,&velMe,&angBody,&ang,&sta );
                if ( posPred.getDistanceTo( posBall ) < 0.8*SS->getMaximalKickDist() )
                {
                    Log.log( 508, "dash 1x (stop), turn 1x, dist %f, angle %f, opp %f",
                             dDist2, angTurn, dDistOpp );
                    WM->logCircle( 508, posPred, SS->getMaximalKickDist(), true );
                    socFinal = soc;
                }
            }
            if ( socFinal.isIllegal()  && ! socTurn.isIllegal() )
            {
                Log.log( 508, "can do turn" );
                WM->logCircle( 508, posPred1, SS->getMaximalKickDist(), true );
                socFinal = socTurn;
            }
            if ( socFinal.isIllegal()  && ! socDash1.isIllegal() )
            {
                Log.log( 508, "do dash 1x" );
                WM->logCircle( 508, posDash1, SS->getMaximalKickDist(), true );
                socFinal = socDash1;
            }
        }

    }
    // if there are no opponents, we are wrongly directed, and we will be closely
    // to the ball, see whether we can first update our heading
    else if ( fabs( angTurn ) > angThreshold && !bOppClose &&
              dDist1 < 0.7*SS->getMaximalKickDist() )
    {
        soc = turnBodyToPoint( posPred1 + VecPosition(1,dDesBody, POLAR), 1 );
        Log.log( 508, "turn 1x, dist %f, angle %f, opp %f",
                 dDist1, angTurn, dDistOpp );
        WM->logCircle( 508, posPred1, SS->getMaximalKickDist(), true );
        socFinal = soc;
    }
    else if ( // fabs( angTurn ) > angThreshold &&
        !bOppClose &&
        dDist2 < 0.7*SS->getMaximalKickDist() )
    {
        soc = turnBodyToPoint( posPred2 + VecPosition(1,dDesBody, POLAR), 2 );
        Log.log( 508, "turn 2x, dist %f, angle %f, opp %f",
                 dDist2, angTurn, dDistOpp );
        WM->logCircle( 508, posPred2, SS->getMaximalKickDist(), true );
        socFinal = soc;
    }


    else if ( socCollide.commandType != CMD_ILLEGAL &&
              fabs( angTurn ) > angThreshold )
    {
        Log.log( 508, "collide with ball on purpose" );
        WM->logCircle( 508, posDash1, SS->getMaximalKickDist(), true );
        socFinal = socCollide;
    }
    else if ( socDash1.commandType != CMD_ILLEGAL )
    {
        Log.log( 508, "do dash 1x" );
        WM->logCircle( 508, posDash1, SS->getMaximalKickDist(), true );
        socFinal = socDash1;
    }


    // cannot intercept ball in three cycles
    WM->setFeature( feature_type,
                    Feature( WM->getTimeLastSeeMessage(),
                             WM->getTimeLastSenseMessage(),
                             WM->getTimeLastHearMessage(),
                             OBJECT_ILLEGAL,
                             -1,
                             socFinal ) );
    return socFinal;
}

/*! This skill enables a goalkeeper to intercept a ball which is close to him.
    The objective is to move in such a way that the ball will come within the
    catchable distance from the agent in one or two cycles. To this end the
    prediction methods from the world model are used to predict the ball
    position in the next cycle and two cycles into the future. It is then
    determined whether it is possible to move the agent within the
    catchable area from one of these positions using all logical combinations
    of turn and dash commands. If it is not possible to intercept the ball
    within two cycles then this skill returns an illegal command to indicate
    that it cannot be performed. First it is determined whether the goalkeeper
    can intercept the ball in one cycle. To this end the position of the ball
    in the next cycle is predicted and a calculation is performed to decide
    whether a single dash can move the agent within catchable distance from
    this position. If it turns out that this is the case, the corresponding
    dash is performed. However, if the predicted position is not close enough
    to the ball then it is assumed that the ball cannot be intercepted with a
    single dash. In these cases, two alternatives are explored to see if the
    ball can be intercepted in two cycles.
    The first alternative is to determine whether the agent can intercept the
    ball by performing two dash commands. To this end two dash commands are
    generated. If the predicted position of the agent after these two
    dashes lies within catchable distance from the ball then the first dash is
    performed. Otherwise, a second alternative is tried to determine whether
    the agent can intercept the ball by performing a turn
    followed by a dash. To this end the global position of the ball is
    predicted two cycles into the future and a turn command is generated
    that will turn the agent towards this point. The agent's position after
    executing this command is then predicted after which a dash command is
    generated that will bring the agent as close as possible to the predicted
    ball position. If it turns out that the predicted position of the agent
    after the dash lies within catchable distance from the ball then the first
    command (i.e. the turn) in the sequence of two is performed.
    Otherwise, an illegal command is returned to indicate that the
    skill cannot be performed. The close interception procedure is heavily
    based on the method used by CMU'99 by Peter Stone.
    \return command to intercept ball in two cycles, CMD_ILLEGAL otherwise */
SoccerCommand BasicPlayer::interceptCloseGoalie( )
{
    SoccerCommand soc;
    double        dPower, dDist;
    AngDeg        ang;
    VecPosition   posClosestToBall;

    // initialize all variables with information from worldmodel.
    VecPosition   posPred   = WM->predictAgentPos( 1, 0 );
    VecPosition   posBall   = WM->predictPosAfterNrCycles( OBJECT_BALL, 1 );
    VecPosition   velMe     = WM->getAgentGlobalVelocity( );
    Stamina       sta       = WM->getAgentStamina( );
    AngDeg        angBody   = WM->getAgentGlobalBodyAngle( );
    Line          lineGoalie=Line::makeLineFromPositionAndAngle(posPred,angBody);

    // when it is theoretical possible
    // try one dash and check whether ball is in catchable area
    dDist =SS->getBallSpeedMax()+SS->getPlayerSpeedMax()+SS->getCatchableAreaL();
    if ( WM->getRelativeDistance( OBJECT_BALL ) < dDist )
    {
        posClosestToBall = lineGoalie.getPointOnLineClosestTo( posBall );
        dPower           = WM->getPowerForDash(
                               posClosestToBall-posPred,
                               angBody,
                               velMe,
                               sta.getEffort()           );
        posPred          = WM->predictAgentPos( 1, (int)dPower);
        if ( posPred.getDistanceTo( posBall ) < SS->getCatchableAreaL() )
            return SoccerCommand( CMD_DASH, dPower );
    }

    // when it is theoretical possible
    // try two dashes and check whether ball is in catchable area
    // otherwise try first two  dashes and check whether ball is in catchable
    // area, thereafter for turn and dash.
    dDist = SS->getBallSpeedMax()*(1.0+SS->getBallDecay())
            + 2*SS->getPlayerSpeedMax()
            + SS->getCatchableAreaL();
    if ( WM->getRelativeDistance( OBJECT_BALL ) < dDist )
    {
        // try two dashes
        // first predict the position in the next cycle when dash with full power
        // is performed. Then calculate the dash power to reach the point where the
        // ball will be in two cycles and predict the global position of the agent
        // after a dash with this power. If the position is in the catchable area
        // return a dash with full power.
        posBall = WM->predictPosAfterNrCycles( OBJECT_BALL, 2 );
        soc     = dashToPoint( posBall );
        WM->predictAgentStateAfterCommand(soc,&posPred,&velMe,&angBody,&ang,&sta );
        dPower=WM->getPowerForDash(posBall-posPred,angBody,velMe,sta.getEffort());
        WM->predictStateAfterDash( dPower, &posPred, &velMe, &sta, angBody,
                                   WM->getAgentObjectType() );
        if ( posPred.getDistanceTo(posBall) < SS->getCatchableAreaL() )
            return soc;

        // try one turn and a dash
        posBall = WM->predictPosAfterNrCycles( OBJECT_BALL, 2 );
        posPred = WM->predictAgentPos( 2, 0 );
        ang     = (posBall - posPred).getDirection();
        ang     = VecPosition::normalizeAngle( ang - angBody );
        if (fabs( ang ) > PS->getPlayerWhenToTurnAngle() ) // if we want to turn
        {
            soc = turnBodyToPoint( posBall, 2 );             // perform turn
            WM->predictAgentStateAfterCommand(soc,&posPred,&velMe,&angBody,&ang,&sta
                                             );
            dPower=WM->getPowerForDash(posBall-posPred, angBody,
                                       velMe, sta.getEffort());
            WM->predictStateAfterDash( dPower, &posPred, &velMe, &sta, angBody);
            if ( posPred.getDistanceTo(posBall) < SS->getCatchableAreaL() )
                return soc;
        }
    }

    // did not succeed
    return SoccerCommand( CMD_ILLEGAL );
}

/*! This skill enables an agent to kick the ball from its current
    position to a given position 'posTarget' in such a way that it has
    a remaining speed equal to 'dEndSpeed' when it reaches this
    position. However, it is possible that the ball cannot reach this
    velocity with a single kick either because the needed magnitude of
    the generated velocity vector exceeds the maximum speed of the
    ball or due to the fact that the current ball speed in combination
    with the position of the ball relative to the agent make it
    impossible to achieve the required acceleration. If the magnitude
    of needed velocity vector is larger than ball speed max it is
    certain that even in the optimal situation (i.e. if the ball lies
    directly in front of the agent and has zero velocity) the agent
    will not be able to kick the ball to the target position at the
    desired speed. In this case the expected ball movement is computed
    after executing a kick with maximum power into the direction of
    'posTarget'.  If the magnitude of the resulting movement vector is
    larger than a given percentage ('getPlayerWhenToKick' defined in
    the PlayerSettings) of the maximum ball speed then this kick is
    actually performed despite the fact that it cannot produce the
    wanted effect. Otherwise, the agent shoots the ball close to his
    body and directly in front of him using the kickBallCloseToBody
    skill. In this way he will be able to kick the ball with more
    power in the next cycle. However, if the magnitude of the desired
    velocity vector is smaller than ball speed max it is possible to
    reach the target point at the desired speed in the optimal
    situation.  If the power that must be supplied to the kick command
    to achieve this acceleration is less than or equal to the maximum
    power the accelerateBallToVelocity skill is used to perform the
    desired kick.  Otherwise, the agent uses the kickBallCloseToBody
    skill to put the ball in a better kicking position for the next
    cycle.

   \param posTarget target position where the ball should be shot to
   \param endSpeed desired speed ball should have in target position
   \return SoccerCommand that produces kick  */
SoccerCommand BasicPlayer::kickTo( VecPosition posTarget, double dEndSpeed )
{
    VecPosition posBall  = WM->getBallPos();
    VecPosition velBall  = WM->getGlobalVelocity(OBJECT_BALL);
    VecPosition posTraj  = posTarget - posBall;
    VecPosition posAgent = WM->getAgentGlobalPosition();
    VecPosition velDes   = VecPosition(
                               WM->getKickSpeedToTravel( posTraj.getMagnitude(), dEndSpeed ),
                               posTraj.getDirection(),
                               POLAR                                                       );
    double      dPower;
    AngDeg      angActual;

    if ( WM->predictAgentPos(1, 0 ).getDistanceTo( posBall + velDes ) <
            SS->getBallSize() + SS->getPlayerSize() )
    {
        Line line = Line::makeLineFromTwoPoints( posBall, posBall + velDes );
        VecPosition posBodyProj = line.getPointOnLineClosestTo( posAgent );
        double dDist = posBall.getDistanceTo( posBodyProj );
        if ( velDes.getMagnitude() < dDist )
            dDist -=  SS->getBallSize() + SS->getPlayerSize();
        else
            dDist +=  SS->getBallSize() + SS->getPlayerSize();
        Log.log( 101, "kick results in collision, change velDes from (%f,%f)",
                 velDes.getX(), velDes.getY() );
        velDes.setVecPosition( dDist, velDes.getDirection(), POLAR );
    }

    Log.log( 101, "ball (%f,%f), agent (%f,%f), to (%f,%f) ang %f %f" ,
             WM->getBallPos().getX(), WM->getBallPos().getY(),
             WM->getAgentGlobalPosition().getX(),
             WM->getAgentGlobalPosition().getY(),
             posTarget.getX(), posTarget.getY(),
             WM->getAgentGlobalBodyAngle(),
             WM->getAgentGlobalNeckAngle() );
    Log.log( 101, "relpos body (%f,%f), vel. ball:(%f,%f) dist: %f (%f,%f,%f)" ,
             WM->getRelativeDistance( OBJECT_BALL ),
             WM->getRelativeAngle( OBJECT_BALL, true ),
             velBall.getX(), velBall.getY(),
             SS->getMaximalKickDist(), SS->getPlayerSize(), SS->getBallSize(),
             SS->getKickableMargin() );


    double   dDistOpp;
    ObjectT  objOpp     = WM->getClosestInSetTo( OBJECT_SET_OPPONENTS,
                          OBJECT_BALL, &dDistOpp );

    if ( velDes.getMagnitude() > SS->getBallSpeedMax() ) // can never reach point
    {
        dPower     = SS->getMaxPower();
        double   dSpeed     = WM->getActualKickPowerRate() * dPower;
        double   tmp        = velBall.rotate(-velDes.getDirection()).getY();
        angActual  = velDes.getDirection() - asinDeg( tmp / dSpeed );
        double   dSpeedPred = (WM->getGlobalVelocity(OBJECT_BALL)+
                               VecPosition(dSpeed,angActual, POLAR )
                              ).getMagnitude();

        // but ball acceleration in right direction is very high
        if ( dSpeedPred > PS->getPlayerWhenToKick()*SS->getBallAccelMax() )
        {
            Log.log( 101, "pos (%f,%f) too far, but can acc ball good to %f k=%f,%f",
                     velDes.getX(), velDes.getY(), dSpeedPred, dSpeed, tmp );
            return accelerateBallToVelocity( velDes );    // shoot nevertheless
        }
        else if ( WM->getActualKickPowerRate() >
                  PS->getPlayerWhenToKick() * SS->getKickPowerRate() )
        {
            Log.log( 101, "point too far, freeze ball" ); // ball well-positioned
            return freezeBall();                          // freeze ball
        }
        else
        {
            Log.log( 101, "point too far, reposition ball (k_r = %f)",
                     WM->getActualKickPowerRate()/(SS->getKickPowerRate()) );
            return kickBallCloseToBody( 0 );            // else position ball better
        }
    }
    else                                            // can reach point
    {
        VecPosition accBallDes = velDes - velBall;
        dPower = WM->getKickPowerForSpeed(accBallDes.getMagnitude());
        if ( dPower <= 1.05*SS->getMaxPower() || // with current ball speed
                (dDistOpp < 2.0 && dPower <= 1.30*SS->getMaxPower() ) )
        {                               // 1.05 since cannot get ball fully perfect
            Log.log( 101, "point good and can reach point %f", dPower );
            return accelerateBallToVelocity( velDes );  // perform shooting action
        }
        else
        {
            Log.log( 101, "point good, but reposition ball since need %f",dPower );
            SoccerCommand soc = kickBallCloseToBody( 0 );
            VecPosition   posPredBall;
            WM->predictBallInfoAfterCommand( soc, &posPredBall );
            dDistOpp = posPredBall.getDistanceTo( WM->getGlobalPosition( objOpp ) );
            return soc;            // position ball better
        }
    }
}


/*! This skill enables an agent to turn towards a global angle while
    keeping the ball in front of him. It is used, for example, when a
    defender has intercepted the ball in his defensive area and faces
    his own goal. In this situation the defender usually wants to pass
    the ball up the field into an area that is currently not visible
    to him and to this end he will first use this skill to turn with
    the ball towards the opponent's goal. Turning with the ball
    requires a sequence of commands to be performed. The ball first
    has to be kicked to a desired position relative to the agent, then
    it has to be stopped dead at that position and finally the agent
    must turn towards the ball again. Each time when this skill is
    called it has to be determined which part of the sequence still
    has to be executed. This is done as follows.  If the absolute
    difference between the desired angle and the global angle of the
    ball relative to the position of the agent is larger than the
    value 'angKickThr' then the kickBallCloseToBody skill is used to
    kick the ball to a position close to the agent and at the desired
    angle.  Otherwise, it is checked whether the ball still has speed
    from the previous action. If the remaining ball speed exceeds the
    given value 'dFreezeThr' then the ball is stopped dead at its
    current position using the freezeBall skill. In all other cases
    the agent turns his body towards the specified angle 'ang'.

    \param ang global direction in which ball and player should be faced
    \param angKickThr when ball angle difference is larger than this value
                      ball is repositioned
    \param dFreezeThr when ball lies correct, but has speed higher than this
                      value, ball is frozen.
    \return Soccercommand to turn with the ball to global angle 'ang'.
   */

/*
SoccerCommand BasicPlayer::turnWithBallTo( AngDeg ang, AngDeg angKickThr,
                                           double dFreezeThr )
{
  // if opponent is close
  // if ball is located more than 'angKickThr' degrees from ang
  //  kick ball to point right in front of player in direction ang
  // else if ball has still speed higher than 'dFreezeThr'
  /// freezeBall
  // else
  //  turn to direction 'ang'
  VecPosition posAgent = WM->getAgentGlobalPosition();
  VecPosition posBall  = WM->getBallPos();
  AngDeg      angBody  = WM->getAgentGlobalBodyAngle();
  AngDeg      angDiff  = (posBall-posAgent).getDirection() - ang;
              angDiff  = VecPosition::normalizeAngle( angDiff );
  double      dDist;
  ObjectT     objOpp   = WM->getClosestInSetTo( OBJECT_SET_OPPONENTS,
                           WM->getAgentObjectType(), &dDist );
  VecPosition posOpp   = WM->getGlobalPosition( objOpp );

  if( objOpp != OBJECT_ILLEGAL && dDist < 2.5 )
  {
    if( posBall.getDistanceTo( posOpp ) < posBall.getDistanceTo( posAgent ) )
    {
      ang = (posOpp - posAgent).getDirection() + 180;
      return kickBallCloseToBody( VecPosition::normalizeAngle( ang-angBody ));
    }
  }
  else if( fabs( angDiff ) > angKickThr )
  {
    Log.log( 101, "turnWithBall: kick ball close %f", ang );
    return kickBallCloseToBody( VecPosition::normalizeAngle( ang - angBody ) );
  }

  // hier niet altijd freezen -> kan dan niet meer goed liggen omdat je zelf
  // doorschiet.
  if( WM->getBallSpeed() > dFreezeThr )
  {
    Log.log( 101, "turnWithBall: freeze ball" );
    return freezeBall();
  }

  ACT->putCommandInQueue( alignNeckWithBody() );
  return turnBodyToPoint( posAgent + VecPosition(1.0, ang, POLAR ) );
}
*/
SoccerCommand BasicPlayer::turnWithBallTo( AngDeg ang, AngDeg, double )
{
    // if opponent is closer to the ball than I am
    //  kick ball away from his direction
    // if the ball will be in my kick_range in the next cycle
    //  turn to direction ang
    // if ball will be in kick range if frozen
    //   freeze_ball
    // else
    //   kickBallCloseToBody(ang)
    VecPosition   posAgent= WM->getAgentGlobalPosition();
    VecPosition   posBall = WM->getBallPos();
    AngDeg        angBody = WM->getAgentGlobalBodyAngle();
    double        dDist;
    ObjectT       objOpp  = WM->getClosestInSetTo( OBJECT_SET_OPPONENTS,
                            WM->getAgentObjectType(), &dDist );
    VecPosition   posOpp  = WM->getGlobalPosition( objOpp );
    SoccerCommand soc     = turnBodyToPoint(posAgent+VecPosition(1.0,ang,POLAR));

    if ( objOpp != OBJECT_ILLEGAL && dDist < 2.5 )
    {
        AngDeg angBall = (posBall-posAgent).getDirection();
        AngDeg angOpp  = (posOpp -posAgent).getDirection();
        if ( fabs( VecPosition::normalizeAngle( angBall - angOpp ) ) < 90  )
        {
            ang = angOpp + 180;
            Log.log( 101, "turnWithBall: kick ball away from opp at ang %f", angOpp);
            return kickBallCloseToBody( VecPosition::normalizeAngle( ang-angBody ));
        }
    }

    VecPosition posAgentPred = WM->predictAgentPosAfterCommand( soc );
    VecPosition posBallPred  = WM->predictPosAfterNrCycles( OBJECT_BALL, 1 );
    if ( posAgentPred.getDistanceTo( posBallPred ) < 0.8*SS->getMaximalKickDist()
            ||
            ( posAgentPred.getDistanceTo(posBallPred) < 0.9*SS->getMaximalKickDist()
              && WM->getBallSpeed() < 0.1 ))
    {
        Log.log( 101, "turnWithBall: turn since ball will be kickable in t+1" );
        return soc;
    }

    posAgentPred = WM->predictPosAfterNrCycles( WM->getAgentObjectType(), 1, 0 );

    // if ball will not be in kickable distance in next turn, we have to freeze
    // the ball. Do this only when current ball position (same as frozen ball
    // position) lies within kickable distance of predicted agent position in
    // next cycle. When ball lies at edge of the kickable distance, do not
    // freeze, since when vel.is not completely correct can move outside area
    // but first we try to collide with the ball, the ball will then lie still
    // and we can turn in one cycle to the desired angle in the next cycle
    SoccerCommand socCollide = collideWithBall( );
    if ( ! socCollide.isIllegal( ) )
    {
        Log.log( 101, "turnWithBall: collide with ball" );
        return socCollide;
    }
    else if ( posAgentPred.getDistanceTo(posBall) < 0.8*SS->getMaximalKickDist()
              &&
              WM->getBallSpeed() > 0.1  )
    {
        Log.log( 101, "turnWithBall: freeze ball" );
        return freezeBall();
    }
    else
    {
        Log.log( 101, "turnWithBall: kick ball close to desired turnangle %f",ang);
        return kickBallCloseToBody( VecPosition::normalizeAngle( ang - angBody ) );
    }
}




/*! This skill enables an agent to move along a line to a given
    position 'pos' on this line. It is used, for example, by the
    goalkeeper who often wants to stay on a line in front of his goal
    and move to different positions on this line depending on where
    the ball is located. Furthermore, it can also used by defenders
    for marking an opponent player by moving along a line between this
    player and the ball. The idea is that the agent must try to move
    as fast as possible to the desired point 'pos' along the line
    thereby keeping the number of turns to a minimum to avoid wasting
    cycles. Apart from the target position 'pos', this skill receives
    several additional arguments for determining whether the agent
    should turn or dash in the current situation. Since the agent can
    only move forwards or backwards into the direction of his body, it
    is important that he tries to keep the orientation of his body
    aligned with the direction of the line in order to be able to move
    quickly to the target point. A given angle 'ang' denotes the
    desired body angle (global) of the agent in the point 'pos'. The
    line l can thus be defined as going through 'pos' and having
    global direction 'ang'. Due to the noise that is added to the
    movement of the agent, the orientation of his body will never be
    exactly equal to 'ang' and as a result the agent' s position will
    start to deviate from the line. Each time when this skill is
    called, the agent's desired orientation is therefore slightly
    adjusted depending on his position with respect to l. If the
    distance d between the agent's current position and the line l is
    smaller than the given value 'dDistThr' then 'ang' remains
    unchanged. However, if d exceeds 'dDistThr' then 'ang' is adjusted
    in such a way that the agent will move closer to l in subsequent
    cycles. This is done by either increasing or decreasing the
    desired orientation 'ang' by 'angCorr' degrees depending on which
    side of the line the agent is located and on a prediction of the
    agent's movement in the forthcoming cycles. This prediction is
    represented by a given value 'iSign' which equals 1 if the agent
    is expected to move in the same direction as 'ang' and -1 if he
    will move in the opposite direction.  Adjusting 'ang' in this way
    has the effect that in subsequent cycles the agent will move
    closer to the line again if this is necessary. The final decision
    whether to turn or dash is now made by comparing the agent's
    current body angle to the desired orientation. If the difference
    between these two angles is larger than 'angThr' degrees then the
    agent uses the turnBodyToPoint skill to turn in the desired
    direction.  Otherwise, the dashToPoint skill is called to move
    towards the target position.

    \param pos global position to which the agent wants to move
    \param ang desired global body angle of agent in position 'pos'
    \param dDistThr threshold value that defines if desired angle is adjusted
    \param iSign indication whether agent predicts that he will move
    in the same direction as 'ang' in the subsequent cycles (iSign=1)
    or in the opposite direction (iSign=-1)
    \param angThr threshold value that specifies when agent will perform a turn
    command
    \param angCorr correction term with which angle is adjusted if necessary
    \return SoccerCommand that will move agent along line defined by position
    'pos' and angle 'ang'. */
SoccerCommand BasicPlayer::moveToPosAlongLine( VecPosition pos, AngDeg ang,
        double dDistThr, int iSign, AngDeg angThr, AngDeg angCorr )
{
    Line        l        = Line::makeLineFromPositionAndAngle( pos, ang );
    VecPosition posBall  = WM->getBallPos();
    VecPosition posAgent = WM->getAgentGlobalPosition();
    AngDeg      angBody  = WM->getAgentGlobalBodyAngle();
    VecPosition posProj  = l.getPointOnLineClosestTo( posAgent );
    double      dDist    = posAgent.getDistanceTo( posProj );
    double      dDiff    = pos.getDistanceTo     ( posProj );

    // if deviated too much from line, compensate
    if ( dDist > dDistThr )
    {
        // check on which side of line agent is located
        VecPosition posOrg(0,0);
        Line        m            = Line::makeLineFromTwoPoints( posOrg, posAgent );
        VecPosition posIntersect = l.getIntersection( m );
        int         iSide;
        if ( posAgent.getDistanceTo(posOrg) < posIntersect.getDistanceTo( posOrg ) )
            iSide = +1;
        else
            iSide = -1;

        // adjust desired turning angle to move back to line in coming cycles
        ang = ang + iSign * iSide * angCorr;
    }

    Log.log( 553, "y difference to defend point %f", dDiff );
    // if current body angle differs much from desired turning angle, turn body
    if ( fabs( VecPosition::normalizeAngle( ang - angBody ) ) > angThr )
    {
        Log.log( 553, "angle differs too much body = %f, des = %f", angBody, ang );
        return turnBodyToPoint( posAgent + VecPosition( 1.0, ang, POLAR ) );
    }
    else if (  posBall.getDistanceTo( OBJECT_BALL ) < 60 && dDiff > 0.6 )
        return dashToPoint( pos );  // move later when ball is far from pen. area.
    else if ( posBall.getDistanceTo( OBJECT_BALL ) < 30  && dDiff > 0.3 )
        return dashToPoint( pos );  // move earlier when is ball near pen. area.
    else
        return SoccerCommand( CMD_ILLEGAL );
}


/********************** HIGH LEVEL SKILLS ************************************/

/*! When the ball-interception skill is called, it is first determined whether
    it is possible for the agent to intercept the ball within two cycles using
    the intermediate player skill closeIntercept (for the goalkeeper
    closeIntereptGoalie). If it turns out that the ball cannot be intercepted
    within  two cycles then the agent uses an iterative scheme to compute the
    optimal interception point. This is done using the method
    'getInterceptionPointBall'.
    \param isGoalie indicates whether the current player is a goalkeeper or not
    \return SoccerCommand to intercept the ball. */
SoccerCommand BasicPlayer::intercept( bool isGoalie )
{
    SoccerCommand soc = (isGoalie)? interceptCloseGoalie():interceptClose(),soc2;
    VecPosition   pos = WM->getAgentGlobalPosition();

    if ( soc.commandType != CMD_ILLEGAL
            && isGoalie )
    {
        Log.log( 502, "intercept in two cycles" );
        return soc;
    }
    Log.log( 608, "start intercept, obj %d", WM->getAgentObjectType()  );
    soc2 = WM->predictCommandToInterceptBall( WM->getAgentObjectType(), soc );
    if ( soc2.isIllegal() )
        return turnBodyToPoint( pos + VecPosition( 1.0, 0, POLAR ) );
    return soc2;
}

/*! This skill enables an agent to dribble with the ball, i.e. to move with the
    ball while keeping it within a certain distance. This amounts to repeatedly
    kicking  the ball at a certain speed into a desired direction and then
    intercepting it again.  Two arguments, the angle 'ang' and type 'dribbleT',
    are supplied to this skill which respectively denote the global direction
    towards which the agent wants to dribble and the kind of dribble that must
    be performed. We distinguish three kinds of dribbling:
    - DRIBBLE FAST: a fast dribble action in which the agent kicks the ball
                    relatively far ahead of him.
    - DRIBBLE SLOW: a slower dribble action in which the agent keeps the ball
                    closer than on a fast dribble.
    - DRIBBLE WITH BALL: a safe dribble action in which the agent keeps the
                    ball very close to his body.

    It is important to realize that this skill is only called when the
    ball is located within the agent's kickable range. This means that
    it is only responsible for the kicking part of the overall
    dribbling behavior, i.e.  it only causes the ball to be kicked a
    certain distance ahead into the desired direction 'ang'. If the
    absolute angle between 'ang' and the agent's body direction is
    larger than DribbleTurnAngle (which currently has a value of 30
    degrees) then the agent uses the turnWithBallTo skill to turn with
    the ball towards the global angle 'ang'. Otherwise, he uses the
    kickTo skill to kick the ball into the desired direction towards a
    point that lies a certain distance ahead depending on the type of
    dribble. After the kick, the ball will move out of the agent's
    kickable range and as a result the agent will try to intercept it
    using the intercept skill. The dribbling skill can then be called
    again once the agent has succeeded in intercepting the ball.  This
    sequence of kicking and intercepting will repeat itself until the
    agent decides to perform another skill. Note that during the
    dribble the power of a kick depends on the distance that the ball
    should travel and on the speed that it should have when it reaches
    the target point.  In our current implementation this speed equals
    0.5 (=DribbleKickEndSpeed) for any type of dribble. Experiments
    have shown that lower end speed values cause the agent to
    intercept the ball before it reaches the target point which slows
    the dribble down significantly.

    \param ang global direction in which should be dribbled
    \param dribbleT type of dribble that should be performed
    \return SoccerCommand to dribble in direction 'ang' */
SoccerCommand BasicPlayer::dribble( AngDeg ang, DribbleT dribbleT )
{
    double        dLength;
    AngDeg        angBody  = WM->getAgentGlobalBodyAngle();
    VecPosition   posAgent = WM->getAgentGlobalPosition();
    SoccerCommand soc;

    // if not turned into correct direction, turn with the ball to that angle
    AngDeg angDiff = VecPosition::normalizeAngle( ang - angBody );
    if ( fabs( angDiff ) > PS->getDribbleAngThr() )
        return turnWithBallTo( ang, PS->getTurnWithBallAngThr(),
                               PS->getTurnWithBallFreezeThr() );

    switch ( dribbleT )
    {
    case DRIBBLE_WITHBALL:
        dLength = 2.0;
        break;
    case DRIBBLE_SLOW:
        dLength = 5.0;
        break;
    case DRIBBLE_FAST:
        dLength = 10.0;
        break;
    default:
        dLength = 0.0;
        break;
    }

    // determine shooting point, relative to agent since moving in that dir.
    VecPosition posDribble = posAgent + VecPosition( dLength, angBody, POLAR );

    // adjust when point lies outside side of field
    // to a point that lies at distance 2.0 from the side of the field
    if ( fabs( posDribble.getY() ) > PITCH_WIDTH/2.0  - 3.0 )
        posDribble.setY( (PITCH_WIDTH/2.0  - 3.0)*sign(posDribble.getY()) );
    if ( fabs( posDribble.getX() ) > PITCH_LENGTH/2.0 - 3.0 )
        posDribble.setX( (PITCH_LENGTH/2.0  - 3.0)*sign(posDribble.getX()) );

    soc = kickTo( posDribble, 0.5 );

    // if ball is kickable but already heading in the right direction, so only
    // small correction term is necessary, start intercepting
    SoccerCommand soc2      = intercept( false );
    if ( soc.dPower < 7  && WM->isCollisionAfterCommand( soc2 ) == false )
    {
        Log.log( 560, "wanted to dribble, but only small kick %f", soc.dPower );
        return soc2;
    }
    return soc;
}

/*! This skill enables an agent to pass the ball directly to another player. It
    receives two arguments, 'pos' and 'passType', which respectively denote the
    position (of usually a teammate) to which the agent wants to pass the ball
    and the kind of pass (either normal or fast) that should be given. This
    skill uses the kickTo skill to pass the ball to the specified position
    with a certain desired end speed depending on the type of pass.
    \param pos position of object to which a direct pass should be given
    \param passType kind of pass (either PASS_NORMAL or PASS_FAST )
    \return SoccerCommand to perform a direct pass to object 'o' */
SoccerCommand BasicPlayer::directPass( VecPosition pos, PassT passType)
{
    if ( passType == PASS_NORMAL )
        return kickTo( pos, PS->getPassEndSpeed() );
    else if ( passType == PASS_FAST )
        return kickTo( pos, PS->getFastPassEndSpeed() );
    else
        return SoccerCommand( CMD_ILLEGAL );
}

/*! This skill enables an agent to give a leading pass to another player. A
    leading pass is a pass into open space that `leads' the receiver, i.e.
    instead of passing the ball di:rectly to another player it is kicked just
    ahead of him. In this way the receiver is able to intercept the ball while
    moving in a forward direction and this will speed up the attack. This skill
    receives two arguments, an object o (usually a teammate) and dist, which
    respectively denote the intended receiver of the leading pass and the
    `leading distance' ahead of the receiver. It uses the kickTo skill to pass
    the ball to a point that lies dist in front of the current position of o.
    Here `in front of' means in positive x-direction, i.e. at a global angle of
    0 degrees. Note that the desired end speed for a leading pass is always
    equal to PassEndSpeed (currently 1.4) since the leading aspect of the pass
    might cause the receiver to miss the ball when its speed is higher.
    \param o object to which a leading pass should be given
    \param dDist distance in front of o to which is passed
    \return SoccerCommand to perform a leading pass to object 'o' */
SoccerCommand BasicPlayer::leadingPass( ObjectT o, double dDist, DirectionT dir)
{
    VecPosition posShoot = WM->getGlobalPosition( o );
    if ( dir != DIR_ILLEGAL && dir != DIR_CENTER )
        posShoot+=VecPosition(dDist,SoccerTypes::getAngleFromDirection(dir),POLAR);
    return kickTo( posShoot, PS->getPassEndSpeed() );
}

/*! This skill enables an agent to give a more advanced type of pass
    called a through pass. With a through pass the ball is not passed
    directly to another player or just ahead of him, but it is kicked
    into open space between the opponent defenders and the opponent
    goalkeeper in such a way that a teammate (usually an attacker)
    will be able to reach the ball before an opponent does. If a
    through pass is executed successfully it often causes a
    disorganization of the opponent's defense which will enable an
    attacker to get the ball close to the enemy goal. This skill takes
    an object o (usually a teammate) as an argument which denotes the
    intended receiver of the through pass. The position p on the field
    to which the ball should be kicked is determined by drawing a line
    l from the object's current position to a given point 'pos' also
    supplied as an argument) and by computing the safest trajectory
    for the ball to a point on this line. To this end the widest angle
    between opponents from the current ball position to a point p on l
    is calculated. After this, the speed that the ball should have
    when it reaches this point is determined based on the distance
    from the current ball position to p and on the number of cycles n
    that o will need to reach p. If it turns out that the required end
    speed falls outside the range [MinPassEndSpeed .. MaxPassEndSpeed]
    it is set to the closest boundary of this range. The kickTo skill
    is then used to kick the ball to the desired point p at the
    required speed.

    \param o Object to which through pass should be given
    \param posEnd position that together with ball position defines line
              segment on which shooting point should be determined.
    \param *angMax will be filled with the largest angle between the opponents
    \return SoccerCommand to give a throughPass to 'o' */
SoccerCommand BasicPlayer::throughPass( ObjectT o, VecPosition posEnd,
                                        AngDeg *angMax )
{
    VecPosition posShoot = getThroughPassShootingPoint( o, posEnd, angMax );
    double      dEnd     = getEndSpeedForPass( o, posShoot );

    return kickTo( posShoot, dEnd );
}

/*! This skill enables an agent to outplay an opponent. It is used, for
    example, when an attacker wants to get past an enemy defender. This is
    done by passing the ball to open space behind the defender in such a
    way that the attacker can beat the defender to the ball. Note that the
    attacker has an advantage in this situation, since he knows to which
    point he is passing the ball and is already turned in the right
    direction, whereas the defender is not. As result, the attacker has a
    headstart over the defender when trying to intercept the ball. Since a
    player can move faster without the ball, the main objective is to kick
    the ball as far as possible past the opponent while still being able to
    reach it before the opponent does. This skill receives two arguments,
    'pos' and 'o', which respectively denote the point to which the agent
    wants to move with the ball and the object (usually an opponent) that the
    agent wants to outplay in doing so. First it is determined if it is
    possible to outplay the opponent o in the current situation. Let l be the
    line segment that runs from the agent's current position to the given
    point 'pos'. The best point to kick the ball to will be the furthest point
    from on this line that the agent can reach before the opponent does. We
    use a simple geometric calculation to find the point s on l which has
    equal distance to the agent and to the opponent. Let o' be the
    perpendicular projection of the opponent's position onto l and let d1,
    d2 and d3 respectively denote the distance between the agent poisition and
    o', the distance between o' and o and the distance between o' and s. To
    determine s we need to compute the unknown value of d3 using the values
    for d1 and d2 which can be derived from the world model. Since the
    distance from the agent position to s will be equal to the distance
    from o to s. Using this value for d3 we can compute the coordinates of
    the shooting point s. However, in some situations it is not likely that
    shooting the ball to this point will eventually result in outplaying
    the given opponent o on the way to 'pos'. We therefore use the values
    for d1, d2 and d3 to determine whether it is possible to outplay o in
    the current situation, and if so, what the best shooting point will be.
    The following situations are distinguished:
    - d1 + d3 > OutplayMinDist.
    If this condition holds, the opponent is located at a relatively large
    distance from the agent which makes an attempt to outplay him likely to
    be successful. First it is checked whether the agent's body is turned
    suffiiently towards the point 'pos'. If this is not the case then the
    turnWithBallTo skill is used to turn with the ball in the right
    direction. Otherwise, the kickTo skill is used to kick the ball to a
    point on the line l where the agent will be able to intercept it first.
    Note that in general the agent will be able to reach the point s before
    the opponent despite the fact that both players need to travel the same
    distance to this point. This is because the agent has already turned his
    body more or less towards 'pos' (and thus towards s), whereas the opponent
    probably has not. However, the actual point to which the ball is kicked
    is chosen slightly closer to the agent than the point s in order to be
    absolutely sure that he can intercept the ball before the opponent does.
    This safety margin is represented by the parameter OutplayBuffer which has
    a value of 2.5 in our current implementation. For this skill the
    desired end speed when the ball reaches z equals OutplayKickEndSpeed
    (=0.5). Note that the value for this parameter cannot be chosen too
    low, since this will cause the agent to intercept the ball before it
    reaches the target point
    - d1 + d3 < OutplayMinDist and d1 < d2.
    If this condition holds, the opponent is located close to the agent
    which makes it di±cult to outplay him. However, if the agent is already
    turned in the right direction (i.e. towards 'pos') then d1 < d2 implies
    that the distance between the opponent and the line l (denoting the
    agent's desired movement trajectory) is large enough for the agent to
    outplay this opponent when the ball is kicked hard in the direction of
    'pos' (i.e. further ahead than s). This is because the agent can start
    dashing after the ball immediately, whereas the opponent still has to
    turn in the right direction. As a result, the agent will have dashed
    past the opponent by the time the latter has turned correctly and this
    puts him in a good position to intercept the ball before the opponent.
    Therefore it is checked first whether the agent's body is sufficiently
    turned towards 'pos' and if this is not so then the turnWithBallTo skill
    is used toturn with the ball in the right direction. Otherwise, the
    kickTo skill is used to kick the ball past the opponent. In this case the
    point to which the ball is kicked either lies OutplayMaxDist (=20) metres
    ahead of the agent's current position into the direction of 'pos' or
    equals 'pos' when the distance to 'pos' is smaller than this value. In
    all other cases (i.e. d1 +d3 < OutplayMinDist and d1 > d2) this
    skill returns an illegal command to indicate that it is not
    possible to outplay the opponent o on the way to the point 'pos'.
    \param o opponent object that should be outplayed
    \param pos position to which agent wants to move while outplaying 'o'
    \param *posTo position to which ball will be shot
    \return SoccerCommand to outplay object o on the way to 'pos',
       CMD_ILLEGAL when this is not possible. */
SoccerCommand BasicPlayer::outplayOpponent( ObjectT o, VecPosition pos,
        VecPosition *posTo )
{
    // future: take more than one opponent into account

    VecPosition posAgent   = WM->getAgentGlobalPosition();
    AngDeg      angBody    = WM->getAgentGlobalBodyAngle();

    double dMaxDist = 12.0;
    if ( posAgent.getX() > PENALTY_X - 6.0 )
        dMaxDist = 10.0;

    AngDeg      ang        = (pos - posAgent).getDirection();
    // if not heading in the desired direction, first turn with the ball
    AngDeg angTmp = VecPosition::normalizeAngle( ang - angBody ) ;
    if ( fabs( angTmp ) > PS->getDribbleAngThr() )
        return turnWithBallTo( ang, PS->getTurnWithBallAngThr(),
                               PS->getTurnWithBallFreezeThr() );
    ang = WM->getAgentGlobalBodyAngle();
    Line        l          = Line::makeLineFromPositionAndAngle(posAgent,ang);
    VecPosition posObj     = WM->getGlobalPosition( o );
    VecPosition posProj    = l.getPointOnLineClosestTo( posObj );
    double      dDistOpp   = posProj.getDistanceTo( posObj );
    double      dDistAgent = posProj.getDistanceTo( posAgent );
    VecPosition posShoot;

    // we want to know when distance from ball to point p equals distance
    // from opp to point p :
    // d1 + d3 = sqrt(d2^2 + d3^2) > (d1+d3)^2 = d2^2 + d3^2 =>
    // d1^2 + 2*d1*d3 = d2^2 -> d3 = (d2^2 - d1^2 ) / 2*d1
    double dCalcDist;
    if ( o != OBJECT_ILLEGAL )
    {
        dCalcDist = (dDistOpp*dDistOpp-dDistAgent*dDistAgent)/(2*dDistAgent);
        dCalcDist += dDistAgent;
    }
    else
        dCalcDist = dMaxDist;

    Log.log( 552, "outplay opponent %d, calc: %f, opp: %f, agent:  %f",
             SoccerTypes::getIndex( o ) + 1, dCalcDist, dDistOpp, dDistAgent );
    Log.log( 560, "outplay opponent %d, calc: %f, opp: %f, agent:  %f",
             SoccerTypes::getIndex( o ) + 1, dCalcDist, dDistOpp, dDistAgent );

    if ( dCalcDist > 7.0 ) // if point far away, kick there
    {
        dCalcDist = min( posAgent.getDistanceTo( pos ), dCalcDist - 2.5 );
        dCalcDist = min( dMaxDist, dCalcDist );
        posShoot  = posAgent + VecPosition( dCalcDist, ang, POLAR );
    }
    else if ( dDistAgent < dDistOpp - 0.3 ) // point close and well-positioned
    {                                      // shoot far away and outplay opp
        dCalcDist = min( posAgent.getDistanceTo( pos ), dMaxDist );
        posShoot = posAgent + VecPosition( dCalcDist, ang, POLAR );
    }
    else                                   // opponent stands in line
        return SoccerCommand( CMD_ILLEGAL );

    if ( posShoot.getDistanceTo( WM->getAgentGlobalPosition() ) < 2.5 )
    {
        Log.log( 552, "calculated point too close" );
        Log.log( 560, "calculated point too close (%f,%f)", posShoot.getX(),
                 posShoot.getY() );
        return SoccerCommand( CMD_ILLEGAL );
    }
    else if ( WM->getNrInSetInCone( OBJECT_SET_OPPONENTS,PS->getConeWidth(),
                                    posAgent, posShoot ) != 0 )
    {
        Log.log( 552, "outplay: is opponent in cone" );
        Log.log( 560, "outplay: is opponent in cone (%f,%f)", posShoot.getX(),
                 posShoot.getY() );
        return SoccerCommand( CMD_ILLEGAL );
    }
    else if ( WM->getNrInSetInCircle( OBJECT_SET_OPPONENTS,
                                      Circle( posShoot, posShoot.getDistanceTo( posAgent ) ) ) > 1 )
    {
        Log.log( 552, "outplay: nr of opp in circle > 1" );
        Log.log( 560, "outplay: nr of opp in circle > 1" );
        return SoccerCommand( CMD_ILLEGAL );
    }
    else if ( WM->getCurrentTime() - WM->getTimeGlobalAngles(o) < 3 )
    {
        double dDistOpp = posShoot.getDistanceTo( posObj ) ;
        if ( fabs( VecPosition::normalizeAngle( (posShoot - posObj).getDirection() -
                                                WM->getGlobalBodyAngle(o) ) ) < 30 )
            dDistOpp -= 1.0;
        if ( dDistOpp < posAgent.getDistanceTo( posShoot ) )
        {
            Log.log( 560, "outplay: opponent closer" );
            return SoccerCommand( CMD_ILLEGAL );
        }
    }



    if ( posTo != NULL )
        *posTo = posShoot;
    return kickTo( posShoot, 0.5 );
}

/*! This skill enables an agent to clear the ball to a certain area on the
    field. It is useful, for example, when a defender cannot dribble or pass
    the ball to a teammate in a dangerous situation. Using this skill he can
    then kick the ball up the field away from the defensive zone. It is
    important to realize that this skill is only called when the agent has no
    alternative options in the current situation. Clearing the ball amounts to
    kicking it with maximum power into the widest angle between opponents in
    a certain area. The shooting direction is determined using the function
    which returns the direction of the bisector of this widest angle. The area
    on the field from which this angle is selected depends on the type of
    clear which is supplied as an argument to this skill. We distinguish
    three types of clearing:
    - CLEAR BALL DEFENSIVE: clearing the ball away from the defensive zone
      into a triangular area which is defined by the current ball position
      and the center line on the field.
    - CLEAR BALL OFFENSIVE: clearing the ball towards the offensive zone into
      a triangular area which is defined by the current ball position and the
      line segment that coincides with the front line of the opponent's
      penalty area and extends to the left and right side lines.
    - CLEAR BALL GOAL: clearing the ball into a triangular area in front of the
      opponent's goal which is defined by the current ball position and the
      line segment that runs from the center of the opponent's goal to the
      center of the front line of the penalty area.
    \param type type of the clear ball skill
    \param s if specified indicates the part of the field the clearBall should
            be directed to.
    \param angMax if specified (and not NULL) will be filled with the angle
           between the opponents in the direction in which will be shot
    \return SoccerCommand kick command to clear the ball */
SoccerCommand BasicPlayer::clearBall( ClearBallT type, SideT s,AngDeg *angMax )
{
    VecPosition posBall = WM->getBallPos();
    VecPosition posLeft, posRight;
    double      clearDist = PS->getClearBallDist();

    double      dPitchY = PITCH_WIDTH / 2.0;
    if ( type == CLEAR_BALL_DEFENSIVE )
    {
        posLeft.setVecPosition ( 0, - dPitchY + clearDist );
        posRight.setVecPosition( 0, + dPitchY - clearDist );
    }
    else if ( type == CLEAR_BALL_OFFENSIVE )
    {
        posLeft.setVecPosition ( PENALTY_X - clearDist, - dPitchY + clearDist );
        posRight.setVecPosition( PENALTY_X - clearDist, + dPitchY - clearDist );
    }
    else if ( type == CLEAR_BALL_OFFENSIVE_SIDE )
    {
        posLeft.setVecPosition ( PENALTY_X - clearDist, - dPitchY + 8 );
        posRight.setVecPosition( PENALTY_X - clearDist, + dPitchY - 8 );
    }
    else if ( type == CLEAR_BALL_GOAL && posBall.getY() > 0 )
    {
        posLeft.setVecPosition ( max(PENALTY_X - 2.0, posBall.getX()-10),  0.0 );
        posRight.setVecPosition( PITCH_LENGTH/2.0 - 5.0, 0.0 );
    }
    else if ( type == CLEAR_BALL_GOAL && posBall.getY() < 0 )
    {
        posLeft.setVecPosition ( PITCH_LENGTH/2.0 - 5.0, 0.0 );
        posRight.setVecPosition( max(PENALTY_X - 2.0, posBall.getX()-10),  0.0 );
    }
    else
        return SoccerCommand( CMD_ILLEGAL );

    if ( type != CLEAR_BALL_GOAL && s == SIDE_RIGHT ) // take only right part of
    {
        if ( type == CLEAR_BALL_OFFENSIVE_SIDE )
            posLeft.setY( 15.0 );
        else
            posLeft.setY ( 0.0 );                          // field into account

    }
    else if ( type != CLEAR_BALL_GOAL && s == SIDE_LEFT )
    {
        if ( type == CLEAR_BALL_OFFENSIVE_SIDE )
            posRight.setY( -15.0 );
        else
            posRight.setY( 0.0 );
    }

    // get angle of ball with left and right points
    // get the largest angle between these two angles
    AngDeg angLeft  = (posLeft  - posBall).getDirection();
    AngDeg angRight = (posRight - posBall).getDirection();
    double dDist;
    if ( type != CLEAR_BALL_GOAL )
        dDist = PS->getClearBallOppMaxDist();
    else
        dDist = max( posBall.getDistanceTo(posLeft ),
                     posBall.getDistanceTo(posRight) );
    AngDeg ang      = WM->getDirectionOfWidestAngle( posBall, angLeft, angRight,
                      angMax, dDist );

    Line l1 = Line::makeLineFromPositionAndAngle( posBall, ang );
    Line l2 = Line::makeLineFromTwoPoints( posLeft, posRight );
    VecPosition posShoot = l1.getIntersection( l2 );
    Log.log( 560, "angLeft %f, right %f, best %f point (%f,%f)",
             angLeft, angRight, ang, posShoot.getX(), posShoot.getY() );
    if ( type == CLEAR_BALL_GOAL  )
        return kickTo( posShoot, SS->getBallSpeedMax() );
    else if ( type == CLEAR_BALL_OFFENSIVE )
        return kickTo( posShoot, 0.25 );
    else if ( type == CLEAR_BALL_OFFENSIVE_SIDE )
    {
        return kickTo( posShoot, (posBall.getX()>20) ? 1.2 : 2.7);
    }
    else
        return kickTo( posShoot, 0.5 );
}

/*! This skill enables an agent to mark an opponent, i.e. to guard him
    one-on-one with the purpose to minimize his usefulness for the opponent
    team. It can be used, for example, to block the path from the ball to
    an opponent or from an opponent to the goal. In this way the agent can
    prevent this opponent from receiving a pass or from moving closer to the
    goal while also obstructing a possible shot. This skill amounts to
    calculating the desired marking position based on the given arguments
    and then moving to this position. It receives three arguments: an
    object o (usually an opponent) that the agent wants to mark, a distance
    'dDist' representing the desired distance between o and the marking
    position and a type indicator that denotes the type of marking that is
    required. We distinguish three types of marking:
    - MARK BALL: marking the opponent by standing at a distance 'dDist' away
                 from him on the line between him and the ball. This type of
                 marking will make it di±cult for the opponent to receive a
                 pass.
    - MARK GOAL: marking the opponent by standing at a distance 'dDist' away
                 from him on the line between him and the center point of the
                 goal he attacks. This type of marking will make it difficult
                 for the opponent to score a goal.
    - MARK BISECTOR: marking the opponent by standing at a distance 'dDist'
                 away from him on the bisector of the ball-opponent-goal
                 angle. This type of marking enables the agent to intercept
                 both a direct and a leading pass to the opponent.

    After determining the marking position, the agent uses the moveToPos skill
    to move to this position. Note that the decision whether to turn or dash
    in the current situation depends on the angle of the marking position
    relative to the agent's body direction and on the distance to this position
    if this point lies behind the agent. In this case the moveToPos skill uses
    the threshold parameters MarkTurnAngle (=30) and MarkDistanceBack (=3) to
    make this decision. The values for these parameters are such that the
    condition which must hold for allowing a dash is fairly flexible. This is
    done because the marking position will be different in consecutive cycles
    due to the fact that the opponent and the ball move around from each
    cycle to the next. As a result, the agent will be able to actually
    progress towards a point that lies close to the marking position instead
    of constantly turning towards the newly calculated marking position in
    each cycle.
    \param o object that has to be marked
    \param dDist distance marking position is located from object position
    \param mark marking technique that should be used
    \return SoccerCommand to mark object 'o'. */
SoccerCommand BasicPlayer::mark( ObjectT o, double dDist, MarkT mark )
{
    VecPosition posMark  = getMarkingPosition( o, dDist, mark );
    VecPosition posAgent = WM->getAgentGlobalPosition();
    VecPosition posBall  = WM->getGlobalPosition( OBJECT_BALL );
//  AngDeg      angBody  = WM->getAgentGlobalBodyAngle();

    if ( o == OBJECT_BALL )
    {
        if ( posMark.getDistanceTo( posAgent ) < 1.5 )
            return turnBodyToObject( OBJECT_BALL );
        else
            return moveToPos( posMark, 30.0, 3.0, false );
    }

    if ( posAgent.getDistanceTo( posMark ) < 2.0 )
    {
        AngDeg angOpp = (WM->getGlobalPosition( o ) - posAgent).getDirection();
        AngDeg angBall = (posBall - posAgent).getDirection();
        if ( isAngInInterval( angBall, angOpp,
                              VecPosition::normalizeAngle( angOpp + 180 ) ) )
            angOpp += 80;
        else
            angOpp -= 80;
        angOpp  = VecPosition::normalizeAngle( angOpp );
        Log.log( 513, "mark: turn body to ang %f", angOpp );
        return turnBodyToPoint( posAgent + VecPosition( 1.0, angOpp, POLAR )  );
    }
    Log.log( 513, "move to marking position" );

    return moveToPos( posMark, 25, 3.0, false );
}


/*! This skill enables an agent (usually the goalkeeper) to defend his
    own goal line. To this end the agent moves to a position along a
    line l which runs parallel to the goal line at a small distance
    'dDist' (supplied as an argument) from the goal. The guard point
    to which the agent moves depends on the predicted position of the
    ball in the next cycle and is chosen in anticipation of a future
    shot on goal. This means that the guard point is selected in such
    a way that it will be most difficult for the opponent team to pass
    the goalkeeper. To find this point we need to look at the angle
    that the ball makes with respect to the left and right goal posts
    and we need to determine which point on l covers this angle in the
    most optimal way, i.e.  leaves an equal gap to the left and to the
    right of the goalkeeper. Let m be the line that goes through the
    center point of the goal line and through the predicted ball
    position in the next cycle. Since this line m bisects the
    above-mentioned angle, the optimal guard point on l can be
    computed by determining the intersection between l and m. Note
    that in our current implementation the goalkeeper always stays in
    front of the goal mouth to avoid leaving an open goal when the
    ball is passed to an opponent in the center of the penalty
    area. The computed guard point is therefore adjusted if it lies
    too far to the side . After computing the guard point the
    goalkeeper needs to move to this point while keeping sight of the
    ball.  If the distance between the current goalkeeper position and
    the line l is larger than DefendGoalLineMaxDist (which has a value
    of 3.0 in our current implementation) the moveToPos skill is used
    to move directly towards the guard point. This can happen, for
    example, when the goalkeeper has moved forward from his line to
    intercept the ball and now has to move back to his line
    again. Note that the fourth argument supplied to the moveToPos
    skill equals true in this case to indicate that the goalkeeper
    wants to turn his back to the guard point in order to keep the
    ball in sight while moving.  However, if the distance between the
    guard point and l is less than DefendGoalLineMaxDist then the
    moveToPosAlongLine skill is used to move along l to the guard
    point. This skill receives an argument `sign' representing a
    prediction of the agent's movement in the coming cycles.  This
    value is used to adjust the agent's body direction when necessary.
    In this case it can be expected that the goalkeeper will move
    along l in the same direction as the ball and `sign' is therefore
    determined by looking at the ball velocity in cycle t.

    \param dDist distance before goal the goalkeeper will move along
    \return SoccerCommand to defend the goal line. */
SoccerCommand BasicPlayer::defendGoalLine( double dDist )
{
    // determine defending point as intersection keeper line and line ball-goal
    VecPosition posBall    = WM->getBallPos();
    VecPosition posAgent   = WM->getAgentGlobalPosition();
    VecPosition posGoal    = WM->getPosOwnGoal( );
    VecPosition posCenter(sign(posGoal.getX())*(fabs(posGoal.getX())-dDist),0);
    Line        lineGoal   = Line::makeLineFromPositionAndAngle( posCenter, 90 );

    VecPosition posGoalLeft ( posGoal.getX(), -SS->getGoalWidth()/2.0 );
    VecPosition posGoalRight( posGoal.getX(),  SS->getGoalWidth()/2.0 );
    Line left    = Line::makeLineFromTwoPoints( posBall, posGoalLeft  );
    Line right   = Line::makeLineFromTwoPoints( posBall, posGoalRight );
    posGoalLeft  = left.getIntersection ( lineGoal );
    posGoalRight = right.getIntersection( lineGoal );
    double dDistLeft  = posGoalLeft.getDistanceTo( posBall );
    double dDistRight = posGoalRight.getDistanceTo( posBall );
    double dDistLine  = posGoalLeft.getDistanceTo( posGoalRight );
    VecPosition posDefend  = posGoalLeft+
                             VecPosition( 0, (dDistLeft/(dDistLeft+dDistRight))*dDistLine);

    bool        bBallInPen = WM->isInOwnPenaltyArea( posBall );

    // do not stand further to side than goalpost
    if ( fabs( posDefend.getY() ) > SS->getGoalWidth()/2.0 )
        posDefend.setY( sign(posDefend.getY())*SS->getGoalWidth()/2.0);

    // if too far away from line, move directly towards it
    double dDiff = ( bBallInPen == true ) ? 1.5 : 0.5;
    if ( posDefend.getX() + dDiff < posAgent.getX()  )
    {
        Log.log( 553, "move backwards to guard point" );
        return moveToPos( posDefend, 30, -1.0, true ); // always backwards
    }
    else if ( posDefend.getX() - dDiff > posAgent.getX() )
    {
        Log.log( 553, "move forward to guard point" );
        return moveToPos( posDefend, 30, -1.0 );       // always forward
    }

    // desired body angle is in direction of the ball
    // predicted movement direction in subsequent cycles is in moving dir. ball
    AngDeg  angDes;
    if ( fabs( posBall.getY() - posDefend.getY() ) > 0.5 )
        angDes = sign( posBall.getY() - posDefend.getY() )*90.0;
    else
        angDes = sign( WM->getAgentGlobalBodyAngle() )*90.0;
    int     iSign     = sign( WM->getGlobalVelocity( OBJECT_BALL ).getY() );

    // move to position along line: when ball in penalty area, never adjust body
    // angle (value 3.0) and change trajectory when angle difference is > 7
    // when ball is outside pen. area, adjust angle to move to line when more
    // than 0.5 from desired line, adjust when angle diff > 2 with 12 degrees
    if ( bBallInPen )
    {
        Log.log( 553, "move along line, with ball in penalty area x: %f ang %f",
                 posDefend.getX(), angDes );
        return moveToPosAlongLine( posDefend, angDes, 3.0, iSign, 7.0, 12.0 );
    }
    else
    {
        Log.log( 553, "move along line, with ball out pen. area (%f,%f)(%f,%f) %f",
                 WM->getAgentGlobalPosition().getX(),
                 WM->getAgentGlobalPosition().getY(),
                 posBall.getX(), posBall.getY(), WM->getConfidence( OBJECT_BALL) );
        Log.log( 553, "%s", WM->strLastSeeMessage );
        return moveToPosAlongLine( posDefend, angDes, 0.5, iSign, 2.0, 12.0 );
    }
}

/*! This method returns a command to intercept a ball that is currently
    heading towards the goal. The current trajectory of the ball is determined
    and the goalkeeper positions himself on a point on this trajectory just
    before the goal.
    \return SoccerCommand to intercept a ball heading towards the goal. */
SoccerCommand BasicPlayer::interceptScoringAttempt( )
{
    int iSide = 1;
    if ( WM->isPenaltyUs() || WM->isPenaltyThem() )
        iSide = ( WM->getSide() == WM->getSidePenalty() ) ? 1 : -1;

    SoccerCommand soc;
    VecPosition   posAgent = WM->getAgentGlobalPosition();
    VecPosition   posBall  = WM->getBallPos();
    Line          lineBall = Line::makeLineFromPositionAndAngle( posBall,
                             WM->getBallDirection() );
    Line          lineKeeper = Line::makeLineFromPositionAndAngle( posAgent, 90);
    bool          bInterceptAtSide = false;

    // first create intersection point between ball trajectory and rectangle
    // that is located just in front of the goal mouth
    VecPosition   posIntersect = lineBall.getIntersection( lineKeeper );
    if ( fabs( posIntersect.getY() ) > SS->getGoalWidth()/2.0  )
    {
        VecPosition posGoalPost( -iSide*PITCH_LENGTH/2.0,
                                 sign(posBall.getY())*(SS->getGoalWidth()/2.0 + 0.5));
        Line l = Line::makeLineFromPositionAndAngle( posGoalPost, 0 );
        posIntersect = lineBall.getIntersection( l );

        // if intersection point does not cross rectangle between the agent and
        // the goalpost, we just move to edge of the rectangle
        if ( fabs(posIntersect.getX()) > fabs(posGoalPost.getX())  ||
                fabs(posIntersect.getX()) < fabs(posAgent.getX()) )
            posIntersect.setVecPosition( posAgent.getX(), posGoalPost.getY() );
        else
            bInterceptAtSide = true; // interception point is at side of rectangle
    }

    // first try close intercept
    soc = interceptCloseGoalie();
    if ( ! soc.isIllegal() )
    {
        Log.log( 553, "close intercept" );
        return soc;
    }

    // if far away from goal, just intercept ball
    if ( PITCH_LENGTH/2.0 - fabs(posAgent.getX()) > 8.0 )
    {
        Log.log( 553, "I am far away from keeper line: %d %d %f %f",
                 WM->getSide(), WM->getSidePenalty(), -iSide*PITCH_LENGTH/2.0 ,
                 posAgent.getX() );
        return intercept( true );
    }
    else if ( fabs(posBall.getX()) > fabs(posAgent.getX()) &&
              fabs( posBall.getY() ) < SS->getGoalWidth()/2.0 + 2.0 )
    {
        Log.log( 553, "ball heading and ball behind me" );
        return intercept( true );
    }

    // move to interception point
    if ( posIntersect.getDistanceTo( posAgent ) < 0.5 )
    {
        Log.log( 553, "close to intersection point keeperline" );
        soc = turnBodyToObject( OBJECT_BALL );
    }
    else if ( sign( (posIntersect - posAgent ).getDirection() ) ==
              sign( WM->getAgentGlobalBodyAngle()             ) ||
              bInterceptAtSide == true)
    {
        Log.log( 553, "move forward to intersection point keeperline" );
        soc = moveToPos( posIntersect, 20, SS->getGoalWidth() );
    }
    else
    {
        Log.log( 553, "move backward to intersection point keeperline" );
        soc = moveToPos( posIntersect, 20, SS->getGoalWidth(), true );
    }

    return soc;
}

/*! This method returns a command to hold the ball close to your body. When no
    opponents are close the ball is kicked in front of the body of the agent.
    Otherwise it is kiced to that spot in the kickable distance which is
    hardest for the opponent to tackle. */
SoccerCommand BasicPlayer::holdBall( )
{
    double        dDist;
    VecPosition   posAgent = WM->getAgentGlobalPosition();
    ObjectT       objOpp   = WM->getClosestInSetTo( OBJECT_SET_OPPONENTS,
                             OBJECT_BALL, &dDist );
    VecPosition   posOpp   = WM->getGlobalPosition ( objOpp );
    AngDeg        angOpp   = WM->getGlobalBodyAngle( objOpp );
    AngDeg        ang      = 0.0;

    if ( objOpp != OBJECT_ILLEGAL && dDist < 5 )
    {
        // get the angle between the object to the agent
        // check whether object is headed to the left or right of this line
        ang = ( posAgent - posOpp ).getDirection();
        int iSign = -sign(VecPosition::normalizeAngle( angOpp - ang ));
        ang +=  iSign*45 - WM->getAgentGlobalBodyAngle();
        ang = VecPosition::normalizeAngle( ang );
        Log.log( 512, "hold ball: opp close shoot to ang %f", ang );
    }
    else
        Log.log( 512, "hold ball: in direction body: %f", ang );

    if ( WM->getBallPos().getDistanceTo( posAgent + VecPosition( 0.7, ang, POLAR ) )
            < 0.3 )
    {
        SoccerCommand soc          = turnBodyToPoint( WM->getPosOpponentGoal() );
        VecPosition   posBallPred = WM->predictPosAfterNrCycles( OBJECT_BALL, 1 );
        VecPosition   posPred     = WM->predictAgentPosAfterCommand( soc );
        if ( posPred.getDistanceTo( posBallPred ) < 0.85 * SS->getMaximalKickDist() )
        {
            Log.log( 512, "hold ball: turn body to goal, ball remains still" );
            return soc;
        }
    }

    return kickBallCloseToBody( ang, 0.7 ) ;
}

/********************** UTILITY METHODS **************************************/

/*! This method returns the shooting point for a through pass to objTeam.
    This point lies on the line l that is created of the estimated position of
    the teammate after 3 cycles and 'posEnd'. Between these two points the
    maximal angle with respect to the opponents is calculated and the widest
    angle is calculated (this value is stored in angMax afterwards). The
    intersection between this widest angle and the line l is returned as the
    shooting point.
    \param objTeam teammate to give a throughpass to
    \param posEnd end point to which throughpasses are considered
    \param angMax will be filled with the maximal angle between the opponents
    \return position to give a through pass to. */
VecPosition BasicPlayer::getThroughPassShootingPoint( ObjectT objTeam,
        VecPosition posEnd, AngDeg  *angMax  )
{
    VecPosition posTeam = WM->getGlobalPosition( objTeam );
    return getShootPositionOnLine( posTeam, posEnd, angMax );
}

/*! This method uses an iterative scheme to compute the optimal
    interception point of the ball. A loop is executed in which the
    prediction methods are used to predict the position of the ball a
    number of cycles, say i, into the future and to predict the number
    of cycles, say n, that the agent will need to reach this
    position. This is repeated for increasing values of i until n < i
    in which case it is assumed thay the agent should be able to reach
    the predicted ball position before the ball does.  This point is
    chosen as the interception point and the moveToPos skill is used
    to move towards this point.

    \param iCyclesToBall will be filled with the nr of cycles it will take the
                        ball to reach the returned position.
    \param isGoalie indicates whether the current player is a goalkeeper or not
    \return position to intercept the ball. */
VecPosition BasicPlayer::getInterceptionPointBall( int *iMinCyclesBall,
        bool isGoalie )
{
    static Time timeLastMinCycles(-1,0);
    static Time timeLastIntercepted(-1,0);
    VecPosition posPred  = WM->getAgentGlobalPosition();
    VecPosition velMe    = WM->getAgentGlobalVelocity();
    double      dSpeed, dDistExtra;
    VecPosition posMe, posBall;
    AngDeg      ang, angBody, angNeck;
    Stamina     sta;
    double      dMaxDist, dBestX = -100;
    int         iCyclesBall, iCyclesOpp, iFirstBall = -100;//, iCyclesOppPoint;


    if ( (WM->getCurrentTime() - timeLastIntercepted) > 2 )
        timeLastMinCycles.updateTime( -1 );
    timeLastIntercepted = WM->getCurrentTime();
    *iMinCyclesBall = 100;

    if ( isGoalie )
        return getActiveInterceptionPointBall( iMinCyclesBall, isGoalie );

    dMaxDist = (isGoalie) ? SS->getCatchableAreaL() : SS->getMaximalKickDist();

    // predict the position of the agent when current velocity is propogated
    dSpeed     = WM->getAgentSpeed();
    dDistExtra = Geometry::getSumInfGeomSeries( dSpeed, SS->getPlayerDecay() );
    posPred   += VecPosition( dDistExtra, velMe.getDirection(), POLAR );

    ObjectT objOpp = WM->getFastestInSetTo(OBJECT_SET_OPPONENTS,
                                           OBJECT_BALL, &iCyclesOpp);
    Log.log( 501, "closest opponent can get to ball in %d cycles", iCyclesOpp );
    if ( WM->isBeforeGoal( WM->getAgentGlobalPosition() ) )
        iCyclesOpp = 20;
    int i = 2; // otherwise should be done by interceptClose
    // for each loop, check whether agent can reach ball in less cycles
    // do not look further than cycles that fastest opponent can reach the ball
    while ( i <= PS->getPlayerWhenToIntercept() && i <= iCyclesOpp + 2 )
    {
        posBall = WM->predictPosAfterNrCycles( OBJECT_BALL, i );

        // if too far away, try next cycle
        if ( posPred.getDistanceTo(posBall)/SS->getPlayerSpeedMax() > i + dMaxDist
                ||
                WM->isInField( posBall ) == false )
        {
            i++;
            continue;
        }

        // (re-)initialize all the needed variables
        // set ball prediction one further to get right in front of ball line
        posMe   = WM->getAgentGlobalPosition();
        velMe   = WM->getAgentGlobalVelocity();
        angBody = WM->getAgentGlobalBodyAngle();
        angNeck = WM->getAgentGlobalNeckAngle();
        ang     = (posBall - posPred).getDirection();
        ang     = VecPosition::normalizeAngle( ang - angBody );
        sta     = WM->getAgentStamina();
        int turn = 0;

        // as long as not correctly headed for point, simulate a turn command
        while (fabs(ang) > PS->getPlayerWhenToTurnAngle() && turn<5)
        {
            turn++;
            WM->predictStateAfterTurn( WM->getAngleForTurn(ang,velMe.getMagnitude()),
                                       &posMe, &velMe, &angBody, &angNeck,WM->getAgentObjectType(),
                                       &sta             );
            ang      = (posBall - posPred).getDirection();
            ang      = VecPosition::normalizeAngle( ang - angBody );
        }

        if ( turn > 1 )
            Log.log( 501, "nr of turns needed: %d", turn );
        int iTurnTmp = turn;
        // for cycles that are left over after turn(s), execute full power dash
        for ( ; turn < i; turn++ )
            WM->predictStateAfterDash(SS->getMaxPower(),&posMe,&velMe,&sta,angBody);

        // if in kickable distance or passed ball, we can reach the ball!
        if (posMe.getDistanceTo( posBall ) < dMaxDist  ||
                (posMe.getDistanceTo( posPred ) > posBall.getDistanceTo( posPred ) -
                 dMaxDist) )
        {
            Log.log( 501, "can intercept in %d cycles %d turns, ball %f best %f",
                     i, iTurnTmp, posBall.getX(), dBestX );

            if ( *iMinCyclesBall == 100 ) // log first possible interception point
            {
                *iMinCyclesBall = i;
                iFirstBall = i;
            }

            iCyclesBall = i;
            if ( WM->getCurrentTime() + iCyclesBall == timeLastMinCycles )
            {
                Log.log( 501, "choose old interception point %d", iCyclesBall );
                return posBall;
            }
            if ( objOpp == OBJECT_ILLEGAL || isGoalie == true )
            {
                // if no opponent, move to first reachable point
                if ( *iMinCyclesBall == 100 )
                {
                    *iMinCyclesBall = iCyclesBall;
                    i = PS->getPlayerWhenToIntercept() + 1; // and quit
                }
            }
            else if ( WM->isBeforeGoal( WM->getAgentGlobalPosition() ) )
            {
                if ( fabs(posBall.getY()) < fabs(dBestX ) &&
                        iCyclesBall < iFirstBall + 3   &&
                        (iCyclesOpp - iCyclesBall)  >= 2  )
                {
                    dBestX = fabs(posBall.getY());
                    *iMinCyclesBall = iCyclesBall;
                    Log.log( 501, "update best cycles in goalarea to %d, opp %d",
                             *iMinCyclesBall, (iCyclesOpp - iCyclesBall) );
                }

            }
            else if ( posBall.getX() >= dBestX &&
                      fabs( posBall.getY() ) < 32.0 &&
                      fabs( posBall.getX() ) < 50.0 &&
                      ( posBall.getX() > - PITCH_LENGTH/4.0 ||
                        iCyclesBall < iFirstBall + 3 ) &&
                      (iCyclesOpp - iCyclesBall)  >= 1 )
            {
                dBestX = posBall.getX();
                *iMinCyclesBall = iCyclesBall;
                Log.log( 501, "update best cycles to %d, opp diff %d", *iMinCyclesBall,
                         iCyclesOpp - iCyclesBall );
            }
        }
        i++;
    }

    if ( i == 2 || ( i == iCyclesOpp + 3 && *iMinCyclesBall > iCyclesOpp ))
    {
        Log.log( 501, "move to interception point closest opp." );
        *iMinCyclesBall = iCyclesOpp;
    }

    posBall = WM->predictPosAfterNrCycles( OBJECT_BALL, *iMinCyclesBall );
    Log.log( 501, "choose: %d", *iMinCyclesBall );
    timeLastMinCycles = WM->getCurrentTime() + *iMinCyclesBall;

    return posBall;
}


/*! This method intercepts the ball at the first possible position.
    \param iCyclesBall is the nr of cyles after the ball is intercepted
    \param isGoalie bool to indicate that a goalie has to intercept the ball
    \return intercept position */
VecPosition BasicPlayer::getActiveInterceptionPointBall( int *iCyclesBall,
        bool isGoalie )
{
    VecPosition posPred  = WM->getAgentGlobalPosition();
    VecPosition velMe    = WM->getAgentGlobalVelocity();
    double      dSpeed, dDistExtra;
    VecPosition posMe, posBall;
    AngDeg      ang, angBody, angNeck;
    Stamina     sta;
    double      dMaxDist;

    if ( isGoalie &&
            !WM->isInOwnPenaltyArea(WM->predictPosAfterNrCycles( OBJECT_BALL, 45)))
    {
        *iCyclesBall = -1;
        return posBall;
    }

    dMaxDist = (isGoalie) ? SS->getCatchableAreaL() : SS->getMaximalKickDist();

    // predict the position of the agent when current velocity is propogated
    dSpeed     = WM->getAgentSpeed();
    dDistExtra = Geometry::getSumInfGeomSeries( dSpeed, SS->getPlayerDecay() );
    posPred   += VecPosition( dDistExtra, velMe.getDirection(), POLAR );

    // for each loop, check whether agent can reach ball in less cycles
    for ( int i = 0; i <= PS->getPlayerWhenToIntercept(); i++ )
    {
        // (re-)initialize all the needed variables
        // set ball prediction one further to get right in front of ball line
        velMe   = WM->getAgentGlobalVelocity();
        angBody = WM->getAgentGlobalBodyAngle();
        angNeck = WM->getAgentGlobalNeckAngle();
        posBall = WM->predictPosAfterNrCycles( OBJECT_BALL, i + 1 );
        posMe   = WM->getAgentGlobalPosition();
        ang     = (posBall - posPred).getDirection();
        ang     = VecPosition::normalizeAngle( ang - angBody );
        sta     = WM->getAgentStamina();
        int turn = 0;

        // as long as not correctly headed for point, simulate a turn command
        while (fabs(ang) > PS->getPlayerWhenToTurnAngle() && turn<5)
        {
            turn++;
            WM->predictStateAfterTurn( WM->getAngleForTurn(ang,velMe.getMagnitude()),
                                       &posMe, &velMe, &angBody, &angNeck,WM->getAgentObjectType(),
                                       &sta             );
            ang      = (posBall - posPred).getDirection();
            ang      = VecPosition::normalizeAngle( ang - angBody );
        }

        if ( turn > 1 )
        {
            Log.log( 502, "nr of turns needed: %d", turn );
        }

        // for cycles that are left over after turn(s), execute full power dash
        for ( ; turn < i; turn++ )
            WM->predictStateAfterDash(SS->getMaxPower(),&posMe,&velMe,&sta,angBody);

        // if in kickable distance or passed ball, we can reach the ball!
        if (posMe.getDistanceTo( posBall ) < dMaxDist  ||
                (posMe.getDistanceTo( posPred ) > posBall.getDistanceTo( posPred ) +
                 dMaxDist) )
        {
            WM->logCircle( 501, posBall, 2, true );
            WM->logCircle( 501, posBall, 1, true );
            *iCyclesBall = i;
            Log.log( 501, "intercept ball in %d cycles", *iCyclesBall );
            return posBall;
        }
    }

    *iCyclesBall = -1;
    return posBall;
}

VecPosition BasicPlayer::getDribblePoint( DribbleT dribble, double *dDist )
{
    AngDeg        angBody = WM->getAgentGlobalBodyAngle() ;
    VecPosition   posAgent = WM->getAgentGlobalPosition() ;
    VecPosition   pos;
    double        dLength;

    switch ( dribble )
    {
    case DRIBBLE_WITHBALL:
        dLength = 5.0;   // 6: max 4.58, average 2.66
        *dDist = 4;
        break;
    case DRIBBLE_SLOW:
        dLength = 8.0;   // 8: max 6.47, average 3.34
        *dDist = 3.8;
        break;         // 9: max 8.57, average 5.38
    case DRIBBLE_FAST:
        dLength = 14.0;  // 14; max: 15.93 average 11.71
        *dDist = 12;
        break;
    default:
        dLength = 0.0;
        break;
    }

    // get point to shoot ball to
    pos = posAgent + VecPosition( dLength, angBody, POLAR );

    // adjust when point lies outside side of field
    // to a point that lies at distance 2.0 from the side of the field
    if ( ( fabs( pos.getY() ) > PITCH_WIDTH/2.0  - 3.0 && fabs(angBody) > 3) ||
            ( fabs( pos.getX() ) > PITCH_LENGTH/2.0 - 3.0 ) )
        pos = WM->getOuterPositionInField( WM->getAgentGlobalPosition(),
                                           WM->getAgentGlobalBodyAngle(), 2.0, false );

    return pos;
}

/*! This method returns the point on a line segment with which the ball has
    the largest angle with the surrounding opponents. It uses the method
    getDirectionOfWidestAngle. The line is determined by the two position
    p1 and p2. The returned position lies on this line and makes the largest
    angle with the opponents. The actual angle between the opponents is
    returned by 'angLargest'.
    \param p1 first position of line segment
    \param p2 second position of line segment
    \param *angLargest will be filled with the largest angle with the opponents
    \return VecPosition position on line that has the largest angle with the
            opponents */
VecPosition BasicPlayer::getShootPositionOnLine( VecPosition p1,
        VecPosition  p2, AngDeg *angLargest )
{
    VecPosition posBall  = WM->getBallPos();
    Line   line          = Line::makeLineFromTwoPoints( p1, p2 );
    double dRadius       = min( PS->getClearBallOppMaxDist(),
                                posBall.getDistanceTo( p2 )  );
    AngDeg angMin        = (p1 - posBall ).getDirection();
    AngDeg angMax        = (p2 - posBall ).getDirection();
    // not correct when line crosses -180 boundary, but will never happenk
    AngDeg angShoot      = WM->getDirectionOfWidestAngle(
                               posBall, min(angMin, angMax),
                               max(angMin,angMax), angLargest, dRadius );
    Line   line2         = Line::makeLineFromPositionAndAngle( posBall,
                           angShoot );
    return line.getIntersection( line2 );
}

/*! This method returns the end speed for a pass. This end speed is
    determined using two parameters, 'o' and 'posPass' which
    respectively denote the object to which is passed and the position to
    which is passed. First it is determined how many server cycles 'o'
    needs to travel to position 'posPass'. Then the starting speed of the ball
    is determined when it wants to travel to 'posPass' in the same number of
    cycles. This starting speed is adjusted if the corresponding end speed
    of the ball in the passing point lies outside the range
    [MinPassEndSpeed .. MaxPassEndSpeed].
    \param o object to which the ball will be passed
    \param posPass desired passing point where o can intercept the ball
    \return end speed to give to the ball such that player can intercept
                the ball the best. */
double BasicPlayer::getEndSpeedForPass( ObjectT o, VecPosition posPass )
{
    // we want that the ball arrives at that point after length nr of cycles
    // where length is the nr of cycles it takes the player to get there.
    VecPosition posBall = WM->getBallPos();
    double      dDist   = posBall.getDistanceTo( posPass );
    double      dLength = WM->predictNrCyclesToPoint( o, posPass );
    double      dFirst  = WM->getFirstSpeedFromDist( dDist, dLength );
    if ( dFirst > SS->getBallSpeedMax() )
        dFirst = SS->getBallSpeedMax();
    double dEnd         = WM->getEndSpeedFromFirstSpeed( dFirst, dLength );
    if ( dEnd > PS->getPassEndSpeed() )
        dEnd = PS->getPassEndSpeed();
    else if ( dEnd < 0.6 )
        dEnd = 0.6;
    else if ( dLength > 10.0 )
        dEnd = 0.6;

    return dEnd;
}

/*! This method returns a global position on the field which denotes the
position
    to mark object 'o'. It receives three arguments: an
    object o (usually an opponent) that the agent wants to mark, a distance
    'dDist' representing the desired distance between o and the marking
    position and a type indicator that denotes the type of marking that is
    required. We distinguish three types of marking:
    - MARK BALL: marking the opponent by standing at a distance 'dDist' away
                 from him on the line between him and the ball. This type of
                 marking will make it di±cult for the opponent to receive a
                 pass.
    - MARK GOAL: marking the opponent by standing at a distance 'dDist' away
                 from him on the line between him and the center point of the
                 goal he attacks. This type of marking will make it difficult
                 for the opponent to score a goal.
    - MARK BISECTOR: marking the opponent by standing at a distance 'dDist'
                 away from him on the bisector of the ball-opponent-goal
                 angle. This type of marking enables the agent to intercept
                 both a direct and a leading pass to the opponent.
    \param o object that has to be marked
    \param dDist distance marking position is located from object position
    \param mark marking technique that should be used
    \return position that is the marking position. */
VecPosition BasicPlayer::getMarkingPosition( ObjectT o, double dDist,
        MarkT mark)
{
    VecPosition pos      = WM->getGlobalPosition( o );
    // except on back line assume players is moving to goalline
    if ( pos.getX() > - PITCH_LENGTH/2.0 + 4.0 )
        pos -= VecPosition( 1.0, 0.0 );

    return WM->getMarkingPosition( pos, dDist, mark );

}

//new add

/*以下这个函数可以向空隙较大的一方射门，采用夹角比较
 * 参数：自己位置
 * 返回值：command型
 * 需要在BasicPlayer.h里添加内容
 */
SoccerCommand  BasicPlayer::maxAngShoot( VecPosition posAgent )//朝较大的夹角射门
{
    SoccerCommand soc;
    VecPosition posGoalie;
    AngDeg ang_goalie, angup, angdown;
		
    posGoalie = WM->getGlobalPosition(WM->getOppGoalieType());
    ang_goalie = (posGoalie - posAgent).getDirection();
    angup = (VecPosition(52.5,6.0) - posAgent).getDirection();
    angdown = (VecPosition(52.5,-6.0) - posAgent).getDirection();
	
    if(fabs(angup - ang_goalie) > fabs(angdown - ang_goalie))
    {
	soc = kickTo(VecPosition(52.5, -6.5),2.7);
    }
    else 
    {
	soc = kickTo(VecPosition(52.5, 6.5),2.7);
    }
    
    return soc;
}

SoccerCommand  BasicPlayer::maxAngShoot1( VecPosition posAgent )//朝较大的夹角射门
{
    SoccerCommand soc;
		VecPosition posGoal(PITCH_LENGTH/2.0, 
				       (-1 + 2*(WM->getCurrentCycle()%2)) * 
				       0.4 * SS->getGoalWidth());
		soc=kickTo( posGoal,SS->getBallSpeedMax());// 朝球门方向将球以最大力度
    return soc;
}

SoccerCommand BasicPlayer::dribble_Dodge(VecPosition tar, VecPosition posAgent)//闪避带球
{
    SoccerCommand soc;
    //Circle cir(posAgent,7);
    //ObjectT o = WM->getClosestInSetTo(OBJECT_SET_OPPONENTS,posAgent);
    //VecPosition p = WM->getGlobalPosition(o);
    //int num = WM->getNrInSetInCircle( OBJECT_SET_OPPONENTS, cir );
    
    AngDeg ang = ( tar - posAgent ).getDirection();
    soc = dribble(ang, DRIBBLE_FAST );
    
    if( WM->isOpponentAtAngleEx( ang - 45, ang , 7 ) )
    {
	ang += 45;
	soc = dribble( ang , DRIBBLE_FAST );
    }
    else if( WM->isOpponentAtAngleEx( ang, ang + 45 , 7 ) )
    {
	ang -= 45;
	soc = dribble( ang , DRIBBLE_FAST );
    }

    return soc;
}

VecPosition BasicPlayer::pointKickToC()//计算角球应该踢向的点
{
    VecPosition posBall, posKickTo;
    double x, y;
    posBall = WM->getBallPos();
    x = posBall.getX();
    y = posBall.getY();
    
    posKickTo.setX(- x / fabs(x) * 5 + x );
    posKickTo.setY(- y / fabs(y) * 12 + y ); 
    
    return posKickTo;
}

/* 此函数返回界外球应该踢向的点
 * 策略是把球尽可能往敌方半场踢
 * 
 */
VecPosition BasicPlayer::pointKickToK()//计算界外球应该踢向的点
{
    VecPosition posBall, posKickTo;
    double x, y;
    posBall = WM->getBallPos();
    x = posBall.getX();
    y = posBall.getY();
    
    posKickTo.setX( x + 7 );
    posKickTo.setY(- y / fabs(y) * 12 + y ); 
    
    return posKickTo;
}

/*
  // stop and then turn
  // this is stamina intensive??
  if( bReady == true )
     ;
  else if( fabs( angTurn ) > angThreshold && !bOppClose &&
      WM->getAgentSpeed() > 0.1 )
  {
    posBall = WM->predictPosAfterNrCycles( OBJECT_BALL, 2 );
    soc = dashToPoint( posAgent );
    WM->predictAgentStateAfterCommand(soc,&posPred,&velMe,&angBody,&ang,&sta );
    if( posPred.getDistanceTo( posBall ) < 0.8*SS->getMaximalKickDist() )
    {
      Log.log( 508, "dash 1x (stop), turn 1x, dist %f, angle %f, opp %f",
         dDist2, angTurn, dDistOpp );
      WM->logCircle( 508, posPred, SS->getMaximalKickDist(), true );
      socFinal = soc;
    }
  }
  // if there are no opponents, we are wrongly directed, and we will be closely
  // to the ball, see whether we can first update our heading
  else if( fabs( angTurn ) > angThreshold && !bOppClose &&
      dDist1 < 0.7*SS->getMaximalKickDist() )
  {
    soc = turnBodyToPoint( posPred1 + VecPosition(1,dDesBody, POLAR), 1 );
    Log.log( 508, "turn 1x, dist %f, angle %f, opp %f",
      dDist1, angTurn, dDistOpp );
    WM->logCircle( 508, posPred1, SS->getMaximalKickDist(), true );
    socFinal = soc;
  }
  else if( // fabs( angTurn ) > angThreshold &&
      !bOppClose &&
      dDist2 < 0.7*SS->getMaximalKickDist() )
  {
    soc = turnBodyToPoint( posPred2 + VecPosition(1,dDesBody, POLAR), 2 );
    Log.log( 508, "turn 2x, dist %f, angle %f, opp %f",
       dDist2, angTurn, dDistOpp );
    WM->logCircle( 508, posPred2, SS->getMaximalKickDist(), true );
    socFinal = soc;
  }


  else if( socCollide.commandType != CMD_ILLEGAL &&
           fabs( angTurn ) > angThreshold )
  {
    Log.log( 508, "collide with ball on purpose" );
    WM->logCircle( 508, posDash1, SS->getMaximalKickDist(), true );
    socFinal = socCollide;
  }
  else if( socDash1.commandType != CMD_ILLEGAL )
  {
    Log.log( 508, "do dash 1x" );
    WM->logCircle( 508, posDash1, SS->getMaximalKickDist(), true );
    socFinal = socDash1;
  }
*/
