/********************************************
version.c
copyright 1991-95.  Michael D. Brennan

This is a source file for mawk, an implementation of
the AWK programming language.

Mawk is distributed without warranty under the terms of
the GNU General Public License, version 2, 1991.
********************************************/

#include "mawk.h"
#include "patchlev.h"

#define	 VERSION_STRING	 \
  "tawk 1.3.1 Copyright (C) John Roll 1995,2002\n from\nmawk 1.3 %s %s, Copyright (C) Michael D. Brennan\n\n"

static char fmt[] = "%-14s%10lu\n" ;

void print_version()
{

   printf(VERSION_STRING, PATCH_STRING, DATE_STRING) ;
   fflush(stdout) ;

   fprintf(stderr, "compiled limits:\n") ;
   fprintf(stderr, fmt, "max NF", (long) MAX_FIELD) ;
   fprintf(stderr, fmt, "sprintf buffer", (long) SPRINTF_SZ) ;
   exit(0) ;
}
