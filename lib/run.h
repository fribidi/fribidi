/* FriBidi
 * run.h - text run data type
 *
 * $Id: run.h,v 1.1 2004-04-28 02:37:56 behdad Exp $
 * $Author: behdad $
 * $Date: 2004-04-28 02:37:56 $
 * $Revision: 1.1 $
 * $Source: /home/behdad/src/fdo/fribidi/togit/git/../fribidi/fribidi2/lib/run.h,v $
 *
 * Authors:
 *   Behdad Esfahbod, 2001, 2002, 2004
 *   Dov Grobgeld, 1999, 2000
 *
 * Copyright (C) 2004 Sharif FarsiWeb, Inc
 * Copyright (C) 2001,2002 Behdad Esfahbod
 * Copyright (C) 1999,2000 Dov Grobgeld
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
#ifndef _RUN_H
#define _RUN_H

#include <fribidi-common.h>

#include <fribidi-types.h>
#include <fribidi-bidi-types.h>

#include "common.h"

#include <fribidi-begindecls.h>

struct _FriBidiRun
{
  FriBidiRun *prev;
  FriBidiRun *next;

  FriBidiCharType type;
  FriBidiStrIndex pos, len;
  FriBidiLevel level;
};


FriBidiRun *new_run (
  void
);

void free_run (
  FriBidiRun * run
);

FriBidiRun *run_list_encode_bidi_types (
  FriBidiCharType *char_type,
  FriBidiStrIndex len
);

void move_run_before (
  FriBidiRun * p,
  FriBidiRun * list
);

void shadow_run_list (
  FriBidiRun * base,
  FriBidiRun * over
);

#include <fribidi-enddecls.h>

#endif /* !_MEM_H */
/* Editor directions:
 * vim:textwidth=78:tabstop=8:shiftwidth=2:autoindent:cindent
 */
