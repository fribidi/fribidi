/* FriBidi - Library of BiDi algorithm
 * Copyright (C) 2001,2002 Behdad Esfahbod. 
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
 * For licensing issues, contact <fwpg@sharif.edu>. 
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "packtab.h"
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "fribidi_unicode.h"

#ifndef SIZEOF_CHAR
#define SIZEOF_CHAR sizeof (char)
#endif

static void
err (
  char *msg
)
{
  fprintf (stderr, "fribidi_create_char_types: %s\n", msg);
  exit (1);
}

static void
err2 (
  char *fmt,
  char *p
)
{
  fprintf (stderr, "fribidi_create_char_types: ");
  fprintf (stderr, fmt, p);
  fprintf (stderr, "\n");
  exit (1);
}

/* *INDENT-OFF* */
struct
{
  char *name;
  int key;
}
type_names[] =
{
  {"LTR", 0}, {"L", 0},
  {"RTL", 1}, {"R", 1},
  {"AL", 2},
  {"ON", 3},
  {"BN", 4},
  {"AN", 5},
  {"BS", 6}, {"B", 6},
  {"CS", 7},
  {"EN", 8},
  {"ES", 9},
  {"ET", 10},
  {"LRE", 11},
  {"LRO", 12},
  {"NSM", 13},
  {"PDF", 14},
  {"RLE", 15},
  {"RLO", 16},
  {"SS", 17}, {"S", 17},
  {"WS", 18},
};
/* *INDENT-ON* */

#define type_names_count (sizeof (type_names) / sizeof (type_names[0]))

static char *names[type_names_count];

static char *unidata_file;

static char
get_type (
  char *s
)
{
  int i;

  for (i = 0; i < type_names_count; i++)
    if (!strcmp (s, type_names[i].name))
      return type_names[i].key;
  err2 ("type name `%s' not found", s);
  return 0;
}

#define table_name "FriBidiPropertyBlock"
#define key_type_name "FriBidiPropCharType"
#define macro_name "FRIBIDI_GET_TYPE"
#define function_name "fribidi_get_type_internal"
#define char_type_name "FriBidiCharType"
#define char_name "FriBidiChar"
#define prop_to_type_name "fribidi_prop_to_type"
#define default_type "LTR"
#define export_api "FRIBIDI_API"

static int table[FRIBIDI_UNICODE_CHARS];

static void
init_table (
)
{
  int i;
  int deftype = get_type (default_type),
    RTL = get_type ("RTL"), AL = get_type ("AL");

  for (i = 0; i < type_names_count; i++)
    names[i] = 0;
  for (i = type_names_count - 1; i >= 0; i--)
    names[type_names[i].key] = type_names[i].name;

  /* initialize table */
  for (i = 0; i < FRIBIDI_UNICODE_CHARS; i++)
    table[i] = deftype;
  for (i = 0x590; i < 0x600; i++)
    table[i] = RTL;
  for (i = 0xFB1D; i < 0xFB50; i++)
    table[i] = RTL;
  for (i = 0x600; i < 0x7C0; i++)
    table[i] = AL;
  for (i = 0xFB50; i < 0xFE00; i++)
    table[i] = AL;
  for (i = 0xFE70; i < 0xFF00; i++)
    table[i] = AL;
}

static void
read_unicode_data (
)
{
  char s[500], tp[10];
  unsigned int i;
  FILE *f;

  printf ("Reading `UnicodeData.txt'\n");
  if (!(f = fopen (unidata_file, "rt")))
    err2 ("error: cannot open `%s' for reading", unidata_file);
  while (fgets (s, sizeof s, f))
    {
      sscanf (s, "%x;%*[^;];%*[^;];%*[^;];%[^;]", &i, tp);
      table[i] = get_type (tp);
    }
  fclose (f);
}

static char *
headermacro (
  char *file
)
{
  char *t = strdup (file);
  char *p = t;
  while (*p)
    {
      if (*p >= 'a' && *p <= 'z')
	*p += 'A' - 'a';
      else if ((*p < 'A' || *p > 'Z') && (*p < '0' || *p > '9'))
	*p = '_';
      p++;
    }
  return t;
}

static void
write_char_type (
  char *file,
  int max_depth
)
{
  int i;
  FILE *f;
  char *FILENAME = headermacro (file);

  printf ("Writing `%s', it may take a few minutes\n", file);
  if (!(f = fopen (file, "wt")))
    err2 ("error: cannot open `%s' for writing", file);
  fprintf (f, "/*\n"
	   "  This file was automatically created from UnicodeData.txt version %s\n"
	   "  by fribidi_create_char_types\n*/\n\n", FRIBIDI_UNICODE_VERSION);

  fprintf (f, "#ifndef %s\n#define %s\n\n#include \"fribidi.h\"\n\n",
	   FILENAME, FILENAME);

  for (i = 0; i < type_names_count; i++)
    if (names[i])
      fprintf (f, "#define %s FRIBIDI_PROP_TYPE_%s\n", names[i], names[i]);
  fprintf (f, "\n");

  fprintf (f, "#define PACKTAB_UINT8 fribidi_uint8\n");
  fprintf (f, "#define PACKTAB_UINT16 fribidi_uint16\n");
  fprintf (f, "#define PACKTAB_UINT32 fribidi_uint32\n");

  if (!pack_table
      (table, FRIBIDI_UNICODE_CHARS, SIZEOF_CHAR, max_depth, 3, names,
       key_type_name, table_name, macro_name, f))
    err ("error: insufficient memory, decrease max_depth");

  for (i = type_names_count - 1; i >= 0; i--)
    if (names[i])
      fprintf (f, "#undef %s\n", names[i]);

  fprintf (f,
	   "/*======================================================================\n"
	   " *  %s() returns the bidi type of a character.\n"
	   " *----------------------------------------------------------------------*/\n"
	   "%s %s\n"
	   "%s (%s uch)\n"
	   "{\n"
	   "  if (uch < 0x%x)\n"
	   "    return %s[(unsigned char)%s (uch)];\n"
	   "  else\n"
	   "    return FRIBIDI_TYPE_%s;\n"
	   "  /* Non-Unicode chars */\n"
	   "}\n"
	   "\n",
	   function_name, export_api, char_type_name, function_name,
	   char_name, FRIBIDI_UNICODE_CHARS, prop_to_type_name, macro_name,
	   default_type);
  fprintf (f, "\n#endif /* %s */\n", FILENAME);

  fclose (f);
}

int
main (
  int argc,
  char **argv
)
{
  int max_depth;
  char file[50], *p;
  if (argc < 2)
    err ("usage: fribidi_create_char_types max_depth [UnicodeData.txt path]");
  p = (argc >= 3) ? argv[2] : "unidata";
  unidata_file = malloc (50 + strlen (p));
  sprintf (unidata_file, "%s/UnicodeData.txt", p);
  max_depth = atoi (argv[1]);
  if (!max_depth)
    err ("invalid depth");
  if (max_depth < 2 || max_depth > 9)
    err2 ("invalid max_depth `%s', max_depth should be between 2 and 9",
	  argv[1]);
  sprintf (file, "fribidi_tab_char_type_%d.i", max_depth);
  init_table ();
  read_unicode_data ();
  write_char_type (file, max_depth);
  return 0;
}
