/* FriBidi
 * fribidi-joining-types.h - character joining types
 *
 * $Id: fribidi-joining-types.h,v 1.1 2004-06-13 20:11:42 behdad Exp $
 * $Author: behdad $
 * $Date: 2004-06-13 20:11:42 $
 * $Revision: 1.1 $
 * $Source: /home/behdad/src/fdo/fribidi/togit/git/../fribidi/fribidi2/lib/fribidi-joining-types.h,v $
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
#ifndef _FRIBIDI_JOINING_TYPES_H
#define _FRIBIDI_JOINING_TYPES_H

#include "fribidi-common.h"

#include "fribidi-types.h"

#include "fribidi-begindecls.h"

/* 
 * Define bit masks that joining types are based on, each mask has
 * only one bit set.
 */

#define FRIBIDI_MASK_JOINS_RIGHT	0x01	/* May join to right */
#define FRIBIDI_MASK_JOINS_LEFT		0x02	/* May join to right */
#define FRIBIDI_MASK_ARAB_SHAPES	0x04	/* May Arabic shape */
#define FRIBIDI_MASK_TRANSPARENT	0x10	/* Is transparent */
#define FRIBIDI_MASK_IGNORED		0x20	/* Is ignored */
#define FRIBIDI_MASK_LIGATURED		0x40	/* Is ligatured */

/*
 * Define values for FriBidiJoiningType
 */

/* nUn-joining */
#define FRIBIDI_JOINING_TYPE_U_VAL	( 0 )

/* Right-joining */
#define FRIBIDI_JOINING_TYPE_R_VAL	\
	( FRIBIDI_MASK_JOINS_RIGHT | FRIBIDI_MASK_ARAB_SHAPES )

/* Dual-joining */
#define FRIBIDI_JOINING_TYPE_D_VAL	\
	( FRIBIDI_MASK_JOINS_RIGHT | FRIBIDI_MASK_JOINS_LEFT \
	| FRIBIDI_MASK_ARAB_SHAPES )

/* join-Causing */
#define FRIBIDI_JOINING_TYPE_C_VAL	\
	( FRIBIDI_MASK_JOINS_RIGHT | FRIBIDI_MASK_JOINS_LEFT )

/* Left-joining */
#define FRIBIDI_JOINING_TYPE_L_VAL	\
	( FRIBIDI_MASK_JOINS_LEFT | FRIBIDI_MASK_ARAB_SHAPES )

/* Transparent */
#define FRIBIDI_JOINING_TYPE_T_VAL	\
	( FRIBIDI_MASK_TRANSPARENT | FRIBIDI_MASK_ARAB_SHAPES )

/* iGnored */
#define FRIBIDI_JOINING_TYPE_G_VAL	( FRIBIDI_MASK_IGNORED )


enum _FriBidiJoiningTypeEnum
{
# define _FRIBIDI_ADD_TYPE(TYPE,SYMBOL) \
	FRIBIDI_JOINING_TYPE_##TYPE = FRIBIDI_JOINING_TYPE_##TYPE##_VAL,
# include "fribidi-joining-types-list.h"
# undef _FRIBIDI_ADD_TYPE
  _FRIBIDI_JOINING_TYPE_JUNK	/* Don't use this */
};

#ifdef __FRIBIDI_DOC
typedef enum _FriBidiJoiningTypeEnum FriBidiJoiningType;
#else /* !__FRIBIDI_DOC */
typedef fribidi_uint8 FriBidiJoiningType;
#endif /* !__FRIBIDI_DOC */


/*
 * Defining macros for needed queries, It is fully dependent on the 
 * implementation of FriBidiJoiningType.
 */

/* Joins to right? */
#define FRIBIDI_JOINS_RIGHT(p)	((p) & FRIBIDI_MASK_JOINS_RIGHT)

/* Joins to left? */
#define FRIBIDI_JOINS_LEFT(p)	((p) & FRIBIDI_MASK_JOINS_LEFT)

/* Joins to left? */
#define FRIBIDI_ARAB_SHAPES(p)	((p) & FRIBIDI_MASK_ARAB_SHAPES)

/* Is transparent in joining? */
#define FRIBIDI_IS_JOIN_TRANSPARENT(p)	((p) & FRIBIDI_MASK_TRANSPARENT)

/* Is ignored in joining? */
#define FRIBIDI_IS_JOIN_IGNORED(p)	((p) & FRIBIDI_MASK_IGNORED)

/* Is skipped in joining? */
#define FRIBIDI_IS_JOIN_SKIPPED(p)	\
	((p) & (FRIBIDI_MASK_TRANSPARENT | FRIBIDI_MASK_IGNORED))


#define fribidi_joining_type_name FRIBIDI_NAMESPACE(joining_type_name)
/* fribidi_joining_type_name - get joining type name
 *
 * This function returns the joining type name of a joining type.  The
 * returned string is a static string and should not be freed.
 */
FRIBIDI_ENTRY const char *
fribidi_joining_type_name (
  FriBidiJoiningType j		/* input joining type */
) FRIBIDI_GNUC_CONST;

#include "fribidi-enddecls.h"

#endif /* !_FRIBIDI_JOINING_TYPES_H */
/* Editor directions:
 * vim:textwidth=78:tabstop=8:shiftwidth=2:autoindent:cindent
 */
