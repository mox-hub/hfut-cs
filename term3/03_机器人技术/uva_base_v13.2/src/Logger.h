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

/*! \file Logger.h
<pre>
<b>File:</b>          Logger.h
<b>Project:</b>       Robocup Soccer Simulation Team: UvA Trilearn
<b>Authors:</b>       Jelle Kok
<b>Created:</b>       3/3/2000
<b>Last Revision:</b> $ID$
<b>Contents:</b>      This file contains the class to log information about the
               system to any output stream. A range can be specified
               for which the received log information is printed.
               Furthermore it is possible to print the time since the
               timer of the Logger has been restarted.
<hr size=2>
<h2><b>Changes</b></h2>
<b>Date</b>             <b>Author</b>          <b>Comment</b>
3/3/2001         Jelle Kok       Initial version created
</pre>
*/

#ifndef _LOGGER_
#define _LOGGER_

#include <iostream>   // needed for ostream (logging to output stream)
#include <fstream>    // needed for fstream (logging to file)
#include <string>     // needed for string
#include <iomanip>    // needed for setw
#include <set>        // needed for set

#ifdef WIN32
#include <windows.h>  // needed for DWORD and GetTickCount() function
#include <time.h>     // needed for time_t
#else
#include <sys/time.h> // needed for timeval
#endif

using namespace std;

#define MAX_LOG_LINE 3072 /*!< maximum size of a log message */
#define MAX_HEADER   128  /*!< maximum size of the header    */

/*****************************************************************************/
/*********************** CLASS TIMING ****************************************/
/*****************************************************************************/

/*!This class holds a timer. This timer can be set (restartTime) and text can
   be printed with the elapsed time since the timer was restarted.. */
class Timing
{
#ifdef WIN32
    DWORD  time1;                  /*! < the number of milliseconds that have
                                       elapsed since the Windows was started */
#else
    struct timeval time1;          /*!< Time the timer has last been restarted.*/
#endif

public:
    // methods to restart the timer, get the elapsed time and print messages
#ifdef WIN32
    static double getTimeDifference ( DWORD tv1, DWORD tv2 );
#else
    static double getTimeDifference ( struct   timeval t1,
                                      struct   timeval t2 );
#endif
    void          printTimeDiffWithText ( ostream& os,
                                          char     *str,
                                          int      iFactor = 1000 );
    double        getElapsedTime ( int      iFactor = 1 );
    void          restartTime ( );
} ;


/*****************************************************************************/
/**************************** LOGGER *****************************************/
/*****************************************************************************/

/*! This class makes it possible to log information on different abstraction
    levels. All messages are passed to the log method 'log' with a level
    indication. When it has been specified that this level should be logged
    using either the 'addLogLevel' or 'addLogRange' method
    the message is logged, otherwise it is ignored. This makes it
    possible to print only  the information you are interested in.
    There is one global Log class which is used by all classes that use the
    Logger. This instantation of the Logger is located in the file Logger.C and
    is called 'Log'. All classes that want use this Logger should make a
    reference to it using the line 'extern Logger Log;' and can then use
    this Logger with the Log.log( ... ) methods. Furthermore the Logger also
    contains a timer with makes it possible to print the time since the timer
    has been restarted. */
class Logger
{
    Timing   m_timing;               /*!< timer to print timing information    */
    char     m_buf[MAX_LOG_LINE];    /*!< buffer needed by different methods   */
    set<int> m_setLogLevels;         /*!< set that contains all log levels     */

    pthread_mutex_t mutex_stream;
    char     m_strHeader[MAX_HEADER];/*!< header string printed before msg     */
    ostream* m_os;                   /*!< output stream to print messages to   */
    string   m_strSignal;            /*!< temporary string for other messages  */

public:
    Logger ( ostream& os=cout, int iMinLogLevel=0, int iMaxLogLevel = 0 );

    // different methods associated with logging messages
    bool     log ( int         iLevel, string str );
    bool     log ( int         i,      const char   *str, ... );
    bool     logWithTime ( int         iLevel, const char   *str, ... );
    bool     logFromSignal ( int         iLevel, const char   *str, ... );
    bool     logSignal ( );

    void     restartTimer ( );
    Timing   getTiming ( );
    bool     isInLogLevel ( int         iLevel );

    bool     addLogLevel ( int         iLevel );
    bool     addLogRange ( int         iMin,   int iMax );

    char*    getHeader ( );
    bool     setHeader ( const  char        *str );
    bool     setHeader ( int         i );
    bool     setHeader ( int         i1,     int    i2 );

    bool     setOutputStream ( ostream&    os );
    ostream& getOutputStream ( );
    void     showLogLevels ( ostream&    os );
};


#endif
