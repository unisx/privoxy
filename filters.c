const char filters_rcs[] = "$Id: filters.c,v 1.66 2006/09/23 13:26:38 roro Exp $";
/*********************************************************************
 *
 * File        :  $Source: /cvsroot/ijbswa/current/filters.c,v $
 *
 * Purpose     :  Declares functions to parse/crunch headers and pages.
 *                Functions declared include:
 *                   `acl_addr', `add_stats', `block_acl', `block_imageurl',
 *                   `block_url', `url_actions', `domain_split',
 *                   `filter_popups', `forward_url', 'redirect_url',
 *                   `ij_untrusted_url', `intercept_url', `pcrs_filter_respose',
 *                   `ijb_send_banner', `trust_url', `gif_deanimate_response',
 *                   `jpeg_inspect_response'
 *
 * Copyright   :  Written by and Copyright (C) 2001, 2004 the SourceForge
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
 *    $Log: filters.c,v $
 *    Revision 1.66  2006/09/23 13:26:38  roro
 *    Replace TABs by spaces in source code.
 *
 *    Revision 1.65  2006/09/21 12:54:43  fabiankeil
 *    Fix +redirect{}. Didn't work with -fast-redirects.
 *
 *    Revision 1.64  2006/08/31 10:55:49  fabiankeil
 *    Block requests for untrusted URLs with status
 *    code 403 instead of 200.
 *
 *    Revision 1.63  2006/08/31 10:11:28  fabiankeil
 *    Don't free p which is still in use and will be later
 *    freed by free_map(). Don't claim the referrer is unknown
 *    when the client didn't set one.
 *
 *    Revision 1.62  2006/08/14 00:27:47  david__schmidt
 *    Feature request 595948: Re-Filter logging in single line
 *
 *    Revision 1.61  2006/08/03 02:46:41  david__schmidt
 *    Incorporate Fabian Keil's patch work:http://www.fabiankeil.de/sourcecode/privoxy/
 *
 *    Revision 1.60  2006/07/18 14:48:46  david__schmidt
 *    Reorganizing the repository: swapping out what was HEAD (the old 3.1 branch)
 *    with what was really the latest development (the v_3_0_branch branch)
 *
 *    Revision 1.58.2.9  2006/01/29 23:10:56  david__schmidt
 *    Multiple filter file support
 *
 *    Revision 1.58.2.8  2005/05/07 21:50:55  david__schmidt
 *    A few memory leaks plugged (mostly on error paths)
 *
 *    Revision 1.58.2.7  2004/10/03 12:53:32  david__schmidt
 *    Add the ability to check jpeg images for invalid
 *    lengths of comment blocks.  Defensive strategy
 *    against the exploit:
 *       Microsoft Security Bulletin MS04-028
 *       Buffer Overrun in JPEG Processing (GDI+) Could
 *       Allow Code Execution (833987)
 *    Enabled with +inspect-jpegs in actions files.
 *
 *    Revision 1.58.2.6  2003/12/06 22:18:27  gliptak
 *    Correcting compile problem with FEATURE_IMAGE_BLOCKING
 *
 *    Revision 1.58.2.5  2003/11/11 13:10:31  oes
 *    Fixed bug #839859: "See why" link URL now gets url-encoded.
 *
 *    Revision 1.58.2.4  2003/02/28 12:52:45  oes
 *    Fixed a typo
 *
 *    Revision 1.58.2.3  2002/09/25 14:51:51  oes
 *    Added basic support for OPTIONS and TRACE HTTP methods:
 *    New function direct_response which handles OPTIONS and
 *    TRACE requests whose Max-Forwards header field is zero.
 *
 *    Revision 1.58.2.2  2002/08/01 17:18:28  oes
 *    Fixed BR 537651 / SR 579724 (MSIE image detect improper for IE/Mac)
 *
 *    Revision 1.58.2.1  2002/07/26 15:18:53  oes
 *    - Bugfix: Executing a filters without jobs no longer results in
 *      turing off *all* filters.
 *    - Security fix: Malicious web servers can't cause a seg fault
 *      through bogus chunk sizes anymore
 *
 *    Revision 1.58  2002/04/24 02:11:17  oes
 *    Jon's multiple AF patch: url_actions now evaluates rules
 *    from all AFs.
 *
 *    Revision 1.57  2002/04/08 20:38:34  swa
 *    fixed JB spelling
 *
 *    Revision 1.56  2002/04/05 15:51:24  oes
 *     - bugfix: error-pages now get correct request protocol
 *     - fix for invalid HTML in trust info
 *
 *    Revision 1.55  2002/04/02 16:13:51  oes
 *    Fix: No "Go there anyway" for SSL
 *
 *    Revision 1.54  2002/04/02 14:55:56  oes
 *    Bugfix: is_untrusted_url() now depends on FEATURE_TRUST, not FEATURE_COOKIE_JAR
 *
 *    Revision 1.53  2002/03/26 22:29:54  swa
 *    we have a new homepage!
 *
 *    Revision 1.52  2002/03/24 16:35:57  jongfoster
 *    Removing logo
 *
 *    Revision 1.51  2002/03/24 15:23:33  jongfoster
 *    Name changes
 *
 *    Revision 1.50  2002/03/24 13:25:43  swa
 *    name change related issues
 *
 *    Revision 1.49  2002/03/16 20:29:14  oes
 *    Cosmetics
 *
 *    Revision 1.48  2002/03/13 20:25:34  oes
 *    Better logging for content filters
 *
 *    Revision 1.47  2002/03/13 00:30:52  jongfoster
 *    Killing warnings
 *    Added option of always sending redirect for imageblock,
 *    currently disabled with #if 0.
 *
 *    Revision 1.46  2002/03/12 01:42:49  oes
 *    Introduced modular filters
 *
 *    Revision 1.45  2002/03/08 16:47:50  oes
 *    Added choice beween GIF and PNG built-in images
 *
 *    Revision 1.44  2002/03/07 03:49:31  oes
 *     - Fixed compiler warnings etc
 *     - Changed built-in images from GIF to PNG
 *       (with regard to Unisys patent issue)
 *     - Added a 4x4 pattern PNG which is less intrusive
 *       than the logo but also clearly marks the deleted banners
 *
 *    Revision 1.43  2002/01/22 23:51:59  jongfoster
 *    Replacing strsav() with the safer string_append().
 *
 *    Adding missing html_encode() to error message generators.  Where encoded
 *    and unencoded versions of a string were provided, removing the unencoded
 *    one.
 *
 *    Revision 1.42  2002/01/17 21:00:32  jongfoster
 *    Moving all our URL and URL pattern parsing code to urlmatch.c.
 *
 *    Using a single, simple url_match(pattern,url) function - rather than
 *    the 3-line match routine which was repeated all over the place.
 *
 *    Renaming free_url to free_url_spec, since it frees a struct url_spec.
 *
 *    Using parse_http_url() to parse URLs without faking a HTTP
 *    request line for parse_http_request().
 *
 *    Revision 1.41  2001/11/13 00:14:07  jongfoster
 *    Fixing stupid bug now I've figured out what || means.
 *    (It always returns 0 or 1, not one of it's paramaters.)
 *
 *    Revision 1.40  2001/10/26 17:37:55  oes
 *    - Re-enabled Netscape 200/404 bug workaround in block_url():
 *      - Removed OS/2 special case
 *      - Made block_url() independant from sed() having been run
 *    - Made trust_url independant from sed() having been run
 *    - Made is_imageurl independant from sed() having been run.
 *      It now checks User-Agent: and Accept: by itself.
 *
 *
 *    Revision 1.39  2001/10/25 03:40:48  david__schmidt
 *    Change in porting tactics: OS/2's EMX porting layer doesn't allow multiple
 *    threads to call select() simultaneously.  So, it's time to do a real, live,
 *    native OS/2 port.  See defines for __EMX__ (the porting layer) vs. __OS2__
 *    (native). Both versions will work, but using __OS2__ offers multi-threading.
 *
 *    Revision 1.38  2001/10/23 21:32:33  jongfoster
 *    Adding error-checking to selected functions
 *
 *    Revision 1.37  2001/10/22 15:33:56  david__schmidt
 *    Special-cased OS/2 out of the Netscape-abort-on-404-in-js problem in
 *    filters.c.  Added a FIXME in front of the offending code.  I'll gladly
 *    put in a better/more robust fix for all parties if one is presented...
 *    It seems that just returning 200 instead of 404 would pretty much fix
 *    it for everyone, but I don't know all the history of the problem.
 *
 *    Revision 1.36  2001/10/10 16:44:16  oes
 *    Added match_portlist function
 *
 *    Revision 1.35  2001/10/07 15:41:23  oes
 *    Replaced 6 boolean members of csp with one bitmap (csp->flags)
 *
 *    New function remove_chunked_transfer_coding that strips chunked
 *      transfer coding to plain and is called by pcrs_filter_response
 *      and gif_deanimate_response if neccessary
 *
 *    Improved handling of zero-change re_filter runs
 *
 *    pcrs_filter_response and gif_deanimate_response now remove
 *      chunked transfer codeing before processing the body.
 *
 *    Revision 1.34  2001/09/20 15:49:36  steudten
 *
 *    Fix BUG: Change int size to size_t size in pcrs_filter_response().
 *    See cgi.c fill_template().
 *
 *    Revision 1.33  2001/09/16 17:05:14  jongfoster
 *    Removing unused #include showarg.h
 *
 *    Revision 1.32  2001/09/16 13:21:27  jongfoster
 *    Changes to use new list functions.
 *
 *    Revision 1.31  2001/09/16 11:38:02  jongfoster
 *    Splitting fill_template() into 2 functions:
 *    template_load() loads the file
 *    template_fill() performs the PCRS regexps.
 *    This is because the CGI edit interface has a "table row"
 *    template which is used many times in the page - this
 *    change means it's only loaded from disk once.
 *
 *    Revision 1.30  2001/09/16 11:00:10  jongfoster
 *    New function alloc_http_response, for symmetry with free_http_response
 *
 *    Revision 1.29  2001/09/13 23:32:40  jongfoster
 *    Moving image data to cgi.c rather than cgi.h
 *    Fixing a GPF under Win32 (and any other OS that protects global
 *    constants from being written to).
 *
 *    Revision 1.28  2001/09/10 10:18:51  oes
 *    Silenced compiler warnings
 *
 *    Revision 1.27  2001/08/05 16:06:20  jongfoster
 *    Modifiying "struct map" so that there are now separate header and
 *    "map_entry" structures.  This means that functions which modify a
 *    map no longer need to return a pointer to the modified map.
 *    Also, it no longer reverses the order of the entries (which may be
 *    important with some advanced template substitutions).
 *
 *    Revision 1.26  2001/07/30 22:08:36  jongfoster
 *    Tidying up #defines:
 *    - All feature #defines are now of the form FEATURE_xxx
 *    - Permanently turned off WIN_GUI_EDIT
 *    - Permanently turned on WEBDAV and SPLIT_PROXY_ARGS
 *
 *    Revision 1.25  2001/07/26 10:09:46  oes
 *    Made browser detection a little less naive
 *
 *    Revision 1.24  2001/07/25 17:22:51  oes
 *    Added workaround for Netscape bug that prevents display of page when loading a component fails.
 *
 *    Revision 1.23  2001/07/23 13:40:12  oes
 *    Fixed bug that caused document body to be dropped when pcrs joblist was empty.
 *
 *    Revision 1.22  2001/07/18 12:29:34  oes
 *    - Made gif_deanimate_response respect
 *      csp->action->string[ACTION_STRING_DEANIMATE]
 *    - Logging cosmetics
 *
 *    Revision 1.21  2001/07/13 13:59:53  oes
 *     - Introduced gif_deanimate_response which shares the
 *       generic content modification interface of pcrs_filter_response
 *       and acts as a wrapper to deanimate.c:gif_deanimate()
 *     - Renamed re_process_buffer to pcrs_filter_response
 *     - pcrs_filter_response now returns NULL on failiure
 *     - Removed all #ifdef PCRS
 *
 *    Revision 1.20  2001/07/01 17:01:04  oes
 *    Added comments and missing return statement in is_untrusted_url()
 *
 *    Revision 1.19  2001/06/29 21:45:41  oes
 *    Indentation, CRLF->LF, Tab-> Space
 *
 *    Revision 1.18  2001/06/29 13:27:38  oes
 *    - Cleaned up, renamed and reorderd functions
 *      and improved comments
 *
 *    - block_url:
 *      - Ported to CGI platform. Now delivers
 *        http_response or NULL
 *      - Unified HTML and GIF generation (moved image detection
 *        and GIF generation here from jcc.c:chat())
 *      - Fixed HTTP status to:
 *       -  403 (Forbidden) for the "blocked" HTML message
 *       -  200 (OK) for GIF answers
 *       -  302 (Redirect) for redirect to GIF
 *
 *    - trust_url:
 *      - Ported to CGI platform. Now delivers
 *        http_response or NULL
 *      - Separated detection of untrusted URL into
 *        (bool)is_untrusted_url
 *      - Added enforcement of untrusted requests
 *
 *    - Moved redirect_url() from cgi.c to here
 *      and ported it to the CGI platform
 *
 *    - Removed logentry from cancelled commit
 *
 *    Revision 1.17  2001/06/09 10:55:28  jongfoster
 *    Changing BUFSIZ ==> BUFFER_SIZE
 *
 *    Revision 1.16  2001/06/07 23:10:26  jongfoster
 *    Allowing unanchored domain patterns to back off and retry
 *    if they partially match.  Optimized right-anchored patterns.
 *    Moving ACL and forward files into config file.
 *    Replacing struct gateway with struct forward_spec
 *
 *    Revision 1.15  2001/06/03 19:12:00  oes
 *    extracted-CGI relevant stuff
 *
 *    Revision 1.14  2001/06/01 10:30:55  oes
 *    Added optional left-anchoring to domaincmp
 *
 *    Revision 1.13  2001/05/31 21:21:30  jongfoster
 *    Permissionsfile / actions file changes:
 *    - Changed "permission" to "action" throughout
 *    - changes to file format to allow string parameters
 *    - Moved helper functions to actions.c
 *
 *    Revision 1.12  2001/05/31 17:35:20  oes
 *
 *     - Enhanced domain part globbing with infix and prefix asterisk
 *       matching and optional unanchored operation
 *
 *    Revision 1.11  2001/05/29 11:53:23  oes
 *    "See why" link added to "blocked" page
 *
 *    Revision 1.10  2001/05/29 09:50:24  jongfoster
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
 *    Revision 1.9  2001/05/27 22:17:04  oes
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
 *    Revision 1.8  2001/05/26 17:13:28  jongfoster
 *    Filled in a function comment.
 *
 *    Revision 1.7  2001/05/26 15:26:15  jongfoster
 *    ACL feature now provides more security by immediately dropping
 *    connections from untrusted hosts.
 *
 *    Revision 1.6  2001/05/26 00:28:36  jongfoster
 *    Automatic reloading of config file.
 *    Removed obsolete SIGHUP support (Unix) and Reload menu option (Win32).
 *    Most of the global variables have been moved to a new
 *    struct configuration_spec, accessed through csp->config->globalname
 *    Most of the globals remaining are used by the Win32 GUI.
 *
 *    Revision 1.5  2001/05/25 22:34:30  jongfoster
 *    Hard tabs->Spaces
 *
 *    Revision 1.4  2001/05/22 18:46:04  oes
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
 *    Revision 1.3  2001/05/20 16:44:47  jongfoster
 *    Removing last hardcoded Junkbusters.com URLs.
 *
 *    Revision 1.2  2001/05/20 01:21:20  jongfoster
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
 *    Revision 1.1.1.1  2001/05/15 13:58:52  oes
 *    Initial import of version 2.9.3 source tree
 *
 *
 *********************************************************************/


#include "config.h"

#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

#ifndef _WIN32
#ifndef __OS2__
#include <unistd.h>
#endif /* ndef __OS2__ */
#include <netinet/in.h>
#else
#include <winsock2.h>
#endif /* ndef _WIN32 */

#ifdef __OS2__
#include <utils.h>
#endif /* def __OS2__ */

#include "project.h"
#include "filters.h"
#include "encode.h"
#include "parsers.h"
#include "ssplit.h"
#include "errlog.h"
#include "jbsockets.h"
#include "miscutil.h"
#include "actions.h"
#include "cgi.h"
#include "list.h"
#include "deanimate.h"
#include "urlmatch.h"

#ifdef _WIN32
#include "win32.h"
#endif

const char filters_h_rcs[] = FILTERS_H_VERSION;

/* Fix a problem with Solaris.  There should be no effect on other
 * platforms.
 * Solaris's isspace() is a macro which uses it's argument directly
 * as an array index.  Therefore we need to make sure that high-bit
 * characters generate +ve values, and ideally we also want to make
 * the argument match the declared parameter type of "int".
 */
#define ijb_isdigit(__X) isdigit((int)(unsigned char)(__X))


#ifdef FEATURE_ACL
/*********************************************************************
 *
 * Function    :  block_acl
 *
 * Description :  Block this request?
 *                Decide yes or no based on ACL file.
 *
 * Parameters  :
 *          1  :  dst = The proxy or gateway address this is going to.
 *                      Or NULL to check all possible targets.
 *          2  :  csp = Current client state (buffers, headers, etc...)
 *                      Also includes the client IP address.
 *
 * Returns     : 0 = FALSE (don't block) and 1 = TRUE (do block)
 *
 *********************************************************************/
int block_acl(struct access_control_addr *dst, struct client_state *csp)
{
   struct access_control_list *acl = csp->config->acl;

   /* if not using an access control list, then permit the connection */
   if (acl == NULL)
   {
      return(0);
   }

   /* search the list */
   while (acl != NULL)
   {
      if ((csp->ip_addr_long & acl->src->mask) == acl->src->addr)
      {
         if (dst == NULL)
         {
            /* Just want to check if they have any access */
            if (acl->action == ACL_PERMIT)
            {
               return(0);
            }
         }
         else if ( ((dst->addr & acl->dst->mask) == acl->dst->addr)
           && ((dst->port == acl->dst->port) || (acl->dst->port == 0)))
         {
            if (acl->action == ACL_PERMIT)
            {
               return(0);
            }
            else
            {
               return(1);
            }
         }
      }
      acl = acl->next;
   }

   return(1);

}


/*********************************************************************
 *
 * Function    :  acl_addr
 *
 * Description :  Called from `load_config' to parse an ACL address.
 *
 * Parameters  :
 *          1  :  aspec = String specifying ACL address.
 *          2  :  aca = struct access_control_addr to fill in.
 *
 * Returns     :  0 => Ok, everything else is an error.
 *
 *********************************************************************/
int acl_addr(char *aspec, struct access_control_addr *aca)
{
   int i, masklength, port;
   char *p;

   masklength = 32;
   port       =  0;

   if ((p = strchr(aspec, '/')) != NULL)
   {
      *p++ = '\0';

      if (ijb_isdigit(*p) == 0)
      {
         return(-1);
      }
      masklength = atoi(p);
   }

   if ((masklength < 0) || (masklength > 32))
   {
      return(-1);
   }

   if ((p = strchr(aspec, ':')) != NULL)
   {
      *p++ = '\0';

      if (ijb_isdigit(*p) == 0)
      {
         return(-1);
      }
      port = atoi(p);
   }

   aca->port = port;

   aca->addr = ntohl(resolve_hostname_to_ip(aspec));

   if (aca->addr == INADDR_NONE)
   {
      return(-1);
   }

   /* build the netmask */
   aca->mask = 0;
   for (i=1; i <= masklength ; i++)
   {
      aca->mask |= (1 << (32 - i));
   }

   /* now mask off the host portion of the ip address
    * (i.e. save on the network portion of the address).
    */
   aca->addr = aca->addr & aca->mask;

   return(0);

}
#endif /* def FEATURE_ACL */


/*********************************************************************
 *
 * Function    :  match_portlist
 *
 * Description :  Check if a given number is covered by a comma
 *                separated list of numbers and ranges (a,b-c,d,..)
 *
 * Parameters  :
 *          1  :  portlist = String with list
 *          2  :  port = port to check
 *
 * Returns     :  0 => no match
 *                1 => match
 *
 *********************************************************************/
int match_portlist(const char *portlist, int port)
{
   char *min, *max, *next, *portlist_copy;

   min = next = portlist_copy = strdup(portlist);

   /*
    * Zero-terminate first item and remember offset for next
    */
   if (NULL != (next = strchr(portlist_copy, (int) ',')))
   {
      *next++ = '\0';
   }

   /*
    * Loop through all items, checking for match
    */
   while(min)
   {
      if (NULL == (max = strchr(min, (int) '-')))
      {
         /*
          * No dash, check for equality
          */
         if (port == atoi(min))
         {
            free(portlist_copy);
            return(1);
         }
      }
      else
      {
         /*
          * This is a range, so check if between min and max,
          * or, if max was omitted, between min and 65K
          */
         *max++ = '\0';
         if(port >= atoi(min) && port <= (atoi(max) ? atoi(max) : 65535))
         {
            free(portlist_copy);
            return(1);
         }

      }

      /*
       * Jump to next item
       */
      min = next;

      /*
       * Zero-terminate next item and remember offset for n+1
       */
      if ((NULL != next) && (NULL != (next = strchr(next, (int) ','))))
      {
         *next++ = '\0';
      }
   }

   free(portlist_copy);
   return 0;

}


/*********************************************************************
 *
 * Function    :  block_url
 *
 * Description :  Called from `chat'.  Check to see if we need to block this.
 *
 * Parameters  :
 *          1  :  csp = Current client state (buffers, headers, etc...)
 *
 * Returns     :  NULL => unblocked, else HTTP block response
 *
 *********************************************************************/
struct http_response *block_url(struct client_state *csp)
{
   struct http_response *rsp;
   const char *new_content_type = NULL;

   /*
    * If it's not blocked, don't block it ;-)
    */
   if ((csp->action->flags & ACTION_BLOCK) == 0)
   {
      return NULL;
   }
   if (csp->action->flags & ACTION_REDIRECT)
   {
      log_error(LOG_LEVEL_ERROR, "redirect{} overruled by block.");     
   }
   /*
    * Else, prepare a response
    */
   if (NULL == (rsp = alloc_http_response()))
   {
      return cgi_error_memory();
   }

   /*
    * If it's an image-url, send back an image or redirect
    * as specified by the relevant +image action
    */
#ifdef FEATURE_IMAGE_BLOCKING
   if (((csp->action->flags & ACTION_IMAGE_BLOCKER) != 0)
        && is_imageurl(csp))
   {
      char *p;
      /* determine HOW images should be blocked */
      p = csp->action->string[ACTION_STRING_IMAGE_BLOCKER];

      if(csp->action->flags & ACTION_HANDLE_AS_EMPTY_DOCUMENT)
      {
         log_error(LOG_LEVEL_ERROR, "handle-as-empty-document overruled by handle-as-image.");
      }
#if 1 /* Two alternative strategies, use this one for now: */

      /* and handle accordingly: */
      if ((p == NULL) || (0 == strcmpic(p, "pattern")))
      {
         rsp->status = strdup("403 Request blocked by Privoxy");
         if (rsp->status == NULL)
         {
            free_http_response(rsp);
            return cgi_error_memory();
         }
         rsp->body = bindup(image_pattern_data, image_pattern_length);
         if (rsp->body == NULL)
         {
            free_http_response(rsp);
            return cgi_error_memory();
         }
         rsp->content_length = image_pattern_length;

         if (enlist_unique_header(rsp->headers, "Content-Type", BUILTIN_IMAGE_MIMETYPE))
         {
            free_http_response(rsp);
            return cgi_error_memory();
         }
      }

      else if (0 == strcmpic(p, "blank"))
      {
         rsp->status = strdup("403 Request blocked by Privoxy");
         if (rsp->status == NULL)
         {
            free_http_response(rsp);
            return cgi_error_memory();
         }
         rsp->body = bindup(image_blank_data, image_blank_length);
         if (rsp->body == NULL)
         {
            free_http_response(rsp);
            return cgi_error_memory();
         }
         rsp->content_length = image_blank_length;

         if (enlist_unique_header(rsp->headers, "Content-Type", BUILTIN_IMAGE_MIMETYPE))
         {
            free_http_response(rsp);
            return cgi_error_memory();
         }
      }

      else
      {
         rsp->status = strdup("302 Local Redirect from Privoxy");
         if (rsp->status == NULL)
         {
            free_http_response(rsp);
            return cgi_error_memory();
         }

         if (enlist_unique_header(rsp->headers, "Location", p))
         {
            free_http_response(rsp);
            return cgi_error_memory();
         }
      }

#else /* Following code is disabled for now */

      /* and handle accordingly: */
      if ((p == NULL) || (0 == strcmpic(p, "pattern")))
      {
         p = CGI_PREFIX "send-banner?type=pattern";
      }
      else if (0 == strcmpic(p, "blank"))
      {
         p = CGI_PREFIX "send-banner?type=blank";
      }
      rsp->status = strdup("302 Local Redirect from Privoxy");
      if (rsp->status == NULL)
      {
         free_http_response(rsp);
         return cgi_error_memory();
      }

      if (enlist_unique_header(rsp->headers, "Location", p))
      {
         free_http_response(rsp);
         return cgi_error_memory();
      }
#endif /* Preceeding code is disabled for now */
   }
   else if(csp->action->flags & ACTION_HANDLE_AS_EMPTY_DOCUMENT)
   {
     /*
      *  Send empty document.               
      */
      new_content_type = csp->action->string[ACTION_STRING_CONTENT_TYPE];

      freez(rsp->body);
      rsp->body = strdup(" ");
      rsp->content_length = 1;

      rsp->status = strdup("403 Request blocked by Privoxy");
      if (rsp->status == NULL)
      {
         free_http_response(rsp);
         return cgi_error_memory();
      }
      if (new_content_type != 0)
      {
         log_error(LOG_LEVEL_HEADER, "Overwriting Content-Type with %s", new_content_type);
         if (enlist_unique_header(rsp->headers, "Content-Type", new_content_type))
         {
            free_http_response(rsp);
            return cgi_error_memory();
         }
      }

   }
   else
#endif /* def FEATURE_IMAGE_BLOCKING */

   /*
    * Else, generate an HTML "blocked" message:
    */
   {
      jb_err err;
      struct map * exports;
      char *p;

      /*
       * Workaround for stupid Netscape bug which prevents
       * pages from being displayed if loading a referenced
       * JavaScript or style sheet fails. So make it appear
       * as if it succeeded.
       */
      if ( NULL != (p = get_header_value(csp->headers, "User-Agent:"))
           && !strncmpic(p, "mozilla", 7) /* Catch Netscape but */
           && !strstr(p, "Gecko")         /* save Mozilla, */
           && !strstr(p, "compatible")    /* MSIE */
           && !strstr(p, "Opera"))        /* and Opera. */
      {
         rsp->status = strdup("200 Request for blocked URL");
      }
      else
      {
         rsp->status = strdup("404 Request for blocked URL");
      }

      if (rsp->status == NULL)
      {
         free_http_response(rsp);
         return cgi_error_memory();
      }

      exports = default_exports(csp, NULL);
      if (exports == NULL)
      {
         free_http_response(rsp);
         return cgi_error_memory();
      }

#ifdef FEATURE_FORCE_LOAD
      err = map(exports, "force-prefix", 1, FORCE_PREFIX, 1);
      if (csp->http->ssl != 0)
#endif /* ndef FEATURE_FORCE_LOAD */
      {
         err = map_block_killer(exports, "force-support");
      }

      if (!err) err = map(exports, "protocol", 1, csp->http->ssl ? "https://" : "http://", 1);
      if (!err) err = map(exports, "hostport", 1, html_encode(csp->http->hostport), 0);
      if (!err) err = map(exports, "path", 1, html_encode(csp->http->path), 0);
      if (!err) err = map(exports, "path-ue", 1, url_encode(csp->http->path), 0);

      if (err)
      {
         free_map(exports);
         free_http_response(rsp);
         return cgi_error_memory();
      }

      err = template_fill_for_cgi(csp, "blocked", exports, rsp);
      if (err)
      {
         free_http_response(rsp);
         return cgi_error_memory();
      }
   }

   return finish_http_response(rsp);

}


#ifdef FEATURE_TRUST
/*********************************************************************
 *
 * Function    :  trust_url FIXME: I should be called distrust_url
 *
 * Description :  Calls is_untrusted_url to determine if the URL is trusted
 *                and if not, returns a HTTP 403 response with a reject message.
 *
 * Parameters  :
 *          1  :  csp = Current client state (buffers, headers, etc...)
 *
 * Returns     :  NULL => trusted, else http_response.
 *
 *********************************************************************/
struct http_response *trust_url(struct client_state *csp)
{
   struct http_response *rsp;
   struct map * exports;
   char buf[BUFFER_SIZE];
   char *p;
   struct url_spec **tl;
   struct url_spec *t;
   jb_err err;

   /*
    * Don't bother to work on trusted URLs
    */
   if (!is_untrusted_url(csp))
   {
      return NULL;
   }

   /*
    * Else, prepare a response:
    */
   if (NULL == (rsp = alloc_http_response()))
   {
      return cgi_error_memory();
   }

   rsp->status = strdup("403 Request blocked by Privoxy");
   exports = default_exports(csp, NULL);
   if (exports == NULL || rsp->status == NULL)
   {
      free_http_response(rsp);
      return cgi_error_memory();
   }

   /*
    * Export the protocol, host, port, and referrer information
    */
   err = map(exports, "hostport", 1, csp->http->hostport, 1);
   if (!err) err = map(exports, "protocol", 1, csp->http->ssl ? "https://" : "http://", 1); 
   if (!err) err = map(exports, "path", 1, csp->http->path, 1);

   if (NULL != (p = get_header_value(csp->headers, "Referer:")))
   {
      if (!err) err = map(exports, "referrer", 1, html_encode(p), 0);
   }
   else
   {
      if (!err) err = map(exports, "referrer", 1, "none set", 1);
   }

   if (err)
   {
      free_map(exports);
      free_http_response(rsp);
      return cgi_error_memory();
   }

   /*
    * Export the trust list
    */
   p = strdup("");
   for (tl = csp->config->trust_list; (t = *tl) != NULL ; tl++)
   {
      sprintf(buf, "<li>%s</li>\n", t->spec);
      string_append(&p, buf);
   }
   err = map(exports, "trusted-referrers", 1, p, 0);

   if (err)
   {
      free_map(exports);
      free_http_response(rsp);
      return cgi_error_memory();
   }

   /*
    * Export the trust info, if available
    */
   if (csp->config->trust_info->first)
   {
      struct list_entry *l;

      p = strdup("");
      for (l = csp->config->trust_info->first; l ; l = l->next)
      {
         sprintf(buf, "<li> <a href=\"%s\">%s</a><br>\n",l->str, l->str);
         string_append(&p, buf);
      }
      err = map(exports, "trust-info", 1, p, 0);
   }
   else
   {
      err = map_block_killer(exports, "have-trust-info");
   }

   if (err)
   {
      free_map(exports);
      free_http_response(rsp);
      return cgi_error_memory();
   }

   /*
    * Export the force prefix or the force conditional block killer
    */
#ifdef FEATURE_FORCE_LOAD
   err = map(exports, "force-prefix", 1, FORCE_PREFIX, 1);
#else /* ifndef FEATURE_FORCE_LOAD */
   err = map_block_killer(exports, "force-support");
#endif /* ndef FEATURE_FORCE_LOAD */

   if (err)
   {
      free_map(exports);
      free_http_response(rsp);
      return cgi_error_memory();
   }

   /*
    * Build the response
    */
   err = template_fill_for_cgi(csp, "untrusted", exports, rsp);
   if (err)
   {
      free_http_response(rsp);
      return cgi_error_memory();
   }

   return finish_http_response(rsp);
}
#endif /* def FEATURE_TRUST */


#ifdef FEATURE_FAST_REDIRECTS
/*********************************************************************
 *
 * Function    :  redirect_url
 *
 * Description :  Checks for redirection URLs and returns a HTTP redirect
 *                to the destination URL, if necessary
 *
 * Parameters  :
 *          1  :  csp = Current client state (buffers, headers, etc...)
 *
 * Returns     :  NULL if URL was clean, HTTP redirect otherwise.
 *
 *********************************************************************/
struct http_response *redirect_url(struct client_state *csp)
{
   char *p, *q;
   struct http_response *rsp;
   char *redirect_mode = NULL;
   int x, y;

   if ((csp->action->flags & ACTION_REDIRECT))
   {
      q = csp->action->string[ACTION_STRING_REDIRECT];
   }
   else if ((csp->action->flags & ACTION_FAST_REDIRECTS))
   {
      redirect_mode = csp->action->string[ACTION_STRING_FAST_REDIRECTS];
      if (0 == strcmpic(redirect_mode, "check-decoded-url"))
      {  
         p = q = csp->http->path; 
         log_error(LOG_LEVEL_REDIRECTS, "Decoding path: %s if necessary.", p);
         while (*p)
         {
            if (*p == '%') /* Escape sequence? */
            {
               /* Yes, translate from hexadecimal to decimal */
               p++;
               /* First byte */
               x=((int)*p++)-48;
               if (x>9) x-=7;
               x<<=4;
               /* Second byte */
               y=((int)*p++)-48;
               if (y>9)y-=7;
               /* Merge */
               *q++=(char)(x|y);
            }
            else
            {
               /* No, forward character. */
               *q++=*p++;
            }
         }
         *q='\0';
      }
      p = q = csp->http->path;
      log_error(LOG_LEVEL_REDIRECTS, "Checking path for redirects: %s", p);

      /*
       * find the last URL encoded in the request
       */
      while ((p = strstr(p, "http://")) != NULL)
      {
         q = p++;
      }
   }
   else
   {
      /* All redirection actions are disabled */
      return NULL;
   }
   /*
    * if there was any, generate and return a HTTP redirect
    */
   if (q != csp->http->path)
   {
      log_error(LOG_LEVEL_REDIRECTS, "redirecting to: %s", q);

      if (NULL == (rsp = alloc_http_response()))
      {
         return cgi_error_memory();
      }

      if ( enlist_unique_header(rsp->headers, "Location", q)
        || (NULL == (rsp->status = strdup("302 Local Redirect from Privoxy"))) )
      {
         free_http_response(rsp);
         return cgi_error_memory();
      }

      return finish_http_response(rsp);
   }
   else
   {
      return NULL;
   }

}
#endif /* def FEATURE_FAST_REDIRECTS */


#ifdef FEATURE_IMAGE_BLOCKING
/*********************************************************************
 *
 * Function    :  is_imageurl
 *
 * Description :  Given a URL, decide whether it is an image or not,
 *                using either the info from a previous +image action
 *                or, #ifdef FEATURE_IMAGE_DETECT_MSIE, and the browser
 *                is MSIE and not on a Mac, tell from the browser's accept
 *                header.
 *
 * Parameters  :
 *          1  :  csp = Current client state (buffers, headers, etc...)
 *
 * Returns     :  True (nonzero) if URL is an image, false (0)
 *                otherwise
 *
 *********************************************************************/
int is_imageurl(struct client_state *csp)
{
#ifdef FEATURE_IMAGE_DETECT_MSIE
   char *tmp;

   tmp = get_header_value(csp->headers, "User-Agent:");
   if (tmp && strstr(tmp, "MSIE") && !strstr(tmp, "Mac_"))
   {
      tmp = get_header_value(csp->headers, "Accept:");
      if (tmp && strstr(tmp, "image/gif"))
      {
         /* Client will accept HTML.  If this seems counterintuitive,
          * blame Microsoft.
          */
         return(0);
      }
      else
      {
         return(1);
      }
   }
#endif /* def FEATURE_IMAGE_DETECT_MSIE */

   return ((csp->action->flags & ACTION_IMAGE) != 0);

}
#endif /* def FEATURE_IMAGE_BLOCKING */


#ifdef FEATURE_TRUST
/*********************************************************************
 *
 * Function    :  is_untrusted_url
 *
 * Description :  Should we "distrust" this URL (and block it)?
 *
 *                Yes if it matches a line in the trustfile, or if the
 *                    referrer matches a line starting with "+" in the
 *                    trustfile.
 *                No  otherwise.
 *
 * Parameters  :
 *          1  :  csp = Current client state (buffers, headers, etc...)
 *
 * Returns     :  0 => trusted, 1 => untrusted
 *
 *********************************************************************/
int is_untrusted_url(struct client_state *csp)
{
   struct file_list *fl;
   struct block_spec *b;
   struct url_spec **trusted_url;
   struct http_request rhttp[1];
   const char * referer;
   jb_err err;

   /*
    * If we don't have a trustlist, we trust everybody
    */
   if (((fl = csp->tlist) == NULL) || ((b  = fl->f) == NULL))
   {
      return 0;
   }

   memset(rhttp, '\0', sizeof(*rhttp));

   /*
    * Do we trust the request URL itself?
    */
   for (b = b->next; b ; b = b->next)
   {
      if (url_match(b->url, csp->http))
      {
         return b->reject;
      }
   }

   if (NULL == (referer = get_header_value(csp->headers, "Referer:")))
   {
      /* no referrer was supplied */
      return 1;
   }


   /*
    * If not, do we maybe trust its referrer?
    */
   err = parse_http_url(referer, rhttp, csp);
   if (err)
   {
      return 1;
   }

   for (trusted_url = csp->config->trust_list; *trusted_url != NULL; trusted_url++)
   {
      if (url_match(*trusted_url, rhttp))
      {
         /* if the URL's referrer is from a trusted referrer, then
          * add the target spec to the trustfile as an unblocked
          * domain and return NULL (which means it's OK).
          */

         FILE *fp;

         if (NULL != (fp = fopen(csp->config->trustfile, "a")))
         {
            char * path;
            char * path_end;
            char * new_entry = strdup("~");

            string_append(&new_entry, csp->http->hostport);

            path = csp->http->path;
            if ( (path[0] == '/')
              && (path[1] == '~')
              && ((path_end = strchr(path + 2, '/')) != NULL))
            {
               /* since this path points into a user's home space
                * be sure to include this spec in the trustfile.
                */
               int path_len = path_end - path; /* save offset */
               path = strdup(path); /* Copy string */
               if (path != NULL)
               {
                  path_end = path + path_len; /* regenerate ptr to new buffer */
                  *(path_end + 1) = '\0'; /* Truncate path after '/' */
               }
               string_join(&new_entry, path);
            }

            if (new_entry != NULL)
            {
               fprintf(fp, "%s\n", new_entry);
               free(new_entry);
            }
            else
            {
               /* FIXME: No way to handle out-of memory, so mostly ignoring it */
               log_error(LOG_LEVEL_ERROR, "Out of memory adding pattern to trust file");
            }

            fclose(fp);
         }
         return 0;
      }
   }
   return 1;
}
#endif /* def FEATURE_TRUST */


/*********************************************************************
 *
 * Function    :  pcrs_filter_response
 *
 * Description :  Execute all text substitutions from all applying
 *                +filter actions on the text buffer that's been accumulated
 *                in csp->iob->buf. If this changes the contents, set
 *                csp->content_length to the modified size and raise the
 *                CSP_FLAG_MODIFIED flag.
 *
 * Parameters  :
 *          1  :  csp = Current client state (buffers, headers, etc...)
 *
 * Returns     :  a pointer to the (newly allocated) modified buffer.
 *                or NULL if there were no hits or something went wrong
 *
 *********************************************************************/
char *pcrs_filter_response(struct client_state *csp)
{
   int hits=0;
   size_t size, prev_size;

   char *old = csp->iob->cur, *new = NULL;
   pcrs_job *job;

   struct file_list *fl;
   struct re_filterfile_spec *b;
   struct list_entry *filtername;

   int i, found_filters = 0;

   /* 
    * Sanity first
    */
   if (csp->iob->cur >= csp->iob->eod)
   {
      return(NULL);
   }
   size = csp->iob->eod - csp->iob->cur;

   /*
    * Need to check the set of re_filterfiles...
    */
   for (i = 0; i < MAX_AF_FILES; i++)
   {
      fl = csp->rlist[i];
      if (NULL != fl)
      {
         if (NULL != fl->f)
         {
           found_filters = 1;
           break;
         }
      }
   }

   if (0 == found_filters)
   {
      log_error(LOG_LEVEL_ERROR, "Unable to get current state of regexp filtering.");
         return(NULL);
   }

   /*
    * If the body has a "chunked" transfer-encoding,
    * get rid of it first, adjusting size and iob->eod
    */
   if (csp->flags & CSP_FLAG_CHUNKED)
   {
      log_error(LOG_LEVEL_RE_FILTER, "Need to de-chunk first");
      if (0 == (size = remove_chunked_transfer_coding(csp->iob->cur, size)))
      {
         return(NULL);
      }
      csp->iob->eod = csp->iob->cur + size;
      csp->flags |= CSP_FLAG_MODIFIED;
   }

   for (i = 0; i < MAX_AF_FILES; i++)
   {
     fl = csp->rlist[i];
     if ((NULL == fl) || (NULL == fl->f))
       break;
   /*
    * For all applying +filter actions, look if a filter by that
    * name exists and if yes, execute it's pcrs_joblist on the
    * buffer.
    */
   for (b = fl->f; b; b = b->next)
   {
      for (filtername = csp->action->multi[ACTION_MULTI_FILTER]->first;
           filtername ; filtername = filtername->next)
      {
         if (strcmp(b->name, filtername->str) == 0)
         {
            int current_hits = 0;

            if ( NULL == b->joblist )
            {
               log_error(LOG_LEVEL_RE_FILTER, "Filter %s has empty joblist. Nothing to do.", b->name);
               continue;
            }

            prev_size = size;
            /* Apply all jobs from the joblist */
            for (job = b->joblist; NULL != job; job = job->next)
            {
               current_hits += pcrs_execute(job, old, size, &new, &size);
               if (old != csp->iob->cur) free(old);
               old=new;
            }

            log_error(LOG_LEVEL_RE_FILTER, "re_filtering %s%s (size %d) with filter %s produced %d hits (new size %d).",
                      csp->http->hostport, csp->http->path, prev_size, b->name, current_hits, size);

            hits += current_hits;
         }
      }
   }
   }

   /*
    * If there were no hits, destroy our copy and let
    * chat() use the original in csp->iob
    */
   if (!hits)
   {
      free(new);
      return(NULL);
   }

   csp->flags |= CSP_FLAG_MODIFIED;
   csp->content_length = size;
   IOB_RESET(csp);

   return(new);

}


/*********************************************************************
 *
 * Function    :  gif_deanimate_response
 *
 * Description :  Deanimate the GIF image that has been accumulated in
 *                csp->iob->buf, set csp->content_length to the modified
 *                size and raise the CSP_FLAG_MODIFIED flag.
 *
 * Parameters  :
 *          1  :  csp = Current client state (buffers, headers, etc...)
 *
 * Returns     :  a pointer to the (newly allocated) modified buffer.
 *                or NULL in case something went wrong.
 *
 *********************************************************************/
char *gif_deanimate_response(struct client_state *csp)
{
   struct binbuffer *in, *out;
   char *p;
   size_t size = csp->iob->eod - csp->iob->cur;

   /*
    * If the body has a "chunked" transfer-encoding,
    * get rid of it first, adjusting size and iob->eod
    */
   if (csp->flags & CSP_FLAG_CHUNKED)
   {
      log_error(LOG_LEVEL_DEANIMATE, "Need to de-chunk first");
      if (0 == (size = remove_chunked_transfer_coding(csp->iob->cur, size)))
      {
         return(NULL);
      }
      csp->iob->eod = csp->iob->cur + size;
      csp->flags |= CSP_FLAG_MODIFIED;
   }

   if (  (NULL == (in =  (struct binbuffer *)zalloc(sizeof *in )))
      || (NULL == (out = (struct binbuffer *)zalloc(sizeof *out))) )
   {
      log_error(LOG_LEVEL_DEANIMATE, "failed! (no mem)");
      return NULL;
   }

   in->buffer = csp->iob->cur;
   in->size = size;

   if (gif_deanimate(in, out, strncmp("last", csp->action->string[ACTION_STRING_DEANIMATE], 4)))
   {
      log_error(LOG_LEVEL_DEANIMATE, "failed! (gif parsing)");
      free(in);
      buf_free(out);
      return(NULL);
   }
   else
   {
      if ((int)size == out->offset)
      {
         log_error(LOG_LEVEL_DEANIMATE, "GIF not changed.");
      }
      else
      {
         log_error(LOG_LEVEL_DEANIMATE, "Success! GIF shrunk from %d bytes to %d.", size, out->offset);
      }
      csp->content_length = out->offset;
      csp->flags |= CSP_FLAG_MODIFIED;
      p = out->buffer;
      free(in);
      free(out);
      return(p);
   }

}


/*********************************************************************
 *
 * Function    :  jpeg_inspect_response
 *
 * Description :  
 *
 * Parameters  :
 *          1  :  csp = Current client state (buffers, headers, etc...)
 *
 * Returns     :  a pointer to the (newly allocated) modified buffer
 *                or NULL in case something went wrong.
 *
 *********************************************************************/
char *jpeg_inspect_response(struct client_state *csp)
{
   struct binbuffer *in = NULL, *out = NULL;
   char *p = NULL;
   size_t size = csp->iob->eod - csp->iob->cur;

   /*
    * If the body has a "chunked" transfer-encoding,
    * get rid of it first, adjusting size and iob->eod
    */
   if (csp->flags & CSP_FLAG_CHUNKED)
   {
      log_error(LOG_LEVEL_DEANIMATE, "Need to de-chunk first");
      if (0 == (size = remove_chunked_transfer_coding(csp->iob->cur, size)))
      {
         return(NULL);
      }
      csp->iob->eod = csp->iob->cur + size;
      csp->flags |= CSP_FLAG_MODIFIED;
   }

   if (NULL == (in =  (struct binbuffer *)zalloc(sizeof *in )))
   {
      log_error(LOG_LEVEL_DEANIMATE, "failed! (jpeg no mem 1)");
      return NULL;
   }

   if (NULL == (out = (struct binbuffer *)zalloc(sizeof *out)))
   {
      log_error(LOG_LEVEL_DEANIMATE, "failed! (jpeg no mem 2)");
      return NULL;
   }

   in->buffer = csp->iob->cur;
   in->size = size;

   /*
    * Calling jpeg_inspect has the side-effect of creating and 
    * modifying the image buffer of "out" directly.
    */
   if (jpeg_inspect(in, out))
   {
      log_error(LOG_LEVEL_DEANIMATE, "failed! (jpeg parsing)");
      free(in);
      buf_free(out);
      return(NULL);

   }
   else
   {
      csp->content_length = out->offset;
      csp->flags |= CSP_FLAG_MODIFIED;
      p = out->buffer;
      free(in);
      free(out);
      return(p);
   }

}


/*********************************************************************
 *
 * Function    :  remove_chunked_transfer_coding
 *
 * Description :  In-situ remove the "chunked" transfer coding as defined
 *                in rfc2616 from a buffer.
 *
 * Parameters  :
 *          1  :  buffer = Pointer to the text buffer
 *          2  :  size = Number of bytes to be processed
 *
 * Returns     :  The new size, i.e. the number of bytes from buffer which
 *                are occupied by the stripped body, or 0 in case something
 *                went wrong
 *
 *********************************************************************/
int remove_chunked_transfer_coding(char *buffer, const size_t size)
{
   size_t newsize = 0;
   unsigned int chunksize = 0;
   char *from_p, *to_p;

   assert(buffer);
   from_p = to_p = buffer;

   if (sscanf(buffer, "%x", &chunksize) != 1)
   {
      log_error(LOG_LEVEL_ERROR, "Invalid first chunksize while stripping \"chunked\" transfer coding");
      return(0);
   }

   while (chunksize > 0)
   {
      if (NULL == (from_p = strstr(from_p, "\r\n")))
      {
         log_error(LOG_LEVEL_ERROR, "Parse error while stripping \"chunked\" transfer coding");
         return(0);
      }

      if ((newsize += chunksize) >= size)
      {
         log_error(LOG_LEVEL_ERROR, "Chunksize exceeds buffer in  \"chunked\" transfer coding");
         return(0);
      }
      from_p += 2;

      memmove(to_p, from_p, (size_t) chunksize);
      to_p = buffer + newsize;
      from_p += chunksize + 2;

      if (sscanf(from_p, "%x", &chunksize) != 1)
      {
         log_error(LOG_LEVEL_ERROR, "Parse error while stripping \"chunked\" transfer coding");
         return(0);
      }
   }

   /* FIXME: Should this get its own loglevel? */
   log_error(LOG_LEVEL_RE_FILTER, "De-chunking successful. Shrunk from %d to %d\n", size, newsize);
   return(newsize);

}


/*********************************************************************
 *
 * Function    :  url_actions
 *
 * Description :  Gets the actions for this URL.
 *
 * Parameters  :
 *          1  :  http = http_request request for blocked URLs
 *          2  :  csp = Current client state (buffers, headers, etc...)
 *
 * Returns     :  N/A
 *
 *********************************************************************/
void url_actions(struct http_request *http,
                 struct client_state *csp)
{
   struct file_list *fl;
   struct url_actions *b;
   int i;

   init_current_action(csp->action);

   for (i = 0; i < MAX_AF_FILES; i++)
   {
      if (((fl = csp->actions_list[i]) == NULL) || ((b = fl->f) == NULL))
      {
         return;
      }

      apply_url_actions(csp->action, http, b);
   }

   return;
}


/*********************************************************************
 *
 * Function    :  apply_url_actions
 *
 * Description :  Applies a list of URL actions.
 *
 * Parameters  :
 *          1  :  action = Destination.
 *          2  :  http = Current URL
 *          3  :  b = list of URL actions to apply
 *
 * Returns     :  N/A
 *
 *********************************************************************/
void apply_url_actions(struct current_action_spec *action,
                       struct http_request *http,
                       struct url_actions *b)
{
   if (b == NULL)
   {
      /* Should never happen */
      return;
   }

   for (b = b->next; NULL != b; b = b->next)
   {
      if (url_match(b->url, http))
      {
         merge_current_action(action, b->action);
      }
   }
}


/*********************************************************************
 *
 * Function    :  forward_url
 *
 * Description :  Should we forward this to another proxy?
 *
 * Parameters  :
 *          1  :  http = http_request request for current URL
 *          2  :  csp = Current client state (buffers, headers, etc...)
 *
 * Returns     :  Pointer to forwarding information.
 *
 *********************************************************************/
const struct forward_spec * forward_url(struct http_request *http,
                                        struct client_state *csp)
{
   static const struct forward_spec fwd_default[1] = { FORWARD_SPEC_INITIALIZER };
   struct forward_spec *fwd = csp->config->forward;

   if (fwd == NULL)
   {
      return fwd_default;
   }

   while (fwd != NULL)
   {
      if (url_match(fwd->url, http))
      {
         return fwd;
      }
      fwd = fwd->next;
   }

   return fwd_default;
}


/*********************************************************************
 *
 * Function    :  direct_response 
 *
 * Description :  Check if Max-Forwards == 0 for an OPTIONS or TRACE
 *                request and if so, return a HTTP 501 to the client.
 *
 *                FIXME: I have a stupid name and I should handle the
 *                requests properly. Still, what we do here is rfc-
 *                compliant, whereas ignoring or forwarding are not.
 *
 * Parameters  :  
 *          1  :  csp = Current client state (buffers, headers, etc...)
 *
 * Returns     :  http_response if , NULL if nonmatch or handler fail
 *
 *********************************************************************/
struct http_response *direct_response(struct client_state *csp)
{
   struct http_response *rsp;
   struct list_entry *p;

   if ((0 == strcmpic(csp->http->gpc, "trace"))
      || (0 == strcmpic(csp->http->gpc, "options")))
   {
      for (p = csp->headers->first; (p != NULL) ; p = p->next)
      {
         if (!strncmp("Max-Forwards:", p->str, 13)
             && (*(p->str+13) != '\0') && (atoi(p->str+13) == 0))
         {
            /* FIXME: We could handle at least TRACE here,
               but that would require a verbatim copy of
               the request which we don't have anymore */

            log_error(LOG_LEVEL_HEADER, "Found Max-Forwards:0 in OPTIONS or TRACE request -- Returning 501");

            /* Get mem for response or fail*/
            if (NULL == (rsp = alloc_http_response()))
            {
               return cgi_error_memory();
            }
            
            if (NULL == (rsp->status = strdup("501 Not Implemented")))
            {
               free_http_response(rsp);
               return cgi_error_memory();
            }

            rsp->is_static = 1;
            return(finish_http_response(rsp));
         }
      }
   }
   return NULL;
}


/*
  Local Variables:
  tab-width: 3
  end:
*/
