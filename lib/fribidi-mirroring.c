/* fribidi-mirroring.c - get mirrored character
 *
 * Copyright (C) 2004  Sharif FarsiWeb, Inc
 * Copyright (C) 2001, 2002, 2004  Behdad Esfahbod
 * Copyright (C) 1999, 2000  Dov Grobgeld
 *
 * This file is part of GNU FriBidi.
 * 
 * GNU FriBidi is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1
 * of the License, or (at your option) any later version.
 * 
 * GNU FriBidi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with GNU FriBidi; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 * 
 * For licensing issues, contact <license@farsiweb.info> or write to
 * Sharif FarsiWeb, Inc., PO Box 13445-389, Tehran, Iran.
 */
/* $Id: fribidi-mirroring.c,v 1.10 2004-06-09 20:01:00 behdad Exp $
 * $Author: behdad $
 * $Date: 2004-06-09 20:01:00 $
 * $Revision: 1.10 $
 * $Source: /home/behdad/src/fdo/fribidi/togit/git/../fribidi/fribidi2/lib/fribidi-mirroring.c,v $
 *
 * Authors:
 *   Behdad Esfahbod, 2001, 2002, 2004
 *   Dov Grobgeld, 1999, 2000
 */

#include "common.h"

#include <fribidi-mirroring.h>

#include "mirroring.tab.i"

FRIBIDI_ENTRY fribidi_boolean
fribidi_get_mirror_char (
  /* input */
  FriBidiChar ch,
  /* output */
  FriBidiChar *mirrored_ch
)
{
  register FriBidiChar result;
  result = FRIBIDI_GET_MIRRORING_DELTA (ch);
  if (mirrored_ch)
    *mirrored_ch = ch + result;
  return result ? true : false;
}

/* Editor directions:
 * Local Variables:
 *   mode: c
 *   c-basic-offset: 2
 *   indent-tabs-mode: t
 *   tab-width: 8
 * End:
 * vim: textwidth=78: autoindent: cindent: shiftwidth=2: tabstop=8:
 */
