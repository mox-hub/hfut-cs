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

/*! \file SoccerTypes.cpp
<pre>
<b>File:</b>          SoccerTypes.cpp
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
               are defined here.  Finally it contains the Time class which
               holds a two-tuple that represents the time in the soccer server.

<hr size=2>
<h2><b>Changes</b></h2>
<b>Date</b>             <b>Author</b>          <b>Comment</b>
28/11/2000       Jelle Kok       Initial version created
</pre>
*/

#include <iostream>       // needed for outputsteam in showcerr
#include <stdio.h>        // needed for sprintf
#ifdef Solaris
#include <strings.h>    // needed for strncmp
#else
#include <string.h>     // needed for strncmp
#endif

#include "SoccerTypes.h"
#include "Parse.h"


/*****************************************************************************/
/********************* CLASS TIME ********************************************/
/*****************************************************************************/

/*! This is the constructor for the Time class, it receives two arguments. The
    actual time and how long the time has stopped.
    \param iTime actual time
    \param iStopped number of cycles time stopped */
Time::Time( int iTime, int iStopped )
{
    m_iTime    = iTime;
    m_iStopped = iStopped;
}

/*! This method updates the time to 'iTime'. When the actual time was already
    'iTime' the current time is kept unchanged and the time stopped is raised
    with one. Otherwise the actual time is changed to 'iTime' and the stopped
    time is set to 0.
    \param iTime new time
    \return boolean indicating whether update was successful */
bool Time::updateTime( int iTime )
{
    if ( m_iTime == iTime )
        m_iStopped++;
    else
    {
        m_iTime    = iTime;
        m_iStopped = 0;
    }
    return true;
}

/*! This methods sets the stopped time, which denotes the number of cycles time
    stood still.
    \param iTime new stopped time
    \return boolean indicating whether update was successful */
bool Time::setTimeStopped( int iTime )
{
    m_iStopped = iTime;
    return true;
}

/*! This method returns the actual time, that is the number of cycles that have
    passed.
    \return actual time. */
int Time::getTime( )
{
    return m_iTime;
}

/*! This method returns the time the time has stopped, that is the number of
    cycles the time stood on the current value.
    \return stopped time */
int Time::getTimeStopped( )
{
    return m_iStopped;
}

/*! This method returns the time difference between two time objects.
    \param t Time with which current time should be compared
    \return time difference */
int Time::getTimeDifference( Time t )
{
    if ( getTime() < t.getTime() )
        return getTime() - t.getTime() - t.getTimeStopped();
    else if ( getTime() == t.getTime() )
        return getTimeStopped() - t.getTimeStopped();
    else
        return getTime() - t.getTime();
}

/*! This method returns a boolean value indicating whether the time
    currently is stopped.

    \return boolean indicating whether the time currently is stopped */
bool Time::isStopped( )
{
    return m_iStopped != 0;
}

/*! This method returns a new time class denoting the time when
    'iCycles' are added to the current time.  There are different
    situations possible. When the added time is positive and the time
    stands still, the cycles are added to the stopped time, otherwise
    they are added to the actual time. When the added time is negative
    and the time stands still, the cycles are subtracted from the
    stopped time.  Otherwise the time is subtracted from the actual
    time.

    \param iCycles denotes the time that should be added (when negative
    subtracted) to the current time

    \return new time object with 'iCycles' added to the current time. 'iCycles'
    can be negative in which case a subtraction is performed. */
Time Time::getTimeAddedWith( int iCycles )
{
    int iTime    = getTime();
    int iStopped = getTimeStopped();

    if ( iCycles > 0 )                           // add cycles
    {
        if ( iStopped > 0 )                        // time stopped
            iStopped += iCycles;                     // add it to stopped cycles
        else
            iTime    += iCycles;                     // otherwise add to normal time
    }
    else                                         // subtract cycles
    {
        if ( iStopped > 0 && iStopped >= iCycles)  // time stopped and enough time
            iStopped += iCycles;                     // subtract cycle (iCycles=neg)
        else if ( iStopped > 0 )                   // stopped but not enough time
        {
            iStopped = 0;                            // take as many as possible
            iCycles += iStopped;
            iTime   += iCycles;                      // and take rest from m_iTime
        }
        else                                       // time not stopped
            iTime   += iCycles;                      // take all from m_iTime
        if ( iTime < 0 )
            iTime = 0;                               // negative time not possible
    }
    return Time( iTime, iStopped );
}

/*! This method adds 'iCycles' to the current time. The current values are
    updated. The method getTimeAddedWith is used to calculated the new time.
    \param iCycles time added to the current time
    \return boolean indicating whether update was successful */
bool Time::addToTime( int iCycles )
{
    *this = getTimeAddedWith( iCycles );
    return true;
}

/*! This method prints the time to the specified output stream. Time is printed
    as the two tuple (t,s) where t denotes the actual time and s the number of
    stopped cycles.
    \param os output stream to which output is written (default cout) */
void Time::show( ostream &os )
{
    os << "(" << getTime() << "," << getTimeStopped() << ")";
}

/*! This method returns the time as if 'i' cycles would be added to the current
    time. The method getTimeAddedWith is used for this. No changes are made to
    the current object.
    \param iCycles denotes the time that should be added to the current time
    \return new time object with 'iCycles' added to the current time */
Time Time::operator + ( const int &i )
{
    return getTimeAddedWith( i );
}

/*! This method returns the result of adding time 't' to the current
    time. No changes are made to the current object.
    It is defined by (t1,s1) + (t2,s2) = (t1+t2,s2). The stopped time of the
    first time tuple is neglected, since this has already been passed.
    \param t Time object that should be added to the current time
    \return new time object with 't' added to the current time */
Time Time::operator + ( Time t )
{
    return Time( getTime() + t.getTime(), t.getTimeStopped() );
}

/*! This method returns the time as if 'i' cycles would be subtracted from the
    current time. The method getTimeAddedWith is used for this. No changes are
    made to the current object.
    \param iCycles denotes the time that should be subtracted from the time
    \return new time object with 'iCycles' subracted  */
Time Time::operator - ( const int &i )
{
    return getTimeAddedWith( -i );
}

/*! This method returns the result the difference between the two times and
    is equal to the method getTimeDifference.
    \param t Time object that should be subtracted from the current time
    \return new time object with 't' subtracted from the current time */
int Time::operator - ( Time t )
{
    return getTimeDifference( t );
}

/*! This method returns a new time object (i,0). The argument 'i' is thus
    denoted as the actual time.
    \param i new time
    \return time object */
void Time::operator = ( const int &i )
{
    updateTime( i );
    setTimeStopped( 0 );
}

/*! This method updates the time by adding 'i' cycles to the current
    time. The method addToTime is used for this.
    \param i denotes the time that should be added to the current time */
void Time::operator += ( const int &i )
{
    addToTime( i );
}

/*! This method updates the time by adding the time 't' to the current
    time.
    It is defined by (t1,s1) + (t2,s2) = (t1+t2,s2). The stopped time of the
    first time tuple is neglected, since this has already been passed.
    \param t Time object that should be added to the current time */
void Time::operator += ( Time t )
{
    updateTime    ( getTime() + t.getTime() );
    setTimeStopped( t.getTimeStopped()      );
}

/*! This method updates the time by subtractign 'i' cycles from the
    current time. The method addToTime is used for this with '-i' as
    its argument.
    \param i denotes the time that should be subtracted */
void Time::operator -= ( const int &i )
{
    addToTime( -i );
}

/*! This method updates the time by subtracting time 't' from the current
    time. It is defined by (t1,s1) + (t2,s2) = (t1-t2,0). The stopped time is
    set to zero.
    \param t Time object that should be subtracted from the current time */
void Time::operator -= ( Time t )
{
    updateTime    ( getTime() - t.getTime() );
    setTimeStopped( 0                       );
}


/*! This method returns a boolean indicating whether the current time
    is inequal to the time specified by the integer 'i'. 'i' is first
    converted to the time object (i,0). When the time difference
    returned by getTimeDifference between these two time objects is
    inequal to zero true is returned, false otherwise

    \param i actual time with which current time should be compared
    \return bool indicating whether current time equals specified time */
bool Time::operator != ( const int &i )
{
    return getTimeDifference( Time(i, 0) ) != 0;
}

/*! This method returns a boolean indicating whether the current time
    is inequal to the time t. When the time difference returned by
    getTimeDifference between these two time objects is inequal to
    zero true is returned, false otherwise.

    \param t time with which current time should be compared
    \return bool indicating whether current time equals specified time */
bool Time::operator != ( Time t )
{
    return getTimeDifference( t ) != 0;
}

/*! This method returns a boolean indicating whether the current time
    equals the time as specified by the integer 'i'. 'i' is first
    converted to the time object (i,0). When the time difference
    returned by getTimeDifference between these two time objects
    equals zero, true is returned, false otherwise

    \param i actual time with which current time should be compared
    \return bool indicating whether current time equals specified time */
bool Time::operator == ( const int &i )
{
    return !( *this != i );
}

/*! This method returns a boolean indicating whether the current time equals
    the time t. When the time difference returned by getTimeDifference between
    these two time objects equals zero, true is returned, false otherwise
    \param t time with which current time should be compared
    \return bool indicating whether current time equals specified time */
bool Time::operator == ( Time t )
{
    return !( *this != t );
}

/*! This method returns a boolean indicating whether the current time
    is smaller than the time t. When the time difference returned by
    getTimeDifference is smaller than zero, true is returned, false
    otherwise

    \param t time with which current time should be compared
    \return bool indicating whether current time is smaller than given time */
bool Time::operator <  ( Time t )
{
    return getTimeDifference( t ) < 0;
}

/*! This method returns a boolean indicating whether the current time
    is smaller than the time denoted by the integer 'i'. Herefore
    first a time object (i,0) is created.

    \param t time with which current time should be compared
    \return bool indicating whether current time is smaller than given time */
bool Time::operator <  ( const int  &i )
{
    return Time( i, 0 ) >= *this ;
}

/*! This method returns a boolean indicating whether the current time
    is smaller than or equal to the time t.

    \param t time with which current time should be compared
    \return bool indicating whether current time is smaller to or equal
     than 't' */
bool Time::operator <= ( Time t )
{
    return ( *this < t ) || (*this == t );
}

/*! This method returns a boolean indicating whether the current time
    is smaller than or equal to the time denoted by the integer
    'i'. Herefore first a time object (i,0) is created.

    \param t time with which current time should be compared
    \return bool indicating whether current time is smaller than or equal to
    't' */
bool Time::operator <= ( const int  &i )
{
    return ( *this < i ) || (*this == i );
}

/*! This method returns a boolean indicating whether the current time is larger
    than the time t, that is it is not smaller than or equal to 't'.
    \param t time with which current time should be compared
    \return bool indicating whether current time is larger than 't' */
bool Time::operator >  ( Time t )
{
    return !( *this <= t );
}

/*! This method returns a boolean indicating whether the current time
    is larger than the time denoted by the integer 'i'. Herefore first
    a time object (i,0) is created.

    \param t time with which current time should be compared

    \return bool indicating whether current time is larger than the
    given time*/
bool Time::operator >  ( const int  &i )
{
    return !( *this <= i );
}

/*! This method returns a boolean indicating whether the current time is larger
    than or equal to than the time t, that is it is not smaller than 't'.
    \param t time with which current time should be compared
    \return bool indicating whether current time is larger or equal than 't' */
bool Time::operator >= ( Time t )
{
    return !( *this < t );

}

/*! This method returns a boolean indicating whether the current time is larger
    than or equal to the time denoted by the integer 'i'. Herefore first a time
    object (i,0) is created.
    \param t time with which current time should be compared
    \return bool indicating whether current time is larger than or equal to the
     given time*/
bool Time::operator >= ( const int  &i )
{
    return !( *this < i );
}

/*! Overloaded version of the C++ output operator for a Time class. This
    operator makes it possible to use Time objects in output statements (e.g.
    cout << t). The current cycle and the stopped time are printed in the
    format (t1,t2).
    \param os output stream to which information should be written
    \param v a Time object which must be printed
    \return output stream containing (x,y) */
ostream& operator <<( ostream &os, Time t )
{
    return os << "(" << t.getTime() << "," << t.getTimeStopped() << ")";
}

/*****************************************************************************/
/********************* CLASS SOCCERCOMMAND ***********************************/
/*****************************************************************************/


/*! This is a constructor for the SoccerCommand class. It creates a command
    using the passed arguments (with all default illegal values). Depending
    on the specified CommandT the parameters are used in different ways. See
    the method makeCommand for an explanation of these values.
    \param com commandType for this SoccerCommand
    \param d1 1st argument, meaning depends on com (default UnknownDoubleValue)
    \param d2 2nd argument, meaning depends on com (default UnknownDoubleValue)
    \param d3 3rd argument, meaning depends on com (default UnknownDoubleValue)
    \return SoccerCommand with the specified parameters.*/
SoccerCommand::SoccerCommand( CommandT com, double d1, double d2, double d3 )
{
    // first initialize variables
    commandType = com;
    dPower      = UnknownDoubleValue;
    dAngle      = UnknownDoubleValue;
    va          = VA_ILLEGAL;
    vq          = VQ_ILLEGAL;
    iTimes      = 1;
    strcpy( str, "\0" );
    if ( com == CMD_CHANGEVIEW )
        makeCommand( commandType, (ViewAngleT)d1, (ViewQualityT)d2 );
    else if ( com != CMD_SAY )
        makeCommand( commandType, d1, d2, d3 );
}

/*! This is a constructor for the SoccerCommand when the commandType is a say
    message.
    \param com commandType for this SoccerCommand (must be CMD_SAY).
    \param msg message for this SoccerCommand */
SoccerCommand::SoccerCommand( CommandT com, char *msg )
{
    makeCommand( com, msg )  ;
}

/*! This method create a SoccerCommand from the specified command type and the
    parameters. The parameters have a different meaning depending on the given
    command type. Not all command types are listed, since the other command
    types need different parameters. So see the other overloaded methods for
    that.
    - CMD_DASH:        d1 = power for dashing, d2 = angle for dash
    - CMD_TURN:        d1 = angle body is turned
    - CMD_TURNNECK     d1 = angle neck is turned
    - CMD_KICK         d1 = power for kick command, d2 = angle for kick
    - CMD_MOVE         d1 = x position, d2 = y position, d3 = body_angle
    - CMD_CATCH        d1 = catch angle
    - CMD_CHANGEPLAYER d1 = player number, d2 = nr of new player type
    - CMD_ATTENTIONTO  d1 = which team, d2 = player number
    - CMD_TACKLE       d1 = power of the tackle
    \param com command type specifying the kind of command
    \param d1 meaning depends on the specified command type (see above)
    \param d2 meaning depends on the specified command type (see above)
    \param d3 meaning depends on the specified command type (see above) */
void SoccerCommand::makeCommand( CommandT com, double d1,
                                 double d2, double d3 )
{
    // set the variables that have a meaning for the specified command type
    commandType = com;
    switch ( com )
    {
    case CMD_TURN:
    case CMD_TURNNECK:
    case CMD_CATCH:
        dAngle = d1;
        break;
    case CMD_DASH:
        dPower = d1;/*! misol.gao fixed for v13.2 */
        dAngle = d2;/*! misol.gao fixed for v13.2 */
        break;/*! misol.gao fixed for v13.2 */
    case CMD_TACKLE:
        dPower = d1;
        break;
    case CMD_KICK:
        dPower = d1;
        dAngle = d2;
        break;
    case CMD_MOVE:
        dX = d1;
        dY = d2;
        if ( d3 != UnknownDoubleValue )
            dAngle = d3;
        else
            dAngle = 0;
        break;
    case CMD_CHANGEPLAYER:
    case CMD_ATTENTIONTO:
    case CMD_POINTTO:
        dX = d1;
        dY = d2;
        break;
    default:
        break;
    }
}

/*! This method creates a SoccerCommand for the command type CMD_CHANGEVIEW.
    \param com command type specifying the kind of command
    \param v view angle for the change view command
    \param q view quality for the change view command */
void SoccerCommand::makeCommand( CommandT com, ViewAngleT v, ViewQualityT q )
{
    commandType = com;
    if ( com == CMD_CHANGEVIEW )
    {
        va = (ViewAngleT)  v;
        vq = (ViewQualityT)q;
    }
}

/*! This method creates a command for the command type CMD_SAY that accepts a
    string as parameter.
    \param com command type specifying the kind of command.
    \param msg string message that is added to the say message. */
void SoccerCommand::makeCommand( CommandT com, char* msg )
{
    commandType = com;
    if ( com == CMD_SAY )
        strcpy( str, msg );
}

/*! This method prints the current command to the specified output stream.
    \param os output stream to which information is printed. */
void SoccerCommand::show( ostream& os )
{
//  os << "time: " << time << " "; // can be illegal when not yet set in WM.

    switch ( commandType )
    {
    case CMD_ILLEGAL:
        os << "illegal\n" ;
        break;
    case CMD_DASH:
        os << commandType << " " << dPower << " " << dAngle << "\n";/*! misol.gao fixed for v13.2 */
        break;
    case CMD_TURN:
    case CMD_TURNNECK:
        os << commandType << " " << dAngle << "\n";
        break;
    case CMD_CATCH:
        os << "catch " << dAngle << "\n";
        break;
    case CMD_KICK:
        os << commandType << " " << dPower << " " << dAngle << "\n";
        break;
    case CMD_MOVE:
        os << commandType << " " << dX << " " << dY << " " << dAngle << "\n";
        break;
    case CMD_SENSEBODY:
        os << "sense_body" << "\n";
        break;
    case CMD_SAY:
        os << "say " << str << "\n";
        break;
    case CMD_CHANGEPLAYER:
        os << "change_player " << (int)dX << " " << (int)dY << "\n";
        break;
    case CMD_ATTENTIONTO:
        os << "attentionto " << (int)dX << " " << (int)dY << "\n";
        break;
    case CMD_TACKLE:
        os << "tackle " << (int)dPower << "\n";
        break;
    case CMD_POINTTO:
        os << "pointto " << dX << " " << dY << "\n";
        break;
    default:
        os << "unknown" << "\n";
        break;
    }
    return;

}

/*! This method returns whether this SoccerCommand is illegal, that is the
    SoccerCommand hasn't been filled yet. This means that no command would be
    performed when this command is sent to the server.
    \return bool indicating whether the current Command is illegal */
bool SoccerCommand::isIllegal( )
{
    return commandType == CMD_ILLEGAL;
}

/*! This method returns a command string that is understood by the
    server from a SoccerCommand. The resulting string is put in the
    second argument and returned by the method. A reference to
    ServerSettings is passed as the second argument to check whether
    the values in the SoccerCommand are legal.

    \param str resulting string (enough space for MAX_MSG should be allocated)
    \param ss reference to serversettings class.
    \return resulting boolean indicating whether error occurred or not */
bool SoccerCommand::getCommandString( char *str, ServerSettings *ss )
{
    SS = ss;
    bool b = true;
    switch ( commandType )
    {
    case CMD_DASH:
        b = makeDashCommand(         str );
        break;
    case CMD_TURN:
        b = makeTurnCommand(         str );
        break;
    case CMD_TURNNECK:
        b = makeTurnNeckCommand(     str );
        break;
    case CMD_CHANGEVIEW:
        b = makeChangeViewCommand(   str );
        break;
    case CMD_CATCH:
        b = makeCatchCommand(        str );
        break;
    case CMD_KICK:
        b = makeKickCommand(         str );
        break;
    case CMD_MOVE:
        b = makeMoveCommand(         str );
        break;
    case CMD_SENSEBODY:
        b = makeSenseBodyCommand(    str );
        break;
    case CMD_SAY:
        b = makeSayCommand(          str );
        break;
    case CMD_CHANGEPLAYER:
        b = makeChangePlayerCommand( str );
        break;
    case CMD_ATTENTIONTO:
        b = makeAttentionToCommand(  str );
        break;
    case CMD_TACKLE:
        b = makeTackleCommand(       str );
        break;
    case CMD_POINTTO:
        b = makePointToCommand(      str );
        break;
    case CMD_ILLEGAL:
        b = true;
        str[0] = '\0';
        break;
    default:
        b = false;
        cerr << "(ActHandler::makeCommandString) Unkown command!"<<"\n";
    }
    if ( b == false )                      // if string could not be created
        str[0] = '\0';                       // create the empty string

    return b;
}

/*! This method makes a catch command from a SoccerCommand and puts the result
    in str. Resulting string looks like: (catch dAngle). Enough space
    should be allocated for str.
    \param command SoccerCommand that is a catch command
    \param str string that will be filled with the corresponding SoccerCommand
    \return bool indicating whether string is filled or not  */
bool SoccerCommand::makeCatchCommand( char *str  )
{
    if ( SS->getMinMoment( ) <= dAngle && dAngle <= SS->getMaxMoment( ) )
        sprintf( str, "(catch %d)", (int)dAngle );
    else
    {
        fprintf(stderr,
                "(SoccerCommand::makeCatchCommand) angle %f out of bounds\n",dAngle);
        return false;
    }
    return true;
}

/*! This method makes a change view command from a SoccerCommand and puts the
    result in str. Resulting string looks like: (change_view va vq).
    Enough space should be allocated for str.
    \param command SoccerCommand that is a change view command
    \param str string that will be filled with the corresponding SoccerCommand
    \return bool indicating whether string is filled or not  */
bool SoccerCommand::makeChangeViewCommand( char *str  )
{
    if ( va != VA_ILLEGAL && vq != VQ_ILLEGAL )
        sprintf( str,"(change_view %s %s)", SoccerTypes::getViewAngleStr  ( va ),
                 SoccerTypes::getViewQualityStr( vq ) );
    else
    {
        fprintf( stderr,
                 "(SoccerCommand::makeChangeViewCommand) wrong arguments %s %s\n",
                 SoccerTypes::getViewAngleStr  ( va ),
                 SoccerTypes::getViewQualityStr( vq )  );
        return false;
    }
    return true;
}

/*! This method makes a dash command from a SoccerCommand and puts the result
    in str. Resulting string looks like: (dash dPower). Enough space
    should be allocated for str.
    \param command SoccerCommand that is a dash command
    \param str string that will be filled with the corresponding SoccerCommand
    \return bool indicating whether string is filled or not  */
bool SoccerCommand::makeDashCommand( char *str  )
{
    dAngle = VecPosition::normalizeAngle(dAngle);/*! misol.gao fixed for v13.2 */
    //cout << dAngle << endl;
    if ( SS->getMinPower() <= dPower && dPower <= SS->getMaxPower() )
        sprintf( str,"(dash %d %d)", (int)dPower, (int)dAngle );/*! misol.gao fixed for v13.2 */
    else
    {
        fprintf( stderr,
                 "(SoccerCommand::makeDashCommand) power %d out of bounds (%d,%d)\n",
                 (int)dPower, SS->getMinPower(), SS->getMaxPower() );
        dPower = 0.0;
        sprintf( str, "(dash 0 0)" );/*! misol.gao fixed for v13.2 */
        return false;
    }
    return true;
}

/*! This method makes a kick command from a SoccerCommand and puts the result
    in str. Resulting string looks like: (kick dPower dAngle).
    Enough space should be allocated for str.
    \param command SoccerCommand that is a kick command
    \param str string that will be filled with the corresponding SoccerCommand
    \return bool indicating whether string is filled or not  */
bool SoccerCommand::makeKickCommand( char *str  )
{
    if ( SS->getMinPower( )  <= dPower && dPower <= SS->getMaxPower( ) &&
            SS->getMinMoment( ) <= dAngle && dAngle <= SS->getMaxMoment( ) )
        sprintf( str,"(kick %d %d)", (int)dPower, (int)dAngle );
    else
    {
        fprintf(stderr,
                "(SoccerCommand::makeKickCommand) one argument %d or %d is wrong\n",
                (int)dPower, (int)dAngle );
        return false;
    }
    return true;
}

/*! This method makes a move command from a SoccerCommand and puts the result
    in str. Resulting string looks like: (move dX dY).
    Enough space should be allocated for str.
    \param command SoccerCommand that is a move command
    \param str string that will be filled with the corresponding SoccerCommand
    \return bool indicating whether string is filled or not  */
bool SoccerCommand::makeMoveCommand( char *str  )
{
    if ( -PITCH_LENGTH/2 - PITCH_MARGIN <= dX &&
            PITCH_LENGTH/2 + PITCH_MARGIN >= dX &&
            -PITCH_WIDTH/2  - PITCH_MARGIN <= dY &&
            PITCH_WIDTH/2  + PITCH_MARGIN >= dY   )
        sprintf( str,"(move %d %d)", (int)dX, (int)dY);
    else
    {
        fprintf( stderr,
                 "(SoccerCommand::makeMoveCommand) one argument %d or %d is wrong\n",
                 (int)dX, (int)dY) ;
        return false;
    }
    return true;
}

/*! This method makes a say command from a SoccerCommand and puts the result
    in str. Resulting string looks like: (say str). Enough space
    should be allocated for str.
    \param command SoccerCommand that is a say command
    \param str_com string that will be filled with corresponding SoccerCommand
    \return bool indicating whether string is filled or not  */
bool SoccerCommand::makeSayCommand( char *str_com  )
{
    if ( str != NULL && str[0] != '\0' )
        sprintf( str_com, "(say \"%s\")", str );
    else
    {
        fprintf( stderr, "(SoccerCommand::makeSayCommand) no string filled in\n" );
        return false;
    }
    return true;
}

/*! This method makes a sense_body command from a SoccerCommand and puts the
    result in str. Resulting string looks like: (sense_body). Enough space
    should be allocated for str.
    \param str string that will be filled with the corresponding SoccerCommand
    \return bool indicating whether string is filled or not  */
bool SoccerCommand::makeSenseBodyCommand( char *str  )
{
    sprintf( str,"(sense_body)" );
    return true;
}

/*! This method makes a turn command from a SoccerCommand and puts the result
    in str. Resulting string looks like: (turn dAngle). Enough space
    should be allocated for str.
    \param command SoccerCommand that is a turn command
    \param str string that will be filled with the corresponding SoccerCommand
    \return bool indicating whether string is filled or not  */
bool SoccerCommand::makeTurnCommand( char *str  )
{
    if ( SS->getMinMoment( ) <= dAngle && dAngle <= SS->getMaxMoment( ) )
        sprintf( str,"(turn %d)", (int)dAngle );
    else
    {
        fprintf( stderr,
                 "(SoccerCommand::makeTurnCommand) argument %d incorrect (%d, %d)\n",
                 (int)dAngle, SS->getMinMoment( ), SS->getMaxMoment( ) );
        dAngle = 0.0;
        sprintf( str, "(turn 0)" );
        return false;
    }
    return true;
}

/*! This method makes a turn_neck command from a SoccerCommand and puts the
    result in str. Resulting string looks like: (turn_neck dAngle).
    Enough space should be allocated for str.
    \param command SoccerCommand that is a turn_neck command
    \param str string that will be filled with the corresponding SoccerCommand
    \return bool indicating whether string is filled or not  */
bool SoccerCommand::makeTurnNeckCommand( char *str  )
{
    if ( SS->getMinNeckMoment( ) <= (int)dAngle &&
            (int)dAngle             <= SS->getMaxNeckMoment( ) )
        sprintf( str,"(turn_neck %d)", (int)dAngle );
    else
    {
        fprintf( stderr,
                 "(SoccerCommand::makeTurnNeckCommand) argument %d is wrong\n",
                 (int)dAngle );
        dAngle = 0.0;
        sprintf( str, "(turn_neck 0)" );
        return false;
    }
    return true;
}

/*! This method makes a change_player_type command from a SoccerCommand and
    puts the result in str. Resulting string looks like:
    (change_player_type dX dY). Where dX stands for the
    teammate that should be changed and dY for the heterogenous player that
    it should become.
    Enough space should be allocated for str.
    \param command SoccerCommand that is a turn_neck command
    \param str string that will be filled with the corresponding SoccerCommand
    \return bool indicating whether string is filled or not  */
bool SoccerCommand::makeChangePlayerCommand( char *str  )
{
    if ( (int)dX > 0  && (int)dX <= MAX_TEAMMATES &&
            (int)dY >= 0 && (int)dY < MAX_HETERO_PLAYERS  )
        sprintf( str,"(change_player_type %d %d)", (int)dX, (int)dY );
    else
    {
        fprintf( stderr,
                 "(SoccerCommand::makeChangePlayerCommand) argument %d or %d is wrong\n",
                 (int)dX, (int)dY  );
        return false;
    }
    return true;
}

/*! This method makes a attentionto command from a SoccerCommand and
    puts the result in str. Resulting string looks like:
    (attentionto opp|our dY). Where 'opp' is used when dX < 0 and
    'our' otherwise. dY stands for the player number of the team
    we want to pay attention to. When dY equals -1.0 the command
    (attentionto off) is created.
    Enough space should be allocated for str.
    \param command SoccerCommand that is a attentionto command
    \param str string that will be filled with the corresponding SoccerCommand
    \return bool indicating whether string is filled or not  */
bool SoccerCommand::makeAttentionToCommand( char *str  )
{
    char strTeam[10];
    if ( dY < 0 )
    {
        sprintf( str, "(attentionto off)" );
        return true;
    }

    if ( dX < 0 )
        strcpy( strTeam, "opp" );
    else
        strcpy( strTeam, "our" );

    if ( (int)dY > 0  && (int)dY <= MAX_TEAMMATES )
        sprintf( str,"(attentionto %s %d)", strTeam, (int)dY );
    else
    {
        fprintf( stderr,
                 "(SoccerCommand::makeAttentionToCommand) argument %s or %d is wrong\n",
                 strTeam, (int)dY  );
        return false;
    }
    return true;
}

/*! This method makes a tackle command from a SoccerCommand and puts the result
    in str. Resulting string looks like: (tackle dPower). Enough space
    should be allocated for str.
    \param command SoccerCommand that is a tackle command
    \param str string that will be filled with the corresponding SoccerCommand
    \return bool indicating whether string is filled or not  */
bool SoccerCommand::makeTackleCommand( char *str  )
{
    if ( SS->getMinPower() <= dPower && dPower <= SS->getMaxPower() )
        sprintf( str,"(tackle %d)", (int)dPower );
    else
    {
        fprintf( stderr,
                 "(SoccerCommand::makeTackleCommand) power %d out of bounds (%d,%d)\n",
                 (int)dPower, SS->getMinPower(), SS->getMaxPower() );
        return false;
    }
    return true;
}

/*! This method makes a pointto command from a SoccerCommand and
    puts the result in str. Resulting string looks like:
    (pointto dist dir | off)). When dX is smaller than  -1.0 the command
    (pointto off) is created.
    Enough space should be allocated for str.
    \param command SoccerCommand that is a pointto command
    \param str string that will be filled with the corresponding SoccerCommand
    \return bool indicating whether string is filled or not  */
bool SoccerCommand::makePointToCommand( char *str  )
{
    if ( dX < 0 )
    {
        sprintf( str, "(pointto off)" );
        return true;
    }

    if ( dY >= SS->getMinMoment() && dY <= SS->getMaxMoment()  )
        sprintf( str,"(pointto %1.2f %1.2f)", dX, dY );
    else
    {
        fprintf( stderr,
                 "(SoccerCommand::makePointToCommand) arg %f or %f is wrong\n", dX, dY  );
        return false;
    }
    return true;
}

/*****************************************************************************/
/********************* CLASS FEATURE *****************************************/
/*****************************************************************************/

/*! This is the constructor for the Feature class. A feature (specific
    information that applies to the current time can be stored here. */
Feature::Feature( )
{
    setFeature( Time(UnknownTime, 0), Time(UnknownTime, 0),Time(UnknownTime, 0),
                OBJECT_ILLEGAL, UnknownDoubleValue );
}

/*! This is the constructor for the Feature class. A feature (specific
    information that applies to the current time can be stored here.
    For example, when the information about the fastest player to the ball
    is calculated, this can be stored in a feauture. This information can
    then be used for another calculation in the same cycle.
    \param timeSee time of see message to which this feature applies.
    \param timeSense time of sense message to which this feature applies.
    \param object object to which this feature applies
    \param dInfo specific information that can be stored about this feature.
    \param soc soccercommand stored with this feature
    \param pos position information stored with this feature */
Feature::Feature( Time timeSee, Time timeSense, Time timeHear, ObjectT object,
                  double dInfo, SoccerCommand soc, VecPosition pos )
{
    setFeature( timeSee, timeSense, timeHear, object, dInfo, soc, pos );
}

/*! This methods sets the values of the Feature class. A feature (specific
    information that applies to the current time can be stored here.
    For example, when the information about the fastest player to the ball
    is calculated, this can be stored in a feauture. This information can
    then be used for another calculation in the same cycle.
    \param timeSee time of see message to which this feature applies.
    \param timeSense time of sense message to which this feature applies.
    \param timeSense time of hear message to which this feature applies.
    \param object object to which this feature applies
    \param dInfo specific information that can be stored about this feature.
    \param soc command stored with this feature
    \param pos position information stored with this feature
    \return boolean indicating whether update was successful. */
bool Feature::setFeature( Time timeSee,  Time timeSense, Time timeHear,
                          ObjectT object, double  dInfo, SoccerCommand soc,
                          VecPosition pos )
{
    bool b;
    b  = setTimeSee( timeSee );
    b &= setTimeSense( timeSense );
    b &= setTimeHear( timeHear );
    b &= setObject( object );
    b &= setInfo( dInfo );
    b &= setCommand( soc );
    b &= setVec( pos );
    return b;
}

/*! This method sets the see time for a feature.
    \param time see time that applies to this feature. */
bool Feature::setTimeSee( Time time )
{
    m_timeSee = time;
    return true;
}

/*! This method returns the see time for a feature.
    \return time that is related to this feature. */
Time Feature::getTimeSee( )
{
    return m_timeSee;
}

/*! This method sets the sense time for a feature.
    \param time sense time that applies to this feature. */
bool Feature::setTimeSense( Time time )
{
    m_timeSense = time;
    return true;
}

/*! This method returns the sense time for a feature.
    \return time that is related to this feature. */
Time Feature::getTimeSense( )
{
    return m_timeSense;
}

/*! This method sets the hear time for a feature.
    \param time hear time that applies to this feature. */
bool Feature::setTimeHear( Time time )
{
    m_timeHear = time;
    return true;
}

/*! This method returns the hear time for a feature.
    \return time that is related to this feature. */
Time Feature::getTimeHear( )
{
    return m_timeHear;
}

/*! This method sets the object for a feature.
    \param object object type that applies to this feature. */
bool Feature::setObject ( ObjectT object )
{
    m_object = object;
    return true;
}

/*! This method returns the object related to this feature.
    \return object stored with this feature. */
ObjectT Feature::getObject( )
{
    return m_object;
}

/*! This method sets the information for a feature.
    \param d double information that applies to this feature. */
bool Feature::setInfo( double d )
{
    m_dInfo = d;
    return true;
}

/*! This method returns the information for a feature.
    \return information stored with this feature. */
double Feature::getInfo( )
{
    return m_dInfo;
}

/*! This method sets the posiiton corresponding to this feature.
    \return boolean indicating whether update was succesfull. */
bool Feature::setVec( VecPosition   pos )
{
    m_vec = pos;
    return true;
}

/*! This method returns the position information for a feature.
    \return position information stored with this feature. */
VecPosition Feature::getVec( )
{
    return m_vec;
}

/*! This method sets the command corresponding to this feature.
    \return boolean indicating whether update was succesfull. */
bool Feature::setCommand( SoccerCommand soc )
{
    m_soc = soc;
    return true;
}

/*! This method returns the command corresponding to this feature.
    \return command stored with this feature. */
SoccerCommand Feature::getCommand( )
{
    return m_soc;
}

/*****************************************************************************/
/********************* CLASS SOCCERTYPES *************************************/
/*****************************************************************************/

/*! This constant defines the string names corresponding to the
    ObjectT names, defined in SoccerTypes.h. Players are not added
    since they depend on the team name. Note that the order is
    important, since the names are in the same order as the ObjectT
    enumeration. */
const char * ObjectNames[] =
{
    "(b)", "(g l)", "(g r)", "(g ?)", "(l l)", "(l r)", "(l b)", "(l t)",
    "(f l t)","(f t l 50)", "(f t l 40)", "(f t l 30)", "(f t l 20)",
    "(f t l 10)", "(f t 0)","(f c t)","(f t r 10)", "(f t r 20)", "(f t r 30)",
    "(f t r 40)","(f t r 50)", "(f r t)", "(f r t 30)", "(f r t 20)","(f r t 10)",
    "(f g r t)" , "(f r 0)", "(f g r b)" , "(f r b 10)", "(f r b 20)",
    "(f r b 30)", "(f r b)"   , "(f b r 50)", "(f b r 40)", "(f b r 30)",
    "(f b r 20)", "(f b r 10)", "(f c b)"   , "(f b 0)"   , "(f b l 10)",
    "(f b l 20)", "(f b l 30)", "(f b l 40)", "(f b l 50)", "(f l b)",
    "(f l b 30)", "(f l b 20)", "(f l b 10)", "(f g l b)" , "(f l 0)",
    "(f g l t)" , "(f l t 10)", "(f l t 20)", "(f l t 30)", "(f p l t)",
    "(f p l c)", "(f p l b)",   "(f p r t)",  "(f p r c)",  "(f p r b)", "(f c)"
};

/*! This method returns the string that corresponds to a specific object. This
    string name is exactly the same as the (short) name of the RoboCup
    Simulation.
    \param strBuf is the string in which the string representation is stored
    \param o ObjectT that has to be converted to the string representation
    \param strTeamName teamname that should be placed in case of player object
    \return pointer to strBuf, which contains the string representation */
char* SoccerTypes::getObjectStr( char* strBuf, ObjectT o,
                                 const char *strTeamName )
{
    if ( o >= OBJECT_BALL && o <=   OBJECT_FLAG_C )
        sprintf( strBuf, ObjectNames[(int)o] );
    else if ( isKnownPlayer( o ) && strTeamName != NULL )
        sprintf( strBuf, "(p %s %d)", strTeamName, getIndex( o ) + 1);
    else if ( isKnownPlayer( o ) && isTeammate( o ) )
        sprintf( strBuf, "(p l %d)", getIndex( o ) + 1);
    else if ( isKnownPlayer( o ) && isOpponent( o ) )
        sprintf( strBuf, "(p r %d)", getIndex( o ) + 1);
    else if ( o == OBJECT_OPPONENT_UNKNOWN || o == OBJECT_TEAMMATE_UNKNOWN )
        sprintf( strBuf, "(p %s)", strTeamName );
    else if ( o == OBJECT_PLAYER_UNKNOWN )
        sprintf( strBuf, "(p)" );
    else if ( o == OBJECT_UNKNOWN )
        sprintf( strBuf, "(unknown)" );
    else
        sprintf( strBuf, "illegal: %d", (int)o );
    return strBuf;
}

/*! This method returns an ObjectT that corresponds to the string
    passed as the first argument. The string representation equals the
    representation used in the Soccer Server. Format is with
    parenthesis, so possible arguments for str are (ball), (p Team_L
    1), etc.

    \param str pointer to string containing string representation of object
    \param isGoalie bool representing the fact whether object is a goalie
    \param strMyTeamName in case of player or opponent object, own teamname
           has to be matched, when it matches it is teammate otherwise opponent

    \return return the corresponding ObjectT, OBJECT_ILLEGAL in case
    of error */
ObjectT SoccerTypes::getObjectFromStr( char** str, bool *isGoalie,
                                       const char* strMyTeamName )
{
    ObjectT o = OBJECT_ILLEGAL;
    char* ptrStr = *str;
    *isGoalie = false;

    switch ( ptrStr[1] )
    {
    case 'b':                       // (ball)
    case 'B':                       // (B) in case of ball very close
        o = OBJECT_BALL;
        break;
    case 'G':
        o = OBJECT_GOAL_UNKNOWN;      // (G) in case of goal very close, ignored
        break;                        // (g l) or (g r) goal left or goal right
    case 'g':
        o = (ptrStr[3] == 'l') ? OBJECT_GOAL_L : OBJECT_GOAL_R;
        break;
    case 'l':                       // (l l), (l r), (l b) or (l t)
        switch ( ptrStr[3] )
        {
        case 'l':
            o = OBJECT_LINE_L;
            break;
        case 'r':
            o = OBJECT_LINE_R;
            break;
        case 'b':
            o = OBJECT_LINE_B;
            break;
        case 't':
            o = OBJECT_LINE_T;
            break;
        default:
            o = OBJECT_ILLEGAL;
            break;
        }
        break;
    case 'F':                       // (F) unkown flag very close.. ignored
        o = OBJECT_UNKNOWN;
        break;
    case 'f':                       // (f ...),  many options...
        switch ( ptrStr[3] )
        {
        case 'l':                   // (f l ... lines on left part of field
            if ( ptrStr[6] == ')' )   // only one character at index '5'
            {
                switch ( ptrStr[5] )
                {
                case '0':
                    o = OBJECT_FLAG_L_0;
                    break; // (f l 0)
                case 't':
                    o = OBJECT_FLAG_L_T;
                    break; // (f l t)
                case 'b':
                    o = OBJECT_FLAG_L_B;
                    break; // (f l b)
                default:
                    o = OBJECT_ILLEGAL;
                    break;
                }
            }
            else                      // more than one character from index '5'
            {
                switch ( ptrStr[7] )
                {
                case '1':             // (f l t 10) or (f l b 10)
                    o = (ptrStr[5]=='t')? OBJECT_FLAG_L_T_10 :OBJECT_FLAG_L_B_10;
                    break;
                case '2':             // (f l t 20) or (f l b 20)
                    o = (ptrStr[5]=='t')? OBJECT_FLAG_L_T_20 :OBJECT_FLAG_L_B_20;
                    break;
                case '3':             // (f l t 30) or (f l b 30)
                    o = (ptrStr[5]=='t')? OBJECT_FLAG_L_T_30 :OBJECT_FLAG_L_B_30;
                    break;
                default:
                    o = OBJECT_ILLEGAL;
                    break;
                }
            }
            break;
        case 'r':                   // (f r ... lines on right side of field
            if ( ptrStr[6] == ')' )   // only one character at index '5'
            {
                switch ( ptrStr[5] )
                {
                case '0':
                    o = OBJECT_FLAG_R_0;
                    break; // (f l 0)
                case 't':
                    o = OBJECT_FLAG_R_T;
                    break; // (f l t)
                case 'b':
                    o = OBJECT_FLAG_R_B;
                    break; // (f l b)
                }
            }
            else
            {
                switch ( ptrStr[7] )    // more than one character from index '5'
                {
                case '1':
                    o = (ptrStr[5]=='t')? OBJECT_FLAG_R_T_10 :OBJECT_FLAG_R_B_10;
                    break;
                case '2':             // (f r t 10) or (f r b 10)
                    o = (ptrStr[5]=='t')? OBJECT_FLAG_R_T_20 :OBJECT_FLAG_R_B_20;
                    break;              // (f r t 20) or (f r b 20)
                case '3':
                    o = (ptrStr[5]=='t')? OBJECT_FLAG_R_T_30 :OBJECT_FLAG_R_B_30;
                    break;              // (f r t 30) or (f r b 30)
                default:
                    o = OBJECT_ILLEGAL;
                    break;
                }
            }
            break;
        case 't':                   // lines on top part of field
            if ( ptrStr[5] == '0' )
                o = OBJECT_FLAG_T_0;    // (f t 0) center flag
            else
            {
                switch ( ptrStr[7] )    // rest of the top flags
                {
                case '1':             // (f t l 10) or (f t r 10)
                    o = (ptrStr[5]=='l') ? OBJECT_FLAG_T_L_10 : OBJECT_FLAG_T_R_10;
                    break;
                case '2':             // (f t l 20) or (f t r 20)
                    o = (ptrStr[5]=='l') ? OBJECT_FLAG_T_L_20 : OBJECT_FLAG_T_R_20;
                    break;
                case '3':             // (f t l 30) or (f t r 30)
                    o = (ptrStr[5]=='l') ? OBJECT_FLAG_T_L_30 : OBJECT_FLAG_T_R_30;
                    break;
                case '4':             // (f t l 40) or (f t r 40)
                    o = (ptrStr[5]=='l') ? OBJECT_FLAG_T_L_40 : OBJECT_FLAG_T_R_40;
                    break;
                case '5':             // (f t l 50) or (f t r 50)
                    o = (ptrStr[5]=='l') ? OBJECT_FLAG_T_L_50 : OBJECT_FLAG_T_R_50;
                    break;
                }
            }
            break;
        case 'b':                   // lines on bottom part of field
            if ( ptrStr[5] == '0')
                o = OBJECT_FLAG_B_0;    // (f b 0) center flag
            else
            {
                switch ( ptrStr[7] )    // rest of the bottom flags
                {
                case '1':             // (f b l 10) or (f b r 10)
                    o = (ptrStr[5]=='l') ? OBJECT_FLAG_B_L_10 : OBJECT_FLAG_B_R_10;
                    break;
                case '2':             // (f b l 20) or (f b r 20)
                    o = (ptrStr[5]=='l') ? OBJECT_FLAG_B_L_20 : OBJECT_FLAG_B_R_20;
                    break;
                case '3':             // (f b l 30) or (f b r 30)
                    o = (ptrStr[5]=='l') ? OBJECT_FLAG_B_L_30 : OBJECT_FLAG_B_R_30;
                    break;
                case '4':             // (f b l 40) or (f b r 40)
                    o = (ptrStr[5]=='l') ? OBJECT_FLAG_B_L_40 : OBJECT_FLAG_B_R_40;
                    break;
                case '5':             // (f b l 50) or (f b r 50)
                    o = (ptrStr[5]=='l') ? OBJECT_FLAG_B_L_50 : OBJECT_FLAG_B_R_50;
                    break;
                }
            }
            break;
        case 'c':                   // center flags
            if ( ptrStr[4] == ')' )
                o = OBJECT_FLAG_C;      // (f c) flag in midpoint field
            else                      // (f c t) or (f c b)
                o = (ptrStr[5] == 't') ? OBJECT_FLAG_C_T : OBJECT_FLAG_C_B;
            break;
        case 'g':                   // goal flags
            if ( ptrStr[5] == 'l' )   // (g l t) or (g l b)
                o = (ptrStr[7] == 't') ? OBJECT_FLAG_G_L_T : OBJECT_FLAG_G_L_B;
            else                      // (g r t) or (g r b)
                o = (ptrStr[7] == 't') ? OBJECT_FLAG_G_R_T : OBJECT_FLAG_G_R_B;
            break;
        case 'p':                   // flags at sides penalty area
            switch ( ptrStr[7] )
            {
            case 't':               // (p l t) or (p r t) top penalty area
                o = (ptrStr[5] == 'l') ? OBJECT_FLAG_P_L_T : OBJECT_FLAG_P_R_T;
                break;
            case 'c':               // (p l c) or (p r c) center penalty area
                o = (ptrStr[5] == 'l') ? OBJECT_FLAG_P_L_C : OBJECT_FLAG_P_R_C;
                break;
            case 'b':               // (p l b) or (p r b) bottom penalty area
                o = (ptrStr[5] == 'l') ? OBJECT_FLAG_P_L_B : OBJECT_FLAG_P_R_B;
                break;
            default:
                o = OBJECT_ILLEGAL;
                break;
            }
            break;
        default:
            o = OBJECT_ILLEGAL;
        }
        break; // end flags (finally)
    case 'p': // (p team nr) or (p team) or (p) player teammate or opponent
    case 'P': // or (P)
        ptrStr += 2;

        if ( Parse::gotoFirstSpaceOrClosingBracket(&ptrStr) == ')' )
            o = OBJECT_PLAYER_UNKNOWN; // if (p) or (P) player is unknown.
        // check also with quotes since later versions use string around "teamname"
        else if ( strncmp( ptrStr+1, strMyTeamName, strlen( strMyTeamName )) == 0 ||
                  strncmp( ptrStr+2, strMyTeamName, strlen( strMyTeamName )) == 0 )
        {
            ptrStr++;
            if ( Parse::gotoFirstSpaceOrClosingBracket(&ptrStr) == ' ' )
            {                                               // also team number
                switch ( Parse::parseFirstInt( &ptrStr ) )    // get team number
                {
                case 1:
                    o = OBJECT_TEAMMATE_1;
                    break;     // and find team member
                case 2:
                    o = OBJECT_TEAMMATE_2;
                    break;
                case 3:
                    o = OBJECT_TEAMMATE_3;
                    break;
                case 4:
                    o = OBJECT_TEAMMATE_4;
                    break;
                case 5:
                    o = OBJECT_TEAMMATE_5;
                    break;
                case 6:
                    o = OBJECT_TEAMMATE_6;
                    break;
                case 7:
                    o = OBJECT_TEAMMATE_7;
                    break;
                case 8:
                    o = OBJECT_TEAMMATE_8;
                    break;
                case 9:
                    o = OBJECT_TEAMMATE_9;
                    break;
                case 10:
                    o = OBJECT_TEAMMATE_10;
                    break;
                case 11:
                    o = OBJECT_TEAMMATE_11;
                    break;
                default:
                    o = OBJECT_ILLEGAL;
                }
                if ( ptrStr[0] != ')' && ptrStr[1] == 'g' ) // goalie
                    *isGoalie = true;
            }
            else
                o = OBJECT_TEAMMATE_UNKNOWN;                  // (p team) but no nr
        }
        else                                              // not a teammate
        {
            ptrStr++;
            if ( Parse::gotoFirstSpaceOrClosingBracket( &ptrStr ) == ' ' )
            {                                               // also team number
                switch ( Parse::parseFirstInt( &ptrStr ) )    // get team numer
                {
                case 1:
                    o = OBJECT_OPPONENT_1;
                    break;     // and return opponent
                case 2:
                    o = OBJECT_OPPONENT_2;
                    break;
                case 3:
                    o = OBJECT_OPPONENT_3;
                    break;
                case 4:
                    o = OBJECT_OPPONENT_4;
                    break;
                case 5:
                    o = OBJECT_OPPONENT_5;
                    break;
                case 6:
                    o = OBJECT_OPPONENT_6;
                    break;
                case 7:
                    o = OBJECT_OPPONENT_7;
                    break;
                case 8:
                    o = OBJECT_OPPONENT_8;
                    break;
                case 9:
                    o = OBJECT_OPPONENT_9;
                    break;
                case 10:
                    o = OBJECT_OPPONENT_10;
                    break;
                case 11:
                    o = OBJECT_OPPONENT_11;
                    break;
                default:
                    o = OBJECT_ILLEGAL;
                }
                if ( ptrStr[0] != ')' && ptrStr[1] == 'g' ) // goalie
                    *isGoalie = true;
            }
            else
                o = OBJECT_OPPONENT_UNKNOWN;                  // number not known

        }
        break;
    default:
        cerr << "(SoccerTypes::getObjectFromStr) Unknown msg: " <<  ptrStr << "\n";
        o = OBJECT_ILLEGAL;
        break;
    }
    // go to the end of the object
    Parse::gotoFirstOccurenceOf( ')', &ptrStr );
    *str=ptrStr;
    return o;
}

/*! This method returns the index of an object relative to the first object
    in that set.
    The index is always 1 smaller than its number, so OBJECT_OPPONENT_1
    will become 0. This can be used for indexing an array of objects.
    \param o ObjectT type of object of which the index has to be calculated
    \return index of object or -1 when o was not a correct object */
int SoccerTypes::getIndex( ObjectT o )
{
    if ( o >= OBJECT_OPPONENT_1 && o <= OBJECT_OPPONENT_11 )
        return o - OBJECT_OPPONENT_1;
    else if ( o >= OBJECT_TEAMMATE_1 && o <= OBJECT_TEAMMATE_11 )
        return o - OBJECT_TEAMMATE_1;
    else if ( o >= OBJECT_GOAL_L && o <= OBJECT_GOAL_R )
        return o - OBJECT_GOAL_L;
    else if ( o >= OBJECT_FLAG_L_T && o <= OBJECT_FLAG_C )
        return o - OBJECT_FLAG_L_T + 2; // 2 added for the two goals
    else if ( o >= OBJECT_LINE_L && o <= OBJECT_LINE_T )
        return o - OBJECT_LINE_L;
    else
        return -1;
}

/*! This method returns the object type of a teammate with index iIndex. When
    iIndex equals 3 for example OBJECT_TEAMMATE_4 is returned.
    \param iIndex index of teammate range is [0..10]
    \return object type corresponding to this index */
ObjectT SoccerTypes::getTeammateObjectFromIndex( int iIndex )
{
    return (ObjectT) ( OBJECT_TEAMMATE_1 + iIndex );
}

/*! This method returns the object type of an opponent with index iIndex. When
    iIndex equals 9 for example OBJECT_OPPONENT_10 is returned.
    \param iIndex index of opponent range is [0..10]
    \return object type corresponding to this index */
ObjectT SoccerTypes::getOpponentObjectFromIndex( int iIndex )
{
    return (ObjectT) ( OBJECT_OPPONENT_1 + iIndex );
}

/*! This method returns a boolean indicating whether the object o is
    part of the object set o_s. OBJECT_TEAMMATE_1 as o and
    OBJECT_SET_TEAMMATES will return for example the value true.

    \param o ObjectT of which should be checked whether it is a part of o_s
    \param o_s ObjectSet in which o should be
    \return true when o is included in set o_s, false otherwise */
bool SoccerTypes::isInSet( ObjectT o, ObjectSetT o_g, ObjectT objGoalie )
{
    switch ( o_g )
    {
    case OBJECT_SET_TEAMMATES:
        return isTeammate( o ) && isKnownPlayer( o );
    case OBJECT_SET_OPPONENTS:
        return isOpponent( o ) && isKnownPlayer( o );
    case OBJECT_SET_PLAYERS:
        return isPlayer  ( o ) && isKnownPlayer( o );
    case OBJECT_SET_FLAGS:
        return isFlag    ( o );
    case OBJECT_SET_LINES:
        return isLine    ( o );
    case OBJECT_SET_TEAMMATES_NO_GOALIE:
        return isTeammate( o ) && isKnownPlayer( o ) &&
               o != objGoalie;
    case OBJECT_SET_ILLEGAL:
    default:
        break;
    }
    return false;
}


/*! This method returns a boolean value which indicates whether a given player
    type belongs to a specific set. The set PS_DEFENDERS for example contains
    both the defender sweeper and the wing defenders. */
bool SoccerTypes::isPlayerTypeInSet( PlayerT p, PlayerSetT p_s )
{
    switch ( p_s )
    {
    case PS_DEFENDERS:
        return p == PT_DEFENDER_CENTRAL || p == PT_DEFENDER_WING;
    case PS_MIDFIELDERS:
        return p == PT_MIDFIELDER_CENTER || p ==  PT_MIDFIELDER_WING;
    case PS_ATTACKERS:
        return p == PT_ATTACKER_WING || p == PT_ATTACKER;
    case PS_ALL:
        return true;
    default:
        break;
    }
    return false;

}


/*! This method determines whether object o is a flag.
    \param o an object type
    \return bool indicating whether o is a flag (return true) or not (false) */
bool SoccerTypes::isFlag( ObjectT o )
{
    return ( o >= OBJECT_FLAG_L_T && o <= OBJECT_FLAG_C ) ||
           ( o >= OBJECT_GOAL_L   && o <= OBJECT_GOAL_R        );
}

/*! This method determines whether object o is a line.
     \param o an object type
     \return bool indicating whether o is a line return true) or not (false) */
bool SoccerTypes::isLine( ObjectT o )
{
    return o >= OBJECT_LINE_L && o <= OBJECT_LINE_T;
}

/*! This method determines whether object o is a goal
    \param o an object type
    \return bool indicating whether o is a goal (return true) or not (false) */
bool SoccerTypes::isGoal( ObjectT o )
{
    return o == OBJECT_GOAL_L || o == OBJECT_GOAL_R;
}

/*! This method determines whether object o is a teammate
    \param o an object type
    \return bool indicating whether o is a teammate (true) or not (false) */
bool SoccerTypes::isTeammate( ObjectT o )
{
    return o >= OBJECT_TEAMMATE_1 && o <= OBJECT_TEAMMATE_UNKNOWN;
}

/*! This method determines whether object o is an opponent
    \param o an object type
    \return bool indicating whether o is an opponent (true) or not (false) */
bool SoccerTypes::isOpponent( ObjectT o )
{
    return o >= OBJECT_OPPONENT_1 && o <= OBJECT_OPPONENT_UNKNOWN;
}

/*! This method determines whether object o is a player without checking
    whether its number or side is available.
    \param o an object type

    \return bool indicating whether o is a known player (true) or not
    (false) */
bool SoccerTypes::isPlayer( ObjectT o )
{
    return isKnownPlayer( o )           || o == OBJECT_TEAMMATE_UNKNOWN ||
           o == OBJECT_OPPONENT_UNKNOWN || o == OBJECT_PLAYER_UNKNOWN;
}

/*! This method determines whether object o is a known player, thus
    containing a number
    \param o an object type

    \return bool indicating whether o is a known player (true) or not
    (false) */
bool SoccerTypes::isKnownPlayer( ObjectT o )
{
    return (o >= OBJECT_OPPONENT_1 && o <= OBJECT_OPPONENT_11) ||
           (o >= OBJECT_TEAMMATE_1 && o <= OBJECT_TEAMMATE_11);
}

/*! This method determines whether object o is a goalie = teammate number is 1
    (for now)
    \param o an object type
    \return bool indicating whether o is a goalie (true) or not (false) */
bool SoccerTypes::isGoalie( ObjectT o )
{
    return o == OBJECT_TEAMMATE_1 || o == OBJECT_OPPONENT_1;
}

/*! This method determines whether object o is the ball
    \param o an object type
    \return bool indicating whether o is the ball (true) or not (false) */
bool SoccerTypes::isBall( ObjectT o )
{
    return o == OBJECT_BALL;
}

/*! This method returns the object representing the own goal
    \param s own side
    \return object of the own goal, OBJECT_ILLEGAL when s is SIDE_ILLEGAL*/
ObjectT SoccerTypes::getOwnGoal( SideT s )
{
    if ( SIDE_LEFT == s )
        return OBJECT_GOAL_L;
    else if ( SIDE_RIGHT == s )
        return OBJECT_GOAL_R;

    cerr << "(SoccerTypes::isOwnGoal) Wrong side argument" << "\n";
    return OBJECT_ILLEGAL;
}

/*! This method returns the object representing the opponent goal
    \param s own side

    \return object of the goal opponent, OBJECT_ILLEGAL when s is
    SIDE_ILLEGAL*/
ObjectT SoccerTypes::getGoalOpponent( SideT s )
{
    if ( SIDE_LEFT == s )
        return OBJECT_GOAL_R;
    else if ( SIDE_RIGHT == s )
        return OBJECT_GOAL_L;

    cerr << "(SoccerTypes::isGoalOpponent) Wrong side argument" << "\n";
    return OBJECT_ILLEGAL;
}

/*! This method returns the global position on the field of a flag (a goal
    is also a flag). Since the global positions for both teams differ, the
    side of the agent team is also needed. Note that the global
    positions of the flags will not change in the second half.
    \param o flag of which global position should be determined
    \param s side of your team.
    \param dGoalWidth for some flags the goalWidth is necessary (default 14.02)
    \return VecPosition representing the global position. x and y value are
            both UnknownDoubleValue when o is not a flag or goal. */
VecPosition SoccerTypes::getGlobalPositionFlag( ObjectT o, SideT s,
        double dGoalWidth )
{
    VecPosition pos;
    if ( !(isFlag(o) || isGoal(o)) )
        return VecPosition(UnknownDoubleValue, UnknownDoubleValue);
    switch ( o ) // for every object the global position is entered
    {
    case OBJECT_GOAL_L:
        pos.setVecPosition( -PITCH_LENGTH/2.0, 0.0 );
        break;
    case OBJECT_GOAL_R:
        pos.setVecPosition(  PITCH_LENGTH/2.0, 0.0 );
        break;
    case OBJECT_FLAG_L_T:
        pos.setVecPosition( -PITCH_LENGTH/2.0, -PITCH_WIDTH/2.0 );
        break;
    case OBJECT_FLAG_T_L_50:
        pos.setVecPosition( -50.0, -PITCH_WIDTH/2.0 - PITCH_MARGIN );
        break;
    case OBJECT_FLAG_T_L_40:
        pos.setVecPosition( -40.0, -PITCH_WIDTH/2.0 - PITCH_MARGIN );
        break;
    case OBJECT_FLAG_T_L_30:
        pos.setVecPosition( -30.0, -PITCH_WIDTH/2.0 - PITCH_MARGIN );
        break;
    case OBJECT_FLAG_T_L_20:
        pos.setVecPosition( -20.0, -PITCH_WIDTH/2.0 - PITCH_MARGIN );
        break;
    case OBJECT_FLAG_T_L_10:
        pos.setVecPosition( -10.0, -PITCH_WIDTH/2.0 - PITCH_MARGIN );
        break;
    case OBJECT_FLAG_T_0:
        pos.setVecPosition(   0.0, -PITCH_WIDTH/2.0 - PITCH_MARGIN );
        break;
    case OBJECT_FLAG_C_T:
        pos.setVecPosition(   0.0, -PITCH_WIDTH/2.0);
        break;
    case OBJECT_FLAG_T_R_10:
        pos.setVecPosition(  10.0,  -PITCH_WIDTH/2.0 - PITCH_MARGIN);
        break;
    case OBJECT_FLAG_T_R_20:
        pos.setVecPosition(  20.0,  -PITCH_WIDTH/2.0 - PITCH_MARGIN);
        break;
    case OBJECT_FLAG_T_R_30:
        pos.setVecPosition(  30.0,  -PITCH_WIDTH/2.0 - PITCH_MARGIN);
        break;
    case OBJECT_FLAG_T_R_40:
        pos.setVecPosition(  40.0,  -PITCH_WIDTH/2.0 - PITCH_MARGIN);
        break;
    case OBJECT_FLAG_T_R_50:
        pos.setVecPosition(  50.0,  -PITCH_WIDTH/2.0 - PITCH_MARGIN);
        break;
    case OBJECT_FLAG_R_T:
        pos.setVecPosition( PITCH_LENGTH/2.0,  -PITCH_WIDTH/2.0 );
        break;
    case OBJECT_FLAG_R_T_30:
        pos.setVecPosition( PITCH_LENGTH/2.0 + PITCH_MARGIN,  -30.0 );
        break;
    case OBJECT_FLAG_R_T_20:
        pos.setVecPosition( PITCH_LENGTH/2.0 + PITCH_MARGIN,  -20.0 );
        break;
    case OBJECT_FLAG_R_T_10:
        pos.setVecPosition( PITCH_LENGTH/2.0 + PITCH_MARGIN,  -10.0 );
        break;
    case OBJECT_FLAG_G_R_T:
        pos.setVecPosition( PITCH_LENGTH/2.0,  -dGoalWidth/2.0 );
        break;
    case OBJECT_FLAG_R_0:
        pos.setVecPosition( PITCH_LENGTH/2.0 + PITCH_MARGIN,  0.0 );
        break;
    case OBJECT_FLAG_G_R_B:
        pos.setVecPosition( PITCH_LENGTH/2.0,  dGoalWidth/2.0 );
        break;
    case OBJECT_FLAG_R_B_10:
        pos.setVecPosition( PITCH_LENGTH/2.0 + PITCH_MARGIN,  10.0 );
        break;
    case OBJECT_FLAG_R_B_20:
        pos.setVecPosition( PITCH_LENGTH/2.0 + PITCH_MARGIN,  20.0 );
        break;
    case OBJECT_FLAG_R_B_30:
        pos.setVecPosition( PITCH_LENGTH/2.0 + PITCH_MARGIN,  30.0 );
        break;
    case OBJECT_FLAG_R_B:
        pos.setVecPosition( PITCH_LENGTH/2.0,  PITCH_WIDTH/2.0 );
        break;
    case OBJECT_FLAG_B_R_50:
        pos.setVecPosition(  50.0, PITCH_WIDTH/2.0 + PITCH_MARGIN );
        break;
    case OBJECT_FLAG_B_R_40:
        pos.setVecPosition(  40.0, PITCH_WIDTH/2.0 + PITCH_MARGIN );
        break;
    case OBJECT_FLAG_B_R_30:
        pos.setVecPosition(  30.0, PITCH_WIDTH/2.0 + PITCH_MARGIN );
        break;
    case OBJECT_FLAG_B_R_20:
        pos.setVecPosition(  20.0, PITCH_WIDTH/2.0 + PITCH_MARGIN );
        break;
    case OBJECT_FLAG_B_R_10:
        pos.setVecPosition(  10.0, PITCH_WIDTH/2.0 + PITCH_MARGIN );
        break;
    case OBJECT_FLAG_C_B:
        pos.setVecPosition(   0.0, PITCH_WIDTH/2.0 );
        break;
    case OBJECT_FLAG_B_0:
        pos.setVecPosition(   0.0, PITCH_WIDTH/2.0 + PITCH_MARGIN );
        break;
    case OBJECT_FLAG_B_L_10:
        pos.setVecPosition( -10.0, PITCH_WIDTH/2.0 + PITCH_MARGIN );
        break;
    case OBJECT_FLAG_B_L_20:
        pos.setVecPosition( -20.0, PITCH_WIDTH/2.0 + PITCH_MARGIN );
        break;
    case OBJECT_FLAG_B_L_30:
        pos.setVecPosition( -30.0, PITCH_WIDTH/2.0 + PITCH_MARGIN );
        break;
    case OBJECT_FLAG_B_L_40:
        pos.setVecPosition( -40.0, PITCH_WIDTH/2.0 + PITCH_MARGIN );
        break;
    case OBJECT_FLAG_B_L_50:
        pos.setVecPosition( -50.0, PITCH_WIDTH/2.0 + PITCH_MARGIN );
        break;
    case OBJECT_FLAG_L_B:
        pos.setVecPosition( -PITCH_LENGTH/2.0,  PITCH_WIDTH/2.0 );
        break;
    case OBJECT_FLAG_L_B_30:
        pos.setVecPosition( -PITCH_LENGTH/2.0 - PITCH_MARGIN,  30.0 );
        break;
    case OBJECT_FLAG_L_B_20:
        pos.setVecPosition( -PITCH_LENGTH/2.0 - PITCH_MARGIN,  20.0 );
        break;
    case OBJECT_FLAG_L_B_10:
        pos.setVecPosition( -PITCH_LENGTH/2.0 - PITCH_MARGIN,  10.0 );
        break;
    case OBJECT_FLAG_G_L_B:
        pos.setVecPosition( -PITCH_LENGTH/2.0,  dGoalWidth/2.0 );
        break;
    case OBJECT_FLAG_L_0:
        pos.setVecPosition( -PITCH_LENGTH/2.0 - PITCH_MARGIN,  0.0 );
        break;
    case OBJECT_FLAG_G_L_T:
        pos.setVecPosition( -PITCH_LENGTH/2.0,  -dGoalWidth/2.0 );
        break;
    case OBJECT_FLAG_L_T_10:
        pos.setVecPosition( -PITCH_LENGTH/2.0 - PITCH_MARGIN, -10.0 );
        break;
    case OBJECT_FLAG_L_T_20:
        pos.setVecPosition( -PITCH_LENGTH/2.0 - PITCH_MARGIN, -20.0 );
        break;
    case OBJECT_FLAG_L_T_30:
        pos.setVecPosition( -PITCH_LENGTH/2.0 - PITCH_MARGIN, -30.0 );
        break;
    case OBJECT_FLAG_P_L_T:
        pos.setVecPosition( -PITCH_LENGTH/2.0 + PENALTY_AREA_LENGTH,
                            - PENALTY_AREA_WIDTH/2.0 );
        break;
    case OBJECT_FLAG_P_L_C:
        pos.setVecPosition( -PITCH_LENGTH/2.0 + PENALTY_AREA_LENGTH, 0.0 );
        break;
    case OBJECT_FLAG_P_L_B:
        pos.setVecPosition( -PITCH_LENGTH/2.0 + PENALTY_AREA_LENGTH,
                            PENALTY_AREA_WIDTH/2.0 );
        break;
    case OBJECT_FLAG_P_R_T:
        pos.setVecPosition(  PITCH_LENGTH/2.0 - PENALTY_AREA_LENGTH,
                             -PENALTY_AREA_WIDTH/2.0 );
        break;
    case OBJECT_FLAG_P_R_C:
        pos.setVecPosition(  PITCH_LENGTH/2.0 - PENALTY_AREA_LENGTH, 0.0 );
        break;
    case OBJECT_FLAG_P_R_B:
        pos.setVecPosition(  PITCH_LENGTH/2.0 - PENALTY_AREA_LENGTH,
                             PENALTY_AREA_WIDTH/2.0 );
        break;
    case OBJECT_FLAG_C:
        pos.setVecPosition(  0.0 , 0.0 );
        break;
    default:
        cerr << "(SoccerTypes::getGlobalPositionObject) wrong objecttype! " <<
             (int)o << "\n" ;
    }

    if ( s == SIDE_RIGHT ) // change side for team on the right side.
        pos.setVecPosition( -pos.getX(), -pos.getY() );
    return pos;
}

/*! This method returns the global angle of a lines on the field.  The
    global angle differs for the left and right side. For both teams
    the line behind the opponent goal is seen with global angle
    0. Only for the left team this is the right line and for the right
    team this is the left line.

    \param o one of the four line objects
    \param s side on which the team was started
    \return AngDeg global angle of this line. */
AngDeg SoccerTypes::getGlobalAngleLine( ObjectT o , SideT s )
{
    AngDeg angle;
    switch ( o )
    {
    case OBJECT_LINE_L:
        angle = 180.0;
        break;
    case OBJECT_LINE_R:
        angle =   0.0;
        break;
    case OBJECT_LINE_T:
        angle = -90.0;
        break;
    case OBJECT_LINE_B:
        angle =  90.0;
        break;
    default:
        cerr << "(SoccerTypes::getGlobalAngleLine) wrong objecttype! " <<
             (int)o << "\n";
        return UnknownAngleValue;
    }

    if ( s == SIDE_RIGHT )
        angle += 180;

    return VecPosition::normalizeAngle( angle );
}

/*! This method returns the string representation of a PlayModeT as is used
    in the Robocup Soccer Simulation and also said by the referee.
    \param pm PlayModeT which should be converted
    \return  pointer to the string (enough memory has to be allocated) */
char* SoccerTypes::getPlayModeStr( PlayModeT pm )
{
    switch ( pm )
    {
    case PM_BEFORE_KICK_OFF:
        return "before_kick_off";
    case PM_KICK_OFF_LEFT:
        return "kick_off_l";
    case PM_KICK_OFF_RIGHT:
        return "kick_off_r";
    case PM_KICK_IN_LEFT:
        return "kick_in_l";
    case PM_KICK_IN_RIGHT:
        return "kick_in_r";
    case PM_CORNER_KICK_LEFT:
        return "corner_kick_l";
    case PM_CORNER_KICK_RIGHT:
        return "corner_kick_r";
    case PM_GOAL_KICK_LEFT:
        return "goal_kick_l";
    case PM_GOAL_KICK_RIGHT:
        return "goal_kick_r";
    case PM_GOAL_LEFT:
        return "goal_r";
    case PM_GOAL_RIGHT:
        return "goal_l";
    case PM_FREE_KICK_FAULT_LEFT:
        return "free_kick_fault_l";
    case PM_FREE_KICK_FAULT_RIGHT:
        return "free_kick_fault_r";
    case PM_FREE_KICK_LEFT:
        return "free_kick_l";
    case PM_FREE_KICK_RIGHT:
        return "free_kick_r";
    case PM_INDIRECT_FREE_KICK_RIGHT:
        return "indirect_free_kick_r";
    case PM_INDIRECT_FREE_KICK_LEFT:
        return "indirect_free_kick_l";
    case PM_BACK_PASS_LEFT:
        return "back_pass_l";
    case PM_BACK_PASS_RIGHT:
        return "back_pass_r";
    case PM_OFFSIDE_LEFT:
        return "offside_l";
    case PM_OFFSIDE_RIGHT:
        return "offside_l";
    case PM_PENALTY_SETUP_LEFT:
        return "penalty_setup_left";
    case PM_PENALTY_SETUP_RIGHT:
        return "penalty_setup_right";
    case PM_PENALTY_READY_LEFT:
        return "penalty_ready_left";
    case PM_PENALTY_READY_RIGHT:
        return "penalty_ready_right";
    case PM_PENALTY_TAKEN_LEFT:
        return "penalty_taken_left";
    case PM_PENALTY_TAKEN_RIGHT:
        return "penalty_taken_right";
    case PM_PLAY_ON:
        return "play_on";
    case PM_FROZEN:
        return "play_off";
    case PM_QUIT:
        return "quit";
    case PM_ILLEGAL:
    default:
        return NULL;
    }
}

/*! This method returns the play mode associated with a string.
   \param str representing the play mode
   \return PlayModeT of string, PM_ILLEGAL if it is not recognized */
PlayModeT SoccerTypes::getPlayModeFromStr( char* str )
{
    // all play modes are sent as referee message, so get referee message
    // and look up the associated play mode
    return getPlayModeFromRefereeMessage( getRefereeMessageFromStr( str ) );
}

/*! This method returns the play mode from the referee message.
    \param rm RefereeMessage that contains the play mode
    \return PlayModeT of RefereeMessage, PM_ILLEGAL if it is not recognized */
PlayModeT SoccerTypes::getPlayModeFromRefereeMessage( RefereeMessageT rm )
{
    switch ( rm )
    {
    case REFC_BEFORE_KICK_OFF:
        return PM_BEFORE_KICK_OFF;
    case REFC_KICK_OFF_LEFT:
        return PM_KICK_OFF_LEFT;
    case REFC_KICK_OFF_RIGHT:
        return PM_KICK_OFF_RIGHT;
    case REFC_KICK_IN_LEFT:
        return PM_KICK_IN_LEFT;
    case REFC_KICK_IN_RIGHT:
        return PM_KICK_IN_RIGHT;
    case REFC_CORNER_KICK_LEFT:
        return PM_CORNER_KICK_LEFT;
    case REFC_CORNER_KICK_RIGHT:
        return PM_CORNER_KICK_RIGHT;
    case REFC_GOAL_KICK_LEFT:
        return PM_GOAL_KICK_LEFT;
    case REFC_GOAL_KICK_RIGHT:
        return PM_GOAL_KICK_RIGHT;
    case REFC_FREE_KICK_LEFT:
        return PM_FREE_KICK_LEFT;
    case REFC_FREE_KICK_RIGHT:
        return PM_FREE_KICK_RIGHT;
    case REFC_INDIRECT_FREE_KICK_LEFT:
        return PM_INDIRECT_FREE_KICK_LEFT;
    case REFC_INDIRECT_FREE_KICK_RIGHT:
        return PM_INDIRECT_FREE_KICK_RIGHT;
    case REFC_FREE_KICK_FAULT_LEFT:
        return PM_FREE_KICK_FAULT_LEFT;
    case REFC_FREE_KICK_FAULT_RIGHT:
        return PM_FREE_KICK_FAULT_RIGHT;
    case REFC_BACK_PASS_LEFT:
        return PM_BACK_PASS_LEFT;
    case REFC_BACK_PASS_RIGHT:
        return PM_BACK_PASS_RIGHT;
    case REFC_FOUL_LEFT:
        return PM_FREE_KICK_RIGHT;
    case REFC_FOUL_RIGHT:
        return PM_FREE_KICK_LEFT;
    case REFC_OFFSIDE_LEFT:
        return PM_OFFSIDE_LEFT;
    case REFC_OFFSIDE_RIGHT:
        return PM_OFFSIDE_RIGHT;
    case REFC_GOAL_LEFT:
        return PM_GOAL_LEFT;
    case REFC_GOAL_RIGHT:
        return PM_GOAL_RIGHT;
    case REFC_PENALTY_SETUP_LEFT:
        return PM_PENALTY_SETUP_LEFT;
    case REFC_PENALTY_SETUP_RIGHT:
        return PM_PENALTY_SETUP_RIGHT;
    case REFC_PENALTY_READY_LEFT:
        return PM_PENALTY_READY_LEFT;
    case REFC_PENALTY_READY_RIGHT:
        return PM_PENALTY_READY_RIGHT;
    case REFC_PENALTY_TAKEN_LEFT:
        return PM_PENALTY_TAKEN_LEFT;
    case REFC_PENALTY_TAKEN_RIGHT:
        return PM_PENALTY_TAKEN_RIGHT;
    case REFC_PLAY_ON:
        return PM_PLAY_ON;
    case REFC_FROZEN:
        return PM_FROZEN;
    case REFC_TIME_OVER:
        return PM_TIME_OVER;
    case REFC_QUIT:
        return PM_QUIT;
    default:
        return PM_ILLEGAL;
    }
}

/*! This method returns the string representation of a RefereeMessageT as is
    used in the Robocup Soccer Simulation and said by the referee.
    \param pm RefereeMessageT which should be converted
    \return  pointer to the string (enough memory should be allocated) */
char* SoccerTypes::getRefereeMessageStr( RefereeMessageT rm )
{
    switch ( rm )
    {
    case REFC_BEFORE_KICK_OFF:
        return "before_kick_off";
    case REFC_KICK_OFF_LEFT:
        return "kick_off_l";
    case REFC_KICK_OFF_RIGHT:
        return "kick_off_r";
    case REFC_KICK_IN_LEFT:
        return "kick_in_l";
    case REFC_KICK_IN_RIGHT:
        return "kick_in_r";
    case REFC_CORNER_KICK_LEFT:
        return "corner_kick_l";
    case REFC_CORNER_KICK_RIGHT:
        return "corner_kick_r";
    case REFC_GOAL_KICK_LEFT:
        return "goal_kick_l";
    case REFC_GOAL_KICK_RIGHT:
        return "goal_kick_r";
    case REFC_FREE_KICK_LEFT:
        return "free_kick_l";
    case REFC_FREE_KICK_RIGHT:
        return "free_kick_r";
    case REFC_INDIRECT_FREE_KICK_LEFT:
        return "indirect_free_kick_l";
    case REFC_INDIRECT_FREE_KICK_RIGHT:
        return "indirect_free_kick_r";
    case REFC_FREE_KICK_FAULT_LEFT:
        return "free_kick_fault_l";
    case REFC_FREE_KICK_FAULT_RIGHT:
        return "free_kick_fault_r";
    case REFC_BACK_PASS_LEFT:
        return "back_pass_l";
    case REFC_BACK_PASS_RIGHT:
        return "back_pass_r";
    case REFC_PLAY_ON:
        return "play_on";
    case REFC_FROZEN:
        return "play_off";
    case REFC_QUIT:
        return "quit";
    case REFC_OFFSIDE_LEFT:
        return "offside_l";
    case REFC_OFFSIDE_RIGHT:
        return "offside_r";
    case REFC_HALF_TIME:
        return "half_time";
    case REFC_TIME_UP:
        return "time_up";
    case REFC_TIME_OVER:
        return "time_over";
    case REFC_TIME_UP_WITHOUT_A_TEAM:
        return "time_up_without_a_team";
    case REFC_TIME_EXTENDED:
        return "time_extended";
    case REFC_FOUL_LEFT:
        return "foul_l";
    case REFC_FOUL_RIGHT:
        return "foul_r";
    case REFC_GOAL_LEFT:
        return "goal_l";
    case REFC_GOAL_RIGHT:
        return "goal_r";
    case REFC_DROP_BALL:
        return "drop_ball";
    case REFC_GOALIE_CATCH_BALL_LEFT:
        return "goalie_catch_ball_l";
    case REFC_GOALIE_CATCH_BALL_RIGHT:
        return "goalie_catch_ball_r";
    case REFC_PENALTY_SETUP_LEFT:
        return "penalty_setup_left";
    case REFC_PENALTY_SETUP_RIGHT:
        return "penalty_setup_right";
    case REFC_PENALTY_READY_LEFT:
        return "penalty_ready_left";
    case REFC_PENALTY_READY_RIGHT:
        return "penalty_ready_right";
    case REFC_PENALTY_TAKEN_LEFT:
        return "penalty_taken_left";
    case REFC_PENALTY_TAKEN_RIGHT:
        return "penalty_taken_right";
    case REFC_PENALTY_MISS_LEFT:
        return "penalty_miss_left";
    case REFC_PENALTY_MISS_RIGHT:
        return "penalty_miss_right";
    case REFC_PENALTY_SCORE_LEFT:
        return "penalty_score_left";
    case REFC_PENALTY_SCORE_RIGHT:
        return "penalty_score_right";
    case REFC_PENALTY_FOUL_LEFT:
        return "penalty_foul_left";
    case REFC_PENALTY_FOUL_RIGHT:
        return "penalty_foul_right";
    case REFC_PENALTY_ONFIELD_LEFT:
        return "penalty_onfield_left";
    case REFC_PENALTY_ONFIELD_RIGHT:
        return "penalty_onfield_right";
    case REFC_PENALTY_WINNER_LEFT:
        return "penalty_winner_l";
    case REFC_PENALTY_WINNER_RIGHT:
        return "penalty_winner_r";
    case REFC_PENALTY_DRAW:
        return "penalty_draw";
    case REFC_ILLEGAL:
    default:
        return NULL;
    }
}


/*! This method returns the referee message from the string that is passed.
    \param str pointer to a string with the referee message starting at index 0

    \return RefereeMessageT of string representation, REFC_ILLEGAL if
    it fails */
RefereeMessageT SoccerTypes::getRefereeMessageFromStr( char* str )
{
    switch ( str[0] )
    {
    case 'b':                   // before_kick_off, back_pass_[lr]
        if ( str[1] == 'e' )
            return REFC_BEFORE_KICK_OFF;
        else
            return (str[10] == 'l') ? REFC_BACK_PASS_LEFT : REFC_BACK_PASS_RIGHT;
    case 'c':                   // corner_kick_l or corner_kick_r
        return (str[12] == 'l') ? REFC_CORNER_KICK_LEFT : REFC_CORNER_KICK_RIGHT;
    case 'd':                   // drop_ball
        return REFC_DROP_BALL;
    case 'g':
        switch ( str[5] )
        {
        case 'k':              // goal_kick_l, goal_kick_r
            return (str[10] == 'l') ? REFC_GOAL_KICK_LEFT : REFC_GOAL_KICK_RIGHT;
        case 'e':              // goalie_catch_ball_l, goalie_catch_ball_r
            return (str[18] == 'l') ? REFC_GOALIE_CATCH_BALL_LEFT
                   : REFC_GOALIE_CATCH_BALL_RIGHT;
        case 'l':              // goal_l
            return REFC_GOAL_LEFT;
        case 'r':              // goal_r
            return REFC_GOAL_RIGHT;
        }
    case 'h':                  // half_time
        return REFC_HALF_TIME;
    case 'i':                  // indirect_free_kick_[lr]
        if ( strlen( str ) > 19 )
            return ( str[19] == 'l' )
                   ?  REFC_INDIRECT_FREE_KICK_LEFT
                   :  REFC_INDIRECT_FREE_KICK_RIGHT;
        break;
    case 'f':
        if ( str[5] == 'k' )     // free_kick_[lr], free_kick_fault_[lr]
        {
            if ( str[10] == 'f' )
                return ( str[16] == 'l' )
                       ? REFC_FREE_KICK_FAULT_LEFT
                       : REFC_FREE_KICK_FAULT_RIGHT;
            else
                return (str[10] == 'l') ? REFC_FREE_KICK_LEFT : REFC_FREE_KICK_RIGHT;
        }
        else if ( str[5] == 'l' ) // foul_l
            return REFC_FOUL_LEFT;
        else if ( str[5] == 'r' ) // foul_r
            return REFC_FOUL_RIGHT;
    case 'k':                  // kick_in_l, kick_in_r, kick_off_l, kick_off_r
        if ( str[5] == 'i' )
            return (str[8] == 'l' )   ? REFC_KICK_IN_LEFT   : REFC_KICK_IN_RIGHT;
        else if ( str[5] == 'o' )
            return (str[9] == 'l' )   ? REFC_KICK_OFF_LEFT  : REFC_KICK_OFF_RIGHT;
    case 'o':
        return ( str[8] == 'l' )    ? REFC_OFFSIDE_LEFT   : REFC_OFFSIDE_RIGHT;
    case 'p':                // play_on or penalty_setup_l or penalty_ready_l
        if ( str[1] == 'l' )   // penalty_taken_l, penalty_miss_l, penalty_foul_l
            return REFC_PLAY_ON; // penalty_onfield_l, penalty_score_l,
        else if ( str[8] == 's' && str[9] == 'e' )
            // penalty_winner_l,penalty_draw
            return ( str[14] == 'l' ) ? REFC_PENALTY_SETUP_LEFT
                   : REFC_PENALTY_SETUP_RIGHT;
        else if ( str[8] == 's' && str[9] == 'c' )
            return ( str[14] == 'l' ) ? REFC_PENALTY_SCORE_LEFT
                   : REFC_PENALTY_SCORE_RIGHT;
        else if ( str[8] == 'r' )
            return ( str[14] == 'l' ) ? REFC_PENALTY_READY_LEFT
                   : REFC_PENALTY_READY_RIGHT;
        else if ( str[8] == 't' )
            return ( str[14] == 'l' ) ? REFC_PENALTY_TAKEN_LEFT
                   : REFC_PENALTY_TAKEN_RIGHT;
        else if ( str[8] == 'm' )
            return ( str[13] == 'l' ) ? REFC_PENALTY_MISS_LEFT
                   : REFC_PENALTY_MISS_RIGHT;
        else if ( str[8] == 'o' )
            return ( str[16] == 'l' ) ? REFC_PENALTY_ONFIELD_LEFT
                   : REFC_PENALTY_ONFIELD_RIGHT;
        else if ( str[8] == 'f' )
            return ( str[13] == 'l' ) ? REFC_PENALTY_FOUL_LEFT
                   : REFC_PENALTY_FOUL_RIGHT;
        else if ( str[8] == 'f' )
            return ( str[15] == 'l' ) ? REFC_PENALTY_WINNER_LEFT
                   : REFC_PENALTY_WINNER_RIGHT;
        else if ( str[8] == 'd' )
            return REFC_PENALTY_DRAW;
    case 't':
        if ( str[5] == 'o' )     // time_over
            return REFC_TIME_OVER;
        else if ( str[5] == 'e' ) // time_extended
            return REFC_TIME_EXTENDED;
        else if ( str[5] == 'u' ) // time_up
            return REFC_TIME_UP;
        else if ( str[8] == 'w' ) // time_up_without_a_team
            return REFC_TIME_UP_WITHOUT_A_TEAM;
    default:
        printf("(SoccerTypes::getRefereeMessageFromStr) ?? ref msg %s\n",str);
        return REFC_ILLEGAL;
    }
    printf("(SoccerTypes::getRefereeMessageFromStr) ?? ref msg %s\n",str);
    return REFC_ILLEGAL;
}


/*! This method returns the string representation of a ViewAngleT as is used
    in the Robocup Soccer Simulation
    \param va ViewAngleT which should be converted
    \return  pointer to the string (enough memory should be allocatd) */
char* SoccerTypes::getViewAngleStr( ViewAngleT va )
{
    switch ( va )
    {
    case VA_NARROW:
        return "narrow";
    case VA_NORMAL:
        return "normal";
    case VA_WIDE:
        return "wide";
    case VA_ILLEGAL:
    default:
        return NULL;
    }
}

/*! This method returns et the view angle from the specified string
    \param str pointer to a string that contains view angle string at index 0
    \return ViewAngleT of string, VA_ILLEGAL if it is not recognized */
ViewAngleT SoccerTypes::getViewAngleFromStr( char* str )
{
    switch ( str[1] )
    {
    case 'a':
        return VA_NARROW;
    case 'o':
        return VA_NORMAL;
    case 'i':
        return VA_WIDE;
    default:
        return VA_ILLEGAL;
    }
}

/*! This method returns the half angle value that belongs to the
    ViewAngle that is given as the first argument (VA_NARROW,
    VA_NORMAL or VA_WIDE). The half view angle is returned since this
    makes it easier to check whether an object lies in the view cone
    (the global relative angle must be smaller than the half view
    angle.

    \param va view angle
    \return angle denoting the half of the corresponding view angle */
AngDeg SoccerTypes::getHalfViewAngleValue( ViewAngleT va )
{
    switch ( va )
    {
    case VA_NARROW:
        return 22.5;
    case VA_NORMAL:
        return 45.0;
    case VA_WIDE:
        return 90.0;
    case VA_ILLEGAL:
    default:
        return 0.0;
    }

}

/*! This method returns the string representation of a ViewQualityT as is used
    in the Robocup Soccer Simulation
    \param vq ViewQualityT which should be converted
    \return  pointer to the string (enough memory should be allocated) */
char* SoccerTypes::getViewQualityStr( ViewQualityT vq )
{
    switch ( vq )
    {
    case VQ_HIGH:
        return "high";
    case VQ_LOW:
        return "low";
    case VQ_ILLEGAL:
    default:
        return NULL;
    }
}

/*! This method returns the view quality from the string that is passed as the
    first argument
   \param str pointer to a string that contains view quality string at index 0
   \return ViewQualityT of string, VQ_ILLEGAL if it is not known */
ViewQualityT SoccerTypes::getViewQualityFromStr( char* str )
{
    if ( str[0] == 'h' )      return VQ_HIGH;
    else if ( str[0] == 'l' ) return VQ_LOW;
    else                     return VQ_ILLEGAL;
}

/*! This method returns the string representation of a CommandT as is
    used in the Robocup Soccer Simulation
    \param com CommandT that should be converted
    \return  pointer to the string (enough memory should be allocated) */
char* SoccerTypes::getCommandStr( CommandT com )
{
    switch ( com )
    {
    case CMD_DASH:
        return "dash";
    case CMD_TURN:
        return "turn";
    case CMD_TURNNECK:
        return "turn_neck";
    case CMD_CHANGEVIEW:
        return "change_view";
    case CMD_CATCH:
        return "catch";
    case CMD_KICK:
        return "kick";
    case CMD_MOVE:
        return "move";
    case CMD_SENSEBODY:
        return "sense_body";
    case CMD_SAY:
        return "neck";
    case CMD_ATTENTIONTO:
        return "attentionto";
    case CMD_TACKLE:
        return "tackle";
    case CMD_POINTTO:
        return "pointto";
    case CMD_ILLEGAL:
    default:
        return NULL;

    }
}

/*! This method returns return true when argument is a primary action
    (action that can only be sent once a cycle). This is the case for kick,
    dash, move, turn and catch commands.
    \param CommandT command that should be checked
    \return true when it is a primary action, false otherwise */
bool SoccerTypes::isPrimaryCommand( CommandT com )
{
    return com == CMD_KICK || com == CMD_DASH || com == CMD_MOVE ||
           com == CMD_TURN || com == CMD_CATCH || com == CMD_TACKLE ;
}


/*! This method returns the string representation of a SideT as is used in the
    Robocup Soccer Simulation (r or l).
    \param s SideT which should be converted
    \return pointer to the string */
char* SoccerTypes::getSideStr( SideT s )
{
    switch ( s )
    {
    case SIDE_LEFT:
        return "l";
    case SIDE_RIGHT:
        return "r";
    case SIDE_ILLEGAL:
    default:
        return NULL;
    }
}

/*! This method returns the SideT from the string that is passed as the first
    argument.
    \param str pointer to a string that contains side info string at index 0
    \return SideT of string representation, SIDE_ILLEGAL if it is not known */
SideT SoccerTypes::getSideFromStr( char* str )
{
    if ( str[0] == 'l' )      return SIDE_LEFT;
    else if ( str[0] == 'r' ) return SIDE_RIGHT;
    else                     return SIDE_ILLEGAL;

}

/*! This method returns the string representation of the BallStatus as is used
    in the Robocup Soccer Simulation (in_field, goal_left, goal_right or
    out_of_field).
    \param bs BallStatus which should be converted
    \return pointer to the string (enough memory should be allocated) */
char* SoccerTypes::getBallStatusStr( BallStatusT bs )
{
    switch ( bs )
    {
    case BS_IN_FIELD:
        return "in_field";
    case BS_GOAL_LEFT:
        return "goal_left";
    case BS_GOAL_RIGHT:
        return "goal_right";
    case BS_OUT_OF_FIELD:
        return "out_of_field";
    default:
        return NULL;
    }
}

/*! This method returns the BallStatus from the string that is passed as the
    first argument.
   \param str pointer to a string that contains ball status info at index 0

   \return BallStatus of string representation, BS_ILLEGAL if it is
   not known */
BallStatusT SoccerTypes::getBallStatusFromStr( char* str )
{
    switch ( str[0] )
    {
    case 'i':
        return BS_IN_FIELD;
    case 'o':
        return BS_OUT_OF_FIELD;
    case 'g':
        return (str[5]=='l') ? BS_GOAL_LEFT : BS_GOAL_RIGHT;
    default:
        cout << "(SoccerTypes::getBallStatusFromStr) illegal status " <<
             str << "\n";
        return BS_ILLEGAL;
    }
}

/*! This method returns the angle value corresponding to the direction
    'dir'. */
AngDeg SoccerTypes::getAngleFromDirection( DirectionT dir )
{
    switch ( dir )
    {
    case DIR_NORTH:
        return    0.0;
    case DIR_NORTHWEST:
        return  -45.0;
    case DIR_WEST:
        return  -90.0;
    case DIR_SOUTHWEST:
        return -135.0;
    case DIR_SOUTH:
        return  180.0;
    case DIR_SOUTHEAST:
        return  135.0;
    case DIR_EAST:
        return   90.0;
    case DIR_NORTHEAST:
        return   45.0;
    case DIR_ILLEGAL:
    default:
        break;

    }
    cerr << "(SoccerTypes::getAngleFromDirection) illegal dir " << dir << endl;
    return 0.0;
}





/*****************************************************************************/
/********************* TESTING PURPOSES **************************************/
/*****************************************************************************/

/*
int main( void )
{
  char* s = "(p \"1\" 3)";

      "(see 0 ((g r) 56.3 0) ((f c) 3.8 0 -0 0) ((f r t) 66 -31) ((f r b)
      66 31) ((f p r t) 44.7 -26) ((f p r c) 39.6 0) ((f p r
      b) 44.7 26) ((f g r t) 56.8 -7) ((f g r b) 56.8 7) ((f t
      r 40) 58.6 -41) ((f t r 50) 66.7 -35) ((f b r 40) 58.6
      41) ((f b r 50) 66.7 35) ((f r t 30) 68 -26) ((f r t 20)
      64.7 -18) ((f r t 10) 62.2 -9) ((f r 0) 61.6 0) ((f r b
      10) 62.2 9) ((f r b 20) 64.7 18) ((f r b 30) 68 26) ((b)
      3.7 0 0 0) ((P) 1.2 179) ((P) 0.6 179) ((p "1" 3) nan 0
      0 0 0 0) ((p "1" 4) nan 0 0 0 0 0) ((l r) 56.3 -89))";
  char *ptr = s;
  char buf[1023];
  printf("%s\n", ptr );
  ObjectT o = SoccerTypes::getObjectFromStr(&ptr, "1");
  printf("%d, %s\n", o, SoccerTypes::getObjectStr(buf,o,"1") );
}


int main( void )
{
  Time t( 10, 0 );
  t.show();
  t.updateTime( 10 );
  t.show();
  t.updateTime( 10 );
  t.show();
  cout << t.isStopped();
  t.updateTime( 12 );
  t.show();
  t.updateTime( 12);
  t.setTimeStopped(2);
  Time t2( 12, 3 );
  cout << t.getTimeDifference( t2 ) << " " << t2.isStopped() << "\n";
  cout << (t < t2 ) << ( t > t2 ) << ( t <= t2 ) << (t == t2 ) << "\n";
}

*/
