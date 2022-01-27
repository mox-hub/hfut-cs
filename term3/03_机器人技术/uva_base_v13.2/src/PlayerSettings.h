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

/*! \file PlayerSettings.h
<pre>
<b>File:</b>          PlayerSettings.h
<b>Project:</b>       Robocup Soccer Simulation Team: UvA Trilearn
<b>Authors:</b>       Jelle Kok
<b>Created:</b>       10/12/2000
<b>Last Revision:</b> $ID$
<b>Contents:</b>      This file contains the class definitions for the
               PlayerSettings class that contains the specific
               (threshold) settings which determines the situation in
               which certain actions are performed by the client.
<hr size=2>
<h2><b>Changes</b></h2>
<b>Date</b>             <b>Author</b>          <b>Comment</b>
10/12/2000       Jelle Kok       Initial version created
</pre>
*/

#ifndef _PlayerSettings_
#define _PlayerSettings_

#include "GenericValues.h"

/*****************************************************************************/
/********************* CLASS PlayerSettings **********************************/
/*****************************************************************************/

/*! This class contains all the settings that are important for the client
   (agent) to determine its actions. It contains mostly threshold values to
   determine whether a certain kind of actions should be taken or not.
   Furthermore this class contains all the standard set- and get methods for
   manipulating these values. Although it is normally not the case that these
   values are changed at runtime. The PlayerSettings class is a subclass of the
   GenericValues class and therefore each value in this class can be reached
   through the string name of the corresponding parameter. This may be helpful
   when the parameters are taken from a configuration file. */
class PlayerSettings : public GenericValues
{
    double dPlayerConfThr;         /*!< confidence threshold below which player
                                      information is assumed illegal         */
    double dPlayerHighConfThr;     /*!< confidence threshold above which player
                                      information is assumed very good       */
    double dBallConfThr;           /*!< confidence threshold below which ball
                                      information is assumed illegal         */
    double dPlayerDistTolerance;   /*!< radius in which player has to be to be
                                      mapped from unknown to known player    */
    double dPlayerWhenToTurnAngle; /*!< angle when to turn to ball when moving */
    double dPlayerWhenToKick;      /*!< percentage of kick power rate when kick
                                      is performed                           */
    int    iPlayerWhenToIntercept; /*!< how many cycles to ball when intercept */
    double dClearBallDist;         /*!< distance before the penalty area to
                                      clear the ball to                      */
    double dClearBallOppMaxDist;   /*!< radius in which opponent has to be to
                                       be taken into account for clear ball  */
    double dClearBallToSideAngle;  /*!< minimum angle between opponents before
                                      clear ball is taken into account */
    double dConeWidth;             /*!< Cone width (at distance 1) to check for
                                      opponents when passing to player.      */
    double dPassEndSpeed;          /*!< end speed ball when passed to teammate */
    double dFastPassEndSpeed;      /*!< end speed ball when passed fast        */
    double dPassExtraX;            /*!< extra x value added to x coordinate of
                                      player pos to which is passed          */
    double dFractionWaitNoSee;     /*!< % of simulation step that is waited
                                      before action is sent in case no
                                      see message arrives                    */
    double dFractionWaitSeeBegin;  /*!< % of simulation step that is waited
                                      before action is sent in case see message
                                      arrives in first half cycle            */
    double dFractionWaitSeeEnd;    /*!< % of simulation step that is waited
                                      before action is sent in case see message
                                      arrives in second half cycle           */
    double dMarkDistance;          /*!< This is the distance the agent marks
                                      an opponent.                           */
    double dStratAreaRadius;       /*!< This is the radius around the strategic
                                      position in which an optimal position
                                      is determined.                         */
    double dShootRiskProbability;  /*!< This is the probability for the ball to
                                      enter the goal that is used when scoring
                                      with risk                              */
    int    iCyclesCatchWait;       /*!< Cycles the coach waits after a catch has
                                      been performed before he shoots.       */
    int    iServerTimeOut;         /*!< Number of seconds before the soccer
                                      server is assumed dead.                */
    double dDribbleAngThr;         /*!< Threshold value for angle difference
                                      that indicates when the agent turns
                                      towards the dribble direction.         */
    double dTurnWithBallAngThr;    /*!< Threshold value for angle difference
                                      that indicates when the ball is kicked in
                                      turnWithBallTo skill.                  */
    double dTurnWithBallFreezeThr; /*!< Threshold value for ball speed that
                                      indicates when the ball is frozen in
                                      turnWithBallTo skill.                  */
    int    iInitialFormation;      /*!< Initial formation for the team.        */
    double dMaxYPercentage;        /*!< Maximum y percentage of the field width
                                      for the y position in a strategic
                                      position. */

public:
    PlayerSettings( );

    // all standard get and set methods

    double getPlayerConfThr        (          ) const;
    bool   setPlayerConfThr        ( double d );

    double getPlayerHighConfThr    (          ) const;
    bool   setPlayerHighConfThr    ( double d );

    double getBallConfThr          (          ) const;
    bool   setBallConfThr          ( double d );

    double getPlayerDistTolerance  (          ) const;
    bool   setPlayerDistTolerance  ( double d );

    double getPlayerWhenToTurnAngle(          ) const;
    bool   setPlayerWhenToTurnAngle( double d );

    double getPlayerWhenToKick     (          ) const;
    bool   setPlayerWhenToKick     ( double d );

    int    getPlayerWhenToIntercept(          ) const;
    bool   setPlayerWhenToIntercept( int i    );

    double getClearBallDist        (          ) const;
    bool   setClearBallDist        ( double d );

    double getClearBallOppMaxDist  (          ) const;
    bool   setClearBallOppMaxDist  ( double d );

    double getClearBallToSideAngle (          ) const;
    bool   setClearBallToSideAngle ( double d );

    double getConeWidth            (          ) const;
    bool   setConeWidth            ( double d );

    double getPassEndSpeed         (          ) const;
    bool   setPassEndSpeed         ( double d );

    double getFastPassEndSpeed     (          ) const;
    bool   setFastPassEndSpeed     ( double d );

    double getPassExtraX           (          ) const;
    bool   setPassExtraX           ( double d );

    double getFractionWaitNoSee    (          ) const;
    bool   setFractionWaitNoSee    ( double d );

    double getFractionWaitSeeBegin (          ) const;
    bool   setFractionWaitSeeBegin ( double d );

    double getFractionWaitSeeEnd   (          ) const;
    bool   setFractionWaitSeeEnd   ( double d );

    double getMarkDistance         (          ) const;
    bool   setMarkDistance         ( double d );

    double getStratAreaRadius      (          ) const;
    bool   setStratAreaRadius      ( double d );

    double getShootRiskProbability (          ) const;
    bool   setShootRiskProbability ( double d );

    int    getCyclesCatchWait      (          ) const;
    bool   setCyclesCatchWait      ( int i    );

    int    getServerTimeOut        (          ) const;
    bool   setServerTimeOut        ( int i    );

    double getDribbleAngThr        (          ) const;
    bool   setDribbleAngThr        ( double d );

    double getTurnWithBallAngThr   (          ) const;
    bool   setTurnWithBallAngThr   ( double d );

    double getTurnWithBallFreezeThr(          ) const;
    bool   setTurnWithBallFreezeThr( double d );

    int    getInitialFormation     (          ) const;
    bool   setInitialFormation     ( int i    );

    double getMaxYPercentage       (          ) const;
    bool   setMaxYPercentage       ( double d );


};

#endif
