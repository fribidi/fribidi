/* FriBidi
 * fribidi-types.h - define data types for the rest of the library
 *
 * $Id: fribidi-types.h,v 1.4 2004-05-07 06:30:38 behdad Exp $
 * $Author: behdad $
 * $Date: 2004-05-07 06:30:38 $
 * $Revision: 1.4 $
 * $Source: /home/behdad/src/fdo/fribidi/togit/git/../fribidi/fribidi2/lib/fribidi-types.h,v $
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
#ifndef _FRIBIDI_TYPES_H
#define _FRIBIDI_TYPES_H

#include "fribidi-common.h"

#include "fribidi-begindecls.h"


#if !FRIBIDI_USE_GLIB
# if HAVE_INTTYPES_H
#  ifndef __C2MAN__
#   include <inttypes.h>
#  endif /* !__C2MAN__ */
#  define FRIBIDI_UINT8_LOCAL		uint8_t
#  define FRIBIDI_UINT16_LOCAL		uint16_t
#  define FRIBIDI_UINT32_LOCAL		uint32_t
# else /* !HAVE_INTTYPES_H */
#  if HAVE_STDINT_H
#   ifndef __C2MAN__
#    include <stdint.h>
#   endif /* !__C2MAN__ */
#   define FRIBIDI_UINT8_LOCAL		uint8_t
#   define FRIBIDI_UINT16_LOCAL		uint16_t
#   define FRIBIDI_UINT32_LOCAL		uint32_t
#  else	/* !HAVE_STDINT_H */
#   define FRIBIDI_UINT8_LOCAL		unsigned char
#   if !defined(SIZEOF_SHORT) || SIZEOF_SHORT >= 2
#    define FRIBIDI_UINT16_LOCAL	unsigned short
#   else /* SIZEOF_SHORT < 2 */
#    define FRIBIDI_UINT16_LOCAL	unsigned int
#   endif /* SIZEOF_SHORT < 2 */
#   if !defined(SIZEOF_INT) || SIZEOF_INT >= 4
#    define FRIBIDI_UINT32_LOCAL	unsigned int
#   else /* SIZEOF_INT < 4 */
#    define FRIBIDI_UINT32_LOCAL	unsigned long
#   endif /* SIZEOF_INT < 4 */
#  endif /* !HAVE_STDINT_H */
# endif	/* !HAVE_INTTYPES_H */
# if HAVE_STDBOOL_H
#  ifndef __C2MAN__
#   include <stdbool.h>
#  endif /* !__C2MAN__ */
#  define FRIBIDI_BOOLEAN_LOCAL		bool
# else /* !HAVE_STDBOOL_H */
#  define FRIBIDI_BOOLEAN_LOCAL		int
# endif	/* !HAVE_STDBOOL_H */
# if SIZEOF_WCHAR_T >= 4
#  ifndef __C2MAN__
#   if STDC_HEADERS
#    include <stdlib.h>
#    include <stddef.h>
#   else /* !STDC_HEADERS */
#    if HAVE_STDLIB_H
#     include <stdlib.h>
#    endif /* !HAVE_STDLIB_H */
#   endif /* !STDC_HEADERS */
#  endif /* !__C2MAN__ */
#  define FRIBIDI_UNICHAR_LOCAL		wchar_t
# else /* SIZEOF_WCHAR_T < 4 */
#  define FRIBIDI_UNICHAR_LOCAL		fribidi_uint32
# endif	/* SIZEOF_WCHAR_T < 4 */
#else /* FRIBIDI_USE_GLIB */
#  ifndef __C2MAN__
#   include <glib/gtypes.h>
#   include <glib/gunicode.h>
#  endif /* !__C2MAN__ */
# define FRIBIDI_UINT8_LOCAL		guint8
# define FRIBIDI_UINT16_LOCAL		guint16
# define FRIBIDI_UINT32_LOCAL		guint32
# define FRIBIDI_BOOLEAN_LOCAL		gboolean
# define FRIBIDI_UNICHAR_LOCAL		gunichar
#endif /* FRIBIDI_USE_GLIB */

#ifndef FRIBIDI_UINT8
# define FRIBIDI_UINT8	FRIBIDI_UINT8_LOCAL
#endif /* !FRIBIDI_UINT8 */
#ifndef FRIBIDI_UINT16
# define FRIBIDI_UINT16	FRIBIDI_UINT16_LOCAL
#endif /* !FRIBIDI_UINT16 */
#ifndef FRIBIDI_UINT32
# define FRIBIDI_UINT32	FRIBIDI_UINT32_LOCAL
#endif /* !FRIBIDI_UINT32 */
#ifndef FRIBIDI_BOOLEAN
# define FRIBIDI_BOOLEAN	FRIBIDI_BOOLEAN_LOCAL
#endif /* !FRIBIDI_BOOLEAN */
#ifndef FRIBIDI_UNICHAR
# define FRIBIDI_UNICHAR FRIBIDI_UNICHAR_LOCAL
#endif /* !FRIBIDI_UNICHAR */
#ifndef FRIBIDI_STR_INDEX
# define FRIBIDI_STR_INDEX int
#endif /* FRIBIDI_STR_INDEX */


typedef FRIBIDI_UINT8 fribidi_uint8;
typedef FRIBIDI_UINT16 fribidi_uint16;
typedef FRIBIDI_UINT32 fribidi_uint32;
typedef FRIBIDI_BOOLEAN fribidi_boolean;

typedef FRIBIDI_UNICHAR FriBidiChar;
typedef FRIBIDI_STR_INDEX FriBidiStrIndex;


#ifndef FRIBIDI_MAX_STRING_LENGTH
# define FRIBIDI_MAX_STRING_LENGTH (sizeof (FriBidiStrIndex) == 2 ?	\
		0x7FFE : (sizeof (FriBidiStrIndex) == 1 ? \
		0x7E : 0x8FFFFFFEL))
#endif

#include "fribidi-enddecls.h"

#endif /* !_FRIBIDI_TYPES_H */
/* Editor directions:
 * vim:textwidth=78:tabstop=8:shiftwidth=2:autoindent:cindent
 */
