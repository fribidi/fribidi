/* FriBidi
 * bidi-types.h - define internal bidi types
 *
 * $Id: bidi-types.h,v 1.1 2004-04-25 18:47:57 behdad Exp $
 * $Author: behdad $
 * $Date: 2004-04-25 18:47:57 $
 * $Revision: 1.1 $
 * $Source: /home/behdad/src/fdo/fribidi/togit/git/../fribidi/fribidi2/lib/bidi-types.h,v $
 *
 * Author:
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
#ifndef _BIDI_TYPES_H
#define _BIDI_TYPES_H

#include <fribidi-common.h>

#include <fribidi-types.h>
#include <fribidi-bidi-types.h>

#include "common.h"

#include <fribidi-begindecls.h>

/*
 * Define character types that char_type_tables use.
 * define them to be 0, 1, 2, ... and then in fribidi_get_type.c map them
 * to FriBidiCharTypes.
 */
typedef char FriBidiPropCharType;

enum FriBidiPropEnum
{
# define _FRIBIDI_ADD_TYPE(TYPE,SYMBOL) FRIBIDI_PROP_TYPE_##TYPE,
# include "bidi-types-list.h"
# undef _FRIBIDI_ADD_TYPE
  _FRIBIDI_PROP_TYPES_COUNT
};

#define fribidi_prop_to_type_ FRIBIDI_NAMESPACE(prop_to_type_)
/* Maps fribidi_prop_types to fribidi_types */
extern const FriBidiCharType fribidi_prop_to_type_[];

#if DEBUG

#define fribidi_char_from_bidi_type FRIBIDI_NAMESPACE(char_from_bidi_type)
FRIBIDI_ENTRY char fribidi_char_from_bidi_type (
  FriBidiCharType t		/* input bidi type */
);

#endif /* DEBUG */

#include <fribidi-enddecls.h>

#endif /* !_BIDI_TYPES_H */
/* Editor directions:
 * vim:textwidth=78:tabstop=8:shiftwidth=2:autoindent:cindent
 */
