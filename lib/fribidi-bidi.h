/* FriBidi
 * fribidi-bidi.h - bidirectional algorithm
 *
 * $Id: fribidi-bidi.h,v 1.1 2004-04-25 18:47:57 behdad Exp $
 * $Author: behdad $
 * $Date: 2004-04-25 18:47:57 $
 * $Revision: 1.1 $
 * $Source: /home/behdad/src/fdo/fribidi/togit/git/../fribidi/fribidi2/lib/fribidi-bidi.h,v $
 *
 * Authors:
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
#ifndef _FRIBIDI_BIDI_H
#define _FRIBIDI_BIDI_H

#include "fribidi-common.h"

#include "fribidi-types.h"
#include "fribidi-bidi-types.h"

#include "fribidi-begindecls.h"

#define fribidi_log2vis FRIBIDI_NAMESPACE(log2vis)
/* fribidi_log2vis - get visual string
 *
 * This function converts the logical input string to the visual output
 * strings as specified by the Unicode Bidirectional Algorithm.  As a side
 * effect it also generates mapping lists between the two strings, and the
 * list of embedding levels as defined by the algorithm.  If any of the the
 * lists are passed as NULL, the list is ignored and not filled.
 *
 * Returns: Non-zero if it was successful, or zero if any error occured.
 */
FRIBIDI_ENTRY fribidi_boolean fribidi_log2vis (
  const FriBidiChar *str,	/* input logical string */
  FriBidiStrIndex len,		/* input string length */
  FriBidiCharType *pbase_dir,	/* requested and resolved paragraph
				 * base direction */
  FriBidiChar *visual_str,	/* output visual string */
  FriBidiStrIndex *position_L_to_V_list,	/* output mapping from logical to 
						 * visual string positions */
  FriBidiStrIndex *position_V_to_L_list,	/* output mapping from visual string
						 * back to the logical string
						 * positions */
  FriBidiLevel *embedding_level_list	/* output list of embedding levels */
);

#define fribidi_log2vis_get_embedding_levels FRIBIDI_NAMESPACE(log2vis_get_embedding_levels)
/* fribidi_log2vis_get_embedding_levels - get bidi embedding levels
 *
 * This function finds the bidi embedding levels of a single paragraph,
 * as defined by the Unicode Bidirectional Algorithm.
 *
 * Returns: Non-zero if it was successful, or zero if any error occured.
 */
FRIBIDI_ENTRY fribidi_boolean fribidi_log2vis_get_embedding_levels (
  const FriBidiChar *str,	/* input logical string */
  FriBidiStrIndex len,		/* input string length */
  FriBidiCharType *pbase_dir,	/* requested and resolved paragraph
				 * base direction */
  FriBidiLevel *embedding_level_list	/* output list of embedding levels */
);

#define fribidi_remove_bidi_marks FRIBIDI_NAMESPACE(remove_bidi_marks)
/* fribidi_remove_bidi_marks - remove bidi marks out an string
 *
 * This function removes the bidi marks out of an string and the
 * accompanying lists.  If any of the input lists are NULL, the list is
 * skipped.
 *
 * Bugs: It is not clear that if the input string is a logical, or visual
 * string.  It is know to have problems handling position lists.  In short,
 * it should be avoided.
 *
 * Returns: New length of the string.
 */
FRIBIDI_ENTRY FriBidiStrIndex fribidi_remove_bidi_marks (
  FriBidiChar *str,
  FriBidiStrIndex length,
  FriBidiStrIndex *position_to_this_list,
  FriBidiStrIndex *position_from_this_list,
  FriBidiLevel *embedding_level_list
);

#include "fribidi-enddecls.h"

#endif /* !_FRIBIDI_BIDI_H */
/* Editor directions:
 * vim:textwidth=78:tabstop=8:shiftwidth=2:autoindent:cindent
 */
