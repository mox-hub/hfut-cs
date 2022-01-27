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

/*! \file Connection.h
<pre>
<b>File:</b>          Connection.h
<b>Project:</b>       Robocup Soccer Simulation Team: UvA Trilearn
<b>Authors:</b>       Jelle Kok
<b>Created:</b>       23/11/2000
<b>Last Revision:</b> $ID$
<b>Contents:</b>      This file contains the class declarations for the
               Connection class which sets up a connection with the
               soccer server.
<hr size=2>
<h2><b>Changes</b></h2>
<b>Date</b>             <b>Author</b>          <b>Comment</b>
23/11/2000      Jelle Kok       Initial version created
</pre>
*/

#ifndef _CONNECTION_
#define _CONNECTION_

#include <stdio.h>     // printf, FILE
#include <iostream>    // ostream
#ifdef WIN32
#include <windows.h>   // sockaddr_in
#else
#include <netdb.h>     // sockaddr_in
#include <arpa/inet.h> // sockaddr_in
#endif

using namespace std;

/*! Socket is a combination of a filedescriptor with a server adress.        */
typedef struct _socket {
    int    socketfd  ;                /*!< File descriptor of the socket.    */
    struct sockaddr_in  serv_addr ;   /*!< Server information of the socket. */
} Socket ;

/********************** CONNECTION *******************************************/

/*! This class creates creates a (socket) connection using a hostname and a
    port number. After the connection is created it is possible to send and
    receive messages from this connection. It is based on the client program
    supplied with the soccer server defined in client.c and created by
    Istuki Noda et al.*/
class Connection {

    Socket         m_sock;        /*!< communication protocol with the server. */
    int            m_iMaxMsgSize; /*!< max  message size for send and receive  */
public:

    // constructors
    Connection            (                                               );
    Connection            ( const char *hostname, int  port,   int iSize  );
    ~Connection           (                                               );

    // methods that deal with the connection itself
    bool connect          ( const char *host,     int  port               );
    void disconnect       ( void                                          );
    bool isConnected      ( void                                          )const;

    // methods that deal with the communication over the connection
#ifndef WIN32
    int  message_loop     ( FILE       *in,       FILE *out               );
#endif
    int  receiveMessage   ( char       *msg,      int  maxsize            );
    bool sendMessage      ( const char *msg                               );

    void show             ( ostream    os                                 );

};

#endif
