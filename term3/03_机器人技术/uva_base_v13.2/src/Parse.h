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

/*! \file Parse.h
<pre>
<b>File:</b>          Parse.h
<b>Project:</b>       Robocup Soccer Simulation Team: UvA Trilearn
<b>Authors:</b>       Jelle Kok
<b>Created:</b>       12/02/2001
<b>Last Revision:</b> $ID$
<b>Contents:</b>      Header file for the Parse class. It contains methods
               which can be used for parsing string messages.
<hr size=2>
<h2><b>Changes</b></h2>
<b>Date</b>             <b>Author</b>          <b>Comment</b>
12/02/2001       Jelle Kok       Initial version created
09/06/2001       Remco de Boer   Version including full documentation completed
</pre>
*/

#ifndef _PARSE_
#define _PARSE_

/*****************************************************************************/
/********************   CLASS PARSE   ****************************************/
/*****************************************************************************/

/*! This class contains several static methods which can be used for parsing
    string messages and uses some of the ideas contained in CMU'99 source code
    of Peter Stone. Tests shows that scanning integers has a performance
    increase of 30.3% over the method used by CMUnited and 68.0% over sscanf.
    For parsing doubles the performance increase was 15.4% compared to CMUnited
    and 85.1% compared to sscanf. */
class Parse
{
public:

    // methods which return a specific type of value from a string message
    static double parseFirstDouble              ( char** strMsg                );
    static int    parseFirstInt                 ( char** strMsg                );

    // methods which move to a specific position in a string message
    static char   gotoFirstSpaceOrClosingBracket( char** strMsg                );
    static int    gotoFirstOccurenceOf          ( char   c      , char** strMsg);
    static char   gotoFirstNonSpace             ( char** strMsg                );
};

#endif
