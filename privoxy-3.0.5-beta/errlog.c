const char errlog_rcs[] = "$Id: errlog.c,v 1.45 2006/08/21 11:15:54 david__schmidt Exp $";
/*********************************************************************
 *
 * File        :  $Source: /cvsroot/ijbswa/current/errlog.c,v $
 *
 * Purpose     :  Log errors to a designated destination in an elegant,
 *                printf-like fashion.
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
 *    $Log: errlog.c,v $
 *    Revision 1.45  2006/08/21 11:15:54  david__schmidt
 *    MS Visual C++ build updates
 *
 *    Revision 1.44  2006/08/18 16:03:16  david__schmidt
 *    Tweak for OS/2 build happiness.
 *
 *    Revision 1.43  2006/08/03 02:46:41  david__schmidt
 *    Incorporate Fabian Keil's patch work:
 *    http://www.fabiankeil.de/sourcecode/privoxy/
 *
 *    Revision 1.42  2006/07/18 14:48:46  david__schmidt
 *    Reorganizing the repository: swapping out what was HEAD (the old 3.1 branch)
 *    with what was really the latest development (the v_3_0_branch branch)
 *
 *    Revision 1.40.2.4  2005/04/03 20:10:50  david__schmidt
 *    Thanks to Jindrich Makovicka for a race condition fix for the log
 *    file.  The race condition remains for non-pthread implementations.
 *    Reference patch #1175720.
 *
 *    Revision 1.40.2.3  2003/03/07 03:41:04  david__schmidt
 *    Wrapping all *_r functions (the non-_r versions of them) with mutex 
 *    semaphores for OSX.  Hopefully this will take care of all of those pesky
 *    crash reports.
 *
 *    Revision 1.40.2.2  2002/09/28 00:30:57  david__schmidt
 *    Update error logging to give sane values for thread IDs on Mach kernels.
 *    It's still a hack, but at least it looks farily normal.  We print the
 *    absolute value of the first 4 bytes of the pthread_t modded with 1000.
 *
 *    Revision 1.40.2.1  2002/09/25 12:47:42  oes
 *    Make log_error safe against NULL string arguments
 *
 *    Revision 1.40  2002/05/22 01:27:27  david__schmidt
 *
 *    Add os2_socket_strerr mirroring w32_socket_strerr.
 *
 *    Revision 1.39  2002/04/03 17:15:27  gliptak
 *    zero padding thread ids in log
 *
 *    Revision 1.38  2002/03/31 17:18:59  jongfoster
 *    Win32 only: Enabling STRICT to fix a VC++ compile warning.
 *
 *    Revision 1.37  2002/03/27 14:32:43  david__schmidt
 *    More compiler warning message maintenance
 *
 *    Revision 1.36  2002/03/26 22:29:54  swa
 *    we have a new homepage!
 *
 *    Revision 1.35  2002/03/24 15:23:33  jongfoster
 *    Name changes
 *
 *    Revision 1.34  2002/03/24 13:25:43  swa
 *    name change related issues
 *
 *    Revision 1.33  2002/03/13 00:27:04  jongfoster
 *    Killing warnings
 *
 *    Revision 1.32  2002/03/07 03:46:17  oes
 *    Fixed compiler warnings
 *
 *    Revision 1.31  2002/03/06 23:02:57  jongfoster
 *    Removing tabs
 *
 *    Revision 1.30  2002/03/05 22:43:45  david__schmidt
 *    - Better error reporting on OS/2
 *    - Fix double-slash comment (oops)
 *
 *    Revision 1.29  2002/03/04 23:45:13  jongfoster
 *    Printing thread ID if using Win32 native threads
 *
 *    Revision 1.28  2002/03/04 17:59:59  oes
 *    Deleted deletePidFile(), cosmetics
 *
 *    Revision 1.27  2002/03/04 02:08:01  david__schmidt
 *    Enable web editing of actions file on OS/2 (it had been broken all this time!)
 *
 *    Revision 1.26  2002/01/09 19:05:45  steudten
 *    Fix big memory leak.
 *
 *    Revision 1.25  2002/01/09 14:32:08  oes
 *    Added support for gmtime_r and localtime_r.
 *
 *    Revision 1.24  2001/12/30 14:07:32  steudten
 *    - Add signal handling (unix)
 *    - Add SIGHUP handler (unix)
 *    - Add creation of pidfile (unix)
 *    - Add action 'top' in rc file (RH)
 *    - Add entry 'SIGNALS' to manpage
 *    - Add exit message to logfile (unix)
 *
 *    Revision 1.23  2001/11/07 00:02:13  steudten
 *    Add line number in error output for lineparsing for
 *    actionsfile and configfile.
 *    Special handling for CLF added.
 *
 *    Revision 1.22  2001/11/05 23:43:05  steudten
 *    Add time+date to log files.
 *
 *    Revision 1.21  2001/10/25 03:40:47  david__schmidt
 *    Change in porting tactics: OS/2's EMX porting layer doesn't allow multiple
 *    threads to call select() simultaneously.  So, it's time to do a real, live,
 *    native OS/2 port.  See defines for __EMX__ (the porting layer) vs. __OS2__
 *    (native). Both versions will work, but using __OS2__ offers multi-threading.
 *
 *    Revision 1.20  2001/09/16 23:04:34  jongfoster
 *    Fixing a warning
 *
 *    Revision 1.19  2001/09/13 20:08:06  jongfoster
 *    Adding support for LOG_LEVEL_CGI
 *
 *    Revision 1.18  2001/09/10 11:27:24  oes
 *    Declaration of w32_socket_strerr now conditional
 *
 *    Revision 1.17  2001/09/10 10:17:13  oes
 *    Removed unused variable; Fixed sprintf format
 *
 *    Revision 1.16  2001/07/30 22:08:36  jongfoster
 *    Tidying up #defines:
 *    - All feature #defines are now of the form FEATURE_xxx
 *    - Permanently turned off WIN_GUI_EDIT
 *    - Permanently turned on WEBDAV and SPLIT_PROXY_ARGS
 *
 *    Revision 1.15  2001/07/29 17:41:10  jongfoster
 *    Now prints thread ID for each message (pthreads only)
 *
 *    Revision 1.14  2001/07/19 19:03:48  haroon
 *    - Added case for LOG_LEVEL_POPUPS
 *
 *    Revision 1.13  2001/07/13 13:58:58  oes
 *     - Added case for LOG_LEVEL_DEANIMATE
 *     - Removed all #ifdef PCRS
 *
 *    Revision 1.12  2001/06/09 10:55:28  jongfoster
 *    Changing BUFSIZ ==> BUFFER_SIZE
 *
 *    Revision 1.11  2001/06/01 18:14:49  jongfoster
 *    Changing the calls to strerr() to check HAVE_STRERR (which is defined
 *    in config.h if appropriate) rather than the NO_STRERR macro.
 *
 *    Revision 1.10  2001/05/29 11:52:21  oes
 *    Conditional compilation of w32_socket_error
 *
 *    Revision 1.9  2001/05/28 16:15:17  jongfoster
 *    Improved reporting of errors under Win32.
 *
 *    Revision 1.8  2001/05/26 17:25:14  jongfoster
 *    Added support for CLF (Common Log Format) and fixed LOG_LEVEL_LOG
 *
 *    Revision 1.7  2001/05/26 15:21:28  jongfoster
 *    Activity animation in Win32 GUI now works even if debug==0
 *
 *    Revision 1.6  2001/05/25 21:55:08  jongfoster
 *    Now cleans up properly on FATAL (removes taskbar icon etc)
 *
 *    Revision 1.5  2001/05/22 18:46:04  oes
 *
 *    - Enabled filtering banners by size rather than URL
 *      by adding patterns that replace all standard banner
 *      sizes with the "Junkbuster" gif to the re_filterfile
 *
 *    - Enabled filtering WebBugs by providing a pattern
 *      which kills all 1x1 images
 *
 *    - Added support for PCRE_UNGREEDY behaviour to pcrs,
 *      which is selected by the (nonstandard and therefore
 *      capital) letter 'U' in the option string.
 *      It causes the quantifiers to be ungreedy by default.
 *      Appending a ? turns back to greedy (!).
 *
 *    - Added a new interceptor ijb-send-banner, which
 *      sends back the "Junkbuster" gif. Without imagelist or
 *      MSIE detection support, or if tinygif = 1, or the
 *      URL isn't recognized as an imageurl, a lame HTML
 *      explanation is sent instead.
 *
 *    - Added new feature, which permits blocking remote
 *      script redirects and firing back a local redirect
 *      to the browser.
 *      The feature is conditionally compiled, i.e. it
 *      can be disabled with --disable-fast-redirects,
 *      plus it must be activated by a "fast-redirects"
 *      line in the config file, has its own log level
 *      and of course wants to be displayed by show-proxy-args
 *      Note: Boy, all the #ifdefs in 1001 locations and
 *      all the fumbling with configure.in and acconfig.h
 *      were *way* more work than the feature itself :-(
 *
 *    - Because a generic redirect template was needed for
 *      this, tinygif = 3 now uses the same.
 *
 *    - Moved GIFs, and other static HTTP response templates
 *      to project.h
 *
 *    - Some minor fixes
 *
 *    - Removed some >400 CRs again (Jon, you really worked
 *      a lot! ;-)
 *
 *    Revision 1.4  2001/05/21 19:32:54  jongfoster
 *    Added another #ifdef _WIN_CONSOLE
 *
 *    Revision 1.3  2001/05/20 01:11:40  jongfoster
 *    Added support for LOG_LEVEL_FATAL
 *    Renamed LOG_LEVEL_FRC to LOG_LEVEL_FORCE,
 *    and LOG_LEVEL_REF to LOG_LEVEL_RE_FILTER
 *
 *    Revision 1.2  2001/05/17 22:42:01  oes
 *     - Cleaned CRLF's from the sources and related files
 *     - Repaired logging for REF and FRC
 *
 *    Revision 1.1.1.1  2001/05/15 13:58:51  oes
 *    Initial import of version 2.9.3 source tree
 *
 *
 *********************************************************************/


#include "config.h"
#include "miscutil.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#if !defined(_WIN32) && !defined(__OS2__)
#include <unistd.h>
#endif /* !defined(_WIN32) && !defined(__OS2__) */

#include <errno.h>
#include <assert.h>
#ifdef FEATURE_PTHREAD
#include <pthread.h>
#endif /* def FEATURE_PTHREAD */

#ifdef _WIN32
#ifndef STRICT
#define STRICT
#endif
#include <windows.h>
#ifndef _WIN_CONSOLE
#include "w32log.h"
#endif /* ndef _WIN_CONSOLE */
#endif /* def _WIN32 */
#ifdef _MSC_VER
#define inline __inline
#endif /* def _MSC_VER */

#ifdef __OS2__
#include <sys/socket.h> /* For sock_errno */
#define INCL_DOS
#include <os2.h>
#endif

#include "errlog.h"
#include "project.h"
#include "jcc.h"

const char errlog_h_rcs[] = ERRLOG_H_VERSION;


/*
 * LOG_LEVEL_FATAL cannot be turned off.  (There are
 * some exceptional situations where we need to get a
 * message to the user).
 */
#define LOG_LEVEL_MINIMUM  LOG_LEVEL_FATAL

/* where to log (default: stderr) */
static FILE *logfp = NULL;

/* logging detail level.  */
int debug = (LOG_LEVEL_FATAL | LOG_LEVEL_ERROR | LOG_LEVEL_INFO);  

/* static functions */
static void fatal_error(const char * error_message);
#ifdef _WIN32
static char *w32_socket_strerr(int errcode, char *tmp_buf);
#endif
#ifdef __OS2__
static char *os2_socket_strerr(int errcode, char *tmp_buf);
#endif

#ifdef FEATURE_PTHREAD
static inline void lock_logfile()
{
   pthread_mutex_lock(&log_mutex);
}
static inline void unlock_logfile()
{
   pthread_mutex_unlock(&log_mutex);
}
static inline void lock_loginit()
{
   pthread_mutex_lock(&log_init_mutex);
}
static inline void unlock_loginit()
{
   pthread_mutex_unlock(&log_init_mutex);
}
#else /* ! FEATURE_PTHREAD */
/*
 * FIXME we need a cross-platform locking mechanism.
 * The locking/unlocking functions below should be 
 * fleshed out for non-pthread implementations.
 */ 
static inline void lock_logfile() {}
static inline void unlock_logfile() {}
static inline void lock_loginit() {}
static inline void unlock_loginit() {}
#endif

/*********************************************************************
 *
 * Function    :  fatal_error
 *
 * Description :  Displays a fatal error to standard error (or, on 
 *                a WIN32 GUI, to a dialog box), and exits
 *                JunkBuster with status code 1.
 *
 * Parameters  :
 *          1  :  error_message = The error message to display.
 *
 * Returns     :  Does not return.
 *
 *********************************************************************/
static void fatal_error(const char * error_message)
{
#if defined(_WIN32) && !defined(_WIN_CONSOLE)
   MessageBox(g_hwndLogFrame, error_message, "Privoxy Error", 
      MB_OK | MB_ICONERROR | MB_TASKMODAL | MB_SETFOREGROUND | MB_TOPMOST);  

   /* Cleanup - remove taskbar icon etc. */
   TermLogWindow();

#else /* if !defined(_WIN32) || defined(_WIN_CONSOLE) */
   fputs(error_message, stderr);
#endif /* defined(_WIN32) && !defined(_WIN_CONSOLE) */

#if defined(unix)
   unlink(pidfile);
#endif /* unix */

   exit(1);
}


/*********************************************************************
 *
 * Function    :  init_error_log
 *
 * Description :  Initializes the logging module.  Must call before
 *                calling log_error.
 *
 * Parameters  :
 *          1  :  prog_name  = The program name.
 *          2  :  logfname   = The logfile name, or NULL for stderr.
 *          3  :  debuglevel = The debugging level.
 *
 * Returns     :  N/A
 *
 *********************************************************************/
void init_error_log(const char *prog_name, const char *logfname, int debuglevel)
{
   FILE *fp;

   lock_loginit();

   /* set the logging detail level */
   debug = debuglevel | LOG_LEVEL_MINIMUM;

   if ((logfp != NULL) && (logfp != stderr))
   {
      log_error(LOG_LEVEL_INFO, "(Re-)Open logfile %s", logfname ? logfname : "none");
      lock_logfile();
      fclose(logfp);
   } else {
      lock_logfile();
   }
   logfp = stderr;
   unlock_logfile();

   /* set the designated log file */
   if( logfname )
   {
      if( NULL == (fp = fopen(logfname, "a")) )
      {
         log_error(LOG_LEVEL_FATAL, "init_error_log(): can't open logfile: %s", logfname);
      }

      /* set logging to be completely unbuffered */
      setbuf(fp, NULL);

      lock_logfile();
      logfp = fp;
      unlock_logfile();
   }

   log_error(LOG_LEVEL_INFO, "Privoxy version " VERSION);
   if (prog_name != NULL)
   {
      log_error(LOG_LEVEL_INFO, "Program name: %s", prog_name);
   }

   unlock_loginit();

} /* init_error_log */


/*********************************************************************
 *
 * Function    :  log_error
 *
 * Description :  This is the error-reporting and logging function.
 *
 * Parameters  :
 *          1  :  loglevel  = the type of message to be logged
 *          2  :  fmt       = the main string we want logged, printf-like
 *          3  :  ...       = arguments to be inserted in fmt (printf-like).
 *
 * Returns     :  N/A
 *
 *********************************************************************/
void log_error(int loglevel, char *fmt, ...)
{
   va_list ap;
   char *outbuf= NULL;
   static char *outbuf_save = NULL;
   char * src = fmt;
   int outc = 0;
   long this_thread = 1;  /* was: pthread_t this_thread;*/
#ifdef __OS2__
   PTIB     ptib;
   APIRET   ulrc;
#endif /* __OS2__ */

#if defined(_WIN32) && !defined(_WIN_CONSOLE)
   /*
    * Irrespective of debug setting, a GET/POST/CONNECT makes
    * the taskbar icon animate.  (There is an option to disable
    * this but checking that is handled inside LogShowActivity()).
    */
   if (loglevel == LOG_LEVEL_GPC)
   {
      LogShowActivity();
   }
#endif /* defined(_WIN32) && !defined(_WIN_CONSOLE) */

   /* verify if loglevel applies to current settings and bail out if negative */
   if ((loglevel & debug) == 0)
   {
      return;
   }

   /* protect the whole function because of the static buffer (outbuf) */
   lock_logfile();

   /* FIXME get current thread id */
#ifdef FEATURE_PTHREAD
   this_thread = (long)pthread_self();
#ifdef __MACH__
   /*
    * Mac OSX (and perhaps other Mach instances) doesn't have a debuggable
    * value at the first 4 bytes of pthread_self()'s return value, a pthread_t.
    * pthread_t is supposed to be opaque... but it's fairly random, though, so
    * we make it mostly presentable.
    */
   this_thread = abs(this_thread % 1000);
#endif /* def __MACH__ */
#elif defined(_WIN32)
   this_thread = GetCurrentThreadId();
#elif defined(__OS2__)
   ulrc = DosGetInfoBlocks(&ptib, NULL);
   if (ulrc == 0)
     this_thread = ptib -> tib_ptib2 -> tib2_ultid;
#endif /* def FEATURE_PTHREAD */

   if ( !outbuf_save ) 
   {
      outbuf_save = outbuf = (char*)malloc(BUFFER_SIZE);
      assert(outbuf);
   }
   outbuf = outbuf_save;

    {
       /*
        * Write timestamp into tempbuf.
        *
        * Complex because not all OSs have tm_gmtoff or
        * the %z field in strftime()
        */
       time_t now; 
       struct tm tm_now; 
       time (&now);
#ifdef HAVE_LOCALTIME_R
       tm_now = *localtime_r(&now, &tm_now);
#elif OSX_DARWIN
       pthread_mutex_lock(&localtime_mutex);
       tm_now = *localtime (&now); 
       pthread_mutex_unlock(&localtime_mutex);
#else
       tm_now = *localtime (&now); 
#endif
       strftime(outbuf, BUFFER_SIZE-6, "%b %d %H:%M:%S ", &tm_now); 
       outbuf += strlen( outbuf );
    }
   switch (loglevel)
   {
      case LOG_LEVEL_ERROR:
         outc = sprintf(outbuf, "Privoxy(%08lx) Error: ", this_thread);
         break;
      case LOG_LEVEL_FATAL:
         outc = sprintf(outbuf, "Privoxy(%08lx) Fatal error: ", this_thread);
         break;
      case LOG_LEVEL_GPC:
         outc = sprintf(outbuf, "Privoxy(%08lx) Request: ", this_thread);
         break;
      case LOG_LEVEL_CONNECT:
         outc = sprintf(outbuf, "Privoxy(%08lx) Connect: ", this_thread);
         break;
      case LOG_LEVEL_LOG:
         outc = sprintf(outbuf, "Privoxy(%08lx) Writing: ", this_thread);
         break;
      case LOG_LEVEL_HEADER:
         outc = sprintf(outbuf, "Privoxy(%08lx) Header: ", this_thread);
         break;
      case LOG_LEVEL_INFO:
         outc = sprintf(outbuf, "Privoxy(%08lx) Info: ", this_thread);
         break;
      case LOG_LEVEL_RE_FILTER:
         outc = sprintf(outbuf, "Privoxy(%08lx) Re-Filter: ", this_thread);
         break;
#ifdef FEATURE_FORCE_LOAD
      case LOG_LEVEL_FORCE:
         outc = sprintf(outbuf, "Privoxy(%08lx) Force: ", this_thread);
         break;
#endif /* def FEATURE_FORCE_LOAD */
#ifdef FEATURE_FAST_REDIRECTS
      case LOG_LEVEL_REDIRECTS:
         outc = sprintf(outbuf, "Privoxy(%08lx) Redirect: ", this_thread);
         break;
#endif /* def FEATURE_FAST_REDIRECTS */
      case LOG_LEVEL_DEANIMATE:
         outc = sprintf(outbuf, "Privoxy(%08lx) Gif-Deanimate: ", this_thread);
         break;
      case LOG_LEVEL_CLF:
         outbuf = outbuf_save;
         outc = 0;
         outbuf[0] = '\0';
         break;
#ifdef FEATURE_KILL_POPUPS
      case LOG_LEVEL_POPUPS:
         outc = sprintf(outbuf, "Privoxy(%08lx) Kill-Popups: ", this_thread);
         break;
#endif /* def FEATURE_KILL_POPUPS */
      case LOG_LEVEL_CGI:
         outc = sprintf(outbuf, "Privoxy(%08lx) CGI: ", this_thread);
         break;
      default:
         outc = sprintf(outbuf, "Privoxy(%08lx) UNKNOWN LOG TYPE(%d): ", this_thread, loglevel);
         break;
   }
   
   /* get ready to scan var. args. */
   va_start( ap, fmt );

   /* build formatted message from fmt and var-args */
   while ((*src) && (outc < BUFFER_SIZE-2))
   {
      char tempbuf[BUFFER_SIZE];
      char *sval = NULL;
      int ival;
      unsigned uval;
      long lval;
      unsigned long ulval;
      int oldoutc;
      char ch;
      
      ch = *src++;
      if( ch != '%' )
      {
         outbuf[outc++] = ch;
         continue;
      }

      ch = *src++;
      switch (ch) {
         case '%':
            outbuf[outc++] = '%';
            break;
         case 'd':
            ival = va_arg( ap, int );
            oldoutc = outc;
            outc += sprintf(tempbuf, "%d", ival);
            if (outc < BUFFER_SIZE-1) 
            {
               strcpy(outbuf + oldoutc, tempbuf);
            }
            else
            {
               outbuf[oldoutc] = '\0';
            }
            break;
         case 'u':
            uval = va_arg( ap, unsigned );
            oldoutc = outc;
            outc += sprintf(tempbuf, "%u", uval);
            if (outc < BUFFER_SIZE-1) 
            {
               strcpy(outbuf + oldoutc, tempbuf);
            }
            else
            {
               outbuf[oldoutc] = '\0';
            }
            break;
         case 'l':
            /* this is a modifier that must be followed by u or d */
            ch = *src++;
            if (ch == 'd')
            {
               lval = va_arg( ap, long );
               oldoutc = outc;
               outc += sprintf(tempbuf, "%ld", lval);
            }
            else if (ch == 'u')
            {
               ulval = va_arg( ap, unsigned long );
               oldoutc = outc;
               outc += sprintf(tempbuf, "%lu", ulval);
            }
            else
            {
               /* Error */
               sprintf(outbuf, "Privoxy(%08lx) Error: log_error(): Bad format string:\n"
                               "Format = \"%s\"\n"
                               "Exiting.", this_thread, fmt);
               if( !logfp )
               {
                  logfp = stderr;
               }
               fputs(outbuf, logfp);
               fatal_error(outbuf);
               /* Never get here */
               break;
            }
            if (outc < BUFFER_SIZE-1) 
            {
               strcpy(outbuf + oldoutc, tempbuf);
            }
            else
            {
               outbuf[oldoutc] = '\0';
            }
            break;
         case 'c':
            /*
             * Note that char paramaters are converted to int, so we need to
             * pass "int" to va_arg.  (See K&R, 2nd ed, section A7.3.2, page 202)
             */
            outbuf[outc++] = (char) va_arg( ap, int );
            break;
         case 's':
            sval = va_arg( ap, char * );
            if (sval == NULL)
            {
               sval = "[null]";
            }
            oldoutc = outc;
            outc += strlen(sval);
            if (outc < BUFFER_SIZE-1) 
            {
               strcpy(outbuf + oldoutc, sval);
            }
            else
            {
               outbuf[oldoutc] = '\0';
            }
            break;
         case 'N':
            /* Non-standard: Print a counted string.  Takes 2 parameters:
             * int length, const char * string
             */
            ival = va_arg( ap, int );
            sval = va_arg( ap, char * );
            if (sval == NULL)
            {
               sval = "[null]";
            }
            if (ival < 0)
            {
               ival = 0;
            }
            oldoutc = outc;
            outc += ival;
            if (outc < BUFFER_SIZE-1)
            {
               memcpy(outbuf + oldoutc, sval, (size_t) ival);
            }
            else
            {
               outbuf[oldoutc] = '\0';
            }
            break;
         case 'E':
            /* Non-standard: Print error code from errno */
#ifdef _WIN32
            ival = WSAGetLastError();
            sval = w32_socket_strerr(ival, tempbuf);
#elif __OS2__
            ival = sock_errno();
            if (ival != 0)
              sval = os2_socket_strerr(ival, tempbuf);
            else
            {
              ival = errno;
              sval = strerror(ival);
            }
#else /* ifndef _WIN32 */
            ival = errno; 
#ifdef HAVE_STRERROR
            sval = strerror(ival);
#else /* ifndef HAVE_STRERROR */
            sval = NULL;
#endif /* ndef HAVE_STRERROR */
            if (sval == NULL)
            {
               sprintf(tempbuf, "(errno = %d)", ival);
               sval = tempbuf;
            }
#endif /* ndef _WIN32 */
            oldoutc = outc;
            outc += strlen(sval);
            if (outc < BUFFER_SIZE-1) 
            {
               strcpy(outbuf + oldoutc, sval);
            }
            else
            {
               outbuf[oldoutc] = '\0';
            }
            break;
         case 'T':
            /* Non-standard: Print a Common Log File timestamp */
            {
               /*
                * Write timestamp into tempbuf.
                *
                * Complex because not all OSs have tm_gmtoff or
                * the %z field in strftime()
                */
               time_t now; 
               struct tm *tm_now; 
               struct tm gmt;
#ifdef HAVE_LOCALTIME_R
               struct tm dummy;
#endif
               int days, hrs, mins; 
               time (&now); 
#ifdef HAVE_GMTIME_R
               gmt = *gmtime_r(&now, &gmt);
#elif OSX_DARWIN
               pthread_mutex_lock(&gmtime_mutex);
               gmt = *gmtime(&now);
               pthread_mutex_unlock(&gmtime_mutex);
#else
               gmt = *gmtime(&now);
#endif
#ifdef HAVE_LOCALTIME_R
               tm_now = localtime_r(&now, &dummy);
#elif OSX_DARWIN
               pthread_mutex_lock(&localtime_mutex);
               tm_now = localtime (&now); 
               pthread_mutex_unlock(&localtime_mutex);
#else
               tm_now = localtime (&now); 
#endif
               days = tm_now->tm_yday - gmt.tm_yday; 
               hrs = ((days < -1 ? 24 : 1 < days ? -24 : days * 24) + tm_now->tm_hour - gmt.tm_hour); 
               mins = hrs * 60 + tm_now->tm_min - gmt.tm_min; 
               strftime (tempbuf, BUFFER_SIZE-6, "%d/%b/%Y:%H:%M:%S ", tm_now); 
               sprintf (tempbuf + strlen(tempbuf), "%+03d%02d", mins / 60, abs(mins) % 60); 
            }
            oldoutc = outc;
            outc += strlen(tempbuf);
            if (outc < BUFFER_SIZE-1) 
            {
               strcpy(outbuf + oldoutc, tempbuf);
            }
            else
            {
               outbuf[oldoutc] = '\0';
            }
            break;
         default:
            sprintf(outbuf, "Privoxy(%08lx) Error: log_error(): Bad format string:\n"
                            "Format = \"%s\"\n"
                            "Exiting.", this_thread, fmt);
            if( !logfp )
            {
               logfp = stderr;
            }
            fputs(outbuf_save, logfp);
            unlock_logfile();
            fatal_error(outbuf_save);
            /* Never get here */
            break;

      } /* switch( p ) */

   } /* for( p ... ) */
   
   /* done with var. args */
   va_end( ap );
   
   if (outc >= BUFFER_SIZE-2)
   {
      /* insufficient room for newline and trailing null. */

      static const char warning[] = "... [too long, truncated]\n";

      if (outc < BUFFER_SIZE)
      {
         /* Need to add terminating null in this case. */
         outbuf[outc] = '\0';
      }

      /* Truncate output */
      outbuf[BUFFER_SIZE - sizeof(warning)] = '\0';

      /* Append warning */
      strcat(outbuf, warning);
   }
   else
   {
      /* Add terminating newline and null */
      outbuf[outc++] = '\n';
      outbuf[outc] = '\0';
   }

   /* deal with glibc stupidity - it won't let you initialize logfp */
   if( !logfp )
   {
      logfp = stderr;
   }

   fputs(outbuf_save, logfp);

   if (loglevel == LOG_LEVEL_FATAL)
   {
      fatal_error(outbuf_save);
      /* Never get here */
   }

   unlock_logfile();

#if defined(_WIN32) && !defined(_WIN_CONSOLE)
   /* Write to display */
   LogPutString(outbuf_save);
#endif /* defined(_WIN32) && !defined(_WIN_CONSOLE) */

}


#ifdef _WIN32
/*********************************************************************
 *
 * Function    :  w32_socket_strerr
 *
 * Description :  Translate the return value from WSAGetLastError()
 *                into a string.
 *
 * Parameters  :
 *          1  :  errcode = The return value from WSAGetLastError().
 *          2  :  tmp_buf = A temporary buffer that might be used to
 *                          store the string.
 *
 * Returns     :  String representing the error code.  This may be
 *                a global string constant or a string stored in
 *                tmp_buf.
 *
 *********************************************************************/
static char *w32_socket_strerr(int errcode, char *tmp_buf)
{
#define TEXT_FOR_ERROR(code,text) \
   if (errcode == code)           \
   {                              \
      return #code " - " text;    \
   }

   TEXT_FOR_ERROR(WSAEACCES, "Permission denied")
   TEXT_FOR_ERROR(WSAEADDRINUSE, "Address already in use.")
   TEXT_FOR_ERROR(WSAEADDRNOTAVAIL, "Cannot assign requested address.");
   TEXT_FOR_ERROR(WSAEAFNOSUPPORT, "Address family not supported by protocol family.");
   TEXT_FOR_ERROR(WSAEALREADY, "Operation already in progress.");
   TEXT_FOR_ERROR(WSAECONNABORTED, "Software caused connection abort.");
   TEXT_FOR_ERROR(WSAECONNREFUSED, "Connection refused.");
   TEXT_FOR_ERROR(WSAECONNRESET, "Connection reset by peer.");
   TEXT_FOR_ERROR(WSAEDESTADDRREQ, "Destination address required.");
   TEXT_FOR_ERROR(WSAEFAULT, "Bad address.");
   TEXT_FOR_ERROR(WSAEHOSTDOWN, "Host is down.");
   TEXT_FOR_ERROR(WSAEHOSTUNREACH, "No route to host.");
   TEXT_FOR_ERROR(WSAEINPROGRESS, "Operation now in progress.");
   TEXT_FOR_ERROR(WSAEINTR, "Interrupted function call.");
   TEXT_FOR_ERROR(WSAEINVAL, "Invalid argument.");
   TEXT_FOR_ERROR(WSAEISCONN, "Socket is already connected.");
   TEXT_FOR_ERROR(WSAEMFILE, "Too many open sockets.");
   TEXT_FOR_ERROR(WSAEMSGSIZE, "Message too long.");
   TEXT_FOR_ERROR(WSAENETDOWN, "Network is down.");
   TEXT_FOR_ERROR(WSAENETRESET, "Network dropped connection on reset.");
   TEXT_FOR_ERROR(WSAENETUNREACH, "Network is unreachable.");
   TEXT_FOR_ERROR(WSAENOBUFS, "No buffer space available.");
   TEXT_FOR_ERROR(WSAENOPROTOOPT, "Bad protocol option.");
   TEXT_FOR_ERROR(WSAENOTCONN, "Socket is not connected.");
   TEXT_FOR_ERROR(WSAENOTSOCK, "Socket operation on non-socket.");
   TEXT_FOR_ERROR(WSAEOPNOTSUPP, "Operation not supported.");
   TEXT_FOR_ERROR(WSAEPFNOSUPPORT, "Protocol family not supported.");
   TEXT_FOR_ERROR(WSAEPROCLIM, "Too many processes.");
   TEXT_FOR_ERROR(WSAEPROTONOSUPPORT, "Protocol not supported.");
   TEXT_FOR_ERROR(WSAEPROTOTYPE, "Protocol wrong type for socket.");
   TEXT_FOR_ERROR(WSAESHUTDOWN, "Cannot send after socket shutdown.");
   TEXT_FOR_ERROR(WSAESOCKTNOSUPPORT, "Socket type not supported.");
   TEXT_FOR_ERROR(WSAETIMEDOUT, "Connection timed out.");
   TEXT_FOR_ERROR(WSAEWOULDBLOCK, "Resource temporarily unavailable.");
   TEXT_FOR_ERROR(WSAHOST_NOT_FOUND, "Host not found.");
   TEXT_FOR_ERROR(WSANOTINITIALISED, "Successful WSAStartup not yet performed.");
   TEXT_FOR_ERROR(WSANO_DATA, "Valid name, no data record of requested type.");
   TEXT_FOR_ERROR(WSANO_RECOVERY, "This is a non-recoverable error.");
   TEXT_FOR_ERROR(WSASYSNOTREADY, "Network subsystem is unavailable.");
   TEXT_FOR_ERROR(WSATRY_AGAIN, "Non-authoritative host not found.");
   TEXT_FOR_ERROR(WSAVERNOTSUPPORTED, "WINSOCK.DLL version out of range.");
   TEXT_FOR_ERROR(WSAEDISCON, "Graceful shutdown in progress.");
   /*
    * The following error codes are documented in the Microsoft WinSock
    * reference guide, but don't actually exist.
    *
    * TEXT_FOR_ERROR(WSA_INVALID_HANDLE, "Specified event object handle is invalid.");
    * TEXT_FOR_ERROR(WSA_INVALID_PARAMETER, "One or more parameters are invalid.");
    * TEXT_FOR_ERROR(WSAINVALIDPROCTABLE, "Invalid procedure table from service provider.");
    * TEXT_FOR_ERROR(WSAINVALIDPROVIDER, "Invalid service provider version number.");
    * TEXT_FOR_ERROR(WSA_IO_PENDING, "Overlapped operations will complete later.");
    * TEXT_FOR_ERROR(WSA_IO_INCOMPLETE, "Overlapped I/O event object not in signaled state.");
    * TEXT_FOR_ERROR(WSA_NOT_ENOUGH_MEMORY, "Insufficient memory available.");
    * TEXT_FOR_ERROR(WSAPROVIDERFAILEDINIT, "Unable to initialize a service provider.");
    * TEXT_FOR_ERROR(WSASYSCALLFAILURE, "System call failure.");
    * TEXT_FOR_ERROR(WSA_OPERATION_ABORTED, "Overlapped operation aborted.");
    */

   sprintf(tmp_buf, "(error number %d)", errcode);
   return tmp_buf;
}
#endif /* def _WIN32 */


#ifdef __OS2__
/*********************************************************************
 *
 * Function    :  os2_socket_strerr
 *
 * Description :  Translate the return value from sock_errno()
 *                into a string.
 *
 * Parameters  :
 *          1  :  errcode = The return value from sock_errno().
 *          2  :  tmp_buf = A temporary buffer that might be used to
 *                          store the string.
 *
 * Returns     :  String representing the error code.  This may be
 *                a global string constant or a string stored in
 *                tmp_buf.
 *
 *********************************************************************/
static char *os2_socket_strerr(int errcode, char *tmp_buf)
{
#define TEXT_FOR_ERROR(code,text) \
   if (errcode == code)           \
   {                              \
      return #code " - " text;    \
   }

   TEXT_FOR_ERROR(SOCEPERM          , "Not owner.")
   TEXT_FOR_ERROR(SOCESRCH          , "No such process.")
   TEXT_FOR_ERROR(SOCEINTR          , "Interrupted system call.")
   TEXT_FOR_ERROR(SOCENXIO          , "No such device or address.")
   TEXT_FOR_ERROR(SOCEBADF          , "Bad file number.")
   TEXT_FOR_ERROR(SOCEACCES         , "Permission denied.")
   TEXT_FOR_ERROR(SOCEFAULT         , "Bad address.")
   TEXT_FOR_ERROR(SOCEINVAL         , "Invalid argument.")
   TEXT_FOR_ERROR(SOCEMFILE         , "Too many open files.")
   TEXT_FOR_ERROR(SOCEPIPE          , "Broken pipe.")
   TEXT_FOR_ERROR(SOCEWOULDBLOCK    , "Operation would block.")
   TEXT_FOR_ERROR(SOCEINPROGRESS    , "Operation now in progress.")
   TEXT_FOR_ERROR(SOCEALREADY       , "Operation already in progress.")
   TEXT_FOR_ERROR(SOCENOTSOCK       , "Socket operation on non-socket.")
   TEXT_FOR_ERROR(SOCEDESTADDRREQ   , "Destination address required.")
   TEXT_FOR_ERROR(SOCEMSGSIZE       , "Message too long.")
   TEXT_FOR_ERROR(SOCEPROTOTYPE     , "Protocol wrong type for socket.")
   TEXT_FOR_ERROR(SOCENOPROTOOPT    , "Protocol not available.")
   TEXT_FOR_ERROR(SOCEPROTONOSUPPORT, "Protocol not supported.")
   TEXT_FOR_ERROR(SOCESOCKTNOSUPPORT, "Socket type not supported.")
   TEXT_FOR_ERROR(SOCEOPNOTSUPP     , "Operation not supported.")
   TEXT_FOR_ERROR(SOCEPFNOSUPPORT   , "Protocol family not supported.")
   TEXT_FOR_ERROR(SOCEAFNOSUPPORT   , "Address family not supported by protocol family.")
   TEXT_FOR_ERROR(SOCEADDRINUSE     , "Address already in use.")
   TEXT_FOR_ERROR(SOCEADDRNOTAVAIL  , "Can't assign requested address.")
   TEXT_FOR_ERROR(SOCENETDOWN       , "Network is down.")
   TEXT_FOR_ERROR(SOCENETUNREACH    , "Network is unreachable.")
   TEXT_FOR_ERROR(SOCENETRESET      , "Network dropped connection on reset.")
   TEXT_FOR_ERROR(SOCECONNABORTED   , "Software caused connection abort.")
   TEXT_FOR_ERROR(SOCECONNRESET     , "Connection reset by peer.")
   TEXT_FOR_ERROR(SOCENOBUFS        , "No buffer space available.")
   TEXT_FOR_ERROR(SOCEISCONN        , "Socket is already connected.")
   TEXT_FOR_ERROR(SOCENOTCONN       , "Socket is not connected.")
   TEXT_FOR_ERROR(SOCESHUTDOWN      , "Can't send after socket shutdown.")
   TEXT_FOR_ERROR(SOCETOOMANYREFS   , "Too many references: can't splice.")
   TEXT_FOR_ERROR(SOCETIMEDOUT      , "Operation timed out.")
   TEXT_FOR_ERROR(SOCECONNREFUSED   , "Connection refused.")
   TEXT_FOR_ERROR(SOCELOOP          , "Too many levels of symbolic links.")
   TEXT_FOR_ERROR(SOCENAMETOOLONG   , "File name too long.")
   TEXT_FOR_ERROR(SOCEHOSTDOWN      , "Host is down.")
   TEXT_FOR_ERROR(SOCEHOSTUNREACH   , "No route to host.")
   TEXT_FOR_ERROR(SOCENOTEMPTY      , "Directory not empty.")
   TEXT_FOR_ERROR(SOCEOS2ERR        , "OS/2 Error.")

   sprintf(tmp_buf, "(error number %d)", errcode);
   return tmp_buf;
}
#endif /* def __OS2__ */


/*
  Local Variables:
  tab-width: 3
  end:
*/
