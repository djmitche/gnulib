#!/bin/sh
# Test suite for update-copyright.
# Copyright (C) 2009 Free Software Foundation, Inc.
# This file is part of the GNUlib Library.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

diffout=`diff -u /dev/null /dev/null 2>&1`
if test x"$diffout" = x"" && test $? -eq 0; then
  compare() { diff -u "$@"; }
else
  compare() { cmp "$@"; }
fi

TMP_BASE=update-copyright.test

## ----------------------------- ##
## Examples from documentation.  ##
## ----------------------------- ##

TMP=$TMP_BASE-ex
cat > $TMP.1 <<EOF
Copyright @copyright{} 1990-2005, 2007-2009 Free Software
Foundation, Inc.
EOF
cat > $TMP.2 <<EOF
# Copyright (C) 1990-2005, 2007-2009 Free Software
# Foundation, Inc.
EOF
cat > $TMP.3 <<EOF
/*
 * Copyright &copy; 90,2005,2007-2009
 * Free Software Foundation, Inc.
 */
EOF
cat > $TMP.4 <<EOF
## Copyright (C) 1990-2005, 2007-2009 Free Software
#  Foundation, Inc.
EOF
cat > $TMP.5 <<EOF
Copyright (C) 1990-2005, 2007-2009 Acme, Inc.
EOF
cat > $TMP.6 <<EOF
## Copyright (C) 1990-2005, 2007-2009 Free Software
#  Foundation, Inc.

Copyright (C) 1990-2005, 2007-2009 Free Software Foundation,
Inc.
EOF
cat > $TMP.7 <<EOF
Copyright (C) 1990-2005, 2007-2009 Acme, Inc.

# Copyright (C) 1990-2005, 2007-2009 Free Software
# Foundation, Inc.
EOF

UPDATE_COPYRIGHT_YEAR=2009 \
  update-copyright $TMP.* 1> $TMP-stdout 2> $TMP-stderr
compare /dev/null $TMP-stdout || exit 1
compare - $TMP-stderr <<EOF || exit 1
$TMP.4: warning: FSF copyright statement not found
$TMP.5: warning: FSF copyright statement not found
EOF
compare - $TMP.1 <<EOF || exit 1
Copyright @copyright{} 1990-2005, 2007-2009 Free Software
Foundation, Inc.
EOF
compare - $TMP.2 <<EOF || exit 1
# Copyright (C) 1990-2005, 2007-2009 Free Software
# Foundation, Inc.
EOF
compare - $TMP.3 <<EOF || exit 1
/*
 * Copyright &copy; 90,2005,2007-2009
 * Free Software Foundation, Inc.
 */
EOF
compare - $TMP.4 <<EOF || exit 1
## Copyright (C) 1990-2005, 2007-2009 Free Software
#  Foundation, Inc.
EOF
compare - $TMP.5 <<EOF || exit 1
Copyright (C) 1990-2005, 2007-2009 Acme, Inc.
EOF
compare - $TMP.6 <<EOF || exit 1
## Copyright (C) 1990-2005, 2007-2009 Free Software
#  Foundation, Inc.

Copyright (C) 1990-2005, 2007-2009 Free Software Foundation,
Inc.
EOF
compare - $TMP.7 <<EOF || exit 1
Copyright (C) 1990-2005, 2007-2009 Acme, Inc.

# Copyright (C) 1990-2005, 2007-2009 Free Software
# Foundation, Inc.
EOF

UPDATE_COPYRIGHT_YEAR=2010 \
  update-copyright $TMP.* 1> $TMP-stdout 2> $TMP-stderr
compare /dev/null $TMP-stdout || exit 1
compare - $TMP-stderr <<EOF || exit 1
$TMP.4: warning: FSF copyright statement not found
$TMP.5: warning: FSF copyright statement not found
EOF
compare - $TMP.1 <<EOF || exit 1
Copyright @copyright{} 1990-2005, 2007-2010 Free Software Foundation,
Inc.
EOF
compare - $TMP.2 <<EOF || exit 1
# Copyright (C) 1990-2005, 2007-2010 Free Software Foundation, Inc.
EOF
compare - $TMP.3 <<EOF || exit 1
/*
 * Copyright &copy; 90, 2005, 2007-2010 Free Software Foundation, Inc.
 */
EOF
compare - $TMP.4 <<EOF || exit 1
## Copyright (C) 1990-2005, 2007-2009 Free Software
#  Foundation, Inc.
EOF
compare - $TMP.5 <<EOF || exit 1
Copyright (C) 1990-2005, 2007-2009 Acme, Inc.
EOF
compare - $TMP.6 <<EOF || exit 1
## Copyright (C) 1990-2005, 2007-2009 Free Software
#  Foundation, Inc.

Copyright (C) 1990-2005, 2007-2010 Free Software Foundation, Inc.
EOF
compare - $TMP.7 <<EOF || exit 1
Copyright (C) 1990-2005, 2007-2009 Acme, Inc.

# Copyright (C) 1990-2005, 2007-2010 Free Software Foundation, Inc.
EOF

rm $TMP*

## -------------- ##
## Current year.  ##
## -------------- ##

TMP=$TMP_BASE-current-year
YEAR=`/usr/bin/perl -e 'print [localtime]->[5] + 1900'`;
cat > $TMP <<EOF
'\" Copyright (C) 2006
'\" Free Software Foundation,
'\" Inc.
EOF
update-copyright $TMP 1> $TMP-stdout 2> $TMP-stderr
compare /dev/null $TMP-stdout || exit 1
compare /dev/null $TMP-stderr || exit 1
compare - $TMP <<EOF || exit 1
'\" Copyright (C) 2006, $YEAR Free Software Foundation, Inc.
EOF
rm $TMP*

## ------------------ ##
## Surrounding text.  ##
## ------------------ ##

TMP=$TMP_BASE-surrounding-text
cat > $TMP <<EOF
    Undisturbed text.
dnl Undisturbed text.
dnl Copyright (C) 89
dnl Free Software Foundation, Inc.
dnl   Undisturbed text.
EOF
UPDATE_COPYRIGHT_YEAR=2010 \
  update-copyright $TMP 1> $TMP-stdout 2> $TMP-stderr
compare /dev/null $TMP-stdout || exit 1
compare /dev/null $TMP-stderr || exit 1
compare - $TMP <<EOF || exit 1
    Undisturbed text.
dnl Undisturbed text.
dnl Copyright (C) 1989, 2010 Free Software Foundation, Inc.
dnl   Undisturbed text.
EOF
rm $TMP*

## --------------- ##
## Widest prefix.  ##
## --------------- ##

TMP=$TMP_BASE-widest-prefix
cat > $TMP <<EOF
#### Copyright (C) 1976, 1977, 1978, 1979, 1980, 1981, 1982, 1983, 1984, 1985,
#### 1986, 1987, 1988, 1999, 2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007,
#### 2008 Free Software Foundation, Inc.
EOF
UPDATE_COPYRIGHT_YEAR=2010 \
  update-copyright $TMP 1> $TMP-stdout 2> $TMP-stderr
compare /dev/null $TMP-stdout || exit 1
compare /dev/null $TMP-stderr || exit 1
compare - $TMP <<EOF || exit 1
#### Copyright (C) 1976, 1977, 1978, 1979, 1980, 1981, 1982, 1983, 1984,
#### 1985, 1986, 1987, 1988, 1999, 2000, 2001, 2002, 2003, 2004, 2005,
#### 2006, 2007, 2008, 2010 Free Software Foundation, Inc.
EOF
rm $TMP*

## ------------------- ##
## Prefix too large.  ##
## ------------------- ##

TMP=$TMP_BASE-prefix-too-large
cat > $TMP <<EOF
####  Copyright (C) 1976, 1977, 1978, 1979, 1980, 1981, 1982, 1983, 1984, 1985,
####  1986, 1987, 1988, 1999, 2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007,
####  2008 Free Software Foundation, Inc.
EOF
UPDATE_COPYRIGHT_YEAR=2010 \
  update-copyright $TMP 1> $TMP-stdout 2> $TMP-stderr
compare /dev/null $TMP-stdout || exit 1
compare - $TMP-stderr <<EOF || exit 1
$TMP: warning: FSF copyright statement not found
EOF
compare - $TMP <<EOF || exit 1
####  Copyright (C) 1976, 1977, 1978, 1979, 1980, 1981, 1982, 1983, 1984, 1985,
####  1986, 1987, 1988, 1999, 2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007,
####  2008 Free Software Foundation, Inc.
EOF
rm $TMP*

## ------------- ##
## Blank lines.  ##
## ------------- ##

TMP=$TMP_BASE-blank-lines
cat > $TMP <<EOF
#Copyright (C) 1976, 1977, 1978, 1979, 1980, 1981, 1982, 1983, 1984, 1985,
#
#1986, 1987, 1988, 1999, 2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007,
#2008 Free Software Foundation, Inc.

Copyright (C) 1976, 1977, 1978, 1979, 1980, 1981, 1982, 1983, 1984, 1985,

1986, 1987, 1988, 1999, 2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007,
2008 Free Software Foundation, Inc.
EOF
UPDATE_COPYRIGHT_YEAR=2010 \
  update-copyright $TMP 1> $TMP-stdout 2> $TMP-stderr
compare /dev/null $TMP-stdout || exit 1
compare - $TMP-stderr <<EOF || exit 1
$TMP: warning: FSF copyright statement not found
EOF
compare - $TMP <<EOF || exit 1
#Copyright (C) 1976, 1977, 1978, 1979, 1980, 1981, 1982, 1983, 1984, 1985,
#
#1986, 1987, 1988, 1999, 2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007,
#2008 Free Software Foundation, Inc.

Copyright (C) 1976, 1977, 1978, 1979, 1980, 1981, 1982, 1983, 1984, 1985,

1986, 1987, 1988, 1999, 2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007,
2008 Free Software Foundation, Inc.
EOF
rm $TMP*

## -------------- ##
## Leading tabs.  ##
## -------------- ##

TMP=$TMP_BASE-leading-tabs
cat > $TMP <<EOF
	Copyright (C) 87, 88, 1991, 1992, 1993, 1994, 1995, 1996, 1997, 98,
	 1999, 2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009 Free
	Software Foundation, Inc.
EOF
UPDATE_COPYRIGHT_YEAR=2010 \
  update-copyright $TMP 1> $TMP-stdout 2> $TMP-stderr
compare /dev/null $TMP-stdout || exit 1
compare /dev/null $TMP-stderr || exit 1
compare - $TMP <<EOF || exit 1
	Copyright (C) 87, 88, 1991, 1992, 1993, 1994, 1995, 1996, 1997,
	98, 1999, 2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008,
	2009-2010 Free Software Foundation, Inc.
EOF
rm $TMP*

## -------------------- ##
## Unusual whitespace.  ##
## -------------------- ##

TMP=$TMP_BASE-unusual-ws
cat > $TMP <<EOF
		# Copyright (C) 87, 88, 1991, 1992, 1993, 1994, 1995, 1996, 1997,
		# 98, 1999, 2000, 2001, 2002, 2003,     		  2004, 2005, 2006, 2007, 2008,
		# 2009 Free Software Foundation, Inc.
EOF
UPDATE_COPYRIGHT_YEAR=2010 \
  update-copyright $TMP 1> $TMP-stdout 2> $TMP-stderr
compare /dev/null $TMP-stdout || exit 1
compare /dev/null $TMP-stderr || exit 1
compare - $TMP <<EOF || exit 1
		# Copyright (C) 87, 88, 1991, 1992, 1993, 1994, 1995,
		# 1996, 1997, 98, 1999, 2000, 2001, 2002, 2003, 2004,
		# 2005, 2006, 2007, 2008, 2009-2010 Free Software
		# Foundation, Inc.
EOF
rm $TMP*

## --------- ##
## DOS EOL.  ##
## --------- ##

TMP=$TMP_BASE-dos-eol
tr @ '\015' > $TMP <<\EOF
Rem Copyright (C) 87, 88, 1991, 1992, 1993, 1994, 1995, 1996, 1997,@
Rem 98, 1999, 2000, 2001, 2002, 2003,  2004, 2005, 2006, 2007, 2008,@
Rem 2009 Free Software Foundation, Inc.@
EOF
UPDATE_COPYRIGHT_YEAR=2010 \
  update-copyright $TMP 1> $TMP-stdout 2> $TMP-stderr
compare /dev/null $TMP-stdout || exit 1
compare /dev/null $TMP-stderr || exit 1
tr @ '\015' > $TMP-exp <<\EOF
Rem Copyright (C) 87, 88, 1991, 1992, 1993, 1994, 1995, 1996, 1997, 98,@
Rem 1999, 2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008,@
Rem 2009-2010 Free Software Foundation, Inc.@
EOF
compare $TMP-exp $TMP || exit 1
rm $TMP*

## --------------- ##
## Omitted "(C)".  ##
## --------------- ##

TMP=$TMP_BASE-omitted-circle-c
cat > $TMP <<EOF
  Copyright 87, 88, 1991, 1992, 1993, 1994, 1995, 1996, 1997,
  98, 1999, 2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008,
  2009 Free Software Foundation, Inc.
EOF
UPDATE_COPYRIGHT_YEAR=2010 \
  update-copyright $TMP 1> $TMP-stdout 2> $TMP-stderr
compare /dev/null $TMP-stdout || exit 1
compare /dev/null $TMP-stderr || exit 1
compare - $TMP <<EOF || exit 1
  Copyright 87, 88, 1991, 1992, 1993, 1994, 1995, 1996, 1997, 98, 1999,
  2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009-2010 Free
  Software Foundation, Inc.
EOF
rm $TMP*

## ------------------ ##
## C-style comments.  ##
## ------------------ ##

TMP=$TMP_BASE-c-style-comments
cat > $TMP.star <<EOF
/*  Copyright 87, 88, 1991, 1992, 1993, 1994, 1995, 1996, 1997,
 *  98, 1999, 2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008,
 *  2009 Free Software Foundation, Inc.  */
EOF
cat > $TMP.space <<EOF
  /*Copyright 87, 88, 1991, 1992, 1993, 1994, 1995, 1996, 1997,
    98, 1999, 2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008,
    2009 Free Software Foundation, Inc.  */
EOF
cat > $TMP.single-line <<EOF
/* Copyright 87, 88, 1991, 1992 Free Software Foundation, Inc.  */
EOF
cat > $TMP.single-line-wrapped <<EOF
 /* Copyright 1988, 1991, 1992, 1993 Free Software Foundation, Inc.  */
EOF
cat > $TMP.extra-text-star <<EOF
 /* Copyright 1987, 1988, 1991, 1992 Free Software Foundation, Inc.  End
  * More comments.  */
EOF
cat > $TMP.extra-text-space <<EOF
 /* Copyright 1987, 1988, 1991, 1992 Free Software Foundation, Inc. ***
    * End of comments. */
EOF
UPDATE_COPYRIGHT_YEAR=2010 \
  update-copyright $TMP.* 1> $TMP-stdout 2> $TMP-stderr
compare /dev/null $TMP-stdout || exit 1
compare /dev/null $TMP-stderr || exit 1
compare - $TMP.star <<EOF || exit 1
/*  Copyright 87, 88, 1991, 1992, 1993, 1994, 1995, 1996, 1997, 98,
 *  1999, 2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008,
 *  2009-2010 Free Software Foundation, Inc.  */
EOF
compare - $TMP.space <<EOF || exit 1
  /*Copyright 87, 88, 1991, 1992, 1993, 1994, 1995, 1996, 1997, 98,
    1999, 2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008,
    2009-2010 Free Software Foundation, Inc.  */
EOF
compare - $TMP.single-line <<EOF || exit 1
/* Copyright 87, 88, 1991, 1992, 2010 Free Software Foundation, Inc.  */
EOF
compare - $TMP.single-line-wrapped <<EOF || exit 1
 /* Copyright 1988, 1991, 1992, 1993, 2010 Free Software Foundation,
  * Inc.  */
EOF
compare - $TMP.extra-text-star <<EOF || exit 1
 /* Copyright 1987, 1988, 1991, 1992, 2010 Free Software Foundation,
  * Inc.  End
  * More comments.  */
EOF
compare - $TMP.extra-text-space <<EOF || exit 1
 /* Copyright 1987, 1988, 1991, 1992, 2010 Free Software Foundation,
    Inc. ***
    * End of comments. */
EOF
rm $TMP*

exit 0