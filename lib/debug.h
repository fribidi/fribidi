/* FriBidi
 * debug.h - debug-only interfaces
 *
 * $Id: debug.h,v 1.1.1.1 2004-04-25 18:47:57 behdad Exp $
 * $Author: behdad $
 * $Date: 2004-04-25 18:47:57 $
 * $Revision: 1.1.1.1 $
 * $Source: /home/behdad/src/fdo/fribidi/togit/git/../fribidi/fribidi2/lib/debug.h,v $
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
#ifndef _DEBUG_H
#define _DEBUG_H

#include <fribidi-common.h>

#include <fribidi-types.h>

#include "common.h"

#include <fribidi-begindecls.h>

#if DEBUG

#ifndef FRIBIDI_FPRINTF
# if !__C2MAN__
#  include <stdio.h>
# endif	/* !__C2MAN__ */
# define FRIBIDI_FPRINTF fprintf
# define FRIBIDI_STDERR_ stderr,
#endif /* !FRIBIDI_FPRINTF */

#define MSG(s) FRIBIDI_BEGIN_STMT \
		FRIBIDI_FPRINTF(FRIBIDI_STDERR_ s); \
	FRIBIDI_END_STMT

#define MSG2(s, t) FRIBIDI_BEGIN_STMT \
		FRIBIDI_FPRINTF(FRIBIDI_STDERR_ s, t); \
	FRIBIDI_END_STMT

#define MSG5(s, t, u, v, w) FRIBIDI_BEGIN_STMT \
		FRIBIDI_FPRINTF(FRIBIDI_STDERR_ s, t, u, v, w); \
	FRIBIDI_END_STMT

#ifndef DBG
# define DBG(s) FRIBIDI_BEGIN_STMT \
	if (fribidi_debug_status()) MSG(FRIBIDI ": " s "\n"); \
	FRIBIDI_END_STMT
# define DBG2(s, t) FRIBIDI_BEGIN_STMT \
	if (fribidi_debug_status()) MSG2(FRIBIDI ": " s "\n", t); \
	FRIBIDI_END_STMT
#endif /* !DBG */

#else /* !DEBUG */

#define DBG(s)			/* empty */
#define DBG2(s, t)		/* empty */

#endif /* !DEBUG */

#include <fribidi-enddecls.h>

#endif /* !_DEBUG_H */
/* Editor directions:
 * vim:textwidth=78:tabstop=8:shiftwidth=2:autoindent:cindent
 */
