/* FriBidi
 * mem.h - memory manipulation routines
 *
 * $Id: fribidi-mem.c,v 1.1.1.1 2004-04-25 18:47:57 behdad Exp $
 * $Author: behdad $
 * $Date: 2004-04-25 18:47:57 $
 * $Revision: 1.1.1.1 $
 * $Source: /home/behdad/src/fdo/fribidi/togit/git/../fribidi/fribidi2/lib/fribidi-mem.c,v $
 *
 * Authors:
 *   Behdad Esfahbod, 2001, 2002, 2004
 *
 * Copyright (C) 2004 Sharif FarsiWeb, Inc
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

#include "mem.h"

#include "common.h"

#ifndef FRIBIDI_USE_GLIB

struct _FriBidiMemChunk
{
  int atom_size;
  int area_size;
  int empty_size;
  void *chunk;
};

FriBidiMemChunk *
fribidi_mem_chunk_new (
  /* input */
  const char *name,
  int atom_size,
  unsigned long area_size,
  int alloc_type
)
{
  FriBidiMemChunk *m =
    (FriBidiMemChunk *) fribidi_malloc (sizeof (FriBidiMemChunk));

  m->atom_size = atom_size;
  m->area_size = area_size;
  m->empty_size = 0;
  m->chunk = NULL;

  return m;
}

void *
fribidi_mem_chunk_alloc (
  /* input */
  FriBidiMemChunk *mem_chunk
)
{
  void *m;

  if (mem_chunk->empty_size < mem_chunk->atom_size)
    {
      mem_chunk->chunk = fribidi_malloc (mem_chunk->area_size);
      mem_chunk->empty_size = mem_chunk->area_size;
    }

  m = mem_chunk->chunk;
  mem_chunk->chunk = (void *)
    ((char *) mem_chunk->chunk + mem_chunk->atom_size);
  mem_chunk->empty_size -= mem_chunk->atom_size;

  return m;
}

#endif /* !FRIBIDI_USE_GLIB */

/* Editor directions:
 * vim:textwidth=78:tabstop=8:shiftwidth=2:autoindent:cindent
 */
