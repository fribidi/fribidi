/* FriBidi
 * fribidi-char-type.c - get character bidi type
 *
 * $Id: fribidi-bidi-type.c,v 1.1 2004-04-25 18:47:57 behdad Exp $
 * $Author: behdad $
 * $Date: 2004-04-25 18:47:57 $
 * $Revision: 1.1 $
 * $Source: /home/behdad/src/fdo/fribidi/togit/git/../fribidi/fribidi2/lib/Attic/fribidi-bidi-type.c,v $
 *
 * Authors:
 *   Behdad Esfahbod, 2001, 2002, 2004
 *
 * Copyright (C) 2004 Sharif FarsiWeb, Inc
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

#include <fribidi-bidi-type.h>

#include "bidi-types.h"
#include "bidi-type-table.i"

#include "common.h"

FRIBIDI_ENTRY FriBidiCharType
fribidi_get_bidi_type (
  /* input */
  FriBidiChar ch
)
{
  return get_bidi_type (ch);
}

/* The following is only defined for binary compatibility */
#define fribidi_get_type_internal FRIBIDI_NAMESPACE(get_type_internal)
FRIBIDI_ENTRY FriBidiCharType
fribidi_get_type_internal (
  /* input */
  FriBidiChar ch
)
{
  return get_bidi_type (ch);
}

FRIBIDI_ENTRY void
fribidi_get_bidi_types (
  /* input */
  const FriBidiChar *str,
  FriBidiStrIndex len,
  /* output */
  FriBidiCharType *type
)
{
  for (; len; len--)
    *type++ = fribidi_get_bidi_type (*str++);
}

/* Map fribidi_prop_types to fribidi_types. */
const FriBidiCharType fribidi_prop_to_type_[] = {
# define _FRIBIDI_ADD_TYPE(TYPE,SYMBOL) FRIBIDI_TYPE_##TYPE,
# include "bidi-types-list.h"
# undef _FRIBIDI_ADD_TYPE
};
