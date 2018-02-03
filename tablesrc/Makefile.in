# Makefile.in
#
# Starbase Data Tables	- An Ascii Database for UNIX
# 
#   copyright 1991, 1993, 1995, 1999 John B. Roll jr.
#

PK_LDLIBS	= ../tablelib/libtable.a ../utillib/libutil.a -lm

PROGS = column table sorttable indexsorttable index fixtotable 	\
	cprodtable pastetable search 				\
	edittable coproctable statstable			\
	histtable report					\
	lrfittable						\
	resampletable extracttable				\
	teeproc filebytes					\
	tabletofits 						\
	clock							\
	wctable smoothtable					\
	svdfittable xyfittable					\
	xyshift


SCPTS = listtotable tabletolist headtable tailtable 	 	\
	randtable rand fldtotable 				\
	uniontable intertable diffrtable appendtable		\
	cvstattable						\
	daophototable tabletodaophot sextotable			\
	xytotng							\
	rotatetable shifttable scaletable			\
	jiggletable						\
	xytorttable rttoxytable					\
	fitsbase fitshdr					\
	evalpolytable						\
	tabletohtml						\
	domaintable						\
	tabletosh tabletosetenv					\
	compute3dtable  fun3dtable expr3dtable			\
	rms3dtable      sum3dtable				\
	avg3dtable      sub3dtable				\
	radecboxtable						\
	abssorttable radialsorttable				\
	rowtocoltable coltorowtable				\
	reseqtable						\
	transposetable						\
	formattable						\
	total							\
	classifytable						\
	applyxyfittable xycoefftable				\
	tactable xinc						\
	sigmacliptable						\
	pivottable						\
	hdrvaltable						\
	updatetable						\
	overwrite


CLNKS = project trim check justify headoff header headline dashline
TLNKS = reckon calc row select compute validate
SLNKS = meantable medtable mediantable rmstable stdtable stdevtable mintable maxtable \
	rangetable rngtable sumtable prectable widthtable tottable
INCPY =	radecbox

all: $(PROGS)

afl: 
	$(MAKE) CC=afl-gcc C_DFLAGS=-g all

install: 
	for f in $(PROGS) ; do									\
	    if [ -f $$f$(exeext) ] ; then 							\
	        cp $$f$(exeext) $(bindir)/.;							\
	    else  										\
		cp ../bin/$$f$(exeext) $(bindir)/.;						\
	    fi ;										\
	    echo $$f >> ../Commands;								\
	done
	for f in $(SCPTS) ; do 									\
	    if [ ! -f ../bin/$$f ] ; then 							\
	        $(SED) -e "s%PATH=.%PATH=$(bindir)%"     <$$f > $(bindir)/$$f;			\
	    else										\
	        $(SED) -e "s%PATH=.%PATH=$(bindir)%" 						\
		   -e "/^TABLELIB/s%\$$TABLELIB%\$$TABLELIB:$(libdir)%" <$$f > $(bindir)/$$f;	\
	    fi ;										\
	    chmod +x $(bindir)/$$f;								\
	    echo $$f >> ../Commands;								\
        done
	for f in $(CLNKS) ; do									\
	    (cd $(bindir)/.; $(RM) $$f$(exeext); 						\
	     $(LN_S) column$(exeext) $$f$(exeext));						\
	    echo $$f >> ../Commands;								\
	done
	for f in $(TLNKS) ; do									\
	    (cd $(bindir)/.; $(RM) $$f$(exeext); 						\
	     $(LN_S) table$(exeext)  $$f$(exeext));						\
	    echo $$f >> ../Commands;								\
	done
	for f in $(SLNKS) ; do									\
	    (cd $(bindir)/.; $(RM) $$f$(exeext);						\
	     $(LN_S) statstable$(exeext)  $$f$(exeext));					\
	    echo $$f >> ../Commands;								\
	done
	for f in $(INCPY) ; do									\
	    if [ ! -f ../bin/$$f ] ; then 							\
	        $(INC) < $$f | $(SED) -e "s%PATH=.%PATH=$(bindir)%"  	> $(bindir)/$$f;	\
	    else										\
	        $(INC) < $$f | $(SED) -e "s%PATH=.%PATH=$(bindir)%" 				\
		       -e "/^TABLELIB/s%\$$TABLELIB%\$$TABLELIB:$(libdir)%" > $(bindir)/$$f;	\
	    fi ;										\
	    chmod +x $(bindir)/$$f;								\
	    echo  $$f     >> ../Commands;							\
	done
	(cd $(bindir)/.; $(RM) radeccirc; $(LN_S) radecbox radeccirc)
	echo radeccirc >> ../Commands;

clean::
	rm -f $(PROGS) $(CLNKS) $(TLNKS) $(SLNKS)

