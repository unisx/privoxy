#ifndef W32RES_H_INCLUDED
#define W32RES_H_INCLUDED
#define W32RES_H_VERSION "$Id: w32res.h,v 1.15 2006/07/18 14:48:48 david__schmidt Exp $"
/*********************************************************************
 *
 * File        :  $Source: /cvsroot/ijbswa/current/w32res.h,v $
 *
 * Purpose     :  Identifiers for Windows GUI resources.
 *
 * Copyright   :  Written by and Copyright (C) 2001-2002 members of
 *                the Privoxy team.  http://www.privoxy.org/
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
 *    $Log: w32res.h,v $
 *    Revision 1.15  2006/07/18 14:48:48  david__schmidt
 *    Reorganizing the repository: swapping out what was HEAD (the old 3.1 branch)
 *    with what was really the latest development (the v_3_0_branch branch)
 *
 *    Revision 1.13.2.1  2002/08/21 17:59:06  oes
 *     - "Show Privoxy Window" now a toggle
 *     - Temp kludge to let user and default action file be edited through win32 GUI (FR 592080)
 *
 *    Revision 1.13  2002/03/26 22:57:10  jongfoster
 *    Web server name should begin www.
 *
 *    Revision 1.12  2002/03/24 12:07:36  jongfoster
 *    Consistern name for filters file
 *
 *    Revision 1.11  2002/03/24 12:03:47  jongfoster
 *    Name change
 *
 *    Revision 1.10  2001/07/30 22:08:36  jongfoster
 *    Tidying up #defines:
 *    - All feature #defines are now of the form FEATURE_xxx
 *    - Permanently turned off WIN_GUI_EDIT
 *    - Permanently turned on WEBDAV and SPLIT_PROXY_ARGS
 *
 *    Revision 1.9  2001/07/29 18:43:08  jongfoster
 *    Changing #ifdef _FILENAME_H to FILENAME_H_INCLUDED, to conform to
 *    ANSI C rules.
 *
 *    Revision 1.8  2001/07/13 14:04:59  oes
 *    Removed all #ifdef PCRS
 *
 *    Revision 1.7  2001/06/07 23:08:12  jongfoster
 *    Forward and ACL edit options removed.
 *
 *    Revision 1.6  2001/05/31 21:37:11  jongfoster
 *    GUI changes to rename "permissions file" to "actions file".
 *
 *    Revision 1.5  2001/05/29 09:50:24  jongfoster
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
 *    Revision 1.4  2001/05/26 01:26:34  jongfoster
 *    New #define, WIN_GUI_EDIT, enables the (embryonic) Win32 GUI editor.
 *    This #define cannot be set from ./configure - there's no point, it
 *    doesn't work yet.  See feature request # 425722
 *
 *    Revision 1.3  2001/05/26 00:28:36  jongfoster
 *    Automatic reloading of config file.
 *    Removed obsolete SIGHUP support (Unix) and Reload menu option (Win32).
 *    Most of the global variables have been moved to a new
 *    struct configuration_spec, accessed through csp->config->globalname
 *    Most of the globals remaining are used by the Win32 GUI.
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
 *    Revision 1.1.1.1  2001/05/15 13:59:08  oes
 *    Initial import of version 2.9.3 source tree
 *
 *
 *********************************************************************/

#define IDR_TRAYMENU                      101
#define IDI_IDLE                          102
#define IDR_LOGVIEW                       103
#define IDR_ACCELERATOR                   104
#define IDR_POPUP_SELECTION               105


#define IDI_MAINICON                      200
#define IDI_ANIMATED1                     201
#define IDI_ANIMATED2                     202
#define IDI_ANIMATED3                     203
#define IDI_ANIMATED4                     204
#define IDI_ANIMATED5                     205
#define IDI_ANIMATED6                     206
#define IDI_ANIMATED7                     207
#define IDI_ANIMATED8                     208

#define ID_TOGGLE_SHOWWINDOW              4000
#define ID_HELP_ABOUT                     4001
#define ID_FILE_EXIT                      4002
#define ID_VIEW_CLEARLOG                  4003
#define ID_VIEW_LOGMESSAGES               4004
#define ID_VIEW_MESSAGEHIGHLIGHTING       4005
#define ID_VIEW_LIMITBUFFERSIZE           4006
#define ID_VIEW_ACTIVITYANIMATION         4007
#define ID_HELP_FAQ                       4008
#define ID_HELP_MANUAL                    4009
#define ID_HELP_GPL                       4010
#define ID_HELP_STATUS                    4011
#ifdef FEATURE_TOGGLE
#define ID_TOGGLE_ENABLED                 4012
#endif /* def FEATURE_TOGGLE */

/* Break these out so they are easier to extend, but keep consecutive */
#define ID_TOOLS_EDITCONFIG               5000
#define ID_TOOLS_EDITDEFAULTACTIONS       5001
#define ID_TOOLS_EDITUSERACTIONS          5002
#define ID_TOOLS_EDITFILTERS              5003

#ifdef FEATURE_TRUST
#define ID_TOOLS_EDITTRUST                5004
#endif /* def FEATURE_TRUST */

/*
 * The following symbols are declared in <afxres.h> in VC++.
 * However, mingw32 doesn't have that header.  Let's 
 * always declare them here, for consistency.
 * These are the VC++ values.
 */
#define IDC_STATIC      (-1)
#define ID_EDIT_COPY  30000


#endif /* ndef W32RES_H_INCLUDED */

/*
  Local Variables:
  tab-width: 3
  end:
*/
