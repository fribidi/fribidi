/* FriBidi
 * fribidi-bidi-types.h - character bidi types
 *
 * $Id: fribidi-bidi-types.h,v 1.5 2004-06-07 20:38:21 behdad Exp $
 * $Author: behdad $
 * $Date: 2004-06-07 20:38:21 $
 * $Revision: 1.5 $
 * $Source: /home/behdad/src/fdo/fribidi/togit/git/../fribidi/fribidi2/lib/fribidi-bidi-types.h,v $
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
#ifndef _FRIBIDI_BIDI_TYPES_H
#define _FRIBIDI_BIDI_TYPES_H

#include "fribidi-common.h"

#include "fribidi-types.h"

#include "fribidi-begindecls.h"

typedef signed char FriBidiLevel;

typedef struct _FriBidiRun FriBidiRun;

/* 
 * Define some bit masks, that character types are based on, each one has
 * only one bit on.
 */

/* Do not use enum, because 16bit processors do not allow 32bit enum values. */

#define FRIBIDI_MASK_RTL	0x00000001L	/* Is right to left */
#define FRIBIDI_MASK_ARABIC	0x00000002L	/* Is arabic */

/* Each char can be only one of the three following. */
#define FRIBIDI_MASK_STRONG	0x00000010L	/* Is strong */
#define FRIBIDI_MASK_WEAK	0x00000020L	/* Is weak */
#define FRIBIDI_MASK_NEUTRAL	0x00000040L	/* Is neutral */
#define FRIBIDI_MASK_SENTINEL	0x00000080L	/* Is sentinel */
/* Sentinels are not valid chars, just identify the start and end of strings. */

/* Each char can be only one of the five following. */
#define FRIBIDI_MASK_LETTER	0x00000100L	/* Is letter: L, R, AL */
#define FRIBIDI_MASK_NUMBER	0x00000200L	/* Is number: EN, AN */
#define FRIBIDI_MASK_NUMSEPTER	0x00000400L	/* Is number separator or terminator: ES, ET, CS */
#define FRIBIDI_MASK_SPACE	0x00000800L	/* Is space: BN, BS, SS, WS */
#define FRIBIDI_MASK_EXPLICIT	0x00001000L	/* Is expilict mark: LRE, RLE, LRO, RLO, PDF */

/* Can be on only if FRIBIDI_MASK_SPACE is also on. */
#define FRIBIDI_MASK_SEPARATOR	0x00002000L	/* Is test separator: BS, SS */
/* Can be on only if FRIBIDI_MASK_EXPLICIT is also on. */
#define FRIBIDI_MASK_OVERRIDE	0x00004000L	/* Is explicit override: LRO, RLO */

/* The following must be to make types pairwise different, some of them can
   be removed but are here because of efficiency (make queries faster). */

#define FRIBIDI_MASK_ES		0x00010000L
#define FRIBIDI_MASK_ET		0x00020000L
#define FRIBIDI_MASK_CS		0x00040000L

#define FRIBIDI_MASK_NSM	0x00080000L
#define FRIBIDI_MASK_BN		0x00100000L

#define FRIBIDI_MASK_BS		0x00200000L
#define FRIBIDI_MASK_SS		0x00400000L
#define FRIBIDI_MASK_WS		0x00800000L

/* We reserve the sign bit for user's private use: we will never use it,
   then negative character types will be never assigned. */


/*
 * Define values for FriBidiCharType
 */

/* Strong left to right */
#define FRIBIDI_TYPE_LTR_VAL	( FRIBIDI_MASK_STRONG + FRIBIDI_MASK_LETTER )
/* Right to left characters */
#define FRIBIDI_TYPE_RTL_VAL	( FRIBIDI_MASK_STRONG + FRIBIDI_MASK_LETTER \
				+ FRIBIDI_MASK_RTL)
/* Arabic characters */
#define FRIBIDI_TYPE_AL_VAL	( FRIBIDI_MASK_STRONG + FRIBIDI_MASK_LETTER \
				+ FRIBIDI_MASK_RTL + FRIBIDI_MASK_ARABIC )
/* Left-To-Right embedding */
#define FRIBIDI_TYPE_LRE_VAL	(FRIBIDI_MASK_STRONG + FRIBIDI_MASK_EXPLICIT)
/* Right-To-Left embedding */
#define FRIBIDI_TYPE_RLE_VAL	( FRIBIDI_MASK_STRONG + FRIBIDI_MASK_EXPLICIT \
				+ FRIBIDI_MASK_RTL )
/* Left-To-Right override */
#define FRIBIDI_TYPE_LRO_VAL	( FRIBIDI_MASK_STRONG + FRIBIDI_MASK_EXPLICIT \
				+ FRIBIDI_MASK_OVERRIDE )
/* Right-To-Left override */
#define FRIBIDI_TYPE_RLO_VAL	( FRIBIDI_MASK_STRONG + FRIBIDI_MASK_EXPLICIT \
				+ FRIBIDI_MASK_RTL + FRIBIDI_MASK_OVERRIDE )

/* Pop directional override */
#define FRIBIDI_TYPE_PDF_VAL	( FRIBIDI_MASK_WEAK + FRIBIDI_MASK_EXPLICIT )
/* European digit */
#define FRIBIDI_TYPE_EN_VAL	( FRIBIDI_MASK_WEAK + FRIBIDI_MASK_NUMBER )
/* Arabic digit */
#define FRIBIDI_TYPE_AN_VAL	( FRIBIDI_MASK_WEAK + FRIBIDI_MASK_NUMBER \
				+ FRIBIDI_MASK_ARABIC )
/* European number separator */
#define FRIBIDI_TYPE_ES_VAL	( FRIBIDI_MASK_WEAK + FRIBIDI_MASK_NUMSEPTER \
				+ FRIBIDI_MASK_ES )
/* European number terminator */
#define FRIBIDI_TYPE_ET_VAL	( FRIBIDI_MASK_WEAK + FRIBIDI_MASK_NUMSEPTER \
				+ FRIBIDI_MASK_ET )
/* Common Separator */
#define FRIBIDI_TYPE_CS_VAL	( FRIBIDI_MASK_WEAK + FRIBIDI_MASK_NUMSEPTER \
				+ FRIBIDI_MASK_CS )
/* Non spacing mark */
#define FRIBIDI_TYPE_NSM_VAL	( FRIBIDI_MASK_WEAK + FRIBIDI_MASK_NSM )
/* Boundary neutral */
#define FRIBIDI_TYPE_BN_VAL	( FRIBIDI_MASK_WEAK + FRIBIDI_MASK_SPACE \
				+ FRIBIDI_MASK_BN )

/* Block separator */
#define FRIBIDI_TYPE_BS_VAL	( FRIBIDI_MASK_NEUTRAL + FRIBIDI_MASK_SPACE \
				+ FRIBIDI_MASK_SEPARATOR + FRIBIDI_MASK_BS )
/* Segment separator */
#define FRIBIDI_TYPE_SS_VAL	( FRIBIDI_MASK_NEUTRAL + FRIBIDI_MASK_SPACE \
				+ FRIBIDI_MASK_SEPARATOR + FRIBIDI_MASK_SS )
/* Whitespace */
#define FRIBIDI_TYPE_WS_VAL	( FRIBIDI_MASK_NEUTRAL + FRIBIDI_MASK_SPACE \
				+ FRIBIDI_MASK_WS )
/* Other Neutral */
#define FRIBIDI_TYPE_ON_VAL	( FRIBIDI_MASK_NEUTRAL )


/* The following are used in specifying paragraph direction only. */

/* Weak left to right */
#define FRIBIDI_TYPE_WLTR_VAL	( FRIBIDI_MASK_WEAK )
/* Weak right to left */
#define FRIBIDI_TYPE_WRTL_VAL	( FRIBIDI_MASK_WEAK + FRIBIDI_MASK_RTL )

/* The following is only used internally */

/* Start or end of text (run list) */
#define FRIBIDI_TYPE_SENTINEL	( FRIBIDI_MASK_SENTINEL )

/* Define values for FriBidiCharType. */

#if defined(__C2MAN__) || (defined(FRIBIDI_SIZEOF_INT) && ((FRIBIDI_SIZEOF_INT <= 4) && (FRIBIDI_SIZEOF_INT >= 4)))

typedef enum
{
# define _FRIBIDI_ADD_TYPE(TYPE,SYMBOL) \
	FRIBIDI_TYPE_##TYPE = FRIBIDI_TYPE_##TYPE##_VAL,
# include "fribidi-bidi-types-list.h"
# undef _FRIBIDI_ADD_TYPE
  _FRIBIDI_TYPE_JUNK
} FriBidiCharType;

typedef enum
{
# define _FRIBIDI_PAR_TYPES
# define _FRIBIDI_ADD_TYPE(TYPE,SYMBOL) \
	FRIBIDI_PAR_##TYPE = FRIBIDI_TYPE_##TYPE##_VAL,
# include "fribidi-bidi-types-list.h"
# undef _FRIBIDI_ADD_TYPE
# undef _FRIBIDI_PAR_TYPES
  _FRIBIDI_TYPE_JUNK
} FriBidiParType;

#else

typedef fribidi_uint32 FriBidiCharType;
# define FRIBIDI_TYPE_LTR	FRIBIDI_TYPE_LTR_VAL
# define FRIBIDI_TYPE_RTL	FRIBIDI_TYPE_RTL_VAL
# define FRIBIDI_TYPE_AL	FRIBIDI_TYPE_AL_VAL
# define FRIBIDI_TYPE_EN	FRIBIDI_TYPE_EN_VAL
# define FRIBIDI_TYPE_AN	FRIBIDI_TYPE_AN_VAL
# define FRIBIDI_TYPE_ES	FRIBIDI_TYPE_ES_VAL
# define FRIBIDI_TYPE_ET	FRIBIDI_TYPE_ET_VAL
# define FRIBIDI_TYPE_CS	FRIBIDI_TYPE_CS_VAL
# define FRIBIDI_TYPE_NSM	FRIBIDI_TYPE_NSM_VAL
# define FRIBIDI_TYPE_BN	FRIBIDI_TYPE_BN_VAL
# define FRIBIDI_TYPE_BS	FRIBIDI_TYPE_BS_VAL
# define FRIBIDI_TYPE_SS	FRIBIDI_TYPE_SS_VAL
# define FRIBIDI_TYPE_WS	FRIBIDI_TYPE_WS_VAL
# define FRIBIDI_TYPE_ON	FRIBIDI_TYPE_ON_VAL
# define FRIBIDI_TYPE_LRE	FRIBIDI_TYPE_LRE_VAL
# define FRIBIDI_TYPE_RLE	FRIBIDI_TYPE_RLE_VAL
# define FRIBIDI_TYPE_LRO	FRIBIDI_TYPE_LRO_VAL
# define FRIBIDI_TYPE_RLO	FRIBIDI_TYPE_RLO_VAL
# define FRIBIDI_TYPE_PDF	FRIBIDI_TYPE_PDF_VAL

typedef fribidi_uint32 FriBidiParType;
# define FRIBIDI_PAR_LTR	FRIBIDI_TYPE_LTR_VAL
# define FRIBIDI_PAR_RTL	FRIBIDI_TYPE_RTL_VAL
# define FRIBIDI_PAR_ON		FRIBIDI_TYPE_ON_VAL
# define FRIBIDI_PAR_WLTR	FRIBIDI_TYPE_WLTR_VAL
# define FRIBIDI_PAR_WRTL	FRIBIDI_TYPE_WRTL_VAL

#endif

/* Just for compatibility */
#define FRIBIDI_TYPE_WLTR	FRIBIDI_PAR_WLTR
#define FRIBIDI_TYPE_WL		FRIBIDI_PAR_WLTR
#define FRIBIDI_TYPE_WRTL	FRIBIDI_PAR_WRTL
#define FRIBIDI_TYPE_WR		FRIBIDI_PAR_WRTL
#define FRIBIDI_TYPE_L		FRIBIDI_PAR_LTR
#define FRIBIDI_TYPE_R		FRIBIDI_PAR_RTL
#define FRIBIDI_TYPE_N		FRIBIDI_PAR_ON
#define FRIBIDI_TYPE_B		FRIBIDI_TYPE_BS
#define FRIBIDI_TYPE_S		FRIBIDI_TYPE_SS


/*
 * Defining macros for needed queries, It is fully dependent on the 
 * implementation of FriBidiCharType.
 */


/* Is private-use value? */
#define FRIBIDI_TYPE_PRIVATE(p)	((p) < 0)

/* Is right-to-left level? */
#define FRIBIDI_LEVEL_IS_RTL(lev) ((lev) & 1)

/* Return the direction of the level number, FRIBIDI_TYPE_LTR for even and
   FRIBIDI_TYPE_RTL for odds. */
#define FRIBIDI_LEVEL_TO_DIR(lev) (FRIBIDI_TYPE_LTR | ((lev) & 1))

/* Return the minimum level of the direction, 0 for FRIBIDI_TYPE_LTR and
   1 for FRIBIDI_TYPE_RTL and FRIBIDI_TYPE_AL. */
#define FRIBIDI_DIR_TO_LEVEL(dir) ((FriBidiLevel)((dir) & 1))

/* Is right to left? */
#define FRIBIDI_IS_RTL(p)      ((p) & FRIBIDI_MASK_RTL)
/* Is arabic? */
#define FRIBIDI_IS_ARABIC(p)   ((p) & FRIBIDI_MASK_ARABIC)

/* Is strong? */
#define FRIBIDI_IS_STRONG(p)   ((p) & FRIBIDI_MASK_STRONG)
/* Is weak? */
#define FRIBIDI_IS_WEAK(p)     ((p) & FRIBIDI_MASK_WEAK)
/* Is neutral? */
#define FRIBIDI_IS_NEUTRAL(p)  ((p) & FRIBIDI_MASK_NEUTRAL)
/* Is sentinel? */
#define FRIBIDI_IS_SENTINEL(p) ((p) & FRIBIDI_MASK_SENTINEL)

/* Is letter: L, R, AL? */
#define FRIBIDI_IS_LETTER(p)   ((p) & FRIBIDI_MASK_LETTER)
/* Is number: EN, AN? */
#define FRIBIDI_IS_NUMBER(p)   ((p) & FRIBIDI_MASK_NUMBER)
/* Is number separator or terminator: ES, ET, CS? */
#define FRIBIDI_IS_NUMBER_SEPARATOR_OR_TERMINATOR(p) \
	((p) & FRIBIDI_MASK_NUMSEPTER)
/* Is space: BN, BS, SS, WS? */
#define FRIBIDI_IS_SPACE(p)    ((p) & FRIBIDI_MASK_SPACE)
/* Is explicit mark: LRE, RLE, LRO, RLO, PDF? */
#define FRIBIDI_IS_EXPLICIT(p) ((p) & FRIBIDI_MASK_EXPLICIT)

/* Is test separator: BS, SS? */
#define FRIBIDI_IS_SEPARATOR(p) ((p) & FRIBIDI_MASK_SEPARATOR)

/* Is explicit override: LRO, RLO? */
#define FRIBIDI_IS_OVERRIDE(p) ((p) & FRIBIDI_MASK_OVERRIDE)

/* Some more: */

/* Is left to right letter: LTR? */
#define FRIBIDI_IS_LTR_LETTER(p) \
	((p) & (FRIBIDI_MASK_LETTER | FRIBIDI_MASK_RTL) == FRIBIDI_MASK_LETTER)

/* Is right to left letter: RTL, AL? */
#define FRIBIDI_IS_RTL_LETTER(p) \
	((p) & (FRIBIDI_MASK_LETTER | FRIBIDI_MASK_RTL) \
	== (FRIBIDI_MASK_LETTER | FRIBIDI_MASK_RTL))

/* Is ES or CS: ES, CS? */
#define FRIBIDI_IS_ES_OR_CS(p) \
	((p) & (FRIBIDI_MASK_ES | FRIBIDI_MASK_CS))

/* Is explicit or BN: LRE, RLE, LRO, RLO, PDF, BN? */
#define FRIBIDI_IS_EXPLICIT_OR_BN(p) \
	((p) & (FRIBIDI_MASK_EXPLICIT | FRIBIDI_MASK_BN))

/* Is explicit or BN or NSM: LRE, RLE, LRO, RLO, PDF, BN, NSM?
 * This is a good approximation of being zero-width. */
#define FRIBIDI_IS_EXPLICIT_OR_BN_OR_NSM(p) \
	((p) & (FRIBIDI_MASK_EXPLICIT | FRIBIDI_MASK_BN | FRIBIDI_MASK_NSM))

/* Is explicit or separator or BN or WS: LRE, RLE, LRO, RLO, PDF, BS, SS, BN, WS? */
#define FRIBIDI_IS_EXPLICIT_OR_SEPARATOR_OR_BN_OR_WS(p) \
	((p) & (FRIBIDI_MASK_EXPLICIT | FRIBIDI_MASK_SEPARATOR \
		| FRIBIDI_MASK_BN | FRIBIDI_MASK_WS))

/* Define some conversions. */

/* Change numbers: EN, AN to RTL. */
#define FRIBIDI_CHANGE_NUMBER_TO_RTL(p) \
	(FRIBIDI_IS_NUMBER(p) ? FRIBIDI_TYPE_RTL : (p))

/* Override status of an explicit mark:
 * LRO,LRE->LTR, RLO,RLE->RTL, otherwise->ON. */
#define FRIBIDI_EXPLICIT_TO_OVERRIDE_DIR(p) \
	(FRIBIDI_IS_OVERRIDE(p) ? FRIBIDI_LEVEL_TO_DIR(FRIBIDI_DIR_TO_LEVEL(p)) \
				: FRIBIDI_TYPE_ON)

/* Weaken type for paragraph fallback purposes:
 * LTR->WLTR, RTL->WRTL. */
#define FRIBIDI_WEAK_PARAGRAPH(p) (FRIBIDI_TYPE_WLTR | ((p) & 1))


/* Functions finally */

/* fribidi_type_name is the old name of fribidi_bidi_type_name */
#define fribidi_type_name fribidi_bidi_type_name

#define fribidi_bidi_type_name FRIBIDI_NAMESPACE(bidi_type_name)
/* fribidi_bidi_type_name - get bidi type name
 *
 * This function returns the bidi type name of a character type.  The
 * returned string is a static string and should not be freed.
 */
FRIBIDI_ENTRY const char *
fribidi_bidi_type_name (
  FriBidiCharType t		/* input bidi type */
) FRIBIDI_GNUC_CONST;

#include "fribidi-enddecls.h"

#endif /* !_FRIBIDI_BIDI_TYPES_H */
/* Editor directions:
 * vim:textwidth=78:tabstop=8:shiftwidth=2:autoindent:cindent
 */
