/* FriBidi
 * fribidi.c - Unicode bidirectional and Arabic joining/shaping algorithms
 *
 * $Id: fribidi.c,v 1.12 2004-06-18 19:21:33 behdad Exp $
 * $Author: behdad $
 * $Date: 2004-06-18 19:21:33 $
 * $Revision: 1.12 $
 * $Source: /home/behdad/src/fdo/fribidi/togit/git/../fribidi/fribidi2/lib/fribidi.c,v $
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

#include "common.h"

#include <fribidi.h>

FRIBIDI_ENTRY void
fribidi_shape (
  /* input */
  const FriBidiLevel *embedding_levels,
  const FriBidiStrIndex len,
  /* input and output */
  FriBidiChar *str
)
{
  if UNLIKELY
    (len == 0 || !str) return;

  DBG ("in fribidi_shape");

  fribidi_assert (embedding_levels);

  fribidi_shape_mirroring (embedding_levels, len, str);
}


FRIBIDI_ENTRY FriBidiStrIndex
fribidi_remove_bidi_marks (
  FriBidiChar *str,
  const FriBidiStrIndex len,
  FriBidiStrIndex *positions_to_this,
  FriBidiStrIndex *position_from_this_list,
  FriBidiLevel *embedding_levels
)
{
  register FriBidiStrIndex i, j = 0;
  fribidi_boolean private_from_this = false;
  fribidi_boolean status = false;

  if UNLIKELY
    (len == 0)
    {
      status = true;
      goto out;
    }

  DBG ("in fribidi_remove_bidi_marks");

  fribidi_assert (str);

  /* If to_this is to not NULL, we must have from_this as well. If it is
     not given by the caller, we have to make a private instance of it. */
  if (positions_to_this && !position_from_this_list)
    {
      position_from_this_list = fribidi_malloc (sizeof
						(position_from_this_list[0]) *
						len);
      if UNLIKELY
	(!position_from_this_list) goto out;
      private_from_this = true;
      for (i = len - 1; i >= 0; i--)
	position_from_this_list[positions_to_this[i]] = i;
    }

  for (i = 0; i < len; i++)
    if (!FRIBIDI_IS_EXPLICIT_OR_BN (fribidi_get_bidi_type (str[i]))
	&& str[i] != FRIBIDI_CHAR_LRM && str[i] != FRIBIDI_CHAR_RLM)
      {
	str[j] = str[i];
	if (embedding_levels)
	  embedding_levels[j] = embedding_levels[i];
	if (position_from_this_list)
	  position_from_this_list[j] = position_from_this_list[i];
	j++;
      }

  /* Convert the from_this list to to_this */
  if (positions_to_this)
    {
      for (i = 0; i < len; i++)
	positions_to_this[i] = -1;
      for (i = 0; i < len; i++)
	positions_to_this[position_from_this_list[i]] = i;
    }

  status = true;

out:

  if (private_from_this)
    fribidi_free (position_from_this_list);

  return status ? j : -1;
}


FRIBIDI_ENTRY FriBidiLevel
fribidi_log2vis (
  /* input */
  const FriBidiChar *str,
  FriBidiStrIndex len,
  /* input and output */
  FriBidiParType *pbase_dir,
  /* output */
  FriBidiChar *visual_str,
  FriBidiStrIndex *positions_L_to_V,
  FriBidiStrIndex *positions_V_to_L,
  FriBidiLevel *embedding_levels
)
{
  FriBidiLevel max_level = 0;
  fribidi_boolean private_V_to_L = false;
  fribidi_boolean private_embedding_levels = false;
  fribidi_boolean status = false;
  FriBidiArabicProps *ar_props = NULL;

  if UNLIKELY
    (len == 0)
    {
      status = true;
      goto out;
    }

  DBG ("in fribidi_log2vis");

  fribidi_assert (str);
  fribidi_assert (pbase_dir);

  if (!embedding_levels)
    {
      embedding_levels = fribidi_malloc (len * sizeof embedding_levels[0]);
      if (!embedding_levels)
	goto out;
      private_embedding_levels = true;
    }

  max_level = fribidi_get_par_embedding_levels (str, len, NULL, pbase_dir,
						embedding_levels) - 1;
  if UNLIKELY
    (max_level < 0) goto out;

#if !FRIBIDI_NO_ARABIC
  /* Arabic joining */
  {
    ar_props = fribidi_malloc (len * sizeof ar_props[0]);
    fribidi_get_joining_types (str, len, ar_props);
    fribidi_join_arabic (embedding_levels, len, ar_props);
  }
#endif /* !FRIBIDI_NO_ARABIC */

  /* If l2v is to be calculated we must have v2l as well. If it is not
     given by the caller, we have to make a private instance of it. */
  if (positions_L_to_V && !positions_V_to_L)
    {
      positions_V_to_L =
	(FriBidiStrIndex *) fribidi_malloc (sizeof (FriBidiStrIndex) * len);
      if (!positions_V_to_L)
	goto out;
      private_V_to_L = true;
    }

  if (visual_str)
    {
      register FriBidiStrIndex i;

      for (i = len; i >= 0; i--)
	visual_str[i] = str[i];
    }

  fribidi_shape (embedding_levels, len, visual_str);

  status = fribidi_reorder_line (visual_str, len, 0, NULL, *pbase_dir, embedding_levels, positions_L_to_V, positions_V_to_L);

out:

  if (private_V_to_L)
    fribidi_free (positions_V_to_L);

  if (private_embedding_levels)
    fribidi_free (embedding_levels);

  if (ar_props)
    fribidi_free (ar_props);

  return status ? max_level + 1 : 0;
}


const char *fribidi_unicode_version = FRIBIDI_UNICODE_VERSION;

const char *fribidi_version_info =
  "(" FRIBIDI_NAME ") " FRIBIDI_VERSION "\n"
  "interface version " FRIBIDI_INTERFACE_VERSION_STRING ",\n"
  "Unicode Character Database version " FRIBIDI_UNICODE_VERSION ",\n"
  "Configure options"
#if DEBUG
  " --enable-debug"
#endif /* DEBUG */
#if USE_SIMPLE_MALLOC
  " --enable-malloc"
#endif /* USE_SIMPLE_MALLOC */
#if !FRIBIDI_CHARSETS
  " --disable-charsets"
#endif /* !FRIBIDI_CHARSETS */
#if FRIBIDI_USE_GLIB
  " --with-glib"
#else /* !FRIBIDI_USE_GLIB */
  " --without-glib"
#endif /* !FRIBIDI_USE_GLIB */
  ".\n\n"
  "Copyright (C) 2004  Sharif FarsiWeb, Inc.\n"
  "Copyright (C) 2004  Behdad Esfahbod\n"
  FRIBIDI_NAME " comes with NO WARRANTY, to the extent permitted by law.\n"
  "You may redistribute copies of " FRIBIDI_NAME " under\n"
  "the terms of the GNU Lesser General Public License.\n"
  "For more information about these matters, see the file named COPYING.\n"
  "Written by Behdad Esfahbod and Dov Grobgeld.\n";

/* Editor directions:
 * vim:textwidth=78:tabstop=8:shiftwidth=2:autoindent:cindent
 */
