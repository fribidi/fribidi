/* FriBidi
 * fribidi-joining-types.c - character joining types
 *
 * $Id: fribidi-joining-types.c,v 1.2 2004-06-14 18:43:53 behdad Exp $
 * $Author: behdad $
 * $Date: 2004-06-14 18:43:53 $
 * $Revision: 1.2 $
 * $Source: /home/behdad/src/fdo/fribidi/togit/git/../fribidi/fribidi2/lib/fribidi-joining-types.c,v $
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

#include "common.h"

#if !FRIBIDI_NO_ARABIC

#include <fribidi-joining-types.h>

#include "joining-types.h"

enum FriBidiJoiningTypeShortEnum
{
# define _FRIBIDI_ADD_TYPE(TYPE,SYMBOL) TYPE = FRIBIDI_JOINING_TYPE_##TYPE,
# include "fribidi-joining-types-list.h"
# undef _FRIBIDI_ADD_TYPE
  _FRIBIDI_NUM_TYPES
};

#include "joining-type.tab.i"

FRIBIDI_ENTRY FriBidiJoiningType
fribidi_get_joining_type (
  /* input */
  FriBidiChar ch
)
{
  return FRIBIDI_GET_JOINING_TYPE (ch);
}

FRIBIDI_ENTRY void
fribidi_get_joining_types (
  /* input */
  const FriBidiChar *str,
  const FriBidiStrIndex len,
  /* output */
  FriBidiJoiningType *type
)
{
  register FriBidiStrIndex i = len;
  for (; i; i--)
    {
      *type++ = FRIBIDI_GET_JOINING_TYPE (*str);
      str++;
    }
}

FRIBIDI_ENTRY const char *
fribidi_joining_type_name (
  /* input */
  FriBidiJoiningType j
)
{
  switch (j)
    {
#   define _FRIBIDI_ADD_TYPE(TYPE,SYMBOL) case FRIBIDI_JOINING_TYPE_##TYPE: return STRINGIZE(TYPE);
#   include "fribidi-joining-types-list.h"
#   undef _FRIBIDI_ADD_TYPE
    default:
      return "?";
    }
}

#ifdef DEBUG

char
fribidi_char_from_joining_type (
  /* input */
  FriBidiJoiningType j
)
{
  switch (j)
    {
#   define _FRIBIDI_ADD_TYPE(TYPE,SYMBOL) case FRIBIDI_JOINING_TYPE_##TYPE: return SYMBOL;
#   include "fribidi-joining-types-list.h"
#   undef _FRIBIDI_ADD_TYPE
    default:
      return '?';
    }
}

#endif /* DEBUG */

#endif /* !FRIBIDI_NO_ARABIC */

/* Editor directions:
 * vim:textwidth=78:tabstop=8:shiftwidth=2:autoindent:cindent
 */
