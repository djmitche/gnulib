/* Copyright (C) 2000 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

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

#include <config.h>

/* Specification.  */
#include <spawn.h>

/* Store scheduling parameters in the attribute structure.  */
int
posix_spawnattr_setschedparam (posix_spawnattr_t *attr,
			       const struct sched_param *schedparam)
{
  /* Store the scheduling parameters.  */
  attr->_sp = *schedparam;

  return 0;
}