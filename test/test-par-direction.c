/* FriBidi
 * test-par-direction.c - regression test for fribidi_get_par_direction()
 *
 * Copyright (C) 2026 ChipLab
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
  /* LRI, 'a', paragraph separator, Hebrew alef. The unterminated
     isolate must not hide the strong character in the next paragraph. */
  const FriBidiChar text[] = { 0x2066, 0x0061, 0x2029, 0x05D0 };
  FriBidiCharType types[4];
  FriBidiParType direction;

  fribidi_get_bidi_types (text, 4, types);
  direction = fribidi_get_par_direction (types, 4);
  if (direction != FRIBIDI_PAR_RTL)
    {
      fprintf (stderr, "expected RTL, got %s\n",
               fribidi_get_bidi_type_name (direction));
      return 1;
    }

  return 0;
}
