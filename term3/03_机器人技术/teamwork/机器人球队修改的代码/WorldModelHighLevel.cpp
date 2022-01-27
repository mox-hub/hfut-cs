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

/*! \file WorldModelHighLevel.cpp
<pre>
<b>File:</b>          WorldModelHighLevel.cpp
<b>Project:</b>       Robocup Soccer Simulation Team: UvA Trilearn
<b>Authors:</b>       Jelle Kok
<b>Created:</b>       12/02/2001
<b>Last Revision:</b> $ID$
<b>Contents:</b>      class definitions of WorldModel. This class contains
               methods that reason about the world model on a higher
               level and return more abstract information about the current
               world state.
<hr size=2>
<h2><b>Changes</b></h2>
<b>Date</b>             <b>Author</b>          <b>Comment</b>
12/02/2001       Jelle Kok       Initial version created
</pre>
*/

#include<list>            // needed for list<double>
#include<stdio.h>         // needed for printf
#include "WorldModel.h"

//2020.5.7
/*这个函数判断在扇区内有没有敌方球员，若有则ture 否则false
 * 
 */
bool    WorldModel::isOpponentAtAngleEx( AngDeg angA, AngDeg angB, double dDist )
{
    VecPosition posAgent = getAgentGlobalPosition();
    VecPosition posOpp;
    AngDeg angOpp;
    int iIndex;
    
    for( ObjectT o = iterateObjectStart(iIndex, OBJECT_SET_OPPONENTS );
	 o != OBJECT_ILLEGAL;
	 o = iterateObjectNext( iIndex, OBJECT_SET_OPPONENTS ))
    {
	posOpp = getGlobalPosition(o);
	angOpp = ( posOpp - posAgent ).getDirection();
	if ( angA <= angOpp && angOpp <= angB && posAgent.getDistanceTo( posOpp ) < dDist)
	    return true;
    }
    iterateObjectDone( iIndex ) ;
    return false;
}

/*! This method returns the number of objects that are within the circle 'c'
    Only objects are taken into account that are part of the set 'set' and
    have a confidence higher than the threshold defined in PlayerSettings.
    \param c circle in which objects should be located to be counted
    \return number of objects from 'set' in circle 'c'*/
int WorldModel::getNrInSetInCircle( ObjectSetT set, Circle c )
{
    double dConfThr = PS->getPlayerConfThr();
    int    iNr      = 0;
    int    iIndex;

    for ( ObjectT o = iterateObjectStart( iIndex, set, dConfThr );
            o != OBJECT_ILLEGAL;
            o = iterateObjectNext ( iIndex, set, dConfThr ) )
    {
        if ( c.isInside( getGlobalPosition( o ) ) )
            iNr++;
    }
    iterateObjectDone( iIndex );

    return iNr;
}

/*! This method returns the number of visible objects that are part of
    the object set 'set' and located in the rectangle 'rect'. When no
    rectangle is defined (rect=NULL) the whole field is taken into
    account. Only objects with a confidence value higher than the
    threshold defined in PlayerSettings are taken into consideration.

    \param set ObjectSetT from which objects are taken into consideration
    \param rect Rectangle in which objects are counted (default NULL)
    \return number of objects in Rectangle 'rect'.*/
int WorldModel::getNrInSetInRectangle( ObjectSetT set, Rect *rect  )
{
    double dConfThr = PS->getPlayerConfThr();
    int    iNr      = 0;
    int    iIndex;

    for ( ObjectT o = iterateObjectStart( iIndex, set, dConfThr );
            o != OBJECT_ILLEGAL;
            o = iterateObjectNext ( iIndex, set, dConfThr ) )
    {
        if ( rect == NULL || rect->isInside( getGlobalPosition( o ) ) )
            iNr++;
    }
    iterateObjectDone( iIndex );
    return iNr;
}

/*! This method returns the number of objects in the specified cone.
    A cone is like a piece of a pie, in which 'start' is
    the center of the pie, 'end' is the edge of the pie and 'dWidth' is the
    half width of the piece after distance 1. Only objects are taken into
    consideration that are within the set 'set' and have a confidence higher
    than the threshold defined in PlayerSettings.
    \param set ObjectSetT of which objects are taken into consideration
    \param dWidth half width of the cone after distance 1.0
    \param start center of the cone
    \param end position that is the end of the cone.
    \return number of objects part of 'set' and located in this cone. */
int WorldModel::getNrInSetInCone( ObjectSetT set, double dWidth,
                                  VecPosition start , VecPosition end )
{
    double      dConfThr   = PS->getPlayerConfThr();
    int         iNr        = 0;
    int         iIndex;
    Line        line       = Line::makeLineFromTwoPoints( start, end );
    VecPosition posOnLine;
    VecPosition posObj;

    for ( ObjectT o = iterateObjectStart( iIndex, set, dConfThr );
            o != OBJECT_ILLEGAL;
            o = iterateObjectNext ( iIndex, set, dConfThr ) )
    {
        posObj    = getGlobalPosition( o );
        posOnLine = line.getPointOnLineClosestTo( posObj );
        // whether posOnLine lies in cone is checked by three constraints
        // - does it lie in triangle (to infinity)
        // - lies between start and end (and thus not behind me)
        // - does it lie in circle
        if (posOnLine.getDistanceTo(posObj) < dWidth*posOnLine.getDistanceTo(start)
                && line.isInBetween( posOnLine, start, end )
                && start.getDistanceTo( posObj ) < start.getDistanceTo( end ) )
            iNr++;
    }
    iterateObjectDone( iIndex );
    return iNr;
}


/*! This method returns whether the space in direction 'ang'' of
    object 'obj' is occupied by any opponents. */
bool WorldModel::isEmptySpace( ObjectT obj, AngDeg ang, double dDist )
{
    if ( obj == OBJECT_ILLEGAL )
        return false;

    VecPosition pos = getGlobalPosition( obj );
    pos += VecPosition( dDist, ang, POLAR );

    if ( getNrInSetInCircle( OBJECT_SET_OPPONENTS, Circle( pos, dDist ) ) == 0 )
        return true;

    return false;
}


bool WorldModel::coordinateWith( ObjectT obj )
{
    VecPosition pos = getGlobalPosition( obj );
    if ( pos.getDistanceTo( getBallPos() ) < 30.0 &&
            pos.getX() > getBallPos().getX() - 5.0 )
    {

        if ( getFastestInSetTo( OBJECT_SET_TEAMMATES, OBJECT_BALL ) ==
                getAgentObjectType() )
        {
            logCircle( 700, pos, 2.5 );
        }
        Log.log( 700, "coordinate with %d %f %f (%f %f)",
                 obj, pos.getDistanceTo( getBallPos() ),
                 pos.getX(), getBallPos().getX(), getBallPos().getY() );
        return true;
    }

    return false;
}

/*! This method returns the object type of the closest object to the
    ObjectT that is supplied as the second argument. Only objects are
    taken into account that are part of the set 'set' and have a
    confidence higher than the supplied threshold. If no threshold is
    supplied, the threshold defined in PlayerSettings is used.

    \param set ObjectSetT which denotes objects taken into consideration

    \param objTarget ObjectT that represent the type of the object to
    compare to

    \param dDist will be filled with the closest distance

    \param dConfThr minimum confidence threshold for the objects in 'set'
    \return ObjectType that is closest to o */
ObjectT WorldModel::getClosestInSetTo( ObjectSetT set, ObjectT objTarget,
                                       double *dDist, double dConfThr )
{
    if ( dConfThr == -1.0 ) dConfThr      = PS->getPlayerConfThr();
    ObjectT     closestObject = OBJECT_ILLEGAL;
    double      dMinMag       = 1000.0;
    VecPosition v;
    int         iIndex;

    for ( ObjectT o = iterateObjectStart( iIndex, set, dConfThr );
            o != OBJECT_ILLEGAL;
            o = iterateObjectNext ( iIndex, set, dConfThr ) )
    {
        if ( o != objTarget )              // do not include target object
        {
            v = getGlobalPosition( objTarget ) - getGlobalPosition( o );
            if ( v.getMagnitude() < dMinMag )
            {
                dMinMag       = v.getMagnitude();
                closestObject = o;
            }
        }
    }

    iterateObjectDone( iIndex );
    if ( dDist != NULL )
        *dDist = dMinMag;
    return closestObject;
}

/*! This method returns the ojbect type of the closest object to the specified
    position and that is part of the object set 'set' with a confidence
    higher than the
    supplied threshold. If no threshold is supplied, the threshold defined in
    PlayerSettings is used.
    \param set ObjectSetT which denotes objects taken into consideration

    \param pos position to which player should be compared

    \param dDist will be filled with the distance between pos and
    closest object

    \param dConfThr minimum confidence threshold for the objects in 'set'
    \return ObjectT representing object that is closest to pos */
ObjectT WorldModel::getClosestInSetTo( ObjectSetT set, VecPosition pos,
                                       double *dDist,  double dConfThr )
{
    ObjectT     closestObject   = OBJECT_ILLEGAL;
    double      dMinMag         = 1000.0;
    VecPosition v;
    int         iIndex;

    if ( dConfThr == -1.0 ) dConfThr      = PS->getPlayerConfThr();
    for ( ObjectT o = iterateObjectStart( iIndex, set, dConfThr );
            o != OBJECT_ILLEGAL;
            o = iterateObjectNext ( iIndex, set, dConfThr ) )
    {
        v = pos - getGlobalPosition( o );
        if ( v.getMagnitude() < dMinMag )
        {
            dMinMag         = v.getMagnitude();
            closestObject   = o;
        }
    }
    iterateObjectDone( iIndex );
    if ( dDist != NULL )
        *dDist = dMinMag;
    return closestObject;
}

/*! This method returns the closest object in 'set' to the line
    'l'. The projection p of the global position of this object on the
    line 'l' should lie between pos1 and pos2. After the method is
    finished, it returns this object and the last two arguments of
    this method are set to the the distance between the object and p
    and the distance from pos1 to p respectively.

    \param set ObjectSetT which denotes objects taken into consideration
    \param l line to which opponents should be projected
    \param pos1 minimum allowed projection point
    \param pos2 maximum allowed projection point
    \param dDistObjToLine will contain distance from opponent to line l
    \param dDistPos1ToPoint will contain distance from pos1 to projection point
    opponent on line l
    \return object type of closest object to line l */
ObjectT WorldModel::getClosestInSetTo( ObjectSetT set, Line l,
                                       VecPosition pos1, VecPosition pos2,
                                       double *dDistObjToLine, double *dDistPos1ToPoint)
{
    VecPosition posObj;
    VecPosition posOnLine;
    double      dConfThr  = PS->getPlayerConfThr();
    ObjectT     obj       = OBJECT_ILLEGAL;
    double      dDist     ;
    double      dMinDist  = 1000.0;
    double      dDistPos1 = 1000.0;
    int         iIndex;

    for ( ObjectT o = iterateObjectStart( iIndex, set, dConfThr );
            o != OBJECT_ILLEGAL;
            o = iterateObjectNext ( iIndex, set, dConfThr ) )
    {
        posObj    = getGlobalPosition( o );
        posOnLine = l.getPointOnLineClosestTo( posObj );
        dDist     = posObj.getDistanceTo( posOnLine );
        if ( l.isInBetween( posOnLine, pos1, pos2 ) && dDist < dMinDist )
        {
            dMinDist  = dDist;
            obj       = o;
            dDistPos1 = pos1.getDistanceTo( posOnLine );
        }
    }
    iterateObjectDone( iIndex );
    if ( dDistObjToLine != NULL )
        *dDistObjToLine = dMinDist;
    if ( dDistPos1ToPoint != NULL )
        *dDistPos1ToPoint = dDistPos1;

    return obj;
}

/*! This method returns the object type of the closest object relative to
    the agent. Only objects are taken into account that are part of the set
    'set'.
    \param set ObjectSetT which denotes objects taken into consideration
    \param dDist will be filled with the closest relative distance
    \return ObjectType that is closest to the agent*/
ObjectT WorldModel::getClosestRelativeInSet( ObjectSetT set, double *dDist )
{
    ObjectT     closestObject = OBJECT_ILLEGAL;
    double      dMinMag       = 1000.0;
    int         iIndex;

    for ( ObjectT o = iterateObjectStart( iIndex, set, 1.0 );
            o != OBJECT_ILLEGAL;
            o = iterateObjectNext ( iIndex, set, 1.0 ) )
    {
        if ( getRelativeDistance( o ) < dMinMag )
        {
            dMinMag       = getRelativeDistance( o );
            closestObject = o;
        }
    }

    iterateObjectDone( iIndex );
    if ( dDist != NULL )
        *dDist = dMinMag;
    return closestObject;
}

/*! This method returns the object type of the second closest object to the
    object type that is supplied as the second argument. Only objects are taken
    into account within set 'set' and with a confidence higher than the
    supplied threshold. If no threshold is supplied, the threshold defined in
    PlayerSettings is used.
    \param set ObjectSetT which denotes objects taken into consideration
    \param obj ObjectT that represent the type of the object to check
    \param dDist will be filled with the distance to this player.
    \param dConfThr minimum confidence threshold for the objects in 'set'
    \return ObjectType that is second closest to obj */
ObjectT WorldModel::getSecondClosestInSetTo ( ObjectSetT set, ObjectT obj,
        double *dDist,  double dConfThr )
{
    VecPosition v;
    ObjectT     closestObject       = OBJECT_ILLEGAL;
    ObjectT     secondClosestObject = OBJECT_ILLEGAL;
    double      dMinMag             = 1000.0;
    double      dSecondMinMag       = 1000.0;
    int         iIndex;

    if ( dConfThr == -1.0 ) dConfThr = PS->getPlayerConfThr();

    for ( ObjectT o = iterateObjectStart( iIndex, set, dConfThr );
            o != OBJECT_ILLEGAL;
            o = iterateObjectNext ( iIndex, set, dConfThr ) )
    {
        if ( o != obj )
        {
            v = getGlobalPosition( obj ) - getGlobalPosition( o );
            if ( v.getMagnitude() < dMinMag )                 // closer then first
            {
                dSecondMinMag         = dMinMag;                // put first to second
                secondClosestObject   = closestObject;
                dMinMag               = v.getMagnitude();       // and this to first
                closestObject         = o;
            }
            else if ( v.getMagnitude() < dSecondMinMag )      // between 1st and 2nd
            {
                dSecondMinMag         = v.getMagnitude();       // put this to second
                secondClosestObject   = o;
            }
        }
    }
    iterateObjectDone( iIndex );
    if ( dDist != NULL )
        *dDist = dSecondMinMag;
    return secondClosestObject;
}

/*! This method returns the object type of the second closest object relative
    to the agent. Only objects are taken into account within set 'set' and
    which where seen in the last see message.
    \param set ObjectSetT which denotes objects taken into consideration
    \param dDist will be filled with the distance to this this object
    \return ObjectType that is second closest to the agent */
ObjectT WorldModel::getSecondClosestRelativeInSet( ObjectSetT set,
        double *dDist )
{
    ObjectT     closestObject       = OBJECT_ILLEGAL;
    ObjectT     secondClosestObject = OBJECT_ILLEGAL;
    double      dMinMag             = 1000.0;
    double      dSecondMinMag       = 1000.0;
    double      d;
    int         iIndex;

    for ( ObjectT o = iterateObjectStart( iIndex, set, 1.0 );
            o != OBJECT_ILLEGAL;
            o = iterateObjectNext ( iIndex, set, 1.0 ) )
    {
        d = getRelativeDistance( o );
        if ( d < dMinMag )                                // closer then first
        {
            dSecondMinMag         = dMinMag;                // put first to second
            secondClosestObject   = closestObject;
            dMinMag               = d;                      // and this to first
            closestObject         = o;
        }
        else if ( d < dSecondMinMag )                     // between first and 2nd
        {
            dSecondMinMag         = d;                      // put this to second
            secondClosestObject   = o;
        }
    }
    iterateObjectDone( iIndex );
    if ( dDist != NULL )
        *dDist = dSecondMinMag;
    return secondClosestObject;
}


/*! This method returns the object type of the furthest object to the
    ObjectT that is supplied as the second argument. Only objects are
    taken into account that are part of the set 'set' and have a
    confidence higher than the supplied threshold. If no threshold is
    supplied, the threshold defined in PlayerSettings is used.

    \param set ObjectSetT which denotes objects taken into consideration
    \param o ObjectT that represent the type of the object to compare to
    \param dDist will be filled with the furthest distance
    \param dConfThr minimum confidence threshold for the objects in 'set'
    \return ObjectType that is furthest to o */
ObjectT WorldModel::getFurthestInSetTo( ObjectSetT set, ObjectT objTarget,
                                        double *dDist, double dConfThr )
{
    if ( dConfThr == -1.0 ) dConfThr      = PS->getPlayerConfThr();

    ObjectT     furthestObject = OBJECT_ILLEGAL;
    double      dMaxMag       = -1000.0;
    VecPosition v;
    int         iIndex;

    for ( ObjectT o = iterateObjectStart( iIndex, set, dConfThr );
            o != OBJECT_ILLEGAL;
            o = iterateObjectNext ( iIndex, set, dConfThr ) )
    {
        if ( o != objTarget )
        {
            v = getGlobalPosition( objTarget ) - getGlobalPosition( o );
            if ( v.getMagnitude() > dMaxMag )
            {
                dMaxMag        = v.getMagnitude();
                furthestObject = o;
            }
        }
    }
    iterateObjectDone( iIndex );
    if ( dDist != NULL )
        *dDist = dMaxMag;
    return furthestObject;
}

/*! This method returns the type of the object that is located furthest
    relative to the agent. Only objects are taken into account
    that are part of the set 'set'.
    \param set ObjectSetT which denotes objects taken into consideration
    \param dDist will be filled with the furthest relative distance
    \return ObjectType that is furthest to the agent */
ObjectT WorldModel::getFurthestRelativeInSet( ObjectSetT set, double *dDist  )
{
    ObjectT     furthestObject = OBJECT_ILLEGAL;
    double      dMaxMag       = -1000.0;
    int         iIndex;

    for ( ObjectT o = iterateObjectStart( iIndex, set, 1.0 );
            o != OBJECT_ILLEGAL;
            o = iterateObjectNext ( iIndex, set, 1.0 ) )
    {
        if ( getRelativeDistance( o ) > dMaxMag )
        {
            dMaxMag        = getRelativeDistance( o );
            furthestObject = o;
        }
    }
    iterateObjectDone( iIndex );
    if ( dDist != NULL )
        *dDist = dMaxMag;
    return furthestObject;
}


VecPosition WorldModel::getPosClosestOpponentTo( double *dDist, ObjectT o )
{
    if ( o == OBJECT_ILLEGAL )
        o = getAgentObjectType();
    ObjectT objOpp = getClosestInSetTo( OBJECT_SET_OPPONENTS, o, dDist );
    if ( objOpp == OBJECT_ILLEGAL )
        return VecPosition( UnknownDoubleValue, UnknownDoubleValue );

    return getGlobalPosition( objOpp );
}

double WorldModel::getMaxTraveledDistance( ObjectT o )
{
    return (getCurrentTime() - getTimeLastSeen( o ) )*SS->getPlayerSpeedMax();
}


void WorldModel::createInterceptFeatures( )
{
    static int count = 0;
    static Time timeLastCalled(0,0);

    if ( timeLastCalled == getTimeLastSenseMessage() )
        count++;
    else
        count = 0;

    if ( count > 4 )
        cerr << getPlayerNumber() << " called createIntercept too often: " <<
             count << endl;
    // we check all possible next positions of the ball and see
    // whether a player (opponent or teammate) can reach the ball at that point
    // if so, we log this as a feature. We finish when all features have been
    // found.
    ObjectSetT      set = OBJECT_SET_PLAYERS;
    int             iCycles       = -1;
    int             iMinCyclesTeam    = 100;
    int             iMinCyclesOpp     = 100;
    bool            bOnlyMe           = false;

    VecPosition     posObj;
    int             iIndex;
    int             iCyclesToObj  ;

    // no feature available, calculate information
    ObjectT         objFastestTeam             = OBJECT_ILLEGAL;
    ObjectT         objFastestTeamNoGoalie     = OBJECT_ILLEGAL;
    ObjectT         objFastestOpp              = OBJECT_ILLEGAL;
    ObjectT         objFastestPlayer           = OBJECT_ILLEGAL;

    int             iCyclesFastestPlayer       = -1;
    int             iCyclesFastestTeam         = -1;
    int             iCyclesFastestTeamNoGoalie = -1;
    int             iCyclesFastestOpp          = -1;
    int             iCyclesFastestMe           = -1;

    bool            bFinishedPlayer            = false;
    bool            bFinishedTeammates         = false;
    bool            bFinishedTeammatesNoGoalie = false;
    bool            bFinishedOpponents         = false;
    bool            bFinishedMe                = false;
    bool            bFinished                  = false;

    ObjectT         objLog                     = OBJECT_ILLEGAL;
    int             iCyclesLog                 = -1;
    FeatureT        featLog                    = FEATURE_ILLEGAL;

    // for each next position of the ball
    while ( bFinished == false && iCycles <= PS->getPlayerWhenToIntercept() )
    {
        iCycles++;
        iMinCyclesTeam = 100;
        iMinCyclesOpp  = 100;
        Log.log( 460, "fastest loop: %d", iCycles );

        // determine its position and traverse all players to check the teammate
        // and opponent who can reach it first
        posObj     = predictPosAfterNrCycles( OBJECT_BALL, iCycles );
        for ( ObjectT o = iterateObjectStart( iIndex, set );
                o != OBJECT_ILLEGAL;
                o = iterateObjectNext ( iIndex, set ) )
        {
            if ( getGlobalPosition(o).getDistanceTo(posObj)/SS->getPlayerSpeedMax()
                    < iCycles + 1 && (bOnlyMe == false || SoccerTypes::isOpponent( o )
                                      || o == getAgentObjectType() ) )
            {
                Log.log( 460, "call predictNrCyclesToPoint %d %d %d",
                         iCycles, iMinCyclesTeam, iMinCyclesOpp );
                iCyclesToObj = predictNrCyclesToPoint( o, posObj );

                if ( iCyclesToObj < iMinCyclesOpp && SoccerTypes::isOpponent( o ) )
                {
                    iMinCyclesOpp = iCyclesToObj;
                    objFastestOpp = o;
                }
                if ( iCyclesToObj < iMinCyclesTeam && SoccerTypes::isTeammate( o ) )
                {
                    iMinCyclesTeam = iCyclesToObj;
                    objFastestTeam = o;
                }
            }
        }
        iterateObjectDone( iIndex );

        bool bContinue = true;
        bool bLastCall = ( iCycles == PS->getPlayerWhenToIntercept() );
        // log all features that have been solved
        while ( bContinue )
        {
            featLog = FEATURE_ILLEGAL;
            if ( bLastCall )
                iCycles = 100;

            // if player not set yet and either team or opp is smaller than iCycles
            // set fastest player
            if ( bFinishedPlayer == false &&
                    ( min( iMinCyclesTeam, iMinCyclesOpp ) <= iCycles
                      ||
                      bLastCall == true ) )
            {
                featLog              = FEATURE_FASTEST_PLAYER_TO_BALL;
                iCyclesLog           = iCycles;
                iCyclesFastestPlayer = iCycles;
                objLog               = (iMinCyclesTeam<=iMinCyclesOpp) ?
                                       objFastestTeam : objFastestOpp;
                objFastestPlayer     = objLog;
                bFinishedPlayer      = true;
            }
            // if teammate not set yet and min cycles team smaller set it
            else if ( bFinishedTeammates == false &&
                      (iMinCyclesTeam <= iCycles || bFinishedOpponents == true
                       || bLastCall))
            {
                if ( bFinishedOpponents == true )
                    objFastestTeam = getFastestInSetTo( OBJECT_SET_TEAMMATES, posObj,
                                                        VecPosition(0,0), 0, &iCycles );
                featLog            = FEATURE_FASTEST_TEAMMATE_TO_BALL;
                iCyclesLog         = iCycles;
                iCyclesFastestTeam = iCycles;
                objLog             = objFastestTeam;
                bFinishedTeammates = true;
            }
            else if ( bFinishedTeammatesNoGoalie == false &&
                      ( ( iMinCyclesTeam <= iCycles && objFastestTeam != getOwnGoalieType())
                        || bFinishedOpponents == true || bLastCall ) )
            {
                if ( bFinishedOpponents == true && objFastestTeam == getOwnGoalieType())
                    objFastestTeam=getFastestInSetTo( OBJECT_SET_TEAMMATES_NO_GOALIE,
                                                      posObj, VecPosition(0,0), 0, &iCycles );
                featLog                   = FEATURE_FASTEST_TEAMMATE_TO_BALL_NO_GOALIE;
                iCyclesLog                 = iCycles;
                iCyclesFastestTeamNoGoalie = iCycles;
                objLog                     = objFastestTeam;
                objFastestTeamNoGoalie     = objFastestTeam;
                bFinishedTeammatesNoGoalie = true;
            }
            else if ( bFinishedMe == false &&
                      ((iMinCyclesTeam <= iCycles && objFastestTeam == getAgentObjectType())
                       || bFinishedOpponents == true || bLastCall ) )
            {
                if ( bFinishedOpponents == true &&
                        objFastestTeam != getAgentObjectType())
                    iCycles = predictNrCyclesToPoint( getAgentObjectType(), posObj );
                featLog          = FEATURE_INTERCEPT_CYCLES_ME;
                iCyclesLog       = iCycles;
                iCyclesFastestMe = iCycles;
                objLog           = getAgentObjectType();
                bFinishedMe      = true;
            }
            else if ( bFinishedOpponents == false &&
                      ( iMinCyclesOpp <= iCycles  || bLastCall ) )
            {
                featLog            = FEATURE_FASTEST_OPPONENT_TO_BALL;
                iCyclesLog         = iCycles;
                iCyclesFastestOpp  = iCycles;
                objLog             = objFastestOpp;
                bFinishedOpponents = true;

            }
            else
                bContinue = false;

            if ( featLog != FEATURE_ILLEGAL )
            {
                Log.log( 460, "log feature %d object %d in %d cycles sense %d see %d",
                         featLog, objLog, iCyclesLog,getTimeLastSenseMessage().getTime(),
                         getTimeLastSeeMessage().getTime()  );
                setFeature( featLog,
                            Feature( getTimeLastSeeMessage(),
                                     getTimeLastSenseMessage(),
                                     getTimeLastHearMessage(), objLog,
                                     getTimeLastSeeMessage().getTime() + iCyclesLog));
            }
        }
        bFinished = bFinishedTeammates && bFinishedTeammatesNoGoalie;
        if ( bFinished == true )
            bOnlyMe = true;
        bFinished &= bFinishedMe ;
        if ( bFinished == true )
            set = OBJECT_SET_OPPONENTS;
        bFinished &= bFinishedOpponents;
    }
    Log.log( 460, "creatIntercept: team %d me %d opp %d",
             iCyclesFastestTeamNoGoalie, iCyclesFastestMe, iCyclesFastestOpp );
}


/*! This method returns the fastest object to a specified object and fills the
    last argument with the predicted amount of cycles needed to intercept
    this object. Only objects within the set 'set' are taken into
    consideration and the objects have to have a confidence higher than the
    player confidence threshold defined in PlayerSettings.
    \param set ObjectSetT which denotes objects taken into consideration
    \param obj object type of object that should be intercepted
    \param iCyclesToIntercept will be filled with the amount of cycles needed
    \returns object that can intercept object obj fastest */
ObjectT WorldModel::getFastestInSetTo( ObjectSetT set, ObjectT obj,
                                       int *iCyclesToIntercept )
{
    ObjectT  objFastestOpp  = OBJECT_ILLEGAL, objFastestTeam  = OBJECT_ILLEGAL;
    int      iCyclesFastestOpp = 30; // how much do we try
    int     iCyclesFastestTeam;
    bool  bSkip = false;

    FeatureT        feature_type = FEATURE_ILLEGAL;
    ObjectT         fastestObject = OBJECT_ILLEGAL;
    int             iCycles       = -1;

    if ( obj == OBJECT_BALL )
    {
        switch ( set )
        {
        case OBJECT_SET_OPPONENTS:
            feature_type = FEATURE_FASTEST_OPPONENT_TO_BALL;
            break;
        case OBJECT_SET_TEAMMATES:
            feature_type = FEATURE_FASTEST_TEAMMATE_TO_BALL;
            break;
        case OBJECT_SET_TEAMMATES_NO_GOALIE:
            feature_type = FEATURE_FASTEST_TEAMMATE_TO_BALL_NO_GOALIE;
            break;
        case OBJECT_SET_PLAYERS:
            objFastestOpp =
                getFastestInSetTo( OBJECT_SET_OPPONENTS, obj, &iCyclesFastestOpp);
            objFastestTeam =
                getFastestInSetTo( OBJECT_SET_TEAMMATES, obj, &iCyclesFastestTeam);
            if ( iCyclesFastestOpp < iCyclesFastestTeam )
            {
                fastestObject = objFastestOpp;
                iCycles = iCyclesFastestOpp;
            }
            else
            {
                fastestObject = objFastestTeam;
                iCycles = iCyclesFastestTeam;
            }
            bSkip = true;
            feature_type = FEATURE_FASTEST_PLAYER_TO_BALL;
            break;
        default:
            cerr << "WorldModel::getFastestInSetTo unknown set: " << set << endl;
            return OBJECT_ILLEGAL;
        }
        if ( isFeatureRelevant( feature_type ) )
        {
            int i = max(0,
                        ((int)getFeature( feature_type ).getInfo() - getCurrentCycle() ));
            if ( iCyclesToIntercept != NULL )
                *iCyclesToIntercept  = i;
            return getFeature( feature_type ).getObject();
        }

        Log.log( 460, "create intercept features" );
        createInterceptFeatures( );
        Log.log( 460, "call fastest again" );
        return getFastestInSetTo( set, obj, iCyclesToIntercept );
        if ( set == OBJECT_SET_TEAMMATES || set == OBJECT_SET_TEAMMATES_NO_GOALIE )
            objFastestOpp =
                getFastestInSetTo( OBJECT_SET_OPPONENTS, obj, &iCyclesFastestOpp);
    }

    // no feature available, calculate information
    double          dConfThr      = PS->getPlayerConfThr();
    int             iCyclesToObj  ;
    int             iMinCycles    = 100;
    int             iIndex;
    VecPosition     posObj;


    while ( bSkip == false &&
            iCycles < iMinCycles &&
            iCycles <= iCyclesFastestOpp )
    {
        iCycles++;
        iMinCycles = 100;
        posObj     = predictPosAfterNrCycles( obj, iCycles );
        Log.log( 460, "fastest loop: %d fastest_opp %d",
                 iCycles, iCyclesFastestOpp );
        for ( ObjectT o = iterateObjectStart( iIndex, set, dConfThr );
                o != OBJECT_ILLEGAL;
                o = iterateObjectNext ( iIndex, set, dConfThr ) )
        {
            if ( getGlobalPosition(o).getDistanceTo(posObj)/SS->getPlayerSpeedMax()
                    < iMinCycles &&
                    getGlobalPosition(o).getDistanceTo(posObj)/SS->getPlayerSpeedMax()
                    < iCycles + 1 )
            {
                Log.log( 460, "call predictNrCyclesToPoint %d %d",
                         iCycles,iMinCycles );
                iCyclesToObj = predictNrCyclesToPoint( o, posObj );
                if ( iCyclesToObj < iMinCycles )
                {
                    iMinCycles = iCyclesToObj;
                    fastestObject = o;
                }
            }
        }
        iterateObjectDone( iIndex );
    }

    // opponent is faster and we haven't calculated who can go to the
    // interception point the fastest
    if ( fastestObject == OBJECT_ILLEGAL )
        fastestObject = getFastestInSetTo(set,posObj,VecPosition(0,0),0, &iCycles);

    if ( iCyclesToIntercept != NULL )
        *iCyclesToIntercept  = iCycles;

    if ( feature_type != FEATURE_ILLEGAL )
    {
        Log.log( 460, "log feature %d object %d in %d cycles sense %d see %d",
                 feature_type, fastestObject, iCycles,getTimeLastSenseMessage().
                 getTime(), getTimeLastSeeMessage().getTime()  );
        setFeature( feature_type,
                    Feature( getTimeLastSeeMessage(),
                             getTimeLastSenseMessage(),
                             getTimeLastHearMessage(), fastestObject,
                             getTimeLastSeeMessage().getTime() + iCycles ) );
    }

    return fastestObject;
}

/*! This method returns the fastest object to another object that is currently
    located at position 'pos' and has velocity 'vel' that decays with a value
    'dDecay'. The last argument will be filled with the predicted amount of
    cycles needed to reach this object.
    \param set ObjectSetT which denotes objects taken into consideration
    \param pos current position of the object
    \param vel current velocity of the object
    \param dDecay decay value of the velocity of the object
    \param iCyclesToIntercept will be filled with the amount of cycles needed
    \returns object that can reach it fastest */
ObjectT WorldModel::getFastestInSetTo( ObjectSetT set, VecPosition pos,
                                       VecPosition vel, double dDecay, int *iCyclesToIntercept)
{
    double  dConfThr      = PS->getPlayerConfThr();
    ObjectT fastestObject = OBJECT_ILLEGAL;
    int     iCycles       = 0;
    int     iCyclesToObj  ;
    int     iMinCycles    = 100;
    int     iIndex;

    while ( iCycles <= iMinCycles && iCycles < 100)
    {
        iCycles    = iCycles + 1  ;
        iMinCycles = 100;
        Log.log( 460, "fastest to point: %d", iCycles );
        for ( ObjectT o = iterateObjectStart( iIndex, set, dConfThr );
                o != OBJECT_ILLEGAL;
                o = iterateObjectNext ( iIndex, set, dConfThr ) )
        {
            if ( getGlobalPosition(o).getDistanceTo(pos)/SS->getPlayerSpeedMax()
                    < iMinCycles )
            {
                iCyclesToObj = predictNrCyclesToPoint( o, pos );
                if ( iCyclesToObj < iMinCycles )
                {
                    iMinCycles    = iCyclesToObj;
                    fastestObject = o;
                }
            }
        }
        iterateObjectDone( iIndex );
        pos += vel;
        vel *= dDecay;
        if ( vel.getMagnitude( ) < EPSILON ) // we can quit
        {
            iCycles = iMinCycles;
            iMinCycles--;
        }
    }

    if ( iCyclesToIntercept != NULL )
        *iCyclesToIntercept = iCycles;
    return fastestObject;
}

/*! This method returns the first empty spot in the set 'set'. The
    first empty spot is returned as the object which has a lower
    confidence than the threshold player_conf_thr defined in the
    PlayerSettings. This can be used when information of an unknown
    object is perceived. It is set on the first position where there
    is currently no information stored. If 'iUnknownPlayer' is
    specified, the range that corresponds to this unknown player is
    used to dermine the position.

    \param set ObjectSetT consisting of the objects to check
    \param iUnknownPlayer indicates the unknownplayer that has to be mapped

    \return object type of which currently no up to date information
    is stored */
ObjectT WorldModel::getFirstEmptySpotInSet( ObjectSetT set, int iUnknownPlayer)
{
    int     iIndex;

    for ( ObjectT o = iterateObjectStart( iIndex, set,  0.0, true );
            o != OBJECT_ILLEGAL;
            o = iterateObjectNext ( iIndex, set, 0.0, true ) )
    {
        if ( getConfidence( o ) <= PS->getPlayerConfThr() &&
                o != getAgentObjectType() )
            return o;
    }
    return OBJECT_ILLEGAL;
}


/*! This method returns a truth value that represents whether the object
    supplied as the first argument was seen in the last see message. If
    "touch" information was received, i.e. the object was very close to the
    agent but not in its view cone, false is also returned.
    \param ObjectT that represent the type of the object to check
    \return bool indicating whether o was seen in the last see message */
bool WorldModel::isVisible( ObjectT o )
{
    Object *object = getObjectPtrFromType( o );

    if ( object != NULL &&
            object->getTimeLastSeen() == getTimeLastSeeMessage() )
        return true;

    return false;
}

/*! This method determines whether the ball is kicakble, i.e. the ball
    is in the kickable range of the agent (see ServerSettings). This
    value can be different for the different heterogeneous player
    types.

    \return bool indicating whether ball can be kicked. */
bool WorldModel::isBallKickable()
{
    return getRelativeDistance( OBJECT_BALL ) < SS->getMaximalKickDist();
}

/*! This method determines whether the ball is catchable. This only applies
    to a goalie. Three things are tested:
    - the ball hasn't been catched in catch_ban_cycles (see ServerSettings)
      before the current cycle
    - the relative distance to the ball is smaller than the length of the
      catchable area length of the goalie (see also ServerSettings)
    - the ball is in the (own) penalty area.

    \return true when above three constraints are met, false otherwise. */
bool WorldModel::isBallCatchable()
{
    return getTimeSinceLastCatch()            > SS->getCatchBanCycle()  &&
           getRelativeDistance( OBJECT_BALL ) <= SS->getCatchableAreaL() &&
           isInOwnPenaltyArea( getBallPos() );
}

/*! This method checks whether the ball is currently heading towards our own
    goal. For the ball to be heading to our goal a few constraints must be met:
    - ball must be located in our penalty area
    - line of ball heading must intersect with goal line within goal width +
    small constant.
    - ball must pass goal line within 20 cycles.

    If all these constraints are met true is returned, false otherwise
    \return bool indicating whether the ball is heading towards our own goal */
bool WorldModel::isBallHeadingToGoal(  )
{
    int iSide = 1;

    if ( isPenaltyUs() || isPenaltyThem() )
        iSide = ( getSide() == getSidePenalty() ) ? 1 : -1;

    if ( !isConfidenceGood( OBJECT_BALL ) ||
            fabs( getBallPos().getX() ) <  PENALTY_X - 5.0 )
    {
        Log.log( 553, "ball not towards goal: confidence too low" );
        return false;
    }

    // make line from ball heading and goal line
    Line l = Line::makeLineFromPositionAndAngle(getBallPos(),getBallDirection());
    Line l2= Line::makeLineFromTwoPoints( getPosOwnGoal(), getPosOwnGoal() +
                                          VecPosition( 0, 10 ));

    // if intersection is outside goalwidth, not heading to goal
    VecPosition posIntersect = l.getIntersection( l2 );
    if ( fabs(posIntersect.getY()) > SS->getGoalWidth()/2.0 + 3.0)
    {
        Log.log( 553, "ball not towards goal: outside goal %f",
                 posIntersect.getY());
        return false;
    }

    // check whether ball will be behind goal line within 20 cycles.
    VecPosition pos    = getBallPos();
    int         iCycle = 1;
    while ( fabs( pos.getX() ) < PITCH_LENGTH/2.0 && iCycle < 20)
    {
        pos = predictPosAfterNrCycles( OBJECT_BALL, iCycle );
        Log.log( 553, "predicted pos %d cycles: (%f,%f)" ,
                 iCycle, pos.getX(), pos.getY() );
        iCycle ++;
    }

    return ( iCycle == 20 ) ? false : true;
}

/*! This method returns whether the ball is in our possesion. This is defined
     by the fact if the fastest player to the ball is a teammate or not.
     \return bool indicating whether a teammate is the fastest player to the
             ball. */
bool WorldModel::isBallInOurPossesion( )
{
    int     iCyc;
    ObjectT o = getFastestInSetTo( OBJECT_SET_PLAYERS, OBJECT_BALL, &iCyc );

    if ( o == OBJECT_ILLEGAL )
        return false;
    if ( SoccerTypes::isTeammate( o ) )
        return true;
    else
        return false;
}

/*! This method returns whether the ball lies in the own penalty area.
    \return bool indicating whether ball lies in own penalty area.     */
bool WorldModel::isBallInOwnPenaltyArea( )
{
    return isInOwnPenaltyArea( getBallPos() );
}

/*! This method returns whether the specified position lies in the own penalty
    area.
    \param pos position which should be checked
    \return bool indicating whether 'pos' lies in own penalty area. */
bool WorldModel::isInOwnPenaltyArea( VecPosition pos )
{
    ObjectT     objFlag = ( getSide() == SIDE_LEFT  )
                          ?  OBJECT_FLAG_P_L_C
                          :  OBJECT_FLAG_P_R_C ;

    if ( isPenaltyUs() || isPenaltyThem() )
        objFlag = ( getSidePenalty() == SIDE_LEFT ) ? OBJECT_FLAG_P_L_C
                  : OBJECT_FLAG_P_R_C ;
    VecPosition posFlag =SoccerTypes::getGlobalPositionFlag( objFlag, getSide());
    if ( fabs(pos.getX())   > fabs(posFlag.getX()) &&
            fabs( pos.getY() ) < PENALTY_AREA_WIDTH/2.0 )
        return true;

    return false;
}

/*! This method returns whether the specified position lies in the opponent
    penalty area.
    \param pos position which should be checked
    \return boolean indicating whether 'pos' lies in opponent penalty area. */
bool WorldModel::isInTheirPenaltyArea( VecPosition pos )
{
    ObjectT     objFlag = ( getSide() == SIDE_LEFT )
                          ?  OBJECT_FLAG_P_R_C
                          :  OBJECT_FLAG_P_L_C ;
    VecPosition posFlag = SoccerTypes::getGlobalPositionFlag( objFlag,getSide());

    if ( pos.getX() > posFlag.getX() &&
            fabs(pos.getY()) < PENALTY_AREA_WIDTH/2.0 )
        return true;

    return false;
}

/*! This method determines whether the confidence for 'o' is good. The
    confidence of the object is compared to the player_conf_thr
    defined in PlayerSettings. When the confidence is higher than this
    value and the object does not equal the agent object type true is
    returned, otherwise false.

    \param o object of which confidence value should be returned
    \return bool indicating whether object information has good confidence. */
bool WorldModel::isConfidenceGood( ObjectT o )
{
    return getConfidence( o ) > PS->getPlayerConfThr() &&
           o != getAgentObjectType();
}

/*! This method determines whether the confidence for 'o' is very
    good. The confidence of the object is compared to the
    player_high_conf_thr defined in PlayerSettings. When the
    confidence is higher than this value and the object does not equal
    the agent object type true is returned, otherwise false.

    \param o object of which confidence value should be returned
    \return bool indicating whether object information has good confidence. */
bool WorldModel::isConfidenceVeryGood( ObjectT o )
{
    return getConfidence( o ) > PS->getPlayerHighConfThr() &&
           o != getAgentObjectType();
}

/*! This method checks whether the specified object stands onside. This is done
    by comparing the x coordinate of the object to the offside line.
    \return boolean indicating whether 'obj' stands onside. */
bool WorldModel::isOnside( ObjectT obj )
{
    return getGlobalPosition( obj ).getX() < getOffsideX() - 0.5 ;
}

/*! This method determines whether there stands an opponent in the global
    direction of the specified angle and in distance 'dDist'. An opponent is
    considered to stand in the global direction when the angle difference with
    the specified angle is smaller than 60 degrees.
    \param ang angle of the global direction in which to check opponents
    \param dDist distance in which opponents should be checked
    \return bool indicating wheter an opponent was found. */
bool WorldModel::isOpponentAtAngle( AngDeg ang , double dDist )
{
    VecPosition posAgent   = getAgentGlobalPosition();
    VecPosition posOpp;
    AngDeg      angOpp;
    int         iIndex;

    for ( ObjectT o = iterateObjectStart( iIndex, OBJECT_SET_OPPONENTS );
            o != OBJECT_ILLEGAL;
            o = iterateObjectNext ( iIndex, OBJECT_SET_OPPONENTS ) )
    {
        posOpp    = getGlobalPosition( o );
        angOpp    = ( posOpp - posAgent ).getDirection() ;
        if ( fabs( angOpp - ang ) < 60 &&
                posAgent.getDistanceTo( posOpp ) < dDist )
            return true;
        else if ( fabs( angOpp - ang ) < 120 &&
                  posAgent.getDistanceTo( posOpp ) < dDist/2.0 )
            return true;
    }
    iterateObjectDone( iIndex );
    return false;
}

/*! This method returns the inverse confidence, i.e. the time that belongs
    to the specified confidence. This can be used to determine
    the time the object was last seen when the confidence is given. Herefore
    the current time is used.
    \param dConf confidence
    \return server cycle the object was last seen. */
Time WorldModel::getTimeFromConfidence( double dConf )
{
    return getCurrentTime()-(int)((1.00-dConf)*100);
}

/*! This method returns the object type of the last opponent defender. This
    opponent resembles the offside line.
    \param if non-null dX will be filled with the x position of this object
    \return object type of the last opponent defender */
ObjectT WorldModel::getLastOpponentDefender( double *dX )
{
    double  dHighestX = 0.0;
    double  dSecondX  = 0.0, x;

    ObjectT o, oLast = OBJECT_ILLEGAL, oSecondLast = OBJECT_ILLEGAL;
    for ( int i = 0; i < MAX_OPPONENTS ; i ++ )
    {
        o = Opponents[i].getType();
        if ( isConfidenceGood( o ) )
        {
            x = Opponents[i].getGlobalPosition().getX();
            if ( x > dHighestX )        // if larger x than highest
            {
                dSecondX    = dHighestX;  // make second the previous highest
                dHighestX   = x;          // and this the new one
                oSecondLast = oLast;
                oLast       = o;
            }
            else if ( x > dSecondX )    // if smaller than 1st  and larger than 2nd
            {
                dSecondX    = x;          // make it the second
                oSecondLast = o;
            }
        }
    }

    // if highest x is outside pen_area, it cannot be the goalie (unless playing
    // Portugal ;-) ), so assume goalie is just not seen
    if ( dHighestX < PENALTY_X && getOppGoalieType() == OBJECT_ILLEGAL )
    {
        dSecondX    = dHighestX;
        oSecondLast = oLast;
    }
    if ( dX != NULL )
        *dX = dSecondX ;
    return oSecondLast;
}

/*! This method returns the x coordinate of the offside line using the known
    information in the WorldModel. If a player moves beyond this line, he
    stands offside. First the opponent with the second highest x coordinate is
    located, then the maximum of this x coordinate and the ball x coordinate
    is returned.
    \param bIncludeComm boolean indicating whether communicated offside line
          should also be included.
    \return x coordinate of the offside line. */
double WorldModel::getOffsideX( bool bIncludeComm )
{
    double  x, dAgentX;

    getLastOpponentDefender( &dAgentX );
    x = getBallPos().getX();
    x = max( x, dAgentX );
    if ( bIncludeComm == true && getCurrentTime() - m_timeCommOffsideX < 3 )
        x = max( x, m_dCommOffsideX );
    return x ;
}

/*! This method returns the outer position on the field given a position 'pos'
    and a global angle 'ang'. The outer position is defined as the point on
    the field where the line created from this position and angle crosses
    either a side line, goal line or penalty line. To be on the safe side a
    small value is specified, which denotes the distance from the side line
    that should be returned.
    \param pos position on the field from which outer position should be
    calculated
    \param ang global angle which denotes the global direction in pos
    \param dDist distance from line
    \param bWithPenalty boolean denoting whether penalty area should be taken
                        into account (if false only goal line and side line
                        are used.
    \return position denoting the outer position on the field */
VecPosition WorldModel::getOuterPositionInField( VecPosition pos, AngDeg ang,
        double dDist, bool bWithPenalty )
{
    VecPosition posShoot;

    // make shooting line using position and desired direction
    Line lineObj     = Line::makeLineFromPositionAndAngle( pos, ang );

    // get intersection point between the created line and goal line
    Line lineLength  = Line::makeLineFromPositionAndAngle(
                           VecPosition( PITCH_LENGTH/2.0 - dDist, 0.0 ), 90 );
    posShoot         = lineObj.getIntersection( lineLength );

    // check whether it first crosses the penalty line
    Line linePenalty = Line::makeLineFromPositionAndAngle(
                           VecPosition( PENALTY_X - dDist, 0.0 ), 90.0 );
    double dPenaltyY = lineObj.getIntersection(linePenalty).getY();

    if ( bWithPenalty && fabs(dPenaltyY) < PENALTY_AREA_WIDTH/2.0 )
    {
        if ( fabs(dPenaltyY) < PENALTY_AREA_WIDTH/2.0 - 5.0 ||  // crosses inside
                fabs(posShoot.getY()) <  PENALTY_AREA_WIDTH/2.0 )   // or ends inside
            posShoot = lineObj.getIntersection( linePenalty );
    }

    // check where it crosses the side line
    Line lineSide = ( ang < 0 )
                    ? Line::makeLineFromPositionAndAngle(
                        VecPosition( 0.0, - PITCH_WIDTH/2.0 + dDist ),0.0 )
                    : Line::makeLineFromPositionAndAngle(
                        VecPosition( 0.0, + PITCH_WIDTH/2.0 - dDist ),0.0 );

    if ( fabs(posShoot.getY()) > PITCH_WIDTH/2.0 - dDist )
        posShoot = lineObj.getIntersection( lineSide );

    return posShoot;
}

/*! This method determines the (global) direction which has the largest
    angle between the opponents and is located in the interval angMin..
    angMax.
    \param origin of which the angles angMin and angMax are based on.
    \param angMin minimal global direction that should be returned
    \param angMax maximal global direction that should be returned
    \param angLargest will contain the size of the largest angle of the
           direction that is returned
    \param dDist only opponents with relative distance smaller than this value
           will be taken into account.
    \return global direction with the largest angle between opponents */
AngDeg WorldModel::getDirectionOfWidestAngle(VecPosition posOrg, AngDeg angMin,
        AngDeg angMax, AngDeg *angLargest, double dDist)
{
    list<double> v;
    list<double> v2;
    double       temp;
    int          iIndex;
    double       dConf  = PS->getPlayerConfThr();

    // add all angles of all the opponents to the list v
    for ( ObjectT o = iterateObjectStart( iIndex, OBJECT_SET_OPPONENTS, dConf );
            o != OBJECT_ILLEGAL;
            o = iterateObjectNext ( iIndex, OBJECT_SET_OPPONENTS, dConf ) )
    {
        if ( getRelativeDistance( o ) < dDist )
            v.push_back( (getGlobalPosition(o)-posOrg).getDirection());
    }
    iterateObjectDone( iIndex );
    v.sort();

    // if goalkeeper is spotted and he is located within the range that we want
    // to shoot at, make sure the angle with the goalkeeper is large enough, since
    // he has better intercepting capabilities than the normal players

    ObjectT     objGoalie = getOppGoalieType();
    VecPosition posGoalie = getGlobalPosition( objGoalie );
    AngDeg      angGoalie;

    if ( objGoalie != OBJECT_ILLEGAL && posOrg.getX() > PITCH_LENGTH/4.0 &&
            posOrg.getDistanceTo( posGoalie ) < dDist )
    {
        angGoalie = ( posGoalie - posOrg ).getDirection();
        Log.log( 560, "direction_widest_angle: min %f max %f angGoalie %f",
                 angMin, angMax, angGoalie );

        if ( posOrg.getY() > 0 ) // right side of the field
        {
            angGoalie = VecPosition::normalizeAngle( angGoalie - 33 );
            angMax    = max( angMin, min( angGoalie, angMax ) );
        }
        else
        {
            angGoalie = VecPosition::normalizeAngle( angGoalie + 33 );
            angMin    = min( angMax, max( angMin, angGoalie ) );
        }
        Log.log( 560, "direction_widest_angle after: %f %f", angMin, angMax );
    }


    // Create new list with only opponents from interval [angMin..angMax].
    // Note that opponents outside angMin and angMax can have an influence
    // on the largest angle between the opponents, so they should be accounted
    // for. To this end, a projection is defined in both angMin and angMax.
    // The opponent with the smallest global angle difference a to angMin
    // (either inside or outside the interval [angMin..angMax]) is determined
    // and an extra angle angMin - a is added to the list. The situation for
    // angMax is analogous.

    double absMin     = 1000;
    double absMax     = 1000;
    double angProjMin = angMin;
    double angProjMax = angMax;
    double array[MAX_OPPONENTS+2];

    while ( v.size() > 0 )
    {
        if ( fabs( v.front() - angMin ) < absMin )     // opp near angMin
        {
            absMin     = fabs( v.front() - angMin ) ;    // make angMin wider
            angProjMin = angMin - absMin;                // to take him into account
        }
        if ( fabs( v.front() - angMax ) < absMax )     // opp near angMax
        {
            absMax     = fabs( v.front() - angMax ) ;    // make angMax wider
            angProjMax = angMax + absMax;                // to take him into account
        }
        if ( v.front() > angMin && v.front() < angMax ) // opp in range
            v2.push_back( v.front() );                   // add him
        v.pop_front();
    }

    // make all angles relative to angProjMin which has angle 0 and set them in
    // the range 0..360, where the range -180..0 is moved to 180..360. Do this by
    // adding 360 and then subtracting 360 if value is larger than 360.
    v.push_back( 0 );
    while ( v2.size() > 0 ) // for all the opponents
    {
        temp = VecPosition::normalizeAngle(v2.front()-angProjMin)+360.0;
        if ( temp > 360 )
            temp -= 360;
        v.push_back( temp );
        v2.pop_front();
    }
    // add max projection.
    temp = VecPosition::normalizeAngle(angProjMax-angProjMin)+360.0;
    if ( temp > 360 )
        temp -= 360;

    v.push_back( temp );

    // sort the list
    v.sort();

    // put all the values in an array
    int i = 0;
    while ( v.size() > 0 )
    {
        array[i++] = v.front();
        v.pop_front();
    }

    // find the largest angle and determine the associated midpoint direction
    double dLargest = -1000;
    double d;
    double ang      = UnknownAngleValue;
    for ( int j = 0; j < i - 1 ; j ++ )
    {
        d = VecPosition::normalizeAngle(( array[j+1] - array[j] )/2.0);
        if ( d > dLargest )
        {
            ang = angProjMin + array[j] + d;
            ang = VecPosition::normalizeAngle( ang );
            dLargest = d;
        }
    }

    if ( ang == UnknownAngleValue ) // no angle found -> get angle in between
    {
        ang = getBisectorTwoAngles( angMin, angMax );
        if ( angLargest != NULL )
            *angLargest = 360;
    }
    else if ( angLargest != NULL )
        *angLargest = dLargest;

    return ang;
}

/*! This method returns whether the position 'pos' is inside the playfield. */
bool WorldModel::isInField( VecPosition pos, double dMargin )
{
    return Rect(
               VecPosition( + PITCH_LENGTH/2.0 - dMargin,
                            - PITCH_WIDTH/2.0  + dMargin ),
               VecPosition( - PITCH_LENGTH/2.0 + dMargin,
                            + PITCH_WIDTH/2.0  - dMargin )
           ).isInside( pos );
}

/*! This method returns whether the position 'pos' is before the opp goal. */
bool WorldModel::isBeforeGoal( VecPosition pos )
{
    return Rect(
               VecPosition( + PENALTY_X - 2,    - ( SS->getGoalWidth()/2.0 + 1)),
               VecPosition( + PITCH_LENGTH/2.0, + ( SS->getGoalWidth()/2.0 + 1))
           ).isInside( pos );
}

/*! This method determine the strategic position for the specified object. This
    is done using the Formations class. In this class all information
    about the current formation, player number in formation and otheic
    values are stored. The strategic position is based on the position of
    the ball. If the confidence in the position of the ball is lower than the
    threshold defined in PlayerSettings, it is assumed that the ball is at
    position (0,0).
    \param obj for which the strategic position should be calculated.
    \param ft formation for which to calculate the strategic position
    \return VecPosition strategic position for player 'iPlayer' */
VecPosition WorldModel::getStrategicPosition( ObjectT obj, FormationT ft )
{
    return getStrategicPosition( SoccerTypes::getIndex( obj ), ft );
}

/*! This method determine the strategic position for the specified player. This
    is done using the Formations class. In this class all information
    about the current formation, player number in formation and otheic
    values are stored. The strategic position is based on the position of
    the ball. If the confidence in the position of the ball is lower than the
    threshold defined in PlayerSettings, it is assumed that the ball is at
    position (0,0).
    \param iPlayer role in formation for which strategic position should be
      returnd. With default value is -1 it is assumed that the strategic
      position of the agent itself should be returned.
    \param ft formation for which to calculate the strategic position
    \return VecPosition strategic position for player 'iPlayer' */
VecPosition WorldModel::getStrategicPosition( int iPlayer, FormationT ft )
{
    if ( iPlayer > MAX_TEAMMATES )
        cerr << "WM:getStrategicPosition with player nr " << iPlayer << endl;

    VecPosition pos, posBall = getBallPos();
    bool bOwnBall = isBallInOurPossesion();

    // -1 is default -> get player number in formation
    if ( iPlayer == -1 )
        iPlayer = formations->getPlayerInFormation();

    // get maximal allowed x coordinate, this is offside x coordinate
    double dMaxX = max( -0.5, getOffsideX() - 1.5 );

    if ( bOwnBall &&
            getGlobalPosition(
                SoccerTypes::getTeammateObjectFromIndex(iPlayer)).getX()
            < posBall.getX() )
        dMaxX = max( dMaxX, posBall.getX()  );

    // after standing offside we are not allowed to move for ball
    // with a goal kick of them we are not allowed to move into penalty area

    if ( isGoalKickThem() )
        dMaxX = min( dMaxX, PENALTY_X - 1.0 );
    else if ( isBeforeKickOff() )
        dMaxX = min( dMaxX, -2.0 );
    else if ( isOffsideUs() )
        dMaxX = posBall.getX() - 0.5;

    // change the ball position on which strategic position is based
    // depending on the different deadball situation and thus the
    // expected movement of the ball
    if ( isBeforeKickOff() )
        posBall.setVecPosition( 0, 0 );
    else if ( isGoalKickUs() ||
              getTimeSinceLastCatch(  ) < PS->getCyclesCatchWait() + 5  ||
              ( isFreeKickUs() && posBall.getX() < - PENALTY_X ) )
        posBall.setX( -PITCH_LENGTH/4 + 5.0 );
    else if ( getConfidence( OBJECT_BALL ) < PS->getBallConfThr() )
        posBall.setVecPosition( 0.0, 0.0 );
    else if ( isGoalKickThem() ||
              ( isFreeKickThem() && posBall.getX() > PENALTY_X ) )
        posBall.setX( PENALTY_X - 10.0 );
    else if ( isFreeKickThem() )
        posBall.setX( posBall.getX() - 5.0 );
    else if ( isBallInOurPossesion() &&
              !( isDeadBallUs() || isDeadBallThem() ) )
        posBall.setX( posBall.getX() + 5.0 );
    else if ( posBall.getX() < - PENALTY_X + 5.0 )
        posBall = predictPosAfterNrCycles( OBJECT_BALL, 3 );

    // get the strategic position
    pos = formations->getStrategicPosition( iPlayer, posBall, dMaxX,
                                            bOwnBall, PS->getMaxYPercentage(),
                                            ft );
    return pos;
}

/*! This method returns a global position on the field which denotes
    the position to mark position 'pos'. It receives three arguments:
    a position pos (usually an opponent) that the agent wants to mark,
    a distance 'dDist' representing the desired distance between o and
    the marking position and a type indicator that denotes the type of
    marking that is required. We distinguish three types of marking: -
    MARK BALL: marking the opponent by standing at a distance 'dDist'
    away from him on the line between him and the ball. This type of
    marking will make it di�cult for the opponent to receive a pass.
    - MARK GOAL: marking the opponent by standing at a distance
    'dDist' away from him on the line between him and the center point
    of the goal he attacks. This type of marking will make it
    difficult for the opponent to score a goal.  - MARK BISECTOR:
    marking the opponent by standing at a distance 'dDist' away from
    him on the bisector of the ball-opponent-goal angle. This type of
    marking enables the agent to intercept both a direct and a leading
    pass to the opponent.  \param pos position that has to be marked
    \param dDist distance marking position is located from object
    position \param mark marking technique that should be used \return
    position that is the marking position. */
VecPosition WorldModel::getMarkingPosition( VecPosition pos, double dDist,
        MarkT mark)
{
    VecPosition posBall  = getBallPos();
    //edictPosAfterNrCycles( OBJECT_BALL, 3 );
    VecPosition posGoal  = getPosOwnGoal( );
    if ( posBall.getX() < - PITCH_LENGTH/2.0 + 10.0 )
        posGoal.setX( posBall.getX() + 1  );
    else if ( posBall.getX() > -PITCH_LENGTH/3.0 )
    {
        posGoal.setX( -PITCH_LENGTH/2.0 );
        double dY  = posBall.getY();
        if ( fabs( dY ) > 12 )
            dY += ( sign( dY ) > 0   ) -5 ? : 5 ;
        posGoal.setY( dY );
    }

    VecPosition posAgent = getAgentGlobalPosition();
    VecPosition posMark;
    AngDeg      ang, angToGoal, angToBall;

    if ( mark == MARK_GOAL )                      // position in direction goal
    {
        angToGoal = (posGoal-pos).getDirection( );
        Line line = Line::makeLineFromTwoPoints( pos, posGoal );

        // we want to know when distance from ball to point p equals distance
        // from opp to point p :
        // d1 + d3 = sqrt(d2^2 + d3^2) > (d1+d3)^2 = d2^2 + d3^2 =>
        // d1^2 + 2*d1*d3 = d2^2 -> d3 = (d2^2 - d1^2 ) / 2*d1
        double dCalcDist;
        VecPosition posIntersect = line.getPointOnLineClosestTo( posAgent );
        double dDistAgent = posIntersect.getDistanceTo( posAgent );
        double dDistOpp = posIntersect.getDistanceTo( pos );
        dCalcDist = (dDistAgent*dDistAgent-dDistOpp*dDistOpp)/(2*dDistOpp);
        double dExtra = 2.0;
        //    if( posBall.getX() <  PENALTY_X + 5 )
        if ( pos.getDistanceTo(posAgent) < 5 )
            dExtra = 0.0;
        dCalcDist += dDistOpp + dExtra;
        Log.log( 513, "dDistOpp %f dDistAgent %f calc %f min %f",
                 dDistOpp, dDistAgent, dCalcDist, 0.75*pos.getDistanceTo(posGoal));
        dCalcDist = min( dCalcDist, 0.75*pos.getDistanceTo( posGoal ) );
        double x = -PITCH_LENGTH/2 + 4;
        double y = line.getYGivenX( x);
        posMark = pos + VecPosition( dCalcDist, angToGoal, POLAR );
        if ( posMark.getX() < x )
        {
            Log.log( 513, "change posmark to (%f,%f)", x, y );
            posMark.setVecPosition( x, y );
        }
        // if interception point iss outside range or very close to marking
        // point, but far away from opp (is this possible?) move closer.
        if ( ! line.isInBetween( posMark, pos, posGoal ) ||
                ( posMark.getDistanceTo( posAgent ) < 1.5 &&
                  posMark.getDistanceTo( pos ) > 2*dDist ) )
        {
            Log.log( 513, "set marking position at dDist %f", min(dDistAgent,7.0) );
            posMark   = pos + VecPosition( min( dDistAgent, 7.0 ), angToGoal, POLAR );
        }
        Log.log( 513, "marking position calc (%f,%f) pos(%f,%f) calcdist %f",
                 posMark.getX(), posMark.getY(), pos.getX(), pos.getY(),
                 dCalcDist );
    }
    else if ( mark == MARK_BALL )                 // position in direction ball
    {
        angToBall = (posBall-pos).getDirection( );
        posMark   = pos + VecPosition( dDist, angToBall, POLAR );
    }
    else if ( mark == MARK_BISECTOR )            // pos between ball and goal
    {
        angToBall = (posBall - pos).getDirection( );
        angToGoal = (posGoal - pos).getDirection( );
        ang       = getBisectorTwoAngles( angToBall, angToGoal );
        posMark   = pos + VecPosition( dDist, ang ,POLAR );
    }
    if ( fabs( posMark.getX() ) > PITCH_LENGTH/2.0 - 2.0 )
        posMark.setX( sign(posMark.getX())*(PITCH_LENGTH/2.0 - 2.0) );
    return posMark;

}


/*! The actual power with which the ball is kicked depends on the
    relative location of the ball to the player. The kick is more
    powerful when the ball is very close to and in front of the player.
    The actual kickpowerrate with which the power of the kick command
    is multiplied is equal to<BR>
    KickPowerRate*(1 - 0.25*DirDiff / 180 -
    0.25*(DistBall-PlayerSize-BallSize)/KickableMargin)<BR>
    with DirDiff = global angle of the ball rel to the body dir agent<BR>
         DistBall = the distance from the center of the player to the ball<BR>
    See soccermanual
    \return the actual kick power rate with which power is multiplied  */
double WorldModel::getActualKickPowerRate( )
{
// true indicates that relative angle to body should be returned
    double dir_diff      = fabs( getRelativeAngle( OBJECT_BALL, true ) );
    double dist          = getRelativeDistance( OBJECT_BALL ) -
                           SS->getPlayerSize( ) - SS->getBallSize( );
    return SS->getKickPowerRate() *
           ( 1 - 0.25 * dir_diff/180.0 - 0.25 * dist / SS->getKickableMargin());
}

/*! The actual power with which the ball must be kicked depends on the
    relative location of the ball to the player. The kick is more
    powerful when the ball is very close to and in front of the player.
    The actual power with which the ball must be kicked is equal to<BR>
    Speed / KickPowerRate*(1 - 0.25*DirDiff / 180 -
    0.25*(DistBall-PlayerSize-BallSize)/KickableMargin)<BR>
    with DirDiff = global angle of the ball rel to the body dir agent<BR>
         DistBall = the distance from the center of the player to the ball<BR>
    See soccermanual for further information.
    This method receives a speed vector which the ball should have after the
    kick command and calculates the power for the kick command to reach this.
    This value can be higher than is possible to shoot!
    \param dDesiredSpeed the desired speed after the kick command
    \return the actual power for kick command to get dDesiredSpeed */
double WorldModel::getKickPowerForSpeed( double dDesiredSpeed )
{
    // acceleration after kick is calculated by power * eff_kick_power_rate
    // so actual kick power is acceleration / eff_kick_power_rate
    return dDesiredSpeed / getActualKickPowerRate( );
}


/*! This method determines the power with which the ball must be kicked in
    order to travel a given distance and still have a speed after that
    distance
    \param dDistance distance ball should travel
    \param dEndSpeed speed ball should have at target position
    \return power value for kick command */
double WorldModel::getKickSpeedToTravel( double dDistance, double dEndSpeed )
{
    // if endspeed is zero we have an infinite series and return the first term
    // that corresponds to the distance that has to be travelled.
    if ( dEndSpeed < 0.0001  )
        return Geometry::getFirstInfGeomSeries(dDistance, SS->getBallDecay() );

    // use geometric series to calculate number of steps and with that the
    // velocity to give to the ball, we start at endspeed and therefore use
    // the inverse of the ball decay (r).
    // s = a + a*r + .. a*r^n since we calculated from endspeed (a) to
    // firstspeed, firstspeed equals a*r^n = endspeed*r^nr_steps
    double dNrSteps = Geometry::getLengthGeomSeries( dEndSpeed,
                      1.0/SS->getBallDecay( ), dDistance );
    return getFirstSpeedFromEndSpeed( dEndSpeed, (int)rint(dNrSteps) ) ;
}


/*! This method returns the speed that has to be given to the ball when
    it should have an endspeed of 'dEndSpeed' after 'dCycles' number of cycles.
    This can be calculated using a geometric series.
    \param dEndSpeed desired end speed for the ball
    \param dCycles nr of cycles after which ball should have speed 'dEndSpeed'
    \return initial speed given to the ball to have speed 'dEndSpeed' after
    'dCycles' cycles. */
double WorldModel::getFirstSpeedFromEndSpeed( double dEndSpeed, double dCycles,
        double dDecay  )
{
    if ( dDecay < 0 )
        dDecay = SS->getBallDecay();

    // geometric serie: s = a + a*r^1 + .. + a*r^n, now given endspeed = a*r^n ->
    // endspeed = firstspeed * ratio ^ length ->
    // firstpeed = endspeed * ( 1 / ratio ) ^ length
    return dEndSpeed * pow( 1 / dDecay, dCycles );
}

/*! This method returns the speed that has to be given to an object when
    it should have travelled a distance 'dDist' after 'dCycles' number of
    cycles. This can be calculated using a geometric series where 'dDecay'
    is the used decay factor (default this value equals ball_decay).
    \param dDist distance the ball has to travel
    \param dCycles nr of cycles after which ball should have travelled 'dDist'
    \param dDecay decay of the geometric series.
    \return initial speed for the ball to travel 'dDist' in 'dCycles' cycles */
double WorldModel::getFirstSpeedFromDist( double dDist, double dCycles, double
        dDecay )
{
    if ( dDecay < 0 )
        dDecay = SS->getBallDecay();

    return Geometry::getFirstGeomSeries( dDist, dDecay, dCycles);
}

/*! This method returns the speed the ball will have after 'dCycles' cycles
    when it is given an initial speed of 'dFirstSpeed'.
    This can be calculated using a geometric series.
    \param dFirstSpeed given speed to the ball
    \param dCycles nr of cycles after which ball speed should be determined
    \return speed of the ball after 'dCycles' server cycles */
double WorldModel::getEndSpeedFromFirstSpeed(double dFirstSpeed,double dCycles)
{
    // geometric series: s = a + a*r^1 + .. + a*r^n, with firstspeed = a ->
    // endspeed = firstspeed * ratio ^ length ;
    return dFirstSpeed * pow( SS->getBallDecay(), dCycles );
}

/*! This method determines the angle that should be sent to the soccerserver
when
    the player wants to turn angDesiredAngle. This value depends on the current
    velocity and the inertia moment of the player
    \param angDesiredAngle angle that player wants to turn
    \param dSpeed current speed of the player
    \return angle that can be sent with turn command */
AngDeg WorldModel::getAngleForTurn( AngDeg angDesiredAngle, double dSpeed,
                                    ObjectT obj )
{
    AngDeg a = angDesiredAngle * (1.0 + getInertiaMoment( obj ) * dSpeed );
    if ( a > SS->getMaxMoment() )
        return SS->getMaxMoment() ;
    else if ( a < SS->getMinMoment() )
        return SS->getMinMoment() ;
    else
        return a;
}

/*! This method determines the actual angle that is used when 'angTurn' is
    sent to the SoccerServer. This value depends on the current velocity and
    the inertia moment of the player
    \param angAngleForSend angle send with turn command
    \param dSpeed current speed of the player
    \return actual angle that player is turned */
AngDeg WorldModel::getActualTurnAngle( AngDeg angTurn,double dSpeed,ObjectT o )
{
    return angTurn / (1.0 + getInertiaMoment( o ) * dSpeed );
}

/*! This method determines the optimal dash power to mantain an optimal speed
    When the current speed is too high and the distance is very small, a
    negative dash is performed. Otherwise the difference with the maximal speed
    is determined and the dash power rate is set to compensate for this
    difference.
    \param posRelTo relative point to which we want to dash
    \param angBody body angle of the agent
    \param vel current velocity of the agent
    \param dEffort current effort of the player
    \param iCycles desired number of cycles to reach this point
    \return dash power that should be sent with dash command */
double WorldModel::getPowerForDash( VecPosition posRelTo, AngDeg angBody,
                                    VecPosition vel, double dEffort, int iCycles )
{
    // the distance desired is the x-direction to the relative position we
    // we want to move to. If point lies far away, we dash maximal. Furthermore
    // we subtract the x contribution of the velocity because it is not necessary
    // to dash maximal.
    double dDist = posRelTo.rotate(-angBody).getX(); // get distance in direction
    if ( iCycles <= 0 ) iCycles = 1;
    double dAcc  = getFirstSpeedFromDist(dDist,iCycles,SS->getPlayerDecay());
    // get speed to travel now
    if ( dAcc > SS->getPlayerSpeedMax() )            // if too far away
        dAcc = SS->getPlayerSpeedMax();                // set maximum speed
    dAcc -= vel.rotate(-angBody).getX();             // subtract current velocity

    // acceleration = dash_power * dash_power_rate * effort ->
    // dash_power = acceleration / (dash_power_rate * effort )
    double dDashPower = dAcc/(SS->getDashPowerRate() * dEffort );
    if ( dDashPower > SS->getMaxPower() )
        return SS->getMaxPower();
    else if ( dDashPower < SS->getMinPower() )
        return SS->getMinPower();
    else
        return dDashPower;
}


/*! This method returns the closest player in the current formation to the
    position 'pos'. In case of a dead ball situation this method can be used
    to determine whether you should move to the ball.
    \param pos position which is used in the comparision
    \param bIncludeGoalie boolean to determine whether goalkeeper should be
           taken into account
    \param ps player set to which the returned player must belong
    \return role number of the agent in the current formation who is closest
            to this position */
int WorldModel::getClosestPlayerInFormationTo( VecPosition pos,
        bool bIncludeGoalie,
        ObjectT objWithout,
        PlayerSetT ps,
        FormationT ft )
{
    double      dDist = 1000.0;
    VecPosition posStrat;
    int         iPlayer = -1;

    for ( int i = 0; i < MAX_TEAMMATES; i++ )
    {
        if ( bIncludeGoalie == false && i == 0 )
            continue;
        else if ( objWithout == SoccerTypes::getTeammateObjectFromIndex( i ) )
            continue;
        else if ( !SoccerTypes::isPlayerTypeInSet(
                      formations->getPlayerType(i,ft), ps ))
            continue;

        posStrat = getStrategicPosition( i, ft );

        if ( isDeadBallUs( )&&
                getBallPos().getX() < PITCH_LENGTH/3.0 &&
                i >= 9 )
            ;  // don't use attackers when in dead ball situation and not upfront
        else if ( posStrat.getDistanceTo( pos ) < dDist )
        {
            dDist   = posStrat.getDistanceTo( pos );
            iPlayer = i;
        }
    }
    return iPlayer;
}
