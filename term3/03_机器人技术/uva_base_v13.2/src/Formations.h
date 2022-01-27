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

/*! \file Formations.h
<pre>

<b>File:</b>          Formations.h
<b>Project:</b>       Robocup Soccer Simulation Team: UvA Trilearn
<b>Authors:</b>       Jelle Kok
<b>Created:</b>       05/02/2001
<b>Last Revision:</b> $ID$
<b>Contents:</b>      Header file for different classes associated with
               Formations.

- PlayerTypeInfo contains the different information for
one playertype (PT_DEFENDER, PT_ATTACKER, etc.). These
should not be confused with the player_types used in the
soccerserver from version 7.xx.  The information consists
of the attraction to the ball, minimal and maximal x
coordinates, etc.
- FormationTypeInfo contains information for all the
roles in one specific formation. This information
consists of the current formation, the home position for
all the roles, the player_type (PT_DEFENDER, PT_ATTACKER,
etc.) for all the roles and the information about all
player_types
- Formations itself. This class contains all the
information of the different Formations. Furthermore it
contains the current formation that is used and the role
in the formation that is associated with the current
agent.

This formation system is based on the formations used by
the simple FC Portugal team, released in 2000.
<hr size=2>
<h2><b>Changes</b></h2>
<b>Date</b>             <b>Author</b>          <b>Comment</b>
05/02/2001       Jelle Kok       Initial version created
</pre> */

#ifndef _FORMATIONS_
#define _FORMATIONS_

#include "SoccerTypes.h"  // PlayerT

/*****************************************************************************/
/********************** CLASS PLAYERTYPEINFO *********************************/
/*****************************************************************************/

/*! This class contains information for one individual player_type, defined in
SoccerTypes.h. A player_type should not be confused with the player_types
introduced in soccerserver 7.xx. A playerType PlayerT is defined as the kind of
a player. Different possibilities are PT_ATTACKER, PT_MIDFIELDER_WING, etc.
This class contains different characteristics of one playertype. This
information consists of the following values:
- dAttrX - x attraction to the ball for this player type.
- dAtttY - y attraction to the ball for this player type.
- dMinX  - minimal x coordinate for this player
- dMaxX  - maximal x coordinate for this player
- bBehindBall - indicating whether this player type should always stay behind
the ball or not.


This class contains different get and set methods to change the values
associated for this class, normally these are changed when the
Formations class reads in the formation file. */
class PlayerTypeInfo
{
    PlayerT playerType; /*!< This class gives information about this PlayerType*/
    double  dAttrX;     /*!< x attraction to the ball                          */
    double  dAttrY;     /*!< y attraction to the ball                          */
    double  dMinX;      /*!< minimal x coordinate for this player type         */
    double  dMaxX;      /*!< maximal x coordinate for this player type         */
    bool    bBehindBall;/*!< should player always stay behind the ball         */

public:
    PlayerTypeInfo( );
    PlayerTypeInfo( PlayerT, double, double, double, double, bool );

    // method to set all the values at once and displaying them all
    bool    setValues( PlayerT, double, double, double, double, bool  );
    void    show     ( ostream &os = cout                            );

    // standard get and set methods to individually set all the values
    bool    setPlayerType    ( PlayerT type  );
    PlayerT getPlayerType    (               ) const;
    bool    setAttrX         ( double  attrX );
    double  getAttrX         (               ) const;
    bool    setAttrY         ( double  attrY );
    double  getAttrY         (               ) const;
    bool    setMinX          ( double  minX  );
    double  getMinX          (               ) const;
    bool    setMaxX          ( double  maxX  );
    double  getMaxX          (               ) const;
    bool    setBehindBall    ( bool    b     );
    bool    getBehindBall    (               ) const;

};


/*****************************************************************************/
/********************** CLASS FORMATIONTYPEINFO ******************************/
/*****************************************************************************/

/*!This class contains information about one specific formation. It
   contains the formation type (defined in SoccerTypes.h), the home
   position of all the roles (=specific player in a formation), the
   player types for all the roles and the information about the
   different player_types. Furthermore it contains methods to retrieve
   this information for a specific role. */
class FormationTypeInfo
{
    FormationT      formationType;               /*!< type of this formation   */
    VecPosition     posHome[ MAX_TEAMMATES ];    /*!< home position for roles  */
    PlayerT         playerType[ MAX_TEAMMATES ]; /*!< player_types for roles   */
    PlayerTypeInfo  playerTypeInfo[ MAX_PLAYER_TYPES ]; /*!< info for roles    */

public:
    FormationTypeInfo(                    );
    void show(         ostream &os = cout );

    // get and set methods to get information for a player in this formation
    bool            setFormationType         ( FormationT  type                );
    FormationT      getFormationType         (                            ) const;
    bool            setPosHome               ( VecPosition pos,     int atIndex);
    bool            setXPosHome              ( double      x,       int atIndex);
    bool            setYPosHome              ( double      y,       int atIndex);
    VecPosition     getPosHome               ( int         atIndex       ) const;
    bool            setPlayerType            ( PlayerT     type,    int atIndex);
    PlayerT         getPlayerType            ( int         atIndex       ) const;
    bool            setPlayerTypeInfo        ( PlayerTypeInfo info, int atIndex);
    PlayerTypeInfo* getPlayerTypeInfo        ( int         atIndex             );
    PlayerTypeInfo* getPlayerTypeInfoOfPlayer( int         iPlayerInFormation  );
};

/*****************************************************************************/
/********************** CLASS FORMATIONS *************************************/
/*****************************************************************************/

/*!This class is a container for all different Formation Types: it
   contains the information of all the formation types. Furthermore it
   contains two other values: the current formation type that is used
   by the agent and the role of the agent in the current
   formation. These two values fully specify the position of this
   player in the formation. */
class Formations
{
    FormationTypeInfo formations[ MAX_FORMATION_TYPES ]; /*!< stored formations*/
    FormationT        curFormation;       /*!< type of the current formation   */
    int               iPlayerInFormation; /*!< role agent in current formation */
public:
    Formations( const char *strFile = NULL, FormationT ft=FT_ILLEGAL, int iNr=1);
    void show ( ostream &os = cout );

    // method to get the strategic position depending on different factors
    VecPosition getStrategicPosition( int         iPlayer,
                                      VecPosition posBall,
                                      double      dMaxXInPlayMode,
                                      bool        bInBallPossession = false,
                                      double      dMaxYPercentage = 0.75,
                                      FormationT  ft = FT_ILLEGAL );

    // method to read the formations from a formation configuration file.
    bool        readFormations      ( const char *strFile         );

    // standard get and set methods for the different member variables
    bool        setFormation        ( FormationT formation            );
    FormationT  getFormation        (                                 ) const;
    bool        setPlayerInFormation( int number                      );
    int         getPlayerInFormation( ObjectT    obj = OBJECT_ILLEGAL ) const;
    PlayerT     getPlayerType       ( ObjectT    obj,
                                      FormationT ft = FT_ILLEGAL      ) const;
    PlayerT     getPlayerType       ( int        iIndex = -1,
                                      FormationT ft = FT_ILLEGAL      ) const;
};

#endif
