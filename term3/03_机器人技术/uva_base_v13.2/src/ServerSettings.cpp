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

/*! \file ServerSettings.cpp
<pre>
<b>File:</b>          ServerSettings.cpp
<b>Project:</b>       Robocup Soccer Simulation Team: UvA Trilearn
<b>Authors:</b>       Jelle Kok
<b>Created:</b>       28/11/2000
<b>Last Revision:</b> $ID$
<b>Contents:</b>      Code file for class ServerSettings. It contains all the
               member method implementations of the ServerSettings class. This
               class contains all the Soccerserver parameters that are
               available in the configuration files 'server.conf' and
               'player.conf' along with their default values and
               standard set- and get methods for manipulating these
               values.</pre>

<hr size=2>
<pre>
<h2><b>Changes</b></h2>
<b>Date</b>             <b>Author</b>          <b>Comment</b>
28/11/2000       Jelle Kok       Initial version created (based on Emiel Corten)
31/01/2001       Remco de Boer   Settings for server version 7.xx added
27/04/2001       Remco de Boer   drop_ball_time and player.conf parameters added
03/05/2001       Remco de Boer   Version including full documentation completed
</pre>
*/

#include "ServerSettings.h"
#include <stdio.h>
#include <string.h>           // needed for 'strcpy'

/*****************************************************************************/
/*******************   CLASS SERVERSETTINGS   ********************************/
/*****************************************************************************/

/*! Constructor for the ServerSettings class. It sets all the private member
    variables in this class to the values specified in the configuration files
    (server.conf and player.conf) of Soccer Server version 8.xx. These values
    can be changed by calling the method 'readValues' with a new configuration
    file or by calling the method 'setValue' for a specific variable. */
ServerSettings::ServerSettings( ):GenericValues( "ServerSettings", 121 )
{
    // goal-related parameters
    dGoalWidth         = 14.02; // goal_width: the width of the goal

    // player-related parameters
    dPlayerSize        = 0.3;   // player_size: the size (=radius) of a player
    dPlayerDecay       = 0.4;   // player_decay: player speed decay per cycle
    dPlayerRand        = 0.1;   // player_rand: random error in player movement
    dPlayerWeight      = 60.0;  // player_weight: weight of a player (for wind)
    dPlayerSpeedMax    = 1.2;   // player_speed_max: maximum speed of a player
    dPlayerAccelMax    = 1.0;   // player_accel_max: maximum acceleration of a
    // player per cycle

    // stamina-related parameters
    dStaminaMax        = 4000.0;// stamina_max: maximum stamina of a player
    dStaminaIncMax     = 45.0;  // stamina_inc_max: maximum stamina increase of a
    // player per cycle
    dRecoverDecThr     = 0.3;   // recover_dec_thr: percentage of stamina_max
    // below which player recovery decreases
    dRecoverDec        = 0.002; // recover_dec: decrement step per cycle for
    // player recovery
    dRecoverMin        = 0.5;   // recover_min: minimum player recovery
    dEffortDecThr      = 0.3;   // effort_dec_thr: % of stamina_max below
    // which player effort capacity decreases
    dEffortDec         = 0.005; // effort_dec: decrement step per cycle for
    // player effort capacity
    dEffortIncThr      = 0.6;   // effort_incr_thr: percentage of stamina_max
    // above which player effort capacity increases
    dEffortInc         = 0.01;  // effort_inc: increment step per cycle for
    // player effort capacity
    dEffortMin         = 0.6;   // effort_min: minimum value for player effort

    // parameters related to auditory perception
    iHearMax           = 2;     // hear_max: max hearing capacity of a player;
    // a player can hear hear_inc messages in
    // hear_decay simulation cycles
    iHearInc           = 1;     // hear_inc: min hearing capacity of a player,
    // i.e. the number of messages a player can hear
    // in hear_decay simulation cycles
    iHearDecay         = 2;     // hear_decay: decay rate of player hearing
    // capacity, i.e. minimum number of cycles for
    // hear_inc messages

    // parameters related to player turn actions
    dInertiaMoment     = 5.0;   // inertia_moment: inertia moment of a player;
    // affects actual turn angle depending on speed

    // parameters related to sense_body information
    iSenseBodyStep     = 100;   // sense_body_step: length of the interval (ms)
    // between sense_body information messages

    // goalkeeper-related parameters
    dCatchableAreaL    = 2.0;   // catchable_area_l: length of area around
    // goalkeeper in which he can catch the ball
    dCatchableAreaW    = 1.0;   // catchable_area_w: width of area around
    // goalkeeper in which he can catch the ball
    dCatchProbability  = 1.0;   // catch_probability: the probability for a
    // goalkeeper to catch the ball
    iCatchBanCycle     = 5 ;    // catch_ban_cycle: number of cycles after catch
    // in which goalkeeper cannot catch again
    iGoalieMaxMoves    = 2;     // goalie_max_moves: maximum number of 'move'
    // actions allowed for goalkeeper after catch

    // ball-related parameters
    dBallSize          = 0.085; // ball_size: the size (=radius) of the ball
    dBallDecay         = 0.94;  // ball_decay: ball speed decay per cycle
    dBallRand          = 0.05;  // ball_rand: random error in ball movement
    dBallWeight        = 0.2;   // ball_weight: weight of the ball (for wind)
    dBallSpeedMax      = 2.7;   // ball_speed_max: maximum speed of the ball
    dBallAccelMax      = 2.7;   // ball_accel_max: maximum acceleration of the
    // ball per cycle

    // wind-related parameters
    dWindForce         = 0.0;   // wind_force: the force of the wind
    dWindDir           = 0.0;   // wind_dir: the direction of the wind
    dWindRand          = 0.0;   // wind_rand: random error in wind direction
    bWindRandom        = false; // wind_random: is wind force and dir random

    // parameters related to 'dash' and 'kick' commands
    dKickableMargin    = 0.7;   // kickable_margin: margin around player in which
    // ball is kickable; kickable area thus equals
    // kickable_margin + ball_size + player_size
    dCkickMargin       = 1.0;   // ckick_margin: corner kick margin, i.e. the
    // minimum distance to the ball for offending
    // players when a corner kick is taken

    dDashPowerRate     = 0.006; // dash_power_rate: rate by which the 'Power'
    // argument in a 'dash' command is multiplied
    // (thus determining the amount of displacement
    // of the player as a result of the 'dash')
    dKickPowerRate     = 0.027; // kick_power_rate: rate by which the 'Power'
    // argument in a 'kick' command is multiplied
    // (thus determining the amount of displacement
    // of the ball as a result of the 'kick')
    dKickRand          = 0.0;   // kick_rand: random error in kick direction

    // parameters related to visual and auditory perception range
    dVisibleAngle      = 90.0;  // visible_angle: angle of the view cone of a
    // player in the standard view mode
    dAudioCutDist      = 50.0;  // audio_cut_dist: maximum distance over which a
    // spoken message can be heard

    // quantization parameters
    dQuantizeStep      = 0.1;   // quantize_step: quantization step for distance
    // of moving objects
    dQuantizeStepL     = 0.01;  // quantize_step_l: quantization step for dist
    // of landmarks

    // range parameters for basic actuator commands
    iMaxPower          = 100;   // maxpower: maximum power for dash/kick
    iMinPower          = -100;  // minpower: minimum power for dash/kick
    iMaxMoment         = 180;   // maxmoment: maximum angle for turn/kick
    iMinMoment         = -180;  // minmoment: minimum angle for turn/kick
    iMaxNeckMoment     = 180;   // maxneckmoment: maximum angle for turnneck
    iMinNeckMoment     = -180;  // minneckmoment: minimum angle for turnneck
    iMaxNeckAng        = 90;    // maxneckang: maximum neck angle rel to body
    iMinNeckAng        = -90;   // minneckang: minimum neck angle rel to body

    // port-related parameters
    iPort              = 6000;  // port: port number for player connection
    iCoachPort         = 6001;  // coach_port: port number for coach connection
    iOlCoachPort       = 6002;  // ol_coach_port: port number for online coach

    // coach-related parameters
    iSayCoachCntMax    = 128;   // say_coach_cnt_max: maximum number of coach
    // messages possible
    iSayCoachMsgSize   = 128;   // say_coach_msg_size: maximum size of coach
    // messages
    iClangWinSize      = 300;   // clang_win_size: time window which controls how
    // many coach messages can be sent
    iClangDefineWin    = 1;     // clang_define_win: number of define messages by
    // coach per time window
    iClangMetaWin      = 1;     // clang_meta_win: number of meta messages by
    // coach per time window
    iClangAdviceWin    = 1;     // clang_advice_win: number of advice messages by
    // coach per time window
    iClangInfoWin      = 1;     // clang_info_win: number of info messages by
    // coach per time window
    iClangMessDelay    = 50;    // clang_mess_delay: delay of coach messages, ie
    // the number of cycles between send to player
    // and receival of message
    iClangMessPerCycle = 1;     // clang_mess_per_cycle: number of coach messages
    // per cycle
    iSendViStep        = 100;   // send_vi_step: interval of coach's look, i.e.
    // the length of the interval (in ms) between
    // visual messages to the coach

    // time-related parameters
    iSimulatorStep     = 100;   // simulator_step: the length (in ms) of a
    // simulator cycle
    iSendStep          = 150;   // send_step: the length of the interval (in ms)
    // between visual messages to a player in the
    // standard view mode
    iRecvStep          = 10;    // recv_step: the length of the interval (in ms)
    // for accepting commands from a player
    iHalfTime          = 300;   // half_time: the length (in seconds) of a single
    // game half
    iDropBallTime      = 200;   // drop_ball_time: the number of cycles to wait
    // until dropping the ball automatically for free
    // kicks, corner kicks, etc.

    // speech-related parameters
    iSayMsgSize        = 512;   // say_msg_size: the maximum length (in bytes) of
    // a spoken message

    // offside-related parameters
    bUseOffside            = true; // use_offside: a boolean flag indicating
    // whether the offside rule should be applied
    // or not
    dOffsideActiveAreaSize = 5.0;  // offside_active_area_size: offside active
    // area size, i.e. radius of circle around
    // the ball in which player can be offside
    bForbidKickOffOffside  = true; // forbid_kick_off_offside: a boolean flag
    // indicating whether a kick from offside
    // position is allowed
    dOffsideKickMargin     = 9.15; // offside_kick_margin: offside kick margin
    // i.e. the minimum distance to the ball for
    // offending players when a free kick for
    // offside is taken

    // log-related parameters
    bVerbose           = false; // verbose: flag indicating whether verbose mode
    // is active or not; in verbose mode server sends
    // extra error-information
    iRecordVersion     = 3;     // record_version: the type of log record
    bRecordLog         = true;  // record_log: flag indicating whether log record
    // for game should be created
    bSendLog           = true;  // send_log: flag indicating whether send client
    // command log for game should be created
    bLogTimes          = false; // log_times: flag indicating whether ms should
    // be written between cycles in log file
    strcpy( strLogFile, "server.log" );// server log to store all actions receiv
    bSynchMode         = false; // synch_mode: in synchronization mode?
    bFullStateL        = false; // fullstate_l, full information left team
    bFullStateR        = false; // fulstate_r, full information right team

    // heterogeneous player parameters from player.conf
    iPlayerTypes              = 7;     // player_types: the number of player type
    // including the default player type
    iSubsMax                  = 3;     // subs_max: the maximum number of
    // substitutions allowed during a game the
    // value also indicates the maximum number
    // of players allowed for each type
    dPlayerSpeedMaxDeltaMin   = 0.0;   // player_speed_max_delta_min: minimum
    // delta for adjusting player_speed_max
    dPlayerSpeedMaxDeltaMax   = 0.0;   // player_speed_max_delta_max: maximum
    // delta for adjusting player_speed_max
    dStaminaIncMaxDeltaFactor = 0.0;   // stamina_inc_max_delta_factor: amount by
    // which delta is multiplied for
    // stamina_inc_max
    dPlayerDecayDeltaMin      = 0.0;   // player_decay_delta_min: minimum delta
    // for adjusting player_decay
    dPlayerDecayDeltaMax      = 0.2;   // player_decay_delta_max: maximum delta
    // for adjusting player_decay
    dInertiaMomentDeltaFactor = 25.0;  // inertia_moment_delta_factor: amount by
    // which delta is multiplied for
    // inertia_moment
    dDashPowerRateDeltaMin    = 0.0;   // dash_power_rate_delta_min: min delta
    // for adjusting dash_power_rate
    dDashPowerRateDeltaMax    = 0.0;   // dash_power_rate_delta_max: max delta
    // for adjusting dash_power_rate
    dPlayerSizeDeltaFactor    = -100.0;// player_size_delta_factor: amount delta
    // is multiplied by for player_size
    dKickableMarginDeltaMin   = 0.0;   // kickable_margin_delta_min: min delta
    // for adjusting kickable_margin
    dKickableMarginDeltaMax   = 0.2;   // kickable_margin_delta_max: max delta
    // for adjusting kickable_margin
    dKickRandDeltaFactor      = 0.5;   // kick_rand_delta_factor: amount delta is
    // multiplied by for kick_rand
    dExtraStaminaDeltaMin     = 0.0;   // extra_stamina_delta_min: minimum delta
    // for adjusting extra_stamina
    dExtraStaminaDeltaMax     = 100.0; // extra_stamina_delta_max: maximum delta
    // for adjusting extra_stamina
    dEffortMaxDeltaFactor     = -0.002;// effort_max_delta_factor: amount delta
    // is multiplied by for effort_max
    dEffortMinDeltaFactor     = -0.002;// effort_min_delta_factor: amount delta
    // is multiplied by for effort_min
    dNewDashPowerRateDeltaMin = 0.0;   // dash_power_rate_delta_min: minimum
    // delta for adjusting dash_power_rate
    dNewDashPowerRateDeltaMax = 0.002; // dash_power_rate_delta_max: maximum
    // delta for adjusting dash_power_rate
    dNewStaminaIncMaxDeltaFactor=-100000.0;// stamina_inc_max_delta_factor:
    // amount by which delta is multiplied for
    // stamina_inc_max

    // important server parameters not in server.conf or player.conf
    dEffortMax         = 1.0; // effort_max: maximum player effort capacity
    iSlowDownFactor    = 1;   // slow_down_factor: factor to slow down simulator
    // and send intervals
    dVisibleDistance   = 3.0; // visible_distance: distance within which objects
    // are always 'visible' even when not in view cone)
    dExtraStamina      = 0.0; // extra_stamina: extra stamina for heterogeneous
    // player

    // penalty parameters
    dPenDistX          = 11.0;         // pen_dist_x: distance ball from goal
    dPenMaxGoalieDistX = 4.0;          // pen_goalie_max_dist_x: maximum distance
    // for goalie during penalties
    bPenAllowMultKicks = false;        // pen_allow_mult_kicks: can penalty
    // kicker kick multiple times

    // tackle parameters
    dTackleDist        = 2.0;          // tackle_dist: allowed distance in front
    dTackleBackDist    = 0.5;          // tackle_back_dist: allowed distance back
    dTackleWidth       = 1.0;          // tackle_width: allowed distance side
    dTackleExponent    = 6;            // tackle_exponent: exponent need
    iTackleCycles      = 10;           // tackle_cycles: cycles immobile
    dTacklePowerRate   = 0.027;        // tackle_power_rate: power rate tackle

    // parameters which depend on other values
    dMaximalKickDist = dKickableMargin +   // the maximum distance from a player
                       dPlayerSize +       // for which the ball is kickable
                       dBallSize;

    // add all the settings, i.e. each parameter
    // becomes a new generic value for the class

    // goal-related parameters
    addSetting( "goal_width"        , &dGoalWidth       , GENERIC_VALUE_DOUBLE );

    // player-related parameters
    addSetting( "player_size"       , &dPlayerSize      , GENERIC_VALUE_DOUBLE );
    addSetting( "player_decay"      , &dPlayerDecay     , GENERIC_VALUE_DOUBLE );
    addSetting( "player_rand"       , &dPlayerRand      , GENERIC_VALUE_DOUBLE );
    addSetting( "player_weight"     , &dPlayerWeight    , GENERIC_VALUE_DOUBLE );
    addSetting( "player_speed_max"  , &dPlayerSpeedMax  , GENERIC_VALUE_DOUBLE );
    addSetting( "player_accel_max"  , &dPlayerAccelMax  , GENERIC_VALUE_DOUBLE );

    // stamina-related parameters
    addSetting( "stamina_max"       , &dStaminaMax      , GENERIC_VALUE_DOUBLE );
    addSetting( "stamina_inc_max"   , &dStaminaIncMax   , GENERIC_VALUE_DOUBLE );
    addSetting( "recover_dec_thr"   , &dRecoverDecThr   , GENERIC_VALUE_DOUBLE );
    addSetting( "recover_dec"       , &dRecoverDec      , GENERIC_VALUE_DOUBLE );
    addSetting( "recover_min"       , &dRecoverMin      , GENERIC_VALUE_DOUBLE );
    addSetting( "effort_dec_thr"    , &dEffortDecThr    , GENERIC_VALUE_DOUBLE );
    addSetting( "effort_dec"        , &dEffortDec       , GENERIC_VALUE_DOUBLE );
    addSetting( "effort_inc_thr"    , &dEffortIncThr    , GENERIC_VALUE_DOUBLE );
    addSetting( "effort_inc"        , &dEffortInc       , GENERIC_VALUE_DOUBLE );
    addSetting( "effort_min"        , &dEffortMin       , GENERIC_VALUE_DOUBLE );

    // parameters related to auditory perception
    addSetting( "hear_max"          , &iHearMax         , GENERIC_VALUE_INTEGER);
    addSetting( "hear_inc"          , &iHearInc         , GENERIC_VALUE_INTEGER);
    addSetting( "hear_decay"        , &iHearDecay       , GENERIC_VALUE_INTEGER);

    // parameters related to player turn actions
    addSetting( "inertia_moment"    , &dInertiaMoment   , GENERIC_VALUE_DOUBLE );

    // parameters related to sense_body information
    addSetting( "sense_body_step"   , &iSenseBodyStep   , GENERIC_VALUE_INTEGER);

    // goalkeeper-related parameters
    addSetting( "catchable_area_l"  , &dCatchableAreaL  , GENERIC_VALUE_DOUBLE );
    addSetting( "catchable_area_w"  , &dCatchableAreaW  , GENERIC_VALUE_DOUBLE );
    addSetting( "catch_probability" , &dCatchProbability, GENERIC_VALUE_DOUBLE );
    addSetting( "catch_ban_cycle"   , &iCatchBanCycle   , GENERIC_VALUE_INTEGER);
    addSetting( "goalie_max_moves"  , &iGoalieMaxMoves  , GENERIC_VALUE_INTEGER);

    // ball-related parameters
    addSetting( "ball_size"         , &dBallSize        , GENERIC_VALUE_DOUBLE );
    addSetting( "ball_decay"        , &dBallDecay       , GENERIC_VALUE_DOUBLE );
    addSetting( "ball_rand"         , &dBallRand        , GENERIC_VALUE_DOUBLE );
    addSetting( "ball_weight"       , &dBallWeight      , GENERIC_VALUE_DOUBLE );
    addSetting( "ball_speed_max"    , &dBallSpeedMax    , GENERIC_VALUE_DOUBLE );
    addSetting( "ball_accel_max"    , &dBallAccelMax    , GENERIC_VALUE_DOUBLE );

    // wind-related parameters
    addSetting( "wind_force"        , &dWindForce       , GENERIC_VALUE_DOUBLE );
    addSetting( "wind_dir"          , &dWindDir         , GENERIC_VALUE_DOUBLE );
    addSetting( "wind_rand"         , &dWindRand        , GENERIC_VALUE_DOUBLE );
    addSetting( "wind_random"       , &bWindRandom      , GENERIC_VALUE_BOOLEAN);
    // parameters related to 'dash' and 'kick' commands
    addSetting( "kickable_margin"   , &dKickableMargin  , GENERIC_VALUE_DOUBLE );
    addSetting( "ckick_margin"      , &dCkickMargin     , GENERIC_VALUE_DOUBLE );
    addSetting( "dash_power_rate"   , &dDashPowerRate   , GENERIC_VALUE_DOUBLE );
    addSetting( "kick_power_rate"   , &dKickPowerRate   , GENERIC_VALUE_DOUBLE );
    addSetting( "kick_rand"         , &dKickRand        , GENERIC_VALUE_DOUBLE );

    // parameters related to visual and auditory perception range
    addSetting( "visible_angle"     , &dVisibleAngle    , GENERIC_VALUE_DOUBLE );
    addSetting( "audio_cut_dist"    , &dAudioCutDist    , GENERIC_VALUE_DOUBLE );

    // quantization parameters
    addSetting( "quantize_step"     , &dQuantizeStep    , GENERIC_VALUE_DOUBLE );
    addSetting( "quantize_step_l"   , &dQuantizeStepL   , GENERIC_VALUE_DOUBLE );

    // range parameters for basic actuator commands
    addSetting( "maxpower"          , &iMaxPower        , GENERIC_VALUE_INTEGER);
    addSetting( "minpower"          , &iMinPower        , GENERIC_VALUE_INTEGER);
    addSetting( "maxmoment"         , &iMaxMoment       , GENERIC_VALUE_INTEGER);
    addSetting( "minmoment"         , &iMinMoment       , GENERIC_VALUE_INTEGER);
    addSetting( "maxneckmoment"     , &iMaxNeckMoment   , GENERIC_VALUE_INTEGER);
    addSetting( "minneckmoment"     , &iMinNeckMoment   , GENERIC_VALUE_INTEGER);
    addSetting( "maxneckang"        , &iMaxNeckAng      , GENERIC_VALUE_INTEGER);
    addSetting( "minneckang"        , &iMinNeckAng      , GENERIC_VALUE_INTEGER);

    // port-related parameters
    addSetting( "port"              , &iPort            , GENERIC_VALUE_INTEGER);
    addSetting( "coach_port"        , &iCoachPort       , GENERIC_VALUE_INTEGER);
    addSetting( "ol_coach_port"     , &iOlCoachPort     , GENERIC_VALUE_INTEGER);

    // coach-related parameters
    addSetting( "say_coach_cnt_max" , &iSayCoachCntMax  , GENERIC_VALUE_INTEGER);
    addSetting( "say_coach_msg_size", &iSayCoachMsgSize , GENERIC_VALUE_INTEGER);
    addSetting( "clang_win_size"    , &iClangWinSize    , GENERIC_VALUE_INTEGER);
    addSetting( "clang_define_win"  , &iClangDefineWin  , GENERIC_VALUE_INTEGER);
    addSetting( "clang_meta_win"    , &iClangMetaWin    , GENERIC_VALUE_INTEGER);
    addSetting( "clang_advice_win"  , &iClangAdviceWin  , GENERIC_VALUE_INTEGER);
    addSetting( "clang_info_win"    , &iClangInfoWin    , GENERIC_VALUE_INTEGER);
    addSetting( "clang_mess_delay"  , &iClangMessDelay  , GENERIC_VALUE_INTEGER);
    addSetting("clang_mess_per_cycle",&iClangMessPerCycle,GENERIC_VALUE_INTEGER);
    addSetting( "send_vi_step"      , &iSendViStep      , GENERIC_VALUE_INTEGER);

    // time-related parameters
    addSetting( "simulator_step"    , &iSimulatorStep   , GENERIC_VALUE_INTEGER);
    addSetting( "send_step"         , &iSendStep        , GENERIC_VALUE_INTEGER);
    addSetting( "recv_step"         , &iRecvStep        , GENERIC_VALUE_INTEGER);
    addSetting( "half_time"         , &iHalfTime        , GENERIC_VALUE_INTEGER);
    addSetting( "drop_ball_time"    , &iDropBallTime    , GENERIC_VALUE_INTEGER);

    // speech-related parameters
    addSetting( "say_msg_size"      , &iSayMsgSize      , GENERIC_VALUE_INTEGER);

    // offside-related parameters
    addSetting( "use_offside"       , &bUseOffside      , GENERIC_VALUE_BOOLEAN);
    addSetting( "offside_active_area_size", &dOffsideActiveAreaSize,
                GENERIC_VALUE_DOUBLE );
    addSetting( "forbid_kick_off_offside" , &bForbidKickOffOffside ,
                GENERIC_VALUE_BOOLEAN);
    addSetting( "offside_kick_margin",&dOffsideKickMargin,GENERIC_VALUE_DOUBLE );

    // log-related parameters
    addSetting( "verbose"           , &bVerbose         , GENERIC_VALUE_BOOLEAN);
    addSetting( "record_version"    , &iRecordVersion   , GENERIC_VALUE_INTEGER);
    addSetting( "record_log"        , &bRecordLog       , GENERIC_VALUE_BOOLEAN);
    addSetting( "send_log"          , &bSendLog         , GENERIC_VALUE_BOOLEAN);
    addSetting( "log_times"         , &bLogTimes        , GENERIC_VALUE_BOOLEAN);
    addSetting( "log_file"          , &strLogFile       , GENERIC_VALUE_STRING );
    addSetting( "synch_mode"        , &bSynchMode       , GENERIC_VALUE_BOOLEAN);
    addSetting( "fullstate_l"       , &bFullStateL      , GENERIC_VALUE_BOOLEAN);
    addSetting( "fullstate_r"       , &bFullStateR      , GENERIC_VALUE_BOOLEAN);

    // heterogeneous player parameters from player.conf
    addSetting( "player_types"      , &iPlayerTypes     , GENERIC_VALUE_INTEGER);
    addSetting( "subs_max"          , &iSubsMax         , GENERIC_VALUE_INTEGER);
    addSetting( "player_speed_max_delta_min"  , &dPlayerSpeedMaxDeltaMin  ,
                GENERIC_VALUE_DOUBLE );
    addSetting( "player_speed_max_delta_max"  , &dPlayerSpeedMaxDeltaMax  ,
                GENERIC_VALUE_DOUBLE );
    addSetting( "stamina_inc_max_delta_factor", &dStaminaIncMaxDeltaFactor,
                GENERIC_VALUE_DOUBLE );
    addSetting( "player_decay_delta_min"      , &dPlayerDecayDeltaMin     ,
                GENERIC_VALUE_DOUBLE );
    addSetting( "player_decay_delta_max"      , &dPlayerDecayDeltaMax     ,
                GENERIC_VALUE_DOUBLE );
    addSetting( "inertia_moment_delta_factor" , &dInertiaMomentDeltaFactor,
                GENERIC_VALUE_DOUBLE );
    addSetting( "dash_power_rate_delta_min"   , &dDashPowerRateDeltaMin   ,
                GENERIC_VALUE_DOUBLE );
    addSetting( "dash_power_rate_delta_max"   , &dDashPowerRateDeltaMax   ,
                GENERIC_VALUE_DOUBLE );
    addSetting( "player_size_delta_factor"    , &dPlayerSizeDeltaFactor   ,
                GENERIC_VALUE_DOUBLE );
    addSetting( "kickable_margin_delta_min"   , &dKickableMarginDeltaMin  ,
                GENERIC_VALUE_DOUBLE );
    addSetting( "kickable_margin_delta_max"   , &dKickableMarginDeltaMax  ,
                GENERIC_VALUE_DOUBLE );
    addSetting( "kick_rand_delta_factor"      , &dKickRandDeltaFactor     ,
                GENERIC_VALUE_DOUBLE );
    addSetting( "extra_stamina_delta_min"     , &dExtraStaminaDeltaMin    ,
                GENERIC_VALUE_DOUBLE );
    addSetting( "extra_stamina_delta_max"     , &dExtraStaminaDeltaMax    ,
                GENERIC_VALUE_DOUBLE );
    addSetting( "effort_max_delta_factor"     , &dEffortMaxDeltaFactor    ,
                GENERIC_VALUE_DOUBLE );
    addSetting( "effort_min_delta_factor"     , &dEffortMinDeltaFactor    ,
                GENERIC_VALUE_DOUBLE );
    addSetting( "new_dash_power_rate_delta_min", &dNewDashPowerRateDeltaMin   ,
                GENERIC_VALUE_DOUBLE );
    addSetting( "new_dash_power_rate_delta_max", &dNewDashPowerRateDeltaMax   ,
                GENERIC_VALUE_DOUBLE );
    addSetting( "new_stamina_inc_max_delta_factor", &dNewStaminaIncMaxDeltaFactor
                ,GENERIC_VALUE_DOUBLE  );

    // penalty parameters
    addSetting( "pen_dist_x"        , &dPenDistX        , GENERIC_VALUE_DOUBLE );
    addSetting("pen_goalie_max_dist_x",&dPenMaxGoalieDistX,GENERIC_VALUE_DOUBLE);
    addSetting("pen_allow_mult_kicks",&bPenAllowMultKicks,GENERIC_VALUE_BOOLEAN);

    // tackle parameters
    addSetting( "tackle_dist"       , &dTackleDist      , GENERIC_VALUE_DOUBLE );
    addSetting( "tackle_back_dist"  , &dTackleBackDist  , GENERIC_VALUE_DOUBLE );
    addSetting( "tackle_width"      , &dTackleWidth     , GENERIC_VALUE_DOUBLE );
    addSetting( "tackle_exponent"   , &dTackleExponent  , GENERIC_VALUE_DOUBLE );
    addSetting( "tackle_cycles"     , &iTackleCycles    , GENERIC_VALUE_INTEGER);
    addSetting( "tackle_power_rate" , &dTacklePowerRate , GENERIC_VALUE_DOUBLE );

    // important server parameters not in server.conf or player.conf - are now
    addSetting( "effort_max"        , &dEffortMax       , GENERIC_VALUE_DOUBLE );
    addSetting( "slow_down_factor"  , &iSlowDownFactor  , GENERIC_VALUE_INTEGER);
    addSetting( "visible_distance"  , &dVisibleDistance , GENERIC_VALUE_DOUBLE );
    addSetting( "extra_stamina"     , &dExtraStamina    , GENERIC_VALUE_DOUBLE );

    // parameters which depend on other values
    addSetting( "dMaximalKickDist"  , &dMaximalKickDist , GENERIC_VALUE_DOUBLE );
}

/*! This method is originally defined in the superclass GenericValues and is
    overridden in this subclass. It sets the variable denoted by the first
    argument to the value denoted by the second argument.
    \param strName a string representing the name of a variable for which the
    value must be (re)set
    \param strValue a string representing a value which must be assigned to the
    variable denoted by the first argument
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setValue( const char *strName, const char *strValue )
{
    // call to the superclass method
    bool bReturn = GenericValues::setValue( strName, strValue );
    // compute values for parameters which depend on others (reason for override)
    dMaximalKickDist = ( dKickableMargin + dPlayerSize + dBallSize );

    return ( bReturn );
}

/*! This method is originally defined in the superclass GenericValues and is
    overridden in this subclass. It reads the values from a server
    configuration file and assigns them to the proper variables in this class.
    \param strFileName a string representing the name of a configuration file
    \param strSeparator a string representing the separator between the name of
    a variable and its value
    \return a boolean indicating whether the values were read correctly */
bool ServerSettings::readValues( const char *strFileName,
                                 const char *strSeparator )
{
    // call to the superclass method
    bool bReturn = GenericValues::readValues( strFileName, strSeparator );
    // compute values for parameters which depend on others (reason for override)
    dMaximalKickDist = ( dKickableMargin + dPlayerSize + dBallSize );

    return ( bReturn );
}

/*! Set method for the 'dGoalWidth' member variable.
    \param d a double value representing a new width of the goal
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setGoalWidth( double d )
{
    dGoalWidth = d;
    return ( true );
}

/*! Get method for the 'dGoalWidth' member variable.
    \return the width of the goal */
double ServerSettings::getGoalWidth( ) const
{
    return ( dGoalWidth );
}

/*! Set method for the 'dPlayerSize' member variable.
    \param d a double value representing a new player size
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setPlayerSize( double d )
{
    dPlayerSize = d;
    // NOTE: also update parameters for which the value depends on this variable
    dMaximalKickDist = ( dKickableMargin + dPlayerSize + dBallSize );

    return ( true );
}

/*! Get method for the 'dPlayerSize' member variable.
    \return the size (=radius) of a player */
double ServerSettings::getPlayerSize( ) const
{
    return ( dPlayerSize );
}

/*! Set method for the 'dPlayerDecay' member variable.
    \param d a double value representing a new player speed decay per cycle
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setPlayerDecay( double d )
{
    dPlayerDecay = d;
    return ( true );
}

/*! Get method for the 'dPlayerDecay' member variable.
    \return the player speed decay per cycle */
double ServerSettings::getPlayerDecay( ) const
{
    return ( dPlayerDecay );
}

/*! Set method for the 'dPlayerRand' member variable.
    \param d a double value representing a new random error in player movement
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setPlayerRand( double d )
{
    dPlayerRand = d;
    return ( true );
}

/*! Get method for the 'dPlayerRand' member variable.
    \return the random error in player movement */
double ServerSettings::getPlayerRand( ) const
{
    return ( dPlayerRand );
}

/*! Set method for the 'dPlayerWeight' member variable.
    \param d a double value representing a new weight of a player (for wind)
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setPlayerWeight( double d )
{
    dPlayerWeight = d;
    return ( true );
}

/*! Get method for the 'dPlayerWeight' member variable.
    \return the weight of a player (for wind) */
double ServerSettings::getPlayerWeight( ) const
{
    return ( dPlayerWeight );
}

/*! Set method for the 'dPlayerSpeedMax' member variable.
    \param d a double value representing a new maximum speed of a player
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setPlayerSpeedMax( double d )
{
    dPlayerSpeedMax = d;
    return ( true );
}

/*! Get method for the 'dPlayerSpeedMax' member variable.
    \return the maximum speed of a player */
double ServerSettings::getPlayerSpeedMax( ) const
{
    return ( dPlayerSpeedMax );
}

/*! Set method for the 'dPlayerAccelMax' member variable.
    \param d a double value representing a new maximum acceleration of a player
    per cycle
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setPlayerAccelMax( double d )
{
    dPlayerAccelMax = d;
    return ( true );
}

/*! Get method for the 'dPlayerAccelMax' member variable.
    \return the maximum acceleration of a player per cycle */
double ServerSettings::getPlayerAccelMax( ) const
{
    return ( dPlayerAccelMax );
}

/*! Set method for the 'dStaminaMax' member variable.
    \param d a double value representing a new maximum stamina of a player
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setStaminaMax( double d )
{
    dStaminaMax = d;
    return ( true );
}

/*! Get method for the 'dStaminaMax' member variable.
    \return the maximum stamina of a player */
double ServerSettings::getStaminaMax( ) const
{
    return ( dStaminaMax );
}

/*! Set method for the 'dStaminaIncMax' member variable.
    \param d a double value representing a new maximum stamina increase of a
    player per cycle
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setStaminaIncMax( double d )
{
    dStaminaIncMax = d;
    return ( true );
}

/*! Get method for the 'dStaminaIncMax' member variable.
    \return the maximum stamina increase of a player per cycle */
double ServerSettings::getStaminaIncMax( ) const
{
    return ( dStaminaIncMax );
}

/*! Set method for the 'dRecoverDecThr' member variable.
    \param d a double value representing a new percentage of stamina_max below
    which player recovery decreases
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setRecoverDecThr( double d )
{
    dRecoverDecThr = d;
    return ( true );
}

/*! Get method for the 'dRecoverDecThr' member variable.
    \return percentage of stamina_max below which player recovery decreases */
double ServerSettings::getRecoverDecThr( ) const
{
    return ( dRecoverDecThr );
}

/*! Set method for the 'dRecoverDec' member variable.
    \param d a double value representing a new decrement step per cycle for
    player recovery
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setRecoverDec( double d )
{
    dRecoverDec = d;
    return ( true );
}

/*! Get method for the 'dRecoverDec' member variable.
    \return the decrement step per cycle for player recovery */
double ServerSettings::getRecoverDec( ) const
{
    return ( dRecoverDec );
}

/*! Set method for the 'dRecoverMin' member variable.
    \param d a double value representing a new minimum player recovery
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setRecoverMin( double d )
{
    dRecoverMin = d;
    return ( true );
}

/*! Get method for the 'dRecoverMin' member variable.
    \return the minimum player recovery */
double ServerSettings::getRecoverMin( ) const
{
    return ( dRecoverMin );
}

/*! Set method for the 'dEffortDecThr' member variable.
    \param d a double value representing a new percentage of stamina_max below
    which player effort capacity decreases
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setEffortDecThr( double d )
{
    dEffortDecThr = d;
    return ( true );
}

/*! Get method for the 'dEffortDecThr' member variable.
    \return the percentage of stamina_max below which player effort capacity
    decreases */
double ServerSettings::getEffortDecThr( ) const
{
    return ( dEffortDecThr );
}

/*! Set method for the 'dEffortDec' member variable.
    \param d a double value representing a new decrement step per cycle for
    player effort capacity
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setEffortDec( double d )
{
    dEffortDec = d;
    return ( true );
}

/*! Get method for the 'dEffortDec' member variable.
    \return the decrement step per cycle for player effort capacity */
double ServerSettings::getEffortDec( ) const
{
    return ( dEffortDec );
}

/*! Set method for the 'dEffortIncThr' member variable.
    \param d a double value representing a new percentage of stamina_max above
    which player effort capacity increases
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setEffortIncThr( double d )
{
    dEffortIncThr = d;
    return ( true );
}

/*! Get method for the 'dEffortIncThr' member variable.
    \return the percentage of stamina_max above which player effort capacity
    increases */
double ServerSettings::getEffortIncThr( ) const
{
    return ( dEffortIncThr );
}

/*! Set method for the 'dEffortInc' member variable.
    \param d a double value representing a new increment step per cycle for
    player effort capacity
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setEffortInc( double d )
{
    dEffortInc = d;
    return ( true );
}

/*! Get method for the 'dEffortInc' member variable.
    \return the increment step per cycle for player effort capacity */
double ServerSettings::getEffortInc( ) const
{
    return ( dEffortInc );
}

/*! Set method for the 'dEffortMin' member variable.
    \param d a double value representing a new minimum value for player effort
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setEffortMin( double d )
{
    dEffortMin = d;
    return ( true );
}

/*! Get method for the 'dEffortMin' member variable.
    \return the minimum value for player effort */
double ServerSettings::getEffortMin( ) const
{
    return ( dEffortMin );
}

/*! Set method for the 'iHearMax' member variable.

    \param i an integer value representing a new maximum hearing
    capacity of a player (a player can hear iHearInc messages in
    iHearDecay simulation cycles)

    \return a boolean indicating whether the update was successful */
bool ServerSettings::setHearMax( int i )
{
    iHearMax = i;
    return ( true );
}

/*! Get method for the 'iHearMax' member variable.

    \return the maximum hearing capacity of a player (a player can
    hear iHearInc messages in iHearDecay simulation cycles) */
int ServerSettings::getHearMax( ) const
{
    return ( iHearMax );
}

/*! Set method for the 'iHearInc' member variable.
    \param i an integer value representing a new minimum hearing capacity of a
    player, i.e. the number of messages a player can hear in iHearDecay
    simulation cycles
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setHearInc( int i )
{
    iHearInc = i;
    return ( true );
}

/*! Get method for the 'iHearInc' member variable.
    \return the minimum hearing capacity of a player, i.e. the number of
    messages a player can hear in iHearDecay simulation cycles */
int ServerSettings::getHearInc( ) const
{
    return ( iHearInc );
}

/*! Set method for the 'iHearDecay' member variable.
    \param i an integer value representing a new decay rate of player hearing
    capacity, i.e. minimum number of cycles for iHearInc messages
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setHearDecay( int i )
{
    iHearDecay = i;
    return ( true );
}

/*! Get method for the 'iHearDecay' member variable.
    \return the decay rate of player hearing capacity, i.e. minimum number of
    cycles for iHearInc messages */
int ServerSettings::getHearDecay( ) const
{
    return ( iHearDecay );
}

/*! Set method for the 'dInertiaMoment' member variable.
    \param d a double value representing a new inertia moment of a player
    (affects actual turn angle depending on speed)
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setInertiaMoment( double d )
{
    dInertiaMoment = d;
    return ( true );
}

/*! Get method for the 'dInertiaMoment' member variable.
    \return the inertia moment of a player (affects actual turn angle depending
    on speed) */
double ServerSettings::getInertiaMoment( ) const
{
    return ( dInertiaMoment );
}

/*! Set method for the 'iSenseBodyStep' member variable.
    \param i an integer value representing a new length of the interval (in ms)
    between sense_body information messages
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setSenseBodyStep( int i )
{
    iSenseBodyStep = i;
    return ( true );
}

/*! Get method for the 'iSenseBodyStep' member variable.
    \return the length of the interval (in ms) between sense_body information
    messages */
int ServerSettings::getSenseBodyStep( ) const
{
    // NOTE: do not take slow down factor into account for send intervals
    // already done by server
    return iSenseBodyStep ; // * iSlowDownFactor );
}

/*! Set method for the 'dCatchableAreaL' member variable.
    \param d a double value representing a new length of the area around the
    goalkeeper in which he can catch the ball
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setCatchableAreaL( double d )
{
    dCatchableAreaL = d;
    return ( true );
}

/*! Get method for the 'dCatchableAreaL' member variable.
    \return the length of the area around the goalkeeper in which he can catch
    the ball */
double ServerSettings::getCatchableAreaL( ) const
{
    return ( dCatchableAreaL );
}

/*! Set method for the 'dCatchableAreaW' member variable.
    \param d a double value representing a new width of the area around the
    goalkeeper in which he can catch the ball
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setCatchableAreaW( double d )
{
    dCatchableAreaW = d;
    return ( true );
}

/*! Get method for the 'dCatchableAreaW' member variable.
    \return the width of the area around the goalkeeper in which he can catch
    the ball */
double ServerSettings::getCatchableAreaW( ) const
{
    return ( dCatchableAreaW );
}

/*! Set method for the 'dCatchProbability' member variable.
    \param d a double value representing a new probability for a goalkeeper to
    catch the ball
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setCatchProbability( double d )
{
    dCatchProbability = d;
    return ( true );
}

/*! Get method for the 'dCatchProbability' member variable.
    \return the probability for a goalkeeper to catch the ball */
double ServerSettings::getCatchProbability( ) const
{
    return ( dCatchProbability );
}

/*! Set method for the 'iCatchBanCycle' member variable.
    \param i an integer value representing a new number of cycles after a catch
    in which the goalkeeper cannot catch again
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setCatchBanCycle( int i )
{
    iCatchBanCycle = i;
    return ( true );
}

/*! Get method for the 'iCatchBanCycle' member variable.
    \return the number of cycles after a catch in which the goalkeeper cannot
    catch again */
int ServerSettings::getCatchBanCycle( ) const
{
    return ( iCatchBanCycle );
}

/*! Set method for the 'iGoalieMaxMoves' member variable.
    \param i an integer value representing a new maximum number of 'move'
    actions allowed for a goalkeeper after a catch
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setGoalieMaxMoves( int i )
{
    iGoalieMaxMoves = i;
    return ( true );
}

/*! Get method for the 'iGoalieMaxMoves' member variable.
    \return the maximum number of 'move' actions allowed for a goalkeeper after
    a catch */
int ServerSettings::getGoalieMaxMoves( ) const
{
    return ( iGoalieMaxMoves );
}

/*! Set method for the 'dBallSize' member variable.
    \param d a double value representing a new ball size
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setBallSize( double d )
{
    dBallSize = d;
    // NOTE: also update parameters for which the value depends on this variable
    dMaximalKickDist = ( dKickableMargin + dPlayerSize + dBallSize );

    return ( true );
}

/*! Get method for the 'dBallSize' member variable.
    \return the size (=radius) of the ball */
double ServerSettings::getBallSize( ) const
{
    return ( dBallSize );
}

/*! Set method for the 'dBallDecay' member variable.
    \param d a double value representing a new ball speed decay per cycle
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setBallDecay( double d )
{
    dBallDecay = d;
    return ( true );
}

/*! Get method for the 'dBallDecay' member variable.
    \return the ball speed decay per cycle */
double ServerSettings::getBallDecay( ) const
{
    return ( dBallDecay );
}

/*! Set method for the 'dBallRand' member variable.

    \param d a double value representing a new random error in the
    ball movement

    \return a boolean indicating whether the update was successful */
bool ServerSettings::setBallRand( double d )
{
    dBallRand = d;
    return ( true );
}

/*! Get method for the 'dBallRand' member variable.
    \return the random error in the ball movement */
double ServerSettings::getBallRand( ) const
{
    return ( dBallRand );
}

/*! Set method for the 'dBallWeight' member variable.
    \param d a double value representing a new weight of the ball (for wind)
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setBallWeight( double d )
{
    dBallWeight = d;
    return ( true );
}

/*! Get method for the 'dBallWeight' member variable.
    \return the weight of the ball (for wind) */
double ServerSettings::getBallWeight( ) const
{
    return ( dBallWeight );
}

/*! Set method for the 'dBallSpeedMax' member variable.
    \param d a double value representing a new maximum speed of the ball
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setBallSpeedMax( double d )
{
    dBallSpeedMax = d;
    return ( true );
}

/*! Get method for the 'dBallSpeedMax' member variable.
    \return the maximum speed of the ball */
double ServerSettings::getBallSpeedMax( ) const
{
    return ( dBallSpeedMax );
}

/*! Set method for the 'dBallAccelMax' member variable.
    \param d a double value representing a new maximum acceleration of the ball
    per cycle
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setBallAccelMax( double d )
{
    dBallAccelMax = d;
    return ( true );
}

/*! Get method for the 'dBallAccelMax' member variable.
    \return the maximum acceleration of the ball per cycle */
double ServerSettings::getBallAccelMax( ) const
{
    return ( dBallAccelMax );
}

/*! Set method for the 'dWindForce' member variable.
    \param d a double value representing a new force of the wind
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setWindForce( double d )
{
    dWindForce = d;
    return ( true );
}

/*! Get method for the 'dWindForce' member variable.
    \return the force of the wind */
double ServerSettings::getWindForce( ) const
{
    return ( dWindForce );
}

/*! Set method for the 'dWindDir' member variable.
    \param d a double value representing a new direction of the wind
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setWindDir( double d )
{
    dWindDir = d;
    return ( true );
}

/*! Get method for the 'dWindDir' member variable.
    \return the direction of the wind */
double ServerSettings::getWindDir( ) const
{
    return ( dWindDir );
}

/*! Set method for the 'dWindRand' member variable.
    \param d a double value representing a new random error in wind direction
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setWindRand( double d )
{
    dWindRand = d;
    return ( true );
}

/*! Get method for the 'dWindRand' member variable.
    \return the random error in wind direction */
double ServerSettings::getWindRand( ) const
{
    return ( dWindRand );
}

/*! Set method for the 'bWindRandom' member variable.
    \param b a boolean indicating whether wind force and direction are random
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setWindRandom( bool b )
{
    bWindRandom = b;
    return ( true );
}

/*! Get method for the 'bWindRandom' member variable.
    \return boolean indicating whether wind force and direction are random */
bool ServerSettings::getWindRandom( ) const
{
    return bWindRandom;
}

/*! Set method for the 'dKickableMargin' member variable.
    \param d a double value representing a new margin around a player in which
    the ball is kickable (kickable area thus equals kickable_margin + ball_size
    + player_size)
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setKickableMargin( double d )
{
    dKickableMargin = d;
    // NOTE: also update parameters for which the value depends on this variable
    dMaximalKickDist = ( dKickableMargin + dPlayerSize + dBallSize );

    return ( true );
}

/*! Get method for the 'dKickableMargin' member variable.
    \return the margin around a player in which the ball is kickable (kickable
    area thus equals kickable_margin + ball_size + player_size) */
double ServerSettings::getKickableMargin( ) const
{
    return ( dKickableMargin );
}

/*! Set method for the 'dCkickMargin' member variable.
    \param d a double value representing a new corner kick margin, i.e. a new
    minimum distance to the ball for offending players when a corner kick is
    taken
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setCkickMargin( double d )
{
    dCkickMargin = d;
    return ( true );
}

/*! Get method for the 'dCkickMargin' member variable.
    \return the corner kick margin, i.e. the minimum distance to the ball for
    offending players when a corner kick is taken */
double ServerSettings::getCkickMargin( ) const
{
    return ( dCkickMargin );
}

/*! Set method for the 'dDashPowerRate' member variable.
    \param d a double value representing a new rate by which the 'Power'
    argument in a 'dash' command is multiplied (thus determining the amount of
    displacement of the player as a result of the 'dash')
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setDashPowerRate( double d )
{
    dDashPowerRate = d;
    return ( true );
}

/*! Get method for the 'dDashPowerRate' member variable.
    \return the rate by which the 'Power' argument in a 'dash' command is
    multiplied (thus determining the amount of displacement of the player as a
    result of the 'dash') */
double ServerSettings::getDashPowerRate( ) const
{
    return ( dDashPowerRate );
}

/*! Set method for the 'dKickPowerRate' member variable.
    \param d a double value representing a new rate by which the 'Power'
    argument in a 'kick' command is multiplied (thus determining the amount of
    displacement of the ball as a result of the 'kick')
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setKickPowerRate( double d )
{
    dKickPowerRate = d;
    return ( true );
}

/*! Get method for the 'dKickPowerRate' member variable.
    \return the rate by which the 'Power' argument in a 'kick' command is
    multiplied (thus determining the amount of displacement of the ball as a
    result of the 'kick') */
double ServerSettings::getKickPowerRate( ) const
{
    return ( dKickPowerRate );
}

/*! Set method for the 'dKickRand' member variable.
    \param d a double value representing a new random error in kick direction
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setKickRand( double d )
{
    dKickRand = d;
    return ( true );
}

/*! Get method for the 'dKickRand' member variable.
    \return the random error in kick direction */
double ServerSettings::getKickRand( ) const
{
    return ( dKickRand );
}

/*! Set method for the 'dVisibleAngle' member variable.
    \param d a double value representing a new angle of the view cone of a
    player in the standard view mode
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setVisibleAngle( double d )
{
    dVisibleAngle = d;
    return ( true );
}

/*! Get method for the 'dVisibleAngle' member variable.
    \return the angle of the view cone of a player in the standard view mode */
double ServerSettings::getVisibleAngle( ) const
{
    return ( dVisibleAngle );
}

/*! Set method for the 'dAudioCutDist' member variable.
    \param d a double value representing a new maximum distance over which a
    spoken message can be heard
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setAudioCutDist( double d )
{
    dAudioCutDist = d;
    return ( true );
}

/*! Get method for the 'dAudioCutDist' member variable.
    \return the maximum distance over which a spoken message can be heard */
double ServerSettings::getAudioCutDist( ) const
{
    return ( dAudioCutDist );
}

/*! Set method for the 'dQuantizeStep' member variable.
    \param d a double value representing a new quantization step for the
    distance of moving objects
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setQuantizeStep( double d )
{
    dQuantizeStep = d;
    return ( true );
}

/*! Get method for the 'dQuantizeStep' member variable.
    \return the quantization step for the distance of moving objects */
double ServerSettings::getQuantizeStep( ) const
{
    return ( dQuantizeStep );
}

/*! Set method for the 'dQuantizeStepL' member variable.
    \param d a double value representing a new quantization step for the
    distance of landmarks
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setQuantizeStepL( double d )
{
    dQuantizeStepL = d;
    return ( true );
}

/*! Get method for the 'dQuantizeStepL' member variable.
    \return the quantization step for the distance of landmarks */
double ServerSettings::getQuantizeStepL( ) const
{
    return ( dQuantizeStepL );
}

/*! Set method for the 'iMaxPower' member variable.
    \param i an integer value representing a new maximum power for a dash/kick
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setMaxPower( int i )
{
    iMaxPower = i;
    return ( true );
}

/*! Get method for the 'iMaxPower' member variable.
    \return the maximum power for a dash/kick */
int ServerSettings::getMaxPower( ) const
{
    return ( iMaxPower );
}

/*! Set method for the 'iMinPower' member variable.
    \param i an integer value representing a new minimum power for a dash/kick
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setMinPower( int i )
{
    iMinPower = i;
    return ( true );
}

/*! Get method for the 'iMinPower' member variable.
    \return the minimum power for a dash/kick */
int ServerSettings::getMinPower( ) const
{
    return ( iMinPower );
}

/*! Set method for the 'iMaxMoment' member variable.
    \param i an integer value representing a new maximum angle for a turn/kick
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setMaxMoment( int i )
{
    iMaxMoment = i;
    return ( true );
}

/*! Get method for the 'iMaxMoment' member variable.
    \return the maximum angle for a turn/kick */
int ServerSettings::getMaxMoment( ) const
{
    return ( iMaxMoment );
}

/*! Set method for the 'iMinMoment' member variable.
    \param i an integer value representing a new minimum angle for a turn/kick
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setMinMoment( int i )
{
    iMinMoment = i;
    return ( true );
}

/*! Get method for the 'iMinMoment' member variable.
    \return the minimum angle for a turn/kick */
int ServerSettings::getMinMoment( ) const
{
    return ( iMinMoment );
}

/*! Set method for the 'iMaxNeckMoment' member variable.
    \param i an integer value representing a new maximum angle for a turnneck
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setMaxNeckMoment( int i )
{
    iMaxNeckMoment = i;
    return ( true );
}

/*! Get method for the 'iMaxNeckMoment' member variable.
    \return the maximum angle for a turnneck */
int ServerSettings::getMaxNeckMoment( ) const
{
    return ( iMaxNeckMoment );
}

/*! Set method for the 'iMinNeckMoment' member variable.
    \param i an integer value representing a new minimum angle for a turnneck
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setMinNeckMoment( int i )
{
    iMinNeckMoment = i;
    return ( true );
}

/*! Get method for the 'iMinNeckMoment' member variable.
    \return the minimum angle for a turnneck */
int ServerSettings::getMinNeckMoment( ) const
{
    return ( iMinNeckMoment );
}

/*! Set method for the 'iMaxNeckAng' member variable.

    \param i an integer value representing a new maximum neck angle
    rel. to body

    \return a boolean indicating whether the update was successful */
bool ServerSettings::setMaxNeckAng( int i )
{
    iMaxNeckAng = i;
    return ( true );
}

/*! Get method for the 'iMaxNeckAng' member variable.
    \return the maximum neck angle relative to body */
int ServerSettings::getMaxNeckAng( ) const
{
    return ( iMaxNeckAng );
}

/*! Set method for the 'iMinNeckAng' member variable.

    \param i an integer value representing a new minimum neck angle
    rel. to body

    \return a boolean indicating whether the update was successful */
bool ServerSettings::setMinNeckAng( int i )
{
    iMinNeckAng = i;
    return ( true );
}

/*! Get method for the 'iMinNeckAng' member variable.
    \return the minimum neck angle relative to body */
int ServerSettings::getMinNeckAng( ) const
{
    return ( iMinNeckAng );
}

/*! Set method for the 'iPort' member variable.
    \param i an integer value representing a new port number for a player
    connection
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setPort( int i )
{
    iPort = i;
    return ( true );
}

/*! Get method for the 'iPort' member variable.
    \return the port number for a player connection */
int ServerSettings::getPort( ) const
{
    return ( iPort );
}

/*! Set method for the 'iCoachPort' member variable.
    \param i an integer value representing a new port number for a coach
    connection
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setCoachPort( int i )
{
    iCoachPort = i;
    return ( true );
}

/*! Get method for the 'iCoachPort' member variable.
    \return the port number for a coach connection */
int ServerSettings::getCoachPort( ) const
{
    return ( iCoachPort );
}

/*! Set method for the 'iOlCoachPort' member variable.
    \param i an integer value representing a new port number for an online
             coach connection
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setOlCoachPort( int i )
{
    iOlCoachPort = i;
    return ( true );
}

/*! Get method for the 'iOlCoachPort' member variable.
    \return the port number for an online coach connection */
int ServerSettings::getOlCoachPort( ) const
{
    return ( iOlCoachPort );
}

/*! Set method for the 'iSayCoachCntMax' member variable.
    \param i an integer value representing a new maximum number of coach
    messages possible
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setSayCoachCntMax( int i )
{
    iSayCoachCntMax = i;
    return ( true );
}

/*! Get method for the 'iSayCoachCntMax' member variable.
    \return the maximum number of coach messages possible */
int ServerSettings::getSayCoachCntMax( ) const
{
    return ( iSayCoachCntMax );
}

/*! Set method for the 'iSayCoachMsgSize' member variable.
    \param i an integer value representing a new maximum size of coach messages
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setSayCoachMsgSize( int i )
{
    iSayCoachMsgSize = i;
    return ( true );
}

/*! Get method for the 'iSayCoachMsgSize' member variable.
    \return the maximum size of coach messages */
int ServerSettings::getSayCoachMsgSize( ) const
{
    return ( iSayCoachMsgSize );
}

/*! Set method for the 'iClangWinSize' member variable.
    \param i an integer value representing a new time window which controls how
    many coach messages can be sent
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setClangWinSize( int i )
{
    iClangWinSize = i;
    return ( true );
}

/*! Get method for the 'iClangWinSize' member variable.
    \return time window which controls how many coach messages can be sent */
int ServerSettings::getClangWinSize( ) const
{
    return ( iClangWinSize );
}

/*! Set method for the 'iClangDefineWin' member variable.
    \param i an integer value representing a new number of define messages by
    the coach per time window
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setClangDefineWin( int i )
{
    iClangDefineWin = i;
    return ( true );
}

/*! Get method for the 'iClangDefineWin' member variable.
    \return the number of define messages by the coach per time window */
int ServerSettings::getClangDefineWin( ) const
{
    return ( iClangDefineWin );
}

/*! Set method for the 'iClangMetaWin' member variable.
    \param i an integer value representing a new number of meta messages by the
    coach per time window
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setClangMetaWin( int i )
{
    iClangMetaWin = i;
    return ( true );
}

/*! Get method for the 'iClangMetaWin' member variable.
    \return the number of meta messages by the coach per time window */
int ServerSettings::getClangMetaWin( ) const
{
    return ( iClangMetaWin );
}

/*! Set method for the 'iClangAdviceWin' member variable.
    \param i an integer value representing a new number of advice messages by
    the coach per time window
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setClangAdviceWin( int i )
{
    iClangAdviceWin = i;
    return ( true );
}

/*! Get method for the 'iClangAdviceWin' member variable.
    \return the number of advice messages by the coach per time window */
int ServerSettings::getClangAdviceWin( ) const
{
    return ( iClangAdviceWin );
}

/*! Set method for the 'iClangInfoWin' member variable.
    \param i an integer value representing a new number of info messages by the
    coach per time window
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setClangInfoWin( int i )
{
    iClangInfoWin = i;
    return ( true );
}

/*! Get method for the 'iClangInfoWin' member variable.
    \return the number of info messages by the coach per time window */
int ServerSettings::getClangInfoWin( ) const
{
    return ( iClangInfoWin );
}

/*! Set method for the 'iClangMessDelay' member variable.
    \param i an integer value representing a new delay of coach messages, i.e.
    a new number of cycles between the send to a player and the receival of the
    message
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setClangMessDelay( int i )
{
    iClangMessDelay = i;
    return ( true );
}

/*! Get method for the 'iClangMessDelay' member variable.
    \return the delay of coach messages, i.e. the number of cycles between the
    send to a player and the receival of the message */
int ServerSettings::getClangMessDelay( ) const
{
    return ( iClangMessDelay );
}

/*! Set method for the 'iClangMessPerCycle' member variable.
    \param i an integer value representing a new number of coach messages per
    cycle
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setClangMessPerCycle( int i )
{
    iClangMessPerCycle = i;
    return ( true );
}

/*! Get method for the 'iClangMessPerCycle' member variable.
    \return the number of coach messages per cycle */
int ServerSettings::getClangMessPerCycle( ) const
{
    return ( iClangMessPerCycle );
}

/*! Set method for the 'iSendViStep' member variable.
    \param i an integer value representing a new interval of the coach's look,
    i.e. a new length of the interval (in ms) between visual messages to the
    coach
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setSendViStep( int i )
{
    iSendViStep = i;
    return ( true );
}

/*! Get method for the 'iSendViStep' member variable.
    \return the interval of the coach's look, i.e. the length of the interval
    (in ms) between visual messages to the coach */
int ServerSettings::getSendViStep( ) const
{
    // NOTE: do not take slow down factor into account for send intervals
    // already done by server
    return iSendViStep ; // * iSlowDownFactor );
}

/*! Set method for the 'iSimulatorStep' member variable.
    \param i an integer value representing a new length (in ms) of a simulator
    cycle
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setSimulatorStep( int i )
{
    iSimulatorStep = i;
    return ( true );
}

/*! Get method for the 'iSimulatorStep' member variable.
    \return the length (in ms) of a simulator cycle */
int ServerSettings::getSimulatorStep( ) const
{
    // NOTE: do not take slow down factor into account for send intervals ->
    // already done in supplied server values
    return iSimulatorStep ; //  * iSlowDownFactor ;
}

/*! Set method for the 'iSendStep' member variable.
    \param i an integer value representing a new length of the interval (in ms)
    between visual messages to a player in the standard view mode
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setSendStep( int i )
{
    iSendStep = i;
    return ( true );
}

/*! Get method for the 'iSendStep' member variable.
    \return the length of the interval (in ms) between visual messages to a
    player in the standard view mode */
int ServerSettings::getSendStep( ) const
{
    // NOTE: do not take slow down factor into account for send intervals,
    // already done by server
    return  iSendStep ; // * iSlowDownFactor );
}

/*! Set method for the 'iRecvStep' member variable.
    \param i an integer value representing a new length of the interval (in ms)
    for accepting commands from a player
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setRecvStep( int i )
{
    iRecvStep = i;
    return ( true );
}

/*! Get method for the 'iRecvStep' member variable.
    \return the length of the interval (in ms) for accepting commands from a
    player */
int ServerSettings::getRecvStep( ) const
{
    return ( iRecvStep );
}

/*! Set method for the 'iHalfTime' member variable.

    \param i an integer value representing a new length (in seconds)
    of a single game half

    \return a boolean indicating whether the update was successful */
bool ServerSettings::setHalfTime( int i )
{
    iHalfTime = i;
    return ( true );
}

/*! Get method for the 'iHalfTime' member variable.
    \return the length (in seconds) of a single game half */
int ServerSettings::getHalfTime( ) const
{
    return ( iHalfTime );
}

/*! Set method for the 'iDropBallTime' member variable.
    \param i an integer value representing a new number of cycles to wait until
    dropping the ball automatically for free kicks, corner kicks, etc.
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setDropBallTime( int i )
{
    iDropBallTime = i;
    return ( true );
}

/*! Get method for the 'iDropBallTime' member variable.
    \return the number of cycles to wait until dropping the ball automatically
    for free kicks, corner kicks, etc. */
int ServerSettings::getDropBallTime( ) const
{
    return ( iDropBallTime );
}

/*! Set method for the 'iSayMsgSize' member variable.
    \param i an integer value representing a new maximum length (in bytes) of a
    spoken message
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setSayMsgSize( int i )
{
    iSayMsgSize = i;
    return ( true );
}

/*! Get method for the 'iSayMsgSize' member variable.
    \return the maximum length (in bytes) of a spoken message */
int ServerSettings::getSayMsgSize( ) const
{
    return ( iSayMsgSize );
}

/*! Set method for the 'bUseOffside' member variable.
    \param b a boolean value indicating whether the offside rule should be
    applied or not
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setUseOffside( bool b )
{
    bUseOffside = b;
    return ( true );
}

/*! Get method for the 'bUseOffside' member variable.

    \return a boolean flag indicating whether the offside rule should
    be applied or not */
bool ServerSettings::getUseOffside( ) const
{
    return ( bUseOffside );
}

/*! Set method for the 'dOffsideActiveAreaSize' member variable.
    \param d a double value representing a new offside active area size, i.e.
    radius of circle around the ball in which player can be offside
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setOffsideActiveAreaSize( double d )
{
    dOffsideActiveAreaSize = d;
    return ( true );
}

/*! Get method for the 'dOffsideActiveAreaSize' member variable.
    \return the offside active area size, i.e. radius of circle around the ball
    in which player can be offside */
double ServerSettings::getOffsideActiveAreaSize( ) const
{
    return ( dOffsideActiveAreaSize );
}

/*! Set method for the 'bForbidKickOffOffside' member variable.
    \param b a boolean value indicating whether a kick from offside position is
    allowed
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setForbidKickOffOffside( bool b )
{
    bForbidKickOffOffside = b;
    return ( true );
}

/*! Get method for the 'bForbidKickOffOffside' member variable.
    \return a boolean flag indicating whether a kick from offside position is
    allowed */
bool ServerSettings::getForbidKickOffOffside( ) const
{
    return ( bForbidKickOffOffside );
}

/*! Set method for the 'dOffsideKickMargin' member variable.
    \param d a double value representing a new offside kick margin, i.e. a new
    minimum distance to the ball for offending players when a free kick for
    offside is taken
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setOffsideKickMargin( double d )
{
    dOffsideKickMargin = d;
    return ( true );
}

/*! Get method for the 'dOffsideKickMargin' member variable.
    \return the offside kick margin, i.e. the minimum distance to the ball for
    offending players when a free kick for offside is taken */
double ServerSettings::getOffsideKickMargin( ) const
{
    return ( dOffsideKickMargin );
}

/*! Set method for the 'bVerbose' member variable.
    \param b a boolean value indicating whether the verbose mode is active or
    not; in verbose mode the server sends extra error-information
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setVerbose( bool b )
{
    bVerbose = b;
    return ( true );
}

/*! Get method for the 'bVerbose' member variable.
    \return a boolean flag indicating whether the verbose mode is active or
    not; in verbose mode the server sends extra error-information */
bool ServerSettings::getVerbose( ) const
{
    return ( bVerbose );
}

/*! Set method for the 'iRecordVersion' member variable.
    \param i an integer value representing a new type of log record
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setRecordVersion( int i )
{
    iRecordVersion = i;
    return ( true );
}

/*! Get method for the 'iRecordVersion' member variable.
    \return the type of log record */
int ServerSettings::getRecordVersion( ) const
{
    return ( iRecordVersion );
}

/*! Set method for the 'bRecordLog' member variable.
    \param b a boolean value indicating whether a log record for a game should
    be created
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setRecordLog( bool b )
{
    bRecordLog = b;
    return ( true );
}

/*! Get method for the 'bRecordLog' member variable.
    \return a boolean flag indicating whether a log record for a game should
    be created */
bool ServerSettings::getRecordLog( ) const
{
    return ( bRecordLog );
}

/*! Set method for the 'bSendLog' member variable.
    \param b a boolean value indicating whether a send client command log for a
    game should be created
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setSendLog( bool b )
{
    bSendLog = b;
    return ( true );
}

/*! Get method for the 'bSendLog' member variable.
    \return a boolean flag indicating whether a send client command log for a
    game should be created */
bool ServerSettings::getSendLog( ) const
{
    return ( bSendLog );
}

/*! Set method for the 'bLogTimes' member variable.
    \param b a boolean value indicating whether ms should be written between
    cycles in log file
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setLogTimes( bool b )
{
    bLogTimes = b;
    return ( true );
}

/*! Get method for the 'bLogTimes' member variable.
    \return a boolean flag indicating whether ms should be written between
    cycles in log file */
bool ServerSettings::getLogTimes( ) const
{
    return ( bLogTimes );
}

/*! Set method for the 'strLogFile' member variable.
    \param str a string representing a new server log to store all actions
    received
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setLogFile( char *str )
{
    strcpy( strLogFile, str );
    return ( true );
}

/*! Get method for the 'strLogFile' member variable.
    \return name of the server log in which all actions received are stored */
char* ServerSettings::getLogFile( )
{
    return ( strLogFile );
}

/*! Set method for the 'bSynchMode' member variable.
    \param b a boolean representing whether synchronization mode is on
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setSynchMode( bool b )
{
    bSynchMode = b;
    return ( true );
}

/*! Get method for the 'bSynchMode' member variable.
    \return boolean indicating whether synchronization mode is used */
bool ServerSettings::getSynchMode( ) const
{
    return ( bSynchMode );
}

/*! Get method for the 'bFullStateR' member variable.
    \return boolean representing whether full state is on for the right team */
bool ServerSettings::getFullStateRight( ) const
{
    return bFullStateR;
}

/*! Set method for the 'bFullStateR' member variable.
    \param b a boolean representing whether full state is on for the right team
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setFullStateRight( bool b )
{
    bFullStateR = b;
    return true;
}

/*! Set method for the 'bFullStateL' member variable.
    \param b a boolean representing whether full state is on for the left team
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setFullStateLeft( bool b )
{
    bFullStateL = b;
    return true;
}

/*! Get method for the 'bFullStateL' member variable.
    \return boolean representing whether full state is on for the left team */
bool ServerSettings::getFullStateLeft( ) const
{
    return bFullStateL;
}




/*! Set method for the 'iPlayerTypes' member variable.
    \param i an integer value representing a new number of player types
    including the default player type
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setPlayerTypes( int i )
{
    iPlayerTypes = i;
    return ( true );
}

/*! Get method for the 'iPlayerTypes' member variable.
    \return the number of player types including the default player type */
int ServerSettings::getPlayerTypes( ) const
{
    return ( iPlayerTypes );
}

/*! Set method for the 'iSubsMax' member variable.

    \param i an integer value representing a new maximum number of
    substitutions allowed during a game (this value also indicates the
    maximum number of players allowed for each type)

    \return a boolean indicating whether the update was successful */
bool ServerSettings::setSubsMax( int i )
{
    iSubsMax = i;
    return ( true );
}

/*! Get method for the 'iSubsMax' member variable.

    \return the maximum number of substitutions allowed during a game
    (this value also indicates the maximum number of players allowed
    for each type) */
int ServerSettings::getSubsMax( ) const
{
    return ( iSubsMax );
}

/*! Set method for the 'dPlayerSpeedMaxDeltaMin' member variable.
    \param d a double value representing a new minimum delta for adjusting
    player_speed_max
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setPlayerSpeedMaxDeltaMin( double d )
{
    dPlayerSpeedMaxDeltaMin = d;
    return ( true );
}

/*! Get method for the 'dPlayerSpeedMaxDeltaMin' member variable.
    \return the minimum delta for adjusting player_speed_max */
double ServerSettings::getPlayerSpeedMaxDeltaMin( ) const
{
    return ( dPlayerSpeedMaxDeltaMin );
}

/*! Set method for the 'dPlayerSpeedMaxDeltaMax' member variable.
    \param d a double value representing a new maximum delta for adjusting
    player_speed_max
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setPlayerSpeedMaxDeltaMax( double d )
{
    dPlayerSpeedMaxDeltaMax = d;
    return ( true );
}

/*! Get method for the 'dPlayerSpeedMaxDeltaMax' member variable.
    \return the maximum delta for adjusting player_speed_max */
double ServerSettings::getPlayerSpeedMaxDeltaMax( ) const
{
    return ( dPlayerSpeedMaxDeltaMax );
}

/*! Set method for the 'dStaminaIncMaxDeltaFactor' member variable.
    \param d a double value representing a new amount by which delta is
    multiplied for stamina_inc_max
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setStaminaIncMaxDeltaFactor( double d )
{
    dStaminaIncMaxDeltaFactor = d;
    return ( true );
}

/*! Get method for the 'dStaminaIncMaxDeltaFactor' member variable.
    \return the amount by which delta is multiplied for stamina_inc_max */
double ServerSettings::getStaminaIncMaxDeltaFactor( ) const
{
    return ( dStaminaIncMaxDeltaFactor );
}

/*! Set method for the 'dPlayerDecayDeltaMin' member variable.
    \param d a double value representing a new minimum delta for adjusting
    player_decay
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setPlayerDecayDeltaMin( double d )
{
    dPlayerDecayDeltaMin = d;
    return ( true );
}

/*! Get method for the 'dPlayerDecayDeltaMin' member variable.
    \return the minimum delta for adjusting player_decay */
double ServerSettings::getPlayerDecayDeltaMin( ) const
{
    return ( dPlayerDecayDeltaMin );
}

/*! Set method for the 'dPlayerDecayDeltaMax' member variable.
    \param d a double value representing a new maximum delta for adjusting
    player_decay
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setPlayerDecayDeltaMax( double d )
{
    dPlayerDecayDeltaMax = d;
    return ( true );
}

/*! Get method for the 'dPlayerDecayDeltaMax' member variable.
    \return the maximum delta for adjusting player_decay */
double ServerSettings::getPlayerDecayDeltaMax( ) const
{
    return ( dPlayerDecayDeltaMax );
}

/*! Set method for the 'dInertiaMomentDeltaFactor' member variable.
    \param d a double value representing a new amount by which delta is
    multiplied for inertia_moment
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setInertiaMomentDeltaFactor( double d )
{
    dInertiaMomentDeltaFactor = d;
    return ( true );
}

/*! Get method for the 'dInertiaMomentDeltaFactor' member variable.
    \return the amount by which delta is multiplied for inertia_moment */
double ServerSettings::getInertiaMomentDeltaFactor( ) const
{
    return ( dInertiaMomentDeltaFactor );
}

/*! Set method for the 'dDashPowerRateDeltaMin' member variable.
    \param d a double value representing a new minimum delta for adjusting
    dash_power_rate
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setDashPowerRateDeltaMin( double d )
{
    dDashPowerRateDeltaMin = d;
    return ( true );
}

/*! Get method for the 'dDashPowerRateDeltaMin' member variable.
    \return the minimum delta for adjusting dash_power_rate */
double ServerSettings::getDashPowerRateDeltaMin( ) const
{
    return ( dDashPowerRateDeltaMin );
}

/*! Set method for the 'dDashPowerRateDeltaMax' member variable.
    \param d a double value representing a new maximum delta for adjusting
    dash_power_rate
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setDashPowerRateDeltaMax( double d )
{
    dDashPowerRateDeltaMax = d;
    return ( true );
}

/*! Get method for the 'dDashPowerRateDeltaMax' member variable.
    \return the maximum delta for adjusting dash_power_rate */
double ServerSettings::getDashPowerRateDeltaMax( ) const
{
    return ( dDashPowerRateDeltaMax );
}

/*! Set method for the 'dPlayerSizeDeltaFactor' member variable.
    \param d a double value representing a new amount by which delta is
    multiplied for player_size
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setPlayerSizeDeltaFactor( double d )
{
    dPlayerSizeDeltaFactor = d;
    return ( true );
}

/*! Get method for the 'dPlayerSizeDeltaFactor' member variable.
    \return the amount by which delta is multiplied for player_size */
double ServerSettings::getPlayerSizeDeltaFactor( ) const
{
    return ( dPlayerSizeDeltaFactor );
}

/*! Set method for the 'dKickableMarginDeltaMin' member variable.
    \param d a double value representing a new minimum delta for adjusting
    kickable_margin
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setKickableMarginDeltaMin( double d )
{
    dKickableMarginDeltaMin = d;
    return ( true );
}

/*! Get method for the 'dKickableMarginDeltaMin' member variable.
    \return the minimum delta for adjusting kickable_margin */
double ServerSettings::getKickableMarginDeltaMin( ) const
{
    return ( dKickableMarginDeltaMin );
}

/*! Set method for the 'dKickableMarginDeltaMax' member variable.
    \param d a double value representing a new maximum delta for adjusting
    kickable_margin
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setKickableMarginDeltaMax( double d )
{
    dKickableMarginDeltaMax = d;
    return ( true );
}

/*! Get method for the 'dKickableMarginDeltaMax' member variable.
    \return the maximum delta for adjusting kickable_margin */
double ServerSettings::getKickableMarginDeltaMax( ) const
{
    return ( dKickableMarginDeltaMax );
}

/*! Set method for the 'dKickRandDeltaFactor' member variable.
    \param d a double value representing a new amount by which delta is
    multiplied for kick_rand
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setKickRandDeltaFactor( double d )
{
    dKickRandDeltaFactor = d;
    return ( true );
}

/*! Get method for the 'dKickRandDeltaFactor' member variable.
    \return the amount by which delta is multiplied for kick_rand */
double ServerSettings::getKickRandDeltaFactor( ) const
{
    return ( dKickRandDeltaFactor );
}

/*! Set method for the 'dExtraStaminaDeltaMin' member variable.
    \param d a double value representing a new minimum delta for adjusting
    extra_stamina
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setExtraStaminaDeltaMin( double d )
{
    dExtraStaminaDeltaMin = d;
    return ( true );
}

/*! Get method for the 'dExtraStaminaDeltaMin' member variable.
    \return the minimum delta for adjusting extra_stamina */
double ServerSettings::getExtraStaminaDeltaMin( ) const
{
    return ( dExtraStaminaDeltaMin );
}

/*! Set method for the 'dExtraStaminaDeltaMax' member variable.
    \param d a double value representing a new maximum delta for adjusting
    extra_stamina
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setExtraStaminaDeltaMax( double d )
{
    dExtraStaminaDeltaMax = d;
    return ( true );
}

/*! Get method for the 'dExtraStaminaDeltaMax' member variable.
    \return the maximum delta for adjusting extra_stamina */
double ServerSettings::getExtraStaminaDeltaMax( ) const
{
    return ( dExtraStaminaDeltaMax );
}

/*! Set method for the 'dEffortMaxDeltaFactor' member variable.
    \param d a double value representing a new amount by which delta is
    multiplied for effort_max
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setEffortMaxDeltaFactor( double d )
{
    dEffortMaxDeltaFactor = d;
    return ( true );
}

/*! Get method for the 'dEffortMaxDeltaFactor' member variable.
    \return the amount by which delta is multiplied for effort_max */
double ServerSettings::getEffortMaxDeltaFactor( ) const
{
    return ( dEffortMaxDeltaFactor );
}

/*! Set method for the 'dEffortMinDeltaFactor' member variable.
    \param d a double value representing a new amount by which delta is
    multiplied for effort_min
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setEffortMinDeltaFactor( double d )
{
    dEffortMinDeltaFactor = d;
    return ( true );
}

/*! Get method for the 'dEffortMinDeltaFactor' member variable.
    \return the amount by which delta is multiplied for effort_min */
double ServerSettings::getEffortMinDeltaFactor( ) const
{
    return ( dEffortMinDeltaFactor );
}

/*! Set method for the 'dDashPowerRateDeltaMin' member variable.
    \param d a double value representing a new minimum delta for adjusting
    dash_power_rate
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setNewDashPowerRateDeltaMin( double d )
{
    dNewDashPowerRateDeltaMin = d;
    return ( true );
}

/*! Get method for the 'dNewDashPowerRateDeltaMin' member variable.
    \return the minimum delta for adjusting dash_power_rate */
double ServerSettings::getNewDashPowerRateDeltaMin( ) const
{
    return ( dNewDashPowerRateDeltaMin );
}

/*! Set method for the 'dNewDashPowerRateDeltaMax' member variable.
    \param d a double value representing a new maximum delta for adjusting
    dash_power_rate
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setNewDashPowerRateDeltaMax( double d )
{
    dNewDashPowerRateDeltaMax = d;
    return ( true );
}

/*! Get method for the 'dNewDashPowerRateDeltaMax' member variable.
    \return the maximum delta for adjusting dash_power_rate */
double ServerSettings::getNewDashPowerRateDeltaMax( ) const
{
    return ( dNewDashPowerRateDeltaMax );
}

/*! Set method for the 'dNewStaminaIncMaxDeltaFactor' member variable.
    \param d a double value representing a new amount by which delta is
    multiplied for stamina_inc_max
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setNewStaminaIncMaxDeltaFactor( double d )
{
    dNewStaminaIncMaxDeltaFactor = d;
    return ( true );
}

/*! Get method for the 'dNewStaminaIncMaxDeltaFactor' member variable.
    \return the amount by which delta is multiplied for stamina_inc_max */
double ServerSettings::getNewStaminaIncMaxDeltaFactor( ) const
{
    return ( dNewStaminaIncMaxDeltaFactor );
}

/*! Set method for the 'dPenDistX' variable that indicates where the ball will
    be positioned in front of the goalline in case of a penalty kick. */
bool ServerSettings::setPenDistX( double d )
{
    dPenDistX = d;
    return true;
}

/*! Get method for the 'dPenDistX' variable that indicates where the ball will
    be positioned in front of the goalline in case of a penalty kick. */
double ServerSettings::getPenDistX( ) const
{
    return dPenDistX;
}

/*! Set method for the 'dPenMaxGoalieDistX' variable that indicates
    how far the goalie is allowed to move outside its goal in case of a
    penalty kick. */
bool ServerSettings::setPenMaxGoalieDistX( double d )
{
    dPenMaxGoalieDistX = d;
    return true;
}

/*! Get method for the 'dPenMaxGoalieDistX' variable that indicates
    how far the goalie is allowed to move outside its goal in case of a
    penalty kick. */
double ServerSettings::getPenMaxGoalieDistX( ) const
{
    return dPenMaxGoalieDistX;
}

/*! Set method for the 'bPenAllowMultKicks' variable that indicates whether the
    penalty kicker is allowed to kick the ball multiple times. */
bool ServerSettings::setPenAllowMultKicks( bool b )
{
    bPenAllowMultKicks = b;
    return true;
}

/*! Set method for the 'bPenAllowMultKicks' variable that indicates whether the
    penalty kicker is allowed to kick the ball multiple times. */
bool ServerSettings::getPenAllowMultKicks( ) const
{
    return bPenAllowMultKicks;
}

/*! Set method for the 'dTackleDist' variable that indicates the maximum x
    distance to the front of the agent in which a tackle can be performed. The
    probabiltiy of succeeding depends on this distance. */
bool ServerSettings::setTackleDist( double d )
{
    dTackleDist = d;
    return true;
}

/*! Get method for the 'dTackleDist' variable that indicates the maximum x
    distance to the front of the agent in which a tackle can be performed. The
    probabiltiy of succeeding depends on this distance. */
double ServerSettings::getTackleDist( ) const
{
    return dTackleDist;
}

/*! Set method for the 'dTackleBackDist' variable that indicates the maximum x
    distance to the back of the agent in which a tackle can be performed. The
    probabiltiy of succeeding depends on this distance. */
bool ServerSettings::setTackleBackDist( double d )
{
    dTackleBackDist = d;
    return true;
}

/*! Get method for the 'dTackleBackDist' variable that indicates the maximum x
    distance to the back of the agent in which a tackle can be performed. The
    probabiltiy of succeeding depends on this distance. */
double ServerSettings::getTackleBackDist( ) const
{
    return dTackleBackDist;
}

/*! Set method for the 'dTackleWidth' variable that indicates the maximum y
    distance to the agent in which a tackle can be performed. The
    probabiltiy of succeeding depends on this distance. */
bool ServerSettings::setTackleWidth( double d )
{
    dTackleWidth = d;
    return true;
}

/*! Get method for the 'dTackleWidth' variable that indicates the maximum y
    distance to the agent in which a tackle can be performed. The
    probabiltiy of succeeding depends on this distance. */
double ServerSettings::getTackleWidth( ) const
{
    return dTackleWidth;
}

/*! Set method for the 'dTackleExponent' variable that indicates the exponent
    used in order to calculate the probability that the tackle will succeed. */
bool ServerSettings::setTackleExponent( double d )
{
    dTackleExponent = d;
    return true;
}

/*! Get method for the 'dTackleExponent' variable that indicates the exponent
    used in order to calculate the probability that the tackle will succeed. */
double ServerSettings::getTackleExponent( ) const
{
    return dTackleExponent;
}

/*! Set method for the 'dTackleCycles' variable that indicates how many cycles
    a player is immobile after performing a tackle. */
bool ServerSettings::setTackleCycles( int i )
{
    iTackleCycles = i;
    return true;
}

/*! Get method for the 'dTackleCycles' variable that indicates how many cycles
    a player is immobile after performing a tackle. */
int ServerSettings::getTackleCycles( ) const
{
    return iTackleCycles;
}

/*! Set method for the 'dTacklePowerRate' variable that indicates with which
    power the ball is accelerated after a tackle. */
bool ServerSettings::setTacklePowerRate( double d )
{
    dTacklePowerRate = d;
    return true;
}

/*! Get method for the 'dTacklePowerRate' variable that indicates with which
    power the ball is accelerated after a tackle. */
double ServerSettings::getTacklePowerRate( ) const
{
    return dTacklePowerRate;
}

/*! Set method for the 'dEffortMax' member variable.
    \param d a double value representing a new maximum player effort capacity
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setEffortMax( double d )
{
    dEffortMax = d;
    return ( true );
}

/*! Get method for the 'dEffortMax' member variable.
    \return the maximum player effort capacity */
double ServerSettings::getEffortMax(  ) const
{
    return ( dEffortMax );
}

/*! Set method for the 'iSlowDownFactor' member variable.
    \param i an integer value representing a new factor to slow down the
    simulator and send intervals
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setSlowDownFactor( int i )
{
    iSlowDownFactor = i;
    return ( true );
}

/*! Get method for the 'iSlowDownFactor' member variable.
    \return the factor to slow down the simulator and send intervals */
int ServerSettings::getSlowDownFactor( ) const
{
    return ( iSlowDownFactor );
}

/*! Set method for the 'dVisibleDistance' member variable.

    \param d a double value representing a new distance within which
    objects are always 'visible' (even when not in view cone)

    \return a boolean indicating whether the update was successful */
bool ServerSettings::setVisibleDistance( double d )
{
    dVisibleDistance = d;
    return ( true );
}

/*! Get method for the 'dVisibleDistance' member variable.
    \return the distance within which objects are always 'visible' (even when
    not in view cone) */
double ServerSettings::getVisibleDistance( ) const
{
    return ( dVisibleDistance );
}

/*! Set method for the 'dExtraStamina' member variable.
    \param d a double value representing a new amount of extra stamina that can
    be added to the maximum stamina of a player. This value depends on the type
    of heterogeneous player.
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setExtraStamina( double d )
{
    dExtraStamina = d;
    return ( true );
}

/*! Get method for the 'dExtraStamina' member variable.
    \return the amount of extra stamina that can be added to the maximum
    stamina of a player. This value depends on the type of heterogeneous
    player. */
double ServerSettings::getExtraStamina( ) const
{
    return ( dExtraStamina );
}

/*! Set method for the 'dMaximalKickDist' member variable.
    \param d a double value representing a new maximum distance from a player
    for which the ball is still kickable
    \return a boolean indicating whether the update was successful */
bool ServerSettings::setMaximalKickDist( double d )
{
    dMaximalKickDist = d;
    return ( true );
}

/*! Get method for the 'dMaximalKickDist' member variable.
    \return the maximum distance from a player for which the ball is still
    kickable */
double ServerSettings::getMaximalKickDist( ) const
{
    return ( dMaximalKickDist );
}

/*****************************************************************************/
/*******************   CLASS HETEROPLAYERSETTINGS ****************************/
/*****************************************************************************/

/*! This method prints all the values of this heterogeneous player type to
    the supplied output.
    \param os output stream to which values are printed. */
void HeteroPlayerSettings::show( ostream &os )
{
    os.setf( ios::left );
    os << dPlayerSpeedMax << " ";
    os.width( 5 );
    os.precision( 4 );
    os.setf( ios::left );
    os << dStaminaIncMax << " " ;
    os.width( 6 );
    os.precision( 4 );
    os.setf( ios::left );
    os << dPlayerDecay  << " ";
    os.width( 5 );
    os.precision( 4 );
    os.setf( ios::left );
    os << dInertiaMoment  << " " ;
    os.width( 8 );
    os.precision( 4 );
    os.setf( ios::left );
    os << dDashPowerRate << " " ;
    os.width( 3 );
    os.precision( 4 );
    os.setf( ios::left );
    os << dPlayerSize     << " " ;
    os.width( 6 );
    os.precision( 4 );
    os.setf( ios::left );
    os << dKickableMargin << " " ;
    os.width( 6 );
    os.precision( 4 );
    os.setf( ios::left );
    os << dKickRand      << " " ;
    os.width( 5 );
    os.precision( 3 );
    os.setf( ios::left );
    os << dExtraStamina    << " " ;
    os.width( 6 );
    os.precision( 4 );
    os.setf( ios::left );
    os << dEffortMin     << " " ;
    os.width( 6 );
    os.setf( ios::left );
    os.precision( 4 );
    os << dEffortMax      << endl ;
    /*
      os <<        dPlayerSpeedMax << " " << dStaminaIncMax << " " << dPlayerDecay
         << " " << dInertiaMoment  << " " << dDashPowerRate << " " << dPlayerSize
         << " " << dKickableMargin << " " << dKickRand      << " " << dExtraStamina
         << " " << dEffortMax      << " " << dEffortMin     << endl;
    */
}


/********************* TESTING PURPOSES **************************************/
/*

int main( void )
{
  ServerSettings settings;
//  settings.show( cout );
  settings.readValues( "server.conf", ":" );
//  printf( "------------\n" );
  settings.show( cout );
  cout << "player_size: " << settings.getPlayerSize( ) << endl;
}

*/
