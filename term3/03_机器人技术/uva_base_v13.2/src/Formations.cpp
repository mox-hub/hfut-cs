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

/*! \file Formations.cpp

<pre>
<b>File:</b>          Formations.cpp
<b>Project:</b>       Robocup Soccer Simulation Team: UvA Trilearn
<b>Authors:</b>       Jelle Kok
<b>Created:</b>       05/02/2001
<b>Last Revision:</b> $ID$
<b>Contents:</b>      Definitions for the different methods in the classes that
               are associated with Formations.                      .
<hr size=2>
<h2><b>Changes</b></h2>
<b>Date</b>             <b>Author</b>          <b>Comment</b>
05/02/2001       Jelle Kok       Initial version created
</pre>
*/

#include "Formations.h"
#include "Parse.h"       // Parse

#include <fstream>       // ifstream
#include <stdio.h>       // printf
#include <math.h>        // fabs

/*****************************************************************************/
/********************** CLASS PLAYERTYPEINFO *********************************/
/*****************************************************************************/

/*! This method is the default constructor and sets all the values of this
    class to "illegal" values. This method is needed when an array of this
    class is initialized, since then the default constructor (without
    arguments) is called. Afterwards the actual values should be set using
    the method setValues. */
PlayerTypeInfo::PlayerTypeInfo()
{
    setValues( PT_ILLEGAL, UnknownDoubleValue,UnknownDoubleValue,
               UnknownDoubleValue,UnknownDoubleValue, false );
}

/*! This Constructor receives the values for all the member variables as
    arguments and initializes the member variables using the method setValues.
    \param pt PlayerType corresponding to the player type of this class
    \param dAttrX x attraction to the ball
    \param dAttrY y attraction to the ball
    \param dMinX minimal x coordinate for this player type
    \param dMaxX maximal x coordinate for this player type
    \param bBehindBall boolean indicating whether this player type should
                       always stay behind the ball. */
PlayerTypeInfo::PlayerTypeInfo( PlayerT pt, double dAttrX, double dAttrY,
                                double dMinX, double dMaxX, bool bBehindBall  )
{
    setValues( pt, dAttrX, dAttrY, dMinX, dMaxX, bBehindBall );
}

/*! This method receives the values for all the member variables as arguments
    and sets these member variables.
    \param pt PlayerType corresponding to the player type of this class
    \param ax x attraction to the ball
    \param ay y attraction to the ball
    \param minx minimal x coordinate for this player type
    \param maxx maximal x coordinate for this player type
    \param bb boolean indicating whether this player type should always
                       stay behind the ball.
    \return bool indicating whether update was successful. */
bool PlayerTypeInfo::setValues( PlayerT pt,   double ax,   double ay,
                                double minx,  double maxx, bool bb )
{
    playerType      = pt;
    dAttrX          = ax;
    dAttrY          = ay;
    dMinX           = minx;
    dMaxX           = maxx;
    bBehindBall     = bb;

    return true;
}

/*! This method print the different member values separated by comma's to the
    specified output stream.
    \param os output stream to which member values are printed */
void PlayerTypeInfo::show( ostream &os )
{
    os << "(" << (int)playerType << ", " << dAttrX << ", " << dAttrY << ", "
    << dMinX           << ", " << dMaxX  << ", " << bBehindBall
    <<  ")" << endl;

}

/*! This method sets the player type associated with this class.
    \param type new player type
    \return bool indicating whether update was succesfull */
bool PlayerTypeInfo::setPlayerType( PlayerT type )
{
    playerType = type;
    return true;
}

/*! This method returns the player type associated with this class.
    \return player type of this class */
PlayerT PlayerTypeInfo::getPlayerType( ) const
{
    return playerType;
}

/*! This method sets the x attraction to the ball for this player type. The x
    attraction to the ball is a double in the range (0,1). This value is used
    to determine the x coordinate of the strategic position for this player
    type. The x attraction of the ball is multiplied with the x coordinate of
    the ball and added to the home position of the agent to determine the x
    coordinate of the strategic position.
    \param dAttractionX new x attraction for this player type
    \return bool indicating whether update was succesfull */
bool PlayerTypeInfo::setAttrX( double dAttractionX )
{
    dAttrX = dAttractionX;
    return true;
}

/*! This method returns the x attraction to the ball for this player type. The
    x attraction to the ball is a double in the range (0,1). This value is used
    to determine the x coordinate of the strategic position for this player
    type. The x attraction of the ball is multiplied with the x coordinate of
    the ball and added to the home position of the agent to determine the x
    coordinate of the strategic position.
    \return x attraction for this player type */
double PlayerTypeInfo::getAttrX( ) const
{
    return dAttrX;
}

/*! This method sets the y attraction to the ball for this player type. The y
    attraction to the ball is a double in the range (0,1). This value is used
    to determine the y coordinate of the strategic position for this player
    type. The y attraction of the ball is multiplied with the y coordinate of
    the ball and added to the home position of the agent to determine the y
    coordinate of the strategic position.
    \param dAttractionY new y attraction for this player type
    \return bool indicating whether update was succesfull */
bool PlayerTypeInfo::setAttrY( double dAttractionY )
{
    dAttrY = dAttractionY;
    return true;
}

/*! This method returns the y attraction to the ball for this player type. The
    y attraction to the ball is a double in the range (0,1). This value is used
    to determine the y coordinate of the strategic position for this player
    type. The y attraction of the ball is multiplied with the y coordinate of
    the ball and added to the home position of the agent to determine the y
    coordinate of the strategic position.
    \return y attraction for this player type */
double PlayerTypeInfo::getAttrY( ) const
{
    return dAttrY;
}

/*! This method sets the minimal x coordinate for this player type. When the
    calculated x coordinate for the strategic position is lower than this
    value, the x coordinate is set to this minimal x coordinate.
    \param dMinimalX new minimal x coordinate for this player type
    \return bool indicating whether update was succesfull. */
bool PlayerTypeInfo::setMinX( double dMinimalX )
{
    dMinX = dMinimalX;
    return true;
}

/*! This method returns the minimal x coordinate for this player type. When the
    calculated x coordinate for the strategic position is lower than this
    value, the x coordinate is set to this minimal x coordinate.
    \return minimal x coordinate for this player type */
double PlayerTypeInfo::getMinX( ) const
{
    return dMinX;
}

/*! This method sets the maximal x coordinate for this player type. When the
    calculated x coordinate for the strategic position is larger than this
    value, the x coordinate is set to this maximal x coordinate.
    \param dMaximalX new maximal x coordinate for this player type
    \return bool indicating whether update was succesfull. */
bool PlayerTypeInfo::setMaxX( double dMaximalX )
{
    dMaxX = dMaximalX;
    return true;
}

/*! This method returns the maximal x coordinate for this player type. When the
    calculated x coordinate for the strategic position is larger than this
    value, the x coordinate is set to this maximal x coordinate.
    \return maximal x coordinate for this player type */
double PlayerTypeInfo::getMaxX( ) const
{
    return dMaxX;
}

/*!This method sets the value that indicates whether this player type should
   stay behind the ball or not. When set to true and the strategic position for
   this player type is calculated to be in front of the ball. The x coordinate
   of the strategic position is set to the x coordinate of the ball.
   \param b boolean indicating whether this playertype should stay behind the
            ball
   \return bool indicating whether update was succesfull. */
bool PlayerTypeInfo::setBehindBall( bool b )
{
    bBehindBall = b;
    return true;
}

/*! This method returns the value that indicates whether this player type
    should stay behind the ball or not. When set to true and the strategic
    position for this player type is calculated to be in front of the ball.
    The x coordinate of the strategic position is set to the x coordinate
    of the ball.
    \return bool indicating whether to stay behind the ball or not */
bool PlayerTypeInfo::getBehindBall( ) const
{
    return bBehindBall;
}

/*****************************************************************************/
/********************** CLASS FORMATIONTYPEINFO*******************************/
/*****************************************************************************/

/*! This is the default constructor and does nothing. */
FormationTypeInfo::FormationTypeInfo( )
{
}

/*!This method sets the current formation type for this class.
   \param type new formation type for this class.
   \return bool indicating whether update was successful. */
bool FormationTypeInfo::setFormationType( FormationT type )
{
    formationType = type;
    return true;
}

/*! This method return the current formation type for this class.
    \return formation type for this class. */
FormationT FormationTypeInfo::getFormationType( ) const
{
    return formationType;
}

/*! This method prints all the information about this formation to the
    specified output stream. The format is the following:
    - x coordinate of the home position for all the roles
    - y coordinate of the home position for all the roles
    - the player types for all the roles
    - the x attraction for all the player types
    - the y attraction for all the player types
    - indication whether to stay behind the ball for all the player types
    - minimal x coordinate for all the player types
    - maximal x coordinate for all the player types
    \param os output stream for output. */
void FormationTypeInfo::show( ostream &os )
{
    char str[128];
    for ( int i = 0; i < MAX_TEAMMATES; i++ )
    {
        sprintf(str, "%3.2f ", posHome[i].getX() );
        os << str;
    }
    os << endl;
    for ( int i = 0; i < MAX_TEAMMATES; i++ )
    {
        sprintf( str, "%3.2f ", posHome[i].getY() );
        os << str;
    }
    os << endl;
    for ( int i = 0; i < MAX_TEAMMATES; i++ )
    {
        sprintf( str, "%5d ", (int)playerType[i] );
        os << str;
    }
    os << endl;
    for ( int i = 0; i < MAX_PLAYER_TYPES; i++ )
    {
        sprintf( str, "%3.2f ", playerTypeInfo[i].getAttrX() );
        os << str;
    }
    os << endl;
    for ( int i = 0; i < MAX_PLAYER_TYPES; i++ )
    {
        sprintf( str, "%3.2f ", playerTypeInfo[i].getAttrY() );
        os << str;
    }
    os << endl;
    for ( int i = 0; i < MAX_PLAYER_TYPES; i++ )
    {
        sprintf( str, "%5d ", playerTypeInfo[i].getBehindBall() );
        os << str;
    }
    os << endl;
    for ( int i = 0; i < MAX_PLAYER_TYPES; i++ )
    {
        sprintf( str, "%3.2f ", playerTypeInfo[i].getMinX() );
        os << str;
    }
    os << endl;
    for ( int i = 0; i < MAX_PLAYER_TYPES; i++ )
    {
        sprintf( str, "%3.2f ", playerTypeInfo[i].getMaxX() );
        os << str;
    }
    os << endl;
}


/*! This method sets the home position of the role indicated by the number
    'atIndex' in this formation. The home position is the position from which
    the strategic position is calculated and could be interpreted as the
    position a player is located when the ball is at the position (0,0).
    \param pos new home position for the player with role number 'atIndex'
    \param atIndex index of the player with role for which the home
           position should be set.
    \return bool indicating whether update was succesfull. */
bool FormationTypeInfo::setPosHome( VecPosition pos, int atIndex )
{
    posHome[ atIndex ] = pos;
    return true;
}

/*! This method sets the x coordinate of the home position for the player
    with role number 'atIndex'.
    \param x x coordinate for the home position
    \param atIndex role in formation for which x coordinate should be set.
    \return bool indicating whether update was succesfull. */
bool FormationTypeInfo::setXPosHome( double x, int atIndex )
{
    posHome[ atIndex ].setX( x );
    return true;
}

/*! This method sets the y coordinate of the home position for the player
    with role number 'atIndex'.
    \param y y coordinate for the home position
    \param atIndex role number for which y coordinate should be set.
    \return bool indicating whether update was succesfull. */
bool FormationTypeInfo::setYPosHome( double y, int atIndex )
{
    posHome[ atIndex ].setY( y );
    return true;
}

/*! This method returns the home position for the player with role number
    atIndex in this formation. The home position is the position from which the
    strategic position is calculated and could be interpreted as the position a
    player is located when the ball is at the position (0,0).
    \return home position for player number at index 'atIndex' */
VecPosition FormationTypeInfo::getPosHome( int atIndex ) const
{
    return posHome[ atIndex ];
}

/*! This method sets the player type for the player with role number 'atIndex'
    in this formation.
    \param type new player type for role at position 'atIndex'
    \param atIndex role number for which player type should be set.
    \return bool indicating whether update was succesfull. */
bool FormationTypeInfo::setPlayerType( PlayerT type, int atIndex )
{
    playerType[ atIndex ] = type;
    return true;
}

/*! This method returns the player type for the player with role number
    'atIndex' in this formation.
    \return player type for player with role number 'atIndex' */
PlayerT FormationTypeInfo::getPlayerType( int atIndex ) const
{
    return playerType[ atIndex ];
}

/*! This method sets the information for a player type in this formation. Note
    that information is for a player TYPE and not for a player ROLE.
    \param info new player type information for the player type at 'atIndex'.
    \param atIndex number of player type for which information should be set.
    \return bool indicating whether update was succesfull. */
bool FormationTypeInfo::setPlayerTypeInfo( PlayerTypeInfo info, int atIndex )
{
    playerTypeInfo[ atIndex ] = info;
    return true;
}

/*! This method returns (a pointer to) the player type information for the
    player type at position 'atIndex'
    \param atIndex index of which player type information should be returned
    \return pointer to player type information located at index 'atIndex' */
PlayerTypeInfo* FormationTypeInfo::getPlayerTypeInfo( int atIndex )
{
    return &playerTypeInfo[ atIndex ];
}


/*! This method returns (a pointer to) the player type information for the
    player with role number 'iPlayerInFormation'.
    \param iPlayerInFormation role number for which info should be returned
    \return pointer to information for role 'iPlayerInFormation'*/
PlayerTypeInfo* FormationTypeInfo::getPlayerTypeInfoOfPlayer(
    int iPlayerInFormation )
{
    return &playerTypeInfo[ playerType[iPlayerInFormation] ];
}

/*****************************************************************************/
/********************** CLASS FORMATIONS *************************************/
/*****************************************************************************/
//!new add
Formations   *FS;

/*! This is the constructor for the Formations class and needs as arguments
    a formation configuration file, the current formation and the number of the
    agent in this formation (normally at start-up this equals the player
    number).
    \param strFile string representation of the formation configuration file
    \param curFt current formation type (default FT_ILLEGAL)
    \param iNr number of the agent in this formation (default 1)*/
Formations::Formations( const char *strFile, FormationT curFt, int iNr )
{
    //!new add
    FS = this;
    if ( strFile[0] == '\0' )
    {
        cerr << "(Formations::Formations) No Filename given" << endl;
        return;
    }

    if ( readFormations( strFile ) == false )
        cerr << "(Formations::Formations) Error reading file " << strFile << endl;
    curFormation = curFt;
    setPlayerInFormation( iNr );
}

/*! This methods prints all the information of the different formation types to
    the output stream os and furthermore prints the current formation and the
    role number of the agent in this formation.
    \param os output stream to which output is written. */
void Formations::show( ostream &os )
{
    for ( int i = 0 ; i < MAX_FORMATION_TYPES; i ++ )
        formations[i].show( os );
    os << "Current formation: " << (int)curFormation << endl
    << "Player nr in formation: " << iPlayerInFormation ;
}

/*! This method returns the strategic position for a player. It calculates this
    information by taking the home position of the current role in the
    current formation and combines this with the position of the ball using the
    attraction values for the current player type. The attraction values
    defines the percentage of the ball coordinate that is added to the home
    position of the current player type. So when the x coordindate of the home
    position is 10.0, x coordinate ball is 20.0 and x attraction is 0.25. The
    x coordinate of the strategic position will become 10.0 + 0.25*20.0 = 15.0.
    When this value is smaller than the minimal x coordinate or larger than
    the maximal x coordinate, the coordinate is changed to this minimal or
    maximal coordinate respectively. Also when the behind ball value is set,
    the x coordinate of the strategic position is set to this ball coordinate.
    Furthermore when the strategic position is in front of the supplied
    argument dMaxXInPlayMode, the x coordinate is adjusted to this value.
    During normal play mode the supplied value is often the offside line.
    \param iPlayer player number in formation of which strategic position
                   should be determined.
    \param posBall position of the ball
    \param dMaxXInPlayMode, max x coordinate allowed in current play mode. */
VecPosition Formations::getStrategicPosition( int iPlayer, VecPosition posBall,
        double dMaxXInPlayMode, bool bInBallPossession, double dMaxYPercentage,
        FormationT ft )
{
    if ( ft == FT_ILLEGAL )
        ft = curFormation;
    VecPosition     posHome;
    PlayerTypeInfo* ptInfo = formations[ft].
                             getPlayerTypeInfoOfPlayer( iPlayer );
    double x, y;

    // get the home position and calculate the associated strategic position
    posHome = formations[ft].getPosHome( iPlayer );
    y = posHome.getY() + posBall.getY() * ptInfo->getAttrY();
    x = posHome.getX() + posBall.getX() * ptInfo->getAttrX();

    // do not move too much to the side
    if ( fabs( y ) > 0.5*dMaxYPercentage*PITCH_WIDTH )
        y = sign(y)*0.5*dMaxYPercentage*PITCH_WIDTH;

    // when behind ball is set, do not move to point in front of ball
    if ( ptInfo->getBehindBall() == true && x > posBall.getX() )
        x = posBall.getX();

    // do not move past maximal x or before minimal x
    if ( x > ptInfo->getMaxX() )
        x = ptInfo->getMaxX();
    else if ( x < ptInfo->getMinX() )
        x = ptInfo->getMinX();

    // when x coordinate is in front of allowed x value, change it
    if ( x > dMaxXInPlayMode )
        x = dMaxXInPlayMode;

    return VecPosition( x, y );
}

/*! This method reads the formations from the file 'strFile' and has the
    following format:
       - x coordinate of the home position for all the roles
       - y coordinate of the home position for all the roles
       - the player types for all the roles
       - the x attraction for all the player types
       - the y attraction for all the player types
       - indication whether to stay behind the ball for all the player types
       - minimal x coordinate for all the player types
       - maximal x coordinate for all the player types
       - extra y distance added to strat. pos. when in ball possession
    \param strFile string representation of the file.
    \return bool when file was read in succesfully. */
bool Formations::readFormations( const char *strFile )
{
    ifstream in( strFile );
    if ( !in )
    {
        cerr << "(readValues::readValues) Could not open file '" <<
             strFile << "'" << endl;
        return false;
    }

    char strLine[256], *str;
    int            iLineNr          = 0, i;
    int            iForm            = 0; // current formation type that is parsed
    int            iLineInFormation = 0; // current offset of line in formation
    bool           bReturn          = true;
    PlayerTypeInfo *pt_info;

    // read all lines
    while ( bReturn && in.getline( strLine, sizeof(strLine) ) )
    {
        str = &strLine[0];
        iLineNr++;
        // comment and empty lines should be skipped
        if ( !(strLine[0] == '\n' || strLine[0] == '#' || strLine[0]=='\0' ||
                Parse::gotoFirstNonSpace( &str ) == '\0' ) )
        {
            // there are ten different lines in a formation (see comment above)
            // all values for each line are parsed in one iteration
            // after all 10 lines are parsed, the sequence it is resetted.
            switch ( iLineInFormation )
            {
            case 0: // first line is the number of the formation
                iForm = Parse::parseFirstInt( &str );
                break;
            case 1: // the x coordinate of the home pos for all the players
                for ( i = 0 ; i < MAX_TEAMMATES ; i ++ )
                    formations[iForm].setXPosHome(Parse::parseFirstDouble(&str), i);
                break;
            case 2: // the y coordinate of the home pos for all the players
                for ( i = 0 ; i < MAX_TEAMMATES ; i ++ )
                    formations[iForm].setYPosHome(Parse::parseFirstDouble(&str), i);
                break;
            case 3: // the player types for all the players
                for ( i = 0 ; i < MAX_TEAMMATES ; i ++ )
                    formations[iForm].setPlayerType(
                        (PlayerT) Parse::parseFirstInt(&str), i);
                break;
            case 4: // the x attraction for all the player types
                for ( i = 0 ; i < MAX_PLAYER_TYPES ; i ++ )
                {
                    pt_info = formations[iForm].getPlayerTypeInfo( i );
                    pt_info->setAttrX( Parse::parseFirstDouble( &str ) );
                }
                break;
            case 5: // the y attraction for all the player types
                for ( i = 0 ; i < MAX_PLAYER_TYPES ; i ++ )
                {
                    pt_info = formations[iForm].getPlayerTypeInfo( i );
                    pt_info->setAttrY( Parse::parseFirstDouble( &str ) );
                }
                break;
            case 6: // stay behind the ball for all the player types
                for ( i = 0 ; i < MAX_PLAYER_TYPES ; i ++ )
                {
                    pt_info = formations[iForm].getPlayerTypeInfo( i );
                    if ( Parse::parseFirstInt( &str ) == 1 )
                        pt_info->setBehindBall( true );
                    else
                        pt_info->setBehindBall( false );          ;
                }
                break;
            case 7: // the minimal x coordinate for all the player types
                for ( i = 0 ; i < MAX_PLAYER_TYPES ; i ++ )
                {
                    pt_info = formations[iForm].getPlayerTypeInfo( i );
                    pt_info->setMinX( Parse::parseFirstDouble( &str ));
                }
                break;
            case 8:// the maximal x coordinate for all the player types
                for ( i = 0 ; i < MAX_PLAYER_TYPES ; i ++ )
                {
                    pt_info = formations[iForm].getPlayerTypeInfo( i );
                    pt_info->setMaxX( Parse::parseFirstDouble( &str ));
                }
                break;
            default:
                cerr << "(Formations::readFormations) error line " << iLineNr <<endl;
                return false;
            }

            iLineInFormation++;          // go one line further
            if ( iLineInFormation == 9 ) // each formation consists of ten lines
                iLineInFormation = 0;
        }
    }
    return true;
}

/*! This method sets the current formation.
    \param formation new current formation
    \return bool indicating whether the update was successful */
bool Formations::setFormation( FormationT formation )
{
    curFormation = formation;
    return true;
}

/*! This method returns the current formation.
    \return current formation  */
FormationT Formations::getFormation( ) const
{
    return curFormation;
}

/*! This method sets the player number of the agent in the current formation to
    'iNumber'.
    \param iNumber new player number for this agent
    \return bool indicating whether the update was succesfull */
bool Formations::setPlayerInFormation( int iNumber )
{
    iPlayerInFormation = iNumber;
    return true;
}

/*! This method returns the role number of the agent in the current formation
    \return player number for this agent in the current formation */
int Formations::getPlayerInFormation( ObjectT obj  ) const
{
    int i;
    if ( obj == OBJECT_ILLEGAL )
        i = iPlayerInFormation;
    else
        i = SoccerTypes::getIndex( obj );

    return i;
}

/*! This method returns the player type for the specified object
    \return player type for the agent in the current formation */
PlayerT Formations::getPlayerType( ObjectT obj, FormationT ft ) const
{
    if ( ft == FT_ILLEGAL )
        ft = curFormation;
    return formations[ ft ].getPlayerType(
               SoccerTypes::getIndex( obj ) );
}

/*! This method returns the player type for the agent in the current formation
    \return player type for the agent in the current formation */
PlayerT Formations::getPlayerType( int iIndex, FormationT ft ) const
{
    if ( ft == FT_ILLEGAL )
        ft = curFormation;
    if ( iIndex == -1  )
        iIndex = iPlayerInFormation;
    return formations[ ft ].getPlayerType( iIndex );
}

/*************************** TESTING PURPOSES ********************************/

/*
int main( void )
{
  Formations fs( "formations.conf" );
  fs.show( cout );
}
*/
