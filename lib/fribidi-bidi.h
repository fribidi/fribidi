/* FriBidi
 * fribidi-bidi.h - bidirectional algorithm
 *
 * $Id: fribidi-bidi.h,v 1.8 2004-06-14 17:00:33 behdad Exp $
 * $Author: behdad $
 * $Date: 2004-06-14 17:00:33 $
 * $Revision: 1.8 $
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
/* fribidi_get_par_embedding_levels - get bidi embedding levels of a paragraph
 *
 * This function finds the bidi embedding levels of a single paragraph,
 * as defined by the Unicode Bidirectional Algorithm.
 *
 * You can provide either the string, or the bidi types; or both.  If bidi_types are
 * provided, they are used as the bidi types of characters in the string, otherwise
 * the types are computed from the characters in str.  Providing bidi types if
 * available at your side, saves you a few cycles.  Bidi types can be obtained
 * by calling fribidi_get_bidi_types.
 *
 * Returns: Maximum level found plus one, or zero if any error occured
 * (memory allocation failure most probably).
 */
FRIBIDI_ENTRY FriBidiLevel
fribidi_get_par_embedding_levels (
  const FriBidiChar *str,	/* input paragraph string */
  const FriBidiStrIndex len,	/* input string length of the paragraph */
  const FriBidiCharType *bidi_types, /* input bidi types */
  FriBidiParType *pbase_dir,	/* requested and resolved paragraph
				 * base direction */
  FriBidiLevel *embedding_level_list	/* output list of embedding levels */
) FRIBIDI_GNUC_WARN_UNUSED;

#define fribidi_shape_mirroring FRIBIDI_NAMESPACE(shape_mirroring)
/* fribidi_shape - do mirroring shaping
 *
 * This functions replaces mirroring characters on right-to-left embeddings in
 * string str with their mirrored equivalent as returned by
 * fribidi_get_mirror_char().
 */
     FRIBIDI_ENTRY void fribidi_shape_mirroring (
  const FriBidiLevel *embedding_level_list,	/* input list of embedding
						   levels, as returned by
						   fribidi_get_par_embedding_levels */
  const FriBidiStrIndex len,	/* input string length */
  FriBidiChar *str		/* string to shape */
);

#define fribidi_reorder_line FRIBIDI_NAMESPACE(reorder_line)
/* fribidi_reorder_line - reorder a line of logical string to visual
 *
 * This function reorders the characters in a line of text from logical
 * to final visual order.  Also sets position maps if not NULL.
 *
 * You can provide either the string, or the bidi types; or both.  If bidi_types are
 * provided, they are used as the bidi types of characters in the string, otherwise
 * the types are computed from the characters in str.  If you have obtained
 * the embedding levels using custom bidi types, you should provide the same
 * types to this function for valid resutls.  Providing bidi types if
 * available at your side, saves you a few cycles.
 *
 * Note that the bidi types and embedding levels are not reordered.  You can
 * reorder these (or any other) arrays using the position_L_to_V_map later.
 *
 * Some features of this function can be turned on/off using environmental settings
 * functions fribidi_env_*.
 *
 * Returns: Maximum level found in this line plus one, or zero if any error
 * occured (memory allocation failure most probably).
 */
     FRIBIDI_ENTRY FriBidiLevel fribidi_reorder_line (
  const FriBidiLevel *embedding_level_list,	/* input list of embedding levels,
					   as returned by
					   fribidi_get_par_embedding_levels */
  const FriBidiStrIndex len,	/* input length of the line */
  const FriBidiStrIndex off,	/* input offset of the beginning of the line
				   in the paragraph */
  const FriBidiCharType *bidi_types,	/* input bidi types */
  FriBidiChar *str,		/* string to shape */
  FriBidiStrIndex *position_L_to_V_list,	/* output mapping from logical to
						   visual string positions */
  FriBidiStrIndex *position_V_to_L_list	/* output mapping from visual string
					   back to logical string positions */
) FRIBIDI_GNUC_WARN_UNUSED;

#include "fribidi-enddecls.h"

#endif /* !_FRIBIDI_BIDI_H */
/* Editor directions:
 * vim:textwidth=78:tabstop=8:shiftwidth=2:autoindent:cindent
 */
