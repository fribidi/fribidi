#ifndef __C2MAN__
/* FriBidi
 * fribidi-bidi-types-list.h - list of bidi types
 *
 * $Id: fribidi-bidi-types-list.h,v 1.1 2004-05-31 18:43:26 behdad Exp $
 * $Author: behdad $
 * $Date: 2004-05-31 18:43:26 $
 * $Revision: 1.1 $
 * $Source: /home/behdad/src/fdo/fribidi/togit/git/../fribidi/fribidi2/lib/fribidi-bidi-types-list.h,v $
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
/* *INDENT-OFF* */
#endif /* !__C2MAN__ */
#ifndef _FRIBIDI_ADD_TYPE
# define _FRIBIDI_ADD_TYPE(x,y)
#endif
#ifndef _FRIBIDI_ADD_ALIAS
# define _FRIBIDI_ADD_ALIAS(x1,x2)
#endif

#if !_FRIBIDI_PAR_TYPES
/* Bidi types from the standard. */
/* The order of these types is important.  Don't change. */
_FRIBIDI_ADD_TYPE (LTR, 'L')	/* Left-To-Right letter */
_FRIBIDI_ADD_TYPE (RTL, 'R')	/* Right-To-Left letter */
_FRIBIDI_ADD_TYPE (AL, 'A')	/* Arabic Letter */
_FRIBIDI_ADD_TYPE (EN, '1')	/* European Numeral */
_FRIBIDI_ADD_TYPE (AN, '9')	/* Arabic Numeral */
_FRIBIDI_ADD_TYPE (ES, 'w')	/* European number Separator */
_FRIBIDI_ADD_TYPE (ET, 'w')	/* European number Terminator */
_FRIBIDI_ADD_TYPE (CS, 'w')	/* Common Separator */
_FRIBIDI_ADD_TYPE (NSM, '`')	/* Non Spacing Mark */
_FRIBIDI_ADD_TYPE (BN, 'b')	/* Boundary Neutral */
_FRIBIDI_ADD_TYPE (BS, 'B')	/* Block Separator */
_FRIBIDI_ADD_TYPE (SS, 'S')	/* Segment Separator */
_FRIBIDI_ADD_TYPE (WS, '_')	/* White-Space */
_FRIBIDI_ADD_TYPE (ON, 'n')	/* Other Neutral */
_FRIBIDI_ADD_TYPE (LRE, '+')	/* Left-to-Right Embedding */
_FRIBIDI_ADD_TYPE (RLE, '+')	/* Right-to-Left Embedding */
_FRIBIDI_ADD_TYPE (LRO, '+')	/* Left-to-Right Override */
_FRIBIDI_ADD_TYPE (RLO, '+')	/* Right-to-Left Override */
_FRIBIDI_ADD_TYPE (PDF, '-')	/* Pop Directional Flag */

#ifdef _FRIBIDI_ADD_ALIAS
/* The followings are just aliases to types, but with the name that appears in
 * the Unicode database. */
_FRIBIDI_ADD_ALIAS (L, LTR)
_FRIBIDI_ADD_ALIAS (R, RTL)
_FRIBIDI_ADD_ALIAS (B, BS)
_FRIBIDI_ADD_ALIAS (S, SS)
#endif /* _FRIBIDI_ADD_ALIAS */

#ifdef _FRIBIDI_SENTINEL_TYPE
_FRIBIDI_ADD_TYPE (SENTINEL, '$')	/* Sentinel */
#endif /* _FRIBIDI_SENTINEL_TYPES */
#endif /* !_FRIBIDI_PAR_TYPES */

#if _FRIBIDI_PAR_TYPES
_FRIBIDI_ADD_TYPE (LTR, 'L')	/* Left-To-Right paragraph */
_FRIBIDI_ADD_TYPE (RTL, 'R')	/* Right-To-Left paragraph */
_FRIBIDI_ADD_TYPE (WLTR, 'l')	/* Weak left to right paragraph */
_FRIBIDI_ADD_TYPE (WRTL, 'r')	/* Weak right to left paragraph */
_FRIBIDI_ADD_TYPE (ON, 'n')	/* Direction-neutral paragraph */
#endif /* _FRIBIDI_PAR_TYPES */

#ifndef __C2MAN__
/* *INDENT-ON* */
#endif /* !__C2MAN__ */
