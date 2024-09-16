
## Quick Start

configure --prefix=/home/john
make
make install

exit

 Starbase Data Tables	- An Ascii Relational Database for UNIX
 
 DESCRIPTION
 -----------

 This distribution is made up of five independent parts.  Each was developed
 by a different author and comes with its own licensing arangements.  The
 distribution contains a set of command line filter programs and a library of
 subroutines to read/write and operate on data tables.


 Documentation is provided at:

	http://www-cfa.harvard.edu/~john/starbase/starabase.html


A top level configure script is provided.  Among other things, this creates
a Makefile for each package, based on the hand-created Makefile.in files.
It should suffice to run configure and then run make.  Each of the lower 
level directories will be configured and compiled from here.

-
	> configure --prefix=xxx
	> make
	> make install
-

 1. tablelib, tablesrc, utillib - starbase library and program source code.
	This source was written at SAO and is distributed under Smithsonain
	Astrophysical Observatory Copyright

	A configure script is provided.

	The source code should compile on any reasonable unix system.  The
	only real system dependent parts are in file system directory access
	(in utillib).  Macros from autoconf are used to do this.


 2. tawk - a modified version of the mawk programming language
	This code was written by Mike Brennan at Boeing and is distributed
	under a Gnu General Public License.

	A configure script is provided.


 3. gnu sort/join - a modified version of the gnu textutils.
	This code was written by the GNU Project and is distributed
	under a Gnu General Public License.

	A configure script is provided.


 4. bsd derrived source - the jot program.
	A Makefile.in was written at SAO to config and compile jot.c

	This product includes software developed by the University of
	California, Berkeley and its contributors.


 5. radar - The radar library is provided by its author for use with Starbase and
	    further redistribution is not authorized.  The radar library is not 
	    available from the github based distribution.

