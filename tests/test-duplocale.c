/* Test of duplicating a locale object.
   Copyright (C) 2009 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* Written by Bruno Haible <bruno@clisp.org>, 2007.  */

#include <config.h>

#include <locale.h>

#if HAVE_DUPLOCALE

#include <langinfo.h>
#include <monetary.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ASSERT(expr) \
  do									     \
    {									     \
      if (!(expr))							     \
        {								     \
          fprintf (stderr, "%s:%d: assertion failed\n", __FILE__, __LINE__); \
          fflush (stderr);						     \
          abort ();							     \
        }								     \
    }									     \
  while (0)

struct locale_dependent_values
{
  char monetary[100];
  char numeric[100];
  char time[100];
};

static void
get_locale_dependent_values (struct locale_dependent_values *result)
{
  strfmon (result->monetary, sizeof (result->monetary),
	   "%n", 123.75);
  /* result->monetary is usually "$123.75" */
  snprintf (result->numeric, sizeof (result->numeric),
	    "%g", 3.5);
  /* result->numeric is usually "3,5" */
  strcpy (result->time, nl_langinfo (MON_1));
  /* result->time is usually "janvier" */
}

int
main ()
{
  struct locale_dependent_values expected_results;
  locale_t mixed;

  /* Set up a locale which is a mix between different system locales.  */
  setlocale (LC_ALL, "en_US.UTF-8");
  setlocale (LC_NUMERIC, "de_DE.UTF-8");
  setlocale (LC_TIME, "fr_FR.UTF-8");
  get_locale_dependent_values (&expected_results);

  /* Save the locale in a locale_t object.  */
  mixed = duplocale (LC_GLOBAL_LOCALE);
  ASSERT (mixed != NULL);

  /* Set up a default locale.  */
  setlocale (LC_ALL, "C");
  {
    struct locale_dependent_values c_results;
    get_locale_dependent_values (&c_results);
  }

  /* Now use the saved locale again.  */
  uselocale (mixed);
  {
    struct locale_dependent_values results;
    get_locale_dependent_values (&results);
    ASSERT (strcmp (results.monetary, expected_results.monetary) == 0);
    ASSERT (strcmp (results.numeric, expected_results.numeric) == 0);
    ASSERT (strcmp (results.time, expected_results.time) == 0);
  }

  return 0;
}

#else

int
main ()
{
  return 77;
}

#endif