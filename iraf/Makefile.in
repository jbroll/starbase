#
# Makefile for IRAF interface programs
#
# IRAFARCH must exist in the environment for builds or installations 
# to be run

srcdir = @srcdir@

prefix = @prefix@
exec_prefix = $(prefix)
bindir = @prefix@$(ARCH)
exeext = @EXEEXT@

CC = @CC@

LN_S   = ln -s
RM     = rm -f
DEFS   = @DEFS@
LIBS   = @LIBS@ ../utillib/libutil.a 
CFLAGS = @CFLAGS@ 
LDFLAGS= @LDFLAGS@
INCL   = -I../include

.c.o:
	$(CC) -c $(CPPFLAGS) $(DEFS) $(INCL) $(CFLAGS) $<

PROGS = 
SCPTS = imhbase 
IRAFMV= starbase.cl starbase.msg starbase.par 
IRAFPROG=x_starbase.e libpkg.a
IRAFDIR = iraf.pkg/starbase
IRAFPROG= x_starbase.e libpkg.a 

all:
	@if [ "$(IRAFARCH)" = "" ]; then \
            echo "Skipping iraf" 1>&2; \
        else \
	  $(MAKE) $(MAKEFLAGS) targets starbase.msg starbase.par; \
        fi


targets: $(PROGS) 

install: 
	@if [ "$(IRAFARCH)" = "" ]; then \
          exit 0; \
        else \
          echo "Installing into $(bindir): $(PROGS) $(SCPTS)"; \
	  for f in $(SCPTS) ; do \
            sed "s%PATH=.%PATH=$(bindir)%" <$$f >$(bindir)/$$f; \
            chmod +x $(bindir)/$$f; done; \
	  for f in $(IRAFPROG); do \
	     $(RM) $(bindir)/$$f; done; \
	  bindir=$(bindir) mkstarbasecl >$(IRAFDIR)/starbase.cl ;\
	  mkdir -p $(bindir)/iraf 2>/dev/null ;\
	  echo "Installing into $(bindir)/iraf: $(IRAFMV)"; \
	  for f in $(IRAFMV) ; do cp $(IRAFDIR)/$$f $(bindir)/iraf; done ;\
          for f in $(IRAFPROG) ; do cp $(IRAFDIR)/$$f $(bindir)/.; done ;\
	  (cd $(bindir)/iraf; $(RM) bin.$(IRAFARCH); \
                  ln -s $(bindir) bin.$(IRAFARCH); ); \
        fi
# insert in install if some progs are created here
#          for f in $(PROGS) ; do cp $$f $(bindir)/.; done ; \

uninstall:
	for f in $(PROGS) $(SCPTS); do rm -f $(bindir)/$$f; done
	for f in $(IRAFMV); do rm -f $(bindir)/iraf/$$f; done


TAGS: $(SOURCES)
	etags $(SOURCES)

check:

clean:
	rm -f $(PROGS) *.o core *.bak *~ $(CLNKS) $(TLNKS)

mostlyclean: clean

distclean: clean
	rm -f Makefile starbase.msg starbase.par starbase.cl 

realclean: distclean
	rm -f TAGS

# Linking rules.
#

imhhdr:  imhhdr.o  
	$(RM) $@
	$(CC) -o $@  imhhdr.o $(LDFLAGS)  $(LIBS)    

#
# modify for version, date
starbase.msg: ../Version $(IRAFDIR)/starbase.msg.sed
	sed -e "s/Version/`cat ../Version`/" -e "s/Date/`date +%Y-%m-%d`/" \
               < $(IRAFDIR)/starbase.msg.sed >$(IRAFDIR)/starbase.msg

starbase.par: ../Version $(IRAFDIR)/starbase.par.sed
	sed -e "s/Version/`cat ../Version`/" -e "s/Date/`date +%Y-%m-%d`/" \
               < $(IRAFDIR)/starbase.par.sed >$(IRAFDIR)/starbase.par

