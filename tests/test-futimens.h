/* Test of file timestamp modification functions.
   Copyright (C) 2009 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#include "test-utimens-common.h"

/* This file is designed to test both gl_futimens(a,NULL,b) and
   futimens(a,b).  FUNC is the function to test.  Assumes that BASE
   and ASSERT are already defined.  If PRINT, warn before skipping
   tests with status 77.  */
static int
test_futimens (int (*func) (int, struct timespec const *),
               bool print)
{
  int fd = creat (BASE "file", 0600);
  int result;
  struct stat st1;
  struct stat st2;
  ASSERT (0 <= fd);

  /* Sanity check.  */
  ASSERT (fstat (fd, &st1) == 0);
  nap ();
  errno = 0;
  result = func (fd, NULL);
  if (result == -1 && errno == ENOSYS)
    {
      ASSERT (close (fd) == 0);
      ASSERT (unlink (BASE "file") == 0);
      if (print)
        fputs ("skipping test: "
               "setting fd time not supported on this file system\n",
               stderr);
      return 77;
    }
  ASSERT (!result);
  ASSERT (fstat (fd, &st2) == 0);
  /* If utimens truncates to less resolution than the file system
     supports, then time can appear to go backwards between now and a
     follow-up utimens with UTIME_NOW or a NULL timespec.  Use
     UTIMECMP_TRUNCATE_SOURCE to compensate, with st1 as the
     source.  */
  ASSERT (0 <= utimecmp (BASE "file", &st2, &st1, UTIMECMP_TRUNCATE_SOURCE));
  {
    /* On some NFS systems, the 'now' timestamp of creat or a NULL
       timespec is determined by the server, but the 'now' timestamp
       determined by gettime() (as is done when using UTIME_NOW) is
       determined by the client; since the two machines are not
       necessarily on the same clock, this is another case where time
       can appear to go backwards.  The rest of this test cares about
       client time, so manually use gettime() to set both times.  */
    struct timespec ts[2];
    gettime (&ts[0]);
    ts[1] = ts[0];
    ASSERT (func (fd, ts) == 0);
    ASSERT (fstat (fd, &st1) == 0);
    nap ();
  }

  /* Invalid arguments.  */
  errno = 0;
  ASSERT (func (AT_FDCWD, NULL) == -1);
  ASSERT (errno == EBADF);
  {
    struct timespec ts[2] = { { Y2K, UTIME_BOGUS_POS }, { Y2K, 0 } };
    errno = 0;
    ASSERT (func (fd, ts) == -1);
    ASSERT (errno == EINVAL);
  }
  {
    struct timespec ts[2] = { { Y2K, 0 }, { Y2K, UTIME_BOGUS_NEG } };
    errno = 0;
    ASSERT (func (fd, ts) == -1);
    ASSERT (errno == EINVAL);
  }
  ASSERT (fstat (fd, &st2) == 0);
  ASSERT (st1.st_atime == st2.st_atime);
  ASSERT (get_stat_atime_ns (&st1) == get_stat_atime_ns (&st2));
  ASSERT (utimecmp (BASE "file", &st1, &st2, 0) == 0);

  /* Set both times.  */
  {
    struct timespec ts[2] = { { Y2K, BILLION / 2 - 1 }, { Y2K, BILLION - 1 } };
    ASSERT (func (fd, ts) == 0);
    ASSERT (fstat (fd, &st2) == 0);
    ASSERT (st2.st_atime == Y2K);
    ASSERT (0 <= get_stat_atime_ns (&st2));
    ASSERT (get_stat_atime_ns (&st2) < BILLION / 2);
    ASSERT (st2.st_mtime == Y2K);
    ASSERT (0 <= get_stat_mtime_ns (&st2));
    ASSERT (get_stat_mtime_ns (&st2) < BILLION);
  }

  /* Play with UTIME_OMIT, UTIME_NOW.  */
  {
    struct timespec ts[2] = { { BILLION, UTIME_OMIT }, { 0, UTIME_NOW } };
    ASSERT (func (fd, ts) == 0);
    ASSERT (fstat (fd, &st2) == 0);
    ASSERT (st2.st_atime == Y2K);
    ASSERT (0 <= get_stat_atime_ns (&st2));
    ASSERT (get_stat_atime_ns (&st2) <= BILLION / 2);
    ASSERT (utimecmp (BASE "file", &st1, &st2, 0) <= 0);
  }

  /* Cleanup.  */
  ASSERT (close (fd) == 0);
  ASSERT (unlink (BASE "file") == 0);
  return 0;
}