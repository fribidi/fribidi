/* FriBidi
 * fribidi-bidi-type.c - get character bidi type
 *
 * $Id: fribidi-bidi-type.c,v 1.4 2004-05-07 06:30:38 behdad Exp $
 * $Author: behdad $
 * $Date: 2004-05-07 06:30:38 $
 * $Revision: 1.4 $
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

#include "common.h"

#include <fribidi-bidi-type.h>

#include "bidi-types.h"

/*
 * Define character types that char_type_tables use.
 * define them to be 0, 1, 2, ... and then in fribidi_get_type.c map them
 * to FriBidiCharTypes.
 */
typedef unsigned char FriBidiPropCharType;

enum FriBidiPropEnum
{
# define _FRIBIDI_ADD_TYPE(TYPE,SYMBOL) TYPE,
# include "bidi-types-list.h"
# undef _FRIBIDI_ADD_TYPE
  NUM_TYPES
};

#include "bidi-type.tab.i"

/* Map fribidi_prop_types to fribidi_types. */
static const FriBidiCharType prop_to_type[] = {
# define _FRIBIDI_ADD_TYPE(TYPE,SYMBOL) FRIBIDI_TYPE_##TYPE,
# include "bidi-types-list.h"
# undef _FRIBIDI_ADD_TYPE
};

static inline FriBidiCharType
get_bidi_type (
  /* input */
  FriBidiChar uch
)
{
  if (uch < 0x110000)
    return prop_to_type[FRIBIDI_GET_TYPE (uch)];
  else
    return FRIBIDI_TYPE_LTR;
  /* Non-Unicode chars */
}

FRIBIDI_ENTRY FriBidiCharType
fribidi_get_bidi_type (
  /* input */
  FriBidiChar ch
)
{
  return get_bidi_type (ch);
}

/* The following is only defined for binary compatibility */
FriBidiCharType
fribidi_get_type (
  FriBidiChar ch
)
{
  return fribidi_get_bidi_type (ch);
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
  register FriBidiStrIndex i = len;
  for (; i; i--)
    *type++ = get_bidi_type (*str++);
}

/* The following is only defined for binary compatibility */
void
fribidi_get_types (
  const FriBidiChar *str,
  FriBidiStrIndex len,
  FriBidiCharType *type
)
{
  fribidi_get_bidi_types (str, len, type);
}

