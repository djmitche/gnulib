/* Skipping input from a FILE stream.
   Copyright (C) 2007-2008 Free Software Foundation, Inc.

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
#include "freadseek.h"

#include <stdlib.h>
#include <unistd.h>

#include "freadahead.h"

int
freadseek (FILE *fp, size_t offset)
{
  size_t buffered;
  int fd;

  if (offset == 0)
    return 0;

  /* Increment the in-memory pointer.  This is very cheap (no system calls).  */
  buffered = freadahead (fp);
  if (buffered > 0)
    {
      size_t increment = (buffered < offset ? buffered : offset);

      /* Keep this code in sync with freadahead and freadptr!  */
#if defined _IO_ferror_unlocked     /* GNU libc, BeOS */
      fp->_IO_read_ptr += increment;
#elif defined __sferror             /* FreeBSD, NetBSD, OpenBSD, MacOS X, Cygwin */
      fp->_p += increment;
      fp->_r -= increment;
#elif defined _IOERR                /* AIX, HP-UX, IRIX, OSF/1, Solaris, mingw */
# if defined __sun && defined _LP64 /* Solaris/{SPARC,AMD64} 64-bit */
#  define fp_ ((struct { unsigned char *_ptr; \
			 unsigned char *_base; \
			 unsigned char *_end; \
			 long _cnt; \
			 int _file; \
			 unsigned int _flag; \
		       } *) fp)
      fp_->_ptr += increment;
      fp_->_cnt -= increment;
# else
      fp->_ptr += increment;
      fp->_cnt -= increment;
# endif
#elif defined __UCLIBC__            /* uClibc */
# ifdef __STDIO_BUFFERS
      fp->__bufpos += increment;
# else
      abort ();
# endif
#elif defined __QNX__               /* QNX */
      fp->_Next += increment;
#else
 #error "Please port gnulib freadseek.c to your platform! Look at the definition of getc, getc_unlocked on your system, then report this to bug-gnulib."
#endif

      offset -= increment;
      if (offset == 0)
	return 0;
    }

  /* Test whether the stream is seekable or not.  */
  fd = fileno (fp);
  if (fd >= 0 && lseek (fd, 0, SEEK_CUR) >= 0)
    {
      /* FP refers to a regular file.  fseek is most efficient in this case.  */
      return fseek (fp, offset, SEEK_CUR);
    }
  else
    {
      /* FP is a non-seekable stream, possibly not even referring to a file
	 descriptor.  Read OFFSET bytes explicitly and discard them.  */
      char buf[4096];

      do
	{
	  size_t count = (sizeof (buf) < offset ? sizeof (buf) : offset);
	  if (fread (buf, 1, count, fp) < count)
	    {
	      if (ferror (fp))
		/* EOF, or error before or while reading.  */
		return EOF;
	      else
		/* Encountered EOF.  */
		return 0;
	    }
	  offset -= count;
	}
      while (offset > 0);

      return 0;
   }
}