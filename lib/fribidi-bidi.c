/* FriBidi
 * fribidi-bidi.c - bidirectional algorithm
 *
 * $Id: fribidi-bidi.c,v 1.3 2004-04-25 19:12:42 behdad Exp $
 * $Author: behdad $
 * $Date: 2004-04-25 19:12:42 $
 * $Revision: 1.3 $
 * $Source: /home/behdad/src/fdo/fribidi/togit/git/../fribidi/fribidi2/lib/fribidi-bidi.c,v $
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

#include <fribidi-bidi.h>
#include <fribidi-bidi-type.h>
#include <fribidi-mirroring.h>
#include <fribidi-unicode.h>
#include <fribidi-env.h>

#include "mem.h"
#include "bidi-types.h"

#include "common.h"

#undef MAX
#define MAX(a,b) ((a) > (b) ? (a) : (b))

/*======================================================================
 * Typedef for the run-length list.
 *----------------------------------------------------------------------*/
typedef struct _TypeLink TypeLink;

struct _TypeLink
{
  TypeLink *prev;
  TypeLink *next;

  FriBidiCharType type;
  FriBidiStrIndex pos, len;
  FriBidiLevel level;
};

#define FRIBIDI_LEVEL_REMOVED -3
#define FRIBIDI_LEVEL_START   -2
#define FRIBIDI_LEVEL_END     -1

typedef struct
{
  FriBidiCharType override;	/* only LTR, RTL and ON are valid */
  FriBidiLevel level;
}
LevelInfo;

static void
bidi_string_reverse (
  FriBidiChar *str,
  FriBidiStrIndex len
)
{
  FriBidiStrIndex i;
  for (i = 0; i < len / 2; i++)
    {
      FriBidiChar tmp = str[i];
      str[i] = str[len - 1 - i];
      str[len - 1 - i] = tmp;
    }
}

static void
index_array_reverse (
  FriBidiStrIndex *arr,
  FriBidiStrIndex len
)
{
  FriBidiStrIndex i;
  for (i = 0; i < len / 2; i++)
    {
      FriBidiStrIndex tmp = arr[i];
      arr[i] = arr[len - 1 - i];
      arr[len - 1 - i] = tmp;
    }
}

#if !USE_SIMPLE_MALLOC
static TypeLink *free_type_links = NULL;
#endif /* !USE_SIMPLE_MALLOC */

static TypeLink *
new_type_link (
  void
)
{
  TypeLink *link;

#if USE_SIMPLE_MALLOC
  link = fribidi_malloc (sizeof (TypeLink));
#else /* !USE_SIMPLE_MALLOC */
  if (free_type_links)
    {
      link = free_type_links;
      free_type_links = free_type_links->next;
    }
  else
    {
      static FriBidiMemChunk *mem_chunk = NULL;

      if (!mem_chunk)
	mem_chunk = fribidi_chunk_new_for_type (TypeLink
	);

      link = fribidi_chunk_new (TypeLink,
				mem_chunk
      );
    }
#endif /* !USE_SIMPLE_MALLOC */

  link->len = 0;
  link->pos = 0;
  link->level = 0;
  link->next = NULL;
  link->prev = NULL;
  return link;
}

static void
free_type_link (
  TypeLink *link
)
{
#if USE_SIMPLE_MALLOC
  fribidi_free (link);
#else /* !USE_SIMPLE_MALLOC */
  link->next = free_type_links;
  free_type_links = link;
#endif /* !USE_SIMPLE_MALLOC */
}

#define FRIBIDI_ADD_TYPE_LINK(p,q) \
	FRIBIDI_BEGIN_STMT	\
		(p)->len = (q)->pos - (p)->pos;	\
		(p)->next = (q);	\
		(q)->prev = (p);	\
		(p) = (q);	\
	FRIBIDI_END_STMT

static TypeLink *
run_length_encode_types (
  FriBidiCharType *char_type,
  FriBidiStrIndex type_len
)
{
  TypeLink *list, *last, *link;

  FriBidiStrIndex i;

  /* Add the starting link */
  list = new_type_link ();
  list->type = FRIBIDI_TYPE_SOT;
  list->level = FRIBIDI_LEVEL_START;
  last = list;

  /* Sweep over the string_type s */
  for (i = 0; i < type_len; i++)
    if (char_type[i] != last->type)
      {
	link = new_type_link ();
	link->type = char_type[i];
	link->pos = i;
	FRIBIDI_ADD_TYPE_LINK (last, link);
      }

  /* Add the ending link */
  link = new_type_link ();
  link->type = FRIBIDI_TYPE_EOT;
  link->level = FRIBIDI_LEVEL_END;
  link->pos = type_len;
  FRIBIDI_ADD_TYPE_LINK (last, link);

  return list;
}

/* explicits_list is a list like type_rl_list, that holds the explicit
   codes that are removed from rl_list, to reinsert them later by calling
   the override_list.
*/
static void
init_list (
  TypeLink **start,
  TypeLink **end
)
{
  TypeLink *list;
  TypeLink *link;

  /* Add the starting link */
  list = new_type_link ();
  list->type = FRIBIDI_TYPE_SOT;
  list->level = FRIBIDI_LEVEL_START;
  list->len = 0;
  list->pos = 0;

  /* Add the ending link */
  link = new_type_link ();
  link->type = FRIBIDI_TYPE_EOT;
  link->level = FRIBIDI_LEVEL_END;
  link->len = 0;
  link->pos = 0;
  list->next = link;
  link->prev = list;

  *start = list;
  *end = link;
}

/* move an element before another element in a list, the list must have a
   previous element, used to update explicits_list.
   assuming that p have both prev and next or none of them, also update
   the list that p is currently in, if any.
*/
static void
move_element_before (
  TypeLink *p,
  TypeLink *list
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

/* override the rl_list 'base', with the elements in the list 'over', to
   reinsert the previously-removed explicit codes (at X9) from
   'explicits_list' back into 'type_rl_list'. This is used at the end of I2
   to restore the explicit marks, and also to reset the character types of
   characters at L1.

   it is assumed that the 'pos' of the first element in 'base' list is not
   more than the 'pos' of the first element of the 'over' list, and the
   'pos' of the last element of the 'base' list is not less than the 'pos'
   of the last element of the 'over' list. these two conditions are always
   satisfied for the two usages mentioned above.

   TBD: use some explanatory names instead of p, q, ...
*/
static void
override_list (
  TypeLink *base,
  TypeLink *over
)
{
  TypeLink *p = base, *q, *r, *s, *t;
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
	  free_type_link (t);
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
	      r = new_type_link ();
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
	      free_type_link (t);
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
	      free_type_link (t);
	    }
	}
      /* before updating the next and prev links to point to the inserted q,
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

/* Some convenience macros */
#define RL_TYPE(list) ((list)->type)
#define RL_LEN(list) ((list)->len)
#define RL_POS(list) ((list)->pos)
#define RL_LEVEL(list) ((list)->level)

static TypeLink *
merge_with_prev (
  TypeLink *second
)
{
  TypeLink *first = second->prev;
  first->next = second->next;
  first->next->prev = first;
  RL_LEN (first) += RL_LEN (second);
  free_type_link (second);
  return first;
}

static void
compact_list (
  TypeLink *list
)
{
  if (list->next)
    for (list = list->next; list; list = list->next)
      if (RL_TYPE (list->prev) == RL_TYPE (list)
	  && RL_LEVEL (list->prev) == RL_LEVEL (list))
	list = merge_with_prev (list);
}

static void
compact_neutrals (
  TypeLink *list
)
{
  if (list->next)
    {
      for (list = list->next; list; list = list->next)
	{
	  if (RL_LEVEL (list->prev) == RL_LEVEL (list)
	      &&
	      ((RL_TYPE
		(list->prev) == RL_TYPE (list)
		|| (FRIBIDI_IS_NEUTRAL (RL_TYPE (list->prev))
		    && FRIBIDI_IS_NEUTRAL (RL_TYPE (list))))))
	    list = merge_with_prev (list);
	}
    }
}

/*=========================================================================
 * define macros for push and pop the status in to / out of the stack
 *-------------------------------------------------------------------------*/

/* There's some little points in pushing and poping into the status stack:
   1. when the embedding level is not valid (more than FRIBIDI_BIDI_MAX_EXPLICIT_LEVEL=61),
   you must reject it, and not to push into the stack, but when you see a
   PDF, you must find the matching code, and if it was pushed in the stack,
   pop it, it means you must pop if and only if you have pushed the
   matching code, the over_pushed var counts the number of rejected codes yet.
   2. there's a more confusing point too, when the embedding level is exactly
   FRIBIDI_BIDI_MAX_EXPLICIT_LEVEL-1=60, an LRO or LRE must be rejected because the new
   level would be FRIBIDI_BIDI_MAX_EXPLICIT_LEVEL+1=62, that is invalid, but an RLO or RLE
   must be accepted because the new level is FRIBIDI_BIDI_MAX_EXPLICIT_LEVEL=61, that is
   valid, so the rejected codes may be not continuous in the logical order,
   in fact there is at most two continuous intervals of codes, with a RLO or
   RLE between them.  To support this case, the first_interval var counts the
   number of rejected codes in the first interval, when it is 0, means that
   there is only one interval yet.
*/

/* a. If this new level would be valid, then this embedding code is valid.
   Remember (push) the current embedding level and override status.
   Reset current level to this new level, and reset the override status to
   new_override.
   b. If the new level would not be valid, then this code is invalid. Don't
   change the current level or override status.
*/
#define PUSH_STATUS \
    FRIBIDI_BEGIN_STMT \
      if (new_level <= FRIBIDI_BIDI_MAX_EXPLICIT_LEVEL) \
        { \
          if (level == FRIBIDI_BIDI_MAX_EXPLICIT_LEVEL - 1) \
            first_interval = over_pushed; \
          status_stack[stack_size].level = level; \
          status_stack[stack_size].override = override; \
          stack_size++; \
          level = new_level; \
          override = new_override; \
        } else \
	  over_pushed++; \
    FRIBIDI_END_STMT

/* If there was a valid matching code, restore (pop) the last remembered
   (pushed) embedding level and directional override.
*/
#define POP_STATUS \
    FRIBIDI_BEGIN_STMT \
      if (over_pushed || stack_size) \
      { \
        if (over_pushed > first_interval) \
          over_pushed--; \
        else \
          { \
            if (over_pushed == first_interval) \
              first_interval = 0; \
            stack_size--; \
            level = status_stack[stack_size].level; \
            override = status_stack[stack_size].override; \
          } \
      } \
    FRIBIDI_END_STMT

/*==========================================================================
 * There was no support for sor and eor in the absence of Explicit Embedding
 * Levels, so define macros, to support them, with as less change as needed.
 *--------------------------------------------------------------------------*/

/* Return the type of previous char or the sor, if already at the start of
   a run level. */
#define PREV_TYPE_OR_SOR(pp) \
    ( \
     RL_LEVEL(pp->prev) == RL_LEVEL(pp) ? \
      RL_TYPE(pp->prev) : \
      FRIBIDI_LEVEL_TO_DIR(MAX(RL_LEVEL(pp->prev), RL_LEVEL(pp))) \
    )

/* Return the type of next char or the eor, if already at the end of
   a run level. */
#define NEXT_TYPE_OR_EOR(pp) \
    ( \
     !pp->next ? \
      FRIBIDI_LEVEL_TO_DIR(RL_LEVEL(pp)) : \
      (RL_LEVEL(pp->next) == RL_LEVEL(pp) ? \
        RL_TYPE(pp->next) : \
        FRIBIDI_LEVEL_TO_DIR(MAX(RL_LEVEL(pp->next), RL_LEVEL(pp))) \
      ) \
    )


/* Return the embedding direction of a link. */
#define FRIBIDI_EMBEDDING_DIRECTION(list) \
    FRIBIDI_LEVEL_TO_DIR(RL_LEVEL(list))

#if DEBUG
/*======================================================================
 *  For debugging, define some functions for printing the types and the
 *  levels.
 *----------------------------------------------------------------------*/

static char char_from_level_array[] = {
  '!',				/* FRIBIDI_LEVEL_REMOVED, internal error, this level shouldn't be seen.  */
  '^',				/* FRIBIDI_LEVEL_START, indicating start of string. */
  '$',				/* FRIBIDI_LEVEL_END, indicating end of string. */
  /* 0-9,a-z,A-Z are the the only valid levels before resolving implicits.
     after that the level @ may be appear too. */
  '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
  'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
  'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
  'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D',
  'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N',
  'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
  'Y', 'Z',

  '@',				/* only must appear after resolving implicits. */
  '*', '*', '*'			/* overflows, this levels and higher levels show a bug!. */
};

#define fribidi_char_from_level(level) char_from_level_array[(level) + 3]

static void
print_types_re (
  TypeLink *pp
)
{
  MSG ("  Run types  : ");
  while (pp)
    {
      MSG5 ("%d:l%d(%s)[%d] ",
	    pp->pos, pp->len, fribidi_type_name (pp->type), pp->level);
      pp = pp->next;
    }
  MSG ("\n");
}

static void
print_resolved_levels (
  TypeLink *pp
)
{
  MSG ("  Res. levels: ");
  while (pp)
    {
      FriBidiStrIndex i;
      for (i = RL_LEN (pp); i; i--)
	MSG2 ("%c", fribidi_char_from_level (RL_LEVEL (pp)));
      pp = pp->next;
    }
  MSG ("\n");
}

static void
print_resolved_types (
  TypeLink *pp
)
{
  MSG ("  Res. types : ");
  while (pp)
    {
      FriBidiStrIndex i;
      for (i = RL_LEN (pp); i; i--)
	MSG2 ("%c", fribidi_char_from_bidi_type (pp->type));
      pp = pp->next;
    }
  MSG ("\n");
}

static void
print_bidi_string (
  /* input */
  const FriBidiChar *str,
  FriBidiStrIndex len
)
{
  MSG ("  Org. types : ");
  for (; len; len--)
    MSG2 ("%c", fribidi_char_from_bidi_type (fribidi_get_bidi_type (*str++)));
  MSG ("\n");
}
#endif /* DEBUG */

/*======================================================================
 *  This function should follow the Unicode specification closely!
 *----------------------------------------------------------------------*/
static void
fribidi_analyse_string (
  /* input */
  const FriBidiChar *str,
  FriBidiStrIndex len,
  /* input and output */
  FriBidiCharType *pbase_dir,
  /* output */
  TypeLink **ptype_rl_list,
  FriBidiLevel *pmax_level
)
{
  FriBidiLevel base_level, max_level;
  FriBidiCharType base_dir;
  FriBidiStrIndex i;
  TypeLink *type_rl_list, *explicits_list, *explicits_list_end, *pp;

  DBG ("Entering fribidi_analyse_string");

  /* Determinate character types */
  DBG ("  Determine character types");
  {
    FriBidiCharType *char_type =
      (FriBidiCharType *) fribidi_malloc (len * sizeof (FriBidiCharType));
    for (i = 0; i < len; i++)
      char_type[i] = fribidi_get_type (str[i]);

    /* Run length encode the character types */
    type_rl_list = run_length_encode_types (char_type, len);
    fribidi_free (char_type);
  }
  DBG ("  Determine character types, Done");

  init_list (&explicits_list, &explicits_list_end);

  /* Find base level */
  DBG ("  Finding the base level");
  if (FRIBIDI_IS_STRONG (*pbase_dir))
    base_level = FRIBIDI_DIR_TO_LEVEL (*pbase_dir);
  /* P2. P3. Search for first strong character and use its direction as
     base direction */
  else
    {
      /* If no strong base_dir was found, resort to the weak direction
         that was passed on input. */
      base_level = FRIBIDI_DIR_TO_LEVEL (*pbase_dir);
      base_dir = FRIBIDI_TYPE_ON;
      for (pp = type_rl_list; pp; pp = pp->next)
	if (FRIBIDI_IS_LETTER (RL_TYPE (pp)))
	  {
	    base_level = FRIBIDI_DIR_TO_LEVEL (RL_TYPE (pp));
	    base_dir = FRIBIDI_LEVEL_TO_DIR (base_level);
	    break;
	  }
    }
  base_dir = FRIBIDI_LEVEL_TO_DIR (base_level);
  DBG2 ("  Base level : %c", fribidi_char_from_level (base_level));
  DBG2 ("  Base dir   : %c", fribidi_char_from_bidi_type (base_dir));
  DBG ("  Finding the base level, Done");

# if DEBUG
  if (fribidi_debug_status ())
    {
      print_types_re (type_rl_list);
    }
# endif	/* DEBUG */

  /* Explicit Levels and Directions */
  DBG ("Explicit Levels and Directions");
  {
    /* X1. Begin by setting the current embedding level to the paragraph
       embedding level. Set the directional override status to neutral.
       Process each character iteratively, applying rules X2 through X9.
       Only embedding levels from 0 to 61 are valid in this phase. */
    FriBidiLevel level, new_level;
    FriBidiCharType override, new_override;
    FriBidiStrIndex i;
    int stack_size, over_pushed, first_interval;
    LevelInfo *status_stack;
    TypeLink temp_link;

    level = base_level;
    override = FRIBIDI_TYPE_ON;
    /* stack */
    stack_size = 0;
    over_pushed = 0;
    first_interval = 0;
    status_stack =
      (LevelInfo *) fribidi_malloc (sizeof (LevelInfo) *
				    FRIBIDI_BIDI_MAX_RESOLVED_LEVELS);

    for (pp = type_rl_list->next; pp->next; pp = pp->next)
      {
	FriBidiCharType this_type = RL_TYPE (pp);
	if (FRIBIDI_IS_EXPLICIT_OR_BN (this_type))
	  {
	    if (FRIBIDI_IS_STRONG (this_type))
	      {			/* LRE, RLE, LRO, RLO */
		/* 1. Explicit Embeddings */
		/*   X2. With each RLE, compute the least greater odd embedding level. */
		/*   X3. With each LRE, compute the least greater even embedding level. */
		/* 2. Explicit Overrides */
		/*   X4. With each RLO, compute the least greater odd embedding level. */
		/*   X5. With each LRO, compute the least greater even embedding level. */
		new_override = FRIBIDI_EXPLICIT_TO_OVERRIDE_DIR (this_type);
		for (i = RL_LEN (pp); i; i--)
		  {
		    new_level =
		      ((level + FRIBIDI_DIR_TO_LEVEL (this_type) + 2) & ~1) -
		      FRIBIDI_DIR_TO_LEVEL (this_type);
		    PUSH_STATUS;
		  }
	      }
	    else if (this_type == FRIBIDI_TYPE_PDF)
	      {
		/* 3. Terminating Embeddings and overrides */
		/*   X7. With each PDF, determine the matching embedding or
		   override code. */
		for (i = RL_LEN (pp); i; i--)
		  POP_STATUS;
	      }
	    /* X9. Remove all RLE, LRE, RLO, LRO, PDF, and BN codes. */
	    /* Remove element and add it to explicits_list */
	    temp_link.next = pp->next;
	    pp->level = FRIBIDI_LEVEL_REMOVED;
	    move_element_before (pp, explicits_list_end);
	    pp = &temp_link;
	  }
	else
	  {
	    /* X6. For all typed besides RLE, LRE, RLO, LRO, and PDF:
	       a. Set the level of the current character to the current
	       embedding level.
	       b. Whenever the directional override status is not neutral,
	       reset the current character type to the directional override
	       status. */
	    RL_LEVEL (pp) = level;
	    if (!FRIBIDI_IS_NEUTRAL (override))
	      RL_TYPE (pp) = override;
	  }
	/* X8. All explicit directional embeddings and overrides are
	   completely terminated at the end of each paragraph. Paragraph
	   separators are not included in the embedding. */
	/* This function is running on a single paragraph, so we can do
	   X8 after all the input is processed. */
      }

    /* Implementing X8. It has no effect on a single paragraph! */
    level = base_level;
    override = FRIBIDI_TYPE_ON;
    stack_size = 0;
    over_pushed = 0;

    fribidi_free (status_stack);
  }
  /* X10. The remaining rules are applied to each run of characters at the
     same level. For each run, determine the start-of-level-run (sor) and
     end-of-level-run (eor) type, either L or R. This depends on the
     higher of the two levels on either side of the boundary (at the start
     or end of the paragraph, the level of the 'other' run is the base
     embedding level). If the higher level is odd, the type is R, otherwise
     it is L. */
  /* Resolving Implicit Levels can be done out of X10 loop, so only change
     of Resolving Weak Types and Resolving Neutral Types is needed. */

  compact_list (type_rl_list);

# if DEBUG
  if (fribidi_debug_status ())
    {
      print_types_re (type_rl_list);
      print_bidi_string (str, len);
      print_resolved_levels (type_rl_list);
      print_resolved_types (type_rl_list);
    }
# endif	/* DEBUG */

  /* 4. Resolving weak types */
  DBG ("Resolving weak types");
  {
    FriBidiCharType last_strong, prev_type_org;
    fribidi_boolean w4;

    last_strong = base_dir;

    for (pp = type_rl_list->next; pp->next; pp = pp->next)
      {
	FriBidiCharType prev_type, this_type, next_type;

	prev_type = PREV_TYPE_OR_SOR (pp);
	this_type = RL_TYPE (pp);
	next_type = NEXT_TYPE_OR_EOR (pp);

	if (FRIBIDI_IS_STRONG (prev_type))
	  last_strong = prev_type;

	/* W1. NSM
	   Examine each non-spacing mark (NSM) in the level run, and change the
	   type of the NSM to the type of the previous character. If the NSM
	   is at the start of the level run, it will get the type of sor. */
	/* Implementation note: it is important that if the previous character
	   is not sor, then we should merge this run with the previous,
	   because of rules like W5, that we assume all of a sequence of
	   adjacent ETs are in one TypeLink. */
	if (this_type == FRIBIDI_TYPE_NSM)
	  {
	    if (RL_LEVEL (pp->prev) == RL_LEVEL (pp))
	      pp = merge_with_prev (pp);
	    else
	      RL_TYPE (pp) = prev_type;
	    continue;		/* As we know the next condition cannot be true. */
	  }

	/* W2: European numbers. */
	if (this_type == FRIBIDI_TYPE_EN && last_strong == FRIBIDI_TYPE_AL)
	  {
	    RL_TYPE (pp) = FRIBIDI_TYPE_AN;

	    /* Resolving dependency of loops for rules W1 and W2, so we
	       can merge them in one loop. */
	    if (next_type == FRIBIDI_TYPE_NSM)
	      RL_TYPE (pp->next) = FRIBIDI_TYPE_AN;
	  }
      }


    last_strong = base_dir;
    /* Resolving dependency of loops for rules W4 and W5, W5 may
       want to prevent W4 to take effect in the next turn, do this 
       through "w4". */
    w4 = TRUE;
    /* Resolving dependency of loops for rules W4 and W5 with W7,
       W7 may change an EN to L but it sets the prev_type_org if needed,
       so W4 and W5 in next turn can still do their works. */
    prev_type_org = FRIBIDI_TYPE_ON;

    for (pp = type_rl_list->next; pp->next; pp = pp->next)
      {
	FriBidiCharType prev_type, this_type, next_type;

	prev_type = PREV_TYPE_OR_SOR (pp);
	this_type = RL_TYPE (pp);
	next_type = NEXT_TYPE_OR_EOR (pp);

	if (FRIBIDI_IS_STRONG (prev_type))
	  last_strong = prev_type;

	/* W3: Change ALs to R. */
	if (this_type == FRIBIDI_TYPE_AL)
	  {
	    RL_TYPE (pp) = FRIBIDI_TYPE_RTL;
	    w4 = TRUE;
	    prev_type_org = FRIBIDI_TYPE_ON;
	    continue;
	  }

	/* W4. A single european separator changes to a european number.
	   A single common separator between two numbers of the same type
	   changes to that type. */
	if (w4
	    && RL_LEN (pp) == 1 && FRIBIDI_IS_ES_OR_CS (this_type)
	    && FRIBIDI_IS_NUMBER (prev_type_org) && prev_type_org == next_type
	    && (prev_type_org == FRIBIDI_TYPE_EN
		|| this_type == FRIBIDI_TYPE_CS))
	  {
	    RL_TYPE (pp) = prev_type;
	    this_type = RL_TYPE (pp);
	  }
	w4 = TRUE;

	/* W5. A sequence of European terminators adjacent to European
	   numbers changes to All European numbers. */
	if (this_type == FRIBIDI_TYPE_ET
	    && (prev_type_org == FRIBIDI_TYPE_EN
		|| next_type == FRIBIDI_TYPE_EN))
	  {
	    RL_TYPE (pp) = FRIBIDI_TYPE_EN;
	    w4 = FALSE;
	    this_type = RL_TYPE (pp);
	  }

	/* W6. Otherwise change separators and terminators to other neutral. */
	if (FRIBIDI_IS_NUMBER_SEPARATOR_OR_TERMINATOR (this_type))
	  RL_TYPE (pp) = FRIBIDI_TYPE_ON;

	/* W7. Change european numbers to L. */
	if (this_type == FRIBIDI_TYPE_EN && last_strong == FRIBIDI_TYPE_LTR)
	  {
	    RL_TYPE (pp) = FRIBIDI_TYPE_LTR;
	    prev_type_org = (RL_LEVEL (pp) == RL_LEVEL (pp->next) ?
			     FRIBIDI_TYPE_EN : FRIBIDI_TYPE_ON);
	  }
	else
	  prev_type_org = PREV_TYPE_OR_SOR (pp->next);
      }
  }

  compact_neutrals (type_rl_list);

# if DEBUG
  if (fribidi_debug_status ())
    {
      print_resolved_levels (type_rl_list);
      print_resolved_types (type_rl_list);
    }
# endif	/* DEBUG */

  /* 5. Resolving Neutral Types */
  DBG ("Resolving neutral types");
  {
    /* N1. and N2.
       For each neutral, resolve it. */
    for (pp = type_rl_list->next; pp->next; pp = pp->next)
      {
	FriBidiCharType prev_type, this_type, next_type;

	/* "European and arabic numbers are treated as though they were R"
	   FRIBIDI_CHANGE_NUMBER_TO_RTL does this. */
	this_type = FRIBIDI_CHANGE_NUMBER_TO_RTL (RL_TYPE (pp));
	prev_type = FRIBIDI_CHANGE_NUMBER_TO_RTL (PREV_TYPE_OR_SOR (pp));
	next_type = FRIBIDI_CHANGE_NUMBER_TO_RTL (NEXT_TYPE_OR_EOR (pp));

	if (FRIBIDI_IS_NEUTRAL (this_type))
	  RL_TYPE (pp) = (prev_type == next_type) ?
	    /* N1. */ prev_type :
	    /* N2. */ FRIBIDI_EMBEDDING_DIRECTION (pp);
      }
  }

  compact_list (type_rl_list);

# if DEBUG
  if (fribidi_debug_status ())
    {
      print_resolved_levels (type_rl_list);
      print_resolved_types (type_rl_list);
    }
# endif	/* DEBUG */

  /* 6. Resolving implicit levels */
  DBG ("Resolving implicit levels");
  {
    max_level = base_level;

    for (pp = type_rl_list->next; pp->next; pp = pp->next)
      {
	FriBidiCharType this_type;
	int level;

	this_type = RL_TYPE (pp);
	level = RL_LEVEL (pp);

	/* I1. Even */
	/* I2. Odd */
	if (FRIBIDI_IS_NUMBER (this_type))
	  RL_LEVEL (pp) = (level + 2) & ~1;
	else
	  RL_LEVEL (pp) = (level ^ FRIBIDI_DIR_TO_LEVEL (this_type)) +
	    (level & 1);

	if (RL_LEVEL (pp) > max_level)
	  max_level = RL_LEVEL (pp);
      }
  }

  compact_list (type_rl_list);

# if DEBUG
  if (fribidi_debug_status ())
    {
      print_bidi_string (str, len);
      print_resolved_levels (type_rl_list);
      print_resolved_types (type_rl_list);
    }
# endif	/* DEBUG */

/* Reinsert the explicit codes & bn's that already removed, from the
   explicits_list to type_rl_list. */
  DBG ("Reinserting explicit codes");
  {
    TypeLink *p;

    override_list (type_rl_list, explicits_list);
    p = type_rl_list->next;
    if (p->level < 0)
      p->level = base_level;
    for (; p->next; p = p->next)
      if (p->level < 0)
	p->level = p->prev->level;
  }

# if DEBUG
  if (fribidi_debug_status ())
    {
      print_types_re (type_rl_list);
      print_resolved_levels (type_rl_list);
      print_resolved_types (type_rl_list);
    }
# endif	/* DEBUG */

  DBG ("Reset the embedding levels");
  {
    int j, k, state, pos;
    TypeLink *p, *q, *list, *list_end;

    /* L1. Reset the embedding levels of some chars. */
    init_list (&list, &list_end);
    q = list_end;
    state = 1;
    pos = len - 1;
    for (j = len - 1; j >= -1; j--)
      {
	/* if state is on at the very first of string, do this too. */
	if (j >= 0)
	  k = fribidi_get_type (str[j]);
	else
	  k = FRIBIDI_TYPE_ON;
	if (!state && FRIBIDI_IS_SEPARATOR (k))
	  {
	    state = 1;
	    pos = j;
	  }
	else if (state && !FRIBIDI_IS_EXPLICIT_OR_SEPARATOR_OR_BN_OR_WS (k))
	  {
	    state = 0;
	    p = new_type_link ();
	    p->prev = p->next = NULL;
	    p->pos = j + 1;
	    p->len = pos - j;
	    p->type = base_dir;
	    p->level = base_level;
	    move_element_before (p, q);
	    q = p;
	  }
      }
    override_list (type_rl_list, list);
  }

# if DEBUG
  if (fribidi_debug_status ())
    {
      print_types_re (type_rl_list);
      print_resolved_levels (type_rl_list);
      print_resolved_types (type_rl_list);
    }
# endif	/* DEBUG */

  *ptype_rl_list = type_rl_list;
  *pmax_level = max_level;
  *pbase_dir = base_dir;

  DBG ("Leaving fribidi_analyse_string");
  return;
}

/*======================================================================
 *  Frees up the rl_list, must be called after each call to
 *  fribidi_analyse_string(), after the list is not needed anymore.
 *----------------------------------------------------------------------*/
static void
free_rl_list (
  TypeLink *type_rl_list
)
{

  TypeLink *pp;

  DBG ("Entering free_rl_list");

  if (!type_rl_list)
    {
      DBG ("Leaving free_rl_list");
      return;
    }

#if USE_SIMPLE_MALLOC
  pp = type_rl_list;
  while (pp)
    {
      TypeLink *p;

      p = pp;
      pp = pp->next;
      free_type_link (p);
    };
#else /* !USE_SIMPLE_MALLOC */
  for (pp = type_rl_list->next; pp->next; pp = pp->next)
    /* Nothing */ ;
  pp->next = free_type_links;
  free_type_links = type_rl_list;
  type_rl_list = NULL;
#endif /* !USE_SIMPLE_MALLOC */

  DBG ("Leaving free_rl_list");
  return;
}

/*======================================================================
 *  Here starts the exposed front end functions.
 *----------------------------------------------------------------------*/

/*======================================================================
 *  fribidi_remove_bidi_marks() removes bidirectional marks, and returns
 *  the new length, updates each of other inputs if not NULL.
 *----------------------------------------------------------------------*/
FRIBIDI_ENTRY FriBidiStrIndex
fribidi_remove_bidi_marks (
  FriBidiChar *str,
  FriBidiStrIndex length,
  FriBidiStrIndex *position_to_this_list,
  FriBidiStrIndex *position_from_this_list,
  FriBidiLevel *embedding_level_list
)
{
  FriBidiStrIndex i, j;
  fribidi_boolean private_from_this = FALSE;

  DBG ("Entering fribidi_remove_bidi_marks");

  /* If to_this is to not null, we must have from_this as well. If it is
     not given by the caller, we have to make a private instance of it. */
  if (position_to_this_list && !position_from_this_list)
    {
      private_from_this = TRUE;
      position_from_this_list =
	(FriBidiStrIndex *) fribidi_malloc (sizeof (FriBidiStrIndex) *
					    length);
    }

  j = 0;
  for (i = 0; i < length; i++)
    if (!FRIBIDI_IS_EXPLICIT (fribidi_get_type (str[i]))
	&& str[i] != FRIBIDI_CHAR_LRM && str[i] != FRIBIDI_CHAR_RLM)
      {
	str[j] = str[i];
	if (embedding_level_list)
	  embedding_level_list[j] = embedding_level_list[i];
	if (position_from_this_list)
	  position_from_this_list[j] = position_from_this_list[i];
	j++;
      }

  /* Convert the from_this list to to_this */
  if (position_to_this_list)
    {
      DBG ("  Converting from_this list to to_this");
      for (i = 0; i < length; i++)
	position_to_this_list[i] = -1;
      for (i = 0; i < length; i++)
	position_to_this_list[position_from_this_list[i]] = i;
      DBG ("  Converting from_this list to to_this, Done");
    }

  if (private_from_this)
    fribidi_free (position_from_this_list);

  DBG ("Leaving fribidi_remove_bidi_marks");
  return j;
}


/*======================================================================
 *  fribidi_log2vis() calls the function_analyse_string() and then
 *  does reordering and fills in the output strings.
 *----------------------------------------------------------------------*/
FRIBIDI_ENTRY fribidi_boolean
fribidi_log2vis (
  /* input */
  const FriBidiChar *str,
  FriBidiStrIndex len,
  /* input and output */
  FriBidiCharType *pbase_dir,
  /* output */
  FriBidiChar *visual_str,
  FriBidiStrIndex *position_L_to_V_list,
  FriBidiStrIndex *position_V_to_L_list,
  FriBidiLevel *embedding_level_list
)
{
  TypeLink *type_rl_list, *pp = NULL;
  FriBidiLevel max_level;
  fribidi_boolean private_V_to_L = FALSE;

  DBG ("Entering fribidi_log2vis()\n");

  if (len == 0)
    {
      DBG ("Leaving fribidi_log2vis()\n");
      return TRUE;
    }

  /* If l2v is to be calculated we must have v2l as well. If it is not
     given by the caller, we have to make a private instance of it. */
  if (position_L_to_V_list && !position_V_to_L_list)
    {
      private_V_to_L = TRUE;
      position_V_to_L_list =
	(FriBidiStrIndex *) fribidi_malloc (sizeof (FriBidiStrIndex) * len);
    }

  if (len > FRIBIDI_MAX_STRING_LENGTH && position_V_to_L_list)
    {
#     if DEBUG
      MSG2 (FRIBIDI ": cannot handle strings > %ld characters\n",
	    (long) FRIBIDI_MAX_STRING_LENGTH);
#     endif /* DEBUG */
      return FALSE;
    }
  fribidi_analyse_string (str, len, pbase_dir,
			  /* output */
			  &type_rl_list, &max_level);

  /* 7. Reordering resolved levels */
  DBG ("Reordering resolved levels\n");
  {
    FriBidiLevel level_idx;
    FriBidiStrIndex i;

    /* Set up the ordering array to sorted order */
    if (position_V_to_L_list)
      {
	DBG ("  Initialize position_V_to_L_list\n");
	for (i = 0; i < len; i++)
	  position_V_to_L_list[i] = i;
	DBG ("  Initialize position_V_to_L_list, Done\n");
      }
    /* Copy the logical string to the visual */
    if (visual_str)
      {
	DBG ("  Initialize visual_str\n");
	for (i = 0; i < len; i++)
	  visual_str[i] = str[i];
	visual_str[len] = 0;
	DBG ("  Initialize visual_str, Done\n");
      }

    /* Assign the embedding level array */
    if (embedding_level_list)
      {
	DBG ("  Fill the embedding levels array\n");
	for (pp = type_rl_list->next; pp->next; pp = pp->next)
	  {
	    FriBidiStrIndex i, pos, len;
	    FriBidiLevel level;

	    pos = pp->pos;
	    len = pp->len;
	    level = pp->level;
	    for (i = 0; i < len; i++)
	      embedding_level_list[pos + i] = level;
	  }
	DBG ("  Fill the embedding levels array, Done\n");
      }

    /* Reorder both the outstring and the order array */
    if (visual_str || position_V_to_L_list)
      {
	if (fribidi_mirroring_status () && visual_str)
	  {
	    /* L4. Mirror all characters that are in odd levels and have mirrors. */
	    DBG ("  Mirroring\n");
	    for (pp = type_rl_list->next; pp->next; pp = pp->next)
	      {
		if (pp->level & 1)
		  {
		    FriBidiStrIndex i;
		    for (i = RL_POS (pp); i < RL_POS (pp) + RL_LEN (pp); i++)
		      {
			FriBidiChar mirrored_ch;
			if (fribidi_get_mirror_char
			    (visual_str[i], &mirrored_ch))
			  visual_str[i] = mirrored_ch;
		      }
		  }
	      }
	    DBG ("  Mirroring, Done\n");
	  }

	if (fribidi_reorder_nsm_status ())
	  {
	    /* L3. Reorder NSMs. */
	    DBG ("  Reordering NSM sequences\n");
	    /* We apply this rule before L2, so go backward in odd levels. */
	    for (pp = type_rl_list->next; pp->next; pp = pp->next)
	      {
		if (pp->level & 1)
		  {
		    FriBidiStrIndex i, seq_end = 0;
		    fribidi_boolean is_nsm_seq;

		    is_nsm_seq = FALSE;
		    for (i = RL_POS (pp) + RL_LEN (pp) - 1; i >= RL_POS (pp);
			 i--)
		      {
			FriBidiCharType this_type;

			this_type = fribidi_get_type (str[i]);
			if (is_nsm_seq && this_type != FRIBIDI_TYPE_NSM)
			  {
			    if (visual_str)
			      {
				bidi_string_reverse (visual_str + i,
						     seq_end - i + 1);
			      }
			    if (position_V_to_L_list)
			      {
				index_array_reverse (position_V_to_L_list + i,
						     seq_end - i + 1);
			      }
			    is_nsm_seq = 0;
			  }
			else if (!is_nsm_seq && this_type == FRIBIDI_TYPE_NSM)
			  {
			    seq_end = i;
			    is_nsm_seq = 1;
			  }
		      }
		    if (is_nsm_seq)
		      {
			DBG
			  ("Warning: NSMs at the beggining of run level.\n");
		      }
		  }
	      }
	    DBG ("  Reordering NSM sequences, Done\n");
	  }

	/* L2. Reorder. */
	DBG ("  Reordering\n");
	for (level_idx = max_level; level_idx > 0; level_idx--)
	  {
	    for (pp = type_rl_list->next; pp->next; pp = pp->next)
	      {
		if (RL_LEVEL (pp) >= level_idx)
		  {
		    /* Find all stretches that are >= level_idx */
		    FriBidiStrIndex len = RL_LEN (pp), pos = RL_POS (pp);
		    TypeLink *pp1 = pp->next;
		    while (pp1->next && RL_LEVEL (pp1) >= level_idx)
		      {
			len += RL_LEN (pp1);
			pp1 = pp1->next;
		      }
		    pp = pp1->prev;
		    if (visual_str)
		      bidi_string_reverse (visual_str + pos, len);
		    if (position_V_to_L_list)
		      index_array_reverse (position_V_to_L_list + pos, len);
		  }
	      }
	  }
	DBG ("  Reordering, Done\n");
      }

    /* Convert the v2l list to l2v */
    if (position_L_to_V_list)
      {
	DBG ("  Converting v2l list to l2v\n");
	for (i = 0; i < len; i++)
	  position_L_to_V_list[position_V_to_L_list[i]] = i;
	DBG ("  Converting v2l list to l2v, Done\n");
      }
  }
  DBG ("Reordering resolved levels, Done\n");

  if (private_V_to_L)
    fribidi_free (position_V_to_L_list);

  free_rl_list (type_rl_list);

  DBG ("Leaving fribidi_log2vis()\n");
  return TRUE;

}

/*======================================================================
 *  fribidi_log2vis_get_embedding_levels() is used in order to just get
 *  the embedding levels.
 *----------------------------------------------------------------------*/
FRIBIDI_ENTRY fribidi_boolean
fribidi_log2vis_get_embedding_levels (
  /* input */
  const FriBidiChar *str,
  FriBidiStrIndex len,
  /* input and output */
  FriBidiCharType *pbase_dir,
  /* output */
  FriBidiLevel *embedding_level_list
)
{
  TypeLink *type_rl_list, *pp;
  FriBidiLevel max_level;

  DBG ("Entering fribidi_log2vis_get_embedding_levels()\n");

  if (len == 0)
    {
      DBG ("Leaving fribidi_log2vis_get_embedding_levels()\n");
      return TRUE;
    }

  fribidi_analyse_string (str, len, pbase_dir,
			  /* output */
			  &type_rl_list, &max_level);

  for (pp = type_rl_list->next; pp->next; pp = pp->next)
    {
      FriBidiStrIndex i, pos = RL_POS (pp), len = RL_LEN (pp);
      FriBidiLevel level = RL_LEVEL (pp);
      for (i = 0; i < len; i++)
	embedding_level_list[pos + i] = level;
    }

  free_rl_list (type_rl_list);

  DBG ("Leaving fribidi_log2vis_get_embedding_levels()\n");
  return TRUE;
}

/* Editor directions:
 * vim:textwidth=78:tabstop=8:shiftwidth=2:autoindent:cindent
 */
