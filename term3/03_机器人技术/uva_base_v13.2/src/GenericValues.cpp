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

/*! \file GenericValues.cpp
<pre>
<b>File:</b>          GenericValues.cpp
<b>Project:</b>       Robocup Soccer Simulation Team: UvA Trilearn
<b>Authors:</b>       Jelle Kok
<b>Created:</b>       28/11/2000
<b>Last Revision:</b> $ID$
<b>Contents:</b>      Code file for classes GenericValueT and GenericValues.
               The class GenericValueT contains a pointer to a variable
               of a generic type (double, char*, bool, int). This pointer
               is associated with a string. The class GenericValues
               contains a collection of GenericValueT objects. The member
               method implementations for both classes can be found in
               this file.</pre>
<hr size=2>
<pre>
<h2><b>Changes</b></h2>
<b>Date</b>             <b>Author</b>          <b>Comment</b>
28/11/2000       Jelle Kok       Initial version created (based on Emiel Corten)
30/12/2000       Jelle Kok       Initial documentation written
21/05/2001       Remco de Boer   Version including full documentation completed
</pre>
*/

#include "GenericValues.h"
#include "Parse.h"           // needed for 'gotoFirstNonSpace'
#include <stdio.h>           // needed for 'sprintf'
#include <stdlib.h>          // needed for 'free'
#include <string.h>          // needed for 'strdup'
#include <ctype.h>           // needed for 'isdigit'
#include <fstream>           // needed for 'ifstream'

/*****************************************************************************/
/*******************   CLASS GENERICVALUET   *********************************/
/*****************************************************************************/

/*! Constructor for the GenericValueT class. It creates a GenericValueT object
    by setting all the private member variables to the values passed to the
    constructor.
    \param strName a string denoting the name associated with the generic
    variable to which the class pointer will point (the variable can be reached
    through this name)
    \param vAdress a (void) pointer to the actual generic variable
    \param t the (generic) type of the variable associated with the class
    pointer */
GenericValueT::GenericValueT( const char *str, void *vAddr, GenericValueKind t)
{
    m_strName  = strdup( str ); // strdup needed to alloc right amount of memory
    m_vAddress = vAddr;
    m_type     = t;
}

/*! Destructor for the GenericValueT class. It destroys a GenericValueT object
    by freeing all memory allocated to it. */
GenericValueT::~GenericValueT( )
{
    if ( m_strName )
        free( ( char * ) m_strName );
}

/*! Get method for the 'm_strName' member variable.
    \return the name associated with the variable to which the class pointer
    points */
const char* GenericValueT::getName ( )
{
    return ( m_strName );
}

/*! This method sets the variable associated with this GenericValueT
    object to the value indicated by the given string argument. The
    value is always supplied as a string which is then converted into
    the right type for this GenericValueT object.

    \param strValue a string denoting the value to which the variable
    associated with this GenericValueT object must be set

    \return a boolean indicating whether the update was successful */
bool GenericValueT::setValue( const char *strValue )
{
    bool bReturn = true, b;

    // determine the generic type associated with this GenericValueT
    // object and apply the correct conversion of the string argument
    switch ( m_type )
    {
    case GENERIC_VALUE_DOUBLE:
        *( double * ) m_vAddress = atof( strValue ? strValue : 0 );
        break;
    case GENERIC_VALUE_STRING:
        strcpy( ( char * ) m_vAddress, strValue );
        break;
    case GENERIC_VALUE_BOOLEAN:
        b = false;
        if ( !strValue )
            ;
        else if ( isdigit( strValue[ 0 ] ) )
            b = atoi( strValue ? strValue : 0 );
#ifdef WIN32
        else if ( strcmp( strValue, "on"  ) == 0 ||
                  strcmp( strValue, "true") == 0 ||
                  strcmp( strValue, "yes" ) == 0 )
#else
        else if ( strcasecmp( strValue, "on"  ) == 0 ||
                  strcasecmp( strValue, "true") == 0 ||
                  strcasecmp( strValue, "yes" ) == 0 )
#endif
            b = true;

        *( bool * ) m_vAddress = ( b == true ) ? true : false;
        break;
    case GENERIC_VALUE_INTEGER:
        *( int * ) m_vAddress = atoi( strValue ? strValue : 0 );
        break;
    default:
        bReturn = false;
    }

    return ( bReturn );
}

/*! This method determines the value of the variable associated with
    this GenericValueT object. The result is converted to a string
    which is put into the argument to the method (note that enough
    memory must be allocated for this char*). This same string is also
    returned by the method. This enables you to use the method as an
    argument to a function such as 'strcpy'.

    \param strValue a string which after the method call will contain
    the value of the variable associated with this GenericValueT
    object

    \return a string containing the value of the variable associated
    with this GenericValueT object. */
char* GenericValueT::getValue( char *strValue )
{
    // determine the generic type associated with this GenericValueT
    // object and apply the correct conversion into a string
    switch ( m_type )
    {
    case GENERIC_VALUE_DOUBLE:
        sprintf( strValue, "%2f", *( double * ) m_vAddress );
        break;
    case GENERIC_VALUE_STRING:
        sprintf( strValue, "%s", ( char * ) m_vAddress );
        break;
    case GENERIC_VALUE_BOOLEAN:
        sprintf( strValue, "%d", *( int * ) m_vAddress );
        break;
    case GENERIC_VALUE_INTEGER:
        sprintf( strValue, "%d", *( int * ) m_vAddress );
        break;
    default:
        *strValue = '\0';
    }

    return ( strValue );
}

/*! This display method writes the name associated with the variable
    in this GenericValueT object together with its value to the output
    stream indicated by the first argument to the method. Name and
    value are separated by the separator given as the second
    argument. Note that boolean values are written as 'true' and
    'false'.

    \param out the output stream to which the information should be
    written

    \param strSeparator a string representing the separator which
    should be written between the name associated with the variable
    and its value */
void GenericValueT::show( ostream& out, const char *strSeparator )
{
    // write the name associated with the variable in this GenericValueT
    // object followed by the separator to the specified output stream
    out << m_strName << strSeparator;

    // determine the generic type associated with this GenericValueT
    // object and write the correct value to the specified output stream
    switch ( m_type )
    {
    case GENERIC_VALUE_DOUBLE:
        out << *( double * ) m_vAddress;
        break;
    case GENERIC_VALUE_STRING:
        out << ( char * ) m_vAddress;
        break;
    case GENERIC_VALUE_BOOLEAN:
        out << ( ( *( bool * ) m_vAddress == true ) ? "true" : "false");
        break;
    case GENERIC_VALUE_INTEGER:
        out << *( int * ) m_vAddress;
        break;
    default:
        break;
    }

    out << endl;
}


/*****************************************************************************/
/*******************   CLASS GENERICVALUES   *********************************/
/*****************************************************************************/

/*! Constructor for the GenericValues class. It creates a GenericValues object.

    \param strName a string denoting the name associated with this group of
    generic values (this is usually the name of the subclass which contains the
    actual generic variables)
    \param iMaxValues an integer denoting the number of generic values in the
    current collection, i.e. the maximum number that can be stored */
GenericValues::GenericValues( char *strName, int iMaxValues )
{
    m_iValuesTotal = 0;   // total number of values in collection is set to zero

    if ( strName )      // set the name of the collection
        m_strClassName = strdup( strName );

    m_iMaxGenericValues = iMaxValues;

    // a GenericValues object is a collection of GenericValueT objects
    m_values = new GenericValueT*[ iMaxValues ];
}

/*! Destructor for the GenericValues class. It destroys a GenericValues object
    by freeing all memory allocated to it. */
GenericValues::~GenericValues( void )
{
    for ( int i = 0 ; i < getValuesTotal( ) ; i++ )
        delete m_values[ i ];
    delete m_values;

    if ( m_strClassName )
        free( m_strClassName );
}

/*! Get method for the 'm_strClassName' member variable.
    \return the name associated with this group of generic values (this is
    usually the name of the subclass which contains the actual variables) */
char* GenericValues::getClassName( )
{
    return ( m_strClassName );
}

/*! Get method for the 'm_iValuesTotal' member variable.
    \return the number of generic values stored in the collection so far */
int GenericValues::getValuesTotal( )
{
    return ( m_iValuesTotal );
}

/*! This method adds a new generic value to the collection and
    provides a connection between a string name and the variable name
    of the generic value.

    \param strName a string denoting the name associated with the
    variable of the generic value which is added (the variable can be
    reached through this name)

    \param vAddress a (void) pointer to the actual variable itself

    \param type the (generic) type of the variable which is added

    \return a boolean indicating whether the generic value has been succesfully
    added */
bool GenericValues::addSetting( const char *strName, void *vAddress,
                                GenericValueKind type )
{
    if ( getValuePtr( strName ) )              // setting already installed
    {
        cerr << "Setting '" << strName << "' already installed." << endl;
        return false;
    }
    if ( m_iValuesTotal == m_iMaxGenericValues )   // buffer is full
    {
        cerr << "GenericValues::addSetting buffer for " << m_strClassName <<
             " is full (cannot add '" << strName << "')" << endl;
        return false;
    }

    m_values[ m_iValuesTotal++ ] = new GenericValueT( strName, vAddress, type );

    return ( true );
}

/*! This (private) method returns a pointer to the GenericValueT
    object of which the name associated with the variable matches the
    argument passed to the method.

    \param strName a string denoting the name associated with the
    variable of the GenericValueT object to which a pointer should be
    returned

    \return a pointer to the GenericValueT object of which the name
    associated with the variable matches the argument; 0 when a
    GenericValueT object with a variable associated with the given
    name does not exist */
GenericValueT* GenericValues::getValuePtr( const char *strName )
{
    GenericValueT *ptr = 0;

    // search through the collection for a GenericValueT object of which the name
    // associated with the variable matches the argument passed to the method
    for ( int i = 0 ; i < getValuesTotal( ) ; i++ )
    {
        if ( strcmp( m_values[ i ]->getName( ), strName ) == 0 )
        {
            ptr = m_values[ i ];
            break;
        }
    }

    return ( ptr );
}

/*! This method determines the value of the variable of which the name
    associated with it matches the first argument to the method. The value is
    converted to a string which is put into the second argument (note that
    enough memory must be allocated for this char*). This same string is also
    returned by the method. This enables you to use the method as an argument
    to a function such as 'strcpy'.
    \param strName a string denoting the name associated with the variable of
    which the value should be returned
    \param strValue a string which after the method call will contain the value
    of the variable of which the name associated with it matches the first
    argument to the method
    \return a string containing the value of the variable of which the name
    associated with it matches the first argument to the method */
char* GenericValues::getValue( const char *strName, char *strValue )
{
    GenericValueT *parptr;

    parptr = getValuePtr( strName );

    if ( parptr )
        strValue = parptr->getValue( strValue );   // this method returns a string
    else
        strValue[ 0 ] = '\0';

    return ( strValue );
}

/*! This method sets the variable of which the name associated with it
    matches the first argument to the method to the value indicated by
    the second argument. The value is always supplied as a string
    which is then converted into the right type for the generic value
    in question.

    \param strName a string denoting the name associated with the
    variable which must be set

    \param strValue a string denoting the value to which the variable
    indicated by the first argument should be set

    \return a boolean indicating whether the update was successful */
bool GenericValues::setValue( const char *strName, const char *strValue )
{
    bool bReturn = false;
    GenericValueT *parptr;

    parptr = getValuePtr( strName );

    if ( parptr )
        bReturn = parptr->setValue( strValue ); // string is converted to its type

    return ( bReturn );
}

/*! This method reads generic values from a file indicated by the
    first argument to the method and stores them in the collection. In
    this file the names associated with each value must be listed
    first followed by a separator and then the value. The names
    associated with each value should match the string names with
    which the corresponding generic value is added using 'addSetting'.

    \param strFile a string denoting the name of the file from which the values
    must be read
    \param strSeparator a string representing the separator which is written
    between name and value in the configuration file
    \return a boolean indicating whether the values have been read correctly */
bool GenericValues::readValues( const char *strFile, const char *strSeparator )
{
    ifstream in( strFile );

    if ( !in )
    {
        cerr << "(GenericValues::readValues) Could not open file '" <<
             strFile << "'" << endl;
        return ( false );
    }

    bool  bReturn = true;
    char  strLine[ 256 ], strName[ 100 ], strValue[ 100 ];
    char* c;
    int   iLineNr = 0;

    // read each line in the configuration file and store the values
    while ( in.getline( strLine, sizeof( strLine ) ) )
    {
        iLineNr++;

        // skip comment lines and empty lines; " #" is for server.conf version 7.xx
        if ( !( strLine[ 0 ] == '\n' ||
                strLine[ 0 ] == '#'  ||
                strLine[ 0 ] == '\0' ||
                ( strlen( strLine ) > 1 &&
                  strLine[ 0 ] == ' ' &&
                  strLine[ 1 ] == '#' ) ) )
        {
            // convert all characters belonging to the separator to spaces
            if ( strSeparator && ( c = strstr( strLine, strSeparator ) ) != NULL )
                for ( size_t i = 0; i < strlen( strSeparator ); i++ )
                    *( c + i ) = ' ';

            // read the name and value on this line and store the value
            if ( !( sscanf( strLine, "%s%s", strName, strValue ) == 2 &&
                    setValue( strName, strValue ) ) )
            {
                bReturn = false;
                cerr << "(GenericValues::readValues) '" << strFile << "' linenr "
                     << iLineNr << ",  error in '" << strLine << "'" << endl;
            }
        }
    }

    return ( bReturn );
}

/*! This method writes the generic values in the current collection to
    a file indicated by the first argument to the method. The string
    name associated with the value and the value itself are separated
    by a separator which is specified as the second argument.

    \param strFile a string denoting the name of the file to which the
    generic values should be written

    \param strSeparator a string representing the separator which
    should be written between the name associated with each value and
    the value itself

    \param bAppend a boolean indicating whether the values should be
    appended to the given file (=true) or if current contents of the
    file should be overwritten (=false)

    \return a boolean indicating whether the values in the current
    collection have been correctly written to the given file */
bool GenericValues::saveValues( const char *strFile, const char *strSeparator,
                                bool bAppend )
{
    ofstream outf( strFile, ( bAppend == false ? ( ios::out )
                              : ( ios::out | ios::app ) ) );

    if ( !outf )
    {
        cerr << "Could not open file '" << strFile << "'" << endl;
        return ( false );
    }

    // values are written to file using 'show' (note that
    // output stream to write to is a file in this case)
    show( outf, strSeparator );

    return ( true );
}

/*! This display method writes all the generic values in the current
    collection to the output stream indicated by the first argument to
    the method. The name associated with each value and the value
    itself are separated by a separator which is specified as the
    second argument.

    \param out the output stream to which the current collection of
    generic values should be written

    \param strSeparator a string representing the separator which
    should be written between the name associated with each value and
    the value itself */
void GenericValues::show( ostream& out, const char *strSeparator )
{
    for ( int i = 0; i < getValuesTotal( ); i++ )
        m_values[ i ]->show( out, strSeparator );
}

/********************* TESTING PURPOSES **************************************/
/*
int main( void )
{
  GenericValues g("ServerSettings", 6);

  int    i = 10;
  char   c[256];
  sprintf( c, "hallo" );
  bool   b = true;
  double d = 10.14;

  cout << g.addSetting( "var1", &i, GENERIC_VALUE_INTEGER ) << endl;
  cout << g.addSetting( "var2", c, GENERIC_VALUE_STRING  ) << endl;
  cout << g.addSetting( "var3", &d, GENERIC_VALUE_DOUBLE  ) << endl;
  cout << g.addSetting( "var4", &b, GENERIC_VALUE_BOOLEAN ) << endl;
  g.show( cout, ":"  );

  g.setValue( "var1", "11"      );
  g.setValue( "var2", "hoi"     );
  g.setValue( "var3", "20.2342" );
  g.setValue( "var4", "false"   );
  g.show( cout, ":"  );

//  g.setIntegerValue( "var1", 22      );
//  g.setStringValue ( "var2", "hoi2"  );
//  g.setDoubleValue ( "var3", 30.2342 );
//  g.setBooleanValue( "var4", true    );
//  g.show( cout, ":"  );

//  g.readValues( "server.conf", ":" );
//  g.show( cout, ":"  );

  return ( 0 );
}

*/
