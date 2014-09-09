
PK_LDLIBS	= ../tablelib/libtable.a ../utillib/libutil.a cmpfit-1.2/libmpfit.a -lm

PROGS = nlfittable

all: $(PROGS)

nlfittable: cmpfit-1.2/libmpfit.a

cmpfit-1.2/libmpfit.a :
	cd cmpfit-1.2; make

install: 
	for f in $(PROGS) ; do									\
	    if [ -f $$f$(exeext) ] ; then 							\
	        cp $$f$(exeext) $(bindir)/.;							\
	    else  										\
		cp ../bin/$$f$(exeext) $(bindir)/.;						\
	    fi ;										\
	    echo $$f >> ../Commands;								\
	done

clean::
	cd cmpfit-1.2; make clean
	rm -f $(PROGS) $(CLNKS) $(TLNKS)

