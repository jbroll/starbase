# Starbase Makefile
#
 WD=/home/john/src/starbase
 OS=Linux
CPU=x86_64

CC=afl-gcc

# These are the default definitions for all starbase makefiles
#

srcdir	= .
 
AR 	= ar
INC     = $(WD)/tablesrc/xinc
LEX     = flex
LN_S    = ln -s
SED     = sed
RANLIB 	= ranlib
RM      = rm -f
YACC 	= bison -y

exeext	= ""

MAKEDEFS = bindir=$(bindir) libdir=$(libdir) incdir=$(incdir)

Arch	  = `uname`-`uname -i`

# This is the Linux - Makefile definitions file
#

OS_DEFS    = 	-DSTDC_HEADERS=1	\
		-DCONFIG_BROKETS 	\
		-DHAVE_DIRENT_H=1	\
		-DHAVE_SYS_TYPES_H=1 	\
		-DHAVE_SYS_STAT_H=1 	\
		-DHAVE_STDLIB_H=1 	\
		-DHAVE_STRING_H=1 	\
		-DHAVE_MALLOC_H=1 	\
		-DHAVE_MEMORY_H=1 	\
		-DHAVE_MEMCPY=1		\
		-DHAVE_STRINGS_H=1 	\
		-DHAVE_INTTYPES_H=1 	\
		-DHAVE_STDINT_H=1 	\
		-DHAVE_UNISTD_H=1 	\
		-DHAVE_FCNTL_H=1 	\
		-DHAVE_ALLOCA_H=1 	\
		-DHAVE_FLOAT_H=1 	\
		-DHAVE_STDLIB_H=1 	\
		-DHAVE_MEMCHR=1 	\
		-DHAVE_STRCHR=1 	\
		-DHAVE_STRERROR=1 	\
		-DHAVE_STRUCT_TM_TM_ZONE=1 	\
		-DHAVE_TM_ZONE=1 		\
		-D_LARGEFILE_SOURCE		\
		-D_FILE_OFFSET_BITS=64	\
		-DPRINTF_OFF_T="ld"

OS_INCL    = -I. -I$(srcdir)/../tablelib -I$(srcdir)/../include
OS_LDFLAGS =  $(file64)

OS_LDLIBS  = -lm
OS_CFLAGS  = -DSAO -fPIC $(C_CFLAGS) $(C_DFLAGS) $(INCL) $(file64) $(DEFS) -DTABLELIB='"$(libdir)"'

OS_DFLAGS  = 

OS_DYLINK  = -shared
OS_DYLOAD  = -shared

OS_XLIB    = -L/usr/X11R6/lib -lX11


prefix=/home/john/src/starbase
arch=

exec_prefix = $(prefix)
exec_arch =   $(arch)
bindir = $(exec_prefix)/bin$(exec_arch)
libdir = $(exec_prefix)/lib$(exec_arch)
incdir = $(prefix)/include
arch=

SUBDIRS=utillib tablelib tablesrc tablefun tawk nlfit gnu bsd


PROGS = xlsx2table

all: $(PROGS) 

xlsx2table: xlsx2table.py
	xinc xlsx2table.py > xlsx2table
	chmod +x xlsx2table

install:
	cp xlsx2table $(bindir)
	rm $(bindir)/table2xlsx
	ln -s $(bindir)/xlsx2table $(bindir)/table2xlsx

# This is the main rules makefile for all of starbase
#

CFLAGS	= -D$(OS) $(PK_DEFS)   $(OS_DEFS)   $(OS_CFLAGS) $(PK_CFLAGS) $(OS_INCL) $(PK_INCL)
LDLIBS	=         $(PK_LDLIBS) $(OS_LDLIBS)

dummy:

distclean::
	rm -f Makefile

clean::
	rm -f *.a *.o *.bak

.c.o:
	$(CC) -c $(CPPFLAGS) $(INCL) $(CFLAGS) $<

.l.o:
	flex  -t $< > $*.c
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $*.c


