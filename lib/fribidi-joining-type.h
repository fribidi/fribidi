/* FriBidi
 * fribidi-joining-type.h - get character joining type
 *
 * $Id: fribidi-joining-type.h,v 1.1 2004-06-13 20:11:42 behdad Exp $
 * $Author: behdad $
 * $Date: 2004-06-13 20:11:42 $
 * $Revision: 1.1 $
 * $Source: /home/behdad/src/fdo/fribidi/togit/git/../fribidi/fribidi2/lib/Attic/fribidi-joining-type.h,v $
 *
 * Author:
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
#ifndef _FRIBIDI_JOINING_TYPE_H
#define _FRIBIDI_JOINING_TYPE_H

#include "fribidi-common.h"

#include "fribidi-types.h"
#include "fribidi-joining-types.h"

#include "fribidi-begindecls.h"

#define fribidi_get_joining_type FRIBIDI_NAMESPACE(get_joining_type)
/* fribidi_get_joining_type - get character joining type
 *
 * This function returns the joining type of a character.  There are a few
 * macros defined in fribidi-joining-types.h for querying a joining type.
 */
FRIBIDI_ENTRY FriBidiJoiningType
fribidi_get_joining_type (
  FriBidiChar ch		/* input character */
) FRIBIDI_GNUC_CONST;

#include "fribidi-enddecls.h"

#endif /* !_FRIBIDI_JOINING_TYPE_H */
/* Editor directions:
 * vim:textwidth=78:tabstop=8:shiftwidth=2:autoindent:cindent
 */
