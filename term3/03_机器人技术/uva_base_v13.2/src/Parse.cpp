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

/*! \file Parse.cpp
<pre>
<b>File:</b>          Parse.cpp
<b>Project:</b>       Robocup Soccer Simulation Team: UvA Trilearn
<b>Authors:</b>       Jelle Kok
<b>Created:</b>       12/02/2001
<b>Last Revision:</b> $ID$
<b>Contents:</b>      class declarations of Parse class that can be used to
               quickly parse specific string messages.
<hr size=2>
<h2><b>Changes</b></h2>
<b>Date</b>             <b>Author</b>          <b>Comment</b>
12/02/2001       Jelle Kok       Initial version created
</pre>
*/

#include "Parse.h"

#include <ctype.h>    // needed for isdigit
#include <math.h>     // needed for pow
#include <string.h>   // needed for strlen

/*****************************************************************************/
/********************** CLASS PARSE ******************************************/
/*****************************************************************************/

/*! This method walks through the string starting at the character where strMsg
    points to and stops when or the end of the string is reached or a non space
    is reached. strMsg is updated to this new position.
    \param strMsg pointer to a character in a string array
    \return character that is at the first non space, '\0' when end of string
    is reached. */
char Parse::gotoFirstNonSpace( char** strMsg )
{
    while (*strMsg && isspace(**strMsg) )
        (*strMsg)++;
    return  (*strMsg) ?  **strMsg : '\0';

}

/*! This method walks through the string starting at the character where strMsg
    points to and stops when<BR>
    - the end of the string is reached<BR>
    - a character different than ' ' and ')' is read<BR>
    strMsg is changed after this method is called.
    \param strMsg pointer to a character in a string array
    \return first character that is not equal to ' ' or ')', '\0' when string
            didn't contain such a character. */
char Parse::gotoFirstSpaceOrClosingBracket( char** strMsg )
{
    while ( *strMsg && **strMsg!=' ' && **strMsg!=')' )
        (*strMsg)++;
    return  (*strMsg) ?  **strMsg : '\0';
}

/*! This method walks through the string starting at the character where strMsg
    points to and stops when the character c is reached. strMsg is changed
    after this method is called..
    \param c character that is searched for in strMsg.
    \param strMsg pointer to a character in a string array
    \return number of character skipped to reach c, -1 when not found */
int Parse::gotoFirstOccurenceOf( char c, char** strMsg )
{
    int i=0;
    while (**strMsg && **strMsg != c )
    {
        i++;
        (*strMsg)++;
    }
    if ( ! **strMsg )
        return -1;
    return i;
}

/*! This method walks through the string starting at the character where strMsg
    points to and returns the first integer that can be read from this string.
    Any other characters in front of this integer are discarded. After this
    method is returned, strMsg points to the first character after the final
    value of the integer.
    \param strMsg pointer to a character in a string array
    \return first integer that can be read from this string. */
int Parse::parseFirstInt( char** strMsg )
{
    int  iRes  = 0;
    bool bIsMin= false;
    char *str  = *strMsg;

    while ( *str != '\0' && !isdigit(*str) && *str!='-')
        str++;                             // walk to first non digit or minus sign


    if ( *str != '\0' && *str=='-')      // if it was a minus sign, remember
    {
        bIsMin=true;
        str++;
    }

    while ( *str != '\0' && *str<='9' && *str>='0' )  // for all digits
    {
        iRes=iRes*10+(int)(*str-'0');                 // multiply old res with 10
        str++;                                        // and add new value
    }
    *strMsg = str;
    return (bIsMin) ? -iRes : iRes;
}

/*! This method walks through the string starting at the character where strMsg
    points to and returns the first double that can be read from this string.
    Any other characters in front of this integer are discarded. After this
    method is returned, strMsg points to the first character after the final
    value of the double. 4e-3, and NaN or nan are also recognized. When input
    contains NaN or nan, -1000.0 is returned.
    \param strMsg pointer to a character in a string array
    \return first double that can be read from this string. */
double Parse::parseFirstDouble( char** strMsg )
{
    double dRes=0.0, dFrac=1.0;
    bool bIsMin=false, bInDecimal=false, bCont=true;
    char *str = *strMsg;

    // go to first part of double (digit, minus sign or '.')
    while ( *str != '\0' && !isdigit(*str) && *str!='-' && *str!='.')
    {
        // when NaN or nan is double value, return -1000.0
        if ( (*str=='N' && strlen(str)>3 && *(str+1)=='a' && *(str+2)=='N') ||
                (*str=='n' && strlen(str)>3 && *(str+1)=='a' && *(str+2)=='n') )
        {
            *strMsg = str+3;
            return -1000.0;
        }
        else
            str++;
    }

    if ( *str != '\0' && *str=='-')              // if minus sign, remember that
    {
        bIsMin=true;
        str++;
    }

    while ( *str != '\0' && bCont)               // process the number bit by bit
    {
        if ( *str=='.' )                           // in decimal part after '.'
            bInDecimal = true;
        else if ( bInDecimal && *str<='9' && *str>='0') // number and in decimal
        {
            dFrac=dFrac*10.0;                        // shift decimal part to right
            dRes += (double)(*str-'0')/dFrac;        // and add number
        }
        else if ( *str<='9' && *str>='0' )         // if number and not decimal
            dRes=dRes*10+(double)(*str-'0');         // shift left and add number
        else if ( *str=='e' ) // 10.6e-08           // if to power e
        {
            if ( *(str+1) == '+' )                   // determine sign
                dRes *=  pow(10, parseFirstInt(&str)); // and multiply with power
            else if ( *(str+1) == '-' )
            {
                str = str+2;                           // skip -
                dRes /=  pow(10, parseFirstInt(&str)); // and divide by power
            }
            bCont = false;                           // after 'e' stop
        }
        else
            bCont = false;                           // all other cases stop

        if ( bCont == true )                       // update when not stopped yet
            str++;
    }
    *strMsg = str;
    return (bIsMin && dRes != 0.0) ? -dRes : dRes;
}

/*****************************************************************************/
/********************* TESTING PURPOSES **************************************/
/*****************************************************************************/
/*
#include<iostream.h>

int main( void )
{
  double d = 13.6e+15;
  char str[] = "13.6e+15";
  double d2 = 13.6e-15;
  char str2[] = "13.6e-15";

  char *strmsg;
  strmsg = &str[0];
  cout  << d << endl;
  cout << Parse::parseFirstDouble( &strmsg )<< endl;
  strmsg = &str2[0];
  cout  << d2 << endl;
  cout << Parse::parseFirstDouble( &strmsg ) << endl;
  return 0;
}
*/
