const char cgiedit_rcs[] = "$Id: cgiedit.c,v 1.43 2006/07/18 14:48:45 david__schmidt Exp $";
/*********************************************************************
 *
 * File        :  $Source: /cvsroot/ijbswa/current/cgiedit.c,v $
 *
 * Purpose     :  CGI-based actionsfile editor.
 *
 *                Functions declared include: cgi_edit_*
 *
 *                NOTE: The CGIs in this file use parameter names
 *                such as "f" and "s" which are really *BAD* choices.
 *                However, I'm trying to save bytes in the
 *                edit-actions-list HTML page - the standard actions
 *                file generated a 550kbyte page, which is ridiculous.
 *
 *                Stick to the short names in this file for consistency.
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
 *    $Log: cgiedit.c,v $
 *    Revision 1.43  2006/07/18 14:48:45  david__schmidt
 *    Reorganizing the repository: swapping out what was HEAD (the old 3.1 branch)
 *    with what was really the latest development (the v_3_0_branch branch)
 *
 *    Revision 1.41.2.12  2006/01/30 15:16:25  david__schmidt
 *    Remove a little residual debugging info
 *
 *    Revision 1.41.2.11  2006/01/29 23:10:56  david__schmidt
 *    Multiple filter file support
 *
 *    Revision 1.41.2.10  2005/07/04 03:13:43  david__schmidt
 *    Undo some damaging memory leak patches
 *
 *    Revision 1.41.2.9  2005/07/04 00:31:04  david__schmidt
 *    Removing a double free
 *
 *    Revision 1.41.2.8  2005/05/07 21:50:54  david__schmidt
 *    A few memory leaks plugged (mostly on error paths)
 *
 *    Revision 1.41.2.7  2004/02/17 13:30:23  oes
 *    Moved cgi_error_disabled() from cgiedit.c to
 *    cgi.c to re-enable build with --disable-editor.
 *    Fixes Bug #892744. Thanks to Matthew Fischer
 *    for spotting.
 *
 *    Revision 1.41.2.6  2003/12/18 08:13:48  oes
 *    One line lost in last commit
 *
 *    Revision 1.41.2.5  2003/12/17 16:33:47  oes
 *     - All edit functions that redirect back to the list page
 *       now use cgi_redirect
 *     - All redirects now contain useless parameter "foo", whose
 *       value are raw seconds since epoch, in order to force
 *       Opera and Konqueror to properly reload the list. Closes
 *       bug #859993
 *
 *    Revision 1.41.2.4  2003/03/11 11:53:59  oes
 *    Cosmetic: Renamed cryptic variable
 *
 *    Revision 1.41.2.3  2002/11/12 15:01:41  oes
 *    Fix: Don't free uninitialized struct editable_file
 *
 *    Revision 1.41.2.2  2002/08/05 20:02:59  oes
 *    Bugfix: "Insert new section at top" did not work properly if first non-comment line in file was of type FILE_LINE_ACTION
 *
 *    Revision 1.41.2.1  2002/08/02 12:43:14  oes
 *    Fixed bug #588514: first_time now set on a per-string basis in actions_from_radio; javascriptify now called on copies
 *
 *    Revision 1.41  2002/05/21 19:09:45  oes
 *     - Made Add/Edit/Remove URL Submit and Cancel
 *       buttons jump back to relevant section in eal
 *     - Bugfix: remove-url-form needs p export
 *
 *    Revision 1.40  2002/05/19 11:34:35  jongfoster
 *    Handling read-only actions files better - report the actual
 *    error, not "Out of memory"!
 *
 *    Bug report:
 *    http://sourceforge.net/tracker/index.php?func=detail
 *    &aid=557905&group_id=11118&atid=111118
 *
 *    Revision 1.39  2002/05/12 21:39:15  jongfoster
 *    - Adding Doxygen-style comments to structures and #defines.
 *    - Correcting function comments
 *
 *    Revision 1.38  2002/05/03 23:00:38  jongfoster
 *    Support for templates for "standard actions" buttons.
 *    See bug #549871
 *
 *    Revision 1.37  2002/04/30 11:14:52  oes
 *    Made csp the first parameter in *action_to_html
 *
 *    Revision 1.36  2002/04/26 21:53:30  jongfoster
 *    Fixing a memory leak.  (Near, but not caused by, my earlier commit).
 *
 *    Revision 1.35  2002/04/26 21:50:02  jongfoster
 *    Honouring default exports in edit-actions-for-url-filter template.
 *
 *    Revision 1.34  2002/04/26 12:54:17  oes
 *    Adaptions to changes in actions.c
 *
 *    Revision 1.33  2002/04/24 02:17:47  oes
 *     - Moved get_char_param, get_string_param and get_number_param to cgi.c
 *     - Comments
 *     - Activated Jon's code for editing multiple AFs
 *     - cgi_edit_list_actions now provides context-sensitive
 *       help, looks up all action sets from standard.action and
 *       makes buttons for them in the catchall section
 *     - cgi_edit_action_submit now honors a p parameter, looks up
 *       the corresponding action set, and sets the catchall pattern's
 *       actions accordingly.
 *
 *    Revision 1.32  2002/04/19 16:55:31  jongfoster
 *    Fixing newline problems.  If we do our own text file newline
 *    mangling, we don't want the library to do any, so we need to
 *    open the files in *binary* mode.
 *
 *    Revision 1.31  2002/04/18 19:21:08  jongfoster
 *    Added code to detect "conventional" action files, that start
 *    with a set of actions for all URLs (the pattern "/").
 *    These are special-cased in the "edit-actions-list" CGI, so
 *    that a special UI can be written for them.
 *
 *    Revision 1.30  2002/04/10 13:38:35  oes
 *    load_template signature changed
 *
 *    Revision 1.29  2002/04/08 16:59:08  oes
 *    Fixed comment
 *
 *    Revision 1.28  2002/03/27 12:30:29  oes
 *    Deleted unsused variable
 *
 *    Revision 1.27  2002/03/26 23:06:04  jongfoster
 *    Removing duplicate @ifs on the toggle page
 *
 *    Revision 1.26  2002/03/26 22:59:17  jongfoster
 *    Fixing /toggle to display status consistently.
 *
 *    Revision 1.25  2002/03/26 22:29:54  swa
 *    we have a new homepage!
 *
 *    Revision 1.24  2002/03/24 15:23:33  jongfoster
 *    Name changes
 *
 *    Revision 1.23  2002/03/24 13:32:41  swa
 *    name change related issues
 *
 *    Revision 1.22  2002/03/24 13:25:43  swa
 *    name change related issues
 *
 *    Revision 1.21  2002/03/22 18:02:48  jongfoster
 *    Fixing remote toggle
 *
 *    Revision 1.20  2002/03/16 20:28:34  oes
 *    Added descriptions to the filters so users will know what they select in the cgi editor
 *
 *    Revision 1.19  2002/03/16 18:38:14  jongfoster
 *    Stopping stupid or malicious users from breaking the actions
 *    file using the web-based editor.
 *
 *    Revision 1.18  2002/03/16 14:57:44  jongfoster
 *    Full support for enabling/disabling modular filters.
 *
 *    Revision 1.17  2002/03/16 14:26:42  jongfoster
 *    First version of modular filters support - READ ONLY!
 *    Fixing a double-free bug in the out-of-memory handling in map_radio().
 *
 *    Revision 1.16  2002/03/07 03:46:17  oes
 *    Fixed compiler warnings
 *
 *    Revision 1.15  2002/03/06 22:54:35  jongfoster
 *    Automated function-comment nitpicking.
 *
 *    Revision 1.14  2002/03/05 00:24:51  jongfoster
 *    Patch to always edit the current actions file.
 *
 *    Revision 1.13  2002/03/04 02:07:59  david__schmidt
 *    Enable web editing of actions file on OS/2 (it had been broken all this time!)
 *
 *    Revision 1.12  2002/03/03 09:18:03  joergs
 *    Made jumbjuster work on AmigaOS again.
 *
 *    Revision 1.11  2002/01/23 01:03:31  jongfoster
 *    Fixing gcc [CygWin] compiler warnings
 *
 *    Revision 1.10  2002/01/23 00:22:59  jongfoster
 *    Adding new function cgi_edit_actions_section_swap(), to reorder
 *    the actions file.
 *
 *    Adding get_url_spec_param() to get a validated URL pattern.
 *
 *    Moving edit_read_line() out of this file and into loaders.c.
 *
 *    Adding missing html_encode() to many CGI functions.
 *
 *    Moving the functions that #include actionlist.h to the end of the file,
 *    because the Visual C++ 97 debugger gets extremely confused if you try
 *    to debug any code that comes after them in the file.
 *
 *    Major optimizations in cgi_edit_actions_list() to reduce the size of
 *    the generated HTML (down 40% from 550k to 304k), with major side-effects
 *    throughout the editor and templates.  In particular, the length of the
 *    URLs throughout the editor has been drastically reduced, by cutting
 *    paramater names down to 1 character and CGI names down to 3-4
 *    characters, by removing all non-essential CGI paramaters even at the
 *    expense of having to re-read the actions file for the most trivial
 *    page, and by using relative rather than absolute URLs.  This means
 *    that this (typical example):
 *
 *    <a href="http://ijbswa.sourceforge.net/config/edit-actions-url-form?
 *    filename=ijb&amp;ver=1011487572&amp;section=12&amp;pattern=13
 *    &amp;oldval=www.oesterhelt.org%2Fdeanimate-demo">
 *
 *    is now this:
 *
 *    <a href="eau?f=ijb&amp;v=1011487572&amp;p=13">
 *
 *    Revision 1.9  2002/01/17 20:56:22  jongfoster
 *    Replacing hard references to the URL of the config interface
 *    with #defines from project.h
 *
 *    Revision 1.8  2001/11/30 23:35:51  jongfoster
 *    Renaming actionsfile to ijb.action
 *
 *    Revision 1.7  2001/11/13 00:28:24  jongfoster
 *    - Renaming parameters from edit-actions-for-url so that they only
 *      contain legal JavaScript characters.  If we wanted to write
 *      JavaScript that worked with Netscape 4, this is nessacery.
 *      (Note that at the moment the JavaScript doesn't actually work
 *      with Netscape 4, but now this is purely a template issue, not
 *      one affecting code).
 *    - Adding new CGIs for use by non-JavaScript browsers:
 *        edit-actions-url-form
 *        edit-actions-add-url-form
 *        edit-actions-remove-url-form
 *    - Fixing || bug.
 *
 *    Revision 1.6  2001/10/29 03:48:09  david__schmidt
 *    OS/2 native needed a snprintf() routine.  Added one to miscutil, brackedted
 *    by and __OS2__ ifdef.
 *
 *    Revision 1.5  2001/10/25 03:40:48  david__schmidt
 *    Change in porting tactics: OS/2's EMX porting layer doesn't allow multiple
 *    threads to call select() simultaneously.  So, it's time to do a real, live,
 *    native OS/2 port.  See defines for __EMX__ (the porting layer) vs. __OS2__
 *    (native). Both versions will work, but using __OS2__ offers multi-threading.
 *
 *    Revision 1.4  2001/10/23 21:48:19  jongfoster
 *    Cleaning up error handling in CGI functions - they now send back
 *    a HTML error page and should never cause a FATAL error.  (Fixes one
 *    potential source of "denial of service" attacks).
 *
 *    CGI actions file editor that works and is actually useful.
 *
 *    Ability to toggle JunkBuster remotely using a CGI call.
 *
 *    You can turn off both the above features in the main configuration
 *    file, e.g. if you are running a multi-user proxy.
 *
 *    Revision 1.3  2001/10/14 22:12:49  jongfoster
 *    New version of CGI-based actionsfile editor.
 *    Major changes, including:
 *    - Completely new file parser and file output routines
 *    - edit-actions CGI renamed edit-actions-for-url
 *    - All CGIs now need a filename parameter, except for...
 *    - New CGI edit-actions which doesn't need a filename,
 *      to allow you to start the editor up.
 *    - edit-actions-submit now works, and now automatically
 *      redirects you back to the main edit-actions-list handler.
 *
 *    Revision 1.2  2001/09/16 17:05:14  jongfoster
 *    Removing unused #include showarg.h
 *
 *    Revision 1.1  2001/09/16 15:47:37  jongfoster
 *    First version of CGI-based edit interface.  This is very much a
 *    work-in-progress, and you can't actually use it to edit anything
 *    yet.  You must #define FEATURE_CGI_EDIT_ACTIONS for these changes
 *    to have any effect.
 *
 *
 **********************************************************************/


#include "config.h"

/*
 * FIXME: Following includes copied from cgi.c - which are actually needed?
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <sys/stat.h>

#ifdef _WIN32
#define snprintf _snprintf
#endif /* def _WIN32 */

#include "project.h"
#include "cgi.h"
#include "cgiedit.h"
#include "cgisimple.h"
#include "list.h"
#include "encode.h"
#include "actions.h"
#include "miscutil.h"
#include "errlog.h"
#include "loaders.h"
#include "loadcfg.h"
/* loadcfg.h is for global_toggle_state only */
#include "urlmatch.h"

const char cgiedit_h_rcs[] = CGIEDIT_H_VERSION;


#ifdef FEATURE_CGI_EDIT_ACTIONS

/**
 * A line in an editable_file.
 */
struct file_line
{
   /** Next entry in the linked list */
   struct file_line * next;
   
   /** The raw data, to write out if this line is unmodified. */
   char * raw;
   
   /** Comments and/or whitespace to put before this line if it's modified
       and then written out. */
   char * prefix;

   /** The actual data, as a string.  Line continuation and comment removal
       are performed on the data read from file before it's stored here, so
       it will be a single line of data.  */
   char * unprocessed;
   
   /** The type of data on this line.  One of the FILE_LINE_xxx constants. */
   int type;

   /** The actual data, processed into some sensible data type. */
   union
   {

      /** An action specification. */
      struct action_spec action[1];

      /** A name=value pair. */
      struct
      {

         /** The name in the name=value pair. */
         char * name;

         /** The value in the name=value pair, as a string. */
         char * svalue;

         /** The value in the name=value pair, as an integer. */
         int ivalue;

      } setting;

      /* Add more data types here... e.g.


      struct url_spec url[1];

      struct
      {
         struct action_spec action[1];
         const char * name;
      } alias;

      */

   } data;

};

/** This file_line has not been processed yet. */
#define FILE_LINE_UNPROCESSED           1

/** This file_line is blank. Can only appear at the end of a file, due to
    the way the parser works. */
#define FILE_LINE_BLANK                 2

/** This file_line says {{alias}}. */
#define FILE_LINE_ALIAS_HEADER          3

/** This file_line defines an alias. */
#define FILE_LINE_ALIAS_ENTRY           4

/** This file_line defines an {action}. */
#define FILE_LINE_ACTION                5

/** This file_line specifies a URL pattern. */
#define FILE_LINE_URL                   6

/** This file_line says {{settings}}. */
#define FILE_LINE_SETTINGS_HEADER       7

/** This file_line is in a {{settings}} block. */
#define FILE_LINE_SETTINGS_ENTRY        8

/** This file_line says {{description}}. */
#define FILE_LINE_DESCRIPTION_HEADER    9

/** This file_line is in a {{description}} block. */
#define FILE_LINE_DESCRIPTION_ENTRY    10


/**
 * A configuration file, in a format that can be edited and written back to
 * disk.
 */
struct editable_file
{
   struct file_line * lines;  /**< The contents of the file.  A linked list of lines. */
   const char * filename;     /**< Full pathname - e.g. "/etc/privoxy/wibble.action". */
   const char * identifier;   /**< Filename stub - e.g. "wibble".  Use for CGI param. */
                              /**< Pre-encoded with url_encode() for ease of use. */
   const char * version_str;  /**< Last modification time, as a string.  For CGI param. */
                              /**< Can be used in URL without using url_param(). */
   unsigned version;          /**< Last modification time - prevents chaos with
                                   the browser's "back" button.  Note that this is a
                                   time_t cast to an unsigned.  When comparing, always
                                   cast the time_t to an unsigned, and *NOT* vice-versa.
                                   This may lose the top few bits, but they're not
                                   significant anyway. */
   int newline;               /**< Newline convention - one of the NEWLINE_xxx constants.
                                   Note that changing this after the file has been
                                   read in will cause a mess. */
   struct file_line * parse_error; /**< On parse error, this is the offending line. */
   const char * parse_error_text;  /**< On parse error, this is the problem.
                                        (Statically allocated) */
};

/* FIXME: Following non-static functions should be prototyped in .h or made static */

/* Functions to read and write arbitrary config files */
jb_err edit_read_file(struct client_state *csp,
                      const struct map *parameters,
                      int require_version,
                      const char *suffix,
                      struct editable_file **pfile);
jb_err edit_write_file(struct editable_file * file);
void   edit_free_file(struct editable_file * file);

/* Functions to read and write actions files */
jb_err edit_parse_actions_file(struct editable_file * file);
jb_err edit_read_actions_file(struct client_state *csp,
                              struct http_response *rsp,
                              const struct map *parameters,
                              int require_version,
                              struct editable_file **pfile);

/* Error handlers */
jb_err cgi_error_modified(struct client_state *csp,
                          struct http_response *rsp,
                          const char *filename);
jb_err cgi_error_parse(struct client_state *csp,
                       struct http_response *rsp,
                       struct editable_file *file);
jb_err cgi_error_file(struct client_state *csp,
                      struct http_response *rsp,
                      const char *filename);
jb_err cgi_error_file_read_only(struct client_state *csp,
                                struct http_response *rsp,
                                const char *filename);

/* Internal arbitrary config file support functions */
static jb_err edit_read_file_lines(FILE *fp, struct file_line ** pfile, int *newline);
static void edit_free_file_lines(struct file_line * first_line);

/* Internal actions file support functions */
static int match_actions_file_header_line(const char * line, const char * name);
static jb_err split_line_on_equals(const char * line, char ** pname, char ** pvalue);

/* Internal parameter parsing functions */
static jb_err get_file_name_param(struct client_state *csp,
                                  const struct map *parameters,
                                  const char *param_name,
                                  const char *suffix,
                                  char **pfilename,
                                  const char **pparam);

static jb_err get_url_spec_param(struct client_state *csp,
                                 const struct map *parameters,
                                 const char *name,
                                 char **pvalue);


/* Internal actionsfile <==> HTML conversion functions */
static jb_err map_radio(struct map * exports,
                        const char * optionname,
                        const char * values,
                        int value);
static jb_err actions_to_radio(struct map * exports,
                               const struct action_spec *action);
static jb_err actions_from_radio(const struct map * parameters,
                                 struct action_spec *action);


static jb_err map_copy_parameter_html(struct map *out,
                                      const struct map *in,
                                      const char *name);
#if 0 /* unused function */
static jb_err map_copy_parameter_url(struct map *out,
                                     const struct map *in,
                                     const char *name);
#endif /* unused function */

/* Internal convenience functions */
static char *section_target(const unsigned sectionid);

/*********************************************************************
 *
 * Function    :  section_target
 *
 * Description :  Given an unsigned (section id) n, produce a dynamically
 *                allocated string of the form #l<n>, for use in link
 *                targets.
 *
 * Parameters  :
 *          1  :  sectionid = start line number of section
 *
 * Returns     :  String with link target, or NULL if out of
 *                memory
 *
 *********************************************************************/
static char *section_target(const unsigned sectionid)
{
   char buf[30];

   snprintf(buf, 30, "#l%d", sectionid);
   return(strdup(buf));

}


/*********************************************************************
 *
 * Function    :  map_copy_parameter_html
 *
 * Description :  Copy a CGI parameter from one map to another, HTML
 *                encoding it.
 *
 * Parameters  :
 *          1  :  out = target map
 *          2  :  in = source map
 *          3  :  name = name of cgi parameter to copy
 *
 * Returns     :  JB_ERR_OK on success
 *                JB_ERR_MEMORY on out-of-memory
 *                JB_ERR_CGI_PARAMS if the parameter doesn't exist
 *                                  in the source map
 *
 *********************************************************************/
static jb_err map_copy_parameter_html(struct map *out,
                                      const struct map *in,
                                      const char *name)
{
   const char * value;
   jb_err err;

   assert(out);
   assert(in);
   assert(name);

   value = lookup(in, name);
   err = map(out, name, 1, html_encode(value), 0);

   if (err)
   {
      /* Out of memory */
      return err;
   }
   else if (*value == '\0')
   {
      return JB_ERR_CGI_PARAMS;
   }
   else
   {
      return JB_ERR_OK;
   }
}


#if 0 /* unused function */
/*********************************************************************
 *
 * Function    :  map_copy_parameter_url
 *
 * Description :  Copy a CGI parameter from one map to another, URL
 *                encoding it.
 *
 * Parameters  :
 *          1  :  out = target map
 *          2  :  in = source map
 *          3  :  name = name of cgi parameter to copy
 *
 * Returns     :  JB_ERR_OK on success
 *                JB_ERR_MEMORY on out-of-memory
 *                JB_ERR_CGI_PARAMS if the parameter doesn't exist
 *                                  in the source map
 *
 *********************************************************************/
static jb_err map_copy_parameter_url(struct map *out,
                                     const struct map *in,
                                     const char *name)
{
   const char * value;
   jb_err err;

   assert(out);
   assert(in);
   assert(name);

   value = lookup(in, name);
   err = map(out, name, 1, url_encode(value), 0);

   if (err)
   {
      /* Out of memory */
      return err;
   }
   else if (*value == '\0')
   {
      return JB_ERR_CGI_PARAMS;
   }
   else
   {
      return JB_ERR_OK;
   }
}
#endif /* 0 - unused function */

/*********************************************************************
 *
 * Function    :  cgi_edit_actions_url_form
 *
 * Description :  CGI function that displays a form for
 *                edit-actions-url
 *
 * Parameters  :
 *          1  :  csp = Current client state (buffers, headers, etc...)
 *          2  :  rsp = http_response data structure for output
 *          3  :  parameters = map of cgi parameters
 *
 * CGI Parameters
 *           f : (filename) Identifies the file to edit
 *           v : (version) File's last-modified time
 *           p : (pattern) Line number of pattern to edit
 *
 * Returns     :  JB_ERR_OK on success
 *                JB_ERR_MEMORY on out-of-memory
 *                JB_ERR_CGI_PARAMS if the CGI parameters are not
 *                                  specified or not valid.
 *
 *********************************************************************/
jb_err cgi_edit_actions_url_form(struct client_state *csp,
                                 struct http_response *rsp,
                                 const struct map *parameters)
{
   struct map * exports;
   unsigned patternid;
   struct editable_file * file;
   struct file_line * cur_line;
   unsigned line_number;
   unsigned section_start_line_number = 0;
   jb_err err;

   assert(csp);
   assert(rsp);
   assert(parameters);

   if (0 == (csp->config->feature_flags & RUNTIME_FEATURE_CGI_EDIT_ACTIONS))
   {
      return cgi_error_disabled(csp, rsp);
   }

   err = get_number_param(csp, parameters, "p", &patternid);
   if (err)
   {
      return err;
   }

   err = edit_read_actions_file(csp, rsp, parameters, 1, &file);
   if (err)
   {
      /* No filename specified, can't read file, modified, or out of memory. */
      return (err == JB_ERR_FILE ? JB_ERR_OK : err);
   }

   cur_line = file->lines;

   for (line_number = 1; (cur_line != NULL) && (line_number < patternid); line_number++)
   {
      if (cur_line->type == FILE_LINE_ACTION)
      {
         section_start_line_number = line_number;
      }
      cur_line = cur_line->next;
   }

   if ( (cur_line == NULL)
     || (line_number != patternid)
     || (patternid < 1)
     || (cur_line->type != FILE_LINE_URL))
   {
      /* Invalid "patternid" parameter */
      edit_free_file(file);
      return JB_ERR_CGI_PARAMS;
   }

   if (NULL == (exports = default_exports(csp, NULL)))
   {
      edit_free_file(file);
      return JB_ERR_MEMORY;
   }

   err = map(exports, "f", 1, file->identifier, 1);
   if (!err) err = map(exports, "v", 1, file->version_str, 1);
   if (!err) err = map(exports, "p", 1, url_encode(lookup(parameters, "p")), 0);
   if (!err) err = map(exports, "u", 1, html_encode(cur_line->unprocessed), 0);
   if (!err) err = map(exports, "jumptarget", 1, section_target(section_start_line_number), 0);

   edit_free_file(file);

   if (err)
   {
      free_map(exports);
      return err;
   }

   return template_fill_for_cgi(csp, "edit-actions-url-form", exports, rsp);
}


/*********************************************************************
 *
 * Function    :  cgi_edit_actions_add_url_form
 *
 * Description :  CGI function that displays a form for
 *                edit-actions-url
 *
 * Parameters  :
 *          1  :  csp = Current client state (buffers, headers, etc...)
 *          2  :  rsp = http_response data structure for output
 *          3  :  parameters = map of cgi parameters
 *
 * CGI Parameters :
 *           f : (filename) Identifies the file to edit
 *           v : (version) File's last-modified time
 *           s : (section) Line number of section to edit
 *
 * Returns     :  JB_ERR_OK on success
 *                JB_ERR_MEMORY on out-of-memory
 *                JB_ERR_CGI_PARAMS if the CGI parameters are not
 *                                  specified or not valid.
 *
 *********************************************************************/
jb_err cgi_edit_actions_add_url_form(struct client_state *csp,
                                     struct http_response *rsp,
                                     const struct map *parameters)
{
   struct map *exports;
   jb_err err;

   assert(csp);
   assert(rsp);
   assert(parameters);

   if (0 == (csp->config->feature_flags & RUNTIME_FEATURE_CGI_EDIT_ACTIONS))
   {
      return cgi_error_disabled(csp, rsp);
   }

   if (NULL == (exports = default_exports(csp, NULL)))
   {
      return JB_ERR_MEMORY;
   }

   err = map_copy_parameter_html(exports, parameters, "f");
   if (!err) err = map_copy_parameter_html(exports, parameters, "v");
   if (!err) err = map_copy_parameter_html(exports, parameters, "s");

   if (err)
   {
      free_map(exports);
      return err;
   }

   return template_fill_for_cgi(csp, "edit-actions-add-url-form", exports, rsp);
}


/*********************************************************************
 *
 * Function    :  cgi_edit_actions_remove_url_form
 *
 * Description :  CGI function that displays a form for
 *                edit-actions-url
 *
 * Parameters  :
 *          1  :  csp = Current client state (buffers, headers, etc...)
 *          2  :  rsp = http_response data structure for output
 *          3  :  parameters = map of cgi parameters
 *
 * CGI Parameters :
 *           f : (filename) Identifies the file to edit
 *           v : (version) File's last-modified time
 *           p : (pattern) Line number of pattern to edit
 *
 * Returns     :  JB_ERR_OK on success
 *                JB_ERR_MEMORY on out-of-memory
 *                JB_ERR_CGI_PARAMS if the CGI parameters are not
 *                                  specified or not valid.
 *
 *********************************************************************/
jb_err cgi_edit_actions_remove_url_form(struct client_state *csp,
                                     struct http_response *rsp,
                                     const struct map *parameters)
{
   struct map * exports;
   unsigned patternid;
   struct editable_file * file;
   struct file_line * cur_line;
   unsigned line_number;
   unsigned section_start_line_number = 0;
   jb_err err;

   assert(csp);
   assert(rsp);
   assert(parameters);

   if (0 == (csp->config->feature_flags & RUNTIME_FEATURE_CGI_EDIT_ACTIONS))
   {
      return cgi_error_disabled(csp, rsp);
   }

   err = get_number_param(csp, parameters, "p", &patternid);
   if (err)
   {
      return err;
   }

   err = edit_read_actions_file(csp, rsp, parameters, 1, &file);
   if (err)
   {
      /* No filename specified, can't read file, modified, or out of memory. */
      return (err == JB_ERR_FILE ? JB_ERR_OK : err);
   }

   cur_line = file->lines;

   for (line_number = 1; (cur_line != NULL) && (line_number < patternid); line_number++)
   {
      if (cur_line->type == FILE_LINE_ACTION)
      {
         section_start_line_number = line_number;
      }      
      cur_line = cur_line->next;
   }

   if ( (cur_line == NULL)
     || (line_number != patternid)
     || (patternid < 1)
     || (cur_line->type != FILE_LINE_URL))
   {
      /* Invalid "patternid" parameter */
      edit_free_file(file);
      return JB_ERR_CGI_PARAMS;
   }

   if (NULL == (exports = default_exports(csp, NULL)))
   {
      edit_free_file(file);
      return JB_ERR_MEMORY;
   }

   err = map(exports, "f", 1, file->identifier, 1);
   if (!err) err = map(exports, "v", 1, file->version_str, 1);
   if (!err) err = map(exports, "p", 1, url_encode(lookup(parameters, "p")), 0);
   if (!err) err = map(exports, "u", 1, html_encode(cur_line->unprocessed), 0);
   if (!err) err = map(exports, "jumptarget", 1, section_target(section_start_line_number), 0);

   edit_free_file(file);

   if (err)
   {
      free_map(exports);
      return err;
   }

   return template_fill_for_cgi(csp, "edit-actions-remove-url-form", exports, rsp);
}


/*********************************************************************
 *
 * Function    :  edit_write_file
 *
 * Description :  Write a complete file to disk.
 *
 * Parameters  :
 *          1  :  file = File to write.
 *
 * Returns     :  JB_ERR_OK     on success
 *                JB_ERR_FILE   on error writing to file.
 *                JB_ERR_MEMORY on out of memory
 *
 *********************************************************************/
jb_err edit_write_file(struct editable_file * file)
{
   FILE * fp;
   struct file_line * cur_line;
   struct stat statbuf[1];
   char version_buf[22]; /* 22 = ceil(log10(2^64)) + 2 = max number of
                            digits in time_t, assuming this is a 64-bit
                            machine, plus null terminator, plus one
                            for paranoia */

   assert(file);
   assert(file->filename);

   if (NULL == (fp = fopen(file->filename, "wb")))
   {
      return JB_ERR_FILE;
   }

   cur_line = file->lines;
   while (cur_line != NULL)
   {
      if (cur_line->raw)
      {
         if (fputs(cur_line->raw, fp) < 0)
         {
            fclose(fp);
            return JB_ERR_FILE;
         }
      }
      else
      {
         if (cur_line->prefix)
         {
            if (fputs(cur_line->prefix, fp) < 0)
            {
               fclose(fp);
               return JB_ERR_FILE;
            }
         }
         if (cur_line->unprocessed)
         {

            if (NULL != strchr(cur_line->unprocessed, '#'))
            {
               /* Must quote '#' characters */
               int numhash = 0;
               int len;
               char * src;
               char * dest;
               char * str;

               /* Count number of # characters, so we know length of output string */
               src = cur_line->unprocessed;
               while (NULL != (src = strchr(src, '#')))
               {
                  numhash++;
                  src++;
               }
               assert(numhash > 0);

               /* Allocate new memory for string */
               len = strlen(cur_line->unprocessed);
               if (NULL == (str = malloc((size_t) len + 1 + numhash)))
               {
                  /* Uh oh, just trashed file! */
                  fclose(fp);
                  return JB_ERR_MEMORY;
               }

               /* Loop through string from end */
               src  = cur_line->unprocessed + len;
               dest = str + len + numhash;
               for ( ; len >= 0; len--)
               {
                  if ((*dest-- = *src--) == '#')
                  {
                     *dest-- = '\\';
                     numhash--;
                     assert(numhash >= 0);
                  }
               }
               assert(numhash == 0);
               assert(src  + 1 == cur_line->unprocessed);
               assert(dest + 1 == str);

               if (fputs(str, fp) < 0)
               {
                  free(str);
                  fclose(fp);
                  return JB_ERR_FILE;
               }

               free(str);
            }
            else
            {
               /* Can write without quoting '#' characters. */
               if (fputs(cur_line->unprocessed, fp) < 0)
               {
                  fclose(fp);
                  return JB_ERR_FILE;
               }
            }
            if (fputs(NEWLINE(file->newline), fp) < 0)
            {
               fclose(fp);
               return JB_ERR_FILE;
            }
         }
         else
         {
            /* FIXME: Write data from file->data->whatever */
            assert(0);
         }
      }
      cur_line = cur_line->next;
   }

   fclose(fp);


   /* Update the version stamp in the file structure, since we just
    * wrote to the file & changed it's date.
    */
   if (stat(file->filename, statbuf) < 0)
   {
      /* Error, probably file not found. */
      return JB_ERR_FILE;
   }
   file->version = (unsigned)statbuf->st_mtime;

   /* Correct file->version_str */
   freez(file->version_str);
   snprintf(version_buf, 22, "%u", file->version);
   version_buf[21] = '\0';
   file->version_str = strdup(version_buf);
   if (version_buf == NULL)
   {
      return JB_ERR_MEMORY;
   }

   return JB_ERR_OK;
}


/*********************************************************************
 *
 * Function    :  edit_free_file
 *
 * Description :  Free a complete file in memory.
 *
 * Parameters  :
 *          1  :  file = Data structure to free.
 *
 * Returns     :  N/A
 *
 *********************************************************************/
void edit_free_file(struct editable_file * file)
{
   if (!file)
   {
      /* Silently ignore NULL pointer */
      return;
   }

   edit_free_file_lines(file->lines);
   freez(file->filename);
   freez(file->identifier);
   freez(file->version_str);
   file->version = 0;
   file->parse_error_text = NULL; /* Statically allocated */
   file->parse_error = NULL;

   free(file);
}


/*********************************************************************
 *
 * Function    :  edit_free_file_lines
 *
 * Description :  Free an entire linked list of file lines.
 *
 * Parameters  :
 *          1  :  first_line = Data structure to free.
 *
 * Returns     :  N/A
 *
 *********************************************************************/
static void edit_free_file_lines(struct file_line * first_line)
{
   struct file_line * next_line;

   while (first_line != NULL)
   {
      next_line = first_line->next;
      first_line->next = NULL;
      freez(first_line->raw);
      freez(first_line->prefix);
      freez(first_line->unprocessed);
      switch(first_line->type)
      {
         case 0: /* special case if memory zeroed */
         case FILE_LINE_UNPROCESSED:
         case FILE_LINE_BLANK:
         case FILE_LINE_ALIAS_HEADER:
         case FILE_LINE_SETTINGS_HEADER:
         case FILE_LINE_DESCRIPTION_HEADER:
         case FILE_LINE_DESCRIPTION_ENTRY:
         case FILE_LINE_ALIAS_ENTRY:
         case FILE_LINE_URL:
            /* No data is stored for these */
            break;

         case FILE_LINE_ACTION:
            free_action(first_line->data.action);
            break;

         case FILE_LINE_SETTINGS_ENTRY:
            freez(first_line->data.setting.name);
            freez(first_line->data.setting.svalue);
            break;
         default:
            /* Should never happen */
            assert(0);
            break;
      }
      first_line->type = 0; /* paranoia */
      free(first_line);
      first_line = next_line;
   }
}


/*********************************************************************
 *
 * Function    :  match_actions_file_header_line
 *
 * Description :  Match an actions file {{header}} line
 *
 * Parameters  :
 *          1  :  line = String from file
 *          2  :  name = Header to match against
 *
 * Returns     :  0 iff they match.
 *
 *********************************************************************/
static int match_actions_file_header_line(const char * line, const char * name)
{
   size_t len;

   assert(line);
   assert(name);

   /* Look for "{{" */
   if ((line[0] != '{') || (line[1] != '{'))
   {
      return 1;
   }
   line += 2;

   /* Look for optional whitespace */
   while ( (*line == ' ') || (*line == '\t') )
   {
      line++;
   }

   /* Look for the specified name (case-insensitive) */
   len = strlen(name);
   if (0 != strncmpic(line, name, len))
   {
      return 1;
   }
   line += len;

   /* Look for optional whitespace */
   while ( (*line == ' ') || (*line == '\t') )
   {
      line++;
   }

   /* Look for "}}" and end of string*/
   if ((line[0] != '}') || (line[1] != '}') || (line[2] != '\0'))
   {
      return 1;
   }

   /* It matched!! */
   return 0;
}


/*********************************************************************
 *
 * Function    :  match_actions_file_header_line
 *
 * Description :  Match an actions file {{header}} line
 *
 * Parameters  :
 *          1  :  line = String from file.  Must not start with
 *                       whitespace (else infinite loop!)
 *          2  :  pname = Destination for name
 *          2  :  pvalue = Destination for value
 *
 * Returns     :  JB_ERR_OK     on success
 *                JB_ERR_MEMORY on out-of-memory
 *                JB_ERR_PARSE  if there's no "=" sign, or if there's
 *                              nothing before the "=" sign (but empty
 *                              values *after* the "=" sign are legal).
 *
 *********************************************************************/
static jb_err split_line_on_equals(const char * line, char ** pname, char ** pvalue)
{
   const char * name_end;
   const char * value_start;
   size_t name_len;

   assert(line);
   assert(pname);
   assert(pvalue);
   assert(*line != ' ');
   assert(*line != '\t');

   *pname = NULL;
   *pvalue = NULL;

   value_start = strchr(line, '=');
   if ((value_start == NULL) || (value_start == line))
   {
      return JB_ERR_PARSE;
   }

   name_end = value_start - 1;

   /* Eat any whitespace before the '=' */
   while ((*name_end == ' ') || (*name_end == '\t'))
   {
      /*
       * we already know we must have at least 1 non-ws char
       * at start of buf - no need to check
       */
      name_end--;
   }

   name_len = name_end - line + 1; /* Length excluding \0 */
   if (NULL == (*pname = (char *) malloc(name_len + 1)))
   {
      return JB_ERR_MEMORY;
   }
   strncpy(*pname, line, name_len);
   (*pname)[name_len] = '\0';

   /* Eat any the whitespace after the '=' */
   value_start++;
   while ((*value_start == ' ') || (*value_start == '\t'))
   {
      value_start++;
   }

   if (NULL == (*pvalue = strdup(value_start)))
   {
      free(*pname);
      *pname = NULL;
      return JB_ERR_MEMORY;
   }

   return JB_ERR_OK;
}


/*********************************************************************
 *
 * Function    :  edit_parse_actions_file
 *
 * Description :  Parse an actions file in memory.
 *
 *                Passed linked list must have the "data" member
 *                zeroed, and must contain valid "next" and
 *                "unprocessed" fields.  The "raw" and "prefix"
 *                fields are ignored, and "type" is just overwritten.
 *
 *                Note that on error the file may have been
 *                partially parsed.
 *
 * Parameters  :
 *          1  :  file = Actions file to be parsed in-place.
 *
 * Returns     :  JB_ERR_OK     on success
 *                JB_ERR_MEMORY on out-of-memory
 *                JB_ERR_PARSE  on error
 *
 *********************************************************************/
jb_err edit_parse_actions_file(struct editable_file * file)
{
   struct file_line * cur_line;
   size_t len;
   const char * text; /* Text from a line */
   char * name;  /* For lines of the form name=value */
   char * value; /* For lines of the form name=value */
   struct action_alias * alias_list = NULL;
   jb_err err = JB_ERR_OK;

   /* alias_list contains the aliases defined in this file.
    * It might be better to use the "file_line.data" fields
    * in the relavent places instead.
    */

   cur_line = file->lines;

   /* A note about blank line support: Blank lines should only
    * ever occur as the last line in the file.  This function
    * is more forgiving than that - FILE_LINE_BLANK can occur
    * anywhere.
    */

   /* Skip leading blanks.  Should only happen if file is
    * empty (which is valid, but pointless).
    */
   while ( (cur_line != NULL)
        && (cur_line->unprocessed[0] == '\0') )
   {
      /* Blank line */
      cur_line->type = FILE_LINE_BLANK;
      cur_line = cur_line->next;
   }

   if ( (cur_line != NULL)
     && (cur_line->unprocessed[0] != '{') )
   {
      /* File doesn't start with a header */
      file->parse_error = cur_line;
      file->parse_error_text = "First (non-comment) line of the file must contain a header.";
      return JB_ERR_PARSE;
   }

   if ( (cur_line != NULL) && (0 ==
      match_actions_file_header_line(cur_line->unprocessed, "settings") ) )
   {
      cur_line->type = FILE_LINE_SETTINGS_HEADER;

      cur_line = cur_line->next;
      while ((cur_line != NULL) && (cur_line->unprocessed[0] != '{'))
      {
         if (cur_line->unprocessed[0])
         {
            cur_line->type = FILE_LINE_SETTINGS_ENTRY;

            err = split_line_on_equals(cur_line->unprocessed,
                     &cur_line->data.setting.name,
                     &cur_line->data.setting.svalue);
            if (err == JB_ERR_MEMORY)
            {
               return err;
            }
            else if (err != JB_ERR_OK)
            {
               /* Line does not contain a name=value pair */
               file->parse_error = cur_line;
               file->parse_error_text = "Expected a name=value pair on this {{description}} line, but couldn't find one.";
               return JB_ERR_PARSE;
            }
         }
         else
         {
            cur_line->type = FILE_LINE_BLANK;
         }
         cur_line = cur_line->next;
      }
   }

   if ( (cur_line != NULL) && (0 ==
      match_actions_file_header_line(cur_line->unprocessed, "description") ) )
   {
      cur_line->type = FILE_LINE_DESCRIPTION_HEADER;

      cur_line = cur_line->next;
      while ((cur_line != NULL) && (cur_line->unprocessed[0] != '{'))
      {
         if (cur_line->unprocessed[0])
         {
            cur_line->type = FILE_LINE_DESCRIPTION_ENTRY;
         }
         else
         {
            cur_line->type = FILE_LINE_BLANK;
         }
         cur_line = cur_line->next;
      }
   }

   if ( (cur_line != NULL) && (0 ==
      match_actions_file_header_line(cur_line->unprocessed, "alias") ) )
   {
      cur_line->type = FILE_LINE_ALIAS_HEADER;

      cur_line = cur_line->next;
      while ((cur_line != NULL) && (cur_line->unprocessed[0] != '{'))
      {
         if (cur_line->unprocessed[0])
         {
            /* define an alias */
            struct action_alias * new_alias;

            cur_line->type = FILE_LINE_ALIAS_ENTRY;

            err = split_line_on_equals(cur_line->unprocessed, &name, &value);
            if (err == JB_ERR_MEMORY)
            {
               return err;
            }
            else if (err != JB_ERR_OK)
            {
               /* Line does not contain a name=value pair */
               file->parse_error = cur_line;
               file->parse_error_text = "Expected a name=value pair on this {{alias}} line, but couldn't find one.";
               return JB_ERR_PARSE;
            }

            if ((new_alias = zalloc(sizeof(*new_alias))) == NULL)
            {
               /* Out of memory */
               free(name);
               free(value);
               free_alias_list(alias_list);
               return JB_ERR_MEMORY;
            }

            err = get_actions(value, alias_list, new_alias->action);
            if (err)
            {
               /* Invalid action or out of memory */
               free(name);
               free(value);
               free(new_alias);
               free_alias_list(alias_list);
               if (err == JB_ERR_MEMORY)
               {
                  return err;
               }
               else
               {
                  /* Line does not contain a name=value pair */
                  file->parse_error = cur_line;
                  file->parse_error_text = "This alias does not specify a valid set of actions.";
                  return JB_ERR_PARSE;
               }
            }

            free(value);

            new_alias->name = name;

            /* add to list */
            new_alias->next = alias_list;
            alias_list = new_alias;
         }
         else
         {
            cur_line->type = FILE_LINE_BLANK;
         }
         cur_line = cur_line->next;
      }
   }

   /* Header done, process the main part of the file */
   while (cur_line != NULL)
   {
      /* At this point, (cur_line->unprocessed[0] == '{') */
      assert(cur_line->unprocessed[0] == '{');
      text = cur_line->unprocessed + 1;
      len = strlen(text) - 1;
      if (text[len] != '}')
      {
         /* No closing } on header */
         free_alias_list(alias_list);
         file->parse_error = cur_line;
         file->parse_error_text = "Headers starting with '{' must have a "
            "closing bracket ('}').  Headers starting with two brackets ('{{') "
            "must close with two brackets ('}}').";
         return JB_ERR_PARSE;
      }

      if (text[0] == '{')
      {
         /* An invalid {{ header.  */
         free_alias_list(alias_list);
         file->parse_error = cur_line;
         file->parse_error_text = "Unknown or unexpected two-bracket header.  "
            "Please remember that the system (two-bracket) headers must "
            "appear in the order {{settings}}, {{description}}, {{alias}}, "
            "and must appear before any actions (one-bracket) headers.  "
            "Also note that system headers may not be repeated.";
         return JB_ERR_PARSE;
      }

      while ( (*text == ' ') || (*text == '\t') )
      {
         text++;
         len--;
      }
      while ( (len > 0)
           && ( (text[len - 1] == ' ')
             || (text[len - 1] == '\t') ) )
      {
         len--;
      }

      cur_line->type = FILE_LINE_ACTION;

      /* Remove {} and make copy */
      if (NULL == (value = (char *) malloc(len + 1)))
      {
         /* Out of memory */
         free_alias_list(alias_list);
         return JB_ERR_MEMORY;
      }
      strncpy(value, text, len);
      value[len] = '\0';

      /* Get actions */
      err = get_actions(value, alias_list, cur_line->data.action);
      if (err)
      {
         /* Invalid action or out of memory */
         free(value);
         free_alias_list(alias_list);
         if (err == JB_ERR_MEMORY)
         {
            return err;
         }
         else
         {
            /* Line does not contain a name=value pair */
            file->parse_error = cur_line;
            file->parse_error_text = "This header does not specify a valid set of actions.";
            return JB_ERR_PARSE;
         }
      }

      /* Done with string - it was clobbered anyway */
      free(value);

      /* Process next line */
      cur_line = cur_line->next;

      /* Loop processing URL patterns */
      while ((cur_line != NULL) && (cur_line->unprocessed[0] != '{'))
      {
         if (cur_line->unprocessed[0])
         {
            /* Could parse URL here, but this isn't currently needed */

            cur_line->type = FILE_LINE_URL;
         }
         else
         {
            cur_line->type = FILE_LINE_BLANK;
         }
         cur_line = cur_line->next;
      }
   } /* End main while(cur_line != NULL) loop */

   free_alias_list(alias_list);

   return JB_ERR_OK;
}


/*********************************************************************
 *
 * Function    :  edit_read_file_lines
 *
 * Description :  Read all the lines of a file into memory.
 *                Handles whitespace, comments and line continuation.
 *
 * Parameters  :
 *          1  :  fp = File to read from.  On return, this will be
 *                     at EOF but it will not have been closed.
 *          2  :  pfile = Destination for a linked list of file_lines.
 *                        Will be set to NULL on error.
 *          3  :  newline = How to handle newlines.
 *
 * Returns     :  JB_ERR_OK     on success
 *                JB_ERR_MEMORY on out-of-memory
 *
 *********************************************************************/
jb_err edit_read_file_lines(FILE *fp, struct file_line ** pfile, int *newline)
{
   struct file_line * first_line; /* Keep for return value or to free */
   struct file_line * cur_line;   /* Current line */
   struct file_line * prev_line;  /* Entry with prev_line->next = cur_line */
   jb_err rval;

   assert(fp);
   assert(pfile);

   *pfile = NULL;

   cur_line = first_line = zalloc(sizeof(struct file_line));
   if (cur_line == NULL)
   {
      return JB_ERR_MEMORY;
   }

   cur_line->type = FILE_LINE_UNPROCESSED;

   rval = edit_read_line(fp, &cur_line->raw, &cur_line->prefix, &cur_line->unprocessed, newline, NULL);
   if (rval)
   {
      /* Out of memory or empty file. */
      /* Note that empty file is not an error we propogate up */
      free(cur_line);
      return ((rval == JB_ERR_FILE) ? JB_ERR_OK : rval);
   }

   do
   {
      prev_line = cur_line;
      cur_line = prev_line->next = zalloc(sizeof(struct file_line));
      if (cur_line == NULL)
      {
         /* Out of memory */
         edit_free_file_lines(first_line);
         return JB_ERR_MEMORY;
      }

      cur_line->type = FILE_LINE_UNPROCESSED;

      rval = edit_read_line(fp, &cur_line->raw, &cur_line->prefix, &cur_line->unprocessed, newline, NULL);
      if ((rval != JB_ERR_OK) && (rval != JB_ERR_FILE))
      {
         /* Out of memory */
         edit_free_file_lines(first_line);
         return JB_ERR_MEMORY;
      }

   }
   while (rval != JB_ERR_FILE);

   /* EOF */

   /* We allocated one too many - free it */
   prev_line->next = NULL;
   free(cur_line);

   *pfile = first_line;
   return JB_ERR_OK;
}


/*********************************************************************
 *
 * Function    :  edit_read_file
 *
 * Description :  Read a complete file into memory.
 *                Handles CGI parameter parsing.  If requested, also
 *                checks the file's modification timestamp.
 *
 * Parameters  :
 *          1  :  csp = Current client state (buffers, headers, etc...)
 *          2  :  parameters = map of cgi parameters.
 *          3  :  require_version = true to check "ver" parameter.
 *          4  :  suffix = File extension, e.g. ".action".
 *          5  :  pfile = Destination for the file.  Will be set
 *                        to NULL on error.
 *
 * CGI Parameters :
 *    filename :  The name of the file to read, without the
 *                path or ".action" extension.
 *         ver :  (Only if require_version is nonzero)
 *                Timestamp of the actions file.  If wrong, this
 *                function fails with JB_ERR_MODIFIED.
 *
 * Returns     :  JB_ERR_OK     on success
 *                JB_ERR_MEMORY on out-of-memory
 *                JB_ERR_CGI_PARAMS if "filename" was not specified
 *                                  or is not valid.
 *                JB_ERR_FILE   if the file cannot be opened or
 *                              contains no data
 *                JB_ERR_MODIFIED if version checking was requested and
 *                                failed - the file was modified outside
 *                                of this CGI editor instance.
 *
 *********************************************************************/
jb_err edit_read_file(struct client_state *csp,
                      const struct map *parameters,
                      int require_version,
                      const char *suffix,
                      struct editable_file **pfile)
{
   struct file_line * lines;
   FILE * fp;
   jb_err err;
   char * filename;
   const char * identifier;
   struct editable_file * file;
   unsigned version = 0;
   struct stat statbuf[1];
   char version_buf[22];
   int newline = NEWLINE_UNKNOWN;

   assert(csp);
   assert(parameters);
   assert(pfile);

   *pfile = NULL;

   err = get_file_name_param(csp, parameters, "f", suffix,
                             &filename, &identifier);
   if (err)
   {
      return err;
   }

   if (stat(filename, statbuf) < 0)
   {
      /* Error, probably file not found. */
      free(filename);
      return JB_ERR_FILE;
   }
   version = (unsigned) statbuf->st_mtime;

   if (require_version)
   {
      unsigned specified_version;
      err = get_number_param(csp, parameters, "v", &specified_version);
      if (err)
      {
         free(filename);
         return err;
      }

      if (version != specified_version)
      {
         return JB_ERR_MODIFIED;
      }
   }

   if (NULL == (fp = fopen(filename,"rb")))
   {
      free(filename);
      return JB_ERR_FILE;
   }

   err = edit_read_file_lines(fp, &lines, &newline);

   fclose(fp);

   if (err)
   {
      free(filename);
      return err;
   }

   file = (struct editable_file *) zalloc(sizeof(*file));
   if (err)
   {
      free(filename);
      edit_free_file_lines(lines);
      return err;
   }

   file->lines = lines;
   file->newline = newline;
   file->filename = filename;
   file->version = version;
   file->identifier = url_encode(identifier);

   if (file->identifier == NULL)
   {
      edit_free_file(file);
      return JB_ERR_MEMORY;
   }

   /* Correct file->version_str */
   freez(file->version_str);
   snprintf(version_buf, 22, "%u", file->version);
   version_buf[21] = '\0';
   file->version_str = strdup(version_buf);
   if (version_buf == NULL)
   {
      edit_free_file(file);
      return JB_ERR_MEMORY;
   }

   *pfile = file;
   return JB_ERR_OK;
}


/*********************************************************************
 *
 * Function    :  edit_read_actions_file
 *
 * Description :  Read a complete actions file into memory.
 *                Handles CGI parameter parsing.  If requested, also
 *                checks the file's modification timestamp.
 *
 *                If this function detects an error in the categories
 *                JB_ERR_FILE, JB_ERR_MODIFIED, or JB_ERR_PARSE,
 *                then it handles it by filling in the specified
 *                response structure and returning JB_ERR_FILE.
 *
 * Parameters  :
 *          1  :  csp = Current client state (buffers, headers, etc...)
 *          2  :  rsp = HTTP response.  Only filled in on error.
 *          2  :  parameters = map of cgi parameters.
 *          3  :  require_version = true to check "ver" parameter.
 *          4  :  pfile = Destination for the file.  Will be set
 *                        to NULL on error.
 *
 * CGI Parameters :
 *    filename :  The name of the actions file to read, without the
 *                path or ".action" extension.
 *         ver :  (Only if require_version is nonzero)
 *                Timestamp of the actions file.  If wrong, this
 *                function fails with JB_ERR_MODIFIED.
 *
 * Returns     :  JB_ERR_OK     on success
 *                JB_ERR_MEMORY on out-of-memory
 *                JB_ERR_CGI_PARAMS if "filename" was not specified
 *                                  or is not valid.
 *                JB_ERR_FILE  if the file does not contain valid data,
 *                             or if file cannot be opened or
 *                             contains no data, or if version
 *                             checking was requested and failed.
 *
 *********************************************************************/
jb_err edit_read_actions_file(struct client_state *csp,
                              struct http_response *rsp,
                              const struct map *parameters,
                              int require_version,
                              struct editable_file **pfile)
{
   jb_err err;
   struct editable_file *file;

   assert(csp);
   assert(parameters);
   assert(pfile);

   *pfile = NULL;

   err = edit_read_file(csp, parameters, require_version, ".action", &file);
   if (err)
   {
      /* Try to handle if possible */
      if (err == JB_ERR_FILE)
      {
         err = cgi_error_file(csp, rsp, lookup(parameters, "f"));
      }
      else if (err == JB_ERR_MODIFIED)
      {
         err = cgi_error_modified(csp, rsp, lookup(parameters, "f"));
      }
      if (err == JB_ERR_OK)
      {
         /*
          * Signal to higher-level CGI code that there was a problem but we
          * handled it, they should just return JB_ERR_OK.
          */
         err = JB_ERR_FILE;
      }
      return err;
   }

   err = edit_parse_actions_file(file);
   if (err)
   {
      if (err == JB_ERR_PARSE)
      {
         err = cgi_error_parse(csp, rsp, file);
         if (err == JB_ERR_OK)
         {
            /*
             * Signal to higher-level CGI code that there was a problem but we
             * handled it, they should just return JB_ERR_OK.
             */
            err = JB_ERR_FILE;
         }
      }
      edit_free_file(file);
      return err;
   }

   *pfile = file;
   return JB_ERR_OK;
}


/*********************************************************************
 *
 * Function    :  get_file_name_param
 *
 * Description :  Get the name of the file to edit from the parameters
 *                passed to a CGI function.  This function handles
 *                security checks such as blocking urls containing
 *                "/" or ".", prepending the config file directory,
 *                and adding the specified suffix.
 *
 *                (This is an essential security check, otherwise
 *                users may be able to pass "../../../etc/passwd"
 *                and overwrite the password file [linux], "prn:"
 *                and print random data [Windows], etc...)
 *
 *                This function only allows filenames contining the
 *                characters '-', '_', 'A'-'Z', 'a'-'z', and '0'-'9'.
 *                That's probably too restrictive but at least it's
 *                secure.
 *
 * Parameters  :
 *          1  :  csp = Current client state (buffers, headers, etc...)
 *          2  :  parameters = map of cgi parameters
 *          3  :  param_name = The name of the parameter to read
 *          4  :  suffix = File extension, e.g. ".actions"
 *          5  :  pfilename = destination for full filename.  Caller
 *                free()s.  Set to NULL on error.
 *          6  :  pparam = destination for partial filename,
 *                suitable for use in another URL.  Allocated as part
 *                of the map "parameters", so don't free it.
 *                Set to NULL if not specified.
 *
 * Returns     :  JB_ERR_OK         on success
 *                JB_ERR_MEMORY     on out-of-memory
 *                JB_ERR_CGI_PARAMS if "filename" was not specified
 *                                  or is not valid.
 *
 *********************************************************************/
static jb_err get_file_name_param(struct client_state *csp,
                                  const struct map *parameters,
                                  const char *param_name,
                                  const char *suffix,
                                  char **pfilename,
                                  const char **pparam)
{
   const char *param;
   const char *s;
   char *name;
   char *fullpath;
   char ch;
   int len;

   assert(csp);
   assert(parameters);
   assert(suffix);
   assert(pfilename);
   assert(pparam);

   *pfilename = NULL;
   *pparam = NULL;

   param = lookup(parameters, param_name);
   if (!*param)
   {
      return JB_ERR_CGI_PARAMS;
   }

   *pparam = param;

   len = strlen(param);
   if (len >= FILENAME_MAX)
   {
      /* Too long. */
      return JB_ERR_CGI_PARAMS;
   }

   /* Check every character to see if it's legal */
   s = param;
   while ((ch = *s++) != '\0')
   {
      if ( ((ch < 'A') || (ch > 'Z'))
        && ((ch < 'a') || (ch > 'z'))
        && ((ch < '0') || (ch > '9'))
        && (ch != '-')
        && (ch != '_') )
      {
         /* Probable hack attempt. */
         return JB_ERR_CGI_PARAMS;
      }
   }

   /* Append extension */
   name = malloc(len + strlen(suffix) + 1);
   if (name == NULL)
   {
      return JB_ERR_MEMORY;
   }
   strcpy(name, param);
   strcpy(name + len, suffix);

   /* Prepend path */
   fullpath = make_path(csp->config->confdir, name);
   free(name);

   if (fullpath == NULL)
   {
      return JB_ERR_MEMORY;
   }

   /* Success */
   *pfilename = fullpath;

   return JB_ERR_OK;
}


/*********************************************************************
 *
 * Function    :  get_url_spec_param
 *
 * Description :  Get a URL pattern from the parameters
 *                passed to a CGI function.  Removes leading/trailing
 *                spaces and validates it.
 *
 * Parameters  :
 *          1  :  csp = Current client state (buffers, headers, etc...)
 *          2  :  parameters = map of cgi parameters
 *          3  :  name = Name of CGI parameter to read
 *          4  :  pvalue = destination for value.  Will be malloc()'d.
 *                         Set to NULL on error.
 *
 * Returns     :  JB_ERR_OK         on success
 *                JB_ERR_MEMORY     on out-of-memory
 *                JB_ERR_CGI_PARAMS if the parameter was not specified
 *                                  or is not valid.
 *
 *********************************************************************/
static jb_err get_url_spec_param(struct client_state *csp,
                                 const struct map *parameters,
                                 const char *name,
                                 char **pvalue)
{
   const char *orig_param;
   char *param;
   char *s;
   struct url_spec compiled[1];
   jb_err err;

   assert(csp);
   assert(parameters);
   assert(name);
   assert(pvalue);

   *pvalue = NULL;

   orig_param = lookup(parameters, name);
   if (!*orig_param)
   {
      return JB_ERR_CGI_PARAMS;
   }

   /* Copy and trim whitespace */
   param = strdup(orig_param);
   if (param == NULL)
   {
      return JB_ERR_MEMORY;
   }
   chomp(param);

   /* Must be non-empty, and can't allow 1st character to be '{' */
   if (param[0] == '\0' || param[0] == '{')
   {
      free(param);
      return JB_ERR_CGI_PARAMS;
   }

   /* Check for embedded newlines */
   for (s = param; *s != '\0'; s++)
   {
      if ((*s == '\r') || (*s == '\n'))
      {
         free(param);
         return JB_ERR_CGI_PARAMS;
      }
   }

   /* Check that regex is valid */
   s = strdup(param);
   if (s == NULL)
   {
      free(param);
      return JB_ERR_MEMORY;
   }
   err = create_url_spec(compiled, s);
   free(s);
   if (err)
   {
      free(param);
      return (err == JB_ERR_MEMORY) ? JB_ERR_MEMORY : JB_ERR_CGI_PARAMS;
   }
   free_url_spec(compiled);

   if (param[strlen(param) - 1] == '\\')
   {
      /*
       * Must protect trailing '\\' from becoming line continuation character.
       * Two methods: 1) If it's a domain only, add a trailing '/'.
       * 2) For path, add the do-nothing PCRE expression (?:) to the end
       */
      if (strchr(param, '/') == NULL)
      {
         err = string_append(&param, "/");
      }
      else
      {
         err = string_append(&param, "(?:)");
      }
      if (err)
      {
         return err;
      }

      /* Check that the modified regex is valid */
      s = strdup(param);
      if (s == NULL)
      {
         free(param);
         return JB_ERR_MEMORY;
      }
      err = create_url_spec(compiled, s);
      free(s);
      if (err)
      {
         free(param);
         return (err == JB_ERR_MEMORY) ? JB_ERR_MEMORY : JB_ERR_CGI_PARAMS;
      }
      free_url_spec(compiled);
   }

   *pvalue = param;
   return JB_ERR_OK;
}

/*********************************************************************
 *
 * Function    :  map_radio
 *
 * Description :  Map a set of radio button values.  E.g. if you have
 *                3 radio buttons, declare them as:
 *                  <option type="radio" name="xyz" @xyz-a@>
 *                  <option type="radio" name="xyz" @xyz-b@>
 *                  <option type="radio" name="xyz" @xyz-c@>
 *                Then map one of the @xyz-?@ variables to "checked"
 *                and all the others to empty by calling:
 *                map_radio(exports, "xyz", "abc", sel)
 *                Where 'sel' is 'a', 'b', or 'c'.
 *
 * Parameters  :
 *          1  :  exports = Exports map to modify.
 *          2  :  optionname = name for map
 *          3  :  values = null-terminated list of values;
 *          4  :  value = Selected value.
 *
 * CGI Parameters : None
 *
 * Returns     :  JB_ERR_OK     on success
 *                JB_ERR_MEMORY on out-of-memory
 *
 *********************************************************************/
static jb_err map_radio(struct map * exports,
                        const char * optionname,
                        const char * values,
                        int value)
{
   size_t len;
   char * buf;
   char * p;
   char c;

   assert(exports);
   assert(optionname);
   assert(values);

   len = strlen(optionname);
   buf = malloc(len + 3);
   if (buf == NULL)
   {
      return JB_ERR_MEMORY;
   }

   strcpy(buf, optionname);
   p = buf + len;
   *p++ = '-';
   p[1] = '\0';

   while ((c = *values++) != '\0')
   {
      if (c != value)
      {
         *p = c;
         if (map(exports, buf, 1, "", 1))
         {
            return JB_ERR_MEMORY;
         }
      }
   }

   *p = value;
   return map(exports, buf, 0, "checked", 1);
}


/*********************************************************************
 *
 * Function    :  cgi_error_modified
 *
 * Description :  CGI function that is called when a file is modified
 *                outside the CGI editor.
 *
 * Parameters  :
 *          1  :  csp = Current client state (buffers, headers, etc...)
 *          2  :  rsp = http_response data structure for output
 *          3  :  filename = The file that was modified.
 *
 * CGI Parameters : none
 *
 * Returns     :  JB_ERR_OK on success
 *                JB_ERR_MEMORY on out-of-memory error.
 *
 *********************************************************************/
jb_err cgi_error_modified(struct client_state *csp,
                          struct http_response *rsp,
                          const char *filename)
{
   struct map *exports;
   jb_err err;

   assert(csp);
   assert(rsp);
   assert(filename);

   if (NULL == (exports = default_exports(csp, NULL)))
   {
      return JB_ERR_MEMORY;
   }

   err = map(exports, "f", 1, html_encode(filename), 0);
   if (err)
   {
      free_map(exports);
      return err;
   }

   return template_fill_for_cgi(csp, "cgi-error-modified", exports, rsp);
}


/*********************************************************************
 *
 * Function    :  cgi_error_parse
 *
 * Description :  CGI function that is called when a file cannot
 *                be parsed by the CGI editor.
 *
 * Parameters  :
 *          1  :  csp = Current client state (buffers, headers, etc...)
 *          2  :  rsp = http_response data structure for output
 *          3  :  file = The file that was modified.
 *
 * CGI Parameters : none
 *
 * Returns     :  JB_ERR_OK on success
 *                JB_ERR_MEMORY on out-of-memory error.
 *
 *********************************************************************/
jb_err cgi_error_parse(struct client_state *csp,
                       struct http_response *rsp,
                       struct editable_file *file)
{
   struct map *exports;
   jb_err err;
   struct file_line *cur_line;

   assert(csp);
   assert(rsp);
   assert(file);

   if (NULL == (exports = default_exports(csp, NULL)))
   {
      return JB_ERR_MEMORY;
   }

   err = map(exports, "f", 1, file->identifier, 1);
   if (!err) err = map(exports, "parse-error", 1, html_encode(file->parse_error_text), 0);

   cur_line = file->parse_error;
   assert(cur_line);

   if (!err) err = map(exports, "line-raw", 1, html_encode(cur_line->raw), 0);
   if (!err) err = map(exports, "line-data", 1, html_encode(cur_line->unprocessed), 0);

   if (err)
   {
      free_map(exports);
      return err;
   }

   return template_fill_for_cgi(csp, "cgi-error-parse", exports, rsp);
}


/*********************************************************************
 *
 * Function    :  cgi_error_file
 *
 * Description :  CGI function that is called when a file cannot be
 *                opened by the CGI editor.
 *
 * Parameters  :
 *          1  :  csp = Current client state (buffers, headers, etc...)
 *          2  :  rsp = http_response data structure for output
 *          3  :  filename = The file that was modified.
 *
 * CGI Parameters : none
 *
 * Returns     :  JB_ERR_OK on success
 *                JB_ERR_MEMORY on out-of-memory error.
 *
 *********************************************************************/
jb_err cgi_error_file(struct client_state *csp,
                      struct http_response *rsp,
                      const char *filename)
{
   struct map *exports;
   jb_err err;

   assert(csp);
   assert(rsp);
   assert(filename);

   if (NULL == (exports = default_exports(csp, NULL)))
   {
      return JB_ERR_MEMORY;
   }

   err = map(exports, "f", 1, html_encode(filename), 0);
   if (err)
   {
      free_map(exports);
      return err;
   }

   return template_fill_for_cgi(csp, "cgi-error-file", exports, rsp);
}


/*********************************************************************
 *
 * Function    :  cgi_error_file
 *
 * Description :  CGI function that is called when a file cannot be
 *                opened for writing by the CGI editor.
 *
 * Parameters  :
 *          1  :  csp = Current client state (buffers, headers, etc...)
 *          2  :  rsp = http_response data structure for output
 *          3  :  filename = The file that we can't write to
 *
 * CGI Parameters : none
 *
 * Returns     :  JB_ERR_OK on success
 *                JB_ERR_MEMORY on out-of-memory error.
 *
 *********************************************************************/
jb_err cgi_error_file_read_only(struct client_state *csp,
                                struct http_response *rsp,
                                const char *filename)
{
   struct map *exports;
   jb_err err;

   assert(csp);
   assert(rsp);
   assert(filename);

   if (NULL == (exports = default_exports(csp, NULL)))
   {
      return JB_ERR_MEMORY;
   }

   err = map(exports, "f", 1, html_encode(filename), 0);
   if (err)
   {
      free_map(exports);
      return err;
   }

   return template_fill_for_cgi(csp, "cgi-error-file-read-only", exports, rsp);
}


/*********************************************************************
 *
 * Function    :  cgi_edit_actions
 *
 * Description :  CGI function that allows the user to choose which
 *                actions file to edit.
 *
 * Parameters  :
 *          1  :  csp = Current client state (buffers, headers, etc...)
 *          2  :  rsp = http_response data structure for output
 *          3  :  parameters = map of cgi parameters
 *
 * CGI Parameters : None
 *
 * Returns     :  JB_ERR_OK on success
 *                JB_ERR_MEMORY on out-of-memory error
 *
 *********************************************************************/
jb_err cgi_edit_actions(struct client_state *csp,
                        struct http_response *rsp,
                        const struct map *parameters)
{

   if (0 == (csp->config->feature_flags & RUNTIME_FEATURE_CGI_EDIT_ACTIONS))
   {
      return cgi_error_disabled(csp, rsp);
   }

   /* FIXME: Incomplete */

   return cgi_redirect(rsp, CGI_PREFIX "edit-actions-list?f=default");

}


/*********************************************************************
 *
 * Function    :  cgi_edit_actions_list
 *
 * Description :  CGI function that edits the actions list.
 *                FIXME: This function shouldn't FATAL ever.
 *                FIXME: This function doesn't check the retval of map()
 * Parameters  :
 *          1  :  csp = Current client state (buffers, headers, etc...)
 *          2  :  rsp = http_response data structure for output
 *          3  :  parameters = map of cgi parameters
 *
 * CGI Parameters : filename
 *
 * Returns     :  JB_ERR_OK     on success
 *                JB_ERR_MEMORY on out-of-memory
 *                JB_ERR_FILE   if the file cannot be opened or
 *                              contains no data
 *                JB_ERR_CGI_PARAMS if "filename" was not specified
 *                                  or is not valid.
 *
 *********************************************************************/
jb_err cgi_edit_actions_list(struct client_state *csp,
                             struct http_response *rsp,
                             const struct map *parameters)
{
   char * section_template;
   char * url_template;
   char * sections;
   char * urls;
   char buf[150];
   char * s;
   struct map * exports;
   struct map * section_exports;
   struct map * url_exports;
   struct editable_file * file;
   struct file_line * cur_line;
   unsigned line_number = 0;
   unsigned prev_section_line_number = ((unsigned) (-1));
   int i, url_1_2;
   struct file_list * fl;
   struct url_actions * b;
   char * buttons = NULL;
   jb_err err;

   if (0 == (csp->config->feature_flags & RUNTIME_FEATURE_CGI_EDIT_ACTIONS))
   {
      return cgi_error_disabled(csp, rsp);
   }

   if (NULL == (exports = default_exports(csp, NULL)))
   {
      return JB_ERR_MEMORY;
   }

   /* Load actions file */
   err = edit_read_actions_file(csp, rsp, parameters, 0, &file);
   if (err)
   {
      /* No filename specified, can't read file, or out of memory. */
      return (err == JB_ERR_FILE ? JB_ERR_OK : err);
   }

   /* Find start of actions in file */
   cur_line = file->lines;
   line_number = 1;
   while ((cur_line != NULL) && (cur_line->type != FILE_LINE_ACTION))
   {
      cur_line = cur_line->next;
      line_number++;
   }

   /*
    * Conventional actions files should have a match all block
    * at the start:
    * cur_line             = {...global actions...}
    * cur_line->next       = /
    * cur_line->next->next = {...actions...} or EOF
    */
   if ( (cur_line != NULL)
     && (cur_line->type == FILE_LINE_ACTION)
     && (cur_line->next != NULL)
     && (cur_line->next->type == FILE_LINE_URL)
     && (0 == strcmp(cur_line->next->unprocessed, "/"))
     && ( (cur_line->next->next == NULL)
       || (cur_line->next->next->type != FILE_LINE_URL)
      ) )
   {
      /*
       * Generate string with buttons to set actions for "/" to
       * any predefined set of actions (named standard.*, probably
       * residing in standard.action).
       */

      err = template_load(csp, &section_template, "edit-actions-list-button", 0);
      if (err)
      {
         edit_free_file(file);
         free_map(exports);
         if (err == JB_ERR_FILE)
         {
            return cgi_error_no_template(csp, rsp, "edit-actions-list-button");
         }
         return err;
      }

      err = template_fill(&section_template, exports);
      if (err)
      {
         edit_free_file(file);
         free_map(exports);
         return err;
      }

      buttons = strdup("");
      for (i = 0; i < MAX_AF_FILES; i++)
      {
         if (((fl = csp->actions_list[i]) != NULL) && ((b = fl->f) != NULL))
         {
            for (b = b->next; NULL != b; b = b->next)
            {
               if (!strncmp(b->url->spec, "standard.", 9) && *(b->url->spec + 9) != '\0')
               {
                  if (err || (NULL == (section_exports = new_map())))
                  {
                     freez(buttons);
                     free(section_template);
                     edit_free_file(file);
                     free_map(exports);
                     return JB_ERR_MEMORY;
                  }

                  err = map(section_exports, "button-name", 1, b->url->spec + 9, 1);

                  if (err || (NULL == (s = strdup(section_template))))
                  {
                     free_map(section_exports);
                     freez(buttons);
                     free(section_template);
                     edit_free_file(file);
                     free_map(exports);
                     return JB_ERR_MEMORY;
                  }

                  if (!err) err = template_fill(&s, section_exports);
                  free_map(section_exports);
                  if (!err) err = string_join(&buttons, s);
               }
            }
         }
      }
      freez(section_template);
      if (!err) err = map(exports, "all-urls-buttons", 1, buttons, 0);

      /*
       * Conventional actions file, supply extra editing help.
       * (e.g. don't allow them to make it an unconventional one).
       */
      if (!err) err = map_conditional(exports, "all-urls-present", 1);

      snprintf(buf, 150, "%d", line_number);
      if (!err) err = map(exports, "all-urls-s", 1, buf, 1);
      snprintf(buf, 150, "%d", line_number + 2);
      if (!err) err = map(exports, "all-urls-s-next", 1, buf, 1);
      if (!err) err = map(exports, "all-urls-actions", 1,
                          actions_to_html(csp, cur_line->data.action), 0);

       /* Skip the 2 lines */
      cur_line = cur_line->next->next;
      line_number += 2;

      /*
       * Note that prev_section_line_number is NOT set here.
       * This is deliberate and not a bug.  It stops a "Move up"
       * option appearing on the next section.  Clicking "Move
       * up" would make the actions file unconventional, which
       * we don't want, so we hide this option.
       */
   }
   else
   {
      /*
       * Non-standard actions file - does not begin with
       * the "All URLs" section.
       */
      if (!err) err = map_conditional(exports, "all-urls-present", 0);
   }

   /* Set up global exports */

   if (!err) err = map(exports, "f", 1, file->identifier, 1);
   if (!err) err = map(exports, "v", 1, file->version_str, 1);

   /* Discourage private additions to default.action */

   if (!err) err = map_conditional(exports, "default-action",
                                   (strcmp("default", lookup(parameters, "f")) == 0));
   if (err)
   {
      edit_free_file(file);
      free_map(exports);
      return err;
   }

   /* Should do all global exports above this point */

   /* Load templates */

   err = template_load(csp, &section_template, "edit-actions-list-section", 0);
   if (err)
   {
      edit_free_file(file);
      free_map(exports);
      if (err == JB_ERR_FILE)
      {
         return cgi_error_no_template(csp, rsp, "edit-actions-list-section");
      }
      return err;
   }

   err = template_load(csp, &url_template, "edit-actions-list-url", 0);
   if (err)
   {
      free(section_template);
      edit_free_file(file);
      free_map(exports);
      if (err == JB_ERR_FILE)
      {
         return cgi_error_no_template(csp, rsp, "edit-actions-list-url");
      }
      return err;
   }

   err = template_fill(&section_template, exports);
   if (err)
   {
      free(url_template);
      edit_free_file(file);
      free_map(exports);
      free(url_template);
      return err;
   }

   err = template_fill(&url_template, exports);
   if (err)
   {
      free(section_template);
      edit_free_file(file);
      free_map(exports);
      return err;
   }

   if (NULL == (sections = strdup("")))
   {
      free(section_template);
      free(url_template);
      edit_free_file(file);
      free_map(exports);
      return JB_ERR_MEMORY;
   }

   while ((cur_line != NULL) && (cur_line->type == FILE_LINE_ACTION))
   {
      if (NULL == (section_exports = new_map()))
      {
         free(sections);
         free(section_template);
         free(url_template);
         edit_free_file(file);
         free_map(exports);
         return JB_ERR_MEMORY;
      }

      snprintf(buf, 150, "%d", line_number);
      err = map(section_exports, "s", 1, buf, 1);
      if (!err) err = map(section_exports, "actions", 1,
                          actions_to_html(csp, cur_line->data.action), 0);

      if ( (!err)
        && (cur_line->next != NULL)
        && (cur_line->next->type == FILE_LINE_URL))
      {
         /* This section contains at least one URL, don't allow delete */
         err = map_block_killer(section_exports, "empty-section");
      }
      else
      {
         if (!err) err = map_block_keep(section_exports, "empty-section");
      }

      if (prev_section_line_number != ((unsigned)(-1)))
      {
         /* Not last section */
         snprintf(buf, 150, "%d", prev_section_line_number);
         if (!err) err = map(section_exports, "s-prev", 1, buf, 1);
         if (!err) err = map_block_keep(section_exports, "s-prev-exists");
      }
      else
      {
         /* Last section */
         if (!err) err = map_block_killer(section_exports, "s-prev-exists");
      }
      prev_section_line_number = line_number;

      if (err)
      {
         free(sections);
         free(section_template);
         free(url_template);
         edit_free_file(file);
         free_map(exports);
         free_map(section_exports);
         return err;
      }

      /* Should do all section-specific exports above this point */

      if (NULL == (urls = strdup("")))
      {
         free(sections);
         free(section_template);
         free(url_template);
         edit_free_file(file);
         free_map(exports);
         free_map(section_exports);
         return JB_ERR_MEMORY;
      }

      url_1_2 = 2;

      cur_line = cur_line->next;
      line_number++;

      while ((cur_line != NULL) && (cur_line->type == FILE_LINE_URL))
      {
         if (NULL == (url_exports = new_map()))
         {
            free(urls);
            free(sections);
            free(section_template);
            free(url_template);
            edit_free_file(file);
            free_map(exports);
            free_map(section_exports);
            return JB_ERR_MEMORY;
         }

         snprintf(buf, 150, "%d", line_number);
         err = map(url_exports, "p", 1, buf, 1);

         snprintf(buf, 150, "%d", url_1_2);
         if (!err) err = map(url_exports, "url-1-2", 1, buf, 1);

         if (!err) err = map(url_exports, "url-html", 1,
                             html_encode(cur_line->unprocessed), 0);
         if (!err) err = map(url_exports, "url", 1,
                             url_encode(cur_line->unprocessed), 0);

         if (err)
         {
            free(urls);
            free(sections);
            free(section_template);
            free(url_template);
            edit_free_file(file);
            free_map(exports);
            free_map(section_exports);
            free_map(url_exports);
            return err;
         }

         if (NULL == (s = strdup(url_template)))
         {
            free(urls);
            free(sections);
            free(section_template);
            free(url_template);
            edit_free_file(file);
            free_map(exports);
            free_map(section_exports);
            free_map(url_exports);
            return JB_ERR_MEMORY;
         }

         err = template_fill(&s, section_exports);
         if (!err) err = template_fill(&s, url_exports);
         if (!err) err = string_append(&urls, s);

         free_map(url_exports);
         freez(s);

         if (err)
         {
            freez(urls);
            free(sections);
            free(section_template);
            free(url_template);
            edit_free_file(file);
            free_map(exports);
            free_map(section_exports);
            return err;
         }

         url_1_2 = 3 - url_1_2;

         cur_line = cur_line->next;
         line_number++;
      }

      err = map(section_exports, "urls", 1, urls, 0);

      /* Could also do section-specific exports here, but it wouldn't be as fast */

      snprintf(buf, 150, "%d", line_number);
      if (!err) err = map(section_exports, "s-next", 1, buf, 1);

      if ( (cur_line != NULL)
        && (cur_line->type == FILE_LINE_ACTION))
      {
         /* Not last section */
         if (!err) err = map_block_keep(section_exports, "s-next-exists");
      }
      else
      {
         /* Last section */
         if (!err) err = map_block_killer(section_exports, "s-next-exists");
      }

      if (err)
      {
         free(sections);
         free(section_template);
         free(url_template);
         edit_free_file(file);
         free_map(exports);
         free_map(section_exports);
         return err;
      }

      if (NULL == (s = strdup(section_template)))
      {
         free(sections);
         free(section_template);
         free(url_template);
         edit_free_file(file);
         free_map(exports);
         free_map(section_exports);
         return JB_ERR_MEMORY;
      }

      err = template_fill(&s, section_exports);
      if (!err) err = string_append(&sections, s);

      freez(s);
      free_map(section_exports);

      if (err)
      {
         freez(sections);
         free(section_template);
         free(url_template);
         edit_free_file(file);
         free_map(exports);
         return err;
      }
   }

   edit_free_file(file);
   free(section_template);
   free(url_template);

   err = map(exports, "sections", 1, sections, 0);
   if (err)
   {
      free_map(exports);
      return err;
   }

   /* Could also do global exports here, but it wouldn't be as fast */

   return template_fill_for_cgi(csp, "edit-actions-list", exports, rsp);
}


/*********************************************************************
 *
 * Function    :  cgi_edit_actions_for_url
 *
 * Description :  CGI function that edits the Actions list.
 *
 * Parameters  :
 *          1  :  csp = Current client state (buffers, headers, etc...)
 *          2  :  rsp = http_response data structure for output
 *          3  :  parameters = map of cgi parameters
 *
 * CGI Parameters : None
 *
 * Returns     :  JB_ERR_OK     on success
 *                JB_ERR_MEMORY on out-of-memory
 *                JB_ERR_CGI_PARAMS if the CGI parameters are not
 *                                  specified or not valid.
 *
 *********************************************************************/
jb_err cgi_edit_actions_for_url(struct client_state *csp,
                                struct http_response *rsp,
                                const struct map *parameters)
{
   struct map * exports;
   unsigned sectionid;
   struct editable_file * file;
   struct file_line * cur_line;
   unsigned line_number;
   jb_err err;
   struct re_filterfile_spec *filter_group;
   int i, have_filters = 0;

   if (0 == (csp->config->feature_flags & RUNTIME_FEATURE_CGI_EDIT_ACTIONS))
   {
      return cgi_error_disabled(csp, rsp);
   }

   err = get_number_param(csp, parameters, "s", &sectionid);
   if (err)
   {
      return err;
   }

   err = edit_read_actions_file(csp, rsp, parameters, 1, &file);
   if (err)
   {
      /* No filename specified, can't read file, modified, or out of memory. */
      return (err == JB_ERR_FILE ? JB_ERR_OK : err);
   }

   cur_line = file->lines;

   for (line_number = 1; (cur_line != NULL) && (line_number < sectionid); line_number++)
   {
      cur_line = cur_line->next;
   }

   if ( (cur_line == NULL)
     || (line_number != sectionid)
     || (sectionid < 1)
     || (cur_line->type != FILE_LINE_ACTION))
   {
      /* Invalid "sectionid" parameter */
      edit_free_file(file);
      return JB_ERR_CGI_PARAMS;
   }

   if (NULL == (exports = default_exports(csp, NULL)))
   {
      edit_free_file(file);
      return JB_ERR_MEMORY;
   }

   err = map(exports, "f", 1, file->identifier, 1);
   if (!err) err = map(exports, "v", 1, file->version_str, 1);
   if (!err) err = map(exports, "s", 1, url_encode(lookup(parameters, "s")), 0);

   if (!err) err = actions_to_radio(exports, cur_line->data.action);

   for (i = 0; i < MAX_AF_FILES; i++)
   {
      if ((csp->rlist[i] != NULL) && (csp->rlist[i]->f != NULL))
      {
         if (!err) err = map_conditional(exports, "any-filters-defined", 1);
         have_filters = 1;
         break;
      }
   }

   if (err)
   {
      edit_free_file(file);
      free_map(exports);
      return err;
   }

   if (0 == have_filters)
      err = map(exports, "filter-params", 1, "", 1);
   else
   {
      /* We have some entries in the filter list */
      char * result;
      int index = 0;
      char * filter_template;

      err = template_load(csp, &filter_template, "edit-actions-for-url-filter", 0);
      if (err)
      {
         edit_free_file(file);
         free_map(exports);
         if (err == JB_ERR_FILE)
         {
            return cgi_error_no_template(csp, rsp, "edit-actions-for-url-filter");
         }
         return err;
      }

      err = template_fill(&filter_template, exports);

      result = strdup("");

      for (i = 0; i < MAX_AF_FILES; i++)
      {
         if ((csp->rlist[i] != NULL) && (csp->rlist[i]->f != NULL))
         {
            filter_group = csp->rlist[i]->f;
            for (;(!err) && (filter_group != NULL); filter_group = filter_group->next)
            {
               char current_mode = 'x';
               struct list_entry *filter_name;
               char * this_line;
               struct map *line_exports;
               char number[20];

               filter_name = cur_line->data.action->multi_add[ACTION_MULTI_FILTER]->first;
               while ((filter_name != NULL)
                   && (0 != strcmp(filter_group->name, filter_name->str)))
               {
                    filter_name = filter_name->next;
               }

               if (filter_name != NULL)
               {
                  current_mode = 'y';
               }
               else
               {
                  filter_name = cur_line->data.action->multi_remove[ACTION_MULTI_FILTER]->first;
                  while ((filter_name != NULL)
                      && (0 != strcmp(filter_group->name, filter_name->str)))
                  {
                       filter_name = filter_name->next;
                  }
                  if (filter_name != NULL)
                  {
                     current_mode = 'n';
                  }
               }

               /* Generate a unique serial number */
               snprintf(number, sizeof(number), "%x", index++);
               number[sizeof(number) - 1] = '\0';

               line_exports = new_map();
               if (line_exports == NULL)
               {
                  err = JB_ERR_MEMORY;
                  freez(result);
               }
               else
               {
                  if (!err) err = map(line_exports, "index", 1, number, 1);
                  if (!err) err = map(line_exports, "name",  1, filter_group->name, 1);
                  if (!err) err = map(line_exports, "description",  1, filter_group->description, 1);
                  if (!err) err = map_radio(line_exports, "this-filter", "ynx", current_mode);

                  this_line = NULL;
                  if (!err)
                  {
                     this_line = strdup(filter_template);
                     if (this_line == NULL) err = JB_ERR_MEMORY;
                  }
                  if (!err) err = template_fill(&this_line, line_exports);
                  string_join(&result, this_line);

                  free_map(line_exports);
               }
            }
         }
      }
      freez(filter_template);

      if (!err)
      {
         err = map(exports, "filter-params", 1, result, 0);
      }
      else
      {
         freez(result);
      }
   }

   if (!err) err = map_radio(exports, "filter-all", "nx",
      (cur_line->data.action->multi_remove_all[ACTION_MULTI_FILTER] ? 'n' : 'x'));

   edit_free_file(file);

   if (err)
   {
      free_map(exports);
      return err;
   }

   return template_fill_for_cgi(csp, "edit-actions-for-url", exports, rsp);
}


/*********************************************************************
 *
 * Function    :  cgi_edit_actions_submit
 *
 * Description :  CGI function that actually edits the Actions list.
 *
 * Parameters  :
 *          1  :  csp = Current client state (buffers, headers, etc...)
 *          2  :  rsp = http_response data structure for output
 *          3  :  parameters = map of cgi parameters
 *
 * CGI Parameters : None
 *
 * Returns     :  JB_ERR_OK     on success
 *                JB_ERR_MEMORY on out-of-memory
 *                JB_ERR_CGI_PARAMS if the CGI parameters are not
 *                                  specified or not valid.
 *
 *********************************************************************/
jb_err cgi_edit_actions_submit(struct client_state *csp,
                               struct http_response *rsp,
                               const struct map *parameters)
{
   unsigned sectionid;
   char * actiontext;
   char * newtext;
   size_t len;
   struct editable_file * file;
   struct file_line * cur_line;
   unsigned line_number;
   char target[1024];
   jb_err err;
   int index;
   const char * action_set_name;
   char ch;
   struct file_list * fl;
   struct url_actions * b;

   if (0 == (csp->config->feature_flags & RUNTIME_FEATURE_CGI_EDIT_ACTIONS))
   {
      return cgi_error_disabled(csp, rsp);
   }

   err = get_number_param(csp, parameters, "s", &sectionid);
   if (err)
   {
      return err;
   }

   err = edit_read_actions_file(csp, rsp, parameters, 1, &file);
   if (err)
   {
      /* No filename specified, can't read file, modified, or out of memory. */
      return (err == JB_ERR_FILE ? JB_ERR_OK : err);
   }

   cur_line = file->lines;

   for (line_number = 1; (cur_line != NULL) && (line_number < sectionid); line_number++)
   {
      cur_line = cur_line->next;
   }

   if ( (cur_line == NULL)
     || (line_number != sectionid)
     || (sectionid < 1)
     || (cur_line->type != FILE_LINE_ACTION))
   {
      /* Invalid "sectionid" parameter */
      edit_free_file(file);
      return JB_ERR_CGI_PARAMS;
   }

   get_string_param(parameters, "p", &action_set_name);
   if (action_set_name != NULL)
   {
      for (index = 0; index < MAX_AF_FILES; index++)
      {
         if (((fl = csp->actions_list[index]) != NULL) && ((b = fl->f) != NULL))
         {
            for (b = b->next; NULL != b; b = b->next)
            {
               if (!strncmp(b->url->spec, "standard.", 9) && !strcmp(b->url->spec + 9, action_set_name))
               {
                  copy_action(cur_line->data.action, b->action); 
                  goto found;
               }
            }
         }
      }
      edit_free_file(file);
      return JB_ERR_CGI_PARAMS;

      found: ;
   }
   else
   {
      err = actions_from_radio(parameters, cur_line->data.action);
   }

   if(err)
   {
      /* Out of memory */
      edit_free_file(file);
      return err;
   }

   ch = get_char_param(parameters, "filter_all");
   if (ch == 'N')
   {
      list_remove_all(cur_line->data.action->multi_add[ACTION_MULTI_FILTER]);
      list_remove_all(cur_line->data.action->multi_remove[ACTION_MULTI_FILTER]);
      cur_line->data.action->multi_remove_all[ACTION_MULTI_FILTER] = 1;
   }
   else if (ch == 'X')
   {
      cur_line->data.action->multi_remove_all[ACTION_MULTI_FILTER] = 0;
   }

   for (index = 0; !err; index++)
   {
      char key_value[30];
      char key_name[30];
      const char *name;
      char value;

      /* Generate the keys */
      snprintf(key_value, sizeof(key_value), "filter_r%x", index);
      key_value[sizeof(key_value) - 1] = '\0';
      snprintf(key_name, sizeof(key_name), "filter_n%x", index);
      key_name[sizeof(key_name) - 1] = '\0';

      err = get_string_param(parameters, key_name, &name);
      if (err) break;

      if (name == NULL)
      {
         /* End of list */
         break;
      }

      value = get_char_param(parameters, key_value);
      if (value == 'Y')
      {
         list_remove_item(cur_line->data.action->multi_add[ACTION_MULTI_FILTER], name);
         if (!err) err = enlist(cur_line->data.action->multi_add[ACTION_MULTI_FILTER], name);
         list_remove_item(cur_line->data.action->multi_remove[ACTION_MULTI_FILTER], name);
      }
      else if (value == 'N')
      {
         list_remove_item(cur_line->data.action->multi_add[ACTION_MULTI_FILTER], name);
         if (!cur_line->data.action->multi_remove_all[ACTION_MULTI_FILTER])
         {
            list_remove_item(cur_line->data.action->multi_remove[ACTION_MULTI_FILTER], name);
            if (!err) err = enlist(cur_line->data.action->multi_remove[ACTION_MULTI_FILTER], name);
         }
      }
      else if (value == 'X')
      {
         list_remove_item(cur_line->data.action->multi_add[ACTION_MULTI_FILTER], name);
         list_remove_item(cur_line->data.action->multi_remove[ACTION_MULTI_FILTER], name);
      }
   }

   if(err)
   {
      /* Out of memory */
      edit_free_file(file);
      return err;
   }

   if (NULL == (actiontext = actions_to_text(cur_line->data.action)))
   {
      /* Out of memory */
      edit_free_file(file);
      return JB_ERR_MEMORY;
   }

   len = strlen(actiontext);
   if (len == 0)
   {
      /*
       * Empty action - must special-case this.
       * Simply setting len to 1 is sufficient...
       */
      len = 1;
   }

   if (NULL == (newtext = malloc(len + 2)))
   {
      /* Out of memory */
      free(actiontext);
      edit_free_file(file);
      return JB_ERR_MEMORY;
   }
   strcpy(newtext, actiontext);
   free(actiontext);
   newtext[0]       = '{';
   newtext[len]     = '}';
   newtext[len + 1] = '\0';

   freez(cur_line->raw);
   freez(cur_line->unprocessed);
   cur_line->unprocessed = newtext;

   err = edit_write_file(file);
   if (err)
   {
      /* Error writing file */
      if (err == JB_ERR_FILE)
      {
         /* Read-only file. */
         err = cgi_error_file_read_only(csp, rsp, file->identifier);
      }
      edit_free_file(file);
      return err;
   }

   snprintf(target, 1024, CGI_PREFIX "edit-actions-list?foo=%lu&f=%s#l%d",
            (long) time(NULL), file->identifier, sectionid);

   edit_free_file(file);

   return cgi_redirect(rsp, target);
}


/*********************************************************************
 *
 * Function    :  cgi_edit_actions_url
 *
 * Description :  CGI function that actually edits a URL pattern in
 *                an actions file.
 *
 * Parameters  :
 *          1  :  csp = Current client state (buffers, headers, etc...)
 *          2  :  rsp = http_response data structure for output
 *          3  :  parameters = map of cgi parameters
 *
 * CGI Parameters :
 *    filename : Identifies the file to edit
 *         ver : File's last-modified time
 *     section : Line number of section to edit
 *     pattern : Line number of pattern to edit
 *      newval : New value for pattern
 *
 * Returns     :  JB_ERR_OK     on success
 *                JB_ERR_MEMORY on out-of-memory
 *                JB_ERR_CGI_PARAMS if the CGI parameters are not
 *                                  specified or not valid.
 *
 *********************************************************************/
jb_err cgi_edit_actions_url(struct client_state *csp,
                            struct http_response *rsp,
                            const struct map *parameters)
{
   unsigned patternid;
   char * new_pattern;
   struct editable_file * file;
   struct file_line * cur_line;
   unsigned line_number;
   unsigned section_start_line_number = 0;
   char target[1024];
   jb_err err;

   assert(csp);
   assert(rsp);
   assert(parameters);

   if (0 == (csp->config->feature_flags & RUNTIME_FEATURE_CGI_EDIT_ACTIONS))
   {
      return cgi_error_disabled(csp, rsp);
   }

   err = get_number_param(csp, parameters, "p", &patternid);
   if (err)
   {
      return err;
   }
   if (patternid < 1U)
   {
      return JB_ERR_CGI_PARAMS;
   }

   err = get_url_spec_param(csp, parameters, "u", &new_pattern);
   if (err)
   {
      return err;
   }

   err = edit_read_actions_file(csp, rsp, parameters, 1, &file);
   if (err)
   {
      /* No filename specified, can't read file, modified, or out of memory. */
      free(new_pattern);
      return (err == JB_ERR_FILE ? JB_ERR_OK : err);
   }

   line_number = 1;
   cur_line = file->lines;

   while ((cur_line != NULL) && (line_number < patternid))
   {
      if (cur_line->type == FILE_LINE_ACTION)
      {
         section_start_line_number = line_number;
      }      
      cur_line = cur_line->next;
      line_number++;
   }

   if ( (cur_line == NULL)
     || (cur_line->type != FILE_LINE_URL))
   {
      /* Invalid "patternid" parameter */
      free(new_pattern);
      edit_free_file(file);
      return JB_ERR_CGI_PARAMS;
   }

   /* At this point, the line to edit is in cur_line */

   freez(cur_line->raw);
   freez(cur_line->unprocessed);
   cur_line->unprocessed = new_pattern;

   err = edit_write_file(file);
   if (err)
   {
      /* Error writing file */
      if (err == JB_ERR_FILE)
      {
         /* Read-only file. */
         err = cgi_error_file_read_only(csp, rsp, file->identifier);
      }
      edit_free_file(file);
      return err;
   }

   snprintf(target, 1024, CGI_PREFIX "edit-actions-list?foo=%lu&f=%s#l%d",
            (long) time(NULL), file->identifier, section_start_line_number);

   edit_free_file(file);

   return cgi_redirect(rsp, target);
}


/*********************************************************************
 *
 * Function    :  cgi_edit_actions_add_url
 *
 * Description :  CGI function that actually adds a URL pattern to
 *                an actions file.
 *
 * Parameters  :
 *          1  :  csp = Current client state (buffers, headers, etc...)
 *          2  :  rsp = http_response data structure for output
 *          3  :  parameters = map of cgi parameters
 *
 * CGI Parameters :
 *    filename : Identifies the file to edit
 *         ver : File's last-modified time
 *     section : Line number of section to edit
 *      newval : New pattern
 *
 * Returns     :  JB_ERR_OK     on success
 *                JB_ERR_MEMORY on out-of-memory
 *                JB_ERR_CGI_PARAMS if the CGI parameters are not
 *                                  specified or not valid.
 *
 *********************************************************************/
jb_err cgi_edit_actions_add_url(struct client_state *csp,
                                struct http_response *rsp,
                                const struct map *parameters)
{
   unsigned sectionid;
   char * new_pattern;
   struct file_line * new_line;
   struct editable_file * file;
   struct file_line * cur_line;
   unsigned line_number;
   char target[1024];
   jb_err err;

   if (0 == (csp->config->feature_flags & RUNTIME_FEATURE_CGI_EDIT_ACTIONS))
   {
      return cgi_error_disabled(csp, rsp);
   }

   err = get_number_param(csp, parameters, "s", &sectionid);
   if (err)
   {
      return err;
   }
   if (sectionid < 1U)
   {
      return JB_ERR_CGI_PARAMS;
   }

   err = get_url_spec_param(csp, parameters, "u", &new_pattern);
   if (err)
   {
      return err;
   }

   err = edit_read_actions_file(csp, rsp, parameters, 1, &file);
   if (err)
   {
      /* No filename specified, can't read file, modified, or out of memory. */
      free(new_pattern);
      return (err == JB_ERR_FILE ? JB_ERR_OK : err);
   }

   line_number = 1;
   cur_line = file->lines;

   while ((cur_line != NULL) && (line_number < sectionid))
   {
      cur_line = cur_line->next;
      line_number++;
   }

   if ( (cur_line == NULL)
     || (cur_line->type != FILE_LINE_ACTION))
   {
      /* Invalid "sectionid" parameter */
      free(new_pattern);
      edit_free_file(file);
      return JB_ERR_CGI_PARAMS;
   }

   /* At this point, the section header is in cur_line - add after this. */

   /* Allocate the new line */
   new_line = (struct file_line *)zalloc(sizeof(*new_line));
   if (new_line == NULL)
   {
      free(new_pattern);
      edit_free_file(file);
      return JB_ERR_MEMORY;
   }

   /* Fill in the data members of the new line */
   new_line->raw = NULL;
   new_line->prefix = NULL;
   new_line->unprocessed = new_pattern;
   new_line->type = FILE_LINE_URL;

   /* Link new_line into the list, after cur_line */
   new_line->next = cur_line->next;
   cur_line->next = new_line;

   /* Done making changes, now commit */

   err = edit_write_file(file);
   if (err)
   {
      /* Error writing file */
      if (err == JB_ERR_FILE)
      {
         /* Read-only file. */
         err = cgi_error_file_read_only(csp, rsp, file->identifier);
      }
      edit_free_file(file);
      return err;
   }

   snprintf(target, 1024, CGI_PREFIX "edit-actions-list?foo=%lu&f=%s#l%d",
            (long) time(NULL), file->identifier, sectionid);

   edit_free_file(file);

   return cgi_redirect(rsp, target);
}


/*********************************************************************
 *
 * Function    :  cgi_edit_actions_remove_url
 *
 * Description :  CGI function that actually removes a URL pattern from
 *                the actions file.
 *
 * Parameters  :
 *          1  :  csp = Current client state (buffers, headers, etc...)
 *          2  :  rsp = http_response data structure for output
 *          3  :  parameters = map of cgi parameters
 *
 * CGI Parameters :
 *           f : (filename) Identifies the file to edit
 *           v : (version) File's last-modified time
 *           p : (pattern) Line number of pattern to remove
 *
 * Returns     :  JB_ERR_OK     on success
 *                JB_ERR_MEMORY on out-of-memory
 *                JB_ERR_CGI_PARAMS if the CGI parameters are not
 *                                  specified or not valid.
 *
 *********************************************************************/
jb_err cgi_edit_actions_remove_url(struct client_state *csp,
                                   struct http_response *rsp,
                                   const struct map *parameters)
{
   unsigned patternid;
   struct editable_file * file;
   struct file_line * cur_line;
   struct file_line * prev_line;
   unsigned line_number;
   unsigned section_start_line_number = 0;
   char target[1024];
   jb_err err;

   if (0 == (csp->config->feature_flags & RUNTIME_FEATURE_CGI_EDIT_ACTIONS))
   {
      return cgi_error_disabled(csp, rsp);
   }

   err = get_number_param(csp, parameters, "p", &patternid);
   if (err)
   {
      return err;
   }

   err = edit_read_actions_file(csp, rsp, parameters, 1, &file);
   if (err)
   {
      /* No filename specified, can't read file, modified, or out of memory. */
      return (err == JB_ERR_FILE ? JB_ERR_OK : err);
   }

   line_number = 1;
   prev_line = NULL;
   cur_line = file->lines;

   while ((cur_line != NULL) && (line_number < patternid))
   {
      if (cur_line->type == FILE_LINE_ACTION)
      {
         section_start_line_number = line_number;
      }
      prev_line = cur_line;
      cur_line = cur_line->next;
      line_number++;
   }

   if ( (cur_line == NULL)
     || (prev_line == NULL)
     || (cur_line->type != FILE_LINE_URL))
   {
      /* Invalid "patternid" parameter */
      edit_free_file(file);
      return JB_ERR_CGI_PARAMS;
   }

   /* At this point, the line to remove is in cur_line, and the previous
    * one is in prev_line
    */

   /* Unlink cur_line */
   prev_line->next = cur_line->next;
   cur_line->next = NULL;

   /* Free cur_line */
   edit_free_file_lines(cur_line);

   err = edit_write_file(file);
   if (err)
   {
      /* Error writing file */
      if (err == JB_ERR_FILE)
      {
         /* Read-only file. */
         err = cgi_error_file_read_only(csp, rsp, file->identifier);
      }
      edit_free_file(file);
      return err;
   }

   snprintf(target, 1024, CGI_PREFIX "edit-actions-list?foo=%lu&f=%s#l%d",
            (long) time(NULL), file->identifier, section_start_line_number);

   edit_free_file(file);

   return cgi_redirect(rsp, target);
}


/*********************************************************************
 *
 * Function    :  cgi_edit_actions_section_remove
 *
 * Description :  CGI function that actually removes a whole section from
 *                the actions file.  The section must be empty first
 *                (else JB_ERR_CGI_PARAMS).
 *
 * Parameters  :
 *          1  :  csp = Current client state (buffers, headers, etc...)
 *          2  :  rsp = http_response data structure for output
 *          3  :  parameters = map of cgi parameters
 *
 * CGI Parameters :
 *           f : (filename) Identifies the file to edit
 *           v : (version) File's last-modified time
 *           s : (section) Line number of section to edit
 *
 * Returns     :  JB_ERR_OK     on success
 *                JB_ERR_MEMORY on out-of-memory
 *                JB_ERR_CGI_PARAMS if the CGI parameters are not
 *                                  specified or not valid.
 *
 *********************************************************************/
jb_err cgi_edit_actions_section_remove(struct client_state *csp,
                                       struct http_response *rsp,
                                       const struct map *parameters)
{
   unsigned sectionid;
   struct editable_file * file;
   struct file_line * cur_line;
   struct file_line * prev_line;
   unsigned line_number;
   char target[1024];
   jb_err err;

   if (0 == (csp->config->feature_flags & RUNTIME_FEATURE_CGI_EDIT_ACTIONS))
   {
      return cgi_error_disabled(csp, rsp);
   }

   err = get_number_param(csp, parameters, "s", &sectionid);
   if (err)
   {
      return err;
   }

   err = edit_read_actions_file(csp, rsp, parameters, 1, &file);
   if (err)
   {
      /* No filename specified, can't read file, modified, or out of memory. */
      return (err == JB_ERR_FILE ? JB_ERR_OK : err);
   }

   line_number = 1;
   cur_line = file->lines;

   prev_line = NULL;
   while ((cur_line != NULL) && (line_number < sectionid))
   {
      prev_line = cur_line;
      cur_line = cur_line->next;
      line_number++;
   }

   if ( (cur_line == NULL)
     || (cur_line->type != FILE_LINE_ACTION) )
   {
      /* Invalid "sectionid" parameter */
      edit_free_file(file);
      return JB_ERR_CGI_PARAMS;
   }

   if ( (cur_line->next != NULL)
     && (cur_line->next->type == FILE_LINE_URL) )
   {
      /* Section not empty. */
      edit_free_file(file);
      return JB_ERR_CGI_PARAMS;
   }

   /* At this point, the line to remove is in cur_line, and the previous
    * one is in prev_line
    */

   /* Unlink cur_line */
   if (prev_line == NULL)
   {
      /* Removing the first line from the file */
      file->lines = cur_line->next;
   }
   else
   {
      prev_line->next = cur_line->next;
   }
   cur_line->next = NULL;

   /* Free cur_line */
   edit_free_file_lines(cur_line);

   err = edit_write_file(file);
   if (err)
   {
      /* Error writing file */
      if (err == JB_ERR_FILE)
      {
         /* Read-only file. */
         err = cgi_error_file_read_only(csp, rsp, file->identifier);
      }
      edit_free_file(file);
      return err;
   }

   snprintf(target, 1024, CGI_PREFIX "edit-actions-list?foo=%lu&f=%s",
            (long) time(NULL), file->identifier);

   edit_free_file(file);

   return cgi_redirect(rsp, target);
}


/*********************************************************************
 *
 * Function    :  cgi_edit_actions_section_add
 *
 * Description :  CGI function that adds a new empty section to
 *                an actions file.
 *
 * Parameters  :
 *          1  :  csp = Current client state (buffers, headers, etc...)
 *          2  :  rsp = http_response data structure for output
 *          3  :  parameters = map of cgi parameters
 *
 * CGI Parameters :
 *           f : (filename) Identifies the file to edit
 *           v : (version) File's last-modified time
 *           s : (section) Line number of section to add after, 0 for
 *               start of file.
 *
 * Returns     :  JB_ERR_OK     on success
 *                JB_ERR_MEMORY on out-of-memory
 *                JB_ERR_CGI_PARAMS if the CGI parameters are not
 *                                  specified or not valid.
 *
 *********************************************************************/
jb_err cgi_edit_actions_section_add(struct client_state *csp,
                                    struct http_response *rsp,
                                    const struct map *parameters)
{
   unsigned sectionid;
   struct file_line * new_line;
   char * new_text;
   struct editable_file * file;
   struct file_line * cur_line;
   unsigned line_number;
   char target[1024];
   jb_err err;

   if (0 == (csp->config->feature_flags & RUNTIME_FEATURE_CGI_EDIT_ACTIONS))
   {
      return cgi_error_disabled(csp, rsp);
   }

   err = get_number_param(csp, parameters, "s", &sectionid);
   if (err)
   {
      return err;
   }

   err = edit_read_actions_file(csp, rsp, parameters, 1, &file);
   if (err)
   {
      /* No filename specified, can't read file, modified, or out of memory. */
      return (err == JB_ERR_FILE ? JB_ERR_OK : err);
   }

   line_number = 1;
   cur_line = file->lines;

   if (sectionid <= 1U)
   {
      /* Add to start of file */
      if (cur_line != NULL && cur_line->type != FILE_LINE_ACTION)
      {
         /* There's something in the file, find the line before the first
          * action.
          */
         while ( (cur_line->next != NULL)
              && (cur_line->next->type != FILE_LINE_ACTION) )
         {
            cur_line = cur_line->next;
            line_number++;
         }
      }
      else
      {
         /* File starts with action line, so insert at top */
         cur_line = NULL;
      }
   }
   else
   {
      /* Add after stated section. */
      while ((cur_line != NULL) && (line_number < sectionid))
      {
         cur_line = cur_line->next;
         line_number++;
      }

      if ( (cur_line == NULL)
        || (cur_line->type != FILE_LINE_ACTION))
      {
         /* Invalid "sectionid" parameter */
         edit_free_file(file);
         return JB_ERR_CGI_PARAMS;
      }

      /* Skip through the section to find the last line in it. */
      while ( (cur_line->next != NULL)
           && (cur_line->next->type != FILE_LINE_ACTION) )
      {
         cur_line = cur_line->next;
         line_number++;
      }
   }

   /* At this point, the last line in the previous section is in cur_line
    * - add after this.  (Or if we need to add as the first line, cur_line
    * will be NULL).
    */

   new_text = strdup("{}");
   if (NULL == new_text)
   {
      edit_free_file(file);
      return JB_ERR_MEMORY;
   }

   /* Allocate the new line */
   new_line = (struct file_line *)zalloc(sizeof(*new_line));
   if (new_line == NULL)
   {
      free(new_text);
      edit_free_file(file);
      return JB_ERR_MEMORY;
   }

   /* Fill in the data members of the new line */
   new_line->raw = NULL;
   new_line->prefix = NULL;
   new_line->unprocessed = new_text;
   new_line->type = FILE_LINE_ACTION;

   if (cur_line != NULL)
   {
      /* Link new_line into the list, after cur_line */
      new_line->next = cur_line->next;
      cur_line->next = new_line;
   }
   else
   {
      /* Link new_line into the list, as first line */
      new_line->next = file->lines;
      file->lines = new_line;
   }

   /* Done making changes, now commit */

   err = edit_write_file(file);
   if (err)
   {
      /* Error writing file */
      if (err == JB_ERR_FILE)
      {
         /* Read-only file. */
         err = cgi_error_file_read_only(csp, rsp, file->identifier);
      }
      edit_free_file(file);
      return err;
   }

   snprintf(target, 1024, CGI_PREFIX "edit-actions-list?foo=%lu&f=%s",
            (long) time(NULL), file->identifier);

   edit_free_file(file);

   return cgi_redirect(rsp, target);
}


/*********************************************************************
 *
 * Function    :  cgi_edit_actions_section_swap
 *
 * Description :  CGI function that swaps the order of two sections
 *                in the actions file.  Note that this CGI can actually
 *                swap any two arbitrary sections, but the GUI interface
 *                currently only allows consecutive sections to be
 *                specified.
 *
 * Parameters  :
 *          1  :  csp = Current client state (buffers, headers, etc...)
 *          2  :  rsp = http_response data structure for output
 *          3  :  parameters = map of cgi parameters
 *
 * CGI Parameters :
 *           f : (filename) Identifies the file to edit
 *           v : (version) File's last-modified time
 *          s1 : (section1) Line number of first section to swap
 *          s2 : (section2) Line number of second section to swap
 *
 * Returns     :  JB_ERR_OK     on success
 *                JB_ERR_MEMORY on out-of-memory
 *                JB_ERR_CGI_PARAMS if the CGI parameters are not
 *                                  specified or not valid.
 *
 *********************************************************************/
jb_err cgi_edit_actions_section_swap(struct client_state *csp,
                                     struct http_response *rsp,
                                     const struct map *parameters)
{
   unsigned section1;
   unsigned section2;
   struct editable_file * file;
   struct file_line * cur_line;
   struct file_line * prev_line;
   struct file_line * line_before_section1;
   struct file_line * line_start_section1;
   struct file_line * line_end_section1;
   struct file_line * line_after_section1;
   struct file_line * line_before_section2;
   struct file_line * line_start_section2;
   struct file_line * line_end_section2;
   struct file_line * line_after_section2;
   unsigned line_number;
   char target[1024];
   jb_err err;

   if (0 == (csp->config->feature_flags & RUNTIME_FEATURE_CGI_EDIT_ACTIONS))
   {
      return cgi_error_disabled(csp, rsp);
   }

   err = get_number_param(csp, parameters, "s1", &section1);
   if (!err) err = get_number_param(csp, parameters, "s2", &section2);
   if (err)
   {
      return err;
   }

   if (section1 > section2)
   {
      unsigned temp = section2;
      section2 = section1;
      section1 = temp;
   }

   err = edit_read_actions_file(csp, rsp, parameters, 1, &file);
   if (err)
   {
      /* No filename specified, can't read file, modified, or out of memory. */
      return (err == JB_ERR_FILE ? JB_ERR_OK : err);
   }

   /* Start at the beginning... */
   line_number = 1;
   cur_line = file->lines;
   prev_line = NULL;

   /* ... find section1 ... */
   while ((cur_line != NULL) && (line_number < section1))
   {
      prev_line = cur_line;
      cur_line = cur_line->next;
      line_number++;
   }

   if ( (cur_line == NULL)
     || (cur_line->type != FILE_LINE_ACTION) )
   {
      /* Invalid "section1" parameter */
      edit_free_file(file);
      return JB_ERR_CGI_PARAMS;
   }

   /* If no-op, we've validated params and can skip the rest. */
   if (section1 != section2)
   {
      /* ... find the end of section1 ... */
      line_before_section1 = prev_line;
      line_start_section1 = cur_line;
      do
      {
         prev_line = cur_line;
         cur_line = cur_line->next;
         line_number++;
      }
      while ((cur_line != NULL) && (cur_line->type == FILE_LINE_URL));
      line_end_section1 = prev_line;
      line_after_section1 = cur_line;

      /* ... find section2 ... */
      while ((cur_line != NULL) && (line_number < section2))
      {
         prev_line = cur_line;
         cur_line = cur_line->next;
         line_number++;
      }

      if ( (cur_line == NULL)
        || (cur_line->type != FILE_LINE_ACTION) )
      {
         /* Invalid "section2" parameter */
         edit_free_file(file);
         return JB_ERR_CGI_PARAMS;
      }

      /* ... find the end of section2 ... */
      line_before_section2 = prev_line;
      line_start_section2 = cur_line;
      do
      {
         prev_line = cur_line;
         cur_line = cur_line->next;
         line_number++;
      }
      while ((cur_line != NULL) && (cur_line->type == FILE_LINE_URL));
      line_end_section2 = prev_line;
      line_after_section2 = cur_line;

      /* Now have all the pointers we need. Do the swap. */

      /* Change the pointer to section1 to point to section2 instead */
      if (line_before_section1 == NULL)
      {
         file->lines = line_start_section2;
      }
      else
      {
         line_before_section1->next = line_start_section2;
      }

      if (line_before_section2 == line_end_section1)
      {
         /* Consecutive sections */
         line_end_section2->next = line_start_section1;
      }
      else
      {
         line_end_section2->next = line_after_section1;
         line_before_section2->next = line_start_section1;
      }

      /* Set the pointer from the end of section1 to the rest of the file */
      line_end_section1->next = line_after_section2;

      err = edit_write_file(file);
      if (err)
      {
         /* Error writing file */
         if (err == JB_ERR_FILE)
         {
            /* Read-only file. */
            err = cgi_error_file_read_only(csp, rsp, file->identifier);
         }
         edit_free_file(file);
         return err;
      }
   } /* END if (section1 != section2) */

   snprintf(target, 1024, CGI_PREFIX "edit-actions-list?foo=%lu&f=%s",
            (long) time(NULL), file->identifier);

   edit_free_file(file);

   return cgi_redirect(rsp, target);
}


/*********************************************************************
 *
 * Function    :  cgi_toggle
 *
 * Description :  CGI function that adds a new empty section to
 *                an actions file.
 *
 * Parameters  :
 *          1  :  csp = Current client state (buffers, headers, etc...)
 *          2  :  rsp = http_response data structure for output
 *          3  :  parameters = map of cgi parameters
 *
 * CGI Parameters :
 *         set : If present, how to change toggle setting:
 *               "enable", "disable", "toggle", or none (default).
 *        mini : If present, use mini reply template.
 *
 * Returns     :  JB_ERR_OK     on success
 *                JB_ERR_MEMORY on out-of-memory
 *
 *********************************************************************/
jb_err cgi_toggle(struct client_state *csp,
                  struct http_response *rsp,
                  const struct map *parameters)
{
   struct map *exports;
   char mode;
   const char *template_name;

   assert(csp);
   assert(rsp);
   assert(parameters);

   if (0 == (csp->config->feature_flags & RUNTIME_FEATURE_CGI_TOGGLE))
   {
      return cgi_error_disabled(csp, rsp);
   }

   mode = get_char_param(parameters, "set");

   if (mode == 'E')
   {
      /* Enable */
      global_toggle_state = 1;
   }
   else if (mode == 'D')
   {
      /* Disable */
      global_toggle_state = 0;
   }
   else if (mode == 'T')
   {
      /* Toggle */
      global_toggle_state = !global_toggle_state;
   }

   if (NULL == (exports = default_exports(csp, "toggle")))
   {
      return JB_ERR_MEMORY;
   }

   template_name = (get_char_param(parameters, "mini")
                 ? "toggle-mini"
                 : "toggle");

   return template_fill_for_cgi(csp, template_name, exports, rsp);
}


/*********************************************************************
 *
 * Function    :  javascriptify
 *
 * Description :  Converts a string into a form JavaScript will like.
 *
 *                Netscape 4's JavaScript sucks - it doesn't use 
 *                "id" parameters, so you have to set the "name"
 *                used to submit a form element to something JavaScript
 *                will like.  (Or access the elements by index in an
 *                array.  That array contains >60 elements and will
 *                be changed whenever we add a new action to the
 *                editor, so I'm NOT going to use indexes that have
 *                to be figured out by hand.)
 *
 *                Currently the only thing we have to worry about
 *                is "-" ==> "_" conversion.
 *
 *                This is a length-preserving operation so it is
 *                carried out in-place, no memory is allocated
 *                or freed.
 *
 * Parameters  :
 *          1  :  identifier = String to make JavaScript-friendly.
 *
 * Returns     :  N/A
 *
 *********************************************************************/
static void javascriptify(char * identifier)
{
   char * p = identifier;
   while (NULL != (p = strchr(p, '-')))
   {
      *p++ = '_';
   }
}


/*********************************************************************
 *
 * Function    :  actions_to_radio
 *
 * Description :  Converts a actionsfile entry into settings for
 *                radio buttons and edit boxes on a HTML form.
 *
 * Parameters  :
 *          1  :  exports = List of substitutions to add to.
 *          2  :  action  = Action to read
 *
 * Returns     :  JB_ERR_OK     on success
 *                JB_ERR_MEMORY on out-of-memory
 *
 *********************************************************************/
static jb_err actions_to_radio(struct map * exports,
                               const struct action_spec *action)
{
   unsigned mask = action->mask;
   unsigned add  = action->add;
   int mapped_param;
   int checked;
   char current_mode;

   assert(exports);
   assert(action);

   mask = action->mask;
   add  = action->add;

   /* sanity - prevents "-feature +feature" */
   mask |= add;


#define DEFINE_ACTION_BOOL(name, bit)                 \
   if (!(mask & bit))                                 \
   {                                                  \
      current_mode = 'n';                             \
   }                                                  \
   else if (add & bit)                                \
   {                                                  \
      current_mode = 'y';                             \
   }                                                  \
   else                                               \
   {                                                  \
      current_mode = 'x';                             \
   }                                                  \
   if (map_radio(exports, name, "ynx", current_mode)) \
   {                                                  \
      return JB_ERR_MEMORY;                           \
   }

#define DEFINE_ACTION_STRING(name, bit, index)        \
   DEFINE_ACTION_BOOL(name, bit);                     \
   mapped_param = 0;

#define DEFINE_CGI_PARAM_RADIO(name, bit, index, value, is_default)  \
   if (add & bit)                                                    \
   {                                                                 \
      checked = !strcmp(action->string[index], value);               \
   }                                                                 \
   else                                                              \
   {                                                                 \
      checked = is_default;                                          \
   }                                                                 \
   mapped_param |= checked;                                          \
   if (map(exports, name "-param-" value, 1, (checked ? "checked" : ""), 1)) \
   {                                                                 \
      return JB_ERR_MEMORY;                                          \
   }

#define DEFINE_CGI_PARAM_CUSTOM(name, bit, index, default_val)       \
   if (map(exports, name "-param-custom", 1,                         \
           ((!mapped_param) ? "checked" : ""), 1))                   \
   {                                                                 \
      return JB_ERR_MEMORY;                                          \
   }                                                                 \
   if (map(exports, name "-param", 1,                                \
           (((add & bit) && !mapped_param) ?                         \
           action->string[index] : default_val), 1))                 \
   {                                                                 \
      return JB_ERR_MEMORY;                                          \
   }

#define DEFINE_CGI_PARAM_NO_RADIO(name, bit, index, default_val)     \
   if (map(exports, name "-param", 1,                                \
           ((add & bit) ? action->string[index] : default_val), 1))  \
   {                                                                 \
      return JB_ERR_MEMORY;                                          \
   }

#define DEFINE_ACTION_MULTI(name, index)              \
   if (action->multi_add[index]->first)               \
   {                                                  \
      current_mode = 'y';                             \
   }                                                  \
   else if (action->multi_remove_all[index])          \
   {                                                  \
      current_mode = 'n';                             \
   }                                                  \
   else if (action->multi_remove[index]->first)       \
   {                                                  \
      current_mode = 'y';                             \
   }                                                  \
   else                                               \
   {                                                  \
      current_mode = 'x';                             \
   }                                                  \
   if (map_radio(exports, name, "ynx", current_mode)) \
   {                                                  \
      return JB_ERR_MEMORY;                           \
   }

#define DEFINE_ACTION_ALIAS 0 /* No aliases for output */

#include "actionlist.h"

#undef DEFINE_ACTION_MULTI
#undef DEFINE_ACTION_STRING
#undef DEFINE_ACTION_BOOL
#undef DEFINE_ACTION_ALIAS
#undef DEFINE_CGI_PARAM_CUSTOM
#undef DEFINE_CGI_PARAM_RADIO
#undef DEFINE_CGI_PARAM_NO_RADIO

   return JB_ERR_OK;
}


/*********************************************************************
 *
 * Function    :  actions_from_radio
 *
 * Description :  Converts a map of parameters passed to a CGI function
 *                into an actionsfile entry.
 *
 * Parameters  :
 *          1  :  parameters = parameters to the CGI call
 *          2  :  action  = Action to change.  Must be valid before
 *                          the call, actions not specified will be
 *                          left unchanged.
 *
 * Returns     :  JB_ERR_OK     on success
 *                JB_ERR_MEMORY on out-of-memory
 *
 *********************************************************************/
static jb_err actions_from_radio(const struct map * parameters,
                                 struct action_spec *action)
{
   const char * param;
   char * param_dup;
   char ch;
   const char * js_name;
   jb_err err = JB_ERR_OK;

   assert(parameters);
   assert(action);

   /* Statics are generally a potential race condition,
    * but in this case we're safe and don't need semaphores.
    * Be careful if you modify this function.
    * - Jon
    * The js_name_arr's are never free()d, but this is no
    * problem, since they will only be created once and
    * used by all threads thereafter. -oes
    */

#define JAVASCRIPTIFY(dest_var, string)               \
   {                                                  \
     static int first_time = 1;                       \
     static char *js_name_arr;                        \
      if (first_time)                                 \
      {                                               \
         js_name_arr = strdup(string);                \
         javascriptify(js_name_arr);                  \
      }                                               \
      dest_var = js_name_arr;                         \
      first_time = 0;                                 \
   }                                                  \

#define DEFINE_ACTION_BOOL(name, bit)                 \
   JAVASCRIPTIFY(js_name, name);                      \
   ch = get_char_param(parameters, js_name);          \
   if (ch == 'Y')                                     \
   {                                                  \
      action->add  |= bit;                            \
      action->mask |= bit;                            \
   }                                                  \
   else if (ch == 'N')                                \
   {                                                  \
      action->add  &= ~bit;                           \
      action->mask &= ~bit;                           \
   }                                                  \
   else if (ch == 'X')                                \
   {                                                  \
      action->add  &= ~bit;                           \
      action->mask |= bit;                            \
   }                                                  \

#define DEFINE_ACTION_STRING(name, bit, index)                 \
   JAVASCRIPTIFY(js_name, name);                               \
   ch = get_char_param(parameters, js_name);                   \
   if (ch == 'Y')                                              \
   {                                                           \
      param = NULL;                                            \
      JAVASCRIPTIFY(js_name, name "-mode");                    \
      if (!err) err = get_string_param(parameters, js_name, &param);    \
      if ((param == NULL) || (0 == strcmp(param, "CUSTOM")))            \
      {                                                                 \
         JAVASCRIPTIFY(js_name, name "-param");                         \
         if (!err) err = get_string_param(parameters, js_name, &param); \
      }                                                        \
      if (param != NULL)                                       \
      {                                                        \
         if (NULL == (param_dup = strdup(param)))              \
         {                                                     \
            return JB_ERR_MEMORY;                              \
         }                                                     \
         freez(action->string[index]);                         \
         action->add  |= bit;                                  \
         action->mask |= bit;                                  \
         action->string[index] = param_dup;                    \
      }                                                        \
   }                                                           \
   else if (ch == 'N')                                         \
   {                                                           \
      if (action->add & bit)                                   \
      {                                                        \
         freez(action->string[index]);                         \
      }                                                        \
      action->add  &= ~bit;                                    \
      action->mask &= ~bit;                                    \
   }                                                           \
   else if (ch == 'X')                                         \
   {                                                           \
      if (action->add & bit)                                   \
      {                                                        \
         freez(action->string[index]);                         \
      }                                                        \
      action->add  &= ~bit;                                    \
      action->mask |= bit;                                     \
   }                                                           \

#define DEFINE_ACTION_MULTI(name, index)                       \
   JAVASCRIPTIFY(js_name, name);                               \
   ch = get_char_param(parameters, js_name);                   \
   if (ch == 'Y')                                              \
   {                                                           \
      /* FIXME */                                              \
   }                                                           \
   else if (ch == 'N')                                         \
   {                                                           \
      list_remove_all(action->multi_add[index]);               \
      list_remove_all(action->multi_remove[index]);            \
      action->multi_remove_all[index] = 1;                     \
   }                                                           \
   else if (ch == 'X')                                         \
   {                                                           \
      list_remove_all(action->multi_add[index]);               \
      list_remove_all(action->multi_remove[index]);            \
      action->multi_remove_all[index] = 0;                     \
   }                                                           \

#define DEFINE_ACTION_ALIAS 0 /* No aliases for URL parsing */

#include "actionlist.h"

#undef DEFINE_ACTION_MULTI
#undef DEFINE_ACTION_STRING
#undef DEFINE_ACTION_BOOL
#undef DEFINE_ACTION_ALIAS
#undef JAVASCRIPTIFY

   return err;
}


#endif /* def FEATURE_CGI_EDIT_ACTIONS */


/*
  Local Variables:
  tab-width: 3
  end:
*/
