/* Test of rounding to nearest, breaking ties away from zero.
   Copyright (C) 2007 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.  */

/* Written by Ben Pfaff <blp@gnu.org>, 2007.
   Based heavily on Bruno Haible's test-truncl.c. */

#include <config.h>

#include <math.h>

#include <stdio.h>
#include <stdlib.h>

#include "fpucw.h"
#include "isnanl-nolibm.h"

#define ASSERT(expr) \
  do									     \
    {									     \
      if (!(expr))							     \
        {								     \
          fprintf (stderr, "%s:%d: assertion failed\n", __FILE__, __LINE__); \
          abort ();							     \
        }								     \
    }									     \
  while (0)

int
main ()
{
  DECL_LONG_DOUBLE_ROUNDING

  BEGIN_LONG_DOUBLE_ROUNDING ();

  /* Zero.  */
  ASSERT (roundl (0.0L) == 0.0L);
  ASSERT (roundl (-0.0L) == 0.0L);
  /* Positive numbers.  */
  ASSERT (roundl (0.3L) == 0.0L);
  ASSERT (roundl (0.5L) == 1.0L);
  ASSERT (roundl (0.7L) == 1.0L);
  ASSERT (roundl (1.0L) == 1.0L);
  ASSERT (roundl (1.5L) == 2.0L);
  ASSERT (roundl (2.5L) == 3.0L);
  ASSERT (roundl (1.999L) == 2.0L);
  ASSERT (roundl (2.0L) == 2.0L);
  ASSERT (roundl (65535.999L) == 65536.0L);
  ASSERT (roundl (65536.0L) == 65536.0L);
  ASSERT (roundl (65536.001L) == 65536.0L);
  ASSERT (roundl (2.341e31L) == 2.341e31L);
  /* Negative numbers.  */
  ASSERT (roundl (-0.3L) == 0.0L);
  ASSERT (roundl (-0.5L) == -1.0L);
  ASSERT (roundl (-0.7L) == -1.0L);
  ASSERT (roundl (-1.0L) == -1.0L);
  ASSERT (roundl (-1.5L) == -2.0L);
  ASSERT (roundl (-2.5L) == -3.0L);
  ASSERT (roundl (-1.999L) == -2.0L);
  ASSERT (roundl (-2.0L) == -2.0L);
  ASSERT (roundl (-65535.999L) == -65536.0L);
  ASSERT (roundl (-65536.0L) == -65536.0L);
  ASSERT (roundl (-65536.001L) == -65536.0L);
  ASSERT (roundl (-2.341e31L) == -2.341e31L);
  /* Infinite numbers.  */
  ASSERT (roundl (1.0 / 0.0L) == 1.0 / 0.0L);
  ASSERT (roundl (-1.0 / 0.0L) == -1.0 / 0.0L);
  /* NaNs.  */
  ASSERT (isnanl (roundl (0.0L / 0.0L)));

  return 0;
}