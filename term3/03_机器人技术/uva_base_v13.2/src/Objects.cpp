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

/*! \file Objects.cpp
<pre>
<b>File:</b>          Objects.cpp
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
#include"Objects.h"

#include<stdlib.h>       // needed for free
#include<iostream>       // needed for cout
#ifdef Solaris
#include <strings.h>   // needed for strdup
#else
#include <string.h>    // needed for strdup
#endif

/*****************************************************************************/
/********************* CLASS OBJECT ******************************************/
/*****************************************************************************/

/*! This constructor creates an object, all variables are initialized by
   default (illegal) values */
Object::Object( )
{
    objectType                   = OBJECT_ILLEGAL;
}

/*! This method returns the relative angle of this object to the agent.
    This equals the angle of the relative position vector.
    \return relative angle to this object */
AngDeg Object::getRelativeAngle( )
{
    return VecPosition::normalizeAngle(posRelative.getDirection());
}

/*! This method returns the relative distance to this object.
    This equals the magnitude of the relative position vector.
    \return relative distance to this object */
double Object::getRelativeDistance( )
{
    return posRelative.getMagnitude();
}

/*! This method returns the confidence of the information of this object.
    The confidence is related to the last time this object was seen and
    the specified time (normally the time of the last received message).
    \param time current time to compare with time object was last seen
    \return confidence factor of this object */
double Object::getConfidence( Time time )
{
    if ( timeLastSeen.getTime() == -1 )
        return 0.0;
    double dConf =
        max( 0.0, 1.0-(double)(time.getTimeDifference(timeLastSeen))/100.0);
    if ( dConf > 1.0 )
        return 0.0;
    return dConf;
}


/*! This method sets the type of this object (i.e. OBJECT_BALL, FLAG_L_R_T).
    \param o ObjectT representing the type of this object
    \return bool indicating whether value was set. */
bool Object::setType( ObjectT o )
{
    objectType = o;
    return true;
}

/*! This method returns the type of this object.
    \return type of this object */
ObjectT  Object::getType() const
{
    return objectType;
}

/*! This method sets the relative position and the time this information was
    received. The relative position is calculated using the given distance and
    the given (relative) angle.
    \param dDist relative distance to object
    \param ang relative angle to object
    \param time time relative position was received
    \return bool indicating whether the values were set */
bool Object::setRelativePosition( double dDist, AngDeg ang, Time time )
{
    posRelative.setVecPosition( dDist, ang, POLAR );
    setTimeRelativePosition( time );
    return true;
}

/*! This method sets the relative position and the time this information was
    received using a vecPosition.
    \param p new relative position
    \param time time relative position was received
    \return bool indicating whether the values were set */
bool Object::setRelativePosition( VecPosition v, Time time )
{
    posRelative = v;
    setTimeRelativePosition( time );
    return true;
}

/*! This method returns the relative position of this object. The time of this
   information is related to the time returned by getTimeRelativePosition(),
   but is not checked. So if you want to know the relevance of this position
   use this method.
   \return relative position of this object */
VecPosition Object::getRelativePosition() const
{
    return posRelative;
}

/*! This method sets the time the relative position was received.
    \param time time relative position was received
    \return bool indicating whether the value was set */
bool Object::setTimeRelativePosition( Time time )
{
    timeRelativePosition = time;
    return true;
}

/*! This method returns the time that corresponds to the relative position of
    this object.
    \return time of the relative position of this object */
Time Object::getTimeRelativePosition() const
{
    return timeRelativePosition;
}

/*! This method sets the global position and the time this information was
    calculated.
    \param p new global position
    \param time time global position was received
    \return bool indicating whether the values were set */
bool Object::setGlobalPosition( VecPosition p, Time time )
{
    posGlobal = p;
    setTimeGlobalPosition( time );
    return true;
}

/*! This method returns the global position of this object. The time of this
    information is related to the time returned by getTimeGlobalPosition(),
    but is not checked. So if you want to know the relevance of this position
    use this method.
    \return global position of this object */
VecPosition Object::getGlobalPosition() const
{
    return posGlobal;
}

/*! This method sets the time the global position was calculated
    \param time time global position was calculated
    \return bool indicating whether the value was set */
bool Object::setTimeGlobalPosition( Time time )
{
    timeGlobalPosition = time;
    return true;
}

/*! This method returns the time that corresponds to the global position of
    this object.
    \return time of the global position of this object */
Time Object::getTimeGlobalPosition() const
{
    return timeGlobalPosition;
}

/*! This method sets the global position calculated using the last see
    message and the time of this see message. This opposed to the "normal"
    global position that is also updated when no see message has arrived
    in a new cycle.
    \param p new global position
    \param time time global position was received
    \return bool indicating whether the values were set */
bool Object::setGlobalPositionLastSee( VecPosition p, Time time )
{
    posGlobalLastSee = p;
    setTimeGlobalPosDerivedFromSee( time );
    return true;
}

/*! This method returns the global position of this object at the time of
    the last see message. The time of this information is related to the
    time returned by getTimeGlobalPosDerivedFromSee().
    \return global position of this object */
VecPosition Object::getGlobalPositionLastSee() const
{
    return posGlobalLastSee;
}

/*! This method sets the time the global position was calculated
    using a see message.
    \param time time global position was calculated with see message
    \return bool indicating whether the value was set */
bool Object::setTimeGlobalPosDerivedFromSee( Time time )
{
    timeGlobalPosDerivedFromSee = time;
    return true;
}

/*! This method returns the time that the global position was calculated
    using a see message.
    \return time of the global position of this object during the last see*/
Time Object::getTimeGlobalPosDerivedFromSee() const
{
    return timeGlobalPosDerivedFromSee;
}

/*! This method sets the time of the last see message in which this object was
    seen.
    \param time time this object was last seen
    \return bool indicating whether the value was set */
bool Object::setTimeLastSeen( Time time)
{
    timeLastSeen = time;
    return true;
}

/*! This method returns the time that corresponds to the time this object was
    located in the last see message.
    \return time of the last see message that was used to update this object */
Time Object::getTimeLastSeen() const
{
    return timeLastSeen;
}


/*****************************************************************************/
/********************** CLASS FIXEDOBJECT ************************************/
/*****************************************************************************/

/*! This method prints all the information about this FixedObject to the
    specified output stream
    \param os output stream to print all relevant information to. */
void FixedObject::show( ostream & os )
{
    char buf[MAX_TEAM_NAME_LENGTH];

    // DEFAULT_TEAM_NAME is used since it is not a player, name does not matter
    SoccerTypes::getObjectStr( buf, objectType, DEFAULT_TEAM_NAME );
    os <<  buf
    << " rel("  << posRelative << " r:" << posRelative.getMagnitude()
    << " phi:"  << posRelative.getDirection()
    << " t:"    << timeRelativePosition << ")"
    << " seen:" << timeLastSeen << "\n";
}

/*! This method returns the global position of this fixed object. Only works
    when the object type equals a flag or a goal. Furthermore the side of the
    agent has to be passed since the global positions differ for the left and
    the right side. For some flags the size of the goal is important. So this
    value can be passed also. Otherwise the default value is 14.02.
    \param s side of agent (global position differs for left and right side)
    \param dGoalWidth width of a goal, needed for pole objects next to goal
    \return global position of this fixed object. */
VecPosition FixedObject::getGlobalPosition( SideT s, double dGoalWidth )const
{
    return SoccerTypes::getGlobalPositionFlag( getType(), s, dGoalWidth );
}

/*! This methods returns the global angle of this fixed object in the world.
    Only works when the fixed object is a line. The angle for the left team
    rises clockwise, i.e. left=0, bottom=90, etc. For the right team this
    is counterclockwise: right=0, top=90, etc)
    \param s side of agent (angles differ for left and right side)
    \return global angle of this line in the world. */
AngDeg FixedObject::getGlobalAngle( SideT s )
{
    return SoccerTypes::getGlobalAngleLine( getType(), s );
}

/*****************************************************************************/
/********************* CLASS DYNAMICOBJECT ***********************************/
/*****************************************************************************/

/*! This is the constructor for DynamicObject. A DynamicObject is created with
    all the variables initialized by (illegal) default values */
DynamicObject::DynamicObject( ):Object( )
{
    dRelativeDistanceChange   = UnknownDoubleValue;
    dRelativeAngleChange      = UnknownDoubleValue;
}

/*! This method sets the global velocity of this object and the time of this
    information
    \param v new global velocity
    \param time time global velocity was received
    \return bool indicating whether the values were set */
bool DynamicObject::setGlobalVelocity( VecPosition v, Time time)
{
    if ( v.getMagnitude() < EPSILON )
        vecGlobalVelocity.setVecPosition( 0.0, 0.0 );
    else
        vecGlobalVelocity = v;
    setTimeGlobalVelocity( time );
    return true;
}

/*! This method returns the global velocity of this object. The time of this
    information is related to the time returned by getTimeGlobalVelocity().
    \return global position of this object */
VecPosition DynamicObject::getGlobalVelocity( ) const
{
    return vecGlobalVelocity;
}

/*! This method returns the speed of this object.
    The speed is the magnitude of the global velocity of the object
    \return speed of this object (zero for non-moving objects) */
double DynamicObject::getSpeed( ) const
{
    return vecGlobalVelocity.getMagnitude();
}

/*! This method sets the time that corresponds to the last update of the
    global velocity of this object.
    \param time time corresponding to current value of global velocity
    \return bool indicating whether the value was set */
bool DynamicObject::setTimeGlobalVelocity( Time time )
{
    timeGlobalVelocity = time;
    return true;
}

/*! This method returns the time that belongs to the global velocity of this
    object.
    \return time of the global velocity of this object */
Time DynamicObject::getTimeGlobalVelocity() const
{
    return timeGlobalVelocity;
}


/*! This method sets the relative distance change and the time this
    information was calculated.
    \param d new relative distance change
    \param time time relative distance change was calculated
    \return bool indicating whether the values were set */
bool DynamicObject::setRelativeDistanceChange( double d, Time time )
{
    dRelativeDistanceChange = d;
    setTimeChangeInformation( time );
    return true;
}

/*! This method returns the relative distance change of this object. Note that
    this value is zero when object is at the same distance, but at a complete
    different angle. This occurs when an object has moved a lot in one cycle.
    This information belongs to the server time that is returned by
    getTimeChangeInformation().
    \return relative distance change of object in the last cycle */
double DynamicObject::getRelativeDistanceChange() const
{
    return dRelativeDistanceChange;
}

/*! This method sets the relative angle change and the server time this
    information belongs to.
    \param d new relative angle change
    \param time time relative angle change was received
    \return bool indicating whether the values were set */
bool DynamicObject::setRelativeAngleChange( double d, Time time )
{
    dRelativeAngleChange = d;
    setTimeChangeInformation( time );
    return true;
}

/*! This method returns the relative angle change of this object.
    This information belongs to the server time that is returned by
    getTimeChangeInformation().
    \return relative angle change of object in the last cycle */
double DynamicObject::getRelativeAngleChange() const
{
    return dRelativeAngleChange;
}

/*! This method sets the time the change information was calculated.
    \param time time information for change was calculated
    \return bool indicating whether the values was set */
bool DynamicObject::setTimeChangeInformation( Time time )
{
    timeChangeInformation = time ;
    return true;
}

/*! This method returns the server time that belongs to the relative distance
    and relative angle change of this object.
    \return time of the change information of this DynamicObject */
Time DynamicObject::getTimeChangeInformation()  const
{
    return timeChangeInformation;
}

/*! This method sets the global velocity of the object calculated after the last
    see message. The time of this information corresponds to
    'getTimeChangeInformation'.
    \return global body velocity derived from the last see message */
bool DynamicObject::setGlobalVelocityLastSee ( VecPosition vec )
{
    vecGlobalVelocityLastSee = vec;
    return true;
}

/*! This method returns the global velocity of the object calculated after the
    last see message. The time of this information corresponds to
    'getTimeChangeInformation'.
    \return global body velocity derived from the last see message */
VecPosition DynamicObject::getGlobalVelocityLastSee ( )   const
{
    return vecGlobalVelocityLastSee;
}


/*****************************************************************************/
/********************* CLASS PLAYEROBJECT ************************************/
/*****************************************************************************/

/*! This is the constructor for PlayerObject. A PlayerObject is created with
    all variables initialized to (illegal) default values */
PlayerObject::PlayerObject( ):DynamicObject( )
{
    angGlobalBodyAngle   = UnknownAngleValue;
    angGlobalNeckAngle   = UnknownAngleValue;
    isKnownPlayer        = false;
    isGoalie             = false;

    angRelativeBodyAngle = UnknownAngleValue;
    angRelativeNeckAngle = UnknownAngleValue;

    iHeteroPlayerType    = 0;
}

/*! This method sets the facing direction of the body and the time of this
    information (all relative to the agent).
    \param ang new relative facing direction of body
    \param time time corresponding to the facing direction
    \return bool indicating whether the values were set */
bool PlayerObject::setRelativeBodyAngle( AngDeg ang, Time time )
{
    angRelativeBodyAngle = ang;
    setTimeRelativeAngles( time );
    return true;
}

/*! This method returns the relative body angle of this object. This
    information is from the server time that is returned by
    getTimeRelativeAngles().

    \return relative body angle of this object */
AngDeg PlayerObject::getRelativeBodyAngle( )  const
{
    return angRelativeBodyAngle;
}

/*! This method sets the facing direction of the body and the time of this
    information (all global).
    \param ang new global facing direction of body
    \param time time corresponding to the facing direction
    \return bool indicating whether the values were set */
bool PlayerObject::setGlobalBodyAngle( AngDeg ang, Time time)
{
    angGlobalBodyAngle = ang;
    setTimeGlobalAngles( time );
    return true;
}

/*! This method returns the global body angle of this object. This information
    is from the server time that is returned by getTimeGlobalAngles().
    \return global body angle of this object */
AngDeg PlayerObject::getGlobalBodyAngle( )  const
{
    return angGlobalBodyAngle;
}

/*! This method returns the facing direction of the neck and the time of
    this information (all relative to the agent).
    \param ang new relative facing direction of neck
    \param time time facing direction was received
    \return bool indicating whether the values were set */
bool PlayerObject::setRelativeNeckAngle( AngDeg ang, Time time )
{
    angRelativeNeckAngle = ang;
    setTimeRelativeAngles( time );
    return true;
}

/*! This method returns the relative neck angle of this object. This
    information is from  the time  that is returned by getTimeRelativeAngles().
    \return relative neck angle of this object */
AngDeg PlayerObject::getRelativeNeckAngle( )  const
{
    return angRelativeNeckAngle;
}

/*! This method returns the facing direction of the neck and the time of
    this information (all global).
    \param ang new global facing direction of neck
    \param iTime time facing direction was received
    \return bool indicating whether the values were set */
bool PlayerObject::setGlobalNeckAngle( AngDeg ang, Time time )
{
    angGlobalNeckAngle = ang;
    setTimeGlobalAngles( time );
    return true;
}

/*! This method returns the global neck angle of this object. This
    information is from  the time that is returned by getTimeGlobalAngles().
    \return global neck angle of this object */
AngDeg PlayerObject::getGlobalNeckAngle( )  const
{
    return angGlobalNeckAngle;
}

/*! This method sets the time the facing direction was calculated.
    \param time time the facing direction was received
    \return bool indicating whether the values were set */
bool PlayerObject::setTimeRelativeAngles( Time time )
{
    timeRelativeAngles = time;
    return true;
}

/*! This method returns the server time in which the relative body and neck
    angle of this object were calculated.
    \return time of the relative neck and body information */
Time PlayerObject::getTimeRelativeAngles( ) const
{
    return timeRelativeAngles ;
}

/*! This method sets the time the facing direction was calculated.
    \param time time the facing direction was received
    \return bool indicating whether the values were set */
bool PlayerObject::setTimeGlobalAngles( Time time )
{
    timeGlobalAngles = time;
    return true;
}


/*! This method sets the global angle of the agent calculated after the last
    see message. The time of this information corresponds to
    'getTimeChangeInformation', since the change information arrives always
    together with the body and neck information.
    \return global body angle derived from the last see message */
bool PlayerObject::setGlobalBodyAngleLastSee( AngDeg ang )
{
    angGlobalBodyAngleLastSee = ang;
    return true;
}


/*! This method returns the global angle of the agent calculated after
    the last see message. The time of this information corresponds to
    'getTimeChangeInformation', since the change information arrives
    always together with the body and neck information.

    \return global body angle derived from the last see message */
AngDeg PlayerObject::getGlobalBodyAngleLastSee( )  const
{
    return angGlobalBodyAngleLastSee;
}


/*! This method returns the server time in which the global body and neck
    angle of this object were calculated.
    \return time of the global neck and body information */
Time PlayerObject::getTimeGlobalAngles( ) const
{
    return timeGlobalAngles ;
}

/*! This method sets the possible range from which this object must originate
    from. Since the ordening of objects in a 'see' message is always the same
    (first teammates then opponents and always sorted by player number), it
    is possible to derive the range of objects from which the supplied
    information must originate.
    \param objMin minimum object type
    \param objMax maximum object type
    \return bool indicating whether update was successful. */
bool PlayerObject::setPossibleRange( ObjectT objMin, ObjectT objMax )
{
    objRangeMin = objMin;
    objRangeMax = objMax;
    return true;
}

/*! This method returns a boolean indicating whether the supplied object type,
    is in the range of possible object types set by 'setPossibleRange'.
    \param obj Object type that should be checked
    \return boolean indicating whether 'obj' is located in the range. */
bool PlayerObject::isInRange( ObjectT obj, bool bTeammatesFirst )
{
    int  iIndObj = SoccerTypes::getIndex( obj );
    int  iIndMin = SoccerTypes::getIndex( objRangeMin );
    int  iIndMax = SoccerTypes::getIndex( objRangeMax );

    // teammates 0 to 10, opponents 11 to 21 or -10..1 I guess
    if ( SoccerTypes::isOpponent( obj ) )
        iIndObj += ( bTeammatesFirst ) ? 11 : -11 ;
    if ( SoccerTypes::isOpponent( objRangeMin ) )
        iIndMin += ( bTeammatesFirst ) ? 11 : -11 ;
    if ( SoccerTypes::isOpponent( objRangeMax ) )
        iIndMax += ( bTeammatesFirst ) ? 11 : -11 ;

    return iIndMin <= iIndObj && iIndObj <= iIndMax ;
}

/*! This method returns the minimal possible object type as set by
   'setPossibleRange'.
    \return minimal possible object type */
ObjectT PlayerObject::getMinRange( )
{
    return objRangeMin;
}

/*! This method returns the maximal possible object type as set by
   'setPossibleRange'.
    \return maximal possible object type */
ObjectT PlayerObject::getMaxRange( )
{
    return objRangeMax;
}

/*! This method sets whether this dynamic object is a known player or not.
    A known player is a player of which we know the number. If we don't know
    the player number of a player, the player is put at the index of a player
    that isn't seen in a while and the isKnownPlayer attribute is set to false.
    \param b bool indicating whether player number is known
    \return bool indicating whether value was set. */
bool PlayerObject::setIsKnownPlayer( bool b )
{
    isKnownPlayer = b;
    return true;
}

/*! This method returns whether the current object is a known player or not.
    A known player is a player of which we know the number. If we don't know
    the player number of a player, the player is put at the index of a player
    that isn't seen in a while and the isKnownPlayer attribute is set to false.
    \return bool indicating whether player number is known */
bool PlayerObject::getIsKnownPlayer() const
{
    return isKnownPlayer;
}

/*! This method sets whether this dynamic object is a goalie or not.
    \param b bool indicating whether this dynamic object is a goalie
    \return bool indicating whether value was set. */
bool PlayerObject::setIsGoalie( bool b )
{
    isGoalie = b;
    return true;
}

/*! This method returns whether the current object is a goalie or not.
    \return bool indicating whether this dynamic object is a goalie or not */
bool PlayerObject::getIsGoalie() const
{
    return isGoalie;
}


/*! This method prints the information about this PlayerObject to the specified
    output stream. The variables are printed with the default team name.
    \param os output stream to which output is written */
void PlayerObject::show( ostream & os )
{
    show( DEFAULT_TEAM_NAME, os );
}

/*! This method prints the information about this PlayerObject to the specified
    output stream. The variables are printed with the specified team name.
    \param strTeamName team name of this player object
    \param os output stream to which output is written */
void PlayerObject::show( const char* strTeamName , ostream & os )
{
    char buf[MAX_TEAM_NAME_LENGTH];
    SoccerTypes::getObjectStr( buf, objectType, strTeamName );
    os << buf
    << " conf: "              << getConfidence( timeGlobalPosition )
    << " pos: "               << posGlobal
    << ","                    << timeGlobalPosition
    << " vel: "               << vecGlobalVelocity
    << ","                    << timeGlobalVelocity
    << " ang (b:"             << getGlobalBodyAngle()
    << ",n:"                  << angGlobalNeckAngle
    << "),"                   << timeGlobalAngles
    << "known: "              << isKnownPlayer
    << " lastseen:"            << timeLastSeen << "\n";
}

/*! This method sets the heterogeneous index number of this player object.
    \param hetereogeneous index number
    \return bool indicating whether update was successful. */
bool PlayerObject::setHeteroPlayerType( int index )
{
    iHeteroPlayerType = index;
    return true;
}

/*! This method returns the heterogeneous index number of this player object.
    \return hetereogeneous index number */
int  PlayerObject::getHeteroPlayerType( ) const
{
    return iHeteroPlayerType;
}


/*! This method sets the time  related to the last tackle of this object. */
bool PlayerObject::setTimeTackle( Time time )
{
    m_timeTackle = time;
    return true;
}

/*! This method returns the time that is related to the last tackle
    of this object.  */
Time PlayerObject::getTimeTackle( ) const
{
    return m_timeTackle;
}


/*! This method sets the global direction in which the arm of this object
    has last been observed to point. The time related to this update is
    represented by time.  */
bool PlayerObject::setGlobalArm( AngDeg ang, Time time )
{
    m_angGlobalArm = ang;
    return setTimeGlobalArm( time );
}

/*! This method returns the global direction in which the arm of this object
    has last been observed to point.  */
AngDeg PlayerObject::getGlobalArm( ) const
{
    return m_angGlobalArm;
}

/*! This method sets the time that is related to the last time the arm
    of this object has been observed to point in the global direction returned
    by 'getGlobalArm'. */
bool PlayerObject::setTimeGlobalArm( Time time )
{
    m_timeGlobalArm = time;
    return true;
}

/*! This method returns the time that is related to the last time the arm of
    this object has been observed to point in the global direction returned
    by 'getGlobalArm'. */
Time PlayerObject::getTimeGlobalArm( ) const
{
    return m_timeGlobalArm;
}

/*****************************************************************************/
/********************* CLASS BALLOBJECT **************************************/
/*****************************************************************************/

/*! This is the constructor for a BallObject. It does nothing except
    initializing the class. */
BallObject::BallObject():DynamicObject()
{

}

/*! This method prints information about the ball to the specified
    output stream

    \param os output stream to which information is written. */
void BallObject::show( ostream& os)
{
    char buf[MAX_TEAM_NAME_LENGTH];
    SoccerTypes::getObjectStr( buf, objectType, DEFAULT_TEAM_NAME );
    os <<  buf
    << "r: "                 << posRelative.getMagnitude()
    << " phi: "              << posRelative.getDirection()
    << " vel: "              << vecGlobalVelocity
    << " "                   << timeGlobalVelocity
    << " rel: "              << posRelative
    << ", "                  << timeRelativePosition
    << " global: "           << posGlobal
    << ", "                  << timeGlobalPosition
    << ", changes: "         << dRelativeDistanceChange
    << ", "                  << dRelativeAngleChange
    << " last seen:"         << timeLastSeen
    << " globalposderived: " << timeGlobalPosDerivedFromSee
    << "\n";
}

/*****************************************************************************/
/********************* CLASS AGENTOBJECT *************************************/
/*****************************************************************************/

/*! This is the constructor for the class AgentObject and initializes the
    variables with the AgentObject. This the class that contains information
    about the agent itself.
    \param dStaminaMax maximum stamina for this agent (default 4000.0) */
AgentObject::AgentObject( double dStaminaMax ):PlayerObject( )
{
    viewAngle              = VA_NORMAL;
    viewQuality            = VQ_HIGH;

    stamina.setStamina ( dStaminaMax );
    stamina.setEffort  ( 1.0 );
    stamina.setRecovery( 1.0 );
    velSpeedRelToNeck.setVecPosition( 0.0, 0.0 );

    angGlobalNeckAngle   = UnknownAngleValue;
    angBodyAngleRelToNeck  = UnknownAngleValue;
}

/*! This methods prints the information about this AgentObject to the specified
    output stream. The default team name is used as the name.
    \param os output stream to which information is written. */
void AgentObject::show( ostream& os )
{
    show( DEFAULT_TEAM_NAME, os );
}

/*! This methods prints the information about this AgentObject to the specified
    output stream. The specified team name is used as the name
    \param strTeamName team name for this agent.
    \param os output stream to which information is written. */
void AgentObject::show( const char * strTeamName, ostream & os )
{
    char buf[MAX_TEAM_NAME_LENGTH];
    SoccerTypes::getObjectStr( buf, objectType, strTeamName );
    os   <<  buf
    << " (global("    << posGlobal
    << ", "           << timeGlobalPosition
    << ") (vel("      << vecGlobalVelocity
    << ", "           << timeGlobalVelocity
    << ") (angles(n:" << angGlobalNeckAngle
    << ", b:"         << angBodyAngleRelToNeck << ") ";
    stamina.show( os );
}

/*! This method returns the view angle of this PlayerObject.
    The view angle equals VA_NARROW, VA_NORMAL, VA_WIDE or VA_ILLEGAL.
    \return view angle of this PlayerObject */
ViewAngleT AgentObject::getViewAngle() const
{
    return viewAngle;
}

/*! This method returns the difference between the predicted global position
    of the agent and the actual derived global position. This difference can
    be used in determining the actual movement of other objects since the
    noise caused by the difference in the global position of the agent
    is then filtered out. */
VecPosition AgentObject::getPositionDifference() const
{
    return posPositionDifference;
}

/*! This method sets the position difference between the derived global
    position from the previous cycle information and the global position
    from the latest see message.
    \param p new position difference
    \return bool indicating whether the update was succesfull. */
bool  AgentObject::setPositionDifference( VecPosition p )
{
    posPositionDifference = p;
    return true;
}

/*! This method sets the view angle of this AgentObject.
    \param v new view angle (VA_NARROW, VA_NORMAL, VA_WIDE or VA_ILLEGAL)
    \return bool indicating whether value was set */
bool AgentObject::setViewAngle( ViewAngleT v )
{
    viewAngle = v;
    return true;
}

/*! This method returns the view quality of this AgentObject.
    The view angle equals VQ_LOW, VQ_HIGH, or VQ_ILLEGAL.
    \return view quality of this AgentObject */
ViewQualityT AgentObject::getViewQuality() const
{
    return viewQuality;
}

/*! Set the view quality of this AgentObject.
    \param v new view quality (VQ_LOW, VQ_HIGH, VQ_ILLEGAL)
    \return bool indicating whether value was set */
bool AgentObject::setViewQuality( ViewQualityT v )
{
    viewQuality = v;
    return true;
}

/*! This method returns the Stamina of the AgentObject.
    \return stamina from the agent. */
Stamina AgentObject::getStamina( ) const
{
    return stamina;
}

/*! This method sets the stamina of this AgentObject.
    \param sta new stamina for this AgentObject
    \return bool indicating whether value was set */
bool AgentObject::setStamina( Stamina sta )
{
    stamina = sta;
    return true ;
}

/*! This method returns the velocity (speed and direction) of this AgentObject.
    This information is directly availablefrom the sense message, in which the
    speed factor and the angle of this speed (relative to the neck) are given.
    \return velocity agent relative to the neck. */
VecPosition AgentObject::getSpeedRelToNeck( ) const
{
    return velSpeedRelToNeck;
}

/*! This method sets the velocity (speed and direction) of this AgentObject.
    This information comes directly from the sense message.
    \param v new velocity for this agentobject
    \return bool indicating whether value was set */
bool AgentObject::setSpeedRelToNeck( VecPosition v )
{
    velSpeedRelToNeck = v;
    return true;
}

/*! This method sets the global neck angle for this AgentObject.
    \param ang value for the global neck angle.
    \return bool indicating whether value was set */
bool AgentObject::setGlobalNeckAngle( AngDeg ang )
{
    angGlobalNeckAngle= ang;
    angGlobalBodyAngle= VecPosition::normalizeAngle(getBodyAngleRelToNeck()+ang);
    return true ;
}

/*! This method returns the relative angle of the body to the neck of this
    AgentObject.
    Example: global angle neck is 90 degrees and global body angle
    is 0, means that relative angle of body to neck is -90 degrees.
    \return relative body angle to the neck */
AngDeg AgentObject::getBodyAngleRelToNeck( ) const
{
    return angBodyAngleRelToNeck;
}

/*! This method sets the relative body angle to the neck for this AgentObject.
    \param ang relative body angle to the neck
    \return bool indicating whether value was set */
bool AgentObject::setBodyAngleRelToNeck( AngDeg ang )
{
    angBodyAngleRelToNeck = ang;
//  angGlobalBodyAngle = VecPosition::normalizeAngle(getGlobalNeckAngle()+ang);
    return true;
}

/*! This method sets whether it is allowed to move the arm of the agent. */
bool AgentObject::getArmMovable( ) const
{
    return m_bArmMovable;
}

/*! This method returns whether it is allowed to move the arm of the agent. */
bool AgentObject::setArmMovable( bool b )
{
    m_bArmMovable = b;
    return true;
}

/*! This method returns the numer of cycles it will take the arm to expire. In
    case of zero, the arm doesn't point anymore and the other players won't see
    the arm pointing anymore. */
int AgentObject::getArmExpires( ) const
{
    return m_iArmExpires;
}

/*! This method sets the numer of cycles it will take the arm to expire. In
    case of zero, the arm doesn't point anymore and the other players won't see
    the arm pointing anymore. */
bool AgentObject::setArmExpires( int i )
{
    m_iArmExpires = i;
    return true;
}

/*! This method returns the global position to which the arm points. If
    'getArmExpires' returns zero, it means that the arm doesn't point anymore
    and this value is invalid. */
VecPosition AgentObject::getGlobalArmPosition( ) const
{
    return m_posGlobalArm;
}

/*! This method sets the global position to which the arm points. */
bool AgentObject::setGlobalArmPosition ( VecPosition  v )
{
    m_posGlobalArm = v;
    return true;
}

/*! This method returns the number of cycles it will take the tackle to expire.
    In case of 0 a tackle command can be issued. */
int AgentObject::getTackleExpires( ) const
{
    return m_iTackleExpires;
}

/*! This method sets the number of cycles it will take the tackle to expire.
    In case of 0 a tackle command can be issued. */
bool AgentObject::setTackleExpires( int i )
{
    m_iTackleExpires = i;
    return true;
}


/*****************************************************************************/
/********************* CLASS STAMINA *****************************************/
/*****************************************************************************/

/*! This is the constructor for this class. It sets the stamina, effort and
    recovery on the supplied values.
    \param dSta new stamina value (default 4000.0)
    \param dEff new effort value (default 1.0)
    \param dRec new recovery value (default 1.0)*/
Stamina::Stamina( double dSta, double dEff, double dRec, double dCap )/*! misol.gao fixed for v13.2 */
{
    setStamina ( dSta );
    setEffort  ( dEff );
    setRecovery( dRec );
    setCapacity( dCap );/*! misol.gao fixed for v13.2 */
}

double Stamina::getCapacity() const
{
    return m_dCapacity;
}

bool Stamina::setCapacity(double d)
{
    if ( d < 0.0 )
    {
        m_dCapacity = 0.0;
        return false;
    }
    else
    {
        m_dCapacity = d;
        return true;
    }
}



/*! This method prints the stamina values (stamina, effort and recovery) to the
    specified output stream.
    \param os output stream */
void Stamina::show( ostream & os)
{
    os  << "(sta:"   << m_dStamina
    << ", eff:"  << m_dEffort
    << ", rec: " << m_dRecovery << ")" << "\n";
}

/*! This method returns a TiredNessT enumeration that indicates how tired
    this associated agent with this Stamina value is. */
TiredNessT  Stamina::getTiredNess( double dRecDecThr, double dStaminaMax )
{
    double dStaDiffWithThr = getStamina() - dRecDecThr * dStaminaMax;
//  cerr << getStamina() << " " << dStaminaMax <<
//    " " << dRecDecThr << " " << dStaDiffWithThr << endl;
    if ( dStaDiffWithThr < 100 )
        return TIREDNESS_TERRIBLE;
    else if ( dStaDiffWithThr < 750 )
        return TIREDNESS_VERY_BAD;
    else if ( dStaDiffWithThr < 1500 )
        return TIREDNESS_BAD;
    else if ( dStaDiffWithThr < 2250 )
        return TIREDNESS_AVERAGE;

    return TIREDNESS_GOOD;
}

/*! This method returns the current stamina value.
    \return current stamina value (>0) */
double Stamina::getStamina() const
{
    return m_dStamina;
}

/*! This method sets the stamina value. This value should be positive,
    otherwise the value is set to 0 and false is returned.
    \param d new stamina value
    \return bool indicating whether value was set. */
bool Stamina::setStamina( double d )
{
    if ( d < 0.0 )
    {
        m_dStamina = 0.0;
        return false;
    }
    else
        m_dStamina = d;
    return true;
}

/*! This method returns the effort. The effort denotes the percentage of
    the power in a dash that is actually used. Normally this is 1.0
    (100%), but when it comes below a threshold, it decreases. It will again
    rise when stamina becomes higher than a certain threshold defined in
    ServerSettings.
    \return effort value between 0 and 1 */
double Stamina::getEffort() const
{
    return m_dEffort;
}

/*! This method sets the effort value. This value should be between 0 and 1,
    otherwise the value is set to the closest value in this interval
    (0 for negative values, 1 for higher values) and false is returned.
    \param d new effort value (0..1)
    \return bool indicating whether value was set. */
bool Stamina::setEffort( double d )
{
    if ( d < 0.0 )
    {
        m_dEffort = 0.0;
        return false;
    }
    else if ( d > 1.0 )
    {
        m_dEffort = 1.0;
        return false;
    }
    else
        m_dEffort = d;
    return true;
}

/*! This method returns the recovery. Recovery denotes the percentage of
    the stamina increase that is added to the stamina every cycle. If recovery
    is 1.0 all of the increase of stamina is added to the current stamina.
    When stamina becomes below a certain threshold defined in ServerSettings,
    the recovery is decreased. It can never increase!
    \return recovery value between 0 and 1 */
double Stamina::getRecovery() const
{
    return m_dRecovery;
}

/*! This method sets the recovery value. This value should be between 0 and 1,
    otherwise the value is set to the closest value in this interval (0 for
    negative values, 1 for higher values) and false is returned.
    \param d new recovery value (0..1)
    \return bool indicating whether value was set. */
bool Stamina::setRecovery( double d )
{
    if ( d < 0.0 )
    {
        m_dRecovery = 0.0;
        return false;
    }
    else if ( d > 1.0 )
    {
        m_dRecovery = 1.0;
        return false;
    }
    else
        m_dRecovery = d;
    return true;
}

/*****************************************************************************/
/********************* TESTING PURPOSES **************************************/
/*****************************************************************************/

/*
int main( void )
{
  PlayerObject p();
  BallObject b();
  FixedObject s();

  cout << p.getDeltaRelativeAngle() << "\n" <<
      b.getDeltaRelativeDistance() << "\n" <<
  p.getTimeRelativeDistance() << "\n";
  return 0;

}
*/
