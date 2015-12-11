const char urlmatch_rcs[] = "$Id: urlmatch.c,v 1.12 2006/07/18 14:48:47 david__schmidt Exp $";
/*********************************************************************
 *
 * File        :  $Source: /cvsroot/ijbswa/current/urlmatch.c,v $
 *
 * Purpose     :  Declares functions to match URLs against URL
 *                patterns.
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
 *    $Log: urlmatch.c,v $
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
                      struct client_state *csp)
{
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
         *p = tolower((int)(unsigned char)*p);
      }

      /* split the domain name into components */
      http->dcount = ssplit(http->dbuffer, ".", vec, SZ(vec), 1, 1);

      if (http->dcount <= 0)
      {
         /*
          * Error: More than SZ(vec) components in domain
          *    or: no components in domain
          */
         return JB_ERR_PARSE;
      }

      /* save a copy of the pointers in dvec */
      size = http->dcount * sizeof(*http->dvec);

      http->dvec = (char **)malloc(size);
      if (NULL == http->dvec)
      {
         return JB_ERR_MEMORY;
      }

      memcpy(http->dvec, vec, size);
   }

   return JB_ERR_OK;

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
                          struct client_state *csp)
{
   char *buf;
   char *v[10];
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

   /* this could be a CONNECT request */
   if (strcmpic(v[0], "connect") == 0)
   {
      /* Secure */
      is_connect = 1;
   }
   /* or it could be any other basic HTTP request type */
   else if ((0 == strcmpic(v[0], "get"))
         || (0 == strcmpic(v[0], "head"))
         || (0 == strcmpic(v[0], "post"))
         || (0 == strcmpic(v[0], "put"))
         || (0 == strcmpic(v[0], "delete"))
         || (0 == strcmpic(v[0], "options"))
         || (0 == strcmpic(v[0], "trace"))
 
         /* or a webDAV extension (RFC2518) */
         || (0 == strcmpic(v[0], "propfind"))
         || (0 == strcmpic(v[0], "proppatch"))
         || (0 == strcmpic(v[0], "move"))
         || (0 == strcmpic(v[0], "copy"))
         || (0 == strcmpic(v[0], "mkcol"))
         || (0 == strcmpic(v[0], "lock"))
         || (0 == strcmpic(v[0], "unlock"))

         /* Or a Microsoft webDAV extension for Exchange 2000.  See: */
         /*   http://lists.w3.org/Archives/Public/w3c-dist-auth/2002JanMar/0001.html */
         /*   http://msdn.microsoft.com/library/en-us/wss/wss/_webdav_methods.asp */ 
         || (0 == strcmpic(v[0], "bcopy"))
         || (0 == strcmpic(v[0], "bmove"))
         || (0 == strcmpic(v[0], "bdelete"))
         || (0 == strcmpic(v[0], "bpropfind"))
         || (0 == strcmpic(v[0], "bproppatch"))

         /* Or another Microsoft webDAV extension for Exchange 2000.  See: */
         /*   http://systems.cs.colorado.edu/grunwald/MobileComputing/Papers/draft-cohen-gena-p-base-00.txt */
         /*   http://lists.w3.org/Archives/Public/w3c-dist-auth/2002JanMar/0001.html */
         /*   http://msdn.microsoft.com/library/en-us/wss/wss/_webdav_methods.asp */ 
         || (0 == strcmpic(v[0], "subscribe"))
         || (0 == strcmpic(v[0], "unsubscribe"))
         || (0 == strcmpic(v[0], "notify"))
         || (0 == strcmpic(v[0], "poll"))
         )
   {
      /* Normal */
      is_connect = 0;
   }
   else
   {
      /* Unknown HTTP method */
      free(buf);
      return JB_ERR_PARSE;
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

   if ((p = strchr(buf, '/')) != NULL)
   {
      if (NULL == (url->path = strdup(p)))
      {
         freez(url->spec);
         return JB_ERR_MEMORY;
      }
      url->pathlen = strlen(url->path);
      *p = '\0';
   }
   else
   {
      url->path    = NULL;
      url->pathlen = 0;
   }
   if (url->path)
   {
      int errcode;
      char rebuf[BUFFER_SIZE];

      if (NULL == (url->preg = zalloc(sizeof(*url->preg))))
      {
         freez(url->spec);
         freez(url->path);
         return JB_ERR_MEMORY;
      }

      sprintf(rebuf, "^(%s)", url->path);

      errcode = regcomp(url->preg, rebuf,
            (REG_EXTENDED|REG_NOSUB|REG_ICASE));
      if (errcode)
      {
         size_t errlen = regerror(errcode,
            url->preg, rebuf, sizeof(rebuf));

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
   if ((p = strchr(buf, ':')) == NULL)
   {
      url->port = 0;
   }
   else
   {
      *p++ = '\0';
      url->port = atoi(p);
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
         *p = tolower((int)(unsigned char)*p);
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
         size = url->dcount * sizeof(*url->dvec);

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
   if (url->preg)
   {
      regfree(url->preg);
      freez(url->preg);
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
 * Returns     :  0 iff the URL matches the pattern, else nonzero.
 *
 *********************************************************************/
int url_match(const struct url_spec *pattern,
              const struct http_request *url)
{
   return ((pattern->port == 0) || (pattern->port == url->port))
       && ((pattern->dbuffer == NULL) || (domain_match(pattern, url) == 0))
       && ((pattern->path == NULL) ||
            (regexec(pattern->preg, url->path, 0, NULL, 0) == 0)
      );
}


/*
  Local Variables:
  tab-width: 3
  end:
*/
