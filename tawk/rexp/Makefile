
####################################
# This is a makefile for mawk,
# an implementation of AWK (1988).
####################################
#
#

CC = cc
CFLAGS = -g  -DMAWK -I..

O=rexp.o rexp0.o rexp1.o rexp2.o rexp3.o rexpdb.o

all : $(O)
	@cat </dev/null > .done

debug : $(O) $(DB)
	@cat </dev/null > .done

$(O) : rexp.h

clean :
	rm -f *.o .done
