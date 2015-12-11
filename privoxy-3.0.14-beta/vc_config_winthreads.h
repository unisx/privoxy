#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED
/*********************************************************************
 *
 * File        :  $Source: /cvsroot/ijbswa/current/vc_config_winthreads.h,v $
 *
 * Purpose     :  This file should be the first thing included in every
 *                .c file.  (Before even system headers).  It contains 
 *                #define statements for various features.  It was
 *                introduced because the compile command line started
 *                getting ludicrously long with feature defines.
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
 *********************************************************************/


/*
 * Version number - Major (X._._)
 */
#define VERSION_MAJOR 0

/*
 * Version number - Minor (_.X._)
 */
#define VERSION_MINOR 0

/*
 * Version number - Point (_._.X)
 */
#define VERSION_POINT 0

/*
 * Version number, as a string
 */
#define VERSION "0.0.0"

/*
 * Status of the code: alpha, beta or stable
 */
#define CODE_STATUS "UNRELEASED"

/*
 * Regular expression matching for URLs.  (Highly recommended).
 * If neither of these are defined then you can ony use prefix matching.
 * Don't bother to change this here! Use configure instead.
 */
/* #define REGEX_GNU 1 */
#define REGEX_PCRE 1

/* 
 * Should pcre be statically built in instead of linkling with libpcre?
 * (This is determined by configure depending on the availiability of
 * libpcre and user preferences). The name is ugly, but pcre needs it.
 * Don't bother to change this here! Use configure instead.
 */
#define STATIC_PCRE 1

/* 
 * Should pcrs be statically built in instead of linkling with libpcrs?
 * (This is determined by configure depending on the availiability of
 * libpcrs and user preferences).
 * Don't bother to change this here! Use configure instead.
 */
#define STATIC_PCRS 1

/*
 * Allows the use of an ACL to control access to the proxy by IP address.
 */
#define FEATURE_ACL 1

/*
 * Enables the web-based configuration (actionsfile) editor.  If you
 * have a shared proxy, you might want to turn this off.
 */
#define FEATURE_CGI_EDIT_ACTIONS 1

/*
 * Allows the use of jar files to capture cookies.
 */
#define FEATURE_COOKIE_JAR 1

/*
 * Locally redirect remote script-redirect URLs
 */
#define FEATURE_FAST_REDIRECTS 1

/*
 * Bypass filtering for 1 page only
 */
#define FEATURE_FORCE_LOAD 1

/*
 * Allow blocking using images as well as HTML.
 * If you do not define this then everything is blocked as HTML.
 *
 * Note that this is required if you want to use FEATURE_IMAGE_DETECT_MSIE.
 */
#define FEATURE_IMAGE_BLOCKING 1

/*
 * Detect image requests automatically for MSIE.  Will fall back to
 * other image-detection methods (i.e. "+image" permission) for other
 * browsers.
 *
 * You must also define FEATURE_IMAGE_BLOCKING to use this feature.
 *
 * It detects the following header pair as an image request:
 *
 * User-Agent: Mozilla/4.0 (compatible; MSIE 5.5; Windows NT 5.0)
 * Accept: * / *
 *
 * And the following as a HTML request:
 *
 * User-Agent: Mozilla/4.0 (compatible; MSIE 5.5; Windows NT 5.0)
 * Accept: image/gif, image/x-xbitmap, image/jpeg, image/pjpeg, * / *
 *
 * And no, I haven't got that backwards - IE is being wierd.
 *
 * Known limitations: 
 * 1) If you press shift-reload on a blocked HTML page, you get
 *    the image "blocked" page, not the HTML "blocked" page.
 * 2) Once an image "blocked" page has been sent, viewing it 
 *    in it's own browser window *should* bring up the HTML
 *    "blocked" page, but it doesn't.  You need to clear the 
 *    browser cache to get the HTML version again.
 *
 * These limitations are due to IE making inconsistent choices
 * about which "Accept:" header to send.
 */
#define FEATURE_IMAGE_DETECT_MSIE 1

/*
 * Use PNG instead of GIF for built-in images
 */
/* #undef FEATURE_NO_GIFS */

/*
 * Use POSIX threads instead of native threads.
 */
/* #define FEATURE_PTHREAD 1 */

/*
 * Enables statistics function.
 */
#define FEATURE_STATISTICS 1

/*
 * Allow JunkBuster to be "disabled" so it is just a normal non-blocking
 * non-anonymizing proxy.  This is useful if you're trying to access a
 * blocked or broken site - just change the setting in the config file,
 * or use the handy "Disable" menu option in the Windows GUI.
 */
#define FEATURE_TOGGLE 1

/*
 * Allows the use of trust files.
 */
#define FEATURE_TRUST 1


/****************************************************************************
 * The following values are correct for MS VC++97.
 * You should normally not change them.
 ***************************************************************************/


/*
 * Defined on Solaris only.  Makes the system libraries thread safe.
 */
/* #define _REENTRANT 1 */

/*
 * Defined on Solaris only.  Without this, many important functions are not
 * defined in the system headers.
 */
/* #define __EXTENSIONS__ 1 */

/*
 * Defined always.
 * FIXME: Don't know what it does or why we need it.
 * (presumably something to do with MultiThreading?)
 */
#define __MT__ 1

/* If the (nonstandard and thread-safe) function gethostbyname_r
 * is available, select which signature to use
 */
/* #undef HAVE_GETHOSTBYNAME_R_6_ARGS */
/* #undef HAVE_GETHOSTBYNAME_R_5_ARGS */
/* #undef HAVE_GETHOSTBYNAME_R_3_ARGS */

/* If the (nonstandard and thread-safe) function gethostbyaddr_r
 * is available, select which signature to use
 */
/* #undef HAVE_GETHOSTBYADDR_R_8_ARGS */
/* #undef HAVE_GETHOSTBYADDR_R_7_ARGS */
/* #undef HAVE_GETHOSTBYADDR_R_5_ARGS */

/* Defined if you have gmtime_r and localtime_r with a signature
 * of (struct time *, struct tm *)
 */
#undef HAVE_GMTIME_R

/* #define HAVE_LOCALTIME_R 1 */

/* Define to 'int' if <sys/socket.h> doesn't have it.
 */
#define socklen_t int

/* Define if pcre.h must be included as <pcre/pcre.h>
 */
/* #undef PCRE_H_IN_SUBDIR */

/* Define if pcreposix.h must be included as <pcre/pcreposix.h>
 */
/* #undef PCREPOSIX_H_IN_SUBDIR */


/* Define to 1 if you have the <arpa/inet.h> header file. */
#define HAVE_ARPA_INET_H 1

/* Define to 1 if you have the `atexit' function. */
#define HAVE_ATEXIT 1

/* Define if you have the `bcopy' function. */
/* #define HAVE_BCOPY 1 */

/* Define to 1 if you have the <dirent.h> header file, and it defines `DIR'.
   */
#define HAVE_DIRENT_H 1

/* Define to 1 if you have the <errno.h> header file. */
#define HAVE_ERRNO_H 1

/* Define to 1 if you have the <fcntl.h> header file. */
#define HAVE_FCNTL_H 1

/* Define to 1 if you have the `getcwd' function. */
#define HAVE_GETCWD 1

/* Define to 1 if you have the `gethostbyaddr' function. */
#define HAVE_GETHOSTBYADDR 1

/* Define to 1 if you have the `gethostbyname' function. */
#define HAVE_GETHOSTBYNAME 1

/* Define to 1 if you have the `inet_ntoa' function. */
#define HAVE_INET_NTOA 1

/* Define if you have the <inttypes.h> header file. */
/* #define HAVE_INTTYPES_H 1 */

/* Define to 1 if you have the `nsl' library (-lnsl). */
/* #undef HAVE_LIBNSL */

/* Define to 1 if you have the <limits.h> header file. */
#define HAVE_LIMITS_H 1

/* Define to 1 if you have the <locale.h> header file. */
#define HAVE_LOCALE_H 1

/* Define to 1 if you have the `localtime_r' function. */
#undef HAVE_LOCALTIME_R

/* Define to 1 if you have the `memchr' function. */
#define HAVE_MEMCHR 1

/* Define if you have the `memmove' function. */
#define HAVE_MEMMOVE 1

/* Define if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have the `memset' function. */
#define HAVE_MEMSET 1

/* Define to 1 if you have the <ndir.h> header file, and it defines `DIR'. */
/* #undef HAVE_NDIR_H */

/* Define to 1 if you have the <netdb.h> header file. */
#define HAVE_NETDB_H 1

/* Define to 1 if you have the <netinet/in.h> header file. */
#define HAVE_NETINET_IN_H 1

/* Define to 1 if you have the <OS.h> header file. */
/* #undef HAVE_OS_H */

/* Define to 1 if you have the `regcomp' function. */
#define HAVE_REGCOMP 1

/* Define to 1 if you have the `select' function. */
#define HAVE_SELECT 1

/* Define to 1 if you have the `setlocale' function. */
#define HAVE_SETLOCALE 1

/* Define to 1 if you have the `socket' function. */
#define HAVE_SOCKET 1

/* Define to 1 if you have the <stddef.h> header file. */
#define HAVE_STDDEF_H 1

/* Define if you have the <stdint.h> header file. */
/* #define HAVE_STDINT_H 1 */

/* Define if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the `strchr' function. */
#define HAVE_STRCHR 1

/* Define to 1 if you have the `strdup' function. */
#define HAVE_STRDUP 1

/* Define if you have the `strerror' function. */
#define HAVE_STRERROR 1

/* Define to 1 if you have the `strftime' function. */
#define HAVE_STRFTIME 1

/* Define if you have the <strings.h> header file. */
/* #define HAVE_STRINGS_H 1 */

/* Define if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the `strstr' function. */
#define HAVE_STRSTR 1

/* Define to 1 if you have the `strtoul' function. */
#define HAVE_STRTOUL 1

/* Define to 1 if you have the <sys/dir.h> header file, and it defines `DIR'.
   */
/* #undef HAVE_SYS_DIR_H */

/* Define to 1 if you have the <sys/ioctl.h> header file. */
#define HAVE_SYS_IOCTL_H 1

/* Define to 1 if you have the <sys/ndir.h> header file, and it defines `DIR'.
   */
/* #undef HAVE_SYS_NDIR_H */

/* Define to 1 if you have the <sys/socket.h> header file. */
#define HAVE_SYS_SOCKET_H 1

/* Define if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/timeb.h> header file. */
#define HAVE_SYS_TIMEB_H 1

/* Define to 1 if you have the <sys/time.h> header file. */
#define HAVE_SYS_TIME_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <sys/wait.h> header file. */
#define HAVE_SYS_WAIT_H 1

/* Define if you have the <unistd.h> header file. */
/* #define HAVE_UNISTD_H 1 */

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT ""

/* Define to the full name of this package. */
#define PACKAGE_NAME ""

/* Define to the full name and version of this package. */
#define PACKAGE_STRING ""

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME ""

/* Define to the version of this package. */
#define PACKAGE_VERSION ""

/* Define as the return type of signal handlers (`int' or `void'). */
#define RETSIGTYPE void

/* Define to 1 if the `setpgrp' function takes no argument. */
#define SETPGRP_VOID 1

/* The size of a `char *', as computed by sizeof. */
#define SIZEOF_CHAR_P 4

/* The size of a `int', as computed by sizeof. */
#define SIZEOF_INT 4

/* The size of a `long', as computed by sizeof. */
#define SIZEOF_LONG 4

/* The size of a `long long', as computed by sizeof. */
/* #define SIZEOF_LONG_LONG ---not supported--- */

/* The size of a `size_t', as computed by sizeof. */
#define SIZEOF_SIZE_T 4

/* Define if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Define to 1 if you can safely include both <sys/time.h> and <time.h>. */
#define TIME_WITH_SYS_TIME 1

/* Define to 1 if your <sys/time.h> declares `struct tm'. */
/* #undef TM_IN_SYS_TIME */

/* Define to empty if `const' does not conform to ANSI C. */
/* #define const */

/* Define to `int' if <sys/types.h> does not define. */
/* #undef pid_t */

/* Define to `unsigned' if <sys/types.h> does not define. */
/* #define size_t unsigned */

/* Define to 'int' if <sys/socket.h> doesn't have it. */
#define socklen_t int

/*
 * Defined always.
 * FIXME: Don't know what it does or why we need it.
 * (presumably something to do with ANSI Standard C?)
 */
/* Don't define for MS VC++ or you don't get strdup() declared.
#ifndef __STDC__
#define __STDC__ 1
#endif
*/

/*
 * Need to set up this define only for the Pthreads library for
 * Win32, available from http://sources.redhat.com/pthreads-win32/
 */
#if defined(FEATURE_PTHREAD) && defined(_WIN32)
#define __CLEANUP_C
#endif /* defined(FEATURE_PTHREAD) && defined(_WIN32) */

/*
 * Need to keep errlog.c from trying to inline the non-existent
 * locking stubs.  Could be removed once a real platform-specific
 * solution is generated.
 */
#define inline ""

/*
 * BEOS does not currently support POSIX threads.
 * This *should* be detected by ./configure, but let's be sure.
 */
#if defined(FEATURE_PTHREAD) && defined(__BEOS__)
#error BEOS does not support pthread - please run ./configure again with "--disable-pthread"

#endif /* defined(FEATURE_PTHREAD) && defined(__BEOS__) */


#if (!defined(_MSC_VER)) && (!defined(RC_INVOKED))
#error This file is only intended for MS VC++ on Win32.  For other compilers, please run configure.
#endif /* (!defined(_MSC_VER)) && (!defined(RC_INVOKED)) */

#pragma warning ( disable: 4100 4115 4201 4214 4244 4514 )

/*
 * C4100 : unreferenced formal parameter
 * Very common, not a bug
 * 
 * C4115 : named type definition in parentheses
 * #include <windows.h> causes a warning about one of these.
 *
 * C4201 : nonstandard extension used : nameless struct/union
 * Endemic in <windows.h>
 *
 * C4214 nonstandard extension used : bit field types other than int
 * Endemic in <windows.h>
 *
 * C4244 conversion from 'int' to 'char', possible loss of data
 * Should really fix this one.  Throughout the JB code.
 *
 * C4514 unreferenced inline/local function has been removed
 * Caused by #include <windows.h>
 */


#endif /* CONFIG_H_INCLUDED */

