/* Test of test whether an UTF-32 string is entirely upper case.
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

/* Written by Bruno Haible <bruno@clisp.org>, 2009.  */

#include <config.h>

#include "unicase.h"

#include <stdio.h>
#include <stdlib.h>

#include "unistr.h"

#define SIZEOF(array) (sizeof (array) / sizeof (array[0]))
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

#define UNIT uint32_t
#include "test-is-uppercase.h"
#undef UNIT

static void
test_nonascii (int (*my_is) (const uint32_t *, size_t, const char *, bool *))
{
  /* Test cases from Unicode 5.1.0.  */
  {
    static const uint32_t input[] = { 0x24D7 };
    bool result;

    ASSERT (my_is (input, SIZEOF (input), NULL, &result) == 0);
    ASSERT (result == false);
  }
  {
    static const uint32_t input[] = { 0x24BD };
    bool result;

    ASSERT (my_is (input, SIZEOF (input), NULL, &result) == 0);
    ASSERT (result == true);
  }
  {
    static const uint32_t input[] = { 0x02B0 };
    bool result;

    ASSERT (my_is (input, SIZEOF (input), NULL, &result) == 0);
    ASSERT (result == true);
  }
  {
    static const uint32_t input[] = { 0x1D34 };
    bool result;

    ASSERT (my_is (input, SIZEOF (input), NULL, &result) == 0);
    ASSERT (result == true);
  }
  {
    static const uint32_t input[] = { 0x02BD };
    bool result;

    ASSERT (my_is (input, SIZEOF (input), NULL, &result) == 0);
    ASSERT (result == true);
  }
}

int
main ()
{
  test_ascii (u32_is_uppercase);
  test_nonascii (u32_is_uppercase);

  return 0;
}