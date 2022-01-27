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

/*! \file WorldModel.h
<pre>
<b>File:</b>          WorldModel.h
<b>Project:</b>       Robocup Soccer Simulation Team: UvA Trilearn
<b>Authors:</b>       Jelle Kok
<b>Created:</b>       12/02/2001
<b>Last Revision:</b> $ID$
<b>Contents:</b>      class declarations of WorldModel. This class contains
               methods that give information about the current and future
               state of the world (soccer field).
<hr size=2>
<h2><b>Changes</b></h
<b>Date</b>             <b>Author</b>          <b>Comment</b>
12/02/2001       Jelle Kok       Initial version created
</pre>
*/

#ifndef _WORLD_MODEL_
#define _WORLD_MODEL_

#include "Objects.h"        // needed for PlayerObject
#include "PlayerSettings.h" // needed for getPlayerDistTolerance
#include "Logger.h"         // needed for Log
#include "Formations.h"     // needed for getStrategicPosition (prediction)
#include <list>

#ifdef WIN32
#include <windows.h>      // needed for mutex
#else
#include <pthread.h>      // needed for pthread_mutex
#include <sys/time.h>     // needed for gettimeofday
#endif

// #define SM 60
// #define HALF_SM 30
// #define SN 10
// #define PAI 3.1415926

extern Logger Log;          // defined in Logger.C
extern Logger LogDraw;      // defined in Logger.C

#ifdef WIN32
/*! This function shall return the integral value (represented as a double)
    nearest x in the direction of the current rounding mode. The current
    rounding mode is implementation-defined.

    If the current rounding mode rounds toward negative infinity, then rint()
    shall be equivalent to floor() . If the current rounding mode rounds
    toward positive infinity, then rint() shall be equivalent to ceil().
    url: http://www.opengroup.org/onlinepubs/007904975/functions/rint.html */
inline double rint(double x)
{
    return floor(x+0.5);
}

/*! The drand48() function shall return non-negative,
    double-precision, floating-point values, uniformly distributed over the
    interval [0.0,1.0).
    url: http://www.opengroup.org/onlinepubs/007904975/functions/drand48.html*/
inline double drand48()
{
    return ((double)(rand() % 100)) / 100;
}
#endif

/*****************************************************************************/
/********************** CLASS WORLDMODEL *************************************/
/*****************************************************************************/

/*! The Class WorlModel contains all the RoboCup information that is available
    on the field. It contains information about the players, ball, flags and
    lines. Furthermore it contains methods to extract useful information.
    The (large amount of) attributes can be separated into different groups:
    - Environmental information: specific information about the soccer server
    - Match information: general information about the current state of a match
    - Object information: all the objects on the soccer field
    - Action information: actions that the agent has performed

    The methods can also be divided into different groups:
    - Retrieval methods: directly retrieving information of objects
    - Update methods: update world based on new sensory information
    - Prediction methods: predict future states based on past perceptions
    - High-Level methods: deriving high-level conclusions from basic worldstate
*/
class WorldModel
{
// public:
// 	void CleanStation();
// 	void UpStation();
// 	bool HasOpponetIn(int angel1,int angel2,int dist);
// 	int GetBestPass(int &m,int &n,int angel1=0,int angel2=HALF_SM-1,int dist1=2,int dist2=SN-1);
// 	VecPosition GetPosition(int m,int n);
// 	double   getFrontLine();
// 	ObjectT getBestObject(double dDist);
// 	double    getBeyondLine();
// 	bool hasOpponent(AngDeg angA,AngDeg angB,double dDist);
// 	VecPosition getMyPoint();
// 	int ChangeAngelToInt(double angel,bool Is360);
// 	bool IsCanKick();
// 	bool CanseeAngel(double angel);
// 
// private:
// int DecitionWe[SM][SN];
// int DecitionTheir[SM][SN];
// int PassValue[HALF_SM][SN];
private:
  
private:

    /***************************************************************************/
    /*************************** ATTRIBUTES ************************************/
    /***************************************************************************/

    ////////////////////////// ENVIRONMENTAL INFORMATION ////////////////////////

    ServerSettings *SS;                     /*!< Reference to all server params*/
    PlayerSettings *PS;                     /*!< Reference to all client params*/
    HeteroPlayerSettings pt[MAX_HETERO_PLAYERS]; /*!< info hetero player types */
    Formations     *formations;             /*!< Reference to formation used   */

    ////////////////////////// CURRENT MATCH INFORMATION ////////////////////////

    // time information
    Time          timeLastSeeMessage;      /*!< server time of last see msg    */
    Time          timeLastRecvSeeMessage;  /*!< server time received see msg   */
    Time          timeLastSenseMessage;    /*!< server time of last sense msg  */
    Time          timeLastRecvSenseMessage;/*!< server time received sense msg */
    Time          timeLastHearMessage;     /*!< server time of last hear msg   */
    bool          bNewInfo;                /*!< indicates new info from server */
    Time          timeLastCatch;           /*!< time of last catch by goalie   */
    Time          timeLastRefMessage;      /*!< time of last referee message   */

    // player information
    char          strTeamName[MAX_TEAM_NAME_LENGTH]; /*!< Team name            */
    int           iPlayerNumber;           /*!< player number in soccerserver  */
    SideT         sideSide;                /*!< side where the agent started   */

    // match information
    PlayModeT     playMode;                /*!< current play mode in the game  */
    int           iGoalDiff;               /*!< goal difference                */


    ////////////////////////// OBJECTS //////////////////////////////////////////

    // dynamic objects
    BallObject    Ball;                    /*!< information of the ball        */
    AgentObject   agentObject;             /*!< information of the agent itself*/
    PlayerObject  Teammates[MAX_TEAMMATES];/*!< information of all teammates   */
    PlayerObject  Opponents[MAX_OPPONENTS];/*!< information of all opponents   */
    PlayerObject  UnknownPlayers[MAX_TEAMMATES+MAX_OPPONENTS];
    /*!< info unknown players are stored
         here til mapped to known player */
    int           iNrUnknownPlayers;       /*!< number of unknown players      */

    // fixed objects
    FixedObject   Flags[MAX_FLAGS];        /*!< all flag information           */
    FixedObject   Lines[MAX_LINES];        /*!< all line information           */

    ////////////////////////// LOCALIZATION INFORMATION /////////////////////////

    static const int iNrParticlesAgent = 100; /*!<nr of particles used to store
                                                agent position               */
    static const int iNrParticlesBall  = 100; /*! nr of particles used to store
                                                ball position and velocity   */
    VecPosition   particlesPosAgent[iNrParticlesAgent]; /*!< particles to store
                                                         agent position      */
    VecPosition   particlesPosBall[iNrParticlesBall];   /*! particles to store
                                                         ball position       */
    VecPosition   particlesVelBall[iNrParticlesBall];   /*! particles to store
                                                         ball velocity       */
    double        dTotalVarVel;
    double        dTotalVarPos;
    ////////////////////////// PREVIOUS ACTION INFORMATION //////////////////////

    // arrays needed to keep track of actually performed actions.
    SoccerCommand queuedCommands[CMD_MAX_COMMANDS];   /*!<all performed commands,
                                                        set by ActHandler    */
    bool          performedCommands[CMD_MAX_COMMANDS];/*!< commands performed in
                                                        last cycle, index is
                                                        CommandT             */
    int           iCommandCounters[CMD_MAX_COMMANDS]; /*!< counters for all
                                                        performed commands   */


    ////////////////////////// VARIOUS //////////////////////////////////////////

    // attributes only applicable to the coach
    Time          timeCheckBall;           /*!< time bsCheckBall applies to    */
    BallStatusT   bsCheckBall;             /*!< state of the ball              */

    // synchronization
#ifdef WIN32
    CRITICAL_SECTION mutex_newInfo;       /*!< mutex to protect bNewInfo       */
    HANDLE           event_newInfo;       /*!< event for bNewInfo              */
#else
    pthread_mutex_t  mutex_newInfo;       /*!< mutex to protect bNewInfo       */
    pthread_cond_t   cond_newInfo;        /*!< cond variable for bNewInfo      */
#endif
    bool          m_bRecvThink;           /*!< think received in sync. mode    */

    // communication
    char          m_strPlayerMsg[MAX_MSG];/*!< message communicated by player  */
    int           m_iCycleInMsg;          /*!< cycle contained in message      */
    Time          m_timePlayerMsg;        /*!< time corresponding to player msg*/
    int           m_iMessageSender;       /*!< player who send message         */
    char          m_strCommunicate[MAX_SAY_MSG];/*!< string for communicating  */

    // attention to
    ObjectT       m_objFocus;             /*!< object to which is focused.     */

    // offside line
    double        m_dCommOffsideX;        /*!< communicated offside line       */
    Time          m_timeCommOffsideX;     /*!< time Communicated offside line  */

    // feature information
    Feature       m_features[MAX_FEATURES];/*!< features applied to cur. cycle.*/

    // other
    bool          m_bPerformedKick;       /*!<Indicates whether ball was kicked*/

    // heterogeneous player information
    set<ObjectT>  m_setSubstitutedOpp;    /*!< Set of substituted opp players. */

    // upcoming view angle/quality
    SoccerCommand m_changeViewCommand;    /*!< last sent change_view command   */

    // side of penalty shootout
    SideT         m_sidePenalty;

public:
    bool          isOpponentAtAngleEx( AngDeg angA, AngDeg angB, double dDist);
    bool          m_bWasCollision;        /*!<Indicates whether it is collision*/
    Time          m_timeLastCollision;    /*!< Last collision time             */

    // statistics
    int           iNrHoles;                /*!< nr of holes recorded           */
    int           iNrOpponentsSeen;        /*!< total nr of opponents seen     */
    int           iNrTeammatesSeen;        /*!< total nr of teammates seen     */

    // last received messages
    char          strLastSeeMessage  [MAX_MSG];  /*!< Last see message         */
    char          strLastSenseMessage[MAX_MSG];  /*!< Last sense_body message  */
    char          strLastHearMessage [MAX_MSG];  /*!< Last hear message        */
    //!new add
    bool   bSynchSeeMode;
    /***************************************************************************/
    /*************************** OPERATIONS ************************************/
    /***************************************************************************/

    ////////////////////////// DIRECT RETRIEVAL (WorldModel.C) //////////////////

private:
    // private methods
    Object*       getObjectPtrFromType       ( ObjectT        o           );

public:

    // get and set methods of attributes in WorldModel itself
    void          setTimeLastCatch           ( Time           time        );
    int           getTimeSinceLastCatch      (                            );
    bool          setTimeLastRefereeMessage  ( Time           time        );
    Time          getTimeLastRefereeMessage  (                            );
    Time          getCurrentTime             (                            );
    int           getCurrentCycle            (                            );
    bool          isTimeStopped              (                            );
    bool          isLastMessageSee           (                            )const;
    Time          getTimeLastSeeGlobalMessage(                            )const;
    bool          setTimeLastSeeGlobalMessage( Time           time        );
    Time          getTimeLastSeeMessage      (                            )const;
    Time          getTimeLastRecvSeeMessage  (                            )const;
    bool          setTimeLastSeeMessage      ( Time           time        );
    Time          getTimeLastSenseMessage    (                            )const;
    Time          getTimeLastRecvSenseMessage(                            )const;
    bool          setTimeLastSenseMessage    ( Time           time        );
    Time          getTimeLastHearMessage     (                            )const;
    bool          setTimeLastHearMessage     ( Time           time        );
    int           getPlayerNumber            (                            )const;
    bool          setPlayerNumber            ( int            i           );
    SideT         getSide                    (                            )const;
    bool          setSide                    ( SideT          s           );
    const char*   getTeamName                (                            )const;
    bool          setTeamName                ( char           *str        );
    PlayModeT     getPlayMode                (                            )const;
    bool          setPlayMode                ( PlayModeT      pm          );
    int           getGoalDiff                (                            )const;
    int           addOneToGoalDiff           (                            );
    int           subtractOneFromGoalDiff    (                            );
    int           getNrOfCommands            ( CommandT       c           )const;
    bool          setNrOfCommands            ( CommandT       c,
            int            i           );
    Time          getTimeCheckBall           (                            )const;
    bool          setTimeCheckBall           ( Time           time        );
    BallStatusT   getCheckBallStatus         (                            )const;
    bool          setCheckBallStatus         ( BallStatusT    bs          );
    bool          getRecvThink               (                            );
    char*         getCommunicationString     (                            );
    bool          setCommunicationString     ( char*          srt         );
    ObjectT       getObjectFocus             (                            );
    bool          setObjectFocus             ( ObjectT        obj         );
    
    // iterate over a specific object set
    ObjectT       iterateObjectStart         ( int            &iIndex,
            ObjectSetT     g,
            double         dConf = -1.0,
            bool           bForward = false );
    ObjectT       iterateObjectNext          ( int            &iIndex,
            ObjectSetT     g,
            double         dConf = -1.0,
            bool           bForward = false );
    void          iterateObjectDone          ( int            &iIndex          );

    // get and set methods for agent information
    ObjectT       getAgentObjectType         (                            )const;
    int           getAgentIndex              (                            )const;
    bool          setAgentObjectType         ( ObjectT        o           );
    AngDeg        getAgentBodyAngleRelToNeck (                            )const;
    AngDeg        getAgentGlobalNeckAngle    (                            )const;
    AngDeg        getAgentGlobalBodyAngle    (                            );
    Stamina       getAgentStamina            (                            )const;
    TiredNessT    getAgentTiredNess          (                            )const;
    double        getAgentEffort             (                            )const;
    VecPosition   getAgentGlobalVelocity     (                            )const;
    double        getAgentSpeed              (                            )const;
    VecPosition   getAgentGlobalPosition     (                            )const;
    bool          setAgentViewAngle          ( ViewAngleT     va          ) ;
    ViewAngleT    getAgentViewAngle          (                            )const;
    bool          setAgentViewQuality        ( ViewQualityT   vq          ) ;
    ViewQualityT  getAgentViewQuality        (                            )const;
    double        getAgentViewFrequency      ( ViewAngleT     va = VA_ILLEGAL,
            ViewQualityT   vq = VQ_ILLEGAL  );
    bool          getAgentArmMovable         (                                 );
    VecPosition   getAgentArmPosition        (                                 );
    int           getAgentArmExpires         (                                 );

    // get methods for ball information
    VecPosition   getBallPos                 (                                 );
    double        getBallSpeed               (                                 );
    AngDeg        getBallDirection           (                                 );

    // get method for different information about a specific object
    Time          getTimeGlobalPosition      ( ObjectT        o                );
    VecPosition   getGlobalPosition          ( ObjectT        o                );
    Time          getTimeGlobalVelocity      ( ObjectT        o                );
    VecPosition   getGlobalVelocity          ( ObjectT        o                );
    double        getRelativeDistance        ( ObjectT        o                );
    VecPosition   getRelativePosition        ( ObjectT        o                );
    AngDeg        getRelativeAngle           ( ObjectT        o,
            bool           bWithBody = false);
    Time          getTimeGlobalAngles        ( ObjectT        o                );
    AngDeg        getGlobalBodyAngle         ( ObjectT        o                );
    AngDeg        getGlobalNeckAngle         ( ObjectT        o                );
    AngDeg        getGlobalAngle             ( ObjectT        o                );
    double        getConfidence              ( ObjectT        o                );
    bool          isKnownPlayer              ( ObjectT        o                );
    ObjectT       getOppGoalieType           (                                 );
    ObjectT       getOwnGoalieType           (                                 );
    Time          getTimeLastSeen            ( ObjectT        o                );
    Time          getTimeChangeInformation   ( ObjectT        o                );
    VecPosition   getGlobalPositionLastSee   ( ObjectT        o                );
    Time          getTimeGlobalPositionLastSee( ObjectT       o                );
    VecPosition   getGlobalVelocityLastSee   ( ObjectT        o                );
    AngDeg        getGlobalBodyAngleLastSee  ( ObjectT        o                );
    int           getTackleExpires           ( ObjectT        o =OBJECT_ILLEGAL);
    AngDeg        getGlobalArmDirection      ( ObjectT        o                );
    Time          getTimeGlobalArmDirection  ( ObjectT        o                );
    double        getProbTackleSucceeds      ( ObjectT        o = OBJECT_ILLEGAL,
            int            iExtraCycles = 0,
            VecPosition    *pos = NULL      );
    double        getProbTackleClosestOpp    ( int            iExtraCycles = 0 );
    list<ObjectT> getListCloseOpponents      ( VecPosition    pos,
            double         dDist = 15       );

    // set methods for objects
    bool          setIsKnownPlayer           ( ObjectT        o,
            bool           isKnownPlayer    );
    bool          setTimeLastSeen            ( ObjectT        o,
            Time           time             );
    bool          setHeteroPlayerType        ( ObjectT        o,
            int            iPlayer          );

    // formation information
    PlayerT       getPlayerType              ( ObjectT        o =OBJECT_ILLEGAL);
    bool          isInPlayerSet              ( ObjectT        o,
            PlayerSetT     ps               );

    // get method for information about goals
    VecPosition   getPosOpponentGoal         (                                 );
    VecPosition   getPosOwnGoal              (                                 );
    double        getRelDistanceOpponentGoal (                                 );
    AngDeg        getRelAngleOpponentGoal    (                                 );
    ObjectT       getLastOpponentDefender    ( double         *dX = NULL       );

    // get and methods about fixed specifications (heterogeneous players)
    HeteroPlayerSettings getInfoHeteroPlayer ( int            iIndex           );
    HeteroPlayerSettings getHeteroInfoPlayer ( ObjectT        obj              );
    int           getHeteroPlayerType        ( ObjectT        obj              );
    bool          setSubstitutedOpp          ( ObjectT        obj              );
    ObjectT       getSubstitutedOpp          (                                 );
    double        getDashPowerRate           ( ObjectT        obj              );
    double        getPlayerSpeedMax          ( ObjectT        obj              );
    double        getPlayerDecay             ( ObjectT        obj              );
    double        getMaximalKickDist         ( ObjectT        obj              );
    double        getStaminaIncMax           ( ObjectT        obj              );
    double        getPlayerSize              ( ObjectT        obj              );
    double        getInertiaMoment           ( ObjectT        obj              );
    double        getEffortMax               ( ObjectT        obj              );
    double        getEffectiveMaxSpeed       ( ObjectT        obj,
            bool           bWithNoise =false);

    // get method about previous commands
    bool          isQueuedActionPerformed    (                                 );

    // methods that return truth values about current play mode
    bool          isFreeKickUs               (  PlayModeT pm = PM_ILLEGAL      );
    bool          isFreeKickThem             (  PlayModeT pm = PM_ILLEGAL      );
    bool          isCornerKickUs             (  PlayModeT pm = PM_ILLEGAL      );
    bool          isCornerKickThem           (  PlayModeT pm = PM_ILLEGAL      );
    bool          isOffsideUs                (  PlayModeT pm = PM_ILLEGAL      );
    bool          isOffsideThem              (  PlayModeT pm = PM_ILLEGAL      );
    bool          isKickInUs                 (  PlayModeT pm = PM_ILLEGAL      );
    bool          isKickInThem               (  PlayModeT pm = PM_ILLEGAL      );
    bool          isFreeKickFaultUs          (  PlayModeT pm = PM_ILLEGAL      );
    bool          isFreeKickFaultThem        (  PlayModeT pm = PM_ILLEGAL      );
    bool          isKickOffUs                (  PlayModeT pm = PM_ILLEGAL      );
    bool          isKickOffThem              (  PlayModeT pm = PM_ILLEGAL      );
    bool          isBackPassUs               (  PlayModeT pm = PM_ILLEGAL      );
    bool          isBackPassThem             (  PlayModeT pm = PM_ILLEGAL      );
    bool          isGoalKickUs               (  PlayModeT pm = PM_ILLEGAL      );
    bool          isGoalKickThem             (  PlayModeT pm = PM_ILLEGAL      );
    bool          isBeforeKickOff            (  PlayModeT pm = PM_ILLEGAL      );
    bool          isDeadBallUs               (  PlayModeT pm = PM_ILLEGAL      );
    bool          isDeadBallThem             (  PlayModeT pm = PM_ILLEGAL      );
    bool          isPenaltyUs                (  PlayModeT pm = PM_ILLEGAL      );
    bool          isPenaltyThem              (  PlayModeT pm = PM_ILLEGAL      );
    bool          isFullStateOn              (  SideT     s  = SIDE_ILLEGAL    );

    // various methods
    bool          setChangeViewCommand       ( SoccerCommand  soc              );
    SoccerCommand getChangeViewCommand       (                                 );

    SideT         getSidePenalty             (                                 );
    bool          setSidePenalty             ( SideT          side             );

    ////////////////////////// UPDATE METHODS (WorldModelUpdate.C)///////////////

    // processing new information about the objects (filling World Model).
    void          processSeeGlobalInfo       ( ObjectT        o,
            Time           time,
            VecPosition    pos,
            VecPosition    vel,
            AngDeg         angBody,
            AngDeg         angNeck          );
    bool          processNewAgentInfo        ( ViewQualityT   vq,
            ViewAngleT     va,
            double         dStamina,
            double         dEffort,
            double			dCapacity,/*! misol.gao fixed for v13.2 */
            double         dSpeed,
            AngDeg         angSpeed,
            AngDeg         angHeadAngle,
            int            iTackleExpires,
            int            iArmMovable,
            int            iArmExpires,
            VecPosition    posArm           );
    void           processNewObjectInfo       ( ObjectT        o,
            Time           time,
            double         dDist,
            int            iDir,
            double         dDistChange,
            double         dDirChange,
            AngDeg         angRelBodyAng,
            AngDeg         angRelNeckAng,
            bool           isGoalie,
            ObjectT        objMin,
            ObjectT        objMax,
            double         dPointDir,
            bool           isTackling       );
    bool           processPerfectHearInfo     ( ObjectT        o,
            VecPosition    pos,
            double         dConf,
            bool           bIsGoalie=0      );
    bool           processPerfectHearInfoBall ( VecPosition    pos,
            VecPosition    vel,
            double         dConf            );
    bool           processUnsureHearInfo      ( ObjectT        o,
            VecPosition    pos,
            double         dConf            );
    bool           processNewHeteroPlayer     ( int            iIndex,
            double         dPlayerSpeedMax,
            double         dStaminaIncMax,
            double         dPlayerDecay,
            double         dInertiaMoment,
            double         dDashPowerRate,
            double         dPlayerSize,
            double         dKickableMargin,
            double         dKickRand,
            double         dExtraStamina,
            double         dEffortMax,
            double         dEffortMin       );
    void          processCatchedBall         ( RefereeMessageT rm,
            Time           time             );
    void          processQueuedCommands      ( SoccerCommand  commands[],
            int            iCommands        );
    bool          storePlayerMessage         ( int            iPlayer,
            char           *strMsg,
            int            iCycle           );
    bool          processPlayerMessage       (                                 );
    bool          processRecvThink           ( bool           b                );

    // update methods (defined in WorldModelUpdate.C)
    bool          updateAll                  (                                 );

    // update with prediction for new cycle
    bool          updateAfterSenseMessage    (                                 );

private:
    // update with new visual information
    void          processLastSeeMessage      (                                 );
    bool          updateAfterSeeMessage      (                                 );
    bool          updateAgentObjectAfterSee  (                                 );
    bool          updateDynamicObjectAfterSee( ObjectT        o                );

    // update new cycle
    void          processLastSenseMessage    (                                 );
    bool          updateAgentAndBallAfterSense(                                );
    bool          updateBallAfterKick        ( SoccerCommand  soc              );
    bool          updateDynamicObjectForNextCycle( ObjectT    o,
            int            iCycle           );
    bool          updateBallForCollision     ( VecPosition    posAgent         );

    // update from relative to global and vice versa
    bool          updateRelativeFromGlobal   (                                 );
    bool          updateObjectRelativeFromGlobal( ObjectT     o                );

    // methods to determine agent state from perceived information
    bool          calculateStateAgent        ( VecPosition    *posGlobal,
            VecPosition    *velGlobal,
            AngDeg         *angGlobal       );
    void          initParticlesAgent         ( AngDeg         angGlobal        );
    void          initParticlesAgent         ( VecPosition    posInitital      );
    int           checkParticlesAgent        ( AngDeg         angGlobalNeck    );
    void          updateParticlesAgent       ( VecPosition    vel,
            bool           bAfterSense      );
    VecPosition   averageParticles           ( VecPosition    posArray[],
            int            iLength          );
    void          resampleParticlesAgent     ( int            iLeft            );
    bool          calculateStateAgent2       ( VecPosition    *posGlobal,
            VecPosition    *velGlobal,
            AngDeg         *angGlobal       );
    bool          calculateStateAgent3       ( VecPosition    *posGlobal,
            VecPosition    *velGlobal,
            AngDeg         *angGlobal       );
    VecPosition   calculatePosAgentWith2Flags( ObjectT        objFlag1,
            ObjectT        objFlag2         );
    AngDeg        calculateAngleAgentWithPos ( VecPosition    pos              );


    // methods to determine ball state from perceived information
    bool          calculateStateBall         ( VecPosition    *posGlobal,
            VecPosition    *velGlobal       );
    void          initParticlesBall         (  VecPosition    posArray[],
            VecPosition    velArray[],
            int            iLength          );
    void          checkParticlesBall        (  VecPosition    posArray[],
            VecPosition    velArray[],
            int            iLength,
            int            *iNrLeft         );
    void          updateParticlesBall       (  VecPosition    posArray[],
            VecPosition    velArray[],
            int            iLength,
            double         dPower,
            AngDeg         ang              );
    void          resampleParticlesBall      ( VecPosition    posArray[],
            VecPosition    velArray[],
            int            iLength,
            int            iLeft            );
    ObjectT       getMaxRangeUnknownPlayer   ( ObjectT        obj,
            char*          strMsg           );
    VecPosition   calculateVelocityDynamicObject( ObjectT     o                );


    // methods to determine player state from perceived information
    bool          calculateStatePlayer       ( ObjectT        o,
            VecPosition    *posGlobal,
            VecPosition    *velGlobal       );

    // methods to calculate noise range (quantized noise)
    bool          getMinMaxDistQuantizeValue ( double         dInput,
            double         *dMin,
            double         *dMax,
            double         x1,
            double         x2               );
    bool          getMinMaxDirChange         ( double         dOutput,
            double         *dMin,
            double         *dMax,
            double         x1               );
    bool          getMinMaxDistChange        ( double         dOutput,
            double         dDist,
            double         *dMin,
            double         *dMax,
            double         x1,
            double         xDist1,
            double         xDist2           );
    double        invQuantizeMin             ( double         dOutput,
            double         dQuantizeStep    );
    double        invQuantizeMax             ( double         dOutput,
            double         dQuantizeStep    );


public:
    // various update methods
    void          mapUnknownPlayers          ( Time           time             );
    bool          updateSSToHeteroPlayerType ( int            iPlayerType      );
    bool          resetTimeObjects           (                                 );
    void          removeGhosts               (                                 );


    ////////////////////////// PREDICTIONS (WorldModelPredict.C) ////////////////

    // predictions of worldmodel about future states of different objects
    bool          predictStateAfterCommand   ( SoccerCommand  com,
            VecPosition    *pos,
            VecPosition    *vel,
            AngDeg         *angGlobalBody,
            AngDeg         *angGlobalNeck,
            ObjectT        obj=OBJECT_ILLEGAL,
            Stamina        *sta = NULL      );
    bool          predictAgentStateAfterCommand(SoccerCommand com,
            VecPosition    *pos,
            VecPosition    *vel,
            AngDeg         *angBody,
            AngDeg         *angNeck,
            Stamina        *sta             );
    bool          predictObjectStateAfterCommand( ObjectT     obj,
            SoccerCommand com,
            VecPosition    *pos,
            VecPosition    *vel,
            AngDeg         *angBody,
            AngDeg         *angNeck,
            Stamina        *sta             );
    VecPosition   predictAgentPosAfterCommand( SoccerCommand  com              );
    void          predictStateAfterDash      ( double         dActualPower,
            VecPosition    *pos,
            VecPosition    *vel,
            Stamina        *sta,
            double         dDirection,
            ObjectT        o=OBJECT_ILLEGAL);
    void          predictStateAfterTurn      ( AngDeg         dSendAngle,
            VecPosition    *pos,
            VecPosition    *vel,
            AngDeg         *angBody,
            AngDeg         *angNeck,
            ObjectT        obj=OBJECT_ILLEGAL,
            Stamina        *sta = NULL      );
    void          predictBallInfoAfterCommand( SoccerCommand  soc,
            VecPosition    *pos = NULL,
            VecPosition    *vel = NULL      );
    VecPosition   predictPosAfterNrCycles    ( ObjectT        o,
            double         dCycles,
            int            iDashPower = 100,
            VecPosition    *posIn = NULL,
            VecPosition    *velIn = NULL,
            bool           bUpdate = true   );
    VecPosition   predictAgentPos            ( int            iCycles,
            int            iDashPower = 0   );
    VecPosition   predictFinalAgentPos       ( VecPosition    *pos = NULL,
            VecPosition    *vel = NULL      );
    int           predictNrCyclesForDistance ( ObjectT        o,
            double         dDist,
            double         dCurSpeed        );
    int           predictNrCyclesToPoint     ( ObjectT        o,
            VecPosition    posTo            );
    int           predictNrCyclesToObject    ( ObjectT        objFrom,
            ObjectT        objTo            );
    void          predictStaminaAfterDash    ( double         dPower,
            Stamina        *sta             );
    SoccerCommand predictCommandTurnTowards  ( ObjectT        obj,
            VecPosition    posTo,
            int            iCycles,
            double         dDistBack,
            bool           bMoveBack,
            VecPosition    *pos = NULL,
            VecPosition    *vel = NULL,
            AngDeg         *angBody = NULL  );
    SoccerCommand predictCommandToMoveToPos  ( ObjectT        obj,
            VecPosition    posTo,
            int            iCycles,
            double         dDistBack = 2.5,
            bool           bMoveBack = false,
            VecPosition    *pos = NULL,
            VecPosition    *vel = NULL,
            AngDeg         *angBody = NULL  );
    SoccerCommand predictCommandToInterceptBall( ObjectT      obj,
            SoccerCommand  soc,
            int            *iCycles = NULL ,
            VecPosition    *posIntercept=NULL,
            VecPosition    *pos = NULL,
            VecPosition    *vel = NULL,
            AngDeg         *angBody = NULL  );
    bool          isCollisionAfterCommand    ( SoccerCommand  soc              );


    ////////////////////////// HIGH-LEVEL METHODS (WorldModelHighLevel.C) ///////

    // methods that return the number of players in a certain area of the field
    int           getNrInSetInRectangle      ( ObjectSetT     objectSet,
            Rect           *rect = NULL     );
    int           getNrInSetInCircle         ( ObjectSetT     objectSet,
            Circle         c                );
    int           getNrInSetInCone           ( ObjectSetT     objectSet,
            double         dWidth,
            VecPosition    start,
            VecPosition    end              );
    bool          isEmptySpace               ( ObjectT        obj,
            AngDeg         ang,
            double         dDist = 4.0      );
    bool          coordinateWith             ( ObjectT        obj              );

    // method that return the closest or fastest player to a certain pos orobject
    ObjectT       getClosestInSetTo          ( ObjectSetT     objectSet,
            ObjectT        o ,
            double         *dDist = NULL,
            double         dConfThr = -1.0  );
    ObjectT       getClosestInSetTo          ( ObjectSetT     objectSet,
            VecPosition    pos,
            double         *dDist = NULL,
            double         dConfThr = -1.0  );
    ObjectT       getClosestInSetTo          ( ObjectSetT     objectSet,
            Line           l,
            VecPosition    pos1,
            VecPosition    pos2,
            double         *dDistToLine= NULL,
            double         *dDistPos1To=NULL);
    ObjectT       getClosestRelativeInSet    ( ObjectSetT     set,
            double         *dDist = NULL    );
    ObjectT       getSecondClosestInSetTo    ( ObjectSetT     objectSet,
            ObjectT        o,
            double         *dDist = NULL,
            double         dConfThr = -1.0  );
    ObjectT       getSecondClosestRelativeInSet( ObjectSetT   set,
            double         *dDist = NULL    );
    void          createInterceptFeatures    (                                 );
    ObjectT       getFastestInSetTo          ( ObjectSetT     objectSet,
            ObjectT        o,
            int            *iCycles = NULL  );
    ObjectT       getFastestInSetTo          ( ObjectSetT     objectSet,
            VecPosition    pos,
            VecPosition    vel,
            double         dDecay,
            int            *iCycles = NULL  );
    ObjectT       getFurthestInSetTo         ( ObjectSetT     objectSet,
            ObjectT        o ,
            double         *dDist = NULL,
            double         dConfThr = -1.0  );
    ObjectT       getFurthestRelativeInSet   ( ObjectSetT     set,
            double         *dDist = NULL    );
    VecPosition   getPosClosestOpponentTo    ( double         *dDist = NULL,
            ObjectT        o =OBJECT_ILLEGAL);
    double        getMaxTraveledDistance     ( ObjectT        o                );
    // various methods dealing with sets of objects
    ObjectT       getFirstEmptySpotInSet     ( ObjectSetT     objectSet,
            int            iUnknownPlayer=-1);

    // boolean methods that indicate whether an object fulfills a  constraint
    bool          isVisible                  ( ObjectT        o                );
    bool          isBallKickable             (                                 );
    bool          isBallCatchable            (                                 );
    bool          isBallHeadingToGoal        (                                 );
    bool          isBallInOurPossesion       (                                 );
    bool          isBallInOwnPenaltyArea     (                                 );
    bool          isInOwnPenaltyArea         ( VecPosition    pos              );
    bool          isInTheirPenaltyArea       ( VecPosition    pos              );
    bool          isConfidenceGood           ( ObjectT                         );
    bool          isConfidenceVeryGood       ( ObjectT                         );
    bool          isOnside                   ( ObjectT                         );
    bool          isOpponentAtAngle          ( AngDeg         ang,
            double         dDist            );

    // various methods that supply specific information about the field
    Time          getTimeFromConfidence      ( double         dConf            );
    double        getOffsideX                ( bool           bIncludeComm=true);
    VecPosition   getOuterPositionInField    ( VecPosition    pos,
            AngDeg         ang,
            double         dDist = 3.0,
            bool           bWithPenalty=true);
    AngDeg        getDirectionOfWidestAngle  ( VecPosition    posOrg,
            AngDeg         angMin,
            AngDeg         angMax,
            AngDeg         *ang,
            double         dDist            );
    bool          isInField                  ( VecPosition    pos,
            double         dMargin = 1      );
    bool          isBeforeGoal               ( VecPosition    pos              );

    // strategic positioning
    VecPosition   getStrategicPosition      ( ObjectT        obj,
            FormationT     ft = FT_ILLEGAL   );
    VecPosition   getStrategicPosition      ( int            iPlayer = -1,
            FormationT     ft = FT_ILLEGAL   );
    VecPosition   getMarkingPosition        ( VecPosition    pos,
            double         dDist,
            MarkT          mark              );
    int           getClosestPlayerInFormationTo( VecPosition pos,
            bool           bWithGoalie = 1,
            ObjectT        obj=OBJECT_ILLEGAL,
            PlayerSetT     ps = PS_ALL,
            FormationT     ft = FT_ILLEGAL   );


    // methods  for computing the actual argument for a soccer command.
    double        getActualKickPowerRate     (                                 );
    double        getKickPowerForSpeed       ( double         dDesiredSpeed    );
    double        getKickSpeedToTravel       ( double         dDistance,
            double         dEndSpeed        );
    double        getFirstSpeedFromEndSpeed  ( double         dEndSpeed,
            double         dCycles,
            double         dDecay = -1.0    );
    double        getFirstSpeedFromDist      ( double         dDist,
            double         dCycles,
            double         dDecay = -1.0    );
    double        getEndSpeedFromFirstSpeed  ( double         dFirstSpeed,
            double         dCycles          );
    AngDeg        getAngleForTurn            ( AngDeg         angDesiredAngle,
            double         dSpeed,
            ObjectT        o=OBJECT_ILLEGAL);
    AngDeg        getActualTurnAngle         ( AngDeg         angActualAngle,
            double         dSpeed,
            ObjectT        o=OBJECT_ILLEGAL);
    double        getPowerForDash            ( VecPosition    posRelTo,
            AngDeg         angBody,
            VecPosition    vel,
            double         dEffort,
            int            iCycles = 1      );

    //!new add
    void   setSynchSeeMode();
    bool   isSynchSeeMode();
    ////////////////////////// VARIOUS (WorldModel.C) ///////////////////////////

    // constructor
    WorldModel                               ( ServerSettings *ss,
            PlayerSettings *ps,
            Formations     *fs              );
    ~WorldModel                              (                                 );

    // print information about WorldModel
    void          show                       ( ostream        &os = cout       );
    void          show                       ( ObjectSetT     set,
            ostream        &os = cout       );
    void          showQueuedCommands         ( ostream        &os = cout       );
    void          show                       ( ObjectT        o,
            ostream        &os = cout        );

    // methods that deal with timing information
    bool          waitForNewInformation      (                                 );

    // methods that deal with debugging
    void          logObjectInformation       ( int            iLogLevel,
            ObjectT        o                );
    void          logDrawInfo                ( int            iLogLevel        );
    void          logCoordInfo               ( int            iLogLevel        );
    bool          logCircle                  ( int            iLogLevel,
            VecPosition    pos,
            double         dRadius,
            bool           bAll = false     );
    bool          logLine                    ( int            iLogLevel,
            VecPosition    pos1,
            VecPosition    pos2,
            bool           bAll = false     );
    bool          logDrawBallInfo            ( int            iLogLevel        );
    void          drawCoordinationGraph      (                                 );

    // specific static variables
    char m_colorPlayers[11][8];       /*!< color information with which each
                                         player should draw its relevant info*/
    int m_iMultX;                     /*!< This variable denotes with which value
                                         the x coordinates of the draw
                                         information should be multiplied in
                                         order to convert the coordinates to
                                         the coordinate system of the monitor*/
    int m_iMultY;                     /*!< This variable denotes with which value
                                         the y coordinates of the draw
                                         information should be multiplied in
                                         order to convert the coordinates to
                                         the coordinate system of the monitor*/

    bool           isFeatureRelevant        ( FeatureT        type             );
    Feature        getFeature               ( FeatureT        type             );
    bool           setFeature               ( FeatureT        type,
            Feature         feature          );
};

#endif
