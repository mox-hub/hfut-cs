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

/*! \file Objects.h
<pre>
<b>File:</b>          Objects.h
<b>Project:</b>       Robocup Soccer Simulation Team: UvA Trilearn
<b>Authors:</b>       Jelle Kok
<b>Created:</b>       1/12/2000
<b>Last Revision:</b> $ID$
<b>Contents:</b>      class declarations Object, DynamicObject, FixedObject,
               PlayerObject, BallObject and Stamina
<hr size=2>
<h2><b>Changes</b></h2>
<b>Date</b>             <b>Author</b>          <b>Comment</b>
1/12/2001        Jelle Kok       Initial version created
</pre>
*/
#ifndef _OBJECTS_
#define _OBJECTS_

#include "SoccerTypes.h"  // needed for ObjectT

/*****************************************************************************/
/********************** CLASS OBJECT *****************************************/
/*****************************************************************************/

/*! Class Object contains RoboCup information that is available for all objects
    in the simulation. All (relative) information is relative to an agent as
    declared in AgentObject. Update of an object (or one of the subclasses)
    happens by calling the standard get and set methods available in these
    classes. Calculations on these attributes do not occur in these classes,
    but in the update methods of the WorldModel. */
class Object
{
protected:
    ObjectT     objectType;             /*!< Type of this object               */
    Time        timeLastSeen;           /*!< Time last see message has arrived */

    VecPosition posGlobal;              /*!< Global position in the field      */
    Time        timeGlobalPosition;     /*!< Server time of global position    */
    VecPosition posRelative;            /*!< Relative position of the object   */
    Time        timeRelativePosition;   /*!< Server time of relative position  */
    VecPosition posGlobalLastSee;       /*!< Global position of last see msg   */
    Time        timeGlobalPosDerivedFromSee;/*!< Time pos derived from see msg */


public:
    Object( );

    /*! abstract function that should be defined in a subclass */
    virtual void show( ostream& os = cout ) = 0;

    // non-standard get and set methods (all defined here)
    AngDeg      getRelativeAngle              (                                );
    double      getRelativeDistance           (                                );
    double      getConfidence                 ( Time        time               );

    // standard get and set methods
    bool        setType                       ( ObjectT     o                  );
    ObjectT     getType                       (                          ) const;

    bool        setRelativePosition           ( double      dDist,
            AngDeg      dAng,
            Time        time               );
    bool        setRelativePosition           ( VecPosition v,
            Time        time               );
    VecPosition getRelativePosition           (                          ) const;

    bool        setTimeRelativePosition       ( Time        time               );
    Time        getTimeRelativePosition       (                          ) const;

    bool        setGlobalPosition             ( VecPosition p,
            Time        time               );
    VecPosition getGlobalPosition             (                          ) const;

    bool        setTimeGlobalPosition         ( Time        time               );
    Time        getTimeGlobalPosition         (                          ) const;

    bool        setGlobalPositionLastSee      ( VecPosition p,
            Time        time               );
    VecPosition getGlobalPositionLastSee      (                          ) const;

    bool        setTimeGlobalPosDerivedFromSee( Time        time               );
    Time        getTimeGlobalPosDerivedFromSee(                          ) const;

    bool        setTimeLastSeen               ( Time        time               );
    Time        getTimeLastSeen               (                          ) const;

};

/*****************************************************************************/
/********************** CLASS FIXEDOBJECT ************************************/
/*****************************************************************************/

/*! Class FixedObject contains RoboCup information that is available for
    objects that cannot move (flags, goals, lines). No additional information
    is added to the superclass Object. */
class FixedObject : public Object
{
public:
    // specific methods
    VecPosition getGlobalPosition( SideT s, double dGoalWidth = 14.02 ) const;
    AngDeg      getGlobalAngle   ( SideT s                                  );

    void show                    ( ostream & os = cout                      );
};

/*****************************************************************************/
/********************** CLASS DYNAMICOBJECT **********************************/
/*****************************************************************************/

/*! Class DynamicObject contains RoboCup information that is available for
    objects that can move (players, ball). Different variables are added to
    the superclass Object */
class DynamicObject: public Object
{
protected:

    // global velocity information
    VecPosition vecGlobalVelocity;      /*!< Global velocity of the player     */
    Time        timeGlobalVelocity;     /*!< Time of the corresponding velocity*/

    // sensor information
    double      dRelativeDistanceChange;/*!< Relative distance change          */
    double      dRelativeAngleChange;   /*!< Relative angle change             */
    Time        timeChangeInformation;  /*!< Time of change information        */

    VecPosition vecGlobalVelocityLastSee;/*!< vel. derived from last see       */


public:
    DynamicObject( );

    // standard get and set methods
    bool        setRelativeDistanceChange( double      d,   Time time );
    double      getRelativeDistanceChange(                            ) const;

    bool        setRelativeAngleChange   ( double      d,   Time time );
    double      getRelativeAngleChange   (                            ) const;

    bool        setTimeChangeInformation ( Time        time           );
    Time        getTimeChangeInformation (                            )  const;

    bool        setGlobalVelocity        ( VecPosition v,   Time time );
    VecPosition getGlobalVelocity        (                            )  const;
    double      getSpeed                 (                            )  const;

    bool        setTimeGlobalVelocity    ( Time        time           );
    Time        getTimeGlobalVelocity    (                            )  const;

    bool        setGlobalVelocityLastSee ( VecPosition vec            );
    VecPosition getGlobalVelocityLastSee (                            )   const;

};

/*****************************************************************************/
/********************* CLASS PLAYEROBJECT ************************************/
/*****************************************************************************/

/*! Class PlayerObject contains RoboCup information that is available for
    players. Different variables are added to the superclass DynamicObject   */
class PlayerObject: public DynamicObject
{
protected:
    bool    isKnownPlayer;         /*!< are we sure about player number        */
    ObjectT objRangeMin;           /*!< Minimum in range of possible player obj*/
    ObjectT objRangeMax;           /*!< Maximum in range of possible player obj*/
    bool    isGoalie;              /*!< is this object a goalie                */
    AngDeg  angGlobalBodyAngle;    /*!< Global body angle                      */
    AngDeg  angGlobalNeckAngle;    /*!< Global neck angle                      */
    AngDeg  angGlobalBodyAngleLastSee;/*!< Global body angle from last see msg */
    Time    timeGlobalAngles;      /*!< Server time of global angles           */
    int     iHeteroPlayerType;     /*!< index of heterogeneous player type     */

    Time    m_timeTackle;          /*!< time tackle command was observerd      */

    AngDeg  m_angGlobalArm;        /*!< global pointing direction of arm       */
    Time    m_timeGlobalArm;       /*!< time arm last seen                     */

private:
    AngDeg  angRelativeBodyAngle;  /*!< Relative body angle to main player */
    AngDeg  angRelativeNeckAngle;  /*!< Relative neck angle to main player */
    Time    timeRelativeAngles;    /*!< Server time of relative angles     */


public:
    PlayerObject( );

    void    show( ostream & os = cout );
    void    show( const char * strTeamName, ostream & os = cout );

    // standard get and set methods
    bool    setPossibleRange     ( ObjectT objMin, ObjectT objMax );
    bool    isInRange            ( ObjectT obj,    bool bTeamFirst);
    ObjectT getMinRange          (                                );
    ObjectT getMaxRange          (                                );

    bool    setIsKnownPlayer     ( bool     b                     );
    bool    getIsKnownPlayer     (                                ) const;

    bool    setIsGoalie          ( bool     b                     );
    bool    getIsGoalie          (                                ) const;

    bool    setRelativeBodyAngle ( AngDeg   ang,   Time    time   );
    AngDeg  getRelativeBodyAngle (                                ) const;
    bool    setGlobalBodyAngle   ( AngDeg   ang,   Time    time   );
    AngDeg  getGlobalBodyAngle   (                                ) const;

    bool    setRelativeNeckAngle ( AngDeg   ang,   Time    time   );
    AngDeg  getRelativeNeckAngle (                                ) const;
    bool    setGlobalNeckAngle   ( AngDeg   ang,   Time    time   );
    AngDeg  getGlobalNeckAngle   (                                ) const;

    bool    setTimeRelativeAngles( Time     time                  );
    Time    getTimeRelativeAngles(                                ) const;
    bool    setTimeGlobalAngles  ( Time     time                  );
    Time    getTimeGlobalAngles  (                                ) const;

    bool    setGlobalBodyAngleLastSee( AngDeg ang                 );
    AngDeg  getGlobalBodyAngleLastSee(                            ) const;

    bool    setHeteroPlayerType  ( int      index                 ) ;
    int     getHeteroPlayerType  (                                ) const;

    bool    setTimeTackle        ( Time     time                  );
    Time    getTimeTackle        (                                ) const;

    bool    setGlobalArm         ( AngDeg   ang,   Time    time   );
    AngDeg  getGlobalArm         (                                ) const;

    bool    setTimeGlobalArm     ( Time     time                  );
    Time    getTimeGlobalArm     (                                ) const;

};

/*****************************************************************************/
/********************* CLASS BALLOBJECT **************************************/
/*****************************************************************************/

/*! Class PlayerObject contains RoboCup information that is available for the
    ball. No extra variables are added to superclass DynamicObject
*/
class BallObject: public DynamicObject
{
public:
    BallObject();
    void show( ostream & os = cout );

};

/*****************************************************************************/
/********************* CLASS STAMINA *****************************************/
/*****************************************************************************/

/*! The following stamina information is stored in this class.
     - actual stamina
     - recovery, determines how much stamina recovers each cycle, decreases
                  below a certain threshold (never increases)
     - effort, determines which percentage of dash power is actually used.
                  decreases below certain threshold, increases above a higher
                  threshold.
*/
class Stamina
{
    double m_dStamina;              /*!< Stamina value (>0)                */
    double m_dEffort;               /*!< Effort value (0..1)               */
    double m_dRecovery;             /*!< Recovery (0..1)                   */
    double m_dCapacity;             /*!< Capacity value (>0) /* misol.gao fixed for v13.2 */

public:
    Stamina( double dSta = 4000.0, double dEff=1.0, double dRec=1.0 , double dCap = 130600 );/*! misol.gao fixed for v13.2 */
    void         show        ( ostream & os = cout                     );

    // return value how tired an agent is
    TiredNessT   getTiredNess( double dRecDecThr,   double dStaminaMax );

    // standard get and set methods.
    double       getStamina  (                                         ) const;
    bool         setStamina  ( double d                                );
    double       getEffort   (                                         ) const;
    bool         setEffort   ( double d                                );
    double       getRecovery (                                         ) const;
    bool         setRecovery ( double d                                );
    double 		getCapacity(		 ) const;/*! misol.gao fixed for v13.2 */
    bool 		setCapacity( double d);/*! misol.gao fixed for v13.2 */
};

/*****************************************************************************/
/********************* CLASS AGENTOBJECT *************************************/
/*****************************************************************************/

/*! Class AgentObject contains RoboCup information that is available for the
    agent. New variables are declared that extend a normal PlayerObject.*/
class AgentObject: public PlayerObject
{
    ViewAngleT   viewAngle;             /*!< View angle of this agent          */
    ViewQualityT viewQuality;           /*!< View quality of this agent        */

    Stamina      stamina;               /*!< Stamina (stamina, effort, recovery*/
    VecPosition  velSpeedRelToNeck;     /*!< Velocity vector relative to neck  */
    AngDeg       angBodyAngleRelToNeck; /*!< Relative angle of body with neck  */

    VecPosition  posPositionDifference; /*!< Global pos difference with lastsee*/

    bool         m_bArmMovable;         /*!< Indicates whether can move arm.   */
    int          m_iArmExpires;         /*!< Nr. of cycles till arm expires    */
    VecPosition  m_posGlobalArm;       ;/*!< Global point to which arm point   */
    int          m_iTackleExpires;      /*!< Nr. of cycles till tackle expires */

public:
    AgentObject( double dStaminaMax = 4000 );

    void         show( ostream & os = cout );
    void         show( const char * strTeamName, ostream & os = cout );

    // standard get and set methods
    VecPosition  getPositionDifference(                  ) const;
    bool         setPositionDifference( VecPosition  v   );

    ViewAngleT   getViewAngle         (                  ) const;
    bool         setViewAngle         ( ViewAngleT   v   );

    ViewQualityT getViewQuality       (                  ) const;
    bool         setViewQuality       ( ViewQualityT v   );

    Stamina      getStamina           (                  ) const;
    bool         setStamina           ( Stamina      sta );

    VecPosition  getSpeedRelToNeck    (                  ) const;
    bool         setSpeedRelToNeck    ( VecPosition  v   );

    bool         setGlobalNeckAngle   ( AngDeg       ang );

    AngDeg       getBodyAngleRelToNeck(                  ) const;
    bool         setBodyAngleRelToNeck( AngDeg       ang );

    bool         getArmMovable        (                  ) const;
    bool         setArmMovable        ( bool         b   );

    int          getArmExpires        (                  ) const;
    bool         setArmExpires        ( int          i   );

    VecPosition  getGlobalArmPosition (                  ) const;
    bool         setGlobalArmPosition ( VecPosition  v   );

    int          getTackleExpires     (                  ) const;
    bool         setTackleExpires     ( int          i   );
} ;


#endif
