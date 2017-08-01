/* fribidi-brackets.h - get bracket character property
 *
 * Copyright (C) 2004  Sharif FarsiWeb, Inc
 * Copyright (C) 2001, 2002, 2004  Behdad Esfahbod
 * Copyright (C) 1999, 2000, 2017  Dov Grobgeld
 * 
 * This file is part of GNU FriBidi.
 * 
 * GNU FriBidi is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1
 * of the License, or (at your option) any later version.
 * 
 * GNU FriBidi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with GNU FriBidi; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 * 
 * For licensing issues, contact <fribidi.license@gmail.com> or write to
 * Sharif FarsiWeb, Inc., PO Box 13445-389, Tehran, Iran.
 */
/* $Id: fribidi-brackets.h,v 1.10 2004-09-28 07:58:57 behdad Exp $
 * $Author: behdad $
 * $Date: 2004-09-28 07:58:57 $
 * $Revision: 1.10 $
 * $Source: /home/behdad/src/fdo/fribidi/togit/git/../fribidi/fribidi2/lib/fribidi-brackets.h,v $
 *
 * Author(s):
 *   Behdad Esfahbod, 2001, 2002, 2004
 *   Dov Grobgeld, 1999, 2000, 2017
 */
#ifndef _FRIBIDI_BRACKETS_H
#define _FRIBIDI_BRACKETS_H

#include "fribidi-common.h"

#include "fribidi-types.h"
#include "fribidi-bidi-types.h"

#include "fribidi-begindecls.h"

#define fribidi_get_bracket_char FRIBIDI_NAMESPACE(get_bracket_char)
/* fribidi_get_bracket_char - get bracketed character
 *
 * This function finds the bracketed equivalent of a character as defined in
 * the file BidiBrackets.txt of the Unicode Character Database available at
 * http://www.unicode.org/Public/UNIDATA/BidiBrackets.txt.
 *
 * If  the input character is a declared as a brackets character in the
 * Unicode standard and has a bracketed equivalent.  The matching bracketed
 * character is put in the output, otherwise the input character itself is
 * put.
 *
 * Returns: The bracket type of the character. Use the
 * FRIBIDI_IS_BRACKET(FriBidiBracketType) to test if it is a valid
 * property.
 */
FRIBIDI_ENTRY FriBidiBracketType fribidi_get_bracket (
  FriBidiChar ch		    /* input character */
);

FRIBIDI_ENTRY void
fribidi_get_bracket_types (
  /* input */
  const FriBidiChar *str,
  const FriBidiStrIndex len,
  const FriBidiType *types,
  /* output */
  FriBidiBracketType *btypes
);

#define FRIBIDI_IS_BRACKET(bt) ((bt)->bracket_id>0)
#define FRIBIDI_IS_BRACKET_OPEN(bt) ((bt)->is_open>0)

#include "fribidi-enddecls.h"

#endif /* !_FRIBIDI_BRACKETS_H */
/* Editor directions:
 * Local Variables:
 *   mode: c
 *   c-basic-offset: 2
 *   indent-tabs-mode: t
 *   tab-width: 8
 * End:
 * vim: textwidth=78: autoindent: cindent: shiftwidth=2: tabstop=8:
 */
