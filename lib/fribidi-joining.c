/* FriBidi
 * fribidi-joining.h - Arabic joining algorithm
 *
 * $Id: fribidi-joining.c,v 1.2 2004-06-15 11:52:02 behdad Exp $
 * $Author: behdad $
 * $Date: 2004-06-15 11:52:02 $
 * $Revision: 1.2 $
 * $Source: /home/behdad/src/fdo/fribidi/togit/git/../fribidi/fribidi2/lib/fribidi-joining.c,v $
 *
 * Authors:
 *   Behdad Esfahbod, 2004
 *
 * Copyright (C) 2004 Sharif FarsiWeb, Inc
 * Copyright (C) 2004 Behdad Esfahbod
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

#if !FRIBIDI_NO_ARABIC

#include <fribidi-joining.h>
#include <fribidi-env.h>

#include "mem.h"
#include "env.h"
#include "joining-types.h"

#if DEBUG
/*======================================================================
 *  For debugging, define some functions for printing joining types and
 *  properties.
 *----------------------------------------------------------------------*/

static void
print_joining_types (
  /* input */
  const FriBidiLevel *embedding_levels,
  const FriBidiStrIndex len,
  const FriBidiJoiningType *jtypes
)
{
  register FriBidiStrIndex i;

  fribidi_assert (jtypes);

  MSG ("  Join. types: ");
  for (i = 0; i < len; i++)
    MSG2 ("%c", fribidi_char_from_joining_type (jtypes[i],
						!FRIBIDI_LEVEL_IS_RTL
						(embedding_levels[i])));
  MSG ("\n");
}
#endif /* DEBUG */


FRIBIDI_ENTRY void
fribidi_join_arabic (
  /* input */
  const FriBidiLevel *embedding_levels,
  const FriBidiStrIndex len,
  /* input and output */
  FriBidiArabicProps *ar_props
)
{
  if UNLIKELY
    (len == 0) return;

  DBG ("entering fribidi_join_arabic");

  fribidi_assert (embedding_levels);
  fribidi_assert (ar_props);

# if DEBUG
  if UNLIKELY
    (fribidi_debug_status ())
    {
      print_joining_types (embedding_levels, len, ar_props);
    }
# endif	/* DEBUG */

  /* 8.2 Arabic - Cursive Joining */
  DBG ("Arabic cursive joining");
  {
    register FriBidiStrIndex i = 0;

    for (i = 0; i < len; i++)
      {
	register FriBidiStrIndex saved = i;
	register const FriBidiLevel direction =
	  FRIBIDI_LEVEL_IS_RTL (embedding_levels[i]);
	register const FriBidiArabicProps joins_preceding_mask =
	  FRIBIDI_JOINS_PRECEDING_MASK (direction);
	register const FriBidiArabicProps joins_following_mask =
	  FRIBIDI_JOINS_FOLLOWING_MASK (direction);
	register fribidi_boolean joins = false;

	/* Sweep over directional runs */
	for (;
	     i < len
	     && FRIBIDI_LEVEL_IS_RTL (embedding_levels[i]) == direction; i++)
	  {
	    /* R1. Transparent chars are skipped (and so do iGnored chars) */
	    if (FRIBIDI_IS_JOIN_SKIPPED (ar_props[i]))
	      continue;

	    /* R2..R7. */
	    if (!joins)
	      FRIBIDI_UNSET_BITS (ar_props[i], joins_preceding_mask);
	    else if (!FRIBIDI_TEST_BITS (ar_props[i], joins_preceding_mask))
	      FRIBIDI_UNSET_BITS (ar_props[saved], joins_following_mask);
	    else
	      {
		/* This is a FriBidi extension:  we set joining properties
		 * for skipped characters in between. */
		for (saved++; saved < i; saved++)
		  FRIBIDI_SET_BITS (ar_props[saved],
				    joins_preceding_mask |
				    joins_following_mask);
	      }

	    joins = FRIBIDI_TEST_BITS (ar_props[i], joins_following_mask);
	    saved = i;
	  }
	FRIBIDI_UNSET_BITS (ar_props[saved], joins_following_mask);
	i--;
      }
  }

# if DEBUG
  if UNLIKELY
    (fribidi_debug_status ())
    {
      print_joining_types (embedding_levels, len, ar_props);
    }
# endif	/* DEBUG */

  DBG ("leaving fribidi_join_arabic");
}

#endif /* !FRIBIDI_NO_ARABIC */

/* Editor directions:
 * vim:textwidth=78:tabstop=8:shiftwidth=2:autoindent:cindent
 */
