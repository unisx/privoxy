#ifndef KILLPOPUP_H_INCLUDED
#define KILLPOPUP_H_INCLUDED
#define KILLPOPUP_H_VERSION "$Id: killpopup.h,v 1.8 2002/03/26 22:29:55 swa Exp $"
/*********************************************************************
 *
 * File        :  $Source: /cvsroot/ijbswa/current/Attic/killpopup.h,v $
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
 *    $Log: killpopup.h,v $
 *    Revision 1.8  2002/03/26 22:29:55  swa
 *    we have a new homepage!
 *
 *    Revision 1.7  2002/03/24 13:25:43  swa
 *    name change related issues
 *
 *    Revision 1.6  2001/10/07 15:42:41  oes
 *    filter_popups now gets a csp pointer so it can raise the new
 *      CSP_FLAG_MODIFIED flag.
 *
 *    Revision 1.5  2001/07/31 14:44:22  oes
 *    Deleted unused size parameter from filter_popups()
 *
 *    Revision 1.4  2001/07/30 22:08:36  jongfoster
 *    Tidying up #defines:
 *    - All feature #defines are now of the form FEATURE_xxx
 *    - Permanently turned off WIN_GUI_EDIT
 *    - Permanently turned on WEBDAV and SPLIT_PROXY_ARGS
 *
 *    Revision 1.3  2001/07/29 18:59:21  jongfoster
 *    - Changing #define _KILLPOPUP_H to KILLPOPUP_H_INCLUDED
 *    - Adding extern "C" {}
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


#include "project.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef FEATURE_KILL_POPUPS

extern void filter_popups(char *buff, struct client_state *csp);

#endif /* def FEATURE_KILL_POPUPS */

/* Revision control strings from this header and associated .c file */
extern const char killpopup_rcs[];
extern const char killpopup_h_rcs[];

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* ndef KILLPOPUP_H_INCLUDED */

/*
  Local Variables:
  tab-width: 3
  end:
*/
