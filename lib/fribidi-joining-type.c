/* FriBidi
 * fribidi-joining-type.c - get character joining type
 *
 * $Id: fribidi-joining-type.c,v 1.1 2004-06-13 20:11:42 behdad Exp $
 * $Author: behdad $
 * $Date: 2004-06-13 20:11:42 $
 * $Revision: 1.1 $
 * $Source: /home/behdad/src/fdo/fribidi/togit/git/../fribidi/fribidi2/lib/Attic/fribidi-joining-type.c,v $
 *
 * Authors:
 *   Behdad Esfahbod, 2004
 *
 * Copyright (C) 2004 Sharif FarsiWeb, Inc
 * Copyright (C) 2004 Behdad Esfahbod
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

#include <fribidi-joining-type.h>
#include <fribidi-joining-types.h>

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
