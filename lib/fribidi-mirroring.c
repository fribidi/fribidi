/* FriBidi
 * fribidi-mirroring.c - get mirrored character
 *
 * $Id: fribidi-mirroring.c,v 1.1 2004-04-25 18:47:57 behdad Exp $
 * $Author: behdad $
 * $Date: 2004-04-25 18:47:57 $
 * $Revision: 1.1 $
 * $Source: /home/behdad/src/fdo/fribidi/togit/git/../fribidi/fribidi2/lib/fribidi-mirroring.c,v $
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

#include <fribidi-mirroring.h>

#include "mirroring-table.i"

#include "common.h"

FRIBIDI_ENTRY fribidi_boolean
fribidi_get_mirror_char (
  /* input */
  FriBidiChar ch,
  /* output */
  FriBidiChar *mirrored_ch
)
{
  int pos, step;
  fribidi_boolean found;

  pos = step = (nFriBidiMirroredChars / 2) + 1;

  while (step > 1)
    {
      FriBidiChar cmp_ch = FriBidiMirroredChars[pos].ch;
      step = (step + 1) / 2;

      if (cmp_ch < ch)
	{
	  pos += step;
	  if (pos > nFriBidiMirroredChars - 1)
	    pos = nFriBidiMirroredChars - 1;
	}
      else if (cmp_ch > ch)
	{
	  pos -= step;
	  if (pos < 0)
	    pos = 0;
	}
      else
	break;
    }
  found = FriBidiMirroredChars[pos].ch == ch;
  if (mirrored_ch)
    *mirrored_ch = found ? FriBidiMirroredChars[pos].mirrored_ch : ch;

  return found;
}

/* Editor directions:
 * Local Variables:
 *   mode:c
 *   c-basic-offset: 2
 *   indent-tabs-mode:t
 *   tab-width: 8
 * End:
 * vim: textwidth=78: autoindent: cindent: shiftwidth=2: tabstop=8
 */
