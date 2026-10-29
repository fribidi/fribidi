/* FriBidi
 * test-isolate-chain.c - regression test for long chains of isolates
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

/* Resolving the levels of a paragraph made of many sibling isolates used
 * to take quadratic time, because the forward search for the adjacent run
 * walked the whole chain of merged PDI runs from every run. Two texts
 * build such chains: "LRI x PDI" repeated, and "PDI NSM" repeated after a
 * paragraph separator. Each takes seconds at the sizes below with the
 * quadratic code and milliseconds without. The test checks both the
 * resolved levels and that the time stays within a generous bound.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fribidi.h>

#define UNITS 60000
#define MAX_SECONDS 2.0

static int
check (const char *name, const FriBidiChar *text, FriBidiStrIndex len,
       FriBidiParType base_dir, FriBidiLevel expected)
{
  FriBidiCharType *types = malloc (len * sizeof (*types));
  FriBidiBracketType *brackets = malloc (len * sizeof (*brackets));
  FriBidiLevel *levels = malloc (len * sizeof (*levels));
  FriBidiStrIndex i;
  int failed = 0;
  double secs;
  clock_t start;

  if (!types || !brackets || !levels)
    {
      fprintf (stderr, "%s: out of memory\n", name);
      free (types);
      free (brackets);
      free (levels);
      return 1;
    }

  fribidi_get_bidi_types (text, len, types);
  fribidi_get_bracket_types (text, len, types, brackets);

  start = clock ();
  if (!fribidi_get_par_embedding_levels_ex (types, brackets, len, &base_dir,
					    levels))
    {
      fprintf (stderr, "%s: level resolution failed\n", name);
      failed = 1;
    }
  secs = (double) (clock () - start) / CLOCKS_PER_SEC;

  for (i = 0; !failed && i < len; i++)
    if (levels[i] != expected)
      {
	fprintf (stderr, "%s: expected level %d at %d, got %d\n", name,
		 (int) expected, (int) i, (int) levels[i]);
	failed = 1;
      }

  if (secs > MAX_SECONDS)
    {
      fprintf (stderr, "%s: took %.2f s (limit %.1f s)\n", name, secs,
	       MAX_SECONDS);
      failed = 1;
    }

  free (types);
  free (brackets);
  free (levels);
  return failed;
}

int
main (void)
{
  FriBidiChar *text = malloc ((3 * (size_t) UNITS + 2) * sizeof (*text));
  FriBidiStrIndex len = 0;
  int i, failed = 0;

  if (!text)
    {
      fprintf (stderr, "out of memory\n");
      return 1;
    }

  /* 'a', then "LRI x PDI" repeated, then 'b' in an RTL paragraph: the
     isolates are all between L characters and resolve to level 2. */
  text[len++] = 0x0061;
  for (i = 0; i < UNITS; i++)
    {
      text[len++] = 0x2066;
      text[len++] = 0x0078;
      text[len++] = 0x2069;
    }
  text[len++] = 0x0062;
  failed |= check ("back-to-back isolates", text, len, FRIBIDI_PAR_RTL, 2);

  /* Paragraph separator, then "PDI NSM" repeated: all level 0. */
  len = 0;
  text[len++] = 0x2029;
  for (i = 0; i < UNITS; i++)
    {
      text[len++] = 0x2069;
      text[len++] = 0x0300;
    }
  failed |= check ("PDI NSM after separator", text, len, FRIBIDI_PAR_ON, 0);

  free (text);
  return failed;
}
