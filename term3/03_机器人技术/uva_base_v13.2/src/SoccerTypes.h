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

/*! \file SoccerTypes.h
<pre>
<b>File:</b>          SenseHandler.h
<b>Project:</b>       Robocup Soccer Simulation Team: UvA Trilearn
<b>Authors:</b>       Jelle Kok
<b>Created:</b>       28/11/2000
<b>Last Revision:</b> $ID$
<b>Contents:</b>      This file contains the different enumerations and
               constants that are important in the Soccer Server.
               Furthermore it contains the class SoccerCommand which is
               used to denote the different possible soccer commands and
               the class SoccerTypes that contains all kind of static
               methods to translate text strings that are received from
               the server into the soccer types (=enumerations) that
               are defined here. Finally it contains the Time class which
               holds a two-tuple that represents the time in the soccer server.
<hr size=2>
<h2><b>Changes</b></h2>
<b>Date</b>             <b>Author</b>          <b>Comment</b>
28/11/2000       Jelle Kok       Initial version created
</pre>
*/

#ifndef _SOCCERTYPES_
#define _SOCCERTYPES_

#include <iostream>       // needed for output stream.
#include "Geometry.h"     // needed for AngDeg
#include "ServerSettings.h"


/*****************************************************************************/
/********************* DEFINES ***********************************************/
/*****************************************************************************/

#define MAX_TEAMMATES          11       /*!< Maximum number of teammates     */
#define MAX_OPPONENTS          11       /*!< Maximum number of opponents     */
#define MAX_HETERO_PLAYERS     18        /*!< Maximum number of hetero players*/
#define MAX_MSG              4096       /*!< maximum message size from server*/
#define MAX_SAY_MSG            10       /*!< maximum size of say message     */
#define MAX_TEAM_NAME_LENGTH   64       /*!< maximum length of teamname      */
#define MAX_FLAGS              55       /*!< maximum number of flags on field*/
#define MAX_LINES               4       /*!< maximum number of lines on field*/
#define DEFAULT_TEAM_NAME     "Team_L"  /*!< default teamname for own team   */
#define DEFAULT_OPPONENT_NAME "Team_R"  /*!< default teamname for opponent   */
#define PITCH_LENGTH        105.0       /*!< length of the pitch             */
#define PITCH_WIDTH          68.0       /*!< width  of the pitch             */
#define PITCH_MARGIN          5.0       /*!< margin next to the pitch        */
#define PENALTY_AREA_LENGTH  16.5       /*!< length of the penalty area      */
#define PENALTY_AREA_WIDTH   40.32      /*!< width of the penalty area       */
#define PENALTY_X (PITCH_LENGTH/2.0-PENALTY_AREA_LENGTH) /*!< penalty line of
the opponent team               */


/*****************************************************************************/
/********************* CONSTANTS *********************************************/
/*****************************************************************************/
const double  UnknownDoubleValue  = -1000.0; /*!< indicates unknown double   */
const AngDeg  UnknownAngleValue   = -1000.0; /*!< indicates unknown angle    */
const int     UnknownIntValue     = -1000;   /*!< indicates unknown int      */
const int     UnknownTime         = -20;     /*!< indicates unknown time     */
const long    UnknownMessageNr    = -30;     /*!< indicates unknown messagenr*/

/*****************************************************************************/
/********************** ENUMERATIONS *****************************************/
/*****************************************************************************/

/*! ObjectT is an enumeration of all possible objects that are part of the
    RoboCup soccer simulation. The class SoccerTypes contains different methods
    to easily work with these objects and convert them to text strings and text
    strings to ObjectT. */
enum ObjectT { // don't change order
    OBJECT_BALL,             /*!< Ball                     */
    OBJECT_GOAL_L,           /*!< Left goal                */    // 2 goals
    OBJECT_GOAL_R,           /*!< Right goal               */
    OBJECT_GOAL_UNKNOWN,     /*!< Unknown goal             */
    OBJECT_LINE_L,           /*!< Left line                */    // 4 lines
    OBJECT_LINE_R,           /*!< Right line               */
    OBJECT_LINE_B,           /*!< Bottom line              */
    OBJECT_LINE_T,           /*!< Top line                 */
    OBJECT_FLAG_L_T,         /*!< Flag left top            */   // 53 flags
    OBJECT_FLAG_T_L_50,      /*!< Flag top left 50         */
    OBJECT_FLAG_T_L_40,      /*!< Flag top left 40         */
    OBJECT_FLAG_T_L_30,      /*!< Flag top left 30         */
    OBJECT_FLAG_T_L_20,      /*!< Flag top left 20         */
    OBJECT_FLAG_T_L_10,      /*!< Flag top left 10         */
    OBJECT_FLAG_T_0,         /*!< Flag top left 0          */
    OBJECT_FLAG_C_T,         /*!< Flag top center          */
    OBJECT_FLAG_T_R_10,      /*!< Flag top right 10        */
    OBJECT_FLAG_T_R_20,      /*!< Flag top right 20        */
    OBJECT_FLAG_T_R_30,      /*!< Flag top right 30        */
    OBJECT_FLAG_T_R_40,      /*!< Flag top right 40        */
    OBJECT_FLAG_T_R_50,      /*!< Flag top right 50        */
    OBJECT_FLAG_R_T,         /*!< Flag right top           */
    OBJECT_FLAG_R_T_30,      /*!< Flag right top 30        */
    OBJECT_FLAG_R_T_20,      /*!< Flag right top 20        */
    OBJECT_FLAG_R_T_10,      /*!< Flag right top 10        */
    OBJECT_FLAG_G_R_T,       /*!< Flag goal right top      */
    OBJECT_FLAG_R_0,         /*!< Flag right 0             */
    OBJECT_FLAG_G_R_B,       /*!< Flag goal right bottom   */
    OBJECT_FLAG_R_B_10,      /*!< Flag right bottom 10     */
    OBJECT_FLAG_R_B_20,      /*!< Flag right bottom 20     */
    OBJECT_FLAG_R_B_30,      /*!< Flag right bottom 30     */
    OBJECT_FLAG_R_B,         /*!< Flag right bottom        */
    OBJECT_FLAG_B_R_50,      /*!< Flag bottom right 50     */
    OBJECT_FLAG_B_R_40,      /*!< Flag bottom right 40     */
    OBJECT_FLAG_B_R_30,      /*!< Flag bottom right 30     */
    OBJECT_FLAG_B_R_20,      /*!< Flag bottom right 20     */
    OBJECT_FLAG_B_R_10,      /*!< Flag bottom right 10     */
    OBJECT_FLAG_C_B,         /*!< Flag center bottom       */
    OBJECT_FLAG_B_0,         /*!< Flag bottom 0            */
    OBJECT_FLAG_B_L_10,      /*!< Flag bottom left 10      */
    OBJECT_FLAG_B_L_20,      /*!< Flag bottom left 20      */
    OBJECT_FLAG_B_L_30,      /*!< Flag bottom left 30      */
    OBJECT_FLAG_B_L_40,      /*!< Flag bottom left 40      */
    OBJECT_FLAG_B_L_50,      /*!< Flag bottom left 50      */
    OBJECT_FLAG_L_B,         /*!< Flag left bottom         */
    OBJECT_FLAG_L_B_30,      /*!< Flag left bottom 30      */
    OBJECT_FLAG_L_B_20,      /*!< Flag left bottom 20      */
    OBJECT_FLAG_L_B_10,      /*!< Flag left bottom 10      */
    OBJECT_FLAG_G_L_B,       /*!< Flag goal left bottom    */
    OBJECT_FLAG_L_0,         /*!< Flag left 0              */
    OBJECT_FLAG_G_L_T,       /*!< Flag goal left top       */
    OBJECT_FLAG_L_T_10,      /*!< Flag left bottom 10      */
    OBJECT_FLAG_L_T_20,      /*!< Flag left bottom 20      */
    OBJECT_FLAG_L_T_30,      /*!< Flag left bottom 30      */
    OBJECT_FLAG_P_L_T,       /*!< Flag penaly left top     */
    OBJECT_FLAG_P_L_C,       /*!< Flag penaly left center  */
    OBJECT_FLAG_P_L_B,       /*!< Flag penaly left bottom  */
    OBJECT_FLAG_P_R_T,       /*!< Flag penaly right top    */
    OBJECT_FLAG_P_R_C,       /*!< Flag penaly right center */
    OBJECT_FLAG_P_R_B,       /*!< Flag penaly right bottom */
    OBJECT_FLAG_C,           /*!< Flag center field        */
    OBJECT_TEAMMATE_1,       /*!< Teammate nr 1            */    // teammates 61
    OBJECT_TEAMMATE_2,       /*!< Teammate nr 2            */
    OBJECT_TEAMMATE_3,       /*!< Teammate nr 3            */
    OBJECT_TEAMMATE_4,       /*!< Teammate nr 4            */
    OBJECT_TEAMMATE_5,       /*!< Teammate nr 5            */
    OBJECT_TEAMMATE_6,       /*!< Teammate nr 6            */
    OBJECT_TEAMMATE_7,       /*!< Teammate nr 7            */
    OBJECT_TEAMMATE_8,       /*!< Teammate nr 8            */
    OBJECT_TEAMMATE_9,       /*!< Teammate nr 9            */
    OBJECT_TEAMMATE_10,      /*!< Teammate nr 10           */
    OBJECT_TEAMMATE_11,      /*!< Teammate nr 11           */
    OBJECT_TEAMMATE_UNKNOWN, /*!< Teammate nr unkown       */
    OBJECT_OPPONENT_1,       /*!< Opponent nr 1            */    // opponents 73
    OBJECT_OPPONENT_2,       /*!< Opponent nr 2            */
    OBJECT_OPPONENT_3,       /*!< Opponent nr 3            */
    OBJECT_OPPONENT_4,       /*!< Opponent nr 4            */
    OBJECT_OPPONENT_5,       /*!< Opponent nr 5            */
    OBJECT_OPPONENT_6,       /*!< Opponent nr 6            */
    OBJECT_OPPONENT_7,       /*!< Opponent nr 7            */
    OBJECT_OPPONENT_8,       /*!< Opponent nr 8            */
    OBJECT_OPPONENT_9,       /*!< Opponent nr 9            */
    OBJECT_OPPONENT_10,      /*!< Opponent nr 10           */
    OBJECT_OPPONENT_11,      /*!< Opponent nr 11           */
    OBJECT_OPPONENT_UNKNOWN, /*!< Opponent nr unknown      */    // 84
    OBJECT_PLAYER_UNKNOWN,   /*!< Unknown player           */
    OBJECT_UNKNOWN,          /*!< Unknown object           */
    OBJECT_TEAMMATE_GOALIE,  /*!< Goalie of your side      */
    OBJECT_OPPONENT_GOALIE,  /*!< Goalie of opponent side  */
    OBJECT_ILLEGAL,          /*!< illegal object           */
    OBJECT_MAX_OBJECTS       /*!< maximum nr of objects    */ // 90
} ;

/*!The ObjectSetT enumerations holds the different object sets, which
   consists of one or multiple ObjectT types. */
enum ObjectSetT
{
    OBJECT_SET_TEAMMATES,        /*!< teammates                       */
    OBJECT_SET_OPPONENTS,        /*!< opponents                       */
    OBJECT_SET_PLAYERS,          /*!< players                         */
    OBJECT_SET_TEAMMATES_NO_GOALIE,/*!< teammates without the goalie  */
    OBJECT_SET_FLAGS,            /*!< flags                           */
    OBJECT_SET_LINES,            /*!< lines                           */
    OBJECT_SET_ILLEGAL           /*!< illegal                         */
} ;

/*!The PlayModeT enumeration contains all playmodes of the soccer playmode.
   The associated string values can be returned using the methods in the
   SoccerTypes class */
enum PlayModeT {
    PM_BEFORE_KICK_OFF,        /*!< before_kick_off:   before kick off         */
    PM_KICK_OFF_LEFT,          /*!< kick_off_l:        kick off for left team  */
    PM_KICK_OFF_RIGHT,         /*!< kick_off_r:        kick off for right team */
    PM_KICK_IN_LEFT,           /*!< kick_in_l:         kick in for left team   */
    PM_KICK_IN_RIGHT,          /*!< kick_in_r:         kick in for right team  */
    PM_CORNER_KICK_LEFT,       /*!< corner_kick_l:     corner kick left team   */
    PM_CORNER_KICK_RIGHT,      /*!< corner_kick_r:     corner kick right team  */
    PM_GOAL_KICK_LEFT,         /*!< goal_kick_l:       goal kick for left team */
    PM_GOAL_KICK_RIGHT,        /*!< goal_kick_r:       goal kick for right team*/
    PM_GOAL_LEFT,              /*!< goal_l:            goal scored by team left*/
    PM_GOAL_RIGHT,             /*!< goal_r:            goal scored by team righ*/
    PM_FREE_KICK_FAULT_LEFT,   /*!< free_kick_fault_l: free_kick to yourself   */
    PM_FREE_KICK_FAULT_RIGHT,	 /*!< free_kick_fault_r: free_kick to self   */
    PM_FREE_KICK_LEFT,         /*!< free_kick_l:       free kick for left team */
    PM_FREE_KICK_RIGHT,        /*!< free_kick_r:       free kick for right team*/
    PM_INDIRECT_FREE_KICK_RIGHT,/*!<indirect_free_kick_r: ind. free kick right */
    PM_INDIRECT_FREE_KICK_LEFT,/*!< indirect_free_kick_l: ind. free kick left  */
    PM_BACK_PASS_LEFT,         /*!< back_pass_l:       left team passed back   */
    PM_BACK_PASS_RIGHT,        /*!< back_pass_r:       right team passed back  */
    PM_OFFSIDE_LEFT,           /*!< offside_l:         offside for left team   */
    PM_OFFSIDE_RIGHT,          /*!< offside_r:         offside for right team  */
    PM_PLAY_ON,                /*!< play_on:           play on (during match)  */
    PM_TIME_OVER,              /*!< time_over:         time over (after match) */
    PM_PENALTY_SETUP_LEFT,     /*!< penalty_setup_left left setups for penalty */
    PM_PENALTY_SETUP_RIGHT,    /*!< penalty_setup_right right setup for penalty*/
    PM_PENALTY_READY_LEFT,     /*!< penalty_ready_left ready for penalty l team*/
    PM_PENALTY_READY_RIGHT,    /*!< penalty_ready_right ready for pen r team   */
    PM_PENALTY_TAKEN_LEFT,     /*!< penalty_taken_left  penalty started left   */
    PM_PENALTY_TAKEN_RIGHT,    /*!< penalty_taken_right penalty started right  */
    PM_FROZEN,                 /*!< game play is frozen                        */
    PM_QUIT,                   /*!< quit                                       */
    PM_ILLEGAL                 /*!< unknown playmode                           */
} ;

/*! The RefereeT enumeration contains all messages that the referee can sent.
    The SoccerTypes class contains different methods to convert these messages
    to the corresponding playmode. */
enum RefereeMessageT {
    REFC_ILLEGAL,                /*!< unknown message                          */
    REFC_BEFORE_KICK_OFF,        /*!< before_kick_off:   before kick off       */
    REFC_KICK_OFF_LEFT,          /*!< kick_off_l:        kickoff for left team */
    REFC_KICK_OFF_RIGHT,         /*!< kick_off_r:        kickoff for right team*/
    REFC_KICK_IN_LEFT,           /*!< kick_in_l:         kick in for left team */
    REFC_KICK_IN_RIGHT,          /*!< kick_in_r:         kick in for right team*/
    REFC_CORNER_KICK_LEFT,       /*!< corner_kick_l:     corner kick left team */
    REFC_CORNER_KICK_RIGHT,      /*!< corner_kick_r:     corner kick right team*/
    REFC_GOAL_KICK_LEFT,         /*!< goal_kick_l:       goal kick left team   */
    REFC_GOAL_KICK_RIGHT,        /*!< goal_kick_r:       goal kick right team  */
    REFC_FREE_KICK_LEFT,         /*!< free_kick_l:       free kick left team   */
    REFC_FREE_KICK_RIGHT,        /*!< free_kick_r:       free kick right team  */
    REFC_INDIRECT_FREE_KICK_RIGHT,/*!< indirect_free_kick_r: ind freekick right*/
    REFC_INDIRECT_FREE_KICK_LEFT,/*!< indirect_free_kick_l: ind. free kick left*/
    REFC_FREE_KICK_FAULT_LEFT,   /*!< free_kick_fault_l: free_kick to yourself */
    REFC_FREE_KICK_FAULT_RIGHT,  /*!< free_kick_fault_r: free_kick to yourself */
    REFC_BACK_PASS_LEFT,         /*!< back_pass_l:       left team passed back */
    REFC_BACK_PASS_RIGHT,        /*!< back_pass_r:       right team passed back*/
    REFC_PLAY_ON,                /*!< play_on:           play on (during match)*/
    REFC_TIME_OVER,              /*!< time_over:         time over(after match)*/
    REFC_FROZEN,                 /*!< frozen:            game play is frozen   */
    REFC_QUIT,                   /*!< quit:              quit                  */
    REFC_OFFSIDE_LEFT,           /*!< offside_l          offside left          */
    REFC_OFFSIDE_RIGHT,          /*!< offside_r          offside right         */
    REFC_HALF_TIME,              /*!< half_time:         it is half time       */
    REFC_TIME_UP,                /*!< time_up:           match has finished    */
    REFC_TIME_UP_WITHOUT_A_TEAM, /*!< time_up_without_a_team: match finished   */
    REFC_TIME_EXTENDED,          /*!< time_extended:     time cont. overtime   */
    REFC_FOUL_LEFT,              /*!< foul_l:            foul made by left     */
    REFC_FOUL_RIGHT,             /*!< foul_r:            foul made by right    */
    REFC_GOAL_LEFT,              /*!< goal_l:            goal made by left     */
    REFC_GOAL_RIGHT,             /*!< goal_r:            goal made by right    */
    REFC_DROP_BALL,              /*!< drop_ball:         ball is dropped       */
    REFC_GOALIE_CATCH_BALL_LEFT, /*!< goalie_catch_ball_l: left goalie catch   */
    REFC_GOALIE_CATCH_BALL_RIGHT,/*!< goalie_catch_ball_r: right goalie catch  */
    REFC_PENALTY_SETUP_LEFT,     /*!< penalty_setup_left left setup for penalty*/
    REFC_PENALTY_SETUP_RIGHT,    /*!< penalty_setup_right right setups for pen.*/
    REFC_PENALTY_READY_LEFT,     /*!< penalty_ready_left  ready for pen. l team*/
    REFC_PENALTY_READY_RIGHT,    /*!< penalty_ready_right ready for pen. r team*/
    REFC_PENALTY_TAKEN_LEFT,     /*!< penalty_ready_left  pen. taken by  l team*/
    REFC_PENALTY_TAKEN_RIGHT,    /*!< penalty_ready_right pen. taken by  r team*/
    REFC_PENALTY_MISS_LEFT,      /*!< penalty_miss_left   penalty missed r team*/
    REFC_PENALTY_MISS_RIGHT,     /*!< penalty_miss_right  penalty missed l team*/
    REFC_PENALTY_SCORE_LEFT,     /*!< penalty_score_left  penalty score l team */
    REFC_PENALTY_SCORE_RIGHT,    /*!< penalty_score_right penalty score r team */
    REFC_PENALTY_FOUL_LEFT,      /*!< penalty_foul_left   penalty foul l team  */
    REFC_PENALTY_FOUL_RIGHT,     /*!< penalty_foul_right  penalty foul r team  */
    REFC_PENALTY_ONFIELD_LEFT,   /*!< penalty_onfield_left pen. on left field  */
    REFC_PENALTY_ONFIELD_RIGHT,  /*!< penalty_onfield_right pen. on right field*/
    REFC_PENALTY_WINNER_LEFT,    /*!< penalty_winner_l    penalty won by r team*/
    REFC_PENALTY_WINNER_RIGHT,   /*!< penalty_winner_r    penalty won by l team*/
    REFC_PENALTY_DRAW            /*!< penalty_draw        penalty result = draw*/
} ;

/*! The ViewAngleT enumeration contains the different view angles that are
    possible for a player */
enum ViewAngleT  {
    VA_NARROW,  /*!< view angle narrow  */
    VA_NORMAL,  /*!< view angle normal  */
    VA_WIDE,    /*!< view angle wide    */
    VA_ILLEGAL  /*!< illegal view angle */
};

/*!The ViewQualityT enumeration contains the different view qualities possible
   for a player. */
enum ViewQualityT {
    VQ_HIGH,    /*!< view quality high    */
    VQ_LOW,     /*!< view quality low     */
    VQ_ILLEGAL  /*!< illegal view quality */
};

/*!The SideT enumeration contains the two sides */
enum SideT {
    SIDE_LEFT,     /*!< left side    */
    SIDE_RIGHT,    /*!< right SIDE   */
    SIDE_ILLEGAL   /*!< illegal SIDE */
} ;

/*!The CommandT enumeration contains the different types for the SoccerCommand
   that are possible. */
enum CommandT {
    CMD_ILLEGAL,      /*!< illegal command */
    CMD_DASH,         /*!< dash command (player only)         */
    CMD_TURN,         /*!< turn command (player only)         */
    CMD_TURNNECK,     /*!< turn_neck command (player only)    */
    CMD_CHANGEVIEW,   /*!< change view command (player only)  */
    CMD_CATCH,        /*!< catch command (goalie only)        */
    CMD_KICK,         /*!< kick command (player only)         */
    CMD_MOVE,         /*!< move command                       */
    CMD_SENSEBODY,    /*!< sense_body command (player only)   */
    CMD_SAY,          /*!< say command                        */
    CMD_CHANGEPLAYER, /*!< change_player command (coach only) */
    CMD_ATTENTIONTO,  /*!< pay attention to specific player   */
    CMD_TACKLE,       /*!< tackle in current body direction   */
    CMD_POINTTO,      /*!< point arm towards a point on field */
    CMD_MAX_COMMANDS  /*!< maximum number of commands         */
} ;

/*!The PlayerT enumeration contains the different playertypes that are defined
   in a formation. This should not be confused with the later introducted
   player_types in soccerserver 7.xx that denotes the different possible
   heterogeneous players. A player type in the context PlayerT denotes the
   kind of player (midfielder, attacker) on the field. Its role on the pitch.*/
enum PlayerT {
    PT_ILLEGAL,             /*!< illegal player type */
    PT_GOALKEEPER,          /*!< goalkeeper          */
    PT_DEFENDER_CENTRAL,    /*!< central defender    */
    PT_DEFENDER_SWEEPER,    /*!< sweeper defender    */
    PT_DEFENDER_WING,       /*!< wing defender       */
    PT_MIDFIELDER_CENTER,   /*!< central midfielder  */
    PT_MIDFIELDER_WING,     /*!< wing midfielder     */
    PT_ATTACKER_WING,       /*!< wing attacker       */
    PT_ATTACKER,            /*!< central attacker    */
    MAX_PLAYER_TYPES
} ;

/*! The PlayerSetT enumeration contains different sets of playertypes that
    are defined in a formation. Possible sets are all midfielders, etc. */
enum PlayerSetT {
    PS_DEFENDERS,           /*!< all defenders       */
    PS_MIDFIELDERS,         /*!< all midfielders     */
    PS_ATTACKERS,           /*!< all attackers       */
    PS_ALL                  /*!< all players         */
} ;

/*!The FormationT enumeration contains the different formation types that
   are defined. */
enum FormationT {
    FT_ILLEGAL,             /*!< illegal formation type                  */
    FT_INITIAL,             /*!< initial formation type (before kick off)*/
    FT_433_OFFENSIVE,       /*!< 433 offensive formation                 */
    FT_334_OFFENSIVE,       /*!< 434 offensive formation                 */
    FT_DEFENSIVE,           /*!< defensive formation type                */
    FT_OPEN_DEFENSIVE,      /*!< open defensive formation type           */
    FT_343_ATTACKING,       /*!< attacking formation type                */
    MAX_FORMATION_TYPES
} ;

/*!The BallStatus enumeration contains the status of the ball. This is returned
   when the coach has issued the check_ball message. */
enum BallStatusT {
    BS_ILLEGAL,           /*!< illegal ball status      */
    BS_IN_FIELD,          /*!< ball is in the field     */
    BS_GOAL_LEFT,         /*!< ball is in left goal     */
    BS_GOAL_RIGHT,        /*!< ball is in right goal    */
    BS_OUT_OF_FIELD       /*!< ball is not in the field */
} ;

/*!The ActionT enumeration contains different (high-level) actions. */
enum ActionT {
    ACT_ILLEGAL,                    /*!< illegal action (default)              */
    ACT_SEARCH_BALL,                /*!< search for the ball                   */
    ACT_KICK_BALL,                  /*!< kick the ball                         */
    ACT_CATCH_BALL,                 /*!< catch the ball                        */
    ACT_INTERCEPT,                  /*!< intercept the ball                    */
    ACT_MARK,                       /*!< mark an opponent                      */
    ACT_TELEPORT_TO_STRATEGIC_POSITION, /*!< move to a strategic position(move)*/
    ACT_WATCH_BALL,                 /*!< watch the ball                        */
    ACT_ANTICIPATE_BALL,            /*!< turn in anticipation for ball         */
    ACT_GOTO_STRATEGIC_POSITION,    /*!< go to a strategic position (dash)     */
    ACT_TURN_BODY_TO_CENTER,        /*!< turn body to center of field          */
    ACT_MOVE_TO_DEAD_BALL_POSITION, /*!< move to pos in dead ball situation    */
    ACT_INTERCEPT_SCORING_ATTEMPT,  /*!< intercept ball heading to goal        */
    ACT_DEFEND_GOALLINE,            /*!< defend the goalline (for goalkeeper   */
    ACT_TELEPORT_AFTER_CATCH,       /*!< teleport after catch (for goalkeeper) */
    ACT_TACKLE,                     /*!< tackle the ball                       */
    ACT_HOLD_BALL                   /*!< hold the ball                         */
} ;


/*! The MarkT enumeration contains different marking techniques. */
enum MarkT {
    MARK_ILLEGAL,    /*!< illegal marking                                     */
    MARK_GOAL,       /*!< mark goal (stand on obj-goal line)                  */
    MARK_BISECTOR,   /*!< mark bisector stand between goal,obj and ball,obj   */
    MARK_BALL        /*!< mark ball (stand on obj-ball line)                  */
} ;

/*! The DribbleT enumeration contains different dribble possibilities. */
enum DribbleT {
    DRIBBLE_ILLEGAL, /*!< illegal dribbling                                   */
    DRIBBLE_WITHBALL,/*!< dribble with ball very close                        */
    DRIBBLE_SLOW,    /*!< dribble slowly but kicking ball slightly ahead      */
    DRIBBLE_FAST     /*!< dribble fast by kicking ball up front               */
} ;

/*! The PassT enumeration contains different passing possibilities. */
enum PassT {
    PASS_ILLEGAL,    /*!< illegal pass                                        */
    PASS_FAST,       /*!< pass fast to a teammate                             */
    PASS_NORMAL      /*!< pass normal to a teammate                           */
} ;

/*! The ClearBallT enumeration contains different clear ball possibilities. */
enum ClearBallT {
    CLEAR_BALL_ILLEGAL,    /*!< illegal clear ball                            */
    CLEAR_BALL_OFFENSIVE,  /*!< clear ball to the front of the field          */
    CLEAR_BALL_DEFENSIVE,  /*!< clear ball to the center line of the field    */
    CLEAR_BALL_OFFENSIVE_SIDE, /*!< clear ball to front and side of the field */
    CLEAR_BALL_GOAL        /*!< clear ball to position in front of the goal   */
} ;

/*! The TiredNessT enumeration contains different values that indicate how
    tired an agent is. */
enum TiredNessT {
    TIREDNESS_ILLEGAL,     /*!< illegal tiredness value                       */
    TIREDNESS_GOOD,        /*!< player is not tired at all                    */
    TIREDNESS_AVERAGE,     /*!< average tiredness                             */
    TIREDNESS_BAD,         /*!< player starts to get tired                    */
    TIREDNESS_VERY_BAD,    /*!< player is so tired it can hardly move         */
    TIREDNESS_TERRIBLE     /*!< player is so tired it cannot move             */
} ;

/*! The FeaturesT enumeration contains different features that can be saved.
    In this case, features represent specific information concerning the
    current state. When specific information is calculated once (e.g., the
    fastest opponent to the ball). This information can be stored. In the
    next request for this information, the stored result is immediately
    returned. */
enum FeatureT {
    FEATURE_ILLEGAL,                   /*!< illegal feature                    */
    FEATURE_FASTEST_OPPONENT_TO_BALL,  /*!< fastest opponent to the ball       */
    FEATURE_FASTEST_TEAMMATE_TO_BALL,  /*!< fastest teammate to the ball       */
    FEATURE_FASTEST_PLAYER_TO_BALL,    /*!< fastest player to the ball         */
    FEATURE_FASTEST_TEAMMATE_TO_BALL_NO_GOALIE, /*!< fastest teammate, no goali*/
    FEATURE_INTERCEPTION_POINT_BALL,   /*!< interception point ball            */
    FEATURE_INTERCEPT_CLOSE,           /*!< close interception point ball      */
    FEATURE_INTERCEPT_CYCLES_ME,       /*!< nr of cycles for me to intercept   */
    FEATURE_BEST_SCORING_POINT,        /*!< best scoring point in the goal     */
    MAX_FEATURES
} ;

/*! The DirectionT enumeration contains the different directions */
enum DirectionT {
    DIR_ILLEGAL,                 /*!< illegal message                          */
    DIR_NORTH,                   /*!< north direction                          */
    DIR_NORTHWEST,               /*!< north west direction                     */
    DIR_NORTHEAST,               /*!< north east direction                     */
    DIR_CENTER,                  /*!< center direction                         */
    DIR_EAST,                    /*!< east direction                           */
    DIR_WEST,                    /*!< west direction                           */
    DIR_SOUTHWEST,               /*!< south west direction                     */
    DIR_SOUTHEAST,               /*!< south east direction                     */
    DIR_SOUTH,                   /*!< south direction                          */
    DIR_MAX                      /*!< number of directions                     */
} ;

/*! The SucceedT enumeration contains the different succeed rate probabilites*/
enum SucceedT  {
    SUCCEED_ILLEGAL,             /*!< illegal message                          */
    SUCCEED_ALWAYS,              /*!< wil always succeed                       */
    SUCCEED_DOUBTFUL,            /*!< in some occassions it may succeed        */
    SUCCEED_NEVER                /*!< this will never succeed                  */
} ;


/*****************************************************************************/
/********************** CLASS TIME   *****************************************/
/*****************************************************************************/

/*! This class contains the time representation of the soccer server.
    It is represented by an ordered pair (t,s) where t denotes the current
    server cycle and s is the number of cycles since the clock has stopped.
    Here the value for t equals that of the time stamp contained
    in the last message received from the server, whereas the value $s$ will
    always be 0 while the game is in progress. It is only during certain dead
    ball situations (e.g. an offside call leading to a free kick) that this
    value will be different, since in these cases the server time will stop
    while cycles continue to pass (i.e. actions can still be performed).
    Representing the time in this way has the advantage that it allows the
    players to reason about the number of cycles between events in a meaningful
    way. */
class Time
{
    int m_iTime;            /*!< Number of cycles, denoting the time */
    int m_iStopped;         /*!< Number of cycles stopped at m_iTime */

public:
    Time             ( int     iTime = -1, int iStopped = 0 );
    bool   updateTime       ( int     iTime                        );
    bool   setTimeStopped   ( int     iTime                        );
    int    getTime          (                                      );
    int    getTimeStopped   (                                      );
    int    getTimeDifference( Time    t                            );
    bool   isStopped        (                                      );
    Time   getTimeAddedWith ( int     iCycles                      );
    bool   addToTime        ( int     iCycles                      );
    void   show             ( ostream &os = cout                   );

    // overloaded arithmetic operators
    Time   operator +       ( const int  &i );
    Time   operator +       ( Time t );
    Time   operator -       ( const int  &i );
    int    operator -       ( Time t );
    void   operator =       ( const int  &i );
    void   operator +=      ( Time t );
    void   operator +=      ( const int  &i );
    void   operator -=      ( Time t );
    void   operator -=      ( const int  &i );
    bool   operator !=      ( Time t );
    bool   operator !=      ( const int  &i );
    bool   operator ==      ( Time t );
    bool   operator ==      ( const int  &i );
    bool   operator <       ( Time t );
    bool   operator <       ( const int  &i );
    bool   operator <=      ( Time t );
    bool   operator <=      ( const int  &i );
    bool   operator >       ( Time t );
    bool   operator >       ( const int  &i );
    bool   operator >=      ( Time t );
    bool   operator >=      ( const int  &i );


    // methods for producing output
    friend ostream&   operator << ( ostream &os, Time t );
} ;


/*****************************************************************************/
/********************* CLASS SOCCERCOMMAND ***********************************/
/*****************************************************************************/

/*!This class resembles a SoccerCommand that contains all the information about
   a command that can be sent to the server, but the format is independent from
   the server implementation. A SoccerCommand can be created and before it is
   sent to the server, be converted to the actual string representation
   understood by the server. */
class SoccerCommand
{
    ServerSettings *SS; /*!< ServerSettings are used to check ranges of command*/

    // private methods to generate text string to sent to server
    bool  makeCatchCommand       ( char *str );
    bool  makeChangeViewCommand  ( char *str );
    bool  makeDashCommand        ( char *str );
    bool  makeKickCommand        ( char *str );
    bool  makeMoveCommand        ( char *str );
    bool  makeSayCommand         ( char *str );
    bool  makeSenseBodyCommand   ( char *str );
    bool  makeTurnCommand        ( char *str );
    bool  makeTurnNeckCommand    ( char *str );
    bool  makeChangePlayerCommand( char *str );
    bool  makeAttentionToCommand ( char *str );
    bool  makeTackleCommand      ( char *str );
    bool  makePointToCommand     ( char *str );

public:

    // different variables that are used by the different possible commands
    // only the variables that are related to the current commandType have
    // legal values
    Time         time;        /*!< command time, will be set by worldmodel     */
    CommandT     commandType; /*!< type of this command                        */
    double       dAngle;      /*!< angle of this command (for turn,turn_neck,dash)  *//*! misol.gao fixed for v13.2 */
    double       dPower;      /*!< power of this command (for kick,dash)       */
    ViewQualityT vq;          /*!< view quality (for change_view)              */
    ViewAngleT   va;          /*!< view angle (for change_view)                */
    double       dX;          /*!< x coordinate (for move)                     */
    double       dY;          /*!< y coordinate (for move)                     */
    char         str[MAX_SAY_MSG];/*!< str (for say)                           */
    int          iTimes;      /*!< how many cycles will a command  be sent     */

    SoccerCommand( CommandT com = CMD_ILLEGAL, double d1=UnknownDoubleValue,
                   double d2=0.0,/*! misol.gao fixed for v13.2 */
                   double d3=UnknownDoubleValue );
    SoccerCommand( CommandT com,               char   *msg                  );

    // command to set the different values of the SoccerCommand
    void makeCommand( CommandT com, double     d1 = UnknownDoubleValue,
                      double     d2 = 0.0,/*! misol.gao fixed for v13.2 */
                      double     d3 = UnknownDoubleValue      );
    void makeCommand( CommandT com, ViewAngleT v,    ViewQualityT q         );
    void makeCommand( CommandT com, char       *msg                         );

    bool isIllegal  (                                                       );

    void show       ( ostream& os                                           );

    // used to return the string representation of this SoccerCommand
    bool getCommandString( char *str,         ServerSettings *ss           );
} ;

/*****************************************************************************/
/********************* CLASS FEATURE *****************************************/
/*****************************************************************************/

/*! This class contains information for one specific feature (e.g., fastest
    teammate to the ball. A feature corresponds to a specific time and is often
    related to a specific object and a specific value. Therefore, these values
    are stored in this class. */
class Feature
{
    Time          m_timeSee;  /*!< see time corresponding to stored information*/
    Time          m_timeSense;/*!< sense time corresponding to stored info     */
    Time          m_timeHear; /*!< hear time corresponding to stored info      */
    ObjectT       m_object;   /*!< object information stored with this feature */
    double        m_dInfo;    /*!< information stored with this feature        */
    VecPosition   m_vec;      /*!< information stored with this feature        */
    SoccerCommand m_soc;      /*!< command stored with this feature            */

public:
    // standard get and set methods
    Feature(                                                                   );
    Feature(                    Time          timeSee,
                                Time          timeSense,
                                Time          timeHear,
                                ObjectT       object,
                                double        dInfo = UnknownDoubleValue,
                                SoccerCommand soc   = SoccerCommand(CMD_ILLEGAL),
                                VecPosition   pos = VecPosition(0,0)           );
    bool          setFeature  ( Time          timeSee,
                                Time          timeSense,
                                Time          timeHear,
                                ObjectT       object,
                                double        dInfo = UnknownDoubleValue,
                                SoccerCommand soc   = SoccerCommand(CMD_ILLEGAL),
                                VecPosition   pos = VecPosition(0,0)           );
    bool          setTimeSee  ( Time          time                             );
    Time          getTimeSee  (                                                );
    bool          setTimeSense( Time          time                             );
    Time          getTimeSense(                                                );
    bool          setTimeHear ( Time          time                             );
    Time          getTimeHear (                                                );
    bool          setObject   ( ObjectT       obj                              );
    ObjectT       getObject   (                                                );
    bool          setInfo     ( double        d                                );
    double        getInfo     (                                                );
    bool          setVec      ( VecPosition   pos                              );
    VecPosition   getVec      (                                                );
    bool          setCommand  ( SoccerCommand soc                              );
    SoccerCommand getCommand  (                                                );

} ;

/*****************************************************************************/
/********************** CLASS SOCCERTYPES ************************************/
/*****************************************************************************/

/*! The class SoccerTypes contains different methods to work with the
    different enumerations defined in SoccerTypes.h. It is possible to
    convert soccertypes to strings and strings to soccertypes. It is
    also possible to get more specific information about some of the
    soccertypes. All methods are static so it is possible to call the
    methods without instantiating the class. */
class SoccerTypes
{
public:
    // methods that deal with the differen objects
    static char*           getObjectStr              ( char         *strBuf,
            ObjectT      o,
            const char   *strTe=NULL);
    static ObjectT         getObjectFromStr          ( char         **str,
            bool         *isGoalie,
            const char   *strTeam   );
    static bool            isInSet                   ( ObjectT      o,
            ObjectSetT   o_s,
            ObjectT      objectGoalie=
                OBJECT_TEAMMATE_1     );
    static bool            isPlayerTypeInSet         ( PlayerT      p,
            PlayerSetT   p_s        );
    static bool            isFlag                    ( ObjectT      o          );
    static bool            isLine                    ( ObjectT      o          );
    static bool            isGoal                    ( ObjectT      o          );
    static ObjectT         getOwnGoal                ( SideT        s          );
    static ObjectT         getGoalOpponent           ( SideT        s          );
    static bool            isBall                    ( ObjectT      o          );
    static bool            isTeammate                ( ObjectT      o          );
    static bool            isOpponent                ( ObjectT      o          );
    static bool            isGoalie                  ( ObjectT      o          );
    static bool            isPlayer                  ( ObjectT      o          );
    static bool            isKnownPlayer             ( ObjectT      o          );
    static int             getIndex                  ( ObjectT      o          );
    static ObjectT         getTeammateObjectFromIndex( int          iIndex     );
    static ObjectT         getOpponentObjectFromIndex( int          iIndex     );
    static VecPosition     getGlobalPositionFlag     ( ObjectT      o,
            SideT        s,
            double dGoalWidth =14.02);
    static AngDeg          getGlobalAngleLine        ( ObjectT      o,
            SideT        s          );

    // methods that deal with the differen play modes
    static PlayModeT       getPlayModeFromStr        ( char         *str       );
    static PlayModeT       getPlayModeFromRefereeMessage( RefereeMessageT rm   );
    static char*           getPlayModeStr            ( PlayModeT    p          );
    static char*           getRefereeMessageStr      ( RefereeMessageT r       );
    static RefereeMessageT getRefereeMessageFromStr  ( char         *str       );

    // methods that deal with the frequency of the visual information
    static char*           getViewAngleStr           ( ViewAngleT   v          );
    static ViewAngleT      getViewAngleFromStr       ( char         *str       );
    static AngDeg          getHalfViewAngleValue     ( ViewAngleT   va         );
    static char*           getViewQualityStr         ( ViewQualityT v          );
    static ViewQualityT    getViewQualityFromStr     ( char         *str       );

    // methods that deal with the commands
    static char*           getCommandStr             ( CommandT     com        );
    static bool            isPrimaryCommand          ( CommandT     com        );

    // methods that deal with the side information
    static char*           getSideStr                ( SideT        s          );
    static SideT           getSideFromStr            ( char*        str        );

    // methods that deal with the status of the ball.
    static char*           getBallStatusStr          ( BallStatusT  bs         );
    static BallStatusT     getBallStatusFromStr      ( char         *str       );

    static AngDeg          getAngleFromDirection     ( DirectionT   dir        );
} ;

#endif
