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

/*! \file Connection.cpp
<pre>
<b>File:</b>          Connection.cpp
<b>Project:</b>       Robocup Soccer Simulation Team: UvA Trilearn
<b>Authors:</b>       Jelle Kok
<b>Created:</b>       23/11/2000
<b>Last Revision:</b> $ID$
<b>Contents:</b>      This file contains the class definitions for the
               Connection class which sets up a connection with the
               soccer server.
<hr size=2>
<h2><b>Changes</b></h2>
<b>Date</b>             <b>Author</b>          <b>Comment</b>
23/11/2000       Jelle Kok       Initial version created
</pre>
*/

#include <errno.h>       // EWOULDBLOCK
#include <string.h>      // strlen
#include <sys/types.h>   // socket

#ifndef WIN32
#include <unistd.h>      // close
#include <sys/socket.h>  // socket
#endif

#ifdef Solaris
#include <sys/socket.h>  // AF_INET SOCK_DGRAM
#endif

#include "Connection.h"
#include "Logger.h"      // LOG
//#include<iostream>
using namespace std;
extern Logger Log; /*!< This is a reference to the Logger for writing info to*/

/*****************************************************************************/
/********************** CONNECTION *******************************************/
/*****************************************************************************/

/*! Default constructor. Only sets the maximum message size. */
Connection::Connection( )
{
    m_iMaxMsgSize = 2048;
}

/*! Constructor makes a connection with the server using the connect method.
    \param hostname string representation of host machine (string or IP number)
    \param port port number for connection of the host machine
    \param iMaxSize maximum message size that can be sent or received */
Connection::Connection(const char *hostname, int port, int iMaxSize)
{
    m_iMaxMsgSize = iMaxSize;
    if (  connect( hostname, port ) )
        Log.log( 1,"(Connection:connection) Socket connection made with %s:%d",
                 hostname, port );
    else
        Log.log( 1,"(Connection:Connection) Could not create connection with %s:%d"
                 , hostname, port );
}

/*! Deconstructor closes the connection with the server */
Connection::~Connection()
{
    disconnect();
}

/*! This method sets up a connection with the server.
    \param hostname string representation of host machine (string or IP number)
    \param port port number for connection of the host machine
    \return bool indicating whether connection was made */
bool Connection::connect(const char *host, int port )
{
    struct hostent *host_ent;
    struct in_addr *addr_ptr;
    struct sockaddr_in  cli_addr ;
    int    sockfd ;

    m_sock.socketfd = -1 ;

#ifndef WIN32
    if ( (host_ent = (struct hostent*)gethostbyname(host)) == NULL)
    {
        // if not a string, get information from IP adress.
#ifdef Solaris
        if ( inet_addr(host) == ((in_addr_t)-1) )
#else
        if ( inet_addr(host) == INADDR_NONE )
#endif
        {
            cerr << "(Connection::connect) Cannot find host " << host << endl;
            return false ;
        }
    }
    else   // get the necessary information from the hostname (string)
    {
        addr_ptr = (struct in_addr *) *host_ent->h_addr_list;
        host = inet_ntoa(*addr_ptr);
    }
#else
    // winsock initialization
    WORD     wVersionRequested;
    WSADATA  wsaData;
    wVersionRequested = MAKEWORD( 2, 2 );

    if ( WSAStartup( wVersionRequested, &wsaData ) != 0 )
        return false;

    if ( LOBYTE( wsaData.wVersion ) != 2 ||
            HIBYTE( wsaData.wVersion ) != 2 ) {
        WSACleanup();
        return false;
    }

    if (inet_addr(host) == INADDR_NONE) {
        if ((host_ent = (struct hostent *)gethostbyname(host)) == NULL) {
            return false;
        } else {
            addr_ptr = (struct in_addr *) *host_ent->h_addr_list ;
            host = inet_ntoa(*addr_ptr) ;
        }
    }
#endif
    //  Open UDP socket.
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        cerr << "(Connection::connect) Cannot create socket " << host << endl;
        return false ;
    }

    // insert the information of the client
    cli_addr.sin_family      = AF_INET ;
    cli_addr.sin_addr.s_addr = htonl(INADDR_ANY) ;
    cli_addr.sin_port        = htons(0) ;

    // bind the client to the server socket
    if (bind(sockfd, (struct sockaddr *) &cli_addr, sizeof(cli_addr)) < 0)
    {
        cerr << "(Connection::connect) Cannot bind local address " << host << endl;
        return false ;
    }

    //  Fill in the structure with the address of the server.
    m_sock.socketfd = sockfd ;

    m_sock.serv_addr.sin_family       = AF_INET ;
    m_sock.serv_addr.sin_addr.s_addr  = inet_addr(host);
    m_sock.serv_addr.sin_port         = htons(port) ;

    return true;
}

/*! This method closes the current socket connection.  */
void Connection::disconnect( void )
{
    if (isConnected() )
    {
#ifdef WIN32
        closesocket( m_sock.socketfd );
#else
        close(m_sock.socketfd) ;
#endif
        m_sock.socketfd = -1; // This also 'sets' isConnected() to false
    }
}

/*! This method determines whether the socket connection is connected.
    \return bool indicating whether socket connection is available */
bool Connection::isConnected(void) const
{
    return(m_sock.socketfd != -1);
}

/*! This method reads a message from the connection. When there is
    no message available, it blocks till it receives a message.
    \param msg string in which message is stored
    \param maxsize maximum size of the message.
    \return -1: error, 0: 0 bytes were read, 1 when read was succesfull */
int Connection::receiveMessage( char *msg, int maxsize )
{
#ifdef WIN32
    int       servlen;
#elif Solaris
    int       servlen;
#else
    socklen_t servlen ;
#endif
    int n;
    struct sockaddr_in serv_addr ;

    servlen = sizeof(serv_addr) ;

    // receive message from server
    n = recvfrom(m_sock.socketfd, msg, maxsize, 0,
                 (struct sockaddr *)&serv_addr, &servlen);

    if (n < 0)                                  // error
    {
#ifdef WIN32
        if ( n == -1 && errno == WSAEWOULDBLOCK)
#else
        if ( n == -1 && errno == EWOULDBLOCK)
#endif
        {
            msg[0] = '\0' ;
            return 0 ;
        }
        else
            return -1;
    }
    else                                       // succesfull, set new server info
    {                                          // next message will go to there
        m_sock.serv_addr.sin_port = serv_addr.sin_port ;
        msg[n] = '\0' ;

        return ( n == 0 ) ? 0 : 1 ;
    }
}

/*! This method sends a message to the server using the current connection.
    \param msg string which contains message
    \return true on succes, false in case of failure */
bool Connection::sendMessage( const char *msg )
{
    int n;

    n = strlen(msg) + 1 ;
    if ( sendto(m_sock.socketfd, msg, n, 0,
                (struct sockaddr *)&m_sock.serv_addr, sizeof(m_sock.serv_addr)) != n )
        return false ;
    return true ;
}

/*! This method always loops and waits for input. When input is received from
    fpin then this input is send to the server using the current connection.
    When message is received from the server, this message is sent to fpout.
    \param fpin file pointer from which input is read (i.e. stdin )
    \param fpout file pointer to which output should be directed (i.e. stdout)
    \return 0 but only when error occured in reading input */
#ifndef WIN32
int Connection::message_loop( FILE *fpin, FILE *fpout )
{
    fd_set readfds, readfds_bak;
    int in, max_fd, n, ret;
    char buf[m_iMaxMsgSize];

    in = fileno( fpin );
    FD_ZERO( &readfds );
    FD_SET( in, &readfds );
    readfds_bak = readfds;
    max_fd = ((in > m_sock.socketfd) ? in : m_sock.socketfd) = 1;

    while ( 1 )
    {
        readfds = readfds_bak;
        // wait for message from socket or fpin
        if (( ret = select( max_fd, &readfds, NULL, NULL, NULL )) < 0 )
        {
            perror("select");
            break;
        }
        else if ( ret != 0 )
        {
            if ( FD_ISSET(in, &readfds))              // file descriptor fpin was set
            {
                fgets(buf, m_iMaxMsgSize, fpin);        // get the message
                if ( sendMessage(buf ) == false )       // and send it
                    break;
            }
            if ( FD_ISSET(m_sock.socketfd, &readfds ) )// file descriptor was set
            {
                n = receiveMessage(buf, m_iMaxMsgSize); // receive the message
                if ( n == -1 )
                    break;
                else if ( n > 0 )                       // and put it to fpout
                {
                    fputs(buf, fpout);
                    fputc( '\n', fpout);
                }
                fflush(stdout);
            }
        }
    }
    return 0;
}
#endif


/*! This method prints whether the connection is up or not.
    \param os output stream to which output should be directed */
void Connection::show( ostream os )
{
    if ( ! isConnected() )
        os << "Not connected" << endl;
    else
        os << "Connected" << endl;

}

/********************** TESTING PURPOSES *************************************/

/*
int main( void )
{
  char strBuf[m_iMaxMsgSize];
  Connection c( "localhost", 6000 );
  int i;
  c.sendMessage( "(init l (version 6.07))" );
  while( 1 )
  {
    i = c.receiveMessage( strBuf, m_iMaxMsgSize );
    printf("%d|%s|\n", i, strBuf );
  }
  return 0;
}
*/


