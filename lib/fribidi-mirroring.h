/* FriBidi
 * fribidi-mirroring.h - get mirrored character
 *
 * $Id: fribidi-mirroring.h,v 1.1.1.1 2004-04-25 18:47:57 behdad Exp $
 * $Author: behdad $
 * $Date: 2004-04-25 18:47:57 $
 * $Revision: 1.1.1.1 $
 * $Source: /home/behdad/src/fdo/fribidi/togit/git/../fribidi/fribidi2/lib/fribidi-mirroring.h,v $
 *
 * Author:
 *   Behdad Esfahbod, 2001, 2002, 2004
 *
 * Copyright (C) 2004 Sharif FarsiWeb, Inc
 * Copyright (C) 2001,2002 Behdad Esfahbod
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library, in a file named COPYING; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307, USA
 * 
 * For licensing issues, contact <license@farsiweb.info>.
 */
#ifndef _FRIBIDI_MIRRORING_H
#define _FRIBIDI_MIRRORING_H

#include "fribidi-common.h"

#include "fribidi-types.h"

#include "fribidi-begindecls.h"

#define fribidi_get_mirror_char FRIBIDI_NAMESPACE(get_mirror_char)
/* fribidi_get_mirror_char - get mirrored character
 *
 * This function finds the mirrored of a character.  If input character is a
 * mirroring character, the matching mirrored character is put in the output,
 * otherwise the input character itself is put.
 *
 * Returns: if the character has a mirror or not.
 */
FRIBIDI_ENTRY fribidi_boolean fribidi_get_mirror_char (
  FriBidiChar ch,		/* input character */
  FriBidiChar *mirrored_ch	/* output mirrored character */
);

#include "fribidi-enddecls.h"

#endif /* !_FRIBIDI_MIRRORING_H */
/* Editor directions:
 * Local Variables:
 *   mode:c
 *   c-basic-offset: 2
 *   indent-tabs-mode:t
 *   tab-width: 8
 * End:
 * vim: textwidth=78: autoindent: cindent: shiftwidth=2: tabstop=8
 */
