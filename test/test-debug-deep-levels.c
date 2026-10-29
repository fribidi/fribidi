/* FriBidi
 * test-debug-deep-levels.c - regression test for the debug level dump on
 * deeply nested embeddings
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

/* With debug output enabled, fribidi_get_par_embedding_levels_ex() prints
 * the level of every run through a character table. The table used to be
 * shorter than the deepest levels a paragraph can reach, so the dump read
 * past its end. A stack of RLEs followed by a letter reaches the deepest
 * level. Without DEBUG compiled in, fribidi_set_debug() does nothing and
 * the test only checks the resolved level; it is most useful under
 * UBSan/ASan with a debug build.
 */

#include <stdio.h>
#include <fribidi.h>

#define NUM_RLE ((FRIBIDI_BIDI_MAX_EXPLICIT_LEVEL + 1) / 2)
#define LEN (NUM_RLE + 1)

int
main (void)
{
  FriBidiChar text[LEN];
  FriBidiCharType types[LEN];
  FriBidiBracketType brackets[LEN];
  FriBidiLevel levels[LEN];
  FriBidiParType base_dir = FRIBIDI_PAR_LTR;
  int i;

  for (i = 0; i < NUM_RLE; i++)
    text[i] = 0x202B;		/* RLE */
  text[NUM_RLE] = 0x0061;	/* 'a' */

  fribidi_set_debug (1);

  fribidi_get_bidi_types (text, LEN, types);
  fribidi_get_bracket_types (text, LEN, types, brackets);
  if (!fribidi_get_par_embedding_levels_ex (types, brackets, LEN, &base_dir,
					    levels))
    {
      fprintf (stderr, "level resolution failed\n");
      return 1;
    }

  if (levels[NUM_RLE] != FRIBIDI_BIDI_MAX_EXPLICIT_LEVEL + 1)
    {
      fprintf (stderr, "expected level %d, got %d\n",
	       FRIBIDI_BIDI_MAX_EXPLICIT_LEVEL + 1, (int) levels[NUM_RLE]);
      return 1;
    }

  return 0;
}
