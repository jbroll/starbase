
SCPTS = paramcheck paramrange radecbox table_colnum	\
	colvalue sum3d avg3d sub3d

all:

install:
	if [ ! -d $(libdir)/tablefun ] ; then mkdir $(libdir)/tablefun ; fi
	for f in $(SCPTS) ; do cp $$f $(libdir)/tablefun/.; done

