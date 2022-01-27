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

/*! \file PlayerSettings.cpp
<pre>
<b>File:</b>          PlayerSettings.cpp
<b>Project:</b>       Robocup Soccer Simulation Team: UvA Trilearn
<b>Authors:</b>       Jelle Kok
<b>Created:</b>       28/11/2000
<b>Last Revision:</b> $ID$
<b>Contents:</b>      Code file for class PlayerSettings. It contains all the
               member method implementations of the PlayerSettings class.
               This class contains all the settings that are important
               for the client (agent) to determine its actions.
<hr size=2>
<h2><b>Changes</b></h2>
<b>Date</b>             <b>Author</b>          <b>Comment</b>
28/11/2000       Jelle Kok       Initial version created
</pre>
*/

#include "PlayerSettings.h"

/*****************************************************************************/
/********************* CLASS PlayerSettings **********************************/
/*****************************************************************************/

/*! This method initializes all client settings and adds these to the generic
    values class with the effect that they can referenced by their textual
    name. */
PlayerSettings::PlayerSettings( ) : GenericValues("PlayerSettings", 27)
{
    dPlayerConfThr         = 0.88; // threshold below player info is illegal
    dPlayerHighConfThr     = 0.92; // threshold above which player info is high
    dBallConfThr           = 0.90; // threshold below which ball info is illegal
    dPlayerDistTolerance   = 7.5;  // distance when unknownplayer is mapped
    dPlayerWhenToTurnAngle = 7.0;  // angle when to turn when moving
    dPlayerWhenToKick      = 0.85; // % of kick power rate when kick is performed
    iPlayerWhenToIntercept = 30;   // maximum number of interception cycles
    dClearBallDist         = 5.0;  // dist before penalty area to clear ball to
    dClearBallOppMaxDist   = 30.0; // radius in which opp in clear ball has to be
    dClearBallToSideAngle  = 17.0; // minimum angle for clear ball to side
    dConeWidth             = 0.5;  // cone width to check for opponents in pass
    dPassEndSpeed          = 1.2;  // end speed for ball when passed to teammate
    dFastPassEndSpeed      = 1.8;  // end speed for ball when passed fast
    dPassExtraX            = 0.0;  // extra x value added to player passing pos
    dFractionWaitNoSee     = 0.61;  // % of cycle to wait in cycle with no see
    dFractionWaitSeeBegin  = 0.70; // % to wait in cycle with see in begin
    dFractionWaitSeeEnd    = 0.85; // % to wait in cycle with see in 2nd  half
    dMarkDistance          = 5.0;  // mark distance to a player
    dStratAreaRadius       = 5.0;  // radius around strat pos to find optimal pos
    dShootRiskProbability  = 0.88; // prob. of scoring when shooting with risk
    iCyclesCatchWait       = 20;   // cycles to wait after a catch
    iServerTimeOut         = 9;    // seconds before server is assumed dead
    dDribbleAngThr         = 20.0; // angle thr. to turn to ball before dribbling
    dTurnWithBallAngThr    = 45.0; // angle threshold to kick ball
    dTurnWithBallFreezeThr = 0.2;  // ball speed threshold to freeze ball
    iInitialFormation      = 2;    // initial formation number
    dMaxYPercentage        = 0.8;  // max y in strat. pos (percentage of field)

    // add all the settings and link text string to variable
    addSetting( "player_conf_thr"     , &dPlayerConfThr,  GENERIC_VALUE_DOUBLE );
    addSetting( "player_high_conf_thr", &dPlayerHighConfThr,
                GENERIC_VALUE_DOUBLE );
    addSetting( "ball_conf_thr"      ,  &dBallConfThr,    GENERIC_VALUE_DOUBLE );
    addSetting( "player_dist_tolerance",&dPlayerDistTolerance,
                GENERIC_VALUE_DOUBLE );
    addSetting( "player_when_to_turn",  &dPlayerWhenToTurnAngle,
                GENERIC_VALUE_DOUBLE );
    addSetting( "player_when_to_kick", &dPlayerWhenToKick,GENERIC_VALUE_DOUBLE );
    addSetting( "player_when_to_intercept",&iPlayerWhenToIntercept,
                GENERIC_VALUE_INTEGER);
    addSetting( "clear_ball_dist"    ,  &dClearBallDist,  GENERIC_VALUE_DOUBLE );
    addSetting( "clear_ball_opp_max_dist", &dClearBallOppMaxDist,
                GENERIC_VALUE_DOUBLE );
    addSetting( "clear_ball_side_angle", &dClearBallToSideAngle,
                GENERIC_VALUE_DOUBLE );
    addSetting( "cone_width"         ,  &dConeWidth,      GENERIC_VALUE_DOUBLE );
    addSetting( "pass_end_speed"     ,  &dPassEndSpeed,   GENERIC_VALUE_DOUBLE );
    addSetting( "fast_pass_end_speed", &dFastPassEndSpeed,GENERIC_VALUE_DOUBLE );
    addSetting( "pass_extra_x"       ,  &dPassExtraX,     GENERIC_VALUE_DOUBLE );
    addSetting( "wait_no_see"        ,&dFractionWaitNoSee,GENERIC_VALUE_DOUBLE );
    addSetting( "wait_see_begin" , &dFractionWaitSeeBegin,GENERIC_VALUE_DOUBLE );
    addSetting( "wait_see_end"    , &dFractionWaitSeeEnd, GENERIC_VALUE_DOUBLE );
    addSetting( "mark_distance"      ,  &dMarkDistance,   GENERIC_VALUE_DOUBLE );
    addSetting( "strat_area_radius"  ,  &dStratAreaRadius,GENERIC_VALUE_DOUBLE );
    addSetting( "shoot_risk_prob", &dShootRiskProbability,GENERIC_VALUE_DOUBLE );
    addSetting( "cycles_catch_wait"  ,  &iCyclesCatchWait,GENERIC_VALUE_INTEGER);
    addSetting( "server_time_out"    ,  &iServerTimeOut,  GENERIC_VALUE_INTEGER);
    addSetting( "dribble_ang_thr"    ,  &dDribbleAngThr,  GENERIC_VALUE_DOUBLE );
    addSetting( "turn_with_ball_ang_thr" , &dTurnWithBallAngThr,
                GENERIC_VALUE_DOUBLE );
    addSetting( "turn_with_ball_freeze_thr" , &dTurnWithBallFreezeThr,
                GENERIC_VALUE_DOUBLE );
    addSetting( "initial_formation"  , &iInitialFormation,GENERIC_VALUE_INTEGER);
    addSetting( "max_y_percentage"   , &dMaxYPercentage,  GENERIC_VALUE_DOUBLE );

}

/*! This method returns the confidence threshold below which player information
    is assumed illegal
   \return player confidence threshold */
double PlayerSettings::getPlayerConfThr( ) const
{
    return dPlayerConfThr;
}

/*! This method sets the confidence threshold below which player information is
    assumed illegal
    \param d player confidence threshold in range [0..1]
    \return boolean indicating whether update was successful */
bool PlayerSettings::setPlayerConfThr( double d )
{
    dPlayerConfThr = d;
    return true;
}

/*! This method returns the confidence threshold above which player information
    is assumed very good.
    \return player high confidence threshold */
double PlayerSettings::getPlayerHighConfThr( ) const
{
    return dPlayerHighConfThr;
}

/*! This method sets the confidence threshold above which player information is
    assumed very good
    \param d player high confidence threshold in range [0..1]
    \return boolean indicating whether update was successful */
bool PlayerSettings::setPlayerHighConfThr( double d )
{
    dPlayerHighConfThr = d;
    return true;
}

/*! This method returns the confidence threshold below which ball information
    is assumed illegal.
    \return ball confidence threshold */
double PlayerSettings::getBallConfThr( ) const
{
    return dBallConfThr;
}

/*! This method sets the confidence threshold below which ball information is
    assumed illegal
    \param d ball confidence threshold in range [0..1]
    \return boolean indicating whether update was successful */
bool PlayerSettings::setBallConfThr( double d )
{
    dBallConfThr = d;
    return true;
}

/*! This method returns the radius in which a player has to be to be
    mapped from unknown to known player
    \return radius in which player is assumed same player. */
double PlayerSettings::getPlayerDistTolerance( ) const
{
    return dPlayerDistTolerance;
}

/*! This method sets the radius in which a player has to be to be
    mapped from unknown to known player
    \param d radius (>0) in which player is assumed same player
    \return boolean indicating whether update was successful */
bool PlayerSettings::setPlayerDistTolerance( double d )
{
    dPlayerDistTolerance = d;
    return true;
}

/*!This method returns the angle when a player determines to turn to a point
   first before moving towards it.
   \return global angle when player first moves before moving towards point*/
double PlayerSettings::getPlayerWhenToTurnAngle( ) const
{
    return dPlayerWhenToTurnAngle;
}

/*! This method sets the angle when a player determines to turn to a point
    first before moving towards it.
    \param d global angle when player turns in move (interval [0..360]).
    \return boolean indicating whether update was successful */
bool PlayerSettings::setPlayerWhenToTurnAngle( double d )
{
    dPlayerWhenToTurnAngle = d;
    return true;
}

/*! This method returns the percentage of the maximal acceleration in which
    case a kick should still be performed. This value
    is needed to determine whether the ball should be better positioned or
    should be kicked when the ball should be kicked hard.
    If it is possible to accelerate the ball with a higher
    percentage than the returned percentage the kick is performed, in all other
    cases the ball is positioned better.
    \return percentage of ball acceleration when kick should be performed */
double PlayerSettings::getPlayerWhenToKick( ) const
{
    return dPlayerWhenToKick;
}

/*! This method sets the percentage of the maximal acceleration that defines
    in which cases the ball is actually kicked or in which case it is
    positioned better when the ball should be given a very high velocity.
    \param d percentage in range [0..1]
    \return boolean indicating whether update was successful */
bool PlayerSettings::setPlayerWhenToKick( double d )
{
    dPlayerWhenToKick = d;
    return true;
}

/*! This method returns the maximal allowed number of cycles to intercept the
    ball. If it takes more cycles to intercept the ball, the ball is not
    intercepted.
    \return number of intercept cycles */
int PlayerSettings::getPlayerWhenToIntercept( ) const
{
    return iPlayerWhenToIntercept;
}

/*! This methods sets the maximal allowed number of cycles to intercept the
    ball.
    \param i new maximal allowed number of cycles (>0)
    \return boolean indicating whether update was successful */
bool PlayerSettings::setPlayerWhenToIntercept( int i )
{
    iPlayerWhenToIntercept = i;
    return true;
}

/*! This method returns the clear ball distance. When a clear ball is
    performed, the ball is aimed to a point just in front of the penalty area
    of the opponent. This method returns the distance before the penalty area
    to which the ball is aimed.
    \return clear ball distance before opponent penalty area */
double PlayerSettings::getClearBallDist( ) const
{
    return dClearBallDist;
}


/*! This method sets the clear ball distance.
    \param d new clear ball distance before opponent penalty area (>0).
    \return boolean indicating whether update was successful */
bool PlayerSettings::setClearBallDist( double d )
{
    dClearBallDist = d;
    return true;
}

/*! This method returns the distance in which opponents are taken into account
    when a clear ball is issued.
    \return maximum opponent distance for clear ball. */
double PlayerSettings::getClearBallOppMaxDist( ) const
{
    return dClearBallOppMaxDist;
}

/*! This method sets the distance in which opponents are taken into account
    when a clear ball is issued.
    \param d maximum opponent distance for clear ball (>0).
    \return boolean indicating whether update was successful */
bool PlayerSettings::setClearBallOppMaxDist( double d )
{
    dClearBallOppMaxDist = d;
    return true;
}

/*! This method returns the minimum needed angle for a clear ball to the side.
    \return minimum needed angle for clear ball to side */
double PlayerSettings::getClearBallToSideAngle( ) const
{
    return dClearBallToSideAngle;
}

/*! This method sets the minimum needed angle for a clear ball to the side.
    \param d minimum needed angle (>0) for clear ball to side
    \return boolean indicating whether update was successful */
bool PlayerSettings::setClearBallToSideAngle( double d )
{
    dClearBallToSideAngle = d;
    return true;
}

/*! This method returns the cone width that is used to check for opponents when
    passing to a player. A pass is only performed when no opponents are in the
    cone. The cone is specified as the width to one side
    after distance 1. So for a value of 0.5 the cone angle equals
    45 (22.5 to both sides).
    \return cone width in which no opponents are allowed when passing */
double PlayerSettings::getConeWidth( ) const
{
    return dConeWidth;
}

/*! This method sets the cone width in which no opponents are allowed when the
    ball is passed to a teammate. The cone width is specified as the width to
    one side after distance 1. So for a value of 0.5 the cone angle
    equals 45 (22.5 to both sides).
    \param d cone width in which no opponents are allowed when passing (>0)
    \return boolean indicating whether update was successful */
bool PlayerSettings::setConeWidth( double d )
{
    dConeWidth = d;
    return true;
}

/*! This method returns the desired end speed for the ball when a normal pass
    is performed.
   \return desired end speed for ball */
double PlayerSettings::getPassEndSpeed( ) const
{
    return dPassEndSpeed;
}

/*! This method sets the desired end speed for the ball when a normal pass is
    performed.
    \param d desired end speed for ball (>0)
    \return boolean indicating whether update was successful */
bool PlayerSettings::setPassEndSpeed( double d )
{
    dPassEndSpeed = d;
    return true;
}

/*! This method returns the desired end speed for the ball when a fast pass is
    performed.
   \return desired end speed for ball when it is passed fast */
double PlayerSettings::getFastPassEndSpeed( ) const
{
    return dFastPassEndSpeed;
}


/*! This method sets the desired end speed for the ball when a fast pass is
    performed.
    \param d desired end speed for passing ball fast (>0)
    \return boolean indicating whether update was successful */
bool PlayerSettings::setFastPassEndSpeed( double d )
{
    dFastPassEndSpeed = d;
    return true;
}

/*! This method returns the x value that can be added to the position of a
    teammate when a leading pass to this teammate is performed.
    \return x value added to teammate to which is passed */
double PlayerSettings::getPassExtraX( ) const
{
    return dPassExtraX;
}

/*! This method sets the x value that is added to the position of a teammate
    when a leading pass is performed.
    \param d x value added to teammate to which is passed
    \return boolean indicating whether update was successful */
bool PlayerSettings::setPassExtraX( double d )
{
    dPassExtraX = d;
    return true;
}

/*! This method returns the fraction of the simulation step that is waited
    before an action is sent to the server in case no see message will arrive
    in this cycle.
    \return fraction of simulation step that is waited when no see arrives */
double PlayerSettings::getFractionWaitNoSee( ) const
{
    return dFractionWaitNoSee;
}

/*! This method sets the fraction of the simulation step that is waited before
    an action is sent to the server in case no see message will arrive in this
    cycle.
    \param d fraction of simulation step that is waited when no see arrives
    (in range [0..1])
    \return boolean indicating whether update was successful */
bool PlayerSettings::setFractionWaitNoSee( double d )
{
    dFractionWaitNoSee = d;
    return true;
}

/*! This method returns the fraction of the simulation step that is waited
    before an action is sent to the server in case the see message will arrive
    in the first half of the cycle.
    \return fraction of simulation step that is waited when see arrives in
            first half of the cycle */
double PlayerSettings::getFractionWaitSeeBegin( ) const
{
    return dFractionWaitSeeBegin;
}

/*! This method sets the fraction of the simulation step that is waited before
    an action is sent to the server in case the see message will arrive in the
    first part of the cycle.
    \param d fraction of simulation step that is waited when see arrives in the
    first half of the cycle (in range [0..1])
    \return boolean indicating whether update was successful */
bool PlayerSettings::setFractionWaitSeeBegin( double d )
{
    dFractionWaitSeeBegin = d;
    return true;
}

/*! This method returns the fraction of the simulation step that is waited
    before an action is sent to the server in case the see message will arrive
    in the second half of the cycle.
    \return fraction of simulation step that is waited when see arrives in
            second half of the cycle */
double PlayerSettings::getFractionWaitSeeEnd( ) const
{
    return dFractionWaitSeeEnd;
}

/*! This method sets the fraction of the simulation step that is waited before
    an action is sent to the server in case the see message will arrive in the
    second part of the cycle.
    \param d fraction of simulation step that is waited when see arrives in the
    second half of the cycle (in range [0..1])
    \return boolean indicating whether update was successful */
bool PlayerSettings::setFractionWaitSeeEnd( double d )
{
    dFractionWaitSeeEnd = d;
    return true;
}

/*! This method returns the desired distance to an opponent which should be
    marked.
    \return desired marking distance to an opponent */
double PlayerSettings::getMarkDistance( ) const
{
    return dMarkDistance;
}

/*! This method sets the desired distance to an opponent which should be
    marked.
    \param d desired marking distance to an opponent
    \return boolean indicating whether update was successful */
bool PlayerSettings::setMarkDistance( double d )
{
    dMarkDistance = d;
    return true;
}

/*! This method returns the radius around the strategic position in which
    an optimal position is considered.
    \return radius around strategic position in which an optimal position is
    considered */
double PlayerSettings::getStratAreaRadius( ) const
{
    return dStratAreaRadius;
}

/*! This method sets the radius around the strategic position in which
    an optimal position is considered.
    \param d radius around strategic position in which an optimal position is
           considered (>0)
    \return boolean indicating whether update was successful */
bool PlayerSettings::setStratAreaRadius( double d )
{
    dStratAreaRadius = d;
    return true;
}

/*! This method returns the minimum needed probability for the ball to enter
    the goal when a "risky" scoring attempt is performed. That is when an agent
    tries to score although the success rate is not very high, he will always
    shoot to a point in the goal where the probability that the ball will enter
    the goal is higher than the value returned by this method.
    \return minimum needed probability that the ball will enter the goal */
double PlayerSettings::getShootRiskProbability( ) const
{
    return dShootRiskProbability;
}


/*! This method sets the minimum needed probability for the ball to enter
    the goal when a "risky" scoring attempt is performed. That is when an agent
    tries to score although the success rate is not very high, he will always
    shoot to a point in the goal where the probability that the ball will enter
    the goal is higher than this value.
    \param d minimum needed probability that the ball enters the goal [0..1]
    \return boolean indicating whether update was successful */
bool PlayerSettings::setShootRiskProbability( double d )
{
    dShootRiskProbability = d;
    return true;
}

/*! This method returns the number of cycles waited by the goalkeeper after he
    has catched the ball.
    \return number of cycles goalkeeper waits after catch */
int PlayerSettings::getCyclesCatchWait( ) const
{
    return iCyclesCatchWait;
}

/*! This method sets the number of cycles waited by the goalkeeper after he
    has catched the ball.
    \param i number of cycles goalkeeper waits after catch
    \return boolean indicating whether update was successful */
bool PlayerSettings::setCyclesCatchWait( int i )
{
    iCyclesCatchWait = i;
    return true;
}

/*! This method returns the number of seconds before the server is assumed
    dead. When no message is received from the server for this amount of
    seconds, it is assumed that the server program has been closed and the
    agent will exits.
    \return number of seconds before server is assumed dead*/
int PlayerSettings::getServerTimeOut( ) const
{
    return iServerTimeOut;
}

/*! This method sets the number of seconds before the server is assumed dead.
    When no message is received from the server for this amount of seconds, it
    is assumed that the server is stopped and the agent exits.
    \param i number of seconds before server is assumed dead
    \return bool indicating whether update was succesfull. */
bool PlayerSettings::setServerTimeOut( int i )
{
    iServerTimeOut = i;
    return true;
}

/*! This method returns the threshold to determine when the agent turns towards
    the dribble direction when dribbling. When the global angle
    difference is larger than this value, the agent performs a turnWithBallTo.
   \return threshold value for angle */
double PlayerSettings::getDribbleAngThr( ) const
{
    return dDribbleAngThr;
}

/*! This method sets the threshold to determine when the agent turns towards
    the dribble direction when dribbling. When the global angle
    difference is larger than this value, the agent performs a turnWithBallTo.
    \param d threshold value for angle
    \return bool indicating whether update was succesfull. */
bool PlayerSettings::setDribbleAngThr( double d )
{
    dDribbleAngThr = d;
    return true;
}


/*! This method returns the threshold to determine when the ball is kicked to
    another position in the turnWithBallTo skill. When the global angle
    difference is larger than this value, the ball is repositioned otherwise
    it is not.
   \return threshold value for angle */
double PlayerSettings::getTurnWithBallAngThr( ) const
{
    return dTurnWithBallAngThr;
}

/*! This method sets the threshold to determine when the ball is kicked to
    another position in the turnWithBallTo skill. When the global angle
    difference is larger than this value, the ball is repositioned otherwise
    it is not.
    \param d threshold value for angle
    \return bool indicating whether update was succesfull. */
bool PlayerSettings::setTurnWithBallAngThr( double d )
{
    dTurnWithBallAngThr = d;
    return true;
}

/*! This method returns the threshold to determine when the ball is frozen
    in the turnWithBallTo skill. When the ball speed is larger than this value,
    the ball is frozen otherwise not
    \return threshold value for freezing the ball */
double PlayerSettings::getTurnWithBallFreezeThr( ) const
{
    return dTurnWithBallFreezeThr;
}

/*! This method sets the threshold to determine when the ball is frozen
    in the turnWithBallTo skill. When the ball speed is larger than this value,
    the ball is frozen otherwise not
    \param d threshold value for freezing the ball
    \return bool indicating whether update was succesfull. */
bool PlayerSettings::setTurnWithBallFreezeThr( double d )
{
    dTurnWithBallFreezeThr = d;
    return true;
}

/*! This method returns the initial formation of the team.
    \return number of cycles goalkeeper waits after catch */
int PlayerSettings::getInitialFormation( ) const
{
    return iInitialFormation;
}

/*! This method sets the initial formation of the team.
    \param i initital formation of the team
    \return boolean indicating whether update was successful */
bool PlayerSettings::setInitialFormation( int i )
{
    iInitialFormation = i;
    return true;
}

/*! This method returns the percentage of the field width. The corresponding y
    position is the maximum allowed y position for a player's strategic
    position.
    \return maximum y percentage on the field.  */
double PlayerSettings::getMaxYPercentage( ) const
{
    return dMaxYPercentage;
}

/*! This method sets the percentage of the field width. The corresponding y
    position is the maximum allowed y position for a player's strategic
    position.
    \param d percentage of the field width
    \return bool indicating whether update was succesfull. */
bool PlayerSettings::setMaxYPercentage( double d )
{
    dMaxYPercentage = d;
    return true;
}
