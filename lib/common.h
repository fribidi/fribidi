/* FriBidi
 * common.h - common include for library sources
 *
 * $Id: common.h,v 1.3 2004-04-27 23:53:43 behdad Exp $
 * $Author: behdad $
 * $Date: 2004-04-27 23:53:43 $
 * $Revision: 1.3 $
 * $Source: /home/behdad/src/fdo/fribidi/togit/git/../fribidi/fribidi2/lib/common.h,v $
 *
 * Author:
 *   Behdad Esfahbod, 2004
 *
 * Copyright (C) 2004 Sharif FarsiWeb, Inc.
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
#ifndef _COMMON_H
#define _COMMON_H

#if HAVE_CONFIG_H
# include <config.h>
#endif

#include <fribidi-common.h>

#if WIN32
# define FRIBIDI_ENTRY __declspec(dllexport)
#endif /* WIN32 */

#ifndef FALSE
# define FALSE (0==1)
#endif /* !FALSE */

#ifndef TRUE
# define TRUE (!FALSE)
#endif /* !TRUE */

#ifndef NULL
# define NULL (void *) 0
#endif /* !NULL */

#if FRIBIDI_USE_GLIB
# ifndef SIZEOF_LONG
#  define SIZEOF_LONG GLIB_SIZEOF_LONG
# endif	/* !SIZEOF_LONG */
# ifndef SIZEOF_VOID_P
#  define SIZEOF_VOID_P GLIB_SIZEOF_VOID_P
# endif	/* !SIZEOF_VOID_P */
# ifndef __C2MAN__
#  include <glib/gmem.h>
# endif	/* !__C2MAN__ */
# ifndef fribidi_malloc
#  define fribidi_malloc g_malloc
#  define fribidi_free g_free
# endif	/* !fribidi_malloc */
#endif /* FRIBIDI_USE_GLIB */

/* fribidi_malloc and fribidi_free should be used instead of malloc and free. 
 * No need to include any headers. */
#ifndef fribidi_malloc
# if HAVE_STDLIB_H
#  ifndef __C2MAN__
#   include <stdlib.h>
#  endif /* __C2MAN__ */
#  define fribidi_malloc malloc
# else /* !HAVE_STDLIB_H */
#  define fribidi_malloc (void *) malloc
# endif	/* !HAVE_STDLIB_H */
# define fribidi_free free
#endif /* !fribidi_malloc */

/* FRIBIDI_CHUNK_SIZE is the number of bytes in each chunk of memory being
 * allocated for data structure pools. */
#ifndef FRIBIDI_CHUNK_SIZE
# define FRIBIDI_CHUNK_SIZE 4096
#else /* FRIBIDI_CHUNK_SIZE */
# if FRIBIDI_CHUNK_SIZE < 256
#  error FRIBIDI_CHUNK_SIZE now should define the size of a chunk in bytes.
# endif	/* FRIBIDI_CHUNK_SIZE < 256 */
#endif /* FRIBIDI_CHUNK_SIZE */

/* FRIBIDI_BEGIN_STMT should be used at the beginning of your macro
 * definitions that are to behave like simple statements.  Use
 * FRIBIDI_END_STMT at the end of the macro after the semicolon or brace */
#ifndef FRIBIDI_BEGIN_STMT
# define FRIBIDI_BEGIN_STMT do {
# define FRIBIDI_END_STMT } while (0)
#endif /* !FRIBIDI_BEGIN_STMT */

#include "debug.h"

#endif /* !_COMMON_H */
/* Editor directions:
 * vim:textwidth=78:tabstop=8:shiftwidth=2:autoindent:cindent
 */
