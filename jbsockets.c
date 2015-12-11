const char jbsockets_rcs[] = "$Id: jbsockets.c,v 1.41 2006/11/13 19:05:51 fabiankeil Exp $";
/*********************************************************************
 *
 * File        :  $Source: /cvsroot/ijbswa/current/jbsockets.c,v $
 *
 * Purpose     :  Contains wrappers for system-specific sockets code,
 *                so that the rest of Junkbuster can be more
 *                OS-independent.  Contains #ifdefs to make this work
 *                on many platforms.
 *
 * Copyright   :  Written by and Copyright (C) 2001 the SourceForge
 *                Privoxy team. http://www.privoxy.org/
 *
 *                Based on the Internet Junkbuster originally written
 *                by and Copyright (C) 1997 Anonymous Coders and 
 *                Junkbusters Corporation.  http://www.junkbusters.com
 *
 *                This program is free software; you can redistribute it 
 *                and/or modify it under the terms of the GNU General
 *                Public License as published by the Free Software
 *                Foundation; either version 2 of the License, or (at
 *                your option) any later version.
 *
 *                This program is distributed in the hope that it will
 *                be useful, but WITHOUT ANY WARRANTY; without even the
 *                implied warranty of MERCHANTABILITY or FITNESS FOR A
 *                PARTICULAR PURPOSE.  See the GNU General Public
 *                License for more details.
 *
 *                The GNU General Public License should be included with
 *                this file.  If not, you can view it at
 *                http://www.gnu.org/copyleft/gpl.html
 *                or write to the Free Software Foundation, Inc., 59
 *                Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * Revisions   :
 *    $Log: jbsockets.c,v $
 *    Revision 1.41  2006/11/13 19:05:51  fabiankeil
 *    Make pthread mutex locking more generic. Instead of
 *    checking for OSX and OpenBSD, check for FEATURE_PTHREAD
 *    and use mutex locking unless there is an _r function
 *    available. Better safe than sorry.
 *
 *    Fixes "./configure --disable-pthread" and should result
 *    in less threading-related problems on pthread-using platforms,
 *    but it still doesn't fix BR#1122404.
 *
 *    Revision 1.40  2006/09/02 15:36:42  fabiankeil
 *    Follow the OpenBSD port's lead and protect the resolve
 *    functions on OpenBSD as well.
 *
 *    Revision 1.39  2006/08/03 02:46:41  david__schmidt
 *    Incorporate Fabian Keil's patch work:http://www.fabiankeil.de/sourcecode/privoxy/
 *
 *    Revision 1.38  2006/07/18 14:48:46  david__schmidt
 *    Reorganizing the repository: swapping out what was HEAD (the old 3.1 branch)
 *    with what was really the latest development (the v_3_0_branch branch)
 *
 *    Revision 1.35.2.8  2006/01/21 16:16:08  david__schmidt
 *    Thanks to  Edward Carrel for his patch to modernize OSX'spthreads support.  See bug #1409623.
 *
 *    Revision 1.35.2.7  2005/05/07 21:50:55  david__schmidt
 *    A few memory leaks plugged (mostly on error paths)
 *
 *    Revision 1.35.2.6  2003/12/17 16:34:40  oes
 *    Cosmetics
 *
 *    Revision 1.35.2.5  2003/04/29 11:32:54  oes
 *    Don't rely on h_addr being non-NULL after gethostbyname.
 *    Works around an oddness in Max OSX and closes bug #724796
 *
 *    Revision 1.35.2.4  2003/04/04 12:40:20  oes
 *    Made sure the errno set by bind, not close[socket] is used in
 *    bind_port. Probably fixes bugs #713777, #705562.
 *
 *    Revision 1.35.2.3  2003/03/07 03:41:04  david__schmidt
 *    Wrapping all *_r functions (the non-_r versions of them) with mutex semaphores for OSX.  Hopefully this will take care of all of those pesky crash reports.
 *
 *    Revision 1.35.2.2  2002/11/20 14:37:24  oes
 *    Fixed Win32 error logging in bind_port.
 *    Thanks to Oliver Stoeneberg for the hint.
 *
 *    Revision 1.35.2.1  2002/05/26 23:41:27  joergs
 *    AmigaOS: Fixed wrong type of len in write_socket()
 *
 *    Revision 1.35  2002/04/26 15:50:04  joergs
 *    AmigaOS: No socklen_t, added AMIGA to the systems using int instead.
 *
 *    Revision 1.34  2002/04/08 20:31:41  swa
 *    fixed JB spelling
 *
 *    Revision 1.33  2002/04/03 16:02:18  gliptak
 *    Correcting compile warning with older gcc
 *
 *    Revision 1.32  2002/03/31 17:18:59  jongfoster
 *    Win32 only: Enabling STRICT to fix a VC++ compile warning.
 *
 *    Revision 1.31  2002/03/29 03:33:13  david__schmidt
 *    Fix Mac OSX compiler warnings
 *
 *    Revision 1.30  2002/03/27 14:32:43  david__schmidt
 *    More compiler warning message maintenance
 *
 *    Revision 1.29  2002/03/26 22:29:54  swa
 *    we have a new homepage!
 *
 *    Revision 1.28  2002/03/24 13:25:43  swa
 *    name change related issues
 *
 *    Revision 1.27  2002/03/13 00:27:05  jongfoster
 *    Killing warnings
 *
 *    Revision 1.26  2002/03/11 22:07:02  david__schmidt
 *    OS/2 port maintenance:
 *    - Fixed EMX build - it had decayed a little
 *    - Fixed inexplicable crash during FD_ZERO - must be due to a bad macro.
 *      substituted a memset for now.
 *
 *    Revision 1.25  2002/03/09 20:03:52  jongfoster
 *    - Making various functions return int rather than size_t.
 *      (Undoing a recent change).  Since size_t is unsigned on
 *      Windows, functions like read_socket that return -1 on
 *      error cannot return a size_t.
 *
 *      THIS WAS A MAJOR BUG - it caused frequent, unpredictable
 *      crashes, and also frequently caused JB to jump to 100%
 *      CPU and stay there.  (Because it thought it had just
 *      read ((unsigned)-1) == 4Gb of data...)
 *
 *    - The signature of write_socket has changed, it now simply
 *      returns success=0/failure=nonzero.
 *
 *    - Trying to get rid of a few warnings --with-debug on
 *      Windows, I've introduced a new type "jb_socket".  This is
 *      used for the socket file descriptors.  On Windows, this
 *      is SOCKET (a typedef for unsigned).  Everywhere else, it's
 *      an int.  The error value can't be -1 any more, so it's
 *      now JB_INVALID_SOCKET (which is -1 on UNIX, and in
 *      Windows it maps to the #define INVALID_SOCKET.)
 *
 *    - The signature of bind_port has changed.
 *
 *    Revision 1.24  2002/03/07 03:51:36  oes
 *     - Improved handling of failed DNS lookups
 *     - Fixed compiler warnings etc
 *
 *    Revision 1.23  2002/03/05 00:36:01  jongfoster
 *    Fixing bug 514988 - unable to restart Junkbuster
 *
 *    Revision 1.22  2002/03/04 02:08:02  david__schmidt
 *    Enable web editing of actions file on OS/2 (it had been broken all this time!)
 *
 *    Revision 1.21  2002/01/09 14:32:33  oes
 *    Added support for gethostbyname_r and gethostbyaddr_r.
 *
 *    Revision 1.20  2001/11/16 00:48:48  jongfoster
 *    Enabling duplicate-socket detection for all platforms, not
 *    just Win32.
 *
 *    Revision 1.19  2001/10/25 03:40:47  david__schmidt
 *    Change in porting tactics: OS/2's EMX porting layer doesn't allow multiple
 *    threads to call select() simultaneously.  So, it's time to do a real, live,
 *    native OS/2 port.  See defines for __EMX__ (the porting layer) vs. __OS2__
 *    (native). Both versions will work, but using __OS2__ offers multi-threading.
 *
 *    Revision 1.18  2001/09/21 23:02:02  david__schmidt
 *    Cleaning up 2 compiler warnings on OS/2.
 *
 *    Revision 1.17  2001/09/13 20:11:46  jongfoster
 *    Fixing 2 compiler warnings under Win32
 *
 *    Revision 1.16  2001/07/30 22:08:36  jongfoster
 *    Tidying up #defines:
 *    - All feature #defines are now of the form FEATURE_xxx
 *    - Permanently turned off WIN_GUI_EDIT
 *    - Permanently turned on WEBDAV and SPLIT_PROXY_ARGS
 *
 *    Revision 1.15  2001/07/29 17:40:43  jongfoster
 *    Fixed compiler warning by adding a cast
 *
 *    Revision 1.14  2001/07/18 13:47:59  oes
 *    Eliminated dirty hack for getsockbyname()
 *
 *    Revision 1.13  2001/07/15 13:56:57  jongfoster
 *    Removing unused local variable.
 *
 *    Revision 1.12  2001/07/01 17:04:11  oes
 *    Bugfix: accept_connection no longer uses the obsolete hstrerror() function
 *
 *    Revision 1.11  2001/06/29 21:45:41  oes
 *    Indentation, CRLF->LF, Tab-> Space
 *
 *    Revision 1.10  2001/06/29 13:29:15  oes
 *    - Added remote (server) host IP to csp->http->host_ip_addr_str
 *    - Added detection of local socket IP and fqdn
 *    - Removed logentry from cancelled commit
 *
 *    Revision 1.9  2001/06/07 23:06:09  jongfoster
 *    The host parameter to connect_to() is now const.
 *
 *    Revision 1.8  2001/06/03 19:12:07  oes
 *    filled comment
 *
 *    Revision 1.7  2001/05/28 16:14:00  jongfoster
 *    Fixing bug in LOG_LEVEL_LOG
 *
 *    Revision 1.6  2001/05/26 17:28:32  jongfoster
 *    Fixed LOG_LEVEL_LOG
 *
 *    Revision 1.5  2001/05/26 15:26:15  jongfoster
 *    ACL feature now provides more security by immediately dropping
 *    connections from untrusted hosts.
 *
 *    Revision 1.4  2001/05/26 00:37:42  jongfoster
 *    Cosmetic indentation correction.
 *
 *    Revision 1.3  2001/05/25 21:57:54  jongfoster
 *    Now gives a warning under Windows if you try to bind
 *    it to a port that's already in use.
 *
 *    Revision 1.2  2001/05/17 23:01:01  oes
 *     - Cleaned CRLF's from the sources and related files
 *
 *    Revision 1.1.1.1  2001/05/15 13:58:54  oes
 *    Initial import of version 2.9.3 source tree
 *
 *
 *********************************************************************/


#include "config.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>

#ifdef _WIN32

#ifndef STRICT
#define STRICT
#endif
#include <windows.h>
#include <sys/timeb.h>
#include <io.h>

#else

#ifndef __OS2__
#include <unistd.h>
#endif
#include <sys/time.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <netdb.h>
#include <sys/socket.h>

#ifndef __BEOS__
#include <netinet/tcp.h>
#ifndef __OS2__
#include <arpa/inet.h>
#endif
#else
#include <socket.h>
#endif

#if defined(__EMX__) || defined (__OS2__)
#include <sys/select.h>  /* OS/2/EMX needs a little help with select */
#ifdef __OS2__
#include <nerrno.h>
#endif
#endif

#endif

#include "project.h"

#ifdef FEATURE_PTHREAD
#include "jcc.h"
/* jcc.h is for mutex semaphores only */
#endif /* def FEATURE_PTHREAD */

#include "jbsockets.h"
#include "filters.h"
#include "errlog.h"

const char jbsockets_h_rcs[] = JBSOCKETS_H_VERSION;


/*********************************************************************
 *
 * Function    :  connect_to
 *
 * Description :  Open a socket and connect to it.  Will check
 *                that this is allowed according to ACL.
 *
 * Parameters  :
 *          1  :  host = hostname to connect to
 *          2  :  portnum = port to connent on
 *          3  :  csp = Current client state (buffers, headers, etc...)
 *                      Not modified, only used for source IP and ACL.
 *
 * Returns     :  JB_INVALID_SOCKET => failure, else it is the socket
 *                file descriptor.
 *
 *********************************************************************/
jb_socket connect_to(const char *host, int portnum, struct client_state *csp)
{
   struct sockaddr_in inaddr;
   jb_socket fd;
   int addr;
   fd_set wfds;
   struct timeval tv[1];
#if !defined(_WIN32) && !defined(__BEOS__) && !defined(AMIGA)
   int   flags;
#endif /* !defined(_WIN32) && !defined(__BEOS__) && !defined(AMIGA) */

#ifdef FEATURE_ACL
   struct access_control_addr dst[1];
#endif /* def FEATURE_ACL */

   memset((char *)&inaddr, 0, sizeof inaddr);

   if ((addr = resolve_hostname_to_ip(host)) == INADDR_NONE)
   {
      csp->http->host_ip_addr_str = strdup("unknown");
      return(JB_INVALID_SOCKET);
   }

#ifdef FEATURE_ACL
   dst->addr = ntohl((unsigned long) addr);
   dst->port = portnum;

   if (block_acl(dst, csp))
   {
#ifdef __OS2__
      errno = SOCEPERM;
#else
      errno = EPERM;
#endif
      return(JB_INVALID_SOCKET);
   }
#endif /* def FEATURE_ACL */

   inaddr.sin_addr.s_addr = addr;
   inaddr.sin_family      = AF_INET;
   csp->http->host_ip_addr_str = strdup(inet_ntoa(inaddr.sin_addr));

#ifndef _WIN32
   if (sizeof(inaddr.sin_port) == sizeof(short))
#endif /* ndef _WIN32 */
   {
      inaddr.sin_port = htons((unsigned short) portnum);
   }
#ifndef _WIN32
   else
   {
      inaddr.sin_port = htonl((unsigned long)portnum);
   }
#endif /* ndef _WIN32 */

#ifdef _WIN32
   if ((fd = socket(inaddr.sin_family, SOCK_STREAM, 0)) == JB_INVALID_SOCKET)
#else
   if ((fd = socket(inaddr.sin_family, SOCK_STREAM, 0)) < 0)
#endif
   {
      return(JB_INVALID_SOCKET);
   }

#ifdef TCP_NODELAY
   {  /* turn off TCP coalescence */
      int mi = 1;
      setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (char *) &mi, sizeof (int));
   }
#endif /* def TCP_NODELAY */

#if !defined(_WIN32) && !defined(__BEOS__) && !defined(AMIGA) && !defined(__OS2__)
   if ((flags = fcntl(fd, F_GETFL, 0)) != -1)
   {
      flags |= O_NDELAY;
      fcntl(fd, F_SETFL, flags);
   }
#endif /* !defined(_WIN32) && !defined(__BEOS__) && !defined(AMIGA) && !defined(__OS2__) */

   while (connect(fd, (struct sockaddr *) & inaddr, sizeof inaddr) == JB_INVALID_SOCKET)
   {
#ifdef _WIN32
      if (errno == WSAEINPROGRESS)
#elif __OS2__ 
      if (sock_errno() == EINPROGRESS)
#else /* ifndef _WIN32 */
      if (errno == EINPROGRESS)
#endif /* ndef _WIN32 || __OS2__ */
      {
         break;
      }

#ifdef __OS2__ 
      if (sock_errno() != EINTR)
#else
      if (errno != EINTR)
#endif /* __OS2__ */
      {
         close_socket(fd);
         return(JB_INVALID_SOCKET);
      }
   }

#if !defined(_WIN32) && !defined(__BEOS__) && !defined(AMIGA) && !defined(__OS2__)
   if (flags != -1)
   {
      flags &= ~O_NDELAY;
      fcntl(fd, F_SETFL, flags);
   }
#endif /* !defined(_WIN32) && !defined(__BEOS__) && !defined(AMIGA) && !defined(__OS2__) */

   /* wait for connection to complete */
   FD_ZERO(&wfds);
   FD_SET(fd, &wfds);

   tv->tv_sec  = 30;
   tv->tv_usec = 0;

   /* MS Windows uses int, not SOCKET, for the 1st arg of select(). Wierd! */
   if (select((int)fd + 1, NULL, &wfds, NULL, tv) <= 0)
   {
      close_socket(fd);
      return(JB_INVALID_SOCKET);
   }
   return(fd);

}


/*********************************************************************
 *
 * Function    :  write_socket
 *
 * Description :  Write the contents of buf (for n bytes) to socket fd.
 *
 * Parameters  :
 *          1  :  fd = file descriptor (aka. handle) of socket to write to.
 *          2  :  buf = pointer to data to be written.
 *          3  :  len = length of data to be written to the socket "fd".
 *
 * Returns     :  0 on success (entire buffer sent).
 *                nonzero on error.
 *
 *********************************************************************/
#ifdef AMIGA
int write_socket(jb_socket fd, const char *buf, ssize_t len)
#else
int write_socket(jb_socket fd, const char *buf, size_t len)
#endif
{
   if (len == 0)
   {
      return 0;
   }

   if (len < 0) /* constant condition - size_t isn't ever negative */ 
   {
      return 1;
   }

   log_error(LOG_LEVEL_LOG, "%N", len, buf);

#if defined(_WIN32)
   return (send(fd, buf, (int)len, 0) != (int)len);
#elif defined(__BEOS__) || defined(AMIGA)
   return (send(fd, buf, len, 0) != len);
#elif defined(__OS2__)
   /*
    * Break the data up into SOCKET_SEND_MAX chunks for sending...
    * OS/2 seemed to complain when the chunks were too large.
    */
#define SOCKET_SEND_MAX 65000
   {
      int write_len = 0, send_len, send_rc = 0, i = 0;
      while ((i < len) && (send_rc != -1))
      {
         if ((i + SOCKET_SEND_MAX) > len)
            send_len = len - i;
         else
            send_len = SOCKET_SEND_MAX;
         send_rc = send(fd,(char*)buf + i, send_len, 0);
         if (send_rc == -1)
            return 1;
         i = i + send_len;
      }
      return 0;
   }
#else
   return (write(fd, buf, len) != len);
#endif

}


/*********************************************************************
 *
 * Function    :  read_socket
 *
 * Description :  Read from a TCP/IP socket in a platform independent way.
 *
 * Parameters  :
 *          1  :  fd = file descriptor of the socket to read
 *          2  :  buf = pointer to buffer where data will be written
 *                Must be >= len bytes long.
 *          3  :  len = maximum number of bytes to read
 *
 * Returns     :  On success, the number of bytes read is returned (zero
 *                indicates end of file), and the file position is advanced
 *                by this number.  It is not an error if this number is
 *                smaller than the number of bytes requested; this may hap-
 *                pen for example because fewer bytes are actually available
 *                right now (maybe because we were close to end-of-file, or
 *                because we are reading from a pipe, or from a terminal,
 *                or because read() was interrupted by a signal).  On error,
 *                -1 is returned, and errno is set appropriately.  In this
 *                case it is left unspecified whether the file position (if
 *                any) changes.
 *
 *********************************************************************/
int read_socket(jb_socket fd, char *buf, int len)
{
   if (len <= 0)
   {
      return(0);
   }

#if defined(_WIN32)
   return(recv(fd, buf, len, 0));
#elif defined(__BEOS__) || defined(AMIGA) || defined(__OS2__)
   return(recv(fd, buf, (size_t)len, 0));
#else
   return(read(fd, buf, (size_t)len));
#endif
}


/*********************************************************************
 *
 * Function    :  close_socket
 *
 * Description :  Closes a TCP/IP socket
 *
 * Parameters  :
 *          1  :  fd = file descriptor of socket to be closed
 *
 * Returns     :  void
 *
 *********************************************************************/
void close_socket(jb_socket fd)
{
#if defined(_WIN32) || defined(__BEOS__)
   closesocket(fd);
#elif defined(AMIGA)
   CloseSocket(fd); 
#elif defined(__OS2__)
   soclose(fd);
#else
   close(fd);
#endif

}


/*********************************************************************
 *
 * Function    :  bind_port
 *
 * Description :  Call socket, set socket options, and listen.
 *                Called by listen_loop to "boot up" our proxy address.
 *
 * Parameters  :
 *          1  :  hostnam = TCP/IP address to bind/listen to
 *          2  :  portnum = port to listen on
 *          3  :  pfd = pointer used to return file descriptor.
 *
 * Returns     :  if success, returns 0 and sets *pfd.
 *                if failure, returns -3 if address is in use,
 *                                    -2 if address unresolvable,
 *                                    -1 otherwise
 *********************************************************************/
int bind_port(const char *hostnam, int portnum, jb_socket *pfd)
{
   struct sockaddr_in inaddr;
   jb_socket fd;
#ifndef _WIN32
   int one = 1;
#endif /* ndef _WIN32 */

   *pfd = JB_INVALID_SOCKET;

   memset((char *)&inaddr, '\0', sizeof inaddr);

   inaddr.sin_family      = AF_INET;
   inaddr.sin_addr.s_addr = resolve_hostname_to_ip(hostnam);

   if (inaddr.sin_addr.s_addr == INADDR_NONE)
   {
      return(-2);
   }

#ifndef _WIN32
   if (sizeof(inaddr.sin_port) == sizeof(short))
#endif /* ndef _WIN32 */
   {
      inaddr.sin_port = htons((unsigned short) portnum);
   }
#ifndef _WIN32
   else
   {
      inaddr.sin_port = htonl((unsigned long) portnum);
   }
#endif /* ndef _WIN32 */

   fd = socket(AF_INET, SOCK_STREAM, 0);

#ifdef _WIN32
   if (fd == JB_INVALID_SOCKET)
#else
   if (fd < 0)
#endif
   {
      return(-1);
   }

#ifndef _WIN32
   /*
    * This is not needed for Win32 - in fact, it stops
    * duplicate instances of Privoxy from being caught.
    *
    * On UNIX, we assume the user is sensible enough not
    * to start Junkbuster multiple times on the same IP.
    * Without this, stopping and restarting Privoxy
    * from a script fails.
    * Note: SO_REUSEADDR is meant to only take over
    * sockets which are *not* in listen state in Linux,
    * e.g. sockets in TIME_WAIT. YMMV.
    */
   setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *)&one, sizeof(one));
#endif /* ndef _WIN32 */

   if (bind(fd, (struct sockaddr *)&inaddr, sizeof(inaddr)) < 0)
   {
#ifdef _WIN32
      errno = WSAGetLastError();
      if (errno == WSAEADDRINUSE)
#else
      if (errno == EADDRINUSE)
#endif
      {
         close_socket(fd);
         return(-3);
      }
      else
      {
         close_socket(fd);
         return(-1);
      }
   }

   while (listen(fd, 5) == -1)
   {
      if (errno != EINTR)
      {
         return(-1);
      }
   }

   *pfd = fd;
   return 0;

}


/*********************************************************************
 *
 * Function    :  accept_connection
 *
 * Description :  Accepts a connection on a socket.  Socket must have
 *                been created using bind_port().
 *
 * Parameters  :
 *          1  :  csp = Client state, cfd, ip_addr_str, and 
 *                ip_addr_long will be set by this routine.
 *          2  :  fd  = file descriptor returned from bind_port
 *
 * Returns     :  when a connection is accepted, it returns 1 (TRUE).
 *                On an error it returns 0 (FALSE).
 *
 *********************************************************************/
int accept_connection(struct client_state * csp, jb_socket fd)
{
   struct sockaddr_in client, server;
   struct hostent *host = NULL;
   jb_socket afd;
#if defined(_WIN32) || defined(__OS2__) || defined(__APPLE_CC__) || defined(AMIGA)
   /* Wierdness - fix a warning. */
   int c_length, s_length;
#else
   socklen_t c_length, s_length;
#endif
#if defined(HAVE_GETHOSTBYADDR_R_8_ARGS) ||  defined(HAVE_GETHOSTBYADDR_R_7_ARGS) || defined(HAVE_GETHOSTBYADDR_R_5_ARGS)
   struct hostent result;
#if defined(HAVE_GETHOSTBYADDR_R_5_ARGS)
   struct hostent_data hdata;
#else
   char hbuf[HOSTENT_BUFFER_SIZE];
   int thd_err;
#endif /* def HAVE_GETHOSTBYADDR_R_5_ARGS */
#endif /* def HAVE_GETHOSTBYADDR_R_(8|7|5)_ARGS */

   c_length = s_length = sizeof(client);

#ifdef _WIN32
   afd = accept (fd, (struct sockaddr *) &client, &c_length);
   if (afd == JB_INVALID_SOCKET)
   {
      return 0;
   }
#else
   do
   {
      afd = accept (fd, (struct sockaddr *) &client, &c_length);
   } while (afd < 1 && errno == EINTR);
   if (afd < 0)
   {
      return 0;
   }
#endif

   /* 
    * Determine the IP-Adress that the client used to reach us
    * and the hostname associated with that address
    */
   if (!getsockname(afd, (struct sockaddr *) &server, &s_length))
   {
      csp->my_ip_addr_str = strdup(inet_ntoa(server.sin_addr));
#if defined(HAVE_GETHOSTBYADDR_R_8_ARGS)
      gethostbyaddr_r((const char *)&server.sin_addr,
                      sizeof(server.sin_addr), AF_INET,
                      &result, hbuf, HOSTENT_BUFFER_SIZE,
                      &host, &thd_err);
#elif defined(HAVE_GETHOSTBYADDR_R_7_ARGS)
      host = gethostbyaddr_r((const char *)&server.sin_addr,
                      sizeof(server.sin_addr), AF_INET,
                      &result, hbuf, HOSTENT_BUFFER_SIZE, &thd_err);
#elif defined(HAVE_GETHOSTBYADDR_R_5_ARGS)
      if (0 == gethostbyaddr_r((const char *)&server.sin_addr,
                               sizeof(server.sin_addr), AF_INET,
                               &result, &hdata))
      {
         host = &result;
      }
      else
      {
         host = NULL;
      }
#elif FEATURE_PTHREAD
      pthread_mutex_lock(&gethostbyaddr_mutex);
      host = gethostbyaddr((const char *)&server.sin_addr, 
                           sizeof(server.sin_addr), AF_INET);
      pthread_mutex_unlock(&gethostbyaddr_mutex);
#else
      host = gethostbyaddr((const char *)&server.sin_addr, 
                           sizeof(server.sin_addr), AF_INET);
#endif
      if (host == NULL)
      {
         log_error(LOG_LEVEL_ERROR, "Unable to get my own hostname: %E\n");
      }
      else
      {
         csp->my_hostname = strdup(host->h_name);
      }
   }

   csp->cfd    = afd;
   csp->ip_addr_str  = strdup(inet_ntoa(client.sin_addr));
   csp->ip_addr_long = ntohl(client.sin_addr.s_addr);

   return 1;

}


/*********************************************************************
 *
 * Function    :  resolve_hostname_to_ip
 *
 * Description :  Resolve a hostname to an internet tcp/ip address.
 *                NULL or an empty string resolve to INADDR_ANY.
 *
 * Parameters  :
 *          1  :  host = hostname to resolve
 *
 * Returns     :  INADDR_NONE => failure, INADDR_ANY or tcp/ip address if succesful.
 *
 *********************************************************************/
unsigned long resolve_hostname_to_ip(const char *host)
{
   struct sockaddr_in inaddr;
   struct hostent *hostp;
   unsigned int dns_retries = 0;
#if defined(HAVE_GETHOSTBYNAME_R_6_ARGS) || defined(HAVE_GETHOSTBYNAME_R_5_ARGS) || defined(HAVE_GETHOSTBYNAME_R_3_ARGS)
   struct hostent result;
#if defined(HAVE_GETHOSTBYNAME_R_6_ARGS) || defined(HAVE_GETHOSTBYNAME_R_5_ARGS)
   char hbuf[HOSTENT_BUFFER_SIZE];
   int thd_err;
#else /* defined(HAVE_GETHOSTBYNAME_R_3_ARGS) */
   struct hostent_data hdata;
#endif /* def HAVE_GETHOSTBYNAME_R_(6|5)_ARGS */
#endif /* def HAVE_GETHOSTBYNAME_R_(6|5|3)_ARGS */

   if ((host == NULL) || (*host == '\0'))
   {
      return(INADDR_ANY);
   }

   memset((char *) &inaddr, 0, sizeof inaddr);

   if ((inaddr.sin_addr.s_addr = inet_addr(host)) == -1)
   {
#if defined(HAVE_GETHOSTBYNAME_R_6_ARGS)
       while ( gethostbyname_r(host, &result, hbuf,
                      HOSTENT_BUFFER_SIZE, &hostp, &thd_err)
               && (thd_err == TRY_AGAIN) && (dns_retries++ < 10) )
      {   
         log_error(LOG_LEVEL_ERROR, "Timeout #%u while trying to resolve %s. Trying again.",
                                                dns_retries, host);
      }
#elif defined(HAVE_GETHOSTBYNAME_R_5_ARGS)
      hostp = gethostbyname_r(host, &result, hbuf,
                      HOSTENT_BUFFER_SIZE, &thd_err);
#elif defined(HAVE_GETHOSTBYNAME_R_3_ARGS)
      if (0 == gethostbyname_r(host, &result, &hdata))
      {
         hostp = &result;
      }
      else
      {
         hostp = NULL;
      }
#elif FEATURE_PTHREAD
      pthread_mutex_lock(&gethostbyname_mutex);
      while ( NULL == (hostp = gethostbyname(host))
            && (h_errno == TRY_AGAIN) && (dns_retries++ < 10) )
      {   
         log_error(LOG_LEVEL_ERROR, "Timeout #%u while trying to resolve %s. Trying again.",
                                                dns_retries, host);
      }
      pthread_mutex_unlock(&gethostbyname_mutex);
#else
      while ( NULL == (hostp = gethostbyname(host))
            && (h_errno == TRY_AGAIN) && (dns_retries++ < 10) )
      {
         log_error(LOG_LEVEL_ERROR, "Timeout #%u while trying to resolve %s. Trying again.",
                                                dns_retries, host);
      }
#endif /* def HAVE_GETHOSTBYNAME_R_(6|5|3)_ARGS */
      /*
       * On Mac OSX, if a domain exists but doesn't have a type A
       * record associated with it, the h_addr member of the struct
       * hostent returned by gethostbyname is NULL, even if h_length
       * is 4. Therefore the second test below.
       */
      if (hostp == NULL || hostp->h_addr == NULL)
      {
         errno = EINVAL;
         log_error(LOG_LEVEL_ERROR, "could not resolve hostname %s", host);
         return(INADDR_NONE);
      }
      if (hostp->h_addrtype != AF_INET)
      {
#ifdef _WIN32
         errno = WSAEPROTOTYPE;
#else
         errno = EPROTOTYPE;
#endif 
         log_error(LOG_LEVEL_ERROR, "hostname %s resolves to unknown address type.", host);
         return(INADDR_NONE);
      }
      memcpy(
         (char *) &inaddr.sin_addr,
         (char *) hostp->h_addr,
         sizeof(inaddr.sin_addr)
      );
   }
   return(inaddr.sin_addr.s_addr);

}


/*
  Local Variables:
  tab-width: 3
  end:
*/
