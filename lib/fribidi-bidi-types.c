/* FriBidi
 * fribidi-bidi-types.c - character bidi types
 *
 * $Id: fribidi-bidi-types.c,v 1.1 2004-04-25 18:47:57 behdad Exp $
 * $Author: behdad $
 * $Date: 2004-04-25 18:47:57 $
 * $Revision: 1.1 $
 * $Source: /home/behdad/src/fdo/fribidi/togit/git/../fribidi/fribidi2/lib/fribidi-bidi-types.c,v $
 *
 * Authors:
 *   Behdad Esfahbod, 2001, 2002, 2004
 *
 * Copyright (C) 2004 Sharif FarsiWeb, Inc.
 * Copyright (C) 2001,2002 Behdad Esfahbod
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

#include <fribidi-bidi-types.h>

#include "common.h"

#ifdef DEBUG

FRIBIDI_ENTRY char
fribidi_char_from_bidi_type (
  /* input */
  FriBidiCharType t
)
{
  switch (t)
    {
#   define _FRIBIDI_ADD_TYPE(TYPE,SYMBOL) case FRIBIDI_TYPE_##TYPE: return SYMBOL;
#   include "bidi-types-list.h"
#   undef _FRIBIDI_ADD_TYPE
    default:
      return '?';
    }
}

#endif

FRIBIDI_ENTRY const char *
fribidi_bidi_type_name (
  /* input */
  FriBidiCharType t
)
{
  switch (t)
    {
#   define _FRIBIDI_ADD_TYPE(TYPE,SYMBOL) case FRIBIDI_TYPE_##TYPE: return #TYPE;
#   include "bidi-types-list.h"
#   undef _FRIBIDI_ADD_TYPE
    default:
      return "?";
    }
}

/* Editor directions:
 * vim:textwidth=78:tabstop=8:shiftwidth=2:autoindent:cindent
 */
