

#include "table.h"
#include "range.h"

#if _FILE_OFFSET_BITS == 64
#define atoffset(x) strtoll(x, NULL, 10)
#else
#define atoffset(x) atoi(x)
#endif


static void *putrow(data, start, end, row, table, file)
	void		*data;
	off_t		 start;
	off_t		 end;
	TableRow	 row;
	TableHead	 table;
	File		 file;
{
	table_rowput((File) data, table, row, NULL, NULL, 0);
	return data;
}

void *table_rangerows(file, table, R, range, func, data)
	File	 	 file;
	TableHead	 table;
	TableRow	 R;
	Range		*range;
	void    	*(*func)();
	void		*data;
{
		TableRow r;
		int	 j;
		int	 ch;

	if ( !range ) return data;

	for ( j = 0; j < NRange(range); j++ ) {
		off_t	start = NthRange(range, j).start; 
		off_t	end   = NthRange(range, j).end;


	    Seek(file, start-1, 0);
	    while ( (ch = GetChar(file)) != '\n' && ch != -1 ) start++;

	    while ( start < end ) {
		if ( (r = table_rowget(file, table, R, NULL, NULL, 0)) == NULL ) {
			FPrint(Stderr, "table_rangerows: error seeking data file\n");
			return data;
		};

		data = (*func)(data, start, start + r->nbuf, r, table, file);
		start += r->nbuf;
	    }
	}

	return data;
}

void table_blankrangerows(file, range)
        File     	 file;
        Range   	*range;
{
		int	 j;

	if ( !range ) return;

	for ( j = 0; j < NRange(range); j++ ) {
		off_t	start = NthRange(range, j).start; 
		off_t	end   = NthRange(range, j).end;


	    Seek(file, start-1, 0);
	    while ( start < end ) {
		PutC(file, ' ');
		start++;
	    }
	}
}

void table_printrangerows(table, ifile, ofile, range)
	TableHead	 table;
        File     	 ifile;
        File     	 ofile;
        Range   	*range;
{
		TableRow	r;

	if ( !range ) return;

 	r = table_rowloc(NULL, 0, 0);

	table_rangerows(ifile, table, r, range, putrow, ofile);
	table_rowfree(r);
}


int header_row(row, starcol, end_col)
	TableRow	row;
	int		*starcol;
	int		*end_col;
{
		int	header = 0;
		int	i;

    for ( i = 1; i <= row->ncol; i++ ) {
		char	*col = table_colval(row, i);

	while ( *col == ' ' ) col++;
	if ( !strcmp(col, "Offset") ) {
		header++;
		*starcol = i;
	} else
	    if ( !strcmp(col, "Length") ) {
		header++;
		*end_col = i;
	    }
    }

    return header;
}

Range *table_readrange(file, mode, starcol, end_col)
	File	 file;
	int	 mode;		/* Union / Intersection mode	*/
	int	 starcol;
	int	 end_col;
{
		TableRow	 row = NULL, r;
		Range		*r0 = NULL, *r1 = NULL;

		int	ntables = 0;
		int	done = 0;

		off_t	start;
		off_t	end;

	row = table_rowloc(NULL, 0, 0);

	while ( (r = table_rowget(file, NULL, row, NULL, NULL, 0)) )
	    if ( header_row(row, &starcol, &end_col) ) {
		r = table_rowget(file, NULL, row, NULL, NULL, 0); /* dashes */
		if ( mode && ntables >= 2 ) {
		    r0 = range_inter(r0, r1);
		}
		ntables++;
	    } else {
		start = atoffset(table_colval(row, starcol));
		end   = atoffset(table_colval(row, end_col)) + start;

		if ( !mode || (mode && ntables == 1) )
		    r0 = range_add(r0, start, end);
		else
		    r1 = range_add(r1, start, end);
	    }

	if ( !mode )
	    r0 = range_union(r0);
	else
	  if ( ntables >= 2 )
	    r0 = range_inter(r0, r1);

	table_rowfree(row);
	return r0;
}

