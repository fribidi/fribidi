/* FriBidi
 * fribidi-bidi.c - bidirectional algorithm
 *
 * $Id: fribidi-bidi.c,v 1.7 2004-05-31 18:39:39 behdad Exp $
 * $Author: behdad $
 * $Date: 2004-05-31 18:39:39 $
 * $Revision: 1.7 $
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

#include "common.h"

#include <fribidi-bidi.h>
#include <fribidi-bidi-type.h>
#include <fribidi-mirroring.h>
#include <fribidi-unicode.h>
#include <fribidi-env.h>

#include "mem.h"
#include "env.h"
#include "bidi-types.h"
#include "run.h"

#ifndef MAX
# define MAX(a,b) ((a) > (b) ? (a) : (b))
#endif /* !MAX */

typedef struct
{
  FriBidiCharType override;	/* only LTR, RTL and ON are valid */
  FriBidiLevel level;
}
LevelInfo;

/* Some convenience macros */
#define RL_TYPE(list) ((list)->type)
#define RL_LEN(list) ((list)->len)
#define RL_POS(list) ((list)->pos)
#define RL_LEVEL(list) ((list)->level)

static FriBidiRun *
merge_with_prev (
  FriBidiRun *second
)
{
  FriBidiRun *first;

  fribidi_assert (second);
  fribidi_assert (second->next);
  first = second->prev;
  fribidi_assert (first);

  first->next = second->next;
  first->next->prev = first;
  RL_LEN (first) += RL_LEN (second);
  free_run (second);
  return first;
}

static void
compact_list (
  FriBidiRun *list
)
{
  fribidi_assert (list);

  if (list->next)
    for_run_list (list, list)
      if (RL_TYPE (list->prev) == RL_TYPE (list)
	  && RL_LEVEL (list->prev) == RL_LEVEL (list))
      list = merge_with_prev (list);
}

static void
compact_neutrals (
  FriBidiRun *list
)
{
  fribidi_assert (list);

  if (list->next)
    {
      for_run_list (list, list)
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
   1. when the embedding level is not valid (more than
   FRIBIDI_BIDI_MAX_EXPLICIT_LEVEL=61), you must reject it, and not to push
   into the stack, but when you see a PDF, you must find the matching code,
   and if it was pushed in the stack, pop it, it means you must pop if and
   only if you have pushed the matching code, the over_pushed var counts the
   number of rejected codes yet.
   2. there's a more confusing point too, when the embedding level is exactly
   FRIBIDI_BIDI_MAX_EXPLICIT_LEVEL-1=60, an LRO or LRE must be rejected
   because the new level would be FRIBIDI_BIDI_MAX_EXPLICIT_LEVEL+1=62, that
   is invalid, but an RLO or RLE must be accepted because the new level is
   FRIBIDI_BIDI_MAX_EXPLICIT_LEVEL=61, that is valid, so the rejected codes
   may be not continuous in the logical order, in fact there is at most two
   continuous intervals of codes, with a RLO or RLE between them.  To support
   this case, the first_interval var counts the number of rejected codes in
   the first interval, when it is 0, means that there is only one interval yet.
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
      if LIKELY(new_level <= FRIBIDI_BIDI_MAX_EXPLICIT_LEVEL) \
        { \
          if UNLIKELY(level == FRIBIDI_BIDI_MAX_EXPLICIT_LEVEL - 1) \
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
      if (stack_size) \
      { \
        if UNLIKELY(over_pushed > first_interval) \
          over_pushed--; \
        else \
          { \
            if LIKELY(over_pushed == first_interval) \
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

/* Return the type of previous run or the SOR, if already at the start of
   a level run. */
#define PREV_TYPE_OR_SOR(pp) \
    ( \
      RL_LEVEL(pp->prev) == RL_LEVEL(pp) ? \
        RL_TYPE(pp->prev) : \
        FRIBIDI_LEVEL_TO_DIR(MAX(RL_LEVEL(pp->prev), RL_LEVEL(pp))) \
    )

/* Return the type of next run or the EOR, if already at the end of
   a level run. */
#define NEXT_TYPE_OR_EOR(pp) \
    ( \
      RL_LEVEL(pp->next) == RL_LEVEL(pp) ? \
        RL_TYPE(pp->next) : \
        FRIBIDI_LEVEL_TO_DIR(MAX(RL_LEVEL(pp->next), RL_LEVEL(pp))) \
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
  '$',				/* -1 == FRIBIDI_SENTINEL, indicating
				 * start or end of string. */
  /* 0-61 == 0-9,a-z,A-Z are the the only valid levels before resolving
   * implicits.  after that the level @ may be appear too. */
  '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
  'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
  'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
  'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D',
  'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N',
  'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
  'Y', 'Z',

  '@',				/* 62 == only must appear after resolving
				 * implicits. */

  '!',				/* 63 == FRIBIDI_LEVEL_INVALID, internal error,
				 * this level shouldn't be seen.  */

  '*', '*', '*', '*', '*'	/* >= 64 == overflows, this levels and higher
				 * levels show a real bug!. */
};

#define fribidi_char_from_level(level) char_from_level_array[(level) + 1]

static void
print_types_re (
  FriBidiRun *pp
)
{
  fribidi_assert (pp);

  MSG ("  Run types  : ");
  for_run_list (pp, pp)
  {
    MSG5 ("%d:%d(%s)[%d] ",
	  pp->pos, pp->len, fribidi_bidi_type_name (pp->type), pp->level);
  }
  MSG ("\n");
}

static void
print_resolved_levels (
  FriBidiRun *pp
)
{
  fribidi_assert (pp);

  MSG ("  Res. levels: ");
  for_run_list (pp, pp)
  {
    FriBidiStrIndex i;
    for (i = RL_LEN (pp); i; i--)
      MSG2 ("%c", fribidi_char_from_level (RL_LEVEL (pp)));
  }
  MSG ("\n");
}

static void
print_resolved_types (
  FriBidiRun *pp
)
{
  fribidi_assert (pp);

  MSG ("  Res. types : ");
  for_run_list (pp, pp)
  {
    FriBidiStrIndex i;
    for (i = RL_LEN (pp); i; i--)
      MSG2 ("%c", fribidi_char_from_bidi_type (pp->type));
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
  fribidi_assert (str);

  MSG ("  Org. types : ");
  for (; len; len--)
    MSG2 ("%c", fribidi_char_from_bidi_type (fribidi_get_bidi_type (*str++)));
  MSG ("\n");
}
#endif /* DEBUG */

static fribidi_boolean
fribidi_analyse_string (
  const FriBidiChar *str,
  FriBidiStrIndex len,
  FriBidiCharType *pbase_dir,
  FriBidiRun **pmain_run_list,
  FriBidiLevel *pmax_level
) FRIBIDI_GNUC_WARN_UNUSED;

/*======================================================================
 *  This function should follow the Unicode specification closely!
 *----------------------------------------------------------------------*/
     static fribidi_boolean fribidi_analyse_string (
  /* input */
  const FriBidiChar *str,
  FriBidiStrIndex len,
  /* input and output */
  FriBidiCharType *pbase_dir,
  /* output */
  FriBidiRun **pmain_run_list,
  FriBidiLevel *pmax_level
)
{
  FriBidiLevel base_level, max_level;
  FriBidiCharType base_dir;
  FriBidiRun *main_run_list = NULL, *explicits_list = NULL, *pp;
  fribidi_boolean status = false;

  DBG ("entering fribidi_analyse_string");

  fribidi_assert (str);
  fribidi_assert (pbase_dir);
  fribidi_assert (pmain_run_list);
  fribidi_assert (pmax_level);

  /* Determinate character types */
  DBG ("  determine character types");
  {
    FriBidiCharType *char_types =
      (FriBidiCharType *) fribidi_malloc (len * sizeof (FriBidiCharType));
    if (!char_types)
      goto out;
    fribidi_get_bidi_types (str, len, char_types);

    /* Run length encode the character types */
    main_run_list = run_list_encode_bidi_types (char_types, len);
    fribidi_free (char_types);
    if (!main_run_list)
      goto out;
  }
  DBG ("  determine character types, done");

  /* Find base level */
  DBG ("  finding the base level");
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
      for_run_list (pp, main_run_list) if (FRIBIDI_IS_LETTER (RL_TYPE (pp)))
	{
	  base_level = FRIBIDI_DIR_TO_LEVEL (RL_TYPE (pp));
	  base_dir = FRIBIDI_LEVEL_TO_DIR (base_level);
	  break;
	}
    }
  base_dir = FRIBIDI_LEVEL_TO_DIR (base_level);
  DBG2 ("  base level : %c", fribidi_char_from_level (base_level));
  DBG2 ("  base dir   : %c", fribidi_char_from_bidi_type (base_dir));
  DBG ("  finding the base level, done");

# if DEBUG
  if UNLIKELY
    (fribidi_debug_status ())
    {
      print_types_re (main_run_list);
    }
# endif	/* DEBUG */

  /* Explicit Levels and Directions */
  DBG ("pxplicit Levels and Directions");
  {
    FriBidiLevel level, new_level;
    FriBidiCharType override, new_override;
    FriBidiStrIndex i;
    int stack_size, over_pushed, first_interval;
    LevelInfo *status_stack;
    FriBidiRun temp_link;

/* explicits_list is a list like main_run_list, that holds the explicit
   codes that are removed from main_run_list, to reinsert them later by
   calling the shadow_run_list.
*/
    explicits_list = new_run_list ();
    if UNLIKELY
      (!explicits_list) goto out;

    /* X1. Begin by setting the current embedding level to the paragraph
       embedding level. Set the directional override status to neutral.
       Process each character iteratively, applying rules X2 through X9.
       Only embedding levels from 0 to 61 are valid in this phase. */

    level = base_level;
    override = FRIBIDI_TYPE_ON;
    /* stack */
    stack_size = 0;
    over_pushed = 0;
    first_interval = 0;
    status_stack =
      (LevelInfo *) fribidi_malloc (sizeof (LevelInfo) *
				    FRIBIDI_BIDI_MAX_RESOLVED_LEVELS);

    for_run_list (pp, main_run_list)
    {
      FriBidiCharType this_type = RL_TYPE (pp);
      if (FRIBIDI_IS_EXPLICIT_OR_BN (this_type))
	{
	  if (FRIBIDI_IS_STRONG (this_type))
	    {			/* LRE, RLE, LRO, RLO */
	      /* 1. Explicit Embeddings */
	      /*   X2. With each RLE, compute the least greater odd
	         embedding level. */
	      /*   X3. With each LRE, compute the least greater even
	         embedding level. */
	      /* 2. Explicit Overrides */
	      /*   X4. With each RLO, compute the least greater odd
	         embedding level. */
	      /*   X5. With each LRO, compute the least greater even
	         embedding level. */
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
	  pp->level = FRIBIDI_SENTINEL;
	  move_node_before (pp, explicits_list);
	  pp = &temp_link;
	}
      else
	{
	  /* X6. For all types besides RLE, LRE, RLO, LRO, and PDF:
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

  compact_list (main_run_list);

# if DEBUG
  if UNLIKELY
    (fribidi_debug_status ())
    {
      print_types_re (main_run_list);
      print_bidi_string (str, len);
      print_resolved_levels (main_run_list);
      print_resolved_types (main_run_list);
    }
# endif	/* DEBUG */

  /* 4. Resolving weak types */
  DBG ("resolving weak types");
  {
    FriBidiCharType last_strong, prev_type_orig;
    fribidi_boolean w4;

    last_strong = base_dir;

    for_run_list (pp, main_run_list)
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
         adjacent ETs are in one FriBidiRun. */
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
    w4 = true;
    /* Resolving dependency of loops for rules W4 and W5 with W7,
       W7 may change an EN to L but it sets the prev_type_orig if needed,
       so W4 and W5 in next turn can still do their works. */
    prev_type_orig = FRIBIDI_TYPE_ON;

    for_run_list (pp, main_run_list)
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
	  w4 = true;
	  prev_type_orig = FRIBIDI_TYPE_ON;
	  continue;
	}

      /* W4. A single european separator changes to a european number.
         A single common separator between two numbers of the same type
         changes to that type. */
      if (w4
	  && RL_LEN (pp) == 1 && FRIBIDI_IS_ES_OR_CS (this_type)
	  && FRIBIDI_IS_NUMBER (prev_type_orig)
	  && prev_type_orig == next_type
	  && (prev_type_orig == FRIBIDI_TYPE_EN
	      || this_type == FRIBIDI_TYPE_CS))
	{
	  RL_TYPE (pp) = prev_type;
	  this_type = RL_TYPE (pp);
	}
      w4 = true;

      /* W5. A sequence of European terminators adjacent to European
         numbers changes to All European numbers. */
      if (this_type == FRIBIDI_TYPE_ET
	  && (prev_type_orig == FRIBIDI_TYPE_EN
	      || next_type == FRIBIDI_TYPE_EN))
	{
	  RL_TYPE (pp) = FRIBIDI_TYPE_EN;
	  w4 = false;
	  this_type = RL_TYPE (pp);
	}

      /* W6. Otherwise change separators and terminators to other neutral. */
      if (FRIBIDI_IS_NUMBER_SEPARATOR_OR_TERMINATOR (this_type))
	RL_TYPE (pp) = FRIBIDI_TYPE_ON;

      /* W7. Change european numbers to L. */
      if (this_type == FRIBIDI_TYPE_EN && last_strong == FRIBIDI_TYPE_LTR)
	{
	  RL_TYPE (pp) = FRIBIDI_TYPE_LTR;
	  prev_type_orig = (RL_LEVEL (pp) == RL_LEVEL (pp->next) ?
			    FRIBIDI_TYPE_EN : FRIBIDI_TYPE_ON);
	}
      else
	prev_type_orig = PREV_TYPE_OR_SOR (pp->next);
    }
  }

  compact_neutrals (main_run_list);

# if DEBUG
  if UNLIKELY
    (fribidi_debug_status ())
    {
      print_resolved_levels (main_run_list);
      print_resolved_types (main_run_list);
    }
# endif	/* DEBUG */

  /* 5. Resolving Neutral Types */
  DBG ("resolving neutral types");
  {
    /* N1. and N2.
       For each neutral, resolve it. */
    for_run_list (pp, main_run_list)
    {
      FriBidiCharType prev_type, this_type, next_type;

      /* "European and Arabic numbers are treated as though they were R"
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

  compact_list (main_run_list);

# if DEBUG
  if UNLIKELY
    (fribidi_debug_status ())
    {
      print_resolved_levels (main_run_list);
      print_resolved_types (main_run_list);
    }
# endif	/* DEBUG */

  /* 6. Resolving implicit levels */
  DBG ("resolving implicit levels");
  {
    max_level = base_level;

    for_run_list (pp, main_run_list)
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

  compact_list (main_run_list);

# if DEBUG
  if UNLIKELY
    (fribidi_debug_status ())
    {
      print_bidi_string (str, len);
      print_resolved_levels (main_run_list);
      print_resolved_types (main_run_list);
    }
# endif	/* DEBUG */

/* Reinsert the explicit codes & BN's that are already removed, from the
   explicits_list to main_run_list. */
  DBG ("reinserting explicit codes");
  {
    register FriBidiRun *p;
    register fribidi_boolean stat =
      shadow_run_list (main_run_list, explicits_list, true);
    explicits_list = NULL;
    if UNLIKELY
      (!stat) goto out;
    p = main_run_list->next;
    if (p != main_run_list && p->level == FRIBIDI_SENTINEL)
      p->level = base_level;
    for_run_list (p, main_run_list) if (p->level == FRIBIDI_SENTINEL)
      p->level = p->prev->level;
  }

# if DEBUG
  if UNLIKELY
    (fribidi_debug_status ())
    {
      print_types_re (main_run_list);
      print_resolved_levels (main_run_list);
      print_resolved_types (main_run_list);
    }
# endif	/* DEBUG */

  DBG ("reset the embedding levels");
  {
    register int j, k, state, pos;
    register FriBidiRun *p, *q, *list;

    /* L1. Reset the embedding levels of some chars. */
    list = new_run_list ();
    if UNLIKELY
      (!list) goto out;
    q = list;
    state = 1;
    pos = len - 1;
    for (j = len - 1; j >= -1; j--)
      {
	/* if state is on at the very first of the string, do this too. */
	if (j >= 0)
	  k = fribidi_get_bidi_type (str[j]);
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
	    p = new_run ();
	    if UNLIKELY
	      (!p)
	      {
		free_run_list (list);
		goto out;
	      }
	    p->pos = j + 1;
	    p->len = pos - j;
	    p->type = base_dir;
	    p->level = base_level;
	    move_node_before (p, q);
	    q = p;
	  }
      }
    if UNLIKELY
      (!shadow_run_list (main_run_list, list, false)) goto out;
  }

# if DEBUG
  if UNLIKELY
    (fribidi_debug_status ())
    {
      print_types_re (main_run_list);
      print_resolved_levels (main_run_list);
      print_resolved_types (main_run_list);
    }
# endif	/* DEBUG */

  status = true;
  *pmain_run_list = main_run_list;
  *pmax_level = max_level;
  *pbase_dir = base_dir;

  DBG ("leaving fribidi_analyse_string");
out:
  if UNLIKELY
    (explicits_list) free_run_list (explicits_list);
  if UNLIKELY
    (!status && main_run_list) free_run_list (main_run_list);

  return status;
}

static void
bidi_string_reverse (
  FriBidiChar *str,
  FriBidiStrIndex len
)
{
  FriBidiStrIndex i;

  fribidi_assert (str);

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

  fribidi_assert (arr);

  for (i = 0; i < len / 2; i++)
    {
      FriBidiStrIndex tmp = arr[i];
      arr[i] = arr[len - 1 - i];
      arr[len - 1 - i] = tmp;
    }
}


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
  fribidi_boolean private_from_this = false;

  DBG ("entering fribidi_remove_bidi_marks");

  /* If to_this is to not null, we must have from_this as well. If it is
     not given by the caller, we have to make a private instance of it. */
  if (position_to_this_list && !position_from_this_list)
    {
      private_from_this = true;
      position_from_this_list =
	(FriBidiStrIndex *) fribidi_malloc (sizeof (FriBidiStrIndex) *
					    length);
    }

  j = 0;
  for (i = 0; i < length; i++)
    if (!FRIBIDI_IS_EXPLICIT (fribidi_get_bidi_type (str[i]))
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
      DBG ("  converting from_this list to to_this");
      for (i = 0; i < length; i++)
	position_to_this_list[i] = -1;
      for (i = 0; i < length; i++)
	position_to_this_list[position_from_this_list[i]] = i;
      DBG ("  converting from_this list to to_this, done");
    }

  if (private_from_this)
    fribidi_free (position_from_this_list);

  DBG ("leaving fribidi_remove_bidi_marks");
  return j;
}


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
  FriBidiRun *main_run_list = NULL, *pp = NULL;
  FriBidiLevel max_level;
  fribidi_boolean private_V_to_L = false;
  fribidi_boolean status = true;

  DBG ("entering fribidi_log2vis()");

  fribidi_assert (str);
  fribidi_assert (pbase_dir);

  if (len == 0)
    goto out;

  if UNLIKELY
    (len > FRIBIDI_MAX_STRING_LENGTH && (position_V_to_L_list ||
					 position_L_to_V_list))
    {
#     if DEBUG
      MSG2 (FRIBIDI ": cannot handle strings > %ld characters\n",
	    (long) FRIBIDI_MAX_STRING_LENGTH);
#     endif /* DEBUG */
      status = false;
      goto out;
    }

  if UNLIKELY
    (!fribidi_analyse_string (str, len, pbase_dir,
			      /* output */
			      &main_run_list, &max_level))
    {
      status = false;
      goto out;
    }

  /* If l2v is to be calculated we must have v2l as well. If it is not
     given by the caller, we have to make a private instance of it. */
  if (position_L_to_V_list && !position_V_to_L_list)
    {
      position_V_to_L_list =
	(FriBidiStrIndex *) fribidi_malloc (sizeof (FriBidiStrIndex) * len);
      if (!position_V_to_L_list)
	{
	  status = false;
	  goto out;
	}
      private_V_to_L = true;
    }


  /* 7. Reordering resolved levels */
  DBG ("reordering resolved levels");
  {
    FriBidiLevel level_idx;
    FriBidiStrIndex i;

    /* Set up the ordering array to sorted order */
    if (position_V_to_L_list)
      {
	DBG ("  initialize position_V_to_L_list");
	for (i = 0; i < len; i++)
	  position_V_to_L_list[i] = i;
	DBG ("  initialize position_V_to_L_list, done");
      }
    /* Copy the logical string to the visual */
    if (visual_str)
      {
	DBG ("  initialize visual_str");
	for (i = 0; i < len; i++)
	  visual_str[i] = str[i];
	visual_str[len] = 0;
	DBG ("  initialize visual_str, done");
      }

    /* Assign the embedding level array */
    if (embedding_level_list)
      {
	DBG ("  fill the embedding levels array");
	for_run_list (pp, main_run_list)
	{
	  register FriBidiStrIndex i, pos = pp->pos, len = pp->len;
	  register FriBidiLevel level = pp->level;
	  for (i = 0; i < len; i++)
	    embedding_level_list[pos + i] = level;
	}
	DBG ("  fill the embedding levels array, done");
      }

    /* Reorder both the outstring and the order array */
    if (visual_str || position_V_to_L_list)
      {
	if (fribidi_mirroring_status () && visual_str)
	  {
	    /* L4. Mirror all characters that are in odd levels and have mirrors. */
	    DBG ("  mirroring");
	    for_run_list (pp, main_run_list)
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
	    DBG ("  mirroring, done");
	  }

	if (fribidi_reorder_nsm_status ())
	  {
	    /* L3. Reorder NSMs. */
	    DBG ("  reordering NSM sequences");
	    /* We apply this rule before L2, so go backward in odd levels. */
	    for_run_list (pp, main_run_list)
	    {
	      if (pp->level & 1)
		{
		  FriBidiStrIndex i, seq_end = 0;
		  fribidi_boolean is_nsm_seq;

		  is_nsm_seq = false;
		  for (i = RL_POS (pp) + RL_LEN (pp) - 1; i >= RL_POS (pp);
		       i--)
		    {
		      FriBidiCharType this_type;

		      this_type = fribidi_get_bidi_type (str[i]);
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
		      DBG ("warning: NSMs at the beggining of level run.\n");
		    }
		}
	    }
	    DBG ("  reordering NSM sequences, done");
	  }

	/* L2. Reorder. */
	DBG ("  reordering");
	for (level_idx = max_level; level_idx > 0; level_idx--)
	  {
	    for_run_list (pp, main_run_list)
	    {
	      if (RL_LEVEL (pp) >= level_idx)
		{
		  /* Find all stretches that are >= level_idx */
		  register FriBidiStrIndex len = pp->len, pos = pp->pos;
		  register FriBidiRun *pp1 = pp->next;
		  while (pp1->next && pp1->level >= level_idx)
		    {
		      len += pp1->len;
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
	DBG ("  reordering, done");
      }

    /* Convert the v2l list to l2v */
    if (position_L_to_V_list)
      {
	DBG ("  converting v2l list to l2v");
	for (i = 0; i < len; i++)
	  position_L_to_V_list[position_V_to_L_list[i]] = i;
	DBG ("  converting v2l list to l2v, done");
      }
  }
  DBG ("reordering resolved levels, done");

  DBG ("leaving fribidi_log2vis()");
out:
  if (private_V_to_L)
    fribidi_free (position_V_to_L_list);

  free_run_list (main_run_list);

  return status;

}

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
  FriBidiRun *main_run_list, *pp;
  FriBidiLevel max_level;
  fribidi_boolean status = true;

  DBG ("entering fribidi_log2vis_get_embedding_levels()");

  fribidi_assert (str);
  fribidi_assert (pbase_dir);

  if UNLIKELY
    (len == 0) goto out;

  if UNLIKELY
    (!fribidi_analyse_string (str, len, pbase_dir,
			      /* output */
			      &main_run_list, &max_level))
    {
      status = false;
      goto out;
    }

  for_run_list (pp, main_run_list)
  {
    register FriBidiStrIndex i, pos = pp->pos, len = pp->len;
    register FriBidiLevel level = pp->level;
    for (i = 0; i < len; i++)
      embedding_level_list[pos++] = level;
  }

  DBG ("leaving fribidi_log2vis_get_embedding_levels()");
out:
  free_run_list (main_run_list);

  return status;
}

/* Editor directions:
 * vim:textwidth=78:tabstop=8:shiftwidth=2:autoindent:cindent
 */
