/* Test of getdate() function.
   Copyright (C) 2008 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.  */

/* Written by Simon Josefsson <simon@josefsson.org>, 2008.  */

#include <config.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "getdate.h"

#define ASSERT(expr)							\
  do									\
    {									\
      if (!(expr))							\
	{								\
	  fprintf (stderr, "%s:%d: assertion failed\n",			\
		   __FILE__, __LINE__);					\
	  fflush (stderr);						\
	  abort ();							\
	}								\
    }									\
  while (0)

#ifdef DEBUG
#define LOG(str, now, res)						\
  printf ("string `%s' diff %d %d\n",			\
	  str, res.tv_sec - now.tv_sec, res.tv_nsec - now.tv_nsec);
#else
#define LOG(str, now, res) 0
#endif

int
main (int argc, char **argv)
{
  bool ret;
  struct timespec result;
  struct timespec result2;
  struct timespec now;
  const char *p;

  now.tv_sec = 4711;
  now.tv_nsec = 1267;
  p = "now";
  ASSERT (get_date (&result, p, &now));
  LOG (p, now, result);
  ASSERT (now.tv_sec == result.tv_sec && now.tv_nsec == result.tv_nsec);

  now.tv_sec = 4711;
  now.tv_nsec = 1267;
  p = "tomorrow";
  ASSERT (get_date (&result, p, &now));
  LOG (p, now, result);
  ASSERT (now.tv_sec + 24 * 60 * 60 == result.tv_sec
	  && now.tv_nsec == result.tv_nsec);

  now.tv_sec = 4711;
  now.tv_nsec = 1267;
  p = "yesterday";
  ASSERT (get_date (&result, p, &now));
  LOG (p, now, result);
  ASSERT (now.tv_sec - 24 * 60 * 60 == result.tv_sec
	  && now.tv_nsec == result.tv_nsec);

  now.tv_sec = 4711;
  now.tv_nsec = 1267;
  p = "4 hours";
  ASSERT (get_date (&result, p, &now));
  LOG (p, now, result);
  ASSERT (now.tv_sec + 4 * 60 * 60 == result.tv_sec
	  && now.tv_nsec == result.tv_nsec);

  /* test if timezone is not being ignored for day offset */
  now.tv_sec = 4711;
  now.tv_nsec = 1267;
  p = "UTC+400 +24 hours";
  ASSERT (get_date (&result, p, &now));
  LOG (p, now, result);
  p = "UTC+400 +1 day";
  ASSERT (get_date (&result2, p, &now));
  LOG (p, now, result2);
  ASSERT (result.tv_sec == result2.tv_sec
	  && result.tv_nsec == result2.tv_nsec);

  return 0;
}