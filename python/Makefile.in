
PROGS = xlsx2table

all: $(PROGS) 

xlsx2table: xlsx2table.py
	xinc xlsx2table.py > xlsx2table
	chmod +x xlsx2table

install:
	cp xlsx2table $(bindir)
	rm -f $(bindir)/table2xlsx
	ln -s $(bindir)/xlsx2table $(bindir)/table2xlsx

distclean::
	rm -f Makefile

clean::
	rm -f *.a *.o *.bak
