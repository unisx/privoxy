const char killpopup_rcs[] = "$Id: killpopup.c,v 1.16 2002/03/26 22:29:55 swa Exp $";
/*********************************************************************
 *
 * File        :  $Source: /cvsroot/ijbswa/current/Attic/killpopup.c,v $
 *
 * Purpose     :  Handles the filtering of popups.
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
 *    $Log: killpopup.c,v $
 *    Revision 1.16  2002/03/26 22:29:55  swa
 *    we have a new homepage!
 *
 *    Revision 1.15  2002/03/24 13:25:43  swa
 *    name change related issues
 *
 *    Revision 1.14  2002/03/07 03:46:53  oes
 *    Fixed compiler warnings etc
 *
 *    Revision 1.13  2001/11/13 00:16:40  jongfoster
 *    Replacing references to malloc.h with the standard stdlib.h
 *    (See ANSI or K&R 2nd Ed)
 *
 *    Revision 1.12  2001/10/25 03:40:48  david__schmidt
 *    Change in porting tactics: OS/2's EMX porting layer doesn't allow multiple
 *    threads to call select() simultaneously.  So, it's time to do a real, live,
 *    native OS/2 port.  See defines for __EMX__ (the porting layer) vs. __OS2__
 *    (native). Both versions will work, but using __OS2__ offers multi-threading.
 *
 *    Revision 1.11  2001/10/07 15:42:41  oes
 *    filter_popups now gets a csp pointer so it can raise the new
 *      CSP_FLAG_MODIFIED flag.
 *
 *    Revision 1.10  2001/09/22 16:34:44  jongfoster
 *    Removing unneeded #includes
 *
 *    Revision 1.9  2001/07/31 14:44:22  oes
 *    Deleted unused size parameter from filter_popups()
 *
 *    Revision 1.8  2001/07/30 22:08:36  jongfoster
 *    Tidying up #defines:
 *    - All feature #defines are now of the form FEATURE_xxx
 *    - Permanently turned off WIN_GUI_EDIT
 *    - Permanently turned on WEBDAV and SPLIT_PROXY_ARGS
 *
 *    Revision 1.7  2001/07/20 19:29:25  haroon
 *    - In v1.5 forgot to add that I implemented LOG_LEVEL_POPUPS in errlog.c,
 *      errlog.h and killpopup.c. In that case, it is superfluous to have define for
 *      POPUP_VERBOSE, so I removed the defines and logging is now done
 *      via log_error(LOG_LEVEL_POPUPS, ....)
 *
 *    Revision 1.6  2001/07/19 19:11:35  haroon
 *    - Implemented Guy's idea of replacing window.open( with 1;''.concat(
 *    - Implemented Guy's idea of replacing .resizeTo( with .scrollTo(
 *
 *    Revision 1.5  2001/07/18 15:02:52  haroon
 *    improved nuking of window.open
 *
 *    Revision 1.4  2001/06/29 13:29:55  oes
 *    Added FIXMEs (and didn't repair, hehe)
 *
 *    Revision 1.3  2001/05/22 18:56:28  oes
 *    CRLF -> LF
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
 *    Revision 1.1.1.1  2001/05/15 13:58:58  oes
 *    Initial import of version 2.9.3 source tree
 *
 *
 *********************************************************************/


#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <ctype.h>

#if !defined(_WIN32) && !defined(__OS2__)
#include <unistd.h>
#endif

#include "project.h"
#include "killpopup.h"
#include "errlog.h"

const char killpopup_h_rcs[] = KILLPOPUP_H_VERSION;

#ifdef FEATURE_KILL_POPUPS

/*********************************************************************
 *
 * Function    :  filter_popups
 *
 * Description :  Filter the block of data that's been read from the server
 *                for javascript popup code and replace by syntactically
 *                neutral code of the same size.
 *                Raise the CSP_FLAG_MODIFIED flag on success.
 *
 * Parameters  :
 *          1  :  buff = Buffer to scan and modify.  Null terminated.
 *          2  :  csp = Client state pointer
 *
 * Returns     :  void
 *
 *********************************************************************/
void filter_popups(char *buff, struct client_state *csp)
{
   char *start_p = NULL;
   char *close_p = NULL;
   char *p     = NULL;

   /*
    * replace the window.open( with a harmless JavaScript replacement
    * (notice the two single quotes)
    */
   while ((start_p = strstr(buff, "window.open(")) != NULL)
   {
      if (start_p)
      {
         strncpy(start_p, "1;''.concat(", 12);
         log_error(LOG_LEVEL_POPUPS, "Blocked popup window open");
         csp->flags |= CSP_FLAG_MODIFIED;
      }
   }

   /*
    * replace the .resizeTo( with a harmless JavaScript replacement
    */
   while ((start_p = strstr(buff, ".resizeTo(")) != NULL)
   {
      if (start_p)
      {
         strncpy(start_p, ".scrollTo(", 10);
         log_error(LOG_LEVEL_POPUPS, "Blocked popup window resize");
         csp->flags |= CSP_FLAG_MODIFIED;
      }
   }

   /* 
    * Filter onUnload and onExit
    */
   start_p = strstr(buff, "<body");
   if (!start_p) start_p = strstr(buff, "<BODY");
   if (!start_p) start_p = strstr(buff, "<Body");
   if (!start_p) start_p = strstr(buff, "<BOdy");
   if (start_p)
   {
      close_p = strchr(start_p, '>');
      if (close_p)
      {
         /* we are now between <body and the ending > */
         p = strstr(start_p, "onUnload");
         if (p)
         {
            strncpy(p, "_nU_", 4);
            csp->flags |= CSP_FLAG_MODIFIED;
         }
         p = strstr(start_p, "onExit");
         if (p)
         {
            strncpy(p, "_nE_", 4);
            csp->flags |= CSP_FLAG_MODIFIED;
         }
      }
   }

}

#endif /* def FEATURE_KILL_POPUPS */

/*
  Local Variables:
  tab-width: 3
  end:
*/
