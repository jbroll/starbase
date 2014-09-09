# Makefile.in
#
# Starbase Data Tables	- An Ascii Database for UNIX
# 
#   copyright 1991, 1993, 1995, 1999 John B. Roll jr.
#
#

SLALIB=../radar
SLAINC=../radar

PK_LDLIBS = ../tablelib/libtable.a ../utillib/libutil.a -L../radar -lradar -lm
PK_INCL	  = -I/data/mmti/include -I$(SLAINC)
PK_DEFS	  = -DUSE_TCL_STUBS=1

LIBRARY = slalink.so
PROGS   = svdfittable xyfittable

TARGETS = $(PROGS) $(LIBRARY)

all	:
	$(MAKE) binary;


binary	: $(TARGETS)

tcl	:
	$(MAKE) sla.so 

clean::
	rm -f $(PROGS) *.o *.so core *.bak $(CLNKS) $(TLNKS)

slalinktcl.o : slalink.c
	$(CC) $(OS_CFLAGS) $(C_PIC) -DTCL -o slalinktcl.o -c $(CPPFLAGS) $(DEFS) -I$(SLALIB) $(INCL) $(CFLAGS) slalink.c

slalink.o : slalink.c
	$(CC) $(OS_CFLAGS) $(C_PIC) -c $(CPPFLAGS) $(DEFS) -I$(SLALIB) $(INCL) $(CFLAGS) $<

slalink.so: slalink.o
	$(CC) $(OS_CFLAGS) $(OS_DYLOAD) -o slalink.so slalink.o    -L$(SLALIB) $(LDFLAGS) -lradar -lm


#$(CC) -dynamiclib -flat_namespace -undefined suppress -o sla.so     slalinktcl.o -L$(SLALIB) $(LDFLAGS) -lradar -lm

sla.so: slalinktcl.o
	$(CC) $(OS_CFLAGS) $(OS_DYLOAD) -DUSE_TCL_STUBS -o sla.so slalinktcl.o  -L$(SLALIB) $(LDFLAGS) -L/data/mmti/lib -ltclstub8.4 -lradar -lm

INCPY = precess				\
    	fk425 fk524 fk45z fk54z		\
	eqgal galeq eg1950 ge1950	\
    	s2tptable tp2stable

install	:
	@for f in $(PROGS) ; do 								\
	    if [ -f $$f ] ; then								\
		cp $$f$(exeext) $(bindir)/.;							\
	    else  										\
		if [ -f ../bin/$$f ] ; then							\
		    cp ../bin/$$f$(exeext) $(bindir)/.;						\
		fi ;										\
	    fi ;										\
	    echo  $$f     >> ../Commands;							\
	done ;											\
	for f in $(INCPY) ; do									\
	    if [ ! -f ../bin/$$f ] ; then 							\
	        $(INC) < $$f | $(SED) -e "s%PATH=.%PATH=$(bindir)%"  	> $(bindir)/$$f;	\
	    else										\
	        $(INC) < $$f | $(SED) -e "s%PATH=.%PATH=$(bindir)%" 				\
		       -e "/^TABLELIB/s%\$$TABLELIB%\$$TABLELIB:$(libdir)%" > $(bindir)/$$f;	\
	    fi ;										\
	    chmod +x $(bindir)/$$f;								\
	    echo  $$f     >> ../Commands;							\
	done ;											\
	if [ -f slalink.so ] ; then								\
	    cp slalink.so $(libdir)/.;							\
	else  											\
	    cp ../lib/slalink.so $(libdir)/.;						\
	fi ;											\
	if [ -f sla.so ] ; then									\
	    cp sla.so $(libdir)/.;								\
	fi ;											\
	if [ ! -d $(libdir) ] ; then mkdir $(libdir) ; fi ;					\
	if [ -f $(libdir)/table.inc ] ; then							\
	    $(bindir)/tawk '/slalib/,/load/ { next } { print }' < $(libdir)/table.inc > table.tmp; \
	    cp table.tmp $(libdir)/table.inc;						\
	    rm table.tmp; 									\
	fi
	sed -e s%SLALIB%$(libdir)% < table.inc >> $(libdir)/table.inc

xyfittable : xyfittable.o xyfit.o
	$(CC) $(OS_CFLAGS) -o xyfittable $^ -L$(SLALIB) $(LDFLAGS) $(LDLIBS)
