/* FriBidi
 * fribidi-env.h - state manipulation routines
 *
 * $Id: fribidi-env.h,v 1.1 2004-04-25 18:47:57 behdad Exp $
 * $Author: behdad $
 * $Date: 2004-04-25 18:47:57 $
 * $Revision: 1.1 $
 * $Source: /home/behdad/src/fdo/fribidi/togit/git/../fribidi/fribidi2/lib/Attic/fribidi-env.h,v $
 *
 * Author:
 *   Behdad Esfahbod, 2001, 2002, 2004
 *   Dov Grobgeld, 1999, 2000
 *
 * Copyright (C) 2004 Sharif FarsiWeb, Inc
 * Copyright (C) 2001,2002 Behdad Esfahbod
 * Copyright (C) 1999,2000 Dov Grobgeld
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
#ifndef _FRIBIDI_ENV_H
#define _FRIBIDI_ENV_H

#include "fribidi-common.h"

#include "fribidi-types.h"

#include "fribidi-begindecls.h"


#define fribidi_debug_status FRIBIDI_NAMESPACE(debug_status)
/* fribidi_debug_status - get current debugging output status */
FRIBIDI_ENTRY fribidi_boolean fribidi_debug_status (
  void
);

#define fribidi_set_debug FRIBIDI_NAMESPACE(set_debug)
/* fribidi_set_debug - set debugging output on or off
 *
 * This function is used to turn debugging output on or off.  To
 * turn it on, the library should have been compiled with debugging
 * enabled, or it will fail.  The debugging output goes to the standard
 * error output.
 *
 * This flag is off by default.
 *
 * Returns: the new debugging output status.
 */
FRIBIDI_ENTRY fribidi_boolean fribidi_set_debug (
  fribidi_boolean state		/* new state to set */
);


#define fribidi_mirroring_status FRIBIDI_NAMESPACE(mirroring_status)
/* fribidi_mirroring_status - get current mirroring status */
FRIBIDI_ENTRY fribidi_boolean fribidi_mirroring_status (
  void
);

#define fribidi_set_mirroring FRIBIDI_NAMESPACE(set_mirroring)
/* fribidi_set_mirroring - set mirroring on or off
 *
 * This function is used to turn character mirroring on or off.
 * Character mirroring is the act of replacing a mirrorable glyph
 * (character), eg. left paranthesis, with the matching glyph, 
 * eg. right paranthesis, in a right-to-left resolved context.
 * If your rendering engine does mirroring itself, you may want to 
 * turn it off here.
 *
 * This flag is on by default.
 *
 * Returns: the new mirroring status.
 */
FRIBIDI_ENTRY fribidi_boolean fribidi_set_mirroring (
  fribidi_boolean state		/* new state to set */
);


#define fribidi_reorder_nsm_status FRIBIDI_NAMESPACE(reorder_nsm_status)
/* fribidi_reorder_nsm_status - get current marks reordering status */
FRIBIDI_ENTRY fribidi_boolean fribidi_reorder_nsm_status (
  void
);

#define fribidi_set_reorder_nsm FRIBIDI_NAMESPACE(set_reorder_nsm)
/* fribidi_set_reorder_nsm - set marks reordering on or off
 *
 * This function is used to turn non-spacing marks reordering on or
 * off.  Reordering non-spacing marks is the act of placing non-spacing
 * marks (bidi class NSM) after their base character in a right-to-left
 * resolved context.  If your rendering engine expects non-spacing marks
 * always after the base character in the memory representation of the
 * visual string, you need this option on.  An example of where people
 * may need it off is when rendering in the console when non-spacing
 * marks cannot be applied on top of the base character.
 *
 * This flag is on by default.
 *
 * Returns: the new marks reordering status.
 */
FRIBIDI_ENTRY fribidi_boolean fribidi_set_reorder_nsm (
  fribidi_boolean state		/* new state to set */
);


#include "fribidi-enddecls.h"

#endif /* !_FRIBIDI_ENV_H */
/* Editor directions:
 * vim:textwidth=78:tabstop=8:shiftwidth=2:autoindent:cindent
 */
