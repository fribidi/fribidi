/* FriBidi
 * fribidi-mirroring.c - get mirrored character
 *
 * $Id: fribidi-env.c,v 1.1 2004-04-25 18:47:57 behdad Exp $
 * $Author: behdad $
 * $Date: 2004-04-25 18:47:57 $
 * $Revision: 1.1 $
 * $Source: /home/behdad/src/fdo/fribidi/togit/git/../fribidi/fribidi2/lib/Attic/fribidi-env.c,v $
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

#include <fribidi-env.h>

#include "common.h"

#if DEBUG
static fribidi_boolean flag_debug = FALSE;
#endif

FRIBIDI_ENTRY fribidi_boolean
fribidi_debug_status (
  void
)
{
#if DEBUG
  return flag_debug;
#else
  return 0;
#endif
}

FRIBIDI_ENTRY fribidi_boolean
fribidi_set_debug (
  fribidi_boolean state
)
{
#if DEBUG
  return flag_debug = state;
#else
  return 0;
#endif
}

static fribidi_boolean flag_mirroring = TRUE;

FRIBIDI_ENTRY fribidi_boolean
fribidi_set_mirroring (
  fribidi_boolean state
)
{
  return flag_mirroring = state;
}

FRIBIDI_ENTRY fribidi_boolean
fribidi_mirroring_status (
  void
)
{
  return flag_mirroring;
}

static fribidi_boolean flag_reorder_nsm = FALSE;

FRIBIDI_ENTRY fribidi_boolean
fribidi_set_reorder_nsm (
  fribidi_boolean state
)
{
  return flag_reorder_nsm = state;
}

fribidi_boolean
fribidi_reorder_nsm_status (
  void
)
{
  return flag_reorder_nsm;
}


/* Editor directions:
 * vim:textwidth=78:tabstop=8:shiftwidth=2:autoindent:cindent
 */
