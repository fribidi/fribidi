/* FriBidi
 * fribidi.c - Unicode bidirectional and Arabic joining/shaping algorithms
 *
 * $Id: fribidi.c,v 1.8 2004-06-09 14:59:21 behdad Exp $
 * $Author: behdad $
 * $Date: 2004-06-09 14:59:21 $
 * $Revision: 1.8 $
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
  const FriBidiLevel *embedding_level_list,
  const FriBidiStrIndex len,
  /* input and output */
  FriBidiChar *str
)
{
  fribidi_assert (embedding_level_list);

  if UNLIKELY
    (len == 0 || !str) return;

  fribidi_shape_mirroring (embedding_level_list, len, str);
}


FRIBIDI_ENTRY FriBidiStrIndex
fribidi_remove_bidi_marks (
  FriBidiChar *str,
  const FriBidiStrIndex len,
  FriBidiStrIndex *position_to_this_list,
  FriBidiStrIndex *position_from_this_list,
  FriBidiLevel *embedding_level_list
)
{
  register FriBidiStrIndex i, j = 0;
  fribidi_boolean private_from_this = false;
  fribidi_boolean status = false;

  fribidi_assert (str);

  if UNLIKELY
    (len == 0)
    {
      status = true;
      goto out;
    }

  /* If to_this is to not NULL, we must have from_this as well. If it is
     not given by the caller, we have to make a private instance of it. */
  if (position_to_this_list && !position_from_this_list)
    {
      position_from_this_list = fribidi_malloc (sizeof
						(position_from_this_list[0]) *
						len);
      if UNLIKELY
	(!position_from_this_list) goto out;
      private_from_this = true;
      for (i = len - 1; i >= 0; i--)
	position_from_this_list[position_to_this_list[i]] = i;
    }

  for (i = 0; i < len; i++)
    if (!FRIBIDI_IS_EXPLICIT_OR_BN (fribidi_get_bidi_type (str[i]))
	&& str[i] != FRIBIDI_CHAR_LRM && str[i] != FRIBIDI_CHAR_RLM)
      {
	str[j] = str[i];
	if (embedding_level_list)
	  embedding_level_list[j] = embedding_level_list[i];
	if (position_from_this_list)
	  position_from_this_list[j] = position_from_this_list[i];
	j++;
      }

  /* Convert the from_this list to to_this */
  if (position_to_this_list)
    {
      for (i = 0; i < len; i++)
	position_to_this_list[i] = -1;
      for (i = 0; i < len; i++)
	position_to_this_list[position_from_this_list[i]] = i;
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
  FriBidiStrIndex *position_L_to_V_list,
  FriBidiStrIndex *position_V_to_L_list,
  FriBidiLevel *embedding_level_list
)
{
  FriBidiLevel max_level = 0;
  fribidi_boolean private_V_to_L = false;
  fribidi_boolean private_embedding_level_list = false;
  fribidi_boolean status = false;

  fribidi_assert (str);
  fribidi_assert (pbase_dir);

  if UNLIKELY
    (len == 0)
    {
      status = true;
      goto out;
    }

  if (!embedding_level_list)
    {
      embedding_level_list = fribidi_malloc (len);
      if (!embedding_level_list)
	goto out;
      private_embedding_level_list = true;
    }

  max_level = fribidi_get_par_embedding_levels (str, len, pbase_dir,
						embedding_level_list) - 1;
  if UNLIKELY
    (max_level < 0) goto out;

  /* If l2v is to be calculated we must have v2l as well. If it is not
     given by the caller, we have to make a private instance of it. */
  if (position_L_to_V_list && !position_V_to_L_list)
    {
      position_V_to_L_list =
	(FriBidiStrIndex *) fribidi_malloc (sizeof (FriBidiStrIndex) * len);
      if (!position_V_to_L_list)
	goto out;
      private_V_to_L = true;
    }

  if (visual_str)
    {
      register FriBidiStrIndex i;

      for (i = len; i >= 0; i--)
	visual_str[i] = str[i];
    }

  fribidi_shape (embedding_level_list, len, visual_str);

  status = fribidi_reorder_line (embedding_level_list, len, 0, visual_str,
				 position_L_to_V_list, position_V_to_L_list);

out:

  if (private_V_to_L)
    fribidi_free (position_V_to_L_list);

  if (private_embedding_level_list)
    fribidi_free (embedding_level_list);

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
