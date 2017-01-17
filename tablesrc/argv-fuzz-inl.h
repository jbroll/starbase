/*
   american fuzzy lop - sample argv fuzzing wrapper
   ------------------------------------------------

   Written by Michal Zalewski <lcamtuf@google.com>

   Copyright 2015 Google Inc. All rights reserved.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at:

     http://www.apache.org/licenses/LICENSE-2.0

   This file shows a simple way to fuzz command-line parameters with stock
   afl-fuzz. To use, add:

   #include "/path/to/argv-fuzz-inl.h"

   ...to the file containing main(), ideally placing it after all the 
   standard includes. Next, put AFL_INIT_ARGV(); near the very beginning of
   main().FL_INIT_SET0


   This will cause the program to read NUL-delimited input from stdin and
   put it in argv[]. Two subsequent NULs terminate the array. Empty
   params are encoded as a lone 0x02. Lone 0x02 can't be generated, but
   that shouldn't matter in real life.

   If you would like to always preserve argv[0], use this instead:
   AFL_INIT_SET0("prog_name");
*/

#ifndef _HAVE_ARGV_FUZZ_INL
#define _HAVE_ARGV_FUZZ_INL
#ifdef __AFL_COMPILER

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define AFL_INIT_ARGV() do { argv = afl_init_argv(argv, &argc); } while (0)

#define AFL_INIT_SET0(_p) do { \
    argv[0] = (_p); \
    if (!argc) argc = 1; \
  } while (0)

#define AFL_INIT_ARGS(_p) do { \
    argv[argc++] = (_p); \
  } while (0)

#define AFL_FUZZ_FILE(filename) do { afl_fuzz_file(filename); } while (0)

#define MAX_CMDLINE_LEN 100000
#define MAX_CMDLINE_PAR 1000

/* Read the argv unbuffered from fuzzed input
*/
static int afl_read_chunk(char *buffer, int max) {
  buffer[0] = 0;
  int prev = 1;
  int   i;
  for ( i = 0; i < MAX_CMDLINE_LEN - 2; i++) {
    if ( read(0, &buffer[i], 1) != 1 ) break;
    if ( buffer[i] == 0 && prev == 0 ) break;
    prev = buffer[i];
  }

  buffer[i+1] = 0;

  return i;
}

static char** afl_init_argv(char **argv, int* argc) {

  static char  cmdbuff[MAX_CMDLINE_LEN];
  char* cmdline = cmdbuff;

  static char* args[MAX_CMDLINE_PAR];
  int   argn  = 0;

  if ( getenv("AFL_FUZZING") == NULL ) {
      return argv;
  }

  int cmdleng = afl_read_chunk(cmdline, 100000);

  while (*cmdline) {

    args[argn] = cmdline;
    if (args[argn][0] == 0x02 && !args[argn][1]) args[argn]++;
    argn++;

    while (*cmdline) cmdline++;
    cmdline++;
  }

  *argc = argn;

  return args;
}

static void afl_fuzz_file(char *filename) {
  static char  buff[MAX_CMDLINE_LEN];

  int bytes = afl_read_chunk(buff, MAX_CMDLINE_LEN);
  int filed = open(filename, O_CREAT | O_RDWR, 0640);
  bytes = write(filed, buff, bytes);
  close(filed);
}

#undef MAX_CMDLINE_LEN
#undef MAX_CMDLINE_PAR

#else
#define AFL_INIT_ARGV()
#define AFL_INIT_ARGS()
#define AFL_INIT_SET0()
#define AFL_INIT_FILE(filename)
#endif
#endif /* !_HAVE_ARGV_FUZZ_INL */
