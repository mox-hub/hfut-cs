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

/*! \file ServerSettings.h
<pre>
<b>File:</b>          ServerSettings.h
<b>Project:</b>       Robocup Soccer Simulation Team: UvA Trilearn
<b>Authors:</b>       Jelle Kok
<b>Created:</b>       28/11/2000
<b>Last Revision:</b> $ID$
<b>Contents:</b>      Header file for class ServerSettings. It contains all the
               member data and member method declarations of the ServerSettings
               class. This class contains all the Soccerserver parameters that
               are available in the configuration files 'server.conf' and
               'player.conf' along with their default values and standard set-
               and get methods for manipulating these values. This file also
               contains the definition of the HeteroPlayerSettings class which
               contains all the SoccerServer parameters which together define a
               heterogeneous player type.</pre>
<hr size=2>
<pre>
<h2><b>Changes</b></h2>
<b>Date</b>             <b>Author</b>          <b>Comment</b>
28/11/2000       Jelle Kok       Initial version  (based on Emiel Corten)
31/01/2001       Remco de Boer   Settings for server version 7.xx added
27/04/2001       Remco de Boer   add drop_ball_time and player.conf parameters
01/05/2001       Remco de Boer   Version including full documentation completed
07/05/2001       Jelle Kok        HeteroPlayerSettings class and dExtraStamina
08/05/2001       Remco de Boer   Documentation updated
</pre>
*/

#ifndef _SERVERSETTINGS_
#define _SERVERSETTINGS_

#include "GenericValues.h"

/*****************************************************************************/
/*******************   CLASS SERVERSETTINGS   ********************************/
/*****************************************************************************/

/*! This class contains all the Soccerserver parameters that are available in
    the configuration file 'server.conf' along with their default values and
    standard set- and get methods for manipulating these values. The
    ServerSettings class is a subclass of the GenericValues class and therefore
    each value in this class can be reached through the string name of the
    corresponding parameter. */
class ServerSettings:public GenericValues
{
private:
    // private member data

    // all the parameters available in server.conf
    // NOTE: names in server.conf corresponding with member variables
    //       are listed between doxygen-tags to enable quick searching

    // goal-related parameters
    double dGoalWidth;        /*!< goal_width: the width of the goal           */

    // player-related parameters
    double dPlayerSize;       /*!< player_size: the size (=radius) of a player */
    double dPlayerDecay;      /*!< player_decay: player speed decay per cycle  */
    double dPlayerRand;       /*!< player_rand: random error in player movement*/
    double dPlayerWeight;     /*!< player_weight: weight of a player (for wind)*/
    double dPlayerSpeedMax;   /*!< player_speed_max: maximum speed of a player */
    double dPlayerAccelMax;   /*!< player_accel_max: maximum acceleration of a
                                 player per cycle                            */

    // stamina-related parameters
    double dStaminaMax;       /*!< stamina_max: maximum stamina of a player    */
    double dStaminaIncMax;    /*!< stamina_inc_max: maximum stamina increase of a
                                 player per cycle                            */
    double dRecoverDecThr;    /*!< recover_dec_thr: percentage of stamina_max
                                 below which player recovery decreases       */
    double dRecoverDec;       /*!< recover_dec: decrement step per cycle for
                                 player recovery                             */
    double dRecoverMin;       /*!< recover_min: minimum player recovery        */
    double dEffortDecThr;     /*!< effort_dec_thr: percentage of stamina_max
                                 below which player effort capacity decreases*/
    double dEffortDec;        /*!< effort_dec: decrement step per cycle for
                                 player effort capacity                      */
    double dEffortIncThr;     /*!< effort_incr_thr: percentage of stamina_max
                                 above which player effort capacity increases*/
    double dEffortInc;        /*!< effort_inc: increment step per cycle for
                                 player effort capacity                      */
    double dEffortMin;        /*!< effort_min: minimum value for player effort */
    double dEffortMax;        /*!< effort_max: maximum player effort capacity  */


    // parameters related to auditory perception
    int    iHearMax;          /*!< hear_max: maximum hearing capacity of a plyer
                                 a player can hear hear_inc messages in
                                 hear_decay simulation cycles                */
    int    iHearInc;          /*!< hear_inc: minimum hearing capacity of a player
                                 i.e. the number of messages a player can hear
                                 in hear_decay simulation cycles             */
    int    iHearDecay;        /*!< hear_decay: decay rate of player hearing
                                 capacity, i.e. minimum number of cycles for
                                 hear_inc messages                           */

    // parameters related to player turn actions
    double dInertiaMoment;    /*!< inertia_moment: inertia moment of a player;
                                 affects actual turn angle depending on speed*/

    // parameters related to sense_body information
    int    iSenseBodyStep;    /*!< sense_body_step: length of the interval (ms)
                                 between sense_body information messages     */

    // goalkeeper-related parameters
    double dCatchableAreaL;   /*!< catchable_area_l: length of area around
                                 goalkeeper in which he can catch the ball   */
    double dCatchableAreaW;   /*!< catchable_area_w: width of area around
                                 goalkeeper in which he can catch the ball   */
    double dCatchProbability; /*!< catch_probability: the probability for a
                                 goalkeeper to catch the ball                */
    int    iCatchBanCycle;    /*!< catch_ban_cycle: number of cycles after catch
                                 in which goalkeeper cannot catch again      */
    int    iGoalieMaxMoves;   /*!< goalie_max_moves: maximum number of 'move'
                                 actions allowed for goalkeeper after catch  */

    // ball-related parameters
    double dBallSize;         /*!< ball_size: the size (=radius) of the ball   */
    double dBallDecay;        /*!< ball_decay: ball speed decay per cycle      */
    double dBallRand;         /*!< ball_rand: random error in ball movement    */
    double dBallWeight;       /*!< ball_weight: weight of the ball (for wind)  */
    double dBallSpeedMax;     /*!< ball_speed_max: maximum speed of the ball   */
    double dBallAccelMax;     /*!< ball_accel_max: maximum acceleration of the
                                 ball per cycle                              */

    // wind-related parameters
    double dWindForce;        /*!< wind_force: the force of the wind           */
    double dWindDir;          /*!< wind_dir: the direction of the wind         */
    double dWindRand;         /*!< wind_rand: random error in wind direction   */
    bool   bWindRandom;       /*!< wind_random: random wind force and direction*/

    // parameters related to 'dash' and 'kick' commands
    double dKickableMargin;   /*!< kickable_margin: margin around player in which
                                 ball is kickable; kickable area thus equals
                                 kickable_margin + ball_size + player_size   */
    double dCkickMargin;      /*!< ckick_margin: corner kick margin, i.e. the
                                 minimum distance to the ball for offending
                                 players when a corner kick is taken         */
    double dDashPowerRate;    /*!< dash_power_rate: rate by which the 'Power'
                                 argument in a 'dash' command is multiplied
                                 (thus determining the amount of displacement
                                 of the player as a result of the 'dash')    */
    double dKickPowerRate;    /*!< kick_power_rate: rate by which the 'Power'
                                 argument in a 'kick' command is multiplied
                                 (thus determining the amount of displacement
                                 of the ball as a result of the 'kick')      */
    double dKickRand;         /*!< kick_rand: random error in kick direction   */

    // parameters related to visual and auditory perception range
    double dVisibleAngle;     /*!< visible_angle: angle of the view cone of a
                                 player in the standard view mode            */
    double dAudioCutDist;     /*!< audio_cut_dist: maximum distance over which a
                                 spoken message can be heard                 */

    // quantization parameters
    double dQuantizeStep;     /*!< quantize_step: quantization step for distance
                                 of moving objects                           */
    double dQuantizeStepL;    /*!< quantize_step_l: quantization step for
                                 distance of landmarks                       */

    // range parameters for basic actuator commands
    int    iMaxPower;         /*!< maxpower: maximum power for dash/kick       */
    int    iMinPower;         /*!< minpower: minimum power for dash/kick       */
    int    iMaxMoment;        /*!< maxmoment: maximum angle for turn/kick      */
    int    iMinMoment;        /*!< minmoment: minimum angle for turn/kick      */
    int    iMaxNeckMoment;    /*!< maxneckmoment: maximum angle for turnneck   */
    int    iMinNeckMoment;    /*!< minneckmoment: minimum angle for turnneck   */
    int    iMaxNeckAng;       /*!< maxneckang: maximum neck angle rel. to body */
    int    iMinNeckAng;       /*!< minneckang: minimum neck angle rel. to body */

    // port-related parameters
    int    iPort;             /*!< port: port number for player connection     */
    int    iCoachPort;        /*!< coach_port: port number for coach connection*/
    int    iOlCoachPort;      /*!< ol_coach_port: port number for online coach */

    // coach-related parameters
    int    iSayCoachCntMax;   /*!< say_coach_cnt_max: maximum number of coach
                                 messages possible                           */
    int    iSayCoachMsgSize;  /*!< say_coach_msg_size: maximum size of coach
                                 messages                                    */
    int    iClangWinSize;     /*!< clang_win_size: time window which controls how
                                 many coach messages can be sent             */
    int    iClangDefineWin;   /*!< clang_define_win: number of define messages by
                                 coach per time window                       */
    int    iClangMetaWin;     /*!< clang_meta_win: number of meta messages by
                                 coach per time window                       */
    int    iClangAdviceWin;   /*!< clang_advice_win: number of advice messages by
                                 coach per time window                       */
    int    iClangInfoWin;     /*!< clang_info_win: number of info messages by
                                 coach per time window                       */
    int    iClangMessDelay;   /*!< clang_mess_delay: delay of coach messages, ie
                                 the number of cycles between send to player
                                 and receival of message                     */
    int    iClangMessPerCycle;/*!< clang_mess_per_cycle: number of coach messages
                                 per cycle                                   */
    int    iSendViStep;       /*!< send_vi_step: interval of coach's look, i.e.
                                 the length of the interval (in ms) between
                                 visual messages to the coach                */

    // time-related parameters
    int    iSimulatorStep;    /*!< simulator_step: the length (in ms) of a
                                 simulator cycle                             */
    int    iSendStep;         /*!< send_step: the length of the interval (in ms)
                                 between visual messages to a player in the
                                 standard view mode                          */
    int    iRecvStep;         /*!< recv_step: the length of the interval (in ms)
                                 for accepting commands from a player        */
    int    iHalfTime;         /*!< half_time: the length (in seconds) of a single
                                 game half                                   */
    int    iDropBallTime;     /*!< drop_ball_time: the number of cycles to wait
                                 until dropping the ball automatically for free
                                 kicks, corner kicks, etc.                   */

    // speech-related parameters
    int    iSayMsgSize;       /*!< say_msg_size: the maximum length (in bytes) of
                                 a spoken message                            */

    // offside-related parameters
    bool   bUseOffside;       /*!< use_offside: a boolean flag indicating whether
                                 the offside rule should be applied or not   */
    double dOffsideActiveAreaSize;/*!< offside_active_area_size: offside active
                                     area size, i.e. radius of circle around
                                     the ball in which player can be offside */
    bool   bForbidKickOffOffside; /*!< forbid_kick_off_offside: a boolean flag
                                     indicating whether a kick from offside
                                     position is allowed                     */
    double dOffsideKickMargin;/*!< offside_kick_margin: offside kick margin,
                                 i.e. the minimum distance to the ball for
                                 offending players when a free kick for
                                 offside is taken                            */

    // log-related parameters
    bool   bVerbose;          /*!< verbose: flag indicating whether verbose mode
                                 is active or not; in verbose mode server sends
                                 extra error-information                     */
    int    iRecordVersion;    /*!< record_version: the type of log record      */
    bool   bRecordLog;        /*!< record_log: flag indicating whether log record
                                 for game should be created                  */
    bool   bSendLog;          /*!< send_log: flag indicating whether send client
                                 command log for game should be created      */
    bool   bLogTimes;         /*!< log_times: flag indicating whether ms should
                                 be written between cycles in log file       */
    char   strLogFile[ 256 ]; /*!< server log to store all actions received    */
    bool   bSynchMode;        /*!< synch_mode: indicates whether in sync. mode */
    bool   bFullStateL;       /*!< fullstate_l: indicates full_state left team.*/
    bool   bFullStateR;       /*!< fullstate_r: indicates full_state right team*/

    // all the parameters available in player.conf (for heterogeneous players)
    // NOTE: names in player.conf corresponding with member variables
    //       are listed between doxygen-tags to enable quick searching

    int    iPlayerTypes;             /*!< player_types: the number of player
                                        types including the default types    */
    int    iSubsMax;                 /*!< subs_max: the maximum number of
                                        substitutions allowed during a game;
                                        the value also indicates the maximum
                                        number of players allowed for each
                                        type                                 */
    double dPlayerSpeedMaxDeltaMin;  /*!< player_speed_max_delta_min: minimum
                                        delta for adjusting player_speed_max */
    double dPlayerSpeedMaxDeltaMax;  /*!< player_speed_max_delta_max: maximum
                                        delta for adjusting player_speed_max */
    double dStaminaIncMaxDeltaFactor;/*!< stamina_inc_max_delta_factor: amount by
                                        which delta is multiplied for
                                        stamina_inc_max                      */
    double dPlayerDecayDeltaMin;     /*!< player_decay_delta_min: minimum delta
                                        for adjusting player_decay           */
    double dPlayerDecayDeltaMax;     /*!< player_decay_delta_max: maximum delta
                                        for adjusting player_decay           */
    double dInertiaMomentDeltaFactor;/*!< inertia_moment_delta_factor: amount by
                                        which delta is multiplied for
                                        inertia_moment                       */
    double dDashPowerRateDeltaMin;   /*!< dash_power_rate_delta_min: minimum
                                        delta for adjusting dash_power_rate  */
    double dDashPowerRateDeltaMax;   /*!< dash_power_rate_delta_max: maximum
                                        delta for adjusting dash_power_rate  */
    double dPlayerSizeDeltaFactor;   /*!< player_size_delta_factor: amount delta
                                        is multiplied by for player_size     */
    double dKickableMarginDeltaMin;  /*!< kickable_margin_delta_min: minimum
                                        delta for adjusting kickable_margin  */
    double dKickableMarginDeltaMax;  /*!< kickable_margin_delta_max: maximum
                                        delta for adjusting kickable_margin  */
    double dKickRandDeltaFactor;     /*!< kick_rand_delta_factor: amount delta is
                                        multiplied by for kick_rand          */
    double dExtraStaminaDeltaMin;    /*!< extra_stamina_delta_min: minimum delta
                                        for adjusting extra_stamina          */
    double dExtraStaminaDeltaMax;    /*!< extra_stamina_delta_max: maximum delta
                                        for adjusting extra_stamina          */
    double dEffortMaxDeltaFactor;    /*!< effort_max_delta_factor: amount delta
                                        is multiplied by for effort_max      */
    double dEffortMinDeltaFactor;    /*!< effort_min_delta_factor: amount delta
                                        is multiplied by for effort_min      */
    double dNewDashPowerRateDeltaMin;/*!< new_dash_power_rate_delta_min: minimum
	                                delta for adjusting dash_power_rate,
					used from server 8.05                */
    double dNewDashPowerRateDeltaMax;/*!< new_dash_power_rate_delta_min: maximum
                                        delta for adjusting dash_power_rate,
                                        used from server 8.05                */
    double dNewStaminaIncMaxDeltaFactor;/*!< stamina_inc_max_delta_factor: amount
                                        which delta is multiplied for
                                        stamina_inc_max, used from server 8  */

    // other parameters
    int    iSlowDownFactor;          /*!< slow_down_factor: factor to slow down
                                        simulator and send intervals         */
    double dVisibleDistance;         /*!< visible_distance: distance within which
                                        objects are always 'visible' (even when
                                        not in view cone)                    */
    double dExtraStamina;            /*!< extra_stamina: extra stamina for a
                                        heterogeneous player                 */

    // penalty parameters
    double dPenDistX;                /*!< pen_dist_x: x distance for ball from
                                        goalline.                            */
    double dPenMaxGoalieDistX;       /*!< pen_max_goalie_dist_x: max goalie
                                        distance before the goalline.        */
    bool   bPenAllowMultKicks;       /*!< pen_allow_mult_kicks: allow multiple
                                        kicks by the penalty kicker          */

    // tackle parameters
    double dTackleDist;              /*!< tackle_dist: distance in front player
                                        where tackle is possible.            */
    double dTackleBackDist;          /*!< tackle_dist: distance at back of  player
                                        where tackle is possible.            */
    double dTackleWidth;             /*!< tackle_width: distance to side of player
                                        where tackle is possible.            */
    double dTackleExponent;          /*!< tackle_exponent: exponent need to
                                        calculate prob.of success tackle     */
    int    iTackleCycles;            /*!< tackle_cycles: cycles immobile after
                                        tackle                               */
    double dTacklePowerRate;         /*!< tacke_power_rate: acc. power tackle  */

    // parameters which depend on other values
    double dMaximalKickDist;         /*!< the maximum distance from a player for
                                        which the ball is still kickable     */

    // public methods
public:
    // constructor for ServerSettings class
    ServerSettings( );

    // methods 'setValue' and 'readValues' from superclass
    // GenericValues are overridden in this subclass
    bool setValue  ( const char *strName    , const char *strValue       );
    bool readValues( const char *strFilename, const char *Separator      );

    // set- and get methods for private member variables

    // set- and get methods for goal-related parameters
    bool   setGoalWidth                  ( double d    );
    double getGoalWidth                  (             ) const;

    // set- and get methods for player-related parameters
    bool   setPlayerSize                 ( double d    );
    double getPlayerSize                 (             ) const;
    bool   setPlayerDecay                ( double d    );
    double getPlayerDecay                (             ) const;
    bool   setPlayerRand                 ( double d    );
    double getPlayerRand                 (             ) const;
    bool   setPlayerWeight               ( double d    );
    double getPlayerWeight               (             ) const;
    bool   setPlayerSpeedMax             ( double d    );
    double getPlayerSpeedMax             (             ) const;
    bool   setPlayerAccelMax             ( double d    );
    double getPlayerAccelMax             (             ) const;

    // set- and get methods for stamina-related parameters
    bool   setStaminaMax                 ( double d    );
    double getStaminaMax                 (             ) const;
    bool   setStaminaIncMax              ( double d    );
    double getStaminaIncMax              (             ) const;
    bool   setRecoverDecThr              ( double d    );
    double getRecoverDecThr              (             ) const;
    bool   setRecoverDec                 ( double d    );
    double getRecoverDec                 (             ) const;
    bool   setRecoverMin                 ( double d    );
    double getRecoverMin                 (             ) const;
    bool   setEffortDecThr               ( double d    );
    double getEffortDecThr               (             ) const;
    bool   setEffortDec                  ( double d    );
    double getEffortDec                  (             ) const;
    bool   setEffortIncThr               ( double d    );
    double getEffortIncThr               (             ) const;
    bool   setEffortInc                  ( double d    );
    double getEffortInc                  (             ) const;
    bool   setEffortMin                  ( double d    );
    double getEffortMin                  (             ) const;

    // set- and get methods for parameters related to auditory perception
    bool   setHearMax                    ( int i       );
    int    getHearMax                    (             ) const;
    bool   setHearInc                    ( int i       );
    int    getHearInc                    (             ) const;
    bool   setHearDecay                  ( int i       );
    int    getHearDecay                  (             ) const;

    // set- and get methods for parameters related to player turn actions
    bool   setInertiaMoment              ( double d    );
    double getInertiaMoment              (             ) const;

    // set- and get methods for parameters related to sense_body information
    bool   setSenseBodyStep              ( int i       )      ;
    int    getSenseBodyStep              (             ) const;

    // set- and get methods for goalkeeper-related parameters
    bool   setCatchableAreaL             ( double d    );
    double getCatchableAreaL             (             ) const;
    bool   setCatchableAreaW             ( double d    );
    double getCatchableAreaW             (             ) const;
    bool   setCatchProbability           ( double d    );
    double getCatchProbability           (             ) const;
    bool   setCatchBanCycle              ( int    i    )      ;
    int    getCatchBanCycle              (             ) const;
    bool   setGoalieMaxMoves             ( int    i    )      ;
    int    getGoalieMaxMoves             (             ) const;

    // set- and get methods for ball-related parameters
    bool   setBallSize                   ( double d    );
    double getBallSize                   (             ) const;
    bool   setBallDecay                  ( double d    );
    double getBallDecay                  (             ) const;
    bool   setBallRand                   ( double d    );
    double getBallRand                   (             ) const;
    bool   setBallWeight                 ( double d    );
    double getBallWeight                 (             ) const;
    bool   setBallSpeedMax               ( double d    );
    double getBallSpeedMax               (             ) const;
    bool   setBallAccelMax               ( double d    );
    double getBallAccelMax               (             ) const;

    // set- and get methods for wind-related parameters
    bool   setWindForce                  ( double d    );
    double getWindForce                  (             ) const;
    bool   setWindDir                    ( double d    );
    double getWindDir                    (             ) const;
    bool   setWindRand                   ( double d    );
    double getWindRand                   (             ) const;
    bool   setWindRandom                 ( bool   b    );
    bool   getWindRandom                 (             ) const;

    // set- and get methods for parameters related to 'dash' and 'kick' commands
    bool   setKickableMargin             ( double d    );
    double getKickableMargin             (             ) const;
    bool   setCkickMargin                ( double d    );
    double getCkickMargin                (             ) const;
    bool   setDashPowerRate              ( double d    );
    double getDashPowerRate              (             ) const;
    bool   setKickPowerRate              ( double d    );
    double getKickPowerRate              (             ) const;
    bool   setKickRand                   ( double d    );
    double getKickRand                   (             ) const;

    // set- and get methods for parameters related
    // to visual and auditory perception range
    bool   setVisibleAngle               ( double d    );
    double getVisibleAngle               (             ) const;
    bool   setAudioCutDist               ( double d    );
    double getAudioCutDist               (             ) const;

    // set- and get methods for quantization parameters
    bool   setQuantizeStep               ( double d    );
    double getQuantizeStep               (             ) const;
    bool   setQuantizeStepL              ( double d    );
    double getQuantizeStepL              (             ) const;

    // set- and get methods for range parameters for basic actuator commands
    bool setMaxPower                     ( int    i    )      ;
    int  getMaxPower                     (             ) const;
    bool setMinPower                     ( int    i    )      ;
    int  getMinPower                     (             ) const;
    bool setMaxMoment                    ( int    i    )      ;
    int  getMaxMoment                    (             ) const;
    bool setMinMoment                    ( int    i    )      ;
    int  getMinMoment                    (             ) const;
    bool setMaxNeckMoment                ( int    i    )      ;
    int  getMaxNeckMoment                (             ) const;
    bool setMinNeckMoment                ( int    i    )      ;
    int  getMinNeckMoment                (             ) const;
    bool setMaxNeckAng                   ( int    i    )      ;
    int  getMaxNeckAng                   (             ) const;
    bool setMinNeckAng                   ( int    i    )      ;
    int  getMinNeckAng                   (             ) const;

    // set- and get methods for port-related parameters
    bool setPort                         ( int    i    )      ;
    int  getPort                         (             ) const;
    bool setCoachPort                    ( int    i    )      ;
    int  getCoachPort                    (             ) const;
    bool setOlCoachPort                  ( int    i    )      ;
    int  getOlCoachPort                  (             ) const;

    // set- and get methods for coach-related parameters
    bool setSayCoachCntMax               ( int    i    )      ;
    int  getSayCoachCntMax               (             ) const;
    bool setSayCoachMsgSize              ( int    i    )      ;
    int  getSayCoachMsgSize              (             ) const;
    bool setClangWinSize                 ( int    i    )      ;
    int  getClangWinSize                 (             ) const;
    bool setClangDefineWin               ( int    i    )      ;
    int  getClangDefineWin               (             ) const;
    bool setClangMetaWin                 ( int    i    )      ;
    int  getClangMetaWin                 (             ) const;
    bool setClangAdviceWin               ( int    i    )      ;
    int  getClangAdviceWin               (             ) const;
    bool setClangInfoWin                 ( int    i    )      ;
    int  getClangInfoWin                 (             ) const;
    bool setClangMessDelay               ( int    i    )      ;
    int  getClangMessDelay               (             ) const;
    bool setClangMessPerCycle            ( int    i    )      ;
    int  getClangMessPerCycle            (             ) const;
    bool setSendViStep                   ( int    i    )      ;
    int  getSendViStep                   (             ) const;

    // set- and get methods for time-related parameters
    bool   setSimulatorStep              ( int    i    )      ;
    int    getSimulatorStep              (             ) const;
    bool   setSendStep                   ( int    i    )      ;
    int    getSendStep                   (             ) const;
    bool   setRecvStep                   ( int    i    )      ;
    int    getRecvStep                   (             ) const;
    bool   setHalfTime                   ( int    i    )      ;
    int    getHalfTime                   (             ) const;
    bool   setDropBallTime               ( int    i    )      ;
    int    getDropBallTime               (             ) const;

    // set- and get methods for speech-related parameters
    bool   setSayMsgSize                 ( int    i    )      ;
    int    getSayMsgSize                 (             ) const;

    // set- and get methods for offside-related parameters
    bool   setUseOffside                 ( bool   b    )      ;
    bool   getUseOffside                 (             ) const;
    bool   setOffsideActiveAreaSize      ( double d    )      ;
    double getOffsideActiveAreaSize      (             ) const;
    bool   setForbidKickOffOffside       ( bool   b    )      ;
    bool   getForbidKickOffOffside       (             ) const;
    bool   setOffsideKickMargin          ( double d    )      ;
    double getOffsideKickMargin          (             ) const;

    // set- and get methods for log-related parameters
    bool   setVerbose                    ( bool   b    )      ;
    bool   getVerbose                    (             ) const;
    bool   setRecordVersion              ( int    i    )      ;
    int    getRecordVersion              (             ) const;
    bool   setRecordLog                  ( bool   b    )      ;
    bool   getRecordLog                  (             ) const;
    bool   setSendLog                    ( bool   b    )      ;
    bool   getSendLog                    (             ) const;
    bool   setLogTimes                   ( bool   b    )      ;
    bool   getLogTimes                   (             ) const;
    bool   setLogFile                    ( char   *str )      ;
    char*  getLogFile                    (             )      ;
    bool   setSynchMode                  ( bool   b    )      ;
    bool   getSynchMode                  (             ) const;
    bool   setFullStateLeft              ( bool   b    )      ;
    bool   getFullStateLeft              (             ) const;
    bool   setFullStateRight             ( bool   b    )      ;
    bool   getFullStateRight             (             ) const;

    // set- and get methods for heterogeneous player parameters from player.conf
    bool   setPlayerTypes                ( int    i    )      ;
    int    getPlayerTypes                (             ) const;
    bool   setSubsMax                    ( int    i    )      ;
    int    getSubsMax                    (             ) const;
    bool   setPlayerSpeedMaxDeltaMin     ( double d    )      ;
    double getPlayerSpeedMaxDeltaMin     (             ) const;
    bool   setPlayerSpeedMaxDeltaMax     ( double d    )      ;
    double getPlayerSpeedMaxDeltaMax     (             ) const;
    bool   setStaminaIncMaxDeltaFactor   ( double d    )      ;
    double getStaminaIncMaxDeltaFactor   (             ) const;
    bool   setPlayerDecayDeltaMin        ( double d    )      ;
    double getPlayerDecayDeltaMin        (             ) const;
    bool   setPlayerDecayDeltaMax        ( double d    )      ;
    double getPlayerDecayDeltaMax        (             ) const;
    bool   setInertiaMomentDeltaFactor   ( double d    )      ;
    double getInertiaMomentDeltaFactor   (             ) const;
    bool   setDashPowerRateDeltaMin      ( double d    )      ;
    double getDashPowerRateDeltaMin      (             ) const;
    bool   setDashPowerRateDeltaMax      ( double d    )      ;
    double getDashPowerRateDeltaMax      (             ) const;
    bool   setPlayerSizeDeltaFactor      ( double d    )      ;
    double getPlayerSizeDeltaFactor      (             ) const;
    bool   setKickableMarginDeltaMin     ( double d    )      ;
    double getKickableMarginDeltaMin     (             ) const;
    bool   setKickableMarginDeltaMax     ( double d    )      ;
    double getKickableMarginDeltaMax     (             ) const;
    bool   setKickRandDeltaFactor        ( double d    )      ;
    double getKickRandDeltaFactor        (             ) const;
    bool   setExtraStaminaDeltaMin       ( double d    )      ;
    double getExtraStaminaDeltaMin       (             ) const;
    bool   setExtraStaminaDeltaMax       ( double d    )      ;
    double getExtraStaminaDeltaMax       (             ) const;
    bool   setEffortMaxDeltaFactor       ( double d    )      ;
    double getEffortMaxDeltaFactor       (             ) const;
    bool   setEffortMinDeltaFactor       ( double d    )      ;
    double getEffortMinDeltaFactor       (             ) const;
    bool   setNewDashPowerRateDeltaMin   ( double d    )      ;
    double getNewDashPowerRateDeltaMin   (             ) const;
    bool   setNewDashPowerRateDeltaMax   ( double d    )      ;
    double getNewDashPowerRateDeltaMax   (             ) const;
    bool   setNewStaminaIncMaxDeltaFactor( double d    )      ;
    double getNewStaminaIncMaxDeltaFactor(             ) const;

    // penalty parameters
    bool   setPenDistX                   ( double d    )      ;
    double getPenDistX                   (             ) const;
    bool   setPenMaxGoalieDistX          ( double d    )      ;
    double getPenMaxGoalieDistX          (             ) const;
    bool   setPenAllowMultKicks          ( bool   b    )      ;
    bool   getPenAllowMultKicks          (             ) const;

    // tackle parameters
    bool   setTackleDist                 ( double d    )      ;
    double getTackleDist                 (             ) const;
    bool   setTackleBackDist             ( double d    )      ;
    double getTackleBackDist             (             ) const;
    bool   setTackleWidth                ( double d    )      ;
    double getTackleWidth                (             ) const;
    bool   setTackleExponent             ( double d    )      ;
    double getTackleExponent             (             ) const;
    bool   setTackleCycles               ( int    i    )      ;
    int    getTackleCycles               (             ) const;
    bool   setTacklePowerRate            ( double d    )      ;
    double getTacklePowerRate            (             ) const;

    // set- and get methods for parameters not in server.conf or player.conf
    bool   setEffortMax                  ( double d    )      ;
    double getEffortMax                  (             ) const;
    bool   setSlowDownFactor             ( int    i    )      ;
    int    getSlowDownFactor             (             ) const;
    bool   setVisibleDistance            ( double d    )      ;
    double getVisibleDistance            (             ) const;
    bool   setExtraStamina               ( double d    )      ;
    double getExtraStamina               (             ) const;

    // set- and get methods for parameters which depend on other values
    bool   setMaximalKickDist            ( double d    )      ;
    double getMaximalKickDist            (             ) const;
};

/******************************************************************************/
/********************   CLASS HETEROPLAYERTYPES   *****************************/
/******************************************************************************/

/*! This class contains all the SoccerServer parameters which together define a
    heterogeneous player type. For each player type these parameters are
    initialized when the server is started. */
class HeteroPlayerSettings
{
    // public member data
public:
    double dPlayerSpeedMax;   /*!< player_speed_max: maximum speed of a player */
    double dStaminaIncMax;    /*!< stamina_inc_max: maximum stamina increase of a
                                 player per cycle                            */
    double dPlayerDecay;      /*!< player_decay: player speed decay per cycle  */
    double dInertiaMoment;    /*!< inertia_moment: inertia moment of a player;
                                 affects actual turn angle depending on speed*/
    double dDashPowerRate;    /*!< dash_power_rate: rate by which the 'Power'
                                 argument in a 'dash' command is multiplied
                                 (thus determining the amount of displacement
                                 of the player as a result of the 'dash')    */
    double dPlayerSize;       /*!< dash_power_rate: rate by which the 'Power'
                                 argument in a 'dash' command is multiplied
                                 (thus determining the amount of displacement
                                 of the player as a result of the 'dash')    */
    double dKickableMargin;   /*!< kickable_margin: margin around player in which
                                 ball is kickable; kickable area thus equals
                                 kickable_margin + ball_size + player_size   */
    double dKickRand;         /*!< kick_rand: random error in kick direction   */
    double dExtraStamina;     /*!< extra_stamina: extra stamina for heterogeneous
                                 player                                      */
    double dEffortMax;        /*!< effort_max: maximum value for player effort */
    double dEffortMin;        /*!< effort_min: minimum value for player effort */

    double dMaximalKickDist;  /*!< the maximum distance from a player for
                                 which the ball is still kickable            */


    void show( ostream &os = cout );
};

#endif
