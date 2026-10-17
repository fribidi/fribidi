/* FriBidi
 * test-reorder-line.c - regression test for fribidi_reorder_line()
 *
 * Copyright (C) 2026 Mar
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA
 *
 * For licensing issues, contact <fribidi.license@gmail.com>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fribidi.h>

/* Returned by a check that cannot run on this platform. */
#define NOT_RUN 2

/* A short line with an RTL run between two LTR runs reorders correctly,
   both the visual string and the map. */
static int
test_reorder_runs (void)
{
  const FriBidiChar text[] = { 0x0061, 0x0062, 0x05D0, 0x05D1, 0x0063 };
  const FriBidiChar expected_visual[] =
    { 0x0061, 0x0062, 0x05D1, 0x05D0, 0x0063 };
  const FriBidiStrIndex expected_map[] = { 0, 1, 3, 2, 4 };
  const FriBidiStrIndex len = 5;
  FriBidiCharType types[5];
  FriBidiBracketType brackets[5];
  FriBidiLevel levels[5];
  FriBidiChar visual[5];
  FriBidiStrIndex map[5];
  FriBidiParType base_dir = FRIBIDI_PAR_LTR;
  FriBidiStrIndex i;

  fribidi_get_bidi_types (text, len, types);
  fribidi_get_bracket_types (text, len, types, brackets);
  if (!fribidi_get_par_embedding_levels_ex (types, brackets, len, &base_dir,
					    levels))
    {
      fprintf (stderr, "fribidi_get_par_embedding_levels_ex() failed\n");
      return 1;
    }

  memcpy (visual, text, sizeof visual);
  for (i = 0; i < len; i++)
    map[i] = i;

  if (fribidi_reorder_line (FRIBIDI_FLAGS_DEFAULT, types, len, 0, base_dir,
			    levels, visual, map) != 2)
    {
      fprintf (stderr, "fribidi_reorder_line() returned the wrong level\n");
      return 1;
    }

  if (memcmp (visual, expected_visual, sizeof visual) != 0
      || memcmp (map, expected_map, sizeof map) != 0)
    {
      fprintf (stderr, "fribidi_reorder_line() gave unexpected output\n");
      return 1;
    }

  return 0;
}

/* A line with more runs than the run arrays can hold in a size_t is
   rejected, returning 0.  Only reachable where size_t is 32 bits. */
static int
test_reorder_rejects_oversized_runs (void)
{
  const FriBidiStrIndex len = (FriBidiStrIndex) 1 << 27;
  FriBidiCharType *types;
  FriBidiLevel *levels;
  FriBidiStrIndex i;
  int result = 0;

  if (sizeof (size_t) > 4)
    return NOT_RUN;

  types = malloc ((size_t) len * sizeof types[0]);
  levels = malloc ((size_t) len * sizeof levels[0]);
  if (!types || !levels)
    {
      free (types);
      free (levels);
      return NOT_RUN;
    }

  for (i = 0; i < len; i++)
    {
      types[i] = FRIBIDI_TYPE_LTR;
      levels[i] = i & 1;
    }

  if (fribidi_reorder_line (0, types, len, 0, FRIBIDI_PAR_LTR, levels,
			    NULL, NULL) != 0)
    {
      fprintf (stderr, "fribidi_reorder_line() accepted a line with %ld "
	       "runs\n", (long) len);
      result = 1;
    }

  free (types);
  free (levels);
  return result;
}

int
main (void)
{
  int result;

  if (test_reorder_runs ())
    return 1;

  result = test_reorder_rejects_oversized_runs ();
  if (result == NOT_RUN)
    printf ("skipped the size_t overflow check: needs a 32-bit size_t "
	    "and 640 MB of memory\n");
  else if (result)
    return 1;

  return 0;
}
