const char urlmatch_rcs[] = "$Id: urlmatch.c,v 1.21 2007/12/24 16:34:23 fabiankeil Exp $";
/*********************************************************************
 *
 * File        :  $Source: /cvsroot/ijbswa/current/urlmatch.c,v $
 *
 * Purpose     :  Declares functions to match URLs against URL
 *                patterns.
 *
 * Copyright   :  Written by and Copyright (C) 2001-2003, 2006-2007 the SourceForge
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
 *    $Log: urlmatch.c,v $
 *    Revision 1.21  2007/12/24 16:34:23  fabiankeil
 *    Band-aid (and micro-optimization) that makes it less likely to run out of
 *    stack space with overly-complex path patterns. Probably masks the problem
 *    reported by Lee in #1856679. Hohoho.
 *
 *    Revision 1.20  2007/09/02 15:31:20  fabiankeil
 *    Move match_portlist() from filter.c to urlmatch.c.
 *    It's used for url matching, not for filtering.
 *
 *    Revision 1.19  2007/09/02 13:42:11  fabiankeil
 *    - Allow port lists in url patterns.
 *    - Ditch unused url_spec member pathlen.
 *
 *    Revision 1.18  2007/07/30 16:42:21  fabiankeil
 *    Move the method check into unknown_method()
 *    and loop through the known methods instead
 *    of using a screen-long OR chain.
 *
 *    Revision 1.17  2007/04/15 16:39:21  fabiankeil
 *    Introduce tags as alternative way to specify which
 *    actions apply to a request. At the moment tags can be
 *    created based on client and server headers.
 *
 *    Revision 1.16  2007/02/13 13:59:24  fabiankeil
 *    Remove redundant log message.
 *
 *    Revision 1.15  2007/01/28 16:11:23  fabiankeil
 *    Accept WebDAV methods for subversion
 *    in parse_http_request(). Closes FR 1581425.
 *
 *    Revision 1.14  2007/01/06 14:23:56  fabiankeil
 *    Fix gcc43 warnings. Mark *csp as immutable
 *    for parse_http_url() and url_match().
 *    Replace a sprintf call with snprintf.
 *
 *    Revision 1.13  2006/12/06 19:50:54  fabiankeil
 *    parse_http_url() now handles intercepted
 *    HTTP request lines as well. Moved parts
 *    of parse_http_url()'s code into
 *    init_domain_components() so that it can
 *    be reused in chat().
 *
 *    Revision 1.12  2006/07/18 14:48:47  david__schmidt
 *    Reorganizing the repository: swapping out what was HEAD (the old 3.1 branch)
 *    with what was really the latest development (the v_3_0_branch branch)
 *
 *    Revision 1.10.2.7  2003/05/17 15:57:24  oes
 *     - parse_http_url now checks memory allocation failure for
 *       duplication of "*" URL and rejects "*something" URLs
 *       Closes bug #736344
 *     - Added a comment to what might look like a bug in
 *       create_url_spec (see !bug #736931)
 *     - Comment cosmetics
 *
 *    Revision 1.10.2.6  2003/05/07 12:39:48  oes
 *    Fix typo: Default port for https URLs is 443, not 143.
 *    Thanks to Scott Tregear for spotting this one.
 *
 *    Revision 1.10.2.5  2003/02/28 13:09:29  oes
 *    Fixed a rare double free condition as per Bug #694713
 *
 *    Revision 1.10.2.4  2003/02/28 12:57:44  oes
 *    Moved freeing of http request structure to its owner
 *    as per Dan Price's observations in Bug #694713
 *
 *    Revision 1.10.2.3  2002/11/12 16:50:40  oes
 *    Fixed memory leak in parse_http_request() reported by Oliver Stoeneberg. Fixes bug #637073
 *
 *    Revision 1.10.2.2  2002/09/25 14:53:15  oes
 *    Added basic support for OPTIONS and TRACE HTTP methods:
 *    parse_http_url now recognizes the "*" URI as well as
 *    the OPTIONS and TRACE method keywords.
 *
 *    Revision 1.10.2.1  2002/06/06 19:06:44  jongfoster
 *    Adding support for proprietary Microsoft WebDAV extensions
 *
 *    Revision 1.10  2002/05/12 21:40:37  jongfoster
 *    - Removing some unused code
 *
 *    Revision 1.9  2002/04/04 00:36:36  gliptak
 *    always use pcre for matching
 *
 *    Revision 1.8  2002/04/03 23:32:47  jongfoster
 *    Fixing memory leak on error
 *
 *    Revision 1.7  2002/03/26 22:29:55  swa
 *    we have a new homepage!
 *
 *    Revision 1.6  2002/03/24 13:25:43  swa
 *    name change related issues
 *
 *    Revision 1.5  2002/03/13 00:27:05  jongfoster
 *    Killing warnings
 *
 *    Revision 1.4  2002/03/07 03:46:17  oes
 *    Fixed compiler warnings
 *
 *    Revision 1.3  2002/03/03 14:51:11  oes
 *    Fixed CLF logging: Added ocmd member for client's request to struct http_request
 *
 *    Revision 1.2  2002/01/21 00:14:09  jongfoster
 *    Correcting comment style
 *    Fixing an uninitialized memory bug in create_url_spec()
 *
 *    Revision 1.1  2002/01/17 20:53:46  jongfoster
 *    Moving all our URL and URL pattern parsing code to the same file - it
 *    was scattered around in filters.c, loaders.c and parsers.c.
 *
 *    Providing a single, simple url_match(pattern,url) function - rather than
 *    the 3-line match routine which was repeated all over the place.
 *
 *    Renaming free_url to free_url_spec, since it frees a struct url_spec.
 *
 *    Providing parse_http_url() so that URLs can be parsed without faking a
 *    HTTP request line for parse_http_request() or repeating the parsing
 *    code (both of which were techniques that were actually in use).
 *
 *    Standardizing that struct http_request is used to represent a URL, and
 *    struct url_spec is used to represent a URL pattern.  (Before, URLs were
 *    represented as seperate variables and a partially-filled-in url_spec).
 *
 *
 *********************************************************************/


#include "config.h"

#ifndef _WIN32
#include <stdio.h>
#include <sys/types.h>
#endif

#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <string.h>

#if !defined(_WIN32) && !defined(__OS2__)
#include <unistd.h>
#endif

#include "project.h"
#include "urlmatch.h"
#include "ssplit.h"
#include "miscutil.h"
#include "errlog.h"

const char urlmatch_h_rcs[] = URLMATCH_H_VERSION;


/*********************************************************************
 *
 * Function    :  free_http_request
 *
 * Description :  Freez a http_request structure
 *
 * Parameters  :
 *          1  :  http = points to a http_request structure to free
 *
 * Returns     :  N/A
 *
 *********************************************************************/
void free_http_request(struct http_request *http)
{
   assert(http);

   freez(http->cmd);
   freez(http->ocmd);
   freez(http->gpc);
   freez(http->host);
   freez(http->url);
   freez(http->hostport);
   freez(http->path);
   freez(http->ver);
   freez(http->host_ip_addr_str);
   freez(http->dbuffer);
   freez(http->dvec);
   http->dcount = 0;
}

/*********************************************************************
 *
 * Function    :  init_domain_components
 *
 * Description :  Splits the domain name so we can compare it
 *                against wildcards. It used to be part of
 *                parse_http_url, but was separated because the
 *                same code is required in chat in case of
 *                intercepted requests.
 *
 * Parameters  :
 *          1  :  http = pointer to the http structure to hold elements.
 *
 * Returns     :  JB_ERR_OK on success
 *                JB_ERR_MEMORY on out of memory
 *                JB_ERR_PARSE on malformed command/URL
 *                             or >100 domains deep.
 *
 *********************************************************************/
jb_err init_domain_components(struct http_request *http)
{
   char *vec[BUFFER_SIZE];
   size_t size;
   char *p;

   http->dbuffer = strdup(http->host);
   if (NULL == http->dbuffer)
   {
      return JB_ERR_MEMORY;
   }

   /* map to lower case */
   for (p = http->dbuffer; *p ; p++)
   {
      *p = (char)tolower((int)(unsigned char)*p);
   }

   /* split the domain name into components */
   http->dcount = ssplit(http->dbuffer, ".", vec, SZ(vec), 1, 1);

   if (http->dcount <= 0)
   {
      /*
       * Error: More than SZ(vec) components in domain
       *    or: no components in domain
       */
      log_error(LOG_LEVEL_ERROR, "More than SZ(vec) components in domain or none at all.");
      return JB_ERR_PARSE;
   }

   /* save a copy of the pointers in dvec */
   size = (size_t)http->dcount * sizeof(*http->dvec);

   http->dvec = (char **)malloc(size);
   if (NULL == http->dvec)
   {
      return JB_ERR_MEMORY;
   }

   memcpy(http->dvec, vec, size);

   return JB_ERR_OK;
}

/*********************************************************************
 *
 * Function    :  parse_http_url
 *
 * Description :  Parse out the host and port from the URL.  Find the
 *                hostname & path, port (if ':'), and/or password (if '@')
 *
 * Parameters  :
 *          1  :  url = URL (or is it URI?) to break down
 *          2  :  http = pointer to the http structure to hold elements.
 *                       Will be zeroed before use.  Note that this
 *                       function sets the http->gpc and http->ver
 *                       members to NULL.
 *          3  :  csp = Current client state (buffers, headers, etc...)
 *
 * Returns     :  JB_ERR_OK on success
 *                JB_ERR_MEMORY on out of memory
 *                JB_ERR_PARSE on malformed command/URL
 *                             or >100 domains deep.
 *
 *********************************************************************/
jb_err parse_http_url(const char * url,
                      struct http_request *http,
                      const struct client_state *csp)
{
   int host_available = 1; /* A proxy can dream. */

   /*
    * Zero out the results structure
    */
   memset(http, '\0', sizeof(*http));


   /*
    * Save our initial URL
    */
   http->url = strdup(url);
   if (http->url == NULL)
   {
      return JB_ERR_MEMORY;
   }


   /*
    * Check for * URI. If found, we're done.
    */  
   if (*http->url == '*')
   {
      if  ( NULL == (http->path = strdup("*"))
         || NULL == (http->hostport = strdup("")) ) 
      {
         return JB_ERR_MEMORY;
      }
      if (http->url[1] != '\0')
      {
         return JB_ERR_PARSE;
      }
      return JB_ERR_OK;
   }


   /*
    * Split URL into protocol,hostport,path.
    */
   {
      char *buf;
      char *url_noproto;
      char *url_path;

      buf = strdup(url);
      if (buf == NULL)
      {
         return JB_ERR_MEMORY;
      }

      /* Find the start of the URL in our scratch space */
      url_noproto = buf;
      if (strncmpic(url_noproto, "http://",  7) == 0)
      {
         url_noproto += 7;
         http->ssl = 0;
      }
      else if (strncmpic(url_noproto, "https://", 8) == 0)
      {
         url_noproto += 8;
         http->ssl = 1;
      }
      else if (*url_noproto == '/')
      {
        /*
         * Short request line without protocol and host.
         * Most likely because the client's request
         * was intercepted and redirected into Privoxy.
         */
         http->ssl = 0;
         http->host = NULL;
         host_available = 0;
      }
      else
      {
         http->ssl = 0;
      }

      url_path = strchr(url_noproto, '/');
      if (url_path != NULL)
      {
         /*
          * Got a path.
          *
          * NOTE: The following line ignores the path for HTTPS URLS.
          * This means that you get consistent behaviour if you type a
          * https URL in and it's parsed by the function.  (When the
          * URL is actually retrieved, SSL hides the path part).
          */
         http->path = strdup(http->ssl ? "/" : url_path);
         *url_path = '\0';
         http->hostport = strdup(url_noproto);
      }
      else
      {
         /*
          * Repair broken HTTP requests that don't contain a path,
          * or CONNECT requests
          */
         http->path = strdup("/");
         http->hostport = strdup(url_noproto);
      }

      freez(buf);

      if ( (http->path == NULL)
        || (http->hostport == NULL))
      {
         return JB_ERR_MEMORY;
      }
   }

   if (!host_available)
   {
      /* Without host, there is nothing left to do here */
      return JB_ERR_OK;
   }

   /*
    * Split hostport into user/password (ignored), host, port.
    */
   {
      char *buf;
      char *host;
      char *port;

      buf = strdup(http->hostport);
      if (buf == NULL)
      {
         return JB_ERR_MEMORY;
      }

      /* check if url contains username and/or password */
      host = strchr(buf, '@');
      if (host != NULL)
      {
         /* Contains username/password, skip it and the @ sign. */
         host++;
      }
      else
      {
         /* No username or password. */
         host = buf;
      }

      /* check if url contains port */
      port = strchr(host, ':');
      if (port != NULL)
      {
         /* Contains port */
         /* Terminate hostname and point to start of port string */
         *port++ = '\0';
         http->port = atoi(port);
      }
      else
      {
         /* No port specified. */
         http->port = (http->ssl ? 443 : 80);
      }

      http->host = strdup(host);

      free(buf);

      if (http->host == NULL)
      {
         return JB_ERR_MEMORY;
      }
   }

   /*
    * Split domain name so we can compare it against wildcards
    */
   return init_domain_components(http);

}


/*********************************************************************
 *
 * Function    :  unknown_method
 *
 * Description :  Checks whether a method is unknown.
 *
 * Parameters  :
 *          1  :  method = points to a http method
 *
 * Returns     :  TRUE if it's unknown, FALSE otherwise.
 *
 *********************************************************************/
static int unknown_method(const char *method)
{
   static const char *known_http_methods[] = {
      /* Basic HTTP request type */
      "GET", "HEAD", "POST", "PUT", "DELETE", "OPTIONS", "TRACE", "CONNECT",
      /* webDAV extensions (RFC2518) */
      "PROPFIND", "PROPPATCH", "MOVE", "COPY", "MKCOL", "LOCK", "UNLOCK",
      /*
       * Microsoft webDAV extension for Exchange 2000.  See:
       * http://lists.w3.org/Archives/Public/w3c-dist-auth/2002JanMar/0001.html
       * http://msdn.microsoft.com/library/en-us/wss/wss/_webdav_methods.asp
       */ 
      "BCOPY", "BMOVE", "BDELETE", "BPROPFIND", "BPROPPATCH",
      /*
       * Another Microsoft webDAV extension for Exchange 2000.  See:
       * http://systems.cs.colorado.edu/grunwald/MobileComputing/Papers/draft-cohen-gena-p-base-00.txt
       * http://lists.w3.org/Archives/Public/w3c-dist-auth/2002JanMar/0001.html
       * http://msdn.microsoft.com/library/en-us/wss/wss/_webdav_methods.asp
       */ 
      "SUBSCRIBE", "UNSUBSCRIBE", "NOTIFY", "POLL",
      /*
       * Yet another WebDAV extension, this time for
       * Web Distributed Authoring and Versioning (RFC3253)
       */
      "VERSION-CONTROL", "REPORT", "CHECKOUT", "CHECKIN", "UNCHECKOUT",
      "MKWORKSPACE", "UPDATE", "LABEL", "MERGE", "BASELINE-CONTROL", "MKACTIVITY",
      NULL
   };
   int i;

   for (i = 0; NULL != known_http_methods[i]; i++)
   {
      if (0 == strcmpic(method, known_http_methods[i]))
      {
         return FALSE;
      }
   }

   return TRUE;

}


/*********************************************************************
 *
 * Function    :  parse_http_request
 *
 * Description :  Parse out the host and port from the URL.  Find the
 *                hostname & path, port (if ':'), and/or password (if '@')
 *
 * Parameters  :
 *          1  :  req = HTTP request line to break down
 *          2  :  http = pointer to the http structure to hold elements
 *          3  :  csp = Current client state (buffers, headers, etc...)
 *
 * Returns     :  JB_ERR_OK on success
 *                JB_ERR_MEMORY on out of memory
 *                JB_ERR_CGI_PARAMS on malformed command/URL
 *                                  or >100 domains deep.
 *
 *********************************************************************/
jb_err parse_http_request(const char *req,
                          struct http_request *http,
                          const struct client_state *csp)
{
   char *buf;
   char *v[10]; /* XXX: Why 10? We should only need three. */
   int n;
   jb_err err;
   int is_connect = 0;

   memset(http, '\0', sizeof(*http));

   buf = strdup(req);
   if (buf == NULL)
   {
      return JB_ERR_MEMORY;
   }

   n = ssplit(buf, " \r\n", v, SZ(v), 1, 1);
   if (n != 3)
   {
      free(buf);
      return JB_ERR_PARSE;
   }

   /*
    * Fail in case of unknown methods
    * which we might not handle correctly.
    *
    * XXX: There should be a config option
    * to forward requests with unknown methods
    * anyway. Most of them don't need special
    * steps.
    */
   if (unknown_method(v[0]))
   {
      log_error(LOG_LEVEL_ERROR, "Unknown HTTP method detected: %s", v[0]);
      free(buf);
      return JB_ERR_PARSE;
   }

   if (strcmpic(v[0], "CONNECT") == 0)
   {
      is_connect = 1;
   }

   err = parse_http_url(v[1], http, csp);
   if (err)
   {
      free(buf);
      return err;
   }

   /*
    * Copy the details into the structure
    */
   http->ssl = is_connect;
   http->cmd = strdup(req);
   http->gpc = strdup(v[0]);
   http->ver = strdup(v[2]);

   if ( (http->cmd == NULL)
     || (http->gpc == NULL)
     || (http->ver == NULL) )
   {
      free(buf);
      return JB_ERR_MEMORY;
   }

   free(buf);
   return JB_ERR_OK;

}


/*********************************************************************
 *
 * Function    :  simple_domaincmp
 *
 * Description :  Domain-wise Compare fqdn's.  The comparison is
 *                both left- and right-anchored.  The individual
 *                domain names are compared with simplematch().
 *                This is only used by domain_match.
 *
 * Parameters  :
 *          1  :  pv = array of patterns to compare
 *          2  :  fv = array of domain components to compare
 *          3  :  len = length of the arrays (both arrays are the
 *                      same length - if they weren't, it couldn't
 *                      possibly be a match).
 *
 * Returns     :  0 => domains are equivalent, else no match.
 *
 *********************************************************************/
static int simple_domaincmp(char **pv, char **fv, int len)
{
   int n;

   for (n = 0; n < len; n++)
   {
      if (simplematch(pv[n], fv[n]))
      {
         return 1;
      }
   }

   return 0;

}


/*********************************************************************
 *
 * Function    :  domain_match
 *
 * Description :  Domain-wise Compare fqdn's. Governed by the bimap in
 *                pattern->unachored, the comparison is un-, left-,
 *                right-anchored, or both.
 *                The individual domain names are compared with
 *                simplematch().
 *
 * Parameters  :
 *          1  :  pattern = a domain that may contain a '*' as a wildcard.
 *          2  :  fqdn = domain name against which the patterns are compared.
 *
 * Returns     :  0 => domains are equivalent, else no match.
 *
 *********************************************************************/
static int domain_match(const struct url_spec *pattern, const struct http_request *fqdn)
{
   char **pv, **fv;  /* vectors  */
   int    plen, flen;
   int unanchored = pattern->unanchored & (ANCHOR_RIGHT | ANCHOR_LEFT);

   plen = pattern->dcount;
   flen = fqdn->dcount;

   if (flen < plen)
   {
      /* fqdn is too short to match this pattern */
      return 1;
   }

   pv   = pattern->dvec;
   fv   = fqdn->dvec;

   if (unanchored == ANCHOR_LEFT)
   {
      /*
       * Right anchored.
       *
       * Convert this into a fully anchored pattern with
       * the fqdn and pattern the same length
       */
      fv += (flen - plen); /* flen - plen >= 0 due to check above */
      return simple_domaincmp(pv, fv, plen);
   }
   else if (unanchored == 0)
   {
      /* Fully anchored, check length */
      if (flen != plen)
      {
         return 1;
      }
      return simple_domaincmp(pv, fv, plen);
   }
   else if (unanchored == ANCHOR_RIGHT)
   {
      /* Left anchored, ignore all extra in fqdn */
      return simple_domaincmp(pv, fv, plen);
   }
   else
   {
      /* Unanchored */
      int n;
      int maxn = flen - plen;
      for (n = 0; n <= maxn; n++)
      {
         if (!simple_domaincmp(pv, fv, plen))
         {
            return 0;
         }
         /*
          * Doesn't match from start of fqdn
          * Try skipping first part of fqdn
          */
         fv++;
      }
      return 1;
   }

}


/*********************************************************************
 *
 * Function    :  create_url_spec
 *
 * Description :  Creates a "url_spec" structure from a string.
 *                When finished, free with free_url_spec().
 *
 * Parameters  :
 *          1  :  url = Target url_spec to be filled in.  Will be
 *                      zeroed before use.
 *          2  :  buf = Source pattern, null terminated.  NOTE: The
 *                      contents of this buffer are destroyed by this
 *                      function.  If this function succeeds, the
 *                      buffer is copied to url->spec.  If this
 *                      function fails, the contents of the buffer
 *                      are lost forever.
 *
 * Returns     :  JB_ERR_OK - Success
 *                JB_ERR_MEMORY - Out of memory
 *                JB_ERR_PARSE - Cannot parse regex (Detailed message
 *                               written to system log)
 *
 *********************************************************************/
jb_err create_url_spec(struct url_spec * url, const char * buf)
{
   char *p;
   int errcode;
   size_t errlen;
   char rebuf[BUFFER_SIZE];

   assert(url);
   assert(buf);

   /*
    * Zero memory
    */
   memset(url, '\0', sizeof(*url));

   /*
    * Save a copy of the orignal specification
    */
   if ((url->spec = strdup(buf)) == NULL)
   {
      return JB_ERR_MEMORY;
   }

   /* Is it tag pattern? */
   if (0 == strncmpic("TAG:", url->spec, 4))
   {
      if (NULL == (url->tag_regex = zalloc(sizeof(*url->tag_regex))))
      {
         freez(url->spec);
         return JB_ERR_MEMORY;
      }

      /* buf + 4 to skip "TAG:" */
      errcode = regcomp(url->tag_regex, buf + 4, (REG_EXTENDED|REG_NOSUB|REG_ICASE));
      if (errcode)
      {
         errlen = regerror(errcode, url->preg, rebuf, sizeof(rebuf));
         if (errlen > (sizeof(rebuf) - 1))
         {
            errlen = sizeof(rebuf) - 1;
         }
         rebuf[errlen] = '\0';

         log_error(LOG_LEVEL_ERROR, "error compiling %s: %s", url->spec, rebuf);

         freez(url->spec);
         regfree(url->tag_regex);
         freez(url->tag_regex);

         return JB_ERR_PARSE;
      }
      return JB_ERR_OK;
   }

   /* Only reached for URL patterns */
   p = strchr(buf, '/');
   if (NULL != p)
   {
      url->path = strdup(p);
      if (NULL == url->path)
      {
         freez(url->spec);
         return JB_ERR_MEMORY;
      }
      *p = '\0';
   }
   else
   {
      url->path = NULL;
   }
   if (url->path)
   {
      if (NULL == (url->preg = zalloc(sizeof(*url->preg))))
      {
         freez(url->spec);
         freez(url->path);
         return JB_ERR_MEMORY;
      }

      snprintf(rebuf, sizeof(rebuf), "^(%s)", url->path);

      errcode = regcomp(url->preg, rebuf,
            (REG_EXTENDED|REG_NOSUB|REG_ICASE));
      if (errcode)
      {
         errlen = regerror(errcode, url->preg, rebuf, sizeof(rebuf));

         if (errlen > (sizeof(rebuf) - (size_t)1))
         {
            errlen = sizeof(rebuf) - (size_t)1;
         }
         rebuf[errlen] = '\0';

         log_error(LOG_LEVEL_ERROR, "error compiling %s: %s",
            url->spec, rebuf);

         freez(url->spec);
         freez(url->path);
         regfree(url->preg);
         freez(url->preg);

         return JB_ERR_PARSE;
      }
   }

   p = strchr(buf, ':');
   if (NULL != p)
   {
      *p++ = '\0';
      url->port_list = strdup(p);
      if (NULL == url->port_list)
      {
         return JB_ERR_MEMORY;
      }
   }
   else
   {
      url->port_list = NULL;
   }

   if (buf[0] != '\0')
   {
      char *v[150];
      size_t size;

      /*
       * Parse domain part
       */
      if (buf[strlen(buf) - 1] == '.')
      {
         url->unanchored |= ANCHOR_RIGHT;
      }
      if (buf[0] == '.')
      {
         url->unanchored |= ANCHOR_LEFT;
      }

      /* 
       * Split domain into components
       */
      url->dbuffer = strdup(buf);
      if (NULL == url->dbuffer)
      {
         freez(url->spec);
         freez(url->path);
         regfree(url->preg);
         freez(url->preg);
         return JB_ERR_MEMORY;
      }

      /* 
       * Map to lower case
       */
      for (p = url->dbuffer; *p ; p++)
      {
         *p = (char)tolower((int)(unsigned char)*p);
      }

      /* 
       * Split the domain name into components
       */
      url->dcount = ssplit(url->dbuffer, ".", v, SZ(v), 1, 1);

      if (url->dcount < 0)
      {
         freez(url->spec);
         freez(url->path);
         regfree(url->preg);
         freez(url->preg);
         freez(url->dbuffer);
         url->dcount = 0;
         return JB_ERR_MEMORY;
      }
      else if (url->dcount != 0)
      {

         /* 
          * Save a copy of the pointers in dvec
          */
         size = (size_t)url->dcount * sizeof(*url->dvec);

         url->dvec = (char **)malloc(size);
         if (NULL == url->dvec)
         {
            freez(url->spec);
            freez(url->path);
            regfree(url->preg);
            freez(url->preg);
            freez(url->dbuffer);
            url->dcount = 0;
            return JB_ERR_MEMORY;
         }

         memcpy(url->dvec, v, size);
      }
      /*
       * else dcount == 0 in which case we needn't do anything,
       * since dvec will never be accessed and the pattern will
       * match all domains.
       */
   }

   return JB_ERR_OK;

}


/*********************************************************************
 *
 * Function    :  free_url_spec
 *
 * Description :  Called from the "unloaders".  Freez the url
 *                structure elements.
 *
 * Parameters  :
 *          1  :  url = pointer to a url_spec structure.
 *
 * Returns     :  N/A
 *
 *********************************************************************/
void free_url_spec(struct url_spec *url)
{
   if (url == NULL) return;

   freez(url->spec);
   freez(url->dbuffer);
   freez(url->dvec);
   freez(url->path);
   freez(url->port_list);
   if (url->preg)
   {
      regfree(url->preg);
      freez(url->preg);
   }
   if (url->tag_regex)
   {
      regfree(url->tag_regex);
      freez(url->tag_regex);
   }
}


/*********************************************************************
 *
 * Function    :  url_match
 *
 * Description :  Compare a URL against a URL pattern.
 *
 * Parameters  :
 *          1  :  pattern = a URL pattern
 *          2  :  url = URL to match
 *
 * Returns     :  Nonzero if the URL matches the pattern, else 0.
 *
 *********************************************************************/
int url_match(const struct url_spec *pattern,
              const struct http_request *url)
{
   /* XXX: these should probably be functions. */
#define PORT_MATCHES ((NULL == pattern->port_list) || match_portlist(pattern->port_list, url->port))
#define DOMAIN_MATCHES ((NULL == pattern->dbuffer) || (0 == domain_match(pattern, url)))
#define PATH_MATCHES ((NULL == pattern->path) || (0 == regexec(pattern->preg, url->path, 0, NULL, 0)))

   if (pattern->tag_regex != NULL)
   {
      /* It's a tag pattern and shouldn't be matched against URLs */
      return 0;
   } 

   return (PORT_MATCHES && DOMAIN_MATCHES && PATH_MATCHES);

}


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


/*
  Local Variables:
  tab-width: 3
  end:
*/
