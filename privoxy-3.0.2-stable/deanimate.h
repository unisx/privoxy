#ifndef DEANIMATE_H_INCLUDED
#define DEANIMATE_H_INCLUDED
#define DEANIMATE_H_VERSION "$Id: deanimate.h,v 1.8 2002/03/26 22:29:54 swa Exp $"
/*********************************************************************
 *
 * File        :  $Source: /cvsroot/ijbswa/current/Attic/deanimate.h,v $
 *
 * Purpose     :  Declares functions to deanimate GIF images on the fly.
 *                
 *                Functions declared include: gif_deanimate, buf_free
 *                
 *
 * Copyright   :  Written by and Copyright (C) 2001 Andreas S. Oesterhelt
 *                for the Privoxy team. http://www.privoxy.org/
 *
 *                Based on ideas from the Image::DeAnim Perl module by
 *                Ken MacFarlane, <ksm+cpan@universal.dca.net>
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
 *    $Log: deanimate.h,v $
 *    Revision 1.8  2002/03/26 22:29:54  swa
 *    we have a new homepage!
 *
 *    Revision 1.7  2002/03/24 13:25:43  swa
 *    name change related issues
 *
 *    Revision 1.6  2002/03/08 17:46:04  jongfoster
 *    Fixing int/size_t warnings
 *
 *    Revision 1.5  2002/03/07 03:46:17  oes
 *    Fixed compiler warnings
 *
 *    Revision 1.4  2001/07/29 18:50:04  jongfoster
 *    Fixing "extern C" block, and renaming #define _DEANIMATE_H
 *
 *    Revision 1.3  2001/07/18 12:29:05  oes
 *    Updated prototype for gif_deanimate
 *
 *    Revision 1.2  2001/07/13 13:46:20  oes
 *    Introduced GIF deanimation feature
 *
 *
 *********************************************************************/


#ifdef __cplusplus
extern "C" {
#endif

/*
 * A struct that holds a buffer, a read/write offset,
 * and the buffer's capacity.
 */
struct binbuffer
{
   char *buffer; 
   size_t offset;   
   size_t size;    
};

/*
 * Function prototypes
 */
extern int gif_deanimate(struct binbuffer *src, struct binbuffer *dst, int get_first_image);
extern void buf_free(struct binbuffer *buf);

/* 
 * Revision control strings from this header and associated .c file
 */
extern const char deanimate_rcs[];
extern const char deanimate_h_rcs[];

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* ndef DEANIMATE_H_INCLUDED */

/*
  Local Variables:
  tab-width: 3
  end:
*/
