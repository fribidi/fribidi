/* FriBidi
 * fribidi-run.c - text run data type
 *
 * $Id: fribidi-run.c,v 1.1 2004-04-28 02:37:56 behdad Exp $
 * $Author: behdad $
 * $Date: 2004-04-28 02:37:56 $
 * $Revision: 1.1 $
 * $Source: /home/behdad/src/fdo/fribidi/togit/git/../fribidi/fribidi2/lib/fribidi-run.c,v $
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

#include "run.h"
#include "env.h"
#include "bidi-types.h"

#include "common.h"

FriBidiRun *
new_run (
  void
)
{
  FriBidiRun *run;

#if USE_SIMPLE_MALLOC
  run = fribidi_malloc (sizeof (FriBidiRun));
#else /* !USE_SIMPLE_MALLOC */
  if (free_runs)
    {
      run = free_runs;
      free_runs = free_runs->next;
    }
  else
    {
      if (!run_mem_chunk)
	run_mem_chunk = fribidi_chunk_new_for_type (FriBidiRun);

      run = fribidi_chunk_new (FriBidiRun, run_mem_chunk);
    }
#endif /* !USE_SIMPLE_MALLOC */

  run->len = 0;
  run->pos = 0;
  run->level = 0;
  run->next = NULL;
  run->prev = NULL;
  return run;
}

void
free_run (
  FriBidiRun * run
)
{
#if USE_SIMPLE_MALLOC
  fribidi_free (run);
#else /* !USE_SIMPLE_MALLOC */
  run->next = free_runs;
  free_runs = run;
#endif /* !USE_SIMPLE_MALLOC */
}

#define FRIBIDI_ADD_TYPE_LINK(p,q) \
	FRIBIDI_BEGIN_STMT	\
		(p)->len = (q)->pos - (p)->pos;	\
		(p)->next = (q);	\
		(q)->prev = (p);	\
		(p) = (q);	\
	FRIBIDI_END_STMT

FriBidiRun *
run_list_encode_bidi_types (
  FriBidiCharType *char_type,
  FriBidiStrIndex len
)
{
  FriBidiRun *list, *last, *run;

  FriBidiStrIndex i;

  /* Add the starting run */
  list = new_run ();
  list->type = FRIBIDI_TYPE_SOT;
  list->level = FRIBIDI_LEVEL_START;
  last = list;

  /* Sweep over the string_type s */
  for (i = 0; i < len; i++)
    if (char_type[i] != last->type)
      {
	run = new_run ();
	run->type = char_type[i];
	run->pos = i;
	FRIBIDI_ADD_TYPE_LINK (last, run);
      }

  /* Add the ending run */
  run = new_run ();
  run->type = FRIBIDI_TYPE_EOT;
  run->level = FRIBIDI_LEVEL_END;
  run->pos = len;
  FRIBIDI_ADD_TYPE_LINK (last, run);

  return list;
}

/* move a run before another element in a list, the list must have a
   previous element, used to update explicits_list.
   assuming that p have both prev and next or none of them, also update
   the list that p is currently in, if any.
*/
void
move_run_before (
  FriBidiRun * p,
  FriBidiRun * list
)
{
  if (p->prev)
    {
      p->prev->next = p->next;
      p->next->prev = p->prev;
    }
  p->prev = list->prev;
  list->prev->next = p;
  p->next = list;
  list->prev = p;
}

/* override the run list 'base', with the runs in the list 'over', to
   reinsert the previously-removed explicit codes (at X9) from
   'explicits_list' back into 'type_rl_list' for example. This is used at the
   end of I2 to restore the explicit marks, and also to reset the character
   types of characters at L1.

   it is assumed that the 'pos' of the first element in 'base' list is not
   more than the 'pos' of the first element of the 'over' list, and the
   'pos' of the last element of the 'base' list is not less than the 'pos'
   of the last element of the 'over' list. these two conditions are always
   satisfied for the two usages mentioned above.

   Todo:
     use some explanatory names instead of p, q, ...
     rewrite comment above to remove references to special usage.
*/
void
shadow_run_list (
  FriBidiRun * base,
  FriBidiRun * over
)
{
  FriBidiRun *p = base, *q, *r, *s, *t;
  FriBidiStrIndex pos = 0, pos2;

  if (!over)
    return;
  q = over;
  while (q)
    {
      if (!q->len || q->pos < pos)
	{
	  t = q;
	  q = q->next;
	  free_run (t);
	  continue;
	}
      pos = q->pos;
      while (p->next && p->next->pos <= pos)
	p = p->next;
      /* now p is the element that q must be inserted 'in'. */
      pos2 = pos + q->len;
      r = p;
      while (r->next && r->next->pos < pos2)
	r = r->next;
      /* now r is the last element that q affects. */
      if (p == r)
	{
	  /* split p into at most 3 interval, and insert q in the place of
	     the second interval, set r to be the third part. */
	  /* third part needed? */
	  if (p->next && p->next->pos == pos2)
	    r = r->next;
	  else
	    {
	      r = new_run ();
	      *r = *p;
	      if (r->next)
		{
		  r->next->prev = r;
		  r->len = r->next->pos - pos2;
		}
	      else
		r->len -= pos - p->pos;
	      r->pos = pos2;
	    }
	  /* first part needed? */
	  if (p->prev && p->pos == pos)
	    {
	      t = p;
	      p = p->prev;
	      free_run (t);
	    }
	  else
	    p->len = pos - p->pos;
	}
      else
	{
	  /* cut the end of p. */
	  p->len = pos - p->pos;
	  /* if all of p is cut, remove it. */
	  if (!p->len && p->prev)
	    p = p->prev;

	  /* cut the begining of r. */
	  r->pos = pos2;
	  if (r->next)
	    r->len = r->next->pos - pos2;
	  /* if all of r is cut, remove it. */
	  if (!r->len && r->next)
	    r = r->next;

	  /* remove the elements between p and r. */
	  for (s = p->next; s != r;)
	    {
	      t = s;
	      s = s->next;
	      free_run (t);
	    }
	}
      /* before updating the next and prev runs to point to the inserted q,
         we must remember the next element of q in the 'over' list.
       */
      t = q;
      q = q->next;
      p->next = t;
      t->prev = p;
      t->next = r;
      r->prev = t;
    }
}

/* Editor directions:
 * vim:textwidth=78:tabstop=8:shiftwidth=2:autoindent:cindent
 */
