/* FriBidi
 * fribidi-bidi.h - bidirectional algorithm
 *
 * $Id: fribidi-bidi.h,v 1.12 2004-06-21 16:15:27 behdad Exp $
 * $Author: behdad $
 * $Date: 2004-06-21 16:15:27 $
 * $Revision: 1.12 $
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

#define fribidi_get_par_embedding_levels FRIBIDI_NAMESPACE(get_par_embedding_levels)
/* fribidi_get_par_direction - get base paragraph direction
 *
 * This function finds the base direction of a single paragraph,
 * as defined by rule P2 of the Unicode Bidirectional Algorithm available at
 * http://www.unicode.org/reports/tr9/#P2.
 *
 * You can provide either the string, or the bidi types; or both.
 * If bidi_types are provided, they are used as the bidi types of characters
 * in the string, otherwise the types are computed from the characters in str.
 * Providing bidi types if available at your side, saves you a few cycles.
 * Bidi types can be obtained by calling fribidi_get_bidi_types().
 *
 * You typically do not need this function as
 * fribidi_get_par_embedding_levels() knows how to compute base direction
 * itself, but you may need this to implement a more sophisticated paragraph
 * direction handling.  Note that you can pass more than a paragraph to this
 * function and the direction of the first non-neutral paragraph is returned,
 * which is a very good heuristic to set direction of the neutral paragraphs
 * at the beginning of text.  For other neutral paragraphs, better you use the
 * direction of the previous paragraph.  This is today known as the best
 * auto-paragraph-direction-detection scheme!
 *
 * Returns: Base pargraph direction.  No weak paragraph direction is returned,
 * only LTR, RTL, or ON.
 */
FRIBIDI_ENTRY FriBidiParType fribidi_get_par_direction (
  const FriBidiChar *str,	/* input paragraph string */
  const FriBidiStrIndex len,	/* input string length */
  const FriBidiCharType *bidi_types	/* input bidi types */
);

#define fribidi_get_par_embedding_levels FRIBIDI_NAMESPACE(get_par_embedding_levels)
/* fribidi_get_par_embedding_levels - get bidi embedding levels of a paragraph
 *
 * This function finds the bidi embedding levels of a single paragraph,
 * as defined by the Unicode Bidirectional Algorithm available at
 * http://www.unicode.org/reports/tr9/.  This function implements rules P2 to
 * I1 inclusive, and parts 1 to 3 of L1, except for rule X9 which is
 *  implemented in fribidi_remove_bidi_marks().  Part 4 of L1 is implemented
 *  in fribidi_reorder_line().
 *
 * You can provide either the string, or the bidi types; or both.
 * If bidi_types are provided, they are used as the bidi types of characters
 * in the string, otherwise the types are computed from the characters in str.
 * Providing bidi types if available at your side, saves you a few cycles.
 * Bidi types can be obtained by calling fribidi_get_bidi_types().
 *
 * There are a few macros defined in fribidi-bidi-types.h to work with this
 * embedding levels.
 *
 * Returns: Maximum level found plus one, or zero if any error occured
 * (memory allocation failure most probably).
 */
FRIBIDI_ENTRY FriBidiLevel
fribidi_get_par_embedding_levels (
  const FriBidiChar *str,	/* input paragraph string */
  const FriBidiStrIndex len,	/* input string length of the paragraph */
  const FriBidiCharType *bidi_types,	/* input bidi types */
  FriBidiParType *pbase_dir,	/* requested and resolved paragraph
				 * base direction */
  FriBidiLevel *embedding_levels	/* output list of embedding levels */
) FRIBIDI_GNUC_WARN_UNUSED;

#define fribidi_reorder_line FRIBIDI_NAMESPACE(reorder_line)
/* fribidi_reorder_line - reorder a line of logical string to visual
 *
 * This function reorders the characters in a line of text from logical to
 * final visual order.  This function implements part 4 of rule L1, and rules
 * L2 and L3 of the Unicode Bidirectional Algorithm available at
 * http://www.unicode.org/reports/tr9/#Reordering_Resolved_Levels.
 *
 * As a side effect it also sets position maps if not NULL.
 *
 * You can provide either the string str, or the bidi types; or both.  If
 * bidi_types are provided, they are used as the bidi types of characters in
 * the string, otherwise the types are computed from the characters in str.
 * If neither str nor bidi types are provided, visual_str is used instead.
 * Feel free to pass the same string as both str and visual_str, but if you
 * done extensive complicated shaping in visual_str, you better provide
 * logical string as str.  There is no known differences yet between providing
 * logical or visual string as str.
 *
 * If you have obtained the embedding levels using custom bidi types, you
 * should provide the same types to this function for valid resutls.
 * Providing bidi types if available at your side, saves you a few cycles, and
 * you don't need to provide str anymore.
 *
 * You should provide the resolved paragraph direction and embedding levels as
 * set by fribidi_get_par_embedding_levels().  Also note that the embedding
 * levels may change a bit.  To be exact, the embedding level of any sequence
 * of white space at the end of line is reset to the paragraph embedding level
 * (That is part 4 of rule L1).
 *
 * Note that the bidi types and embedding levels are not reordered.  You can
 * reorder these (or any other) arrays using the position_L_to_V_map later.
 *
 * Some features of this function can be turned on/off using environmental
 * settings functions fribidi_env_*().
 *
 * Returns: Maximum level found in this line plus one, or zero if any error
 * occured (memory allocation failure most probably).
 */
     FRIBIDI_ENTRY FriBidiLevel fribidi_reorder_line (
  const FriBidiChar *str,	/* input string */
  const FriBidiStrIndex len,	/* input length of the line */
  const FriBidiStrIndex off,	/* input offset of the beginning of the line
				   in the paragraph */
  const FriBidiCharType *bidi_types,	/* input bidi types */
  const FriBidiParType base_dir,	/* resolved paragraph base direction */
  FriBidiLevel *embedding_levels,	/* list of embedding levels,
					   as returned by
					   fribidi_get_par_embedding_levels */
  FriBidiChar *visual_str,	/* visual string to reorder */
  FriBidiStrIndex *positions_L_to_V,	/* output mapping from logical to
					   visual string positions */
  FriBidiStrIndex *positions_V_to_L	/* output mapping from visual string
					   back to logical string positions */
) FRIBIDI_GNUC_WARN_UNUSED;

#include "fribidi-enddecls.h"

#endif /* !_FRIBIDI_BIDI_H */
/* Editor directions:
 * vim:textwidth=78:tabstop=8:shiftwidth=2:autoindent:cindent
 */
