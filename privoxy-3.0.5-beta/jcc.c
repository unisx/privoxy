const char jcc_rcs[] = "$Id: jcc.c,v 1.104 2006/09/23 13:26:38 roro Exp $";
/*********************************************************************
 *
 * File        :  $Source: /cvsroot/ijbswa/current/jcc.c,v $
 *
 * Purpose     :  Main file.  Contains main() method, main loop, and
 *                the main connection-handling function.
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
 *    $Log: jcc.c,v $
 *    Revision 1.104  2006/09/23 13:26:38  roro
 *    Replace TABs by spaces in source code.
 *
 *    Revision 1.103  2006/09/21 12:54:43  fabiankeil
 *    Fix +redirect{}. Didn't work with -fast-redirects.
 *
 *    Revision 1.102  2006/09/06 13:03:04  fabiankeil
 *    Respond with 400 and a short text message
 *    if the client tries to use Privoxy as FTP proxy.
 *
 *    Revision 1.101  2006/09/06 09:23:37  fabiankeil
 *    Make number of retries in case of forwarded-connect problems
 *    a config file option (forwarded-connect-retries) and use 0 as
 *    default.
 *
 *    Revision 1.100  2006/09/03 19:42:59  fabiankeil
 *    Set random(3) seed.
 *
 *    Revision 1.99  2006/09/02 15:36:42  fabiankeil
 *    Follow the OpenBSD port's lead and protect the resolve
 *    functions on OpenBSD as well.
 *
 *    Revision 1.98  2006/08/24 11:01:34  fabiankeil
 *    --user fix. Only use the user as group if no group is specified.
 *    Solves BR 1492612. Thanks to Spinor S. and David Laight.
 *
 *    Revision 1.97  2006/08/18 15:23:17  david__schmidt
 *    Windows service (re-)integration
 *
 *    The new args are:
 *
 *    --install[:service_name]
 *    --uninstall[:service_name]
 *    --service
 *
 *    They work as follows:
 *    --install will create a service for you and then terminate.
 *    By default the service name will be "privoxy" (without the quotes).
 *    However you can run multiple services if you wish, just by adding
 *    a colon and then a name (no spaces).
 *
 *    --uninstall follows the exact same rules a --install.
 *
 *    --service is used when the program is executed by the service
 *    control manager, and in normal circumstances would never be
 *    used as a command line argument.
 *
 *    Revision 1.96  2006/08/15 20:12:36  david__schmidt
 *    Windows service integration
 *
 *    Revision 1.95  2006/08/03 02:46:41  david__schmidt
 *    Incorporate Fabian Keil's patch work:http://www.fabiankeil.de/sourcecode/privoxy/
 *
 *    Revision 1.94  2006/07/18 14:48:46  david__schmidt
 *    Reorganizing the repository: swapping out what was HEAD (the old 3.1 branch)
 *    with what was really the latest development (the v_3_0_branch branch)
 *
 *    Revision 1.92.2.16  2005/04/03 20:10:50  david__schmidt
 *    Thanks to Jindrich Makovicka for a race condition fix for the log
 *    file.  The race condition remains for non-pthread implementations.
 *    Reference patch #1175720.
 *
 *    Revision 1.92.2.15  2004/10/03 12:53:32  david__schmidt
 *    Add the ability to check jpeg images for invalid
 *    lengths of comment blocks.  Defensive strategy
 *    against the exploit:
 *       Microsoft Security Bulletin MS04-028
 *       Buffer Overrun in JPEG Processing (GDI+) Could
 *       Allow Code Execution (833987)
 *    Enabled with +inspect-jpegs in actions files.
 *
 *    Revision 1.92.2.14  2003/12/12 12:52:53  oes
 *    - Fixed usage info for non-unix platforms
 *    - Fixed small cmdline parsing bug
 *
 *    Revision 1.92.2.13  2003/11/27 19:20:27  oes
 *    Diagnostics: Now preserve the returncode of pthread_create
 *    in errno. Closes BR #775721. Thanks to Geoffrey Hausheer.
 *
 *    Revision 1.92.2.12  2003/07/11 11:34:19  oes
 *    No longer ignore SIGCHLD. Fixes bug #769381
 *
 *    Revision 1.92.2.11  2003/05/14 12:32:02  oes
 *    Close jarfile on graceful exit, remove stray line
 *
 *    Revision 1.92.2.10  2003/05/08 15:13:46  oes
 *    Cosmetics: Killed a warning, a typo and an allocation left at exit
 *
 *    Revision 1.92.2.9  2003/04/03 15:08:42  oes
 *    No longer rely on non-POSIX.1 extensions of getcwd().
 *    Fixes bug #711001
 *
 *    Revision 1.92.2.8  2003/03/31 13:12:32  oes
 *    Replaced setenv() by posix-compliant putenv()
 *    Thanks to Neil McCalden (nmcc AT users.sf.net).
 *
 *    Revision 1.92.2.7  2003/03/17 16:48:59  oes
 *    Added chroot ability, thanks to patch by Sviatoslav Sviridov
 *
 *    Revision 1.92.2.6  2003/03/11 11:55:00  oes
 *    Clean-up and extension of improvements for forked mode:
 *     - Child's return code now consists of flags RC_FLAG_*
 *     - Reporting toggle to parent now properly #ifdef'ed
 *     - Children now report blocking to parent. This enables
 *       statistics in forked mode
 *
 *    Revision 1.92.2.5  2003/03/10 23:45:32  oes
 *    Fixed bug #700381: Non-Threaded version now capable of being toggled.
 *    Children now report having been toggled through _exit(17), parents
 *    watch for that code and toggle themselves if found.
 *
 *    Revision 1.92.2.4  2003/03/07 03:41:04  david__schmidt
 *    Wrapping all *_r functions (the non-_r versions of them) with 
 *    mutex semaphores for OSX.  Hopefully this will take care of all 
 *    of those pesky crash reports.
 *
 *    Revision 1.92.2.3  2003/02/28 12:53:06  oes
 *    Fixed two mostly harmless mem leaks
 *
 *    Revision 1.92.2.2  2002/11/20 14:37:47  oes
 *    Fix: Head of global clients list now initialized to NULL
 *
 *    Revision 1.92.2.1  2002/09/25 14:52:24  oes
 *    Added basic support for OPTIONS and TRACE HTTP methods:
 *     - New interceptor direct_response() added in chat().
 *     - sed() moved to earlier in the process, so that the
 *       Host: header is evaluated before actions and forwarding
 *       are decided on.
 *
 *    Revision 1.92  2002/05/08 16:00:46  oes
 *    Chat's buffer handling:
 *     - Fixed bug with unchecked out-of-mem conditions
 *       while reading client request & server headers
 *     - No longer predict if the buffer limit will be exceeded
 *       in the next read -- check add_to_iob's new
 *       return code. If buffer couldn't be extended
 *       (policy or out-of-mem) while
 *       - reading from client: abort
 *       - reading server headers: send error page
 *       - buffering server body for filter: flush,
 *         and if that fails: send error page
 *
 *    Revision 1.91  2002/04/08 20:35:58  swa
 *    fixed JB spelling
 *
 *    Revision 1.90  2002/04/02 14:57:28  oes
 *    Made sending wafers independent of FEATURE_COOKIE_JAR
 *
 *    Revision 1.89  2002/03/31 17:18:59  jongfoster
 *    Win32 only: Enabling STRICT to fix a VC++ compile warning.
 *
 *    Revision 1.88  2002/03/27 14:32:43  david__schmidt
 *    More compiler warning message maintenance
 *
 *    Revision 1.87  2002/03/26 22:29:54  swa
 *    we have a new homepage!
 *
 *    Revision 1.86  2002/03/25 17:04:55  david__schmidt
 *    Workaround for closing the jarfile before load_config() comes around again
 *
 *    Revision 1.85  2002/03/24 15:23:33  jongfoster
 *    Name changes
 *
 *    Revision 1.84  2002/03/24 13:25:43  swa
 *    name change related issues
 *
 *    Revision 1.83  2002/03/16 23:54:06  jongfoster
 *    Adding graceful termination feature, to help look for memory leaks.
 *    If you enable this (which, by design, has to be done by hand
 *    editing config.h) and then go to http://i.j.b/die, then the program
 *    will exit cleanly after the *next* request.  It should free all the
 *    memory that was used.
 *
 *    Revision 1.82  2002/03/13 00:27:05  jongfoster
 *    Killing warnings
 *
 *    Revision 1.81  2002/03/12 01:42:50  oes
 *    Introduced modular filters
 *
 *    Revision 1.80  2002/03/11 22:07:05  david__schmidt
 *    OS/2 port maintenance:
 *    - Fixed EMX build - it had decayed a little
 *    - Fixed inexplicable crash during FD_ZERO - must be due to a bad macro.
 *      substituted a memset for now.
 *
 *    Revision 1.79  2002/03/09 20:03:52  jongfoster
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
 *    Revision 1.78  2002/03/08 21:35:04  oes
 *    Added optional group supplement to --user option. Will now use default group of user if no group given
 *
 *    Revision 1.77  2002/03/07 03:52:06  oes
 *     - Fixed compiler warnings etc
 *     - Improved handling of failed DNS lookups
 *
 *    Revision 1.76  2002/03/06 22:54:35  jongfoster
 *    Automated function-comment nitpicking.
 *
 *    Revision 1.75  2002/03/06 10:02:19  oes
 *    Fixed stupid bug when --user was not given
 *
 *    Revision 1.74  2002/03/06 00:49:31  jongfoster
 *    Fixing warning on Windows
 *    Making #ifdefs that refer to the same variable consistently
 *    use #ifdef unix rather than mixing #ifdef unix & #ifndef OS2
 *
 *    Revision 1.73  2002/03/05 23:57:30  hal9
 *    Stray character 's' on line 1618 was breaking build.
 *
 *    Revision 1.72  2002/03/05 21:33:45  david__schmidt
 *    - Re-enable OS/2 building after new parms were added
 *    - Fix false out of memory report when resolving CGI templates when no IP
 *      address is available of failed attempt (a la no such domain)
 *
 *    Revision 1.71  2002/03/05 18:13:56  oes
 *    Added --user option
 *
 *    Revision 1.70  2002/03/05 04:52:42  oes
 *    Deleted non-errlog debugging code
 *
 *    Revision 1.69  2002/03/04 23:50:00  jongfoster
 *    Splitting off bind_port() call into bind_port_helper(), with
 *    improved logging.
 *
 *    Revision 1.68  2002/03/04 20:17:32  oes
 *    Fixed usage info
 *
 *    Revision 1.67  2002/03/04 18:18:57  oes
 *    - Removed _DEBUG mode
 *    - Cleand up cmdline parsing
 *    - Introduced --no-daemon, --pidfile options
 *    - Cleaned up signal handling:
 *      - Terminate cleanly on INT, TERM and ABRT
 *      - Schedule logfile for re-opening on HUP
 *      - Ignore CHLD and PIPE
 *      - Leave the rest with their default handlers
 *      - Uniform handler registration
 *    - Added usage() function
 *    - Played styleguide police
 *
 *    Revision 1.66  2002/03/03 15:06:55  oes
 *    Re-enabled automatic config reloading
 *
 *    Revision 1.65  2002/03/03 14:49:11  oes
 *    Fixed CLF logging: Now uses client's original HTTP request
 *
 *    Revision 1.64  2002/03/03 09:18:03  joergs
 *    Made jumbjuster work on AmigaOS again.
 *
 *    Revision 1.63  2002/03/02 04:14:50  david__schmidt
 *    Clean up a little CRLF unpleasantness that suddenly appeared
 *
 *    Revision 1.62  2002/02/20 23:17:23  jongfoster
 *    Detecting some out-of memory conditions and exiting with a log message.
 *
 *    Revision 1.61  2002/01/17 21:01:52  jongfoster
 *    Moving all our URL and URL pattern parsing code to urlmatch.c.
 *
 *    Revision 1.60  2001/12/30 14:07:32  steudten
 *    - Add signal handling (unix)
 *    - Add SIGHUP handler (unix)
 *    - Add creation of pidfile (unix)
 *    - Add action 'top' in rc file (RH)
 *    - Add entry 'SIGNALS' to manpage
 *    - Add exit message to logfile (unix)
 *
 *    Revision 1.59  2001/12/13 14:07:18  oes
 *    Fixed Bug: 503 error page now sent OK
 *
 *    Revision 1.58  2001/11/30 23:37:24  jongfoster
 *    Renaming the Win32 config file to config.txt - this is almost the
 *    same as the corresponding UNIX name "config"
 *
 *    Revision 1.57  2001/11/16 00:47:43  jongfoster
 *    Changing the tty-disconnection code to use setsid().
 *
 *    Revision 1.56  2001/11/13 20:20:54  jongfoster
 *    Tabs->spaces, fixing a bug with missing {} around an if()
 *
 *    Revision 1.55  2001/11/13 20:14:53  jongfoster
 *    Patch for FreeBSD setpgrp() as suggested by Alexander Lazic
 *
 *    Revision 1.54  2001/11/07 00:03:14  steudten
 *    Give reliable return value if an error
 *    occurs not just 0 with new daemon mode.
 *
 *    Revision 1.53  2001/11/05 21:41:43  steudten
 *    Add changes to be a real daemon just for unix os.
 *    (change cwd to /, detach from controlling tty, set
 *    process group and session leader to the own process.
 *    Add DBG() Macro.
 *    Add some fatal-error log message for failed malloc().
 *    Add '-d' if compiled with 'configure --with-debug' to
 *    enable debug output.
 *
 *    Revision 1.52  2001/10/26 20:11:20  jongfoster
 *    Fixing type mismatch
 *
 *    Revision 1.51  2001/10/26 17:38:28  oes
 *    Cosmetics
 *
 *    Revision 1.50  2001/10/25 03:40:48  david__schmidt
 *    Change in porting tactics: OS/2's EMX porting layer doesn't allow multiple
 *    threads to call select() simultaneously.  So, it's time to do a real, live,
 *    native OS/2 port.  See defines for __EMX__ (the porting layer) vs. __OS2__
 *    (native). Both versions will work, but using __OS2__ offers multi-threading.
 *
 *    Revision 1.49  2001/10/23 21:41:35  jongfoster
 *    Added call to initialize the (statically-allocated of course)
 *    "out of memory" CGI response.
 *
 *    Revision 1.48  2001/10/10 19:56:46  jongfoster
 *    Moving some code that wasn't cookie-related out of an #ifdef
 *    FEATURE_COOKIE_JAR
 *
 *    Revision 1.47  2001/10/10 16:44:36  oes
 *    Added CONNECT destination port limitation check
 *
 *    Revision 1.46  2001/10/08 15:17:41  oes
 *    Re-enabled SSL forwarding
 *
 *    Revision 1.45  2001/10/07 15:42:11  oes
 *    Replaced 6 boolean members of csp with one bitmap (csp->flags)
 *
 *    Moved downgrading of the HTTP version from parse_http_request to
 *      chat(), since we can't decide if it is necessary before we have
 *      determined the actions for the URL. The HTTP command is now
 *      *always* re-built so the repairs need no longer be special-cased.
 *
 *    filter_popups now gets a csp pointer so it can raise the new
 *      CSP_FLAG_MODIFIED flag.
 *
 *    Bugfix
 *
 *    Added configurable size limit for the IOB. If the IOB grows so
 *      large that the next read would exceed the limit, the header
 *      is generated, and the header & unfiltered buffer are flushed
 *      to the client. Chat then continues in non-buffering,
 *      non-filtering body mode.
 *
 *    Revision 1.44  2001/10/02 18:13:57  oes
 *    Ooops
 *
 *    Revision 1.43  2001/10/02 15:32:13  oes
 *    Moved generation of hdr
 *
 *    Revision 1.42  2001/09/21 23:02:02  david__schmidt
 *    Cleaning up 2 compiler warnings on OS/2.
 *
 *    Revision 1.41  2001/09/16 17:05:14  jongfoster
 *    Removing unused #include showarg.h
 *
 *    Revision 1.40  2001/09/16 15:41:45  jongfoster
 *    Fixing signed/unsigned comparison warning.
 *
 *    Revision 1.39  2001/09/16 13:21:27  jongfoster
 *    Changes to use new list functions.
 *
 *    Revision 1.38  2001/09/16 13:01:46  jongfoster
 *    Removing redundant function call that zeroed zalloc()'d memory.
 *
 *    Revision 1.37  2001/09/10 11:12:24  oes
 *    Deleted unused variable
 *
 *    Revision 1.36  2001/09/10 10:56:15  oes
 *    Silenced compiler warnings
 *
 *    Revision 1.35  2001/07/31 14:44:22  oes
 *    Deleted unused size parameter from filter_popups()
 *
 *    Revision 1.34  2001/07/30 22:08:36  jongfoster
 *    Tidying up #defines:
 *    - All feature #defines are now of the form FEATURE_xxx
 *    - Permanently turned off WIN_GUI_EDIT
 *    - Permanently turned on WEBDAV and SPLIT_PROXY_ARGS
 *
 *    Revision 1.33  2001/07/29 19:32:00  jongfoster
 *    Renaming _main() [mingw32 only] to real_main(), for ANSI compliance.
 *
 *    Revision 1.32  2001/07/29 18:47:05  jongfoster
 *    Adding missing #include "loadcfg.h"
 *
 *    Revision 1.31  2001/07/29 12:17:48  oes
 *    Applied pthread fix by Paul Lieverse
 *
 *    Revision 1.30  2001/07/25 22:57:13  jongfoster
 *    __BEOS__ no longer overrides FEATURE_PTHREAD.
 *    This is because FEATURE_PTHREAD will soon be widely used, so I
 *    want to keep it simple.
 *
 *    Revision 1.29  2001/07/24 12:47:06  oes
 *    Applied BeOS support update by Eugenia
 *
 *    Revision 1.28  2001/07/23 13:26:12  oes
 *    Fixed bug in popup-killing for the first read that caused binary garbage to be sent between headers and body
 *
 *    Revision 1.27  2001/07/19 19:09:47  haroon
 *    - Added code to take care of the situation where while processing the first
 *      server response (which includes the server header), after finding the end
 *      of the headers we were not looking past the end of the headers for
 *      content modification. I enabled it for filter_popups.
 *      Someone else should look to see if other similar operations should be
 *      done to the discarded portion of the buffer.
 *
 *      Note 2001/07/20: No, the other content modification mechanisms will process
 *                       the whole iob later anyway. --oes
 *
 *    Revision 1.26  2001/07/18 12:31:36  oes
 *    cosmetics
 *
 *    Revision 1.25  2001/07/15 19:43:49  jongfoster
 *    Supports POSIX threads.
 *    Also removed some unused #includes.
 *
 *    Revision 1.24  2001/07/13 14:00:40  oes
 *     - Generic content modification scheme:
 *       Each feature has its own applicability flag that is set
 *       from csp->action->flags.
 *       Replaced the "filtering" int flag , by a function pointer
 *       "content_filter" to the function that will do the content
 *       modification. If it is != NULL, the document will be buffered
 *       and processed through *content_filter, which must set
 *       csp->content_length and return a modified copy of the body
 *       or return NULL (on failiure).
 *     - Changed csp->is_text to the more generic bitmap csp->content_type
 *       which can currently take the valued CT_TEXT or CT_GIF
 *     - Reformatting etc
 *     - Removed all #ifdef PCRS
 *
 *    Revision 1.23  2001/07/02 02:28:25  iwanttokeepanon
 *    Added "#ifdef ACL_FILES" conditional compilation to line 1291 to exclude
 *    the `block_acl' call.  This prevents a compilation error when the user
 *    does not wish to use the "ACL" feature.
 *
 *    Revision 1.22  2001/06/29 21:45:41  oes
 *    Indentation, CRLF->LF, Tab-> Space
 *
 *    Revision 1.21  2001/06/29 13:29:36  oes
 *    - Cleaned up, improved comments
 *    - Unified all possible interceptors (CGI,
 *      block, trust, fast_redirect) in one
 *      place, with one (CGI) answer generation
 *      mechansim. Much clearer now.
 *    - Removed the GIF image generation, which
 *      is now done in filters.c:block_url()
 *    - Made error conditions like domain lookup
 *      failiure or (various) problems while talking
 *      to the server use cgi.c:error_response()
 *      instead of generating HTML/HTTP in chat() (yuck!)
 *    - Removed logentry from cancelled commit
 *
 *    Revision 1.20  2001/06/09 10:55:28  jongfoster
 *    Changing BUFSIZ ==> BUFFER_SIZE
 *
 *    Revision 1.19  2001/06/07 23:12:52  jongfoster
 *    Replacing function pointer in struct gateway with a directly
 *    called function forwarded_connect().
 *    Replacing struct gateway with struct forward_spec
 *
 *    Revision 1.18  2001/06/03 19:12:16  oes
 *    introduced new cgi handling
 *
 *    Revision 1.17  2001/06/01 20:07:23  jongfoster
 *    Now uses action +image-blocker{} rather than config->tinygif
 *
 *    Revision 1.16  2001/06/01 18:49:17  jongfoster
 *    Replaced "list_share" with "list" - the tiny memory gain was not
 *    worth the extra complexity.
 *
 *    Revision 1.15  2001/05/31 21:24:47  jongfoster
 *    Changed "permission" to "action" throughout.
 *    Removed DEFAULT_USER_AGENT - it must now be specified manually.
 *    Moved vanilla wafer check into chat(), since we must now
 *    decide whether or not to add it based on the URL.
 *
 *    Revision 1.14  2001/05/29 20:14:01  joergs
 *    AmigaOS bugfix: PCRS needs a lot of stack, stacksize for child threads
 *    increased.
 *
 *    Revision 1.13  2001/05/29 09:50:24  jongfoster
 *    Unified blocklist/imagelist/permissionslist.
 *    File format is still under discussion, but the internal changes
 *    are (mostly) done.
 *
 *    Also modified interceptor behaviour:
 *    - We now intercept all URLs beginning with one of the following
 *      prefixes (and *only* these prefixes):
 *        * http://i.j.b/
 *        * http://ijbswa.sf.net/config/
 *        * http://ijbswa.sourceforge.net/config/
 *    - New interceptors "home page" - go to http://i.j.b/ to see it.
 *    - Internal changes so that intercepted and fast redirect pages
 *      are not replaced with an image.
 *    - Interceptors now have the option to send a binary page direct
 *      to the client. (i.e. ijb-send-banner uses this)
 *    - Implemented show-url-info interceptor.  (Which is why I needed
 *      the above interceptors changes - a typical URL is
 *      "http://i.j.b/show-url-info?url=www.somesite.com/banner.gif".
 *      The previous mechanism would not have intercepted that, and
 *      if it had been intercepted then it then it would have replaced
 *      it with an image.)
 *
 *    Revision 1.12  2001/05/27 22:17:04  oes
 *
 *    - re_process_buffer no longer writes the modified buffer
 *      to the client, which was very ugly. It now returns the
 *      buffer, which it is then written by chat.
 *
 *    - content_length now adjusts the Content-Length: header
 *      for modified documents rather than crunch()ing it.
 *      (Length info in csp->content_length, which is 0 for
 *      unmodified documents)
 *
 *    - For this to work, sed() is called twice when filtering.
 *
 *    Revision 1.11  2001/05/26 17:27:53  jongfoster
 *    Added support for CLF and fixed LOG_LEVEL_LOG.
 *    Also did CRLF->LF fix of my previous patch.
 *
 *    Revision 1.10  2001/05/26 15:26:15  jongfoster
 *    ACL feature now provides more security by immediately dropping
 *    connections from untrusted hosts.
 *
 *    Revision 1.9  2001/05/26 00:28:36  jongfoster
 *    Automatic reloading of config file.
 *    Removed obsolete SIGHUP support (Unix) and Reload menu option (Win32).
 *    Most of the global variables have been moved to a new
 *    struct configuration_spec, accessed through csp->config->globalname
 *    Most of the globals remaining are used by the Win32 GUI.
 *
 *    Revision 1.8  2001/05/25 22:43:18  jongfoster
 *    Fixing minor memory leak and buffer overflow.
 *
 *    Revision 1.7  2001/05/25 22:34:30  jongfoster
 *    Hard tabs->Spaces
 *
 *    Revision 1.6  2001/05/23 00:13:58  joergs
 *    AmigaOS support fixed.
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
 *    Revision 1.4  2001/05/21 19:34:01  jongfoster
 *    Made failure to bind() a fatal error.
 *
 *    Revision 1.3  2001/05/20 01:21:20  jongfoster
 *    Version 2.9.4 checkin.
 *    - Merged popupfile and cookiefile, and added control over PCRS
 *      filtering, in new "permissionsfile".
 *    - Implemented LOG_LEVEL_FATAL, so that if there is a configuration
 *      file error you now get a message box (in the Win32 GUI) rather
 *      than the program exiting with no explanation.
 *    - Made killpopup use the PCRS MIME-type checking and HTTP-header
 *      skipping.
 *    - Removed tabs from "config"
 *    - Moved duplicated url parsing code in "loaders.c" to a new funcition.
 *    - Bumped up version number.
 *
 *    Revision 1.2  2001/05/17 22:34:44  oes
 *     - Added hint on GIF char array generation to jcc.c
 *     - Cleaned CRLF's from the sources and related files
 *     - Repaired logging for REF and FRC
 *
 *    Revision 1.1.1.1  2001/05/15 13:58:56  oes
 *    Initial import of version 2.9.3 source tree
 *
 *
 *********************************************************************/


#include "config.h"

#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>

#ifdef FEATURE_PTHREAD
#include <pthread.h>
#endif /* def FEATURE_PTHREAD */

#ifdef _WIN32
# ifndef FEATURE_PTHREAD
#  ifndef STRICT
#   define STRICT
#  endif
#  include <windows.h>
#  include <process.h>
# endif /* ndef FEATURE_PTHREAD */

# include "win32.h"
# ifndef _WIN_CONSOLE
#  include "w32log.h"
# endif /* ndef _WIN_CONSOLE */
# include "w32svrapi.h"

#else /* ifndef _WIN32 */

# if !defined (__OS2__)
# include <unistd.h>
# include <sys/wait.h>
# endif /* ndef __OS2__ */
# include <sys/time.h>
# include <sys/stat.h>
# include <sys/ioctl.h>

#ifdef sun
#include <sys/termios.h>
#endif /* sun */

#ifdef unix
#include <pwd.h>
#include <grp.h>
#endif

# include <signal.h>

# ifdef __BEOS__
#  include <socket.h>  /* BeOS has select() for sockets only. */
#  include <OS.h>      /* declarations for threads and stuff. */
# endif

# if defined(__EMX__) || defined(__OS2__)
#  include <sys/select.h>  /* OS/2/EMX needs a little help with select */
# endif
# ifdef __OS2__
#define INCL_DOS
# include <os2.h>
#define bzero(B,N) memset(B,0x00,n)
# endif

# ifndef FD_ZERO
#  include <select.h>
# endif

#endif

#include "project.h"
#include "list.h"
#include "jcc.h"
#include "filters.h"
#include "loaders.h"
#include "parsers.h"
#include "killpopup.h"
#include "miscutil.h"
#include "errlog.h"
#include "jbsockets.h"
#include "gateway.h"
#include "actions.h"
#include "cgi.h"
#include "loadcfg.h"
#include "urlmatch.h"

const char jcc_h_rcs[] = JCC_H_VERSION;
const char project_h_rcs[] = PROJECT_H_VERSION;

int no_daemon = 0;
struct client_state  clients[1];
struct file_list     files[1];

#ifdef FEATURE_STATISTICS
int urls_read     = 0;     /* total nr of urls read inc rejected */
int urls_rejected = 0;     /* total nr of urls rejected */
#endif /* def FEATURE_STATISTICS */

#ifdef FEATURE_GRACEFUL_TERMINATION
int g_terminate = 0;
#endif

static void listen_loop(void);
static void chat(struct client_state *csp);
#ifdef AMIGA
void serve(struct client_state *csp);
#else /* ifndef AMIGA */
static void serve(struct client_state *csp);
#endif /* def AMIGA */

#ifdef __BEOS__
static int32 server_thread(void *data);
#endif /* def __BEOS__ */

#ifdef _WIN32
#define sleep(N)  Sleep(((N) * 1000))
#endif

#ifdef __OS2__
#define sleep(N)  DosSleep(((N) * 100))
#endif

#if defined(OSX_DARWIN) || defined(__OpenBSD__)
#ifdef OSX_DARWIN
/*
 * Hit OSX over the head with a hammer.  Protect all *_r functions.
 */
pthread_mutex_t gmtime_mutex;
pthread_mutex_t localtime_mutex;
#endif /* def OSX_DARWIN */
/*
 * Protect only the resolve functions for OpenBSD.
 */ 
pthread_mutex_t gethostbyaddr_mutex;
pthread_mutex_t gethostbyname_mutex;
#endif /* defined(OSX_DARWIN) || defined(__OpenBSD__) */

#ifdef FEATURE_PTHREAD
pthread_mutex_t log_mutex;
pthread_mutex_t log_init_mutex;
#endif /* FEATURE_PTHREAD */

#if defined(unix) || defined(__EMX__)
const char *basedir = NULL;
const char *pidfile = NULL;
int received_hup_signal = 0;
#endif /* defined unix */

/* The vanilla wafer. */
static const char VANILLA_WAFER[] =
   "NOTICE=TO_WHOM_IT_MAY_CONCERN_"
   "Do_not_send_me_any_copyrighted_information_other_than_the_"
   "document_that_I_am_requesting_or_any_of_its_necessary_components._"
   "In_particular_do_not_send_me_any_cookies_that_"
   "are_subject_to_a_claim_of_copyright_by_anybody._"
   "Take_notice_that_I_refuse_to_be_bound_by_any_license_condition_"
   "(copyright_or_otherwise)_applying_to_any_cookie._";


#if !defined(_WIN32) && !defined(__OS2__) && !defined(AMIGA)
/*********************************************************************
 *
 * Function    :  sig_handler 
 *
 * Description :  Signal handler for different signals.
 *                Exit gracefully on ABRT, TERM and  INT
 *                or set a flag that will cause the errlog
 *                to be reopened by the main thread on HUP.
 *
 * Parameters  :
 *          1  :  the_signal = the signal cause this function to call
 *
 * Returns     :  - 
 *
 *********************************************************************/
static void sig_handler(int the_signal)
{
   switch(the_signal)
   {
      case SIGABRT:
      case SIGTERM:
      case SIGINT:
         log_error(LOG_LEVEL_INFO, "exiting by signal %d .. bye", the_signal);
#if defined(unix)
         unlink(pidfile);
#endif /* unix */
         exit(the_signal);
         break;

      case SIGHUP:
         received_hup_signal = 1;
         break;         

      default:
         /* 
          * We shouldn't be here, unless we catch signals
          * in main() that we can't handle here!
          */
         log_error(LOG_LEVEL_FATAL, "sig_handler: exiting on unexpected signal %d", the_signal);
   }
   return;

}
#endif


/*********************************************************************
 *
 * Function    :  chat
 *
 * Description :  Once a connection to the client has been accepted,
 *                this function is called (via serve()) to handle the
 *                main business of the communication.  When this
 *                function returns, the caller must close the client
 *                socket handle.
 *
 * Parameters  :
 *          1  :  csp = Current client state (buffers, headers, etc...)
 *
 * Returns     :  On success, the number of bytes written are returned (zero
 *                indicates nothing was written).  On error, -1 is returned,
 *                and errno is set appropriately.  If count is zero and the
 *                file descriptor refers to a regular file, 0 will be
 *                returned without causing any other effect.  For a special
 *                file, the results are not portable.
 *
 *********************************************************************/
static void chat(struct client_state *csp)
{
/*
 * This next lines are a little ugly, but they simplifies the if statements
 * below.  Basically if TOGGLE, then we want the if to test if the
 * CSP_FLAG_TOGGLED_ON flag ist set, else we don't.  And if FEATURE_FORCE_LOAD,
 * then we want the if to test for CSP_FLAG_FORCED , else we don't
 */
#ifdef FEATURE_TOGGLE
#   define IS_TOGGLED_ON_AND (csp->flags & CSP_FLAG_TOGGLED_ON) &&
#else /* ifndef FEATURE_TOGGLE */
#   define IS_TOGGLED_ON_AND
#endif /* ndef FEATURE_TOGGLE */
#ifdef FEATURE_FORCE_LOAD
#   define IS_NOT_FORCED_AND !(csp->flags & CSP_FLAG_FORCED) &&
#else /* ifndef FEATURE_FORCE_LOAD */
#   define IS_NOT_FORCED_AND
#endif /* def FEATURE_FORCE_LOAD */

#define IS_ENABLED_AND   IS_TOGGLED_ON_AND IS_NOT_FORCED_AND

   char buf[BUFFER_SIZE];
   char *hdr;
   char *p;
   char *req;
   fd_set rfds;
   int n;
   jb_socket maxfd;
   int server_body;
   int ms_iis5_hack = 0;
   int byte_count = 0;
   unsigned int forwarded_connect_retries = 0;
   unsigned int max_forwarded_connect_retries = csp->config->forwarded_connect_retries;
   const struct forward_spec * fwd;
   struct http_request *http;
   int len; /* for buffer sizes */
#ifdef FEATURE_KILL_POPUPS
   int block_popups;         /* bool, 1==will block popups */
   int block_popups_now = 0; /* bool, 1==currently blocking popups */
#endif /* def FEATURE_KILL_POPUPS */

   int pcrs_filter;        /* bool, 1==will filter through pcrs */
   int gif_deanimate;      /* bool, 1==will deanimate gifs */
   int jpeg_inspect;       /* bool, 1==will inspect jpegs */

   /* Function that does the content filtering for the current request */
   char *(*content_filter)() = NULL;

   /* Skeleton for HTTP response, if we should intercept the request */
   struct http_response *rsp;

   http = csp->http;

   /*
    * Read the client's request.  Note that since we're not using select() we
    * could get blocked here if a client connected, then didn't say anything!
    */

   for (;;)
   {
      len = read_socket(csp->cfd, buf, sizeof(buf));

      if (len <= 0) break;      /* error! */
      
      /*
       * If there is no memory left for buffering the
       * request, there is nothing we can do but hang up
       */
      if (add_to_iob(csp, buf, len))
      {
         return;
      }

      req = get_header(csp);

      if (req == NULL)
      {
         break;    /* no HTTP request! */
      }

      if (*req == '\0')
      {
         continue;   /* more to come! */
      }

#ifdef FEATURE_FORCE_LOAD
      /* If this request contains the FORCE_PREFIX,
       * better get rid of it now and set the force flag --oes
       */

      if (strstr(req, FORCE_PREFIX))
      {
         strclean(req, FORCE_PREFIX);
         log_error(LOG_LEVEL_FORCE, "Enforcing request \"%s\".\n", req);
         csp->flags |= CSP_FLAG_FORCED;
      }

#endif /* def FEATURE_FORCE_LOAD */

      parse_http_request(req, http, csp);
      freez(req);
      break;
   }

   if (http->cmd == NULL)
   {
      strcpy(buf, CHEADER);
      write_socket(csp->cfd, buf, strlen(buf));

      log_error(LOG_LEVEL_CLF, "%s - - [%T] \" \" 400 0", csp->ip_addr_str);

      free_http_request(http);
      return;
   }

   if (!strncmpic(http->cmd, "GET ftp://", 10))
   {
      strcpy(buf, FTP_RESPONSE);
      write_socket(csp->cfd, buf, strlen(buf));

      log_error(LOG_LEVEL_ERROR, "%s tried to use Privoxy as FTP proxy: %s",
         csp->ip_addr_str, http->cmd);

      free_http_request(http);
      return;
   }

   /* decide how to route the HTTP request */

   if ((fwd = forward_url(http, csp)) == NULL)
   {
      log_error(LOG_LEVEL_FATAL, "gateway spec is NULL!?!?  This can't happen!");
      /* Never get here - LOG_LEVEL_FATAL causes program exit */
   }

   /* build the http request to send to the server
    * we have to do one of the following:
    *
    * create = use the original HTTP request to create a new
    *          HTTP request that has either the path component
    *          without the http://domainspec (w/path) or the
    *          full orininal URL (w/url)
    *          Note that the path and/or the HTTP version may
    *          have been altered by now.
    *
    * connect = Open a socket to the host:port of the server
    *           and short-circuit server and client socket.
    *
    * pass =  Pass the request unchanged if forwarding a CONNECT
    *         request to a parent proxy. Note that we'll be sending
    *         the CFAIL message ourselves if connecting to the parent
    *         fails, but we won't send a CSUCCEED message if it works,
    *         since that would result in a double message (ours and the
    *         parent's). After sending the request to the parent, we simply
    *         tunnel.
    *
    * here's the matrix:
    *                        SSL
    *                    0        1
    *                +--------+--------+
    *                |        |        |
    *             0  | create | connect|
    *                | w/path |        |
    *  Forwarding    +--------+--------+
    *                |        |        |
    *             1  | create | pass   |
    *                | w/url  |        |
    *                +--------+--------+
    *
    */

   /*
    * Determine the actions for this URL
    */
#ifdef FEATURE_TOGGLE
   if (!(csp->flags & CSP_FLAG_TOGGLED_ON))
   {
      /* Most compatible set of actions (i.e. none) */
      init_current_action(csp->action);
   }
   else
#endif /* ndef FEATURE_TOGGLE */
   {
      url_actions(http, csp);
   }


   /*
    * Check if a CONNECT request is allowable:
    * In the absence of a +limit-connect action, allow only port 443.
    * If there is an action, allow whatever matches the specificaton.
    */
   if(http->ssl)
   {
      if(  ( !(csp->action->flags & ACTION_LIMIT_CONNECT) && csp->http->port != 443)
           || (csp->action->flags & ACTION_LIMIT_CONNECT
              && !match_portlist(csp->action->string[ACTION_STRING_LIMIT_CONNECT], csp->http->port)) )
      {
         if (csp->action->flags & ACTION_TREAT_FORBIDDEN_CONNECTS_LIKE_BLOCKS)
         {
            /* The response will violate the specs, but makes unblocking easier. */
            log_error(LOG_LEVEL_ERROR, "Marking suspicious CONNECT request from %s for blocking.",
               csp->ip_addr_str);
            csp->action->flags |= ACTION_BLOCK;
            http->ssl = 0;
         }
         else
         {
            strcpy(buf, CFORBIDDEN);
            write_socket(csp->cfd, buf, strlen(buf));
            log_error(LOG_LEVEL_CONNECT, "Denying suspicious CONNECT request from %s", csp->ip_addr_str);
            log_error(LOG_LEVEL_CLF, "%s - - [%T] \" \" 403 0", csp->ip_addr_str);
            return;
         }
      }
   }


   /*
    * Downgrade http version from 1.1 to 1.0 if +downgrade
    * action applies
    */
   if ( (http->ssl == 0)
     && (!strcmpic(http->ver, "HTTP/1.1"))
     && (csp->action->flags & ACTION_DOWNGRADE))
   {
      freez(http->ver);
      http->ver = strdup("HTTP/1.0");

      if (http->ver == NULL)
      {
         log_error(LOG_LEVEL_FATAL, "Out of memory downgrading HTTP version");
      }
   }

   /* 
    * Save a copy of the original request for logging
    */
   http->ocmd = strdup(http->cmd);

   if (http->ocmd == NULL)
   {
      log_error(LOG_LEVEL_FATAL, "Out of memory copying HTTP request line");
   }

   /*
    * (Re)build the HTTP request for non-SSL requests.
    * If forwarding, use the whole URL, else, use only the path.
    */
   if (http->ssl == 0)
   {
      freez(http->cmd);

      http->cmd = strdup(http->gpc);
      string_append(&http->cmd, " ");

      if (fwd->forward_host)
      {
         string_append(&http->cmd, http->url);
      }
      else
      {
         string_append(&http->cmd, http->path);
      }
      string_append(&http->cmd, " ");
      string_append(&http->cmd, http->ver);

      if (http->cmd == NULL)
      {
         log_error(LOG_LEVEL_FATAL, "Out of memory writing HTTP command");
      }
      log_error(LOG_LEVEL_HEADER, "New HTTP Request-Line: %s", http->cmd);
   }
   enlist(csp->headers, http->cmd);


   /*
    * If the user has not supplied any wafers, and the user has not
    * told us to suppress the vanilla wafer, then send the vanilla wafer.
    */
   if (list_is_empty(csp->action->multi[ACTION_MULTI_WAFER])
       && ((csp->action->flags & ACTION_VANILLA_WAFER) != 0))
   {
      enlist(csp->action->multi[ACTION_MULTI_WAFER], VANILLA_WAFER);
   }


#ifdef FEATURE_KILL_POPUPS
   block_popups               = ((csp->action->flags & ACTION_NO_POPUPS) != 0);
#endif /* def FEATURE_KILL_POPUPS */

   pcrs_filter                = (csp->rlist != NULL) &&  /* There are expressions to be used */
                                (!list_is_empty(csp->action->multi[ACTION_MULTI_FILTER]));

   gif_deanimate              = ((csp->action->flags & ACTION_DEANIMATE) != 0);

   jpeg_inspect               = ((csp->action->flags & ACTION_JPEG_INSPECT) != 0);

   /* grab the rest of the client's headers */

   for (;;)
   {
      if ( ( ( p = get_header(csp) ) != NULL) && ( *p == '\0' ) )
      {
         len = read_socket(csp->cfd, buf, sizeof(buf));
         if (len <= 0)
         {
            log_error(LOG_LEVEL_ERROR, "read from client failed: %E");
            return;
         }
         
         /*
          * If there is no memory left for buffering the
          * request, there is nothing we can do but hang up
          */
         if (add_to_iob(csp, buf, len))
         {
            return;
         }
         continue;
      }

      if (p == NULL) break;

      enlist(csp->headers, p);
      freez(p);
   }

   /*
    * We have a request. Now, check to see if we need to
    * intercept it, i.e. If ..
    */

   if (
       /* We may not forward the request by rfc2616 sect 14.31 */
       (NULL != (rsp = direct_response(csp)))

       /* or a CGI call was detected and answered */
       || (NULL != (rsp = dispatch_cgi(csp)))

       /* or we are enabled and... */
       || (IS_ENABLED_AND (

            /* ..the request was blocked */
          ( NULL != (rsp = block_url(csp)))

          /* ..or untrusted */
#ifdef FEATURE_TRUST
          || ( NULL != (rsp = trust_url(csp)))
#endif /* def FEATURE_TRUST */

          /* ..or a fast redirect kicked in */
#ifdef FEATURE_FAST_REDIRECTS
          || ( NULL != (rsp = redirect_url(csp)))
#endif /* def FEATURE_FAST_REDIRECTS */
          ))
      )
   {
      /* Write the answer to the client */
      if (write_socket(csp->cfd, rsp->head, rsp->head_length)
       || write_socket(csp->cfd, rsp->body, rsp->content_length))
      {
         log_error(LOG_LEVEL_ERROR, "write to: %s failed: %E", http->host);
      }

#ifdef FEATURE_STATISTICS
      /* Count as a rejected request */
      csp->flags |= CSP_FLAG_REJECTED;
#endif /* def FEATURE_STATISTICS */

      /* Log (FIXME: All intercept reasons appear as "crunch" with Status 200) */
      log_error(LOG_LEVEL_GPC, "%s%s crunch!", http->hostport, http->path);
      log_error(LOG_LEVEL_CLF, "%s - - [%T] \"%s\" 200 3", csp->ip_addr_str, http->ocmd);

      /* Clean up and return */
      free_http_response(rsp);
      return;
   }

   hdr = sed(client_patterns, add_client_headers, csp);
   if (hdr == NULL)
   {
      /* FIXME Should handle error properly */
      log_error(LOG_LEVEL_FATAL, "Out of memory parsing client header");
   }

   list_remove_all(csp->headers);

   log_error(LOG_LEVEL_GPC, "%s%s", http->hostport, http->path);

   if (fwd->forward_host)
   {
      log_error(LOG_LEVEL_CONNECT, "via %s:%d to: %s",
               fwd->forward_host, fwd->forward_port, http->hostport);
   }
   else
   {
      log_error(LOG_LEVEL_CONNECT, "to %s", http->hostport);
   }

   /* here we connect to the server, gateway, or the forwarder */

   while ( (csp->sfd = forwarded_connect(fwd, http, csp))
         && (errno == EINVAL) && (forwarded_connect_retries++ < max_forwarded_connect_retries))
   {
      log_error(LOG_LEVEL_ERROR, "failed request #%u to connect to %s. Trying again.",
                forwarded_connect_retries, http->hostport);
   }

   if (csp->sfd == JB_INVALID_SOCKET)
   {
      log_error(LOG_LEVEL_CONNECT, "connect to: %s failed: %E",
                http->hostport);

      if (errno == EINVAL)
      {
         rsp = error_response(csp, "no-such-domain", errno);

         log_error(LOG_LEVEL_CLF, "%s - - [%T] \"%s\" 404 0",
                   csp->ip_addr_str, http->ocmd);
      }
      else
      {
         rsp = error_response(csp, "connect-failed", errno);

         log_error(LOG_LEVEL_CLF, "%s - - [%T] \"%s\" 503 0",
                   csp->ip_addr_str, http->ocmd);
      }


      /* Write the answer to the client */
      if(rsp)
      {
         if (write_socket(csp->cfd, rsp->head, rsp->head_length)
          || write_socket(csp->cfd, rsp->body, rsp->content_length))
         {
            log_error(LOG_LEVEL_ERROR, "write to: %s failed: %E", http->host);
         }
      }

      free_http_response(rsp);
      freez(hdr);
      return;
   }

   log_error(LOG_LEVEL_CONNECT, "OK");

   if (fwd->forward_host || (http->ssl == 0))
   {
      /* write the client's (modified) header to the server
       * (along with anything else that may be in the buffer)
       */

      if (write_socket(csp->sfd, hdr, strlen(hdr))
       || (flush_socket(csp->sfd, csp) <  0))
      {
         log_error(LOG_LEVEL_CONNECT, "write header to: %s failed: %E",
                    http->hostport);

         log_error(LOG_LEVEL_CLF, "%s - - [%T] \"%s\" 503 0",
                   csp->ip_addr_str, http->ocmd);

         rsp = error_response(csp, "connect-failed", errno);

         if(rsp)
         {
            if (write_socket(csp->cfd, rsp->head, rsp->head_length)
             || write_socket(csp->cfd, rsp->body, rsp->content_length))
            {
               log_error(LOG_LEVEL_ERROR, "write to: %s failed: %E", http->host);
            }
         }

         free_http_response(rsp);
         freez(hdr);
         return;
      }
   }
   else
   {
      /*
       * We're running an SSL tunnel and we're not forwarding,
       * so just send the "connect succeeded" message to the
       * client, flush the rest, and get out of the way.
       */
      log_error(LOG_LEVEL_CLF, "%s - - [%T] \"%s\" 200 2\n",
                csp->ip_addr_str, http->ocmd);

      if (write_socket(csp->cfd, CSUCCEED, sizeof(CSUCCEED)-1))
      {
         freez(hdr);
         return;
      }
      IOB_RESET(csp);
   }

   /* we're finished with the client's header */
   freez(hdr);

   maxfd = ( csp->cfd > csp->sfd ) ? csp->cfd : csp->sfd;

   /* pass data between the client and server
    * until one or the other shuts down the connection.
    */

   server_body = 0;

   for (;;)
   {
#ifdef __OS2__
      /*
       * FD_ZERO here seems to point to an errant macro which crashes.
       * So do this by hand for now...
       */
      memset(&rfds,0x00,sizeof(fd_set));
#else
      FD_ZERO(&rfds);
#endif
      FD_SET(csp->cfd, &rfds);
      FD_SET(csp->sfd, &rfds);

      n = select((int)maxfd+1, &rfds, NULL, NULL, NULL);

      if (n < 0)
      {
         log_error(LOG_LEVEL_ERROR, "select() failed!: %E");
         return;
      }

      /* this is the body of the browser's request
       * just read it and write it.
       */

      if (FD_ISSET(csp->cfd, &rfds))
      {
         len = read_socket(csp->cfd, buf, sizeof(buf));

         if (len <= 0)
         {
            break; /* "game over, man" */
         }

         if (write_socket(csp->sfd, buf, (size_t)len))
         {
            log_error(LOG_LEVEL_ERROR, "write to: %s failed: %E", http->host);
            return;
         }
         continue;
      }

      /*
       * The server wants to talk.  It could be the header or the body.
       * If `hdr' is null, then it's the header otherwise it's the body.
       * FIXME: Does `hdr' really mean `host'? No.
       */


      if (FD_ISSET(csp->sfd, &rfds))
      {
         fflush( 0 );
         len = read_socket(csp->sfd, buf, sizeof(buf) - 1);

         if (len < 0)
         {
            log_error(LOG_LEVEL_ERROR, "read from: %s failed: %E", http->host);

            log_error(LOG_LEVEL_CLF, "%s - - [%T] \"%s\" 503 0",
                      csp->ip_addr_str, http->ocmd);

            rsp = error_response(csp, "connect-failed", errno);

            if(rsp)
            {
               if (write_socket(csp->cfd, rsp->head, rsp->head_length)
                || write_socket(csp->cfd, rsp->body, rsp->content_length))
               {
                  log_error(LOG_LEVEL_ERROR, "write to: %s failed: %E", http->host);
               }
            }

            free_http_response(rsp);
            return;
         }

         /* Add a trailing zero.  This lets filter_popups
          * use string operations.
          */
         buf[len] = '\0';

#ifdef FEATURE_KILL_POPUPS
         /* Filter the popups on this read. */
         if (block_popups_now)
         {
            filter_popups(buf, csp);
         }
#endif /* def FEATURE_KILL_POPUPS */

         /* Normally, this would indicate that we've read
          * as much as the server has sent us and we can
          * close the client connection.  However, Microsoft
          * in its wisdom has released IIS/5 with a bug that
          * prevents it from sending the trailing \r\n in
          * a 302 redirect header (and possibly other headers).
          * To work around this if we've haven't parsed
          * a full header we'll append a trailing \r\n
          * and see if this now generates a valid one.
          *
          * This hack shouldn't have any impacts.  If we've
          * already transmitted the header or if this is a
          * SSL connection, then we won't bother with this
          * hack.  So we only work on partially received
          * headers.  If we append a \r\n and this still
          * doesn't generate a valid header, then we won't
          * transmit anything to the client.
          */
         if (len == 0)
         {

            if (server_body || http->ssl)
            {
               /*
                * If we have been buffering up the document,
                * now is the time to apply content modification
                * and send the result to the client.
                */
               if (content_filter)
               {
                  /*
                   * If the content filter fails, use the original
                   * buffer and length.
                   * (see p != NULL ? p : csp->iob->cur below)
                   */
                  if (NULL == (p = (*content_filter)(csp)))
                  {
                     csp->content_length = csp->iob->eod - csp->iob->cur;
                  }

                  hdr = sed(server_patterns_light, NULL, csp);

                  if (hdr == NULL)
                  {
                     /* FIXME Should handle error properly */
                     log_error(LOG_LEVEL_FATAL, "Out of memory parsing server header");
                  }

                  if (write_socket(csp->cfd, hdr, strlen(hdr))
                   || write_socket(csp->cfd, p != NULL ? p : csp->iob->cur, csp->content_length))
                  {
                     log_error(LOG_LEVEL_ERROR, "write modified content to client failed: %E");
                     freez(hdr);
                     freez(p);
                     return;
                  }

                  freez(hdr);
                  freez(p);
               }

               break; /* "game over, man" */
            }

            /*
             * This is NOT the body, so
             * Let's pretend the server just sent us a blank line.
             */
            len = sprintf(buf, "\r\n");

            /*
             * Now, let the normal header parsing algorithm below do its
             * job.  If it fails, we'll exit instead of continuing.
             */

            ms_iis5_hack = 1;
         }

         /*
          * If this is an SSL connection or we're in the body
          * of the server document, just write it to the client,
          * unless we need to buffer the body for later content-filtering
          */

         if (server_body || http->ssl)
         {
            if (content_filter)
            {
               /*
                * If there is no memory left for buffering the content, or the buffer limit
                * has been reached, switch to non-filtering mode, i.e. make & write the
                * header, flush the iob and buf, and get out of the way.
                */
               if (add_to_iob(csp, buf, len))
               {
                  size_t hdrlen;
                  int flushed;

                  log_error(LOG_LEVEL_ERROR, "Flushing header and buffers. Stepping back from filtering.");

                  hdr = sed(server_patterns, add_server_headers, csp);
                  if (hdr == NULL)
                  {
                     /* 
                      * Memory is too tight to even generate the header.
                      * Send our static "Out-of-memory" page.
                      */
                     log_error(LOG_LEVEL_ERROR, "Out of memory while trying to flush.");
                     rsp = cgi_error_memory();

                     if (write_socket(csp->cfd, rsp->head, rsp->head_length)
                         || write_socket(csp->cfd, rsp->body, rsp->content_length))
                     {
                        log_error(LOG_LEVEL_ERROR, "write to: %s failed: %E", http->host);
                     }
                     return;
                  }

                  hdrlen = strlen(hdr);

                  if (write_socket(csp->cfd, hdr, hdrlen)
                   || ((flushed = flush_socket(csp->cfd, csp)) < 0)
                   || (write_socket(csp->cfd, buf, (size_t) len)))
                  {
                     log_error(LOG_LEVEL_CONNECT, "Flush header and buffers to client failed: %E");

                     freez(hdr);
                     return;
                  }

                  byte_count += hdrlen + flushed + len;
                  freez(hdr);
                  content_filter = NULL;
                  server_body = 1;

               }
            }
            else
            {
               if (write_socket(csp->cfd, buf, (size_t)len))
               {
                  log_error(LOG_LEVEL_ERROR, "write to client failed: %E");
                  return;
               }
            }
            byte_count += len;
            continue;
         }
         else
         {
            /* we're still looking for the end of the
             * server's header ... (does that make header
             * parsing an "out of body experience" ?
             */

            /* 
             * buffer up the data we just read.  If that fails, 
             * there's little we can do but send our static
             * out-of-memory page.
             */
            if (add_to_iob(csp, buf, len))
            {
               log_error(LOG_LEVEL_ERROR, "Out of memory while looking for end of server headers.");
               rsp = cgi_error_memory();
               
               if (write_socket(csp->cfd, rsp->head, rsp->head_length)
                   || write_socket(csp->cfd, rsp->body, rsp->content_length))
               {
                  log_error(LOG_LEVEL_ERROR, "write to: %s failed: %E", http->host);
               }
               return;
            }

            /* get header lines from the iob */

            while ((p = get_header(csp)) != NULL)
            {
               if (*p == '\0')
               {
                  /* see following note */
                  break;
               }
               enlist(csp->headers, p);
               freez(p);
            }

            /* NOTE: there are no "empty" headers so
             * if the pointer `p' is not NULL we must
             * assume that we reached the end of the
             * buffer before we hit the end of the header.
             */

            if (p)
            {
               if (ms_iis5_hack)
               {
                  /* Well, we tried our MS IIS/5
                   * hack and it didn't work.
                   * The header is incomplete
                   * and there isn't anything
                   * we can do about it.
                   */
                  break;
               }
               else
               {
                  /* Since we have to wait for
                   * more from the server before
                   * we can parse the headers
                   * we just continue here.
                   */
                  continue;
               }
            }

            /* we have now received the entire header.
             * filter it and send the result to the client
             */

            hdr = sed(server_patterns, add_server_headers, csp);
            if (hdr == NULL)
            {
               /* FIXME Should handle error properly */
               log_error(LOG_LEVEL_FATAL, "Out of memory parsing server header");
            }

#ifdef FEATURE_KILL_POPUPS
            /* Start blocking popups if appropriate. */

            if ((csp->content_type & CT_TEXT) &&  /* It's a text / * MIME-Type */
                !http->ssl    &&                  /* We talk plaintext */
                block_popups)                     /* Policy allows */
            {
               block_popups_now = 1;
               /*
                * Filter the part of the body that came in the same read
                * as the last headers:
                */
               filter_popups(csp->iob->cur, csp);
            }

#endif /* def FEATURE_KILL_POPUPS */

            /* Buffer and pcrs filter this if appropriate. */

            if ((csp->content_type & CT_TEXT) &&  /* It's a text / * MIME-Type */
                !http->ssl    &&                  /* We talk plaintext */
                pcrs_filter)                      /* Policy allows */
            {
               content_filter = pcrs_filter_response;
            }

            /* Buffer and gif_deanimate this if appropriate. */

            if ((csp->content_type & CT_GIF)  &&  /* It's an image/gif MIME-Type */
                !http->ssl    &&                  /* We talk plaintext */
                gif_deanimate)                    /* Policy allows */
            {
               content_filter = gif_deanimate_response;
            }

            /* Buffer and jpg_inspect this if appropriate. */

            if ((csp->content_type & CT_JPEG)  &&  /* It's an image/jpeg MIME-Type */
                !http->ssl    &&                   /* We talk plaintext */
                jpeg_inspect)                      /* Policy allows */
            {
               content_filter = jpeg_inspect_response;
            }

            /*
             * Only write if we're not buffering for content modification
             */
            if (!content_filter)
            {
               /* write the server's (modified) header to
                * the client (along with anything else that
                * may be in the buffer)
                */

               if (write_socket(csp->cfd, hdr, strlen(hdr))
                || ((len = flush_socket(csp->cfd, csp)) < 0))
               {
                  log_error(LOG_LEVEL_CONNECT, "write header to client failed: %E");

                  /* the write failed, so don't bother
                   * mentioning it to the client...
                   * it probably can't hear us anyway.
                   */
                  freez(hdr);
                  return;
               }

               byte_count += len;
            }

            /* we're finished with the server's header */

            freez(hdr);
            server_body = 1;

            /* If this was a MS IIS/5 hack then it means
             * the server has already closed the
             * connection.  Nothing more to read.  Time
             * to bail.
             */
            if (ms_iis5_hack)
            {
               break;
            }
         }
         continue;
      }

      return; /* huh? we should never get here */
   }

   log_error(LOG_LEVEL_CLF, "%s - - [%T] \"%s\" 200 %d",
             csp->ip_addr_str, http->ocmd, byte_count);
}


/*********************************************************************
 *
 * Function    :  serve
 *
 * Description :  This is little more than chat.  We only "serve" to
 *                to close any socket that chat may have opened.
 *
 * Parameters  :
 *          1  :  csp = Current client state (buffers, headers, etc...)
 *
 * Returns     :  N/A
 *
 *********************************************************************/
#ifdef AMIGA
void serve(struct client_state *csp)
#else /* ifndef AMIGA */
static void serve(struct client_state *csp)
#endif /* def AMIGA */
{
   chat(csp);
   close_socket(csp->cfd);

   if (csp->sfd != JB_INVALID_SOCKET)
   {
      close_socket(csp->sfd);
   }

   csp->flags &= ~CSP_FLAG_ACTIVE;

}


#ifdef __BEOS__
/*********************************************************************
 *
 * Function    :  server_thread
 *
 * Description :  We only exist to call `serve' in a threaded environment.
 *
 * Parameters  :
 *          1  :  data = Current client state (buffers, headers, etc...)
 *
 * Returns     :  Always 0.
 *
 *********************************************************************/
static int32 server_thread(void *data)
{
   serve((struct client_state *) data);
   return 0;

}
#endif


/*********************************************************************
 *
 * Function    :  usage
 *
 * Description :  Print usage info & exit.
 *
 * Parameters  :  Pointer to argv[0] for identifying ourselves
 *
 * Returns     :  No. ,-)
 *
 *********************************************************************/
void usage(const char *myname)
{
   printf("Privoxy version " VERSION " (" HOME_PAGE_URL ")\n"
#if !defined(unix)
           "Usage: %s [--help] [--version] [configfile]\n"
#else
           "Usage: %s [--help] [--version] [--no-daemon] [--pidfile pidfile] [--user user[.group]] [configfile]\n"
#endif
           "Aborting.\n", myname);
 
   exit(2);

}


/*********************************************************************
 *
 * Function    :  main
 *
 * Description :  Load the config file and start the listen loop.
 *                This function is a lot more *sane* with the `load_config'
 *                and `listen_loop' functions; although it stills does
 *                a *little* too much for my taste.
 *
 * Parameters  :
 *          1  :  argc = Number of parameters (including $0).
 *          2  :  argv = Array of (char *)'s to the parameters.
 *
 * Returns     :  1 if : can't open config file, unrecognized directive,
 *                stats requested in multi-thread mode, can't open the
 *                log file, can't open the jar file, listen port is invalid,
 *                any load fails, and can't bind port.
 *
 *                Else main never returns, the process must be signaled
 *                to terminate execution.  Or, on Windows, use the
 *                "File", "Exit" menu option.
 *
 *********************************************************************/
#ifdef __MINGW32__
int real_main(int argc, const char *argv[])
#else
int main(int argc, const char *argv[])
#endif
{
   int argc_pos = 0;
#ifdef HAVE_RANDOM
   unsigned int random_seed;
#endif /* ifdef HAVE_RANDOM */
#ifdef unix
   struct passwd *pw = NULL;
   struct group *grp = NULL;
   char *p;
   int do_chroot = 0;
#endif

   Argc = argc;
   Argv = argv;

   configfile =
#if !defined(_WIN32)
   "config"
#else
   "config.txt"
#endif
      ;

   /*
    * Parse the command line arguments
    */
   while (++argc_pos < argc)
   {
#ifdef _WIN32
      /* Check to see if the service must be installed or uninstalled */
      if (strncmp(argv[argc_pos], "--install", 9) == 0)
      {
         const char *pName = argv[argc_pos] + 9;
         if (*pName == ':')
            pName++;
         exit( (install_service(pName)) ? 0 : 1 );
      }
      else if (strncmp(argv[argc_pos], "--uninstall", + 11) == 0)
      {
         const char *pName = argv[argc_pos] + 11;
         if (*pName == ':')
            pName++;
         exit((uninstall_service(pName)) ? 0 : 1);
      }
      else if (strcmp(argv[argc_pos], "--service" ) == 0)
      {
         bRunAsService = TRUE;
         w32_set_service_cwd();
         atexit(w32_service_exit_notify);
      }
      else
#endif /* defined(_WIN32) */


#if !defined(_WIN32) || defined(_WIN_CONSOLE)

      if (strcmp(argv[argc_pos], "--help") == 0)
      {
         usage(argv[0]);
      }

      else if(strcmp(argv[argc_pos], "--version") == 0)
      {
         printf("Privoxy version " VERSION " (" HOME_PAGE_URL ")\n");
         exit(0);
      }

#if defined(unix)

     else if (strcmp(argv[argc_pos], "--no-daemon" ) == 0)
      {
         no_daemon = 1;
      }

      else if (strcmp(argv[argc_pos], "--pidfile" ) == 0)
      {
         if (++argc_pos == argc) usage(argv[0]);
         pidfile = strdup(argv[argc_pos]);
      }

      else if (strcmp(argv[argc_pos], "--user" ) == 0)
      {
         if (++argc_pos == argc) usage(argv[argc_pos]);

         if ((NULL != (p = strchr(argv[argc_pos], '.'))) && *(p + 1) != '0')
         {
            *p++ = '\0';
            if (NULL == (grp = getgrnam(p)))
            {
               log_error(LOG_LEVEL_FATAL, "Group %s not found.", p);
            }
         }

         if (NULL == (pw = getpwnam(argv[argc_pos])))
         {
            log_error(LOG_LEVEL_FATAL, "User %s not found.", argv[argc_pos]);
         }

         if (p != NULL) *--p = '\0';
      }

      else if (strcmp(argv[argc_pos], "--chroot" ) == 0)
      {
         do_chroot = 1;
      }

#endif /* defined(unix) */
#endif /* defined(_WIN32) && !defined(_WIN_CONSOLE) */
      {
         configfile = argv[argc_pos];
      }

   } /* -END- while (more arguments) */

#if defined(unix)
   if ( *configfile != '/' )
   {
      char *abs_file, cwd[1024];

      /* make config-filename absolute here */
      if ( !(getcwd(cwd, sizeof(cwd))))
      {
         perror("get working dir failed");
         exit( 1 );
      }

      if (!(basedir = strdup(cwd))
      || (!(abs_file = malloc( strlen( basedir ) + strlen( configfile ) + 5 ))))
      {
         perror("malloc failed");
         exit( 1 );
      }
      strcpy( abs_file, basedir );
      strcat( abs_file, "/" );
      strcat( abs_file, configfile );
      configfile = abs_file;
   }
#endif /* defined unix */


   files->next = NULL;
   clients->next = NULL;

#ifdef AMIGA
   InitAmiga();
#elif defined(_WIN32)
   InitWin32();
#endif

#if defined(OSX_DARWIN) || defined(__OpenBSD__)
   /*
    * Prepare global mutex semaphores
    */
#ifdef OSX_DARWIN
   pthread_mutex_init(&gmtime_mutex,0);
   pthread_mutex_init(&localtime_mutex,0);
#endif /* def OSX_DARWIN */
   pthread_mutex_init(&gethostbyaddr_mutex,0);
   pthread_mutex_init(&gethostbyname_mutex,0);
#endif /* defined(OSX_DARWIN) || defined(__OpenBSD__) */

#ifdef FEATURE_PTHREAD
   pthread_mutex_init(&log_mutex,0);
   pthread_mutex_init(&log_init_mutex,0);
#endif /* FEATURE_PTHREAD */

#ifdef HAVE_RANDOM
   random_seed = (unsigned int)time(NULL);
   srandom(random_seed);
#endif /* ifdef HAVE_RANDOM */

   /*
    * Unix signal handling
    *
    * Catch the abort, interrupt and terminate signals for a graceful exit
    * Catch the hangup signal so the errlog can be reopened.
    * Ignore the broken pipe signals (FIXME: Why?)
    */
#if !defined(_WIN32) && !defined(__OS2__) && !defined(AMIGA)
{
   int idx;
   const int catched_signals[] = { SIGABRT, SIGTERM, SIGINT, SIGHUP, 0 };
   const int ignored_signals[] = { SIGPIPE, 0 };

   for (idx = 0; catched_signals[idx] != 0; idx++)
   {
      if (signal(catched_signals[idx], sig_handler) == SIG_ERR)
      {
         log_error(LOG_LEVEL_FATAL, "Can't set signal-handler for signal %d: %E", catched_signals[idx]);
      }
   }

   for (idx = 0; ignored_signals[idx] != 0; idx++)
   {
      if (signal(ignored_signals[idx], SIG_IGN) == SIG_ERR)
      {
         log_error(LOG_LEVEL_FATAL, "Can't set ignore-handler for signal %d: %E", ignored_signals[idx]);
      }
   }

}
#else /* ifdef _WIN32 */
# ifdef _WIN_CONSOLE
   /*
    * We *are* in a windows console app.
    * Print a verbose messages about FAQ's and such
    */
   printf(win32_blurb);
# endif /* def _WIN_CONSOLE */
#endif /* def _WIN32 */


   /* Initialize the CGI subsystem */
   cgi_init_error_messages();

   /*
    * If runnig on unix and without the --nodaemon
    * option, become a daemon. I.e. fork, detach
    * from tty and get process group leadership
    */
#if defined(unix)
{
   pid_t pid = 0;
#if 0
   int   fd;
#endif

   if (!no_daemon)
   {
      pid  = fork();

      if ( pid < 0 ) /* error */
      {
         perror("fork");
         exit( 3 );
      }
      else if ( pid != 0 ) /* parent */
      {
         int status;
         pid_t wpid;
         /*
          * must check for errors
          * child died due to missing files aso
          */
         sleep( 1 );
         wpid = waitpid( pid, &status, WNOHANG );
         if ( wpid != 0 )
         {
            exit( 1 );
         }
         exit( 0 );
      }
      /* child */
#if 1
      /* Should be more portable, but not as well tested */
      setsid();
#else /* !1 */
#ifdef __FreeBSD__
      setpgrp(0,0);
#else /* ndef __FreeBSD__ */
      setpgrp();
#endif /* ndef __FreeBSD__ */
      fd = open("/dev/tty", O_RDONLY);
      if ( fd )
      {
         /* no error check here */
         ioctl( fd, TIOCNOTTY,0 );
         close ( fd );
      }
#endif /* 1 */
      /* FIXME: should close stderr (fd 2) here too, but the test
       * for existence
       * and load config file is done in listen_loop() and puts
       * some messages on stderr there.
       */

      close( 0 );
      close( 1 );
      chdir("/");

   } /* -END- if (!no_daemon) */

   /*
    * As soon as we have written the PID file, we can switch
    * to the user and group ID indicated by the --user option
    */
   write_pid_file();
   
   if (NULL != pw)
   {
      if (setgid((NULL != grp) ? grp->gr_gid : pw->pw_gid))
      {
         log_error(LOG_LEVEL_FATAL, "Cannot setgid(): Insufficient permissions.");
      }
      if (do_chroot)
      {
         if (!pw->pw_dir)
         {
            log_error(LOG_LEVEL_FATAL, "Home directory for %s undefined", pw->pw_name);
         }
         if (chroot(pw->pw_dir) < 0)
         {
            log_error(LOG_LEVEL_FATAL, "Cannot chroot to %s", pw->pw_dir);
         }
         if (chdir ("/"))
         {
            log_error(LOG_LEVEL_FATAL, "Cannot chdir /");
         }
      }
      if (setuid(pw->pw_uid))
      {
         log_error(LOG_LEVEL_FATAL, "Cannot setuid(): Insufficient permissions.");
      }
      if (do_chroot)
      {
         char putenv_dummy[64];

         strcpy(putenv_dummy, "HOME=/");
         if (putenv(putenv_dummy) != 0)
         {
            log_error(LOG_LEVEL_FATAL, "Cannot putenv(): HOME");
         }                

         snprintf(putenv_dummy, 64, "USER=%s", pw->pw_name);
         if (putenv(putenv_dummy) != 0)
         {
            log_error(LOG_LEVEL_FATAL, "Cannot putenv(): USER");
         }
      }
   }
   else if (do_chroot)
   {
      log_error(LOG_LEVEL_FATAL, "Cannot chroot without --user argument.");
   }
}
#endif /* defined unix */

#ifdef _WIN32
   /* This will be FALSE unless the command line specified --service
    */
   if (bRunAsService)
   {
      /* Yup, so now we must attempt to establish a connection 
       * with the service dispatcher. This will only work if this
       * process was launched by the service control manager to
       * actually run as a service. If this isn't the case, i've
       * known it take around 30 seconds or so for the call to return.
       */

      /* The StartServiceCtrlDispatcher won't return until the service is stopping */
      if (w32_start_service_ctrl_dispatcher(w32ServiceDispatchTable))
      {
         /* Service has run, and at this point is now being stopped, so just return */
         return 0;
      }

#ifdef _WIN_CONSOLE
      printf("Warning: Failed to connect to Service Control Dispatcher\nwhen starting as a service!\n");
#endif
      /* An error occurred. Usually it's because --service was wrongly specified
       * and we were unable to connect to the Service Control Dispatcher because
       * it wasn't expecting us and is therefore not listening.
       *
       * For now, just continue below to call the listen_loop function.
       */
   }
#endif /* def _WIN32 */

   listen_loop();

   /* NOTREACHED */
   return(-1);

}


/*********************************************************************
 *
 * Function    :  bind_port_helper
 *
 * Description :  Bind the listen port.  Handles logging, and aborts
 *                on failure.
 *
 * Parameters  :
 *          1  :  config = Privoxy configuration.  Specifies port
 *                         to bind to.
 *
 * Returns     :  Port that was opened.
 *
 *********************************************************************/
static jb_socket bind_port_helper(struct configuration_spec * config)
{
   int result;
   jb_socket bfd;

   if ( (config->haddr != NULL)
     && (config->haddr[0] == '1')
     && (config->haddr[1] == '2')
     && (config->haddr[2] == '7')
     && (config->haddr[3] == '.') )
   {
      log_error(LOG_LEVEL_INFO, "Listening on port %d for local connections only",
                config->hport);
   }
   else if (config->haddr == NULL)
   {
      log_error(LOG_LEVEL_INFO, "Listening on port %d on all IP addresses",
                config->hport);
   }
   else
   {
      log_error(LOG_LEVEL_INFO, "Listening on port %d on IP address %s",
                config->hport, config->haddr);
   }

   result = bind_port(config->haddr, config->hport, &bfd);

   if (result < 0)
   {
      switch(result)
      {
         case -3 :
            log_error(LOG_LEVEL_FATAL, "can't bind to %s:%d: "
               "There may be another Privoxy or some other "
               "proxy running on port %d",
               (NULL != config->haddr) ? config->haddr : "INADDR_ANY",
                      config->hport, config->hport);

         case -2 :
            log_error(LOG_LEVEL_FATAL, "can't bind to %s:%d: " 
               "The hostname is not resolvable",
               (NULL != config->haddr) ? config->haddr : "INADDR_ANY", config->hport);

         default :
            log_error(LOG_LEVEL_FATAL, "can't bind to %s:%d: because %E",
               (NULL != config->haddr) ? config->haddr : "INADDR_ANY", config->hport);
      }

      /* shouldn't get here */
      return JB_INVALID_SOCKET;
   }

   config->need_bind = 0;

   return bfd;
}


#ifdef _WIN32
/* Without this simple workaround we get this compiler warning from _beginthread
 *     warning C4028: formal parameter 1 different from declaration
 */
void w32_service_listen_loop(void *p)
{
   listen_loop();
}
#endif /* def _WIN32 */


/*********************************************************************
 *
 * Function    :  listen_loop
 *
 * Description :  bind the listen port and enter a "FOREVER" listening loop.
 *
 * Parameters  :  N/A
 *
 * Returns     :  Never.
 *
 *********************************************************************/
static void listen_loop(void)
{
   struct client_state *csp = NULL;
   jb_socket bfd;
   struct configuration_spec * config;

   config = load_config();

   bfd = bind_port_helper(config);

#ifdef FEATURE_GRACEFUL_TERMINATION
   while (!g_terminate)
#else
   for (;;)
#endif
   {
#if !defined(FEATURE_PTHREAD) && !defined(_WIN32) && !defined(__BEOS__) && !defined(AMIGA) && !defined(__OS2__)
      while (waitpid(-1, NULL, WNOHANG) > 0)
      {
         /* zombie children */
      }
#endif /* !defined(FEATURE_PTHREAD) && !defined(_WIN32) && !defined(__BEOS__) && !defined(AMIGA) */

      /*
       * Free data that was used by died threads
       */
      sweep();

#if defined(unix)
      /*
       * Re-open the errlog after HUP signal
       */
      if (received_hup_signal)
      {
         init_error_log(Argv[0], config->logfile, config->debug);
         received_hup_signal = 0;
      }
#endif

#ifdef __OS2__
#ifdef FEATURE_COOKIE_JAR
      /*
       * Need a workaround here: we have to fclose() the jarfile, or we die because it's
       * already open.  I think unload_configfile() is not being run, which should do
       * this work.  Until that can get resolved, we'll use this workaround.
       */
       if (csp)
         if(csp->config)
           if (csp->config->jar)
           {
             fclose(csp->config->jar);
             csp->config->jar = NULL;
           }
#endif /* FEATURE_COOKIE_JAR */
#endif /* __OS2__ */

      if ( NULL == (csp = (struct client_state *) zalloc(sizeof(*csp))) )
      {
         log_error(LOG_LEVEL_FATAL, "malloc(%d) for csp failed: %E", sizeof(*csp));
         continue;
      }

      csp->flags |= CSP_FLAG_ACTIVE;
      csp->sfd    = JB_INVALID_SOCKET;

      csp->config = config = load_config();

      if ( config->need_bind )
      {
         /*
          * Since we were listening to the "old port", we will not see
          * a "listen" param change until the next IJB request.  So, at
          * least 1 more request must be made for us to find the new
          * setting.  I am simply closing the old socket and binding the
          * new one.
          *
          * Which-ever is correct, we will serve 1 more page via the
          * old settings.  This should probably be a "show-proxy-args"
          * request.  This should not be a so common of an operation
          * that this will hurt people's feelings.
          */

         close_socket(bfd);

         bfd = bind_port_helper(config);
      }

      log_error(LOG_LEVEL_CONNECT, "accept connection ... ");

      if (!accept_connection(csp, bfd))
      {
         log_error(LOG_LEVEL_CONNECT, "accept failed: %E");

#ifdef AMIGA
         if(!childs)
         {
            exit(1);
         }
#endif
         freez(csp);
         continue;
      }
      else
      {
         log_error(LOG_LEVEL_CONNECT, "OK");
      }

#ifdef FEATURE_TOGGLE
      if (global_toggle_state)
      {
         csp->flags |= CSP_FLAG_TOGGLED_ON;
      }
#endif /* def FEATURE_TOGGLE */

      if (run_loader(csp))
      {
         log_error(LOG_LEVEL_FATAL, "a loader failed - must exit");
         /* Never get here - LOG_LEVEL_FATAL causes program exit */
      }

#ifdef FEATURE_ACL
      if (block_acl(NULL,csp))
      {
         log_error(LOG_LEVEL_CONNECT, "Connection dropped due to ACL");
         close_socket(csp->cfd);
         freez(csp);
         continue;
      }
#endif /* def FEATURE_ACL */

      /* add it to the list of clients */
      csp->next = clients->next;
      clients->next = csp;

      if (config->multi_threaded)
      {
         int child_id;

/* this is a switch () statment in the C preprocessor - ugh */
#undef SELECTED_ONE_OPTION

/* Use Pthreads in preference to native code */
#if defined(FEATURE_PTHREAD) && !defined(SELECTED_ONE_OPTION)
#define SELECTED_ONE_OPTION
         {
            pthread_t the_thread;
            pthread_attr_t attrs;

            pthread_attr_init(&attrs);
            pthread_attr_setdetachstate(&attrs, PTHREAD_CREATE_DETACHED);
            errno = pthread_create(&the_thread, &attrs,
               (void*)serve, csp);
            child_id = errno ? -1 : 0;
            pthread_attr_destroy(&attrs);
         }
#endif

#if defined(_WIN32) && !defined(_CYGWIN) && !defined(SELECTED_ONE_OPTION)
#define SELECTED_ONE_OPTION
         child_id = _beginthread(
            (void (*)(void *))serve,
            64 * 1024,
            csp);
#endif

#if defined(__OS2__) && !defined(SELECTED_ONE_OPTION)
#define SELECTED_ONE_OPTION
         child_id = _beginthread(
            (void(* _Optlink)(void*))serve,
            NULL,
            64 * 1024,
            csp);
#endif

#if defined(__BEOS__) && !defined(SELECTED_ONE_OPTION)
#define SELECTED_ONE_OPTION
         {
            thread_id tid = spawn_thread
               (server_thread, "server", B_NORMAL_PRIORITY, csp);

            if ((tid >= 0) && (resume_thread(tid) == B_OK))
            {
               child_id = (int) tid;
            }
            else
            {
               child_id = -1;
            }
         }
#endif

#if defined(AMIGA) && !defined(SELECTED_ONE_OPTION)
#define SELECTED_ONE_OPTION
         csp->cfd = ReleaseSocket(csp->cfd, -1);
         
         if((child_id = (int)CreateNewProcTags(
            NP_Entry, (ULONG)server_thread,
            NP_Output, Output(),
            NP_CloseOutput, FALSE,
            NP_Name, (ULONG)"privoxy child",
            NP_StackSize, 200*1024,
            TAG_DONE)))
         {
            childs++;
            ((struct Task *)child_id)->tc_UserData = csp;
            Signal((struct Task *)child_id, SIGF_SINGLE);
            Wait(SIGF_SINGLE);
         }
#endif

#if !defined(SELECTED_ONE_OPTION)
         child_id = fork();

         /* This block is only needed when using fork().
          * When using threads, the server thread was
          * created and run by the call to _beginthread().
          */
         if (child_id == 0)   /* child */
         {
            int rc = 0;
#ifdef FEATURE_TOGGLE
            int inherited_toggle_state = global_toggle_state;
#endif /* def FEATURE_TOGGLE */

            serve(csp);

            /* 
             * If we've been toggled or we've blocked the request, tell Mom
             */

#ifdef FEATURE_TOGGLE
            if (inherited_toggle_state != global_toggle_state)
            {
               rc |= RC_FLAG_TOGGLED;
            }
#endif /* def FEATURE_TOGGLE */

#ifdef FEATURE_STATISTICS  
            if (csp->flags & CSP_FLAG_REJECTED)
            {
               rc |= RC_FLAG_BLOCKED;
            }
#endif /* ndef FEATURE_STATISTICS */

            _exit(rc);
         }
         else if (child_id > 0) /* parent */
         {
            /* in a fork()'d environment, the parent's
             * copy of the client socket and the CSP
             * are not used.
             */
            int child_status;
#if !defined(_WIN32) && !defined(__CYGWIN__)

            wait( &child_status );

            /* 
             * Evaluate child's return code: If the child has
             *  - been toggled, toggle ourselves
             *  - blocked its request, bump up the stats counter
             */

#ifdef FEATURE_TOGGLE
            if (WIFEXITED(child_status) && (WEXITSTATUS(child_status) & RC_FLAG_TOGGLED))
            {
               global_toggle_state = !global_toggle_state;
            }
#endif /* def FEATURE_TOGGLE */

#ifdef FEATURE_STATISTICS
            urls_read++;
            if (WIFEXITED(child_status) && (WEXITSTATUS(child_status) & RC_FLAG_BLOCKED))
            {
               urls_rejected++;
            }
#endif /* def FEATURE_STATISTICS */ 

#endif /* !defined(_WIN32) && defined(__CYGWIN__) */
            close_socket(csp->cfd);
            csp->flags &= ~CSP_FLAG_ACTIVE;
         }
#endif

#undef SELECTED_ONE_OPTION
/* end of cpp switch () */

         if (child_id < 0) /* failed */
         {
            char buf[BUFFER_SIZE];

            log_error(LOG_LEVEL_ERROR, "can't fork: %E");

            sprintf(buf , "Privoxy: can't fork: errno = %d", errno);

            write_socket(csp->cfd, buf, strlen(buf));
            close_socket(csp->cfd);
            csp->flags &= ~CSP_FLAG_ACTIVE;
            sleep(5);
            continue;
         }
      }
      else
      {
         serve(csp);
      }
   }

   /* NOTREACHED unless FEATURE_GRACEFUL_TERMINATION is defined */

   /* Clean up.  Aim: free all memory (no leaks) */
#ifdef FEATURE_GRACEFUL_TERMINATION

   log_error(LOG_LEVEL_ERROR, "Graceful termination requested");

   unload_current_config_file();
   unload_current_actions_file();
   unload_current_re_filterfile();
#ifdef FEATURE_TRUST
   unload_current_trust_file();
#endif

   if (config->multi_threaded)
   {
      int i = 60;
      do
      {
         sleep(1);
         sweep();
      } while ((clients->next != NULL) && (--i > 0));

      if (i <= 0)
      {
         log_error(LOG_LEVEL_ERROR, "Graceful termination failed - still some live clients after 1 minute wait.");
      }
   }
   sweep();
   sweep();

#if defined(unix)
   freez(basedir);
#endif
   freez(configfile);

#ifdef FEATURE_COOKIE_JAR
   if (NULL != config->jar)
   {
      fclose(config->jar);
   }
#endif

#if defined(_WIN32) && !defined(_WIN_CONSOLE)
   /* Cleanup - remove taskbar icon etc. */
   TermLogWindow();
#endif

   exit(0);
#endif /* FEATURE_GRACEFUL_TERMINATION */

}


/*
  Local Variables:
  tab-width: 3
  end:
*/
