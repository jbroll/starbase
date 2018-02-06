/* table_extract.c
 */
/* Starbase Data Tables - An Ascii Database for UNIX
 
   copyright 1991, 1993, 1995, 1999 John B. Roll jr.
 */

#include <fnmatch.h>
#ifdef FNM_EXTMATCH
#define tmatch(string, template) (!fnmatch(template, string, FNM_EXTMATCH))
#else
#define tmatch(string, template) (!fnmatch(template, string, 0))
#endif

#include "table.h"
#ifdef __STDC__
#include <stdarg.h>
#else
#include <varargs.h>
#endif

char *SlurpLine();


#ifdef __STDC__
void table_extractva(File file, ...)
#else
void table_extractva(va_alist)
	va_dcl
#endif
{
	va_list args;  

	TableSection	 sections = NULL;
	int	 	 nsec = 0;
	int	 	 asec = 0;
	char		*sec;


#ifdef __STDC__
	va_start(args, file);
#else
	File	file; 
	va_start(args);
	file = va_arg(args, File);

#endif

	/* Parse the varargs into a table of name--> action pairs.
	 */
	for ( nsec = 1; (sec = va_arg(args, char *)); nsec++ ) {
	    if ( nsec >= asec ) {
		asec *= 2;
		asec += 5;
		ReAlloc(sections, asec * sizeof(struct _TableSection));
	    }
	    sections[nsec].sec = sec;
	    sections[nsec].fun = va_arg(args, ifunct);
	    sections[nsec].dat = va_arg(args, void *);

	    if ( *sections[nsec].sec == '\001' ) {
		sections[0].sec = sections[nsec].sec;
		sections[0].fun = sections[nsec].fun;
		sections[0].dat = sections[nsec].dat;
		nsec--;
	    }
	}

	table_extract(file, sections, nsec);
	Free(sections);
	return;
}

void table_extract(file, sections, nsec)
	File	file;
	TableSection	sections;
	int		nsec;
{
	TableHead 	 header = NULL;
	char	 	 ch = '\f';
	int	 i;

	do {
	    if ( ch == '\f' ) {
			int	marker = 0;

	      TableTop:
	        marker++;
		if ( (header = table_header(file, 0)) == NULL ) break;
		if (   header->name ) {
		  for ( i = 1; i < nsec; i++ ) {
		    if ( tmatch(header->name, sections[i].sec) ) {
			switch ( 
			 sections[i].fun(file, marker, header, sections[i].dat) ) {
			 case  EOF: 
			 case '\f': 
				if (  header ) table_hdrfree(header);
				goto TableTop;
			 case    0:
				if (  header ) table_hdrfree(header);
				break;
				
			 case    1:
			 case    2: return;
			 default  : break;
			}
			break;
		    }
		  }
		}
		if ( i >= nsec && sections[0].sec ) {
		    switch ( 
		     sections[0].fun(file, marker, header, sections[0].dat) ) {
			 case  EOF: 
			 case '\f': 
				if (  header ) table_hdrfree(header);
				goto TableTop;
			 case    0:
				if (  header ) table_hdrfree(header);
				break;
				
			 case    1:
			 case    2: return;
			 default  : break;
		    }
		}

	    }
	    {   TableRow R = NULL;

	    	while ( R = table_rowget(file, NULL, R, NULL, NULL, 0) );
		if ( R ) table_rowfree(R);
	    }
	} while ( 1 );
}

