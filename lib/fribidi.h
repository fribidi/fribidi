/* FriBidi
 * fribidi.h - Unicode bidirectional and Arabic joining/shaping algorithms
 *
 * $Id: fribidi.h,v 1.5 2004-06-09 20:01:00 behdad Exp $
 * $Author: behdad $
 * $Date: 2004-06-09 20:01:00 $
 * $Revision: 1.5 $
 * $Source: /home/behdad/src/fdo/fribidi/togit/git/../fribidi/fribidi2/lib/fribidi.h,v $
 *
 * Author:
 *   Behdad Esfahbod, 2004
 *
 * Copyright (C) 2004 Sharif FarsiWeb, Inc
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
#ifndef _FRIBIDI_H
#define _FRIBIDI_H

#include "fribidi-unicode.h"
#include "fribidi-types.h"
#include "fribidi-env.h"
#include "fribidi-bidi-type.h"
#include "fribidi-bidi.h"
#include "fribidi-mirroring.h"
#if FRIBIDI_CHARSETS
# include "fribidi-char-sets.h"
#endif /* FRIBIDI_CHARSETS */

#include "fribidi-begindecls.h"

/* See fribidi-bidi.h for the core functions fribidi_get_par_embedding_levels
 * and fribidi_reorder_line which are probably the main calls you need.  See
 * README for a better understanding of what calls you need.
 */


#define fribidi_shape FRIBIDI_NAMESPACE(shape)
/* fribidi_shape - do bidi-dependent shaping
 *
 * This function does all shaping work that depends on the resolved embedding
 * levels of the characters.  Currently it does mirroring and Arabic shaping,
 * but the list may grow later.  Individual shaping features can be turned
 * on/off using environmental setting functions fribidi_env_*.  Feel free to
 * do your own shaping before or after calling this function, but you should
 * take care of embedding levels yourself then.
 */
FRIBIDI_ENTRY void fribidi_shape (
  const FriBidiLevel *embedding_level_list,	/* input list of embedding
						   levels, as returned by
						   fribidi_get_par_embedding_levels */
  const FriBidiStrIndex len,	/* input string length */
  FriBidiChar *str		/* string to shape */
);

#define fribidi_remove_bidi_marks FRIBIDI_NAMESPACE(remove_bidi_marks)
/* fribidi_remove_bidi_marks - remove bidi marks out of an string
 *
 * This function removes the bidi and boundary-neutral marks out of an string
 * and the accompanying lists.  If any of the input lists are NULL, the list
 * is skipped.  If str is the visual string, then position_to_this_list is
 * position_L_to_V_list and position_from_this_list is position_V_to_L_list;
 * if str is the logical string, the other way.  Moreover, the position maps
 * should be filled with valid entries.  A position map pointing to a removed
 * character is filled with -1. By the way, you should not use
 * embedding_level_list if str is visual string.  For best results it should
 * be run on a whole paragraph, not lines; but feel free to do otherwise if
 * you know what you are doing.
 *
 * Returns: New length of the string, or -1 if an error occured (memory
 * allocation failure most probably).
 */
FRIBIDI_ENTRY FriBidiStrIndex
fribidi_remove_bidi_marks (
  FriBidiChar *str,		/* input string to clean */
  const FriBidiStrIndex len,	/* input string length */
  FriBidiStrIndex *position_to_this_list,	/* list mapping positions to the
						   order used in str */
  FriBidiStrIndex *position_from_this_list,	/* list mapping positions from the
						   order used in str */
  FriBidiLevel *embedding_level_list	/* list of embedding levels */
) FRIBIDI_GNUC_WARN_UNUSED;


/* Deprecated interface: */

#define fribidi_log2vis FRIBIDI_NAMESPACE(log2vis)
/* fribidi_log2vis - get visual string
 *
 * This function converts the logical input string to the visual output
 * strings as specified by the Unicode Bidirectional Algorithm.  As a side
 * effect it also generates mapping lists between the two strings, and the
 * list of embedding levels as defined by the algorithm.  If any of the the
 * lists are passed as NULL, the list is ignored and not filled.  This
 * function is obsolete because it only handles one-line paragraphs. 
 * Please consider using other functions instead. Deprecated.
 *
 * Returns: Maximum level found plus one, or zero if any error occured
 * (memory allocation failure most probably).
 */
     FRIBIDI_ENTRY FriBidiLevel fribidi_log2vis (
  const FriBidiChar *str,	/* input logical string */
  const FriBidiStrIndex len,	/* input string length */
  FriBidiParType *pbase_dir,	/* requested and resolved paragraph
				 * base direction */
  FriBidiChar *visual_str,	/* output visual string */
  FriBidiStrIndex *position_L_to_V_list,	/* output mapping from logical to 
						 * visual string positions */
  FriBidiStrIndex *position_V_to_L_list,	/* output mapping from visual string
						 * back to the logical string
						 * positions */
  FriBidiLevel *embedding_level_list	/* output list of embedding levels */
)
     FRIBIDI_GNUC_WARN_UNUSED FRIBIDI_GNUC_DEPRECATED;

#define fribidi_log2vis_get_embedding_levels fribidi_get_par_embedding_levels
#define fribidi_get_type fribidi_get_bidi_type



#define fribidi_version_info FRIBIDI_NAMESPACE(version_info)
/* An string containing the version information of the library. */
     extern const char *fribidi_version_info;

#include "fribidi-enddecls.h"

#endif /* !_FRIBIDI_H */
/* Editor directions:
 * vim:textwidth=78:tabstop=8:shiftwidth=2:autoindent:cindent
 */
