/* FriBidi
 * test-reorder-nsm.c - regression test for fribidi_set_reorder_nsm()
 *
 * Copyright (C) 2026 Dov Grobgeld
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

/* Regression test for https://github.com/fribidi/fribidi/issues/136
 *
 * fribidi_log2vis() used to share a single "flags" variable with the
 * fribidi_set_reorder_nsm()/fribidi_set_mirroring() setters. When
 * fribidi_log2vis() was moved from fribidi-deprecated.c into fribidi.c,
 * it got its own copy of that variable, so fribidi_set_reorder_nsm(false)
 * silently stopped having any effect on fribidi_log2vis().
 */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <fribidi.h>

int
main (void)
{
  /* "חַיְפַא" (Haifa) with niqud: het patah yod sheva pe patah alef */
  FriBidiChar input[] =
    { 0x05D7, 0x05B7, 0x05D9, 0x05B0, 0x05E4, 0x05B7, 0x05D0 };
  /* Plain right-to-left reversal, i.e. what the output must be when
     non-spacing mark reordering is turned off. */
  FriBidiChar expected[] =
    { 0x05D0, 0x05B7, 0x05E4, 0x05B0, 0x05D9, 0x05B7, 0x05D7 };
  const FriBidiStrIndex len = 7;
  FriBidiChar visual[7];
  FriBidiParType base_dir;
  int i, failed = 0;

  base_dir = FRIBIDI_PAR_RTL;
  fribidi_set_reorder_nsm (false);

  if (!fribidi_log2vis (input, len, &base_dir, visual, NULL, NULL, NULL))
    {
      fprintf (stderr, "fribidi_log2vis() failed\n");
      return 1;
    }

  if (memcmp (visual, expected, sizeof expected) != 0)
    {
      fprintf (stderr, "fribidi_log2vis() with reorder_nsm disabled "
	       "gave unexpected output\n");
      fprintf (stderr, "expected:");
      for (i = 0; i < len; i++)
	fprintf (stderr, " %04X", expected[i]);
      fprintf (stderr, "\ngot:     ");
      for (i = 0; i < len; i++)
	fprintf (stderr, " %04X", visual[i]);
      fprintf (stderr, "\n");
      failed = 1;
    }

  return failed;
}
