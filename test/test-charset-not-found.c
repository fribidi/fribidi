/* FriBidi
 * test-charset-not-found.c - regression test for fribidi_unicode_to_charset()
 * with FRIBIDI_CHAR_SET_NOT_FOUND
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
#include <fribidi.h>

int
main (void)
{
  /* The output must be null-terminated for an unknown character set too. */
  const FriBidiChar text[] = { 0x0627, 0x0628 };
  char out[2 * 4 + 1] = "garbage!";
  FriBidiCharSet char_set = fribidi_parse_charset ("no-such-charset");
  FriBidiStrIndex len;

  if (char_set != FRIBIDI_CHAR_SET_NOT_FOUND)
    {
      fprintf (stderr, "no-such-charset was parsed as a character set\n");
      return 1;
    }

  len = fribidi_unicode_to_charset (char_set, text, 2, out);
  if (len != 0 || out[0] != '\0')
    {
      fprintf (stderr, "expected an empty string, got length %ld, \"%s\"\n",
               (long) len, out);
      return 1;
    }

  return 0;
}
