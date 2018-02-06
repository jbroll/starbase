/* Starbase Data Tables	- An Ascii Database for UNIX

   copyright 1991, 1993, 1995, 1999 John B. Roll jr.
 */
/* edittable.c

	edittable [options] table keycolumn [editcolumns...] [< input]

		keycolumn		- column in table and input to search on.
		keycolumn:keycolumn	- column in table and column in input to search on.
		keycolumn:keycolumn0:keycolumn1
					- column in table and range columns to search in input.
		keycolumn=value		- column value in table to search, no input.
		keycolumn=value0:value1	- column range in table to search, no input.

		editcolumn		- column in table to edit from input.
		editcolumn:editcolumn	- column in table to edit from column in input.
		editcolumn=value	- column in table to edit with value, no input.

	 Options:
		-x		- verify commands before editing.
		-v		- print # of edits at completion.
		-V		- verify each row edit.

 */

#include <table.h>
#include <tablesearch.h>

#define NCOLFLAGS	3
#define EQUAL		0
#define COLUMN0		1
#define COLUMN1		2

char	*flag   =   "=::";
char	*form[] = { "s", "s", "s" };

typedef struct _Edit  {
	int	 icolumn;
	int	 ocolumn;
	char	*value;
} *Edit;

typedef struct _Edits {
	File		 table;
	int		 here;
	char		*column;
	int		 verify;
	int		 edited;
	TableSearch	 search;
	TableHead	 ihead;
	TableRow	 irow;
	char		*icolumn0;
	char		*icolumn1;
	int		 icolnum0;
	int		 icolnum1;
	char		*icolval0;
	char		*icolval1;
	int		 nedit;
	int		 aedit;
	Edit		 edits;
	int	 output;
} *Edits;

void AddEdit(edit, icol, ocol, value)
	Edits	edit;
	int	icol;
	int	ocol;
	char	*value;
{
	if ( edit->nedit <= edit->aedit ) {
		edit->aedit *= 2;
		edit->aedit += 3;

		ReAlloc(edit->edits, sizeof(struct _Edit) * edit->aedit);
	}

	edit->edits[edit->nedit].icolumn = icol;
	edit->edits[edit->nedit].ocolumn = ocol;
	edit->edits[edit->nedit].value   = value;

	edit->nedit++;
}

int EditVerify(edit, irow, orow)
	Edits edit;
	TableRow	irow;
	TableRow	orow;
{
		File	tty;
		char	ch;
		int	i;

	if ( (tty = Open("/dev/tty", "r")) == NULL ) {
	    FPrint(Stderr, "edittable: can't open /dev/tty to verify execution\n");
	    exit(1);
	}

	FPrint(Stderr, "edittable: Verify row edit\n");
	FPrint(Stderr, "	Editing file: %s\n", edit->search->Table);
	FPrint(Stderr, "\n");


	if ( edit->ihead ) {
	    if ( edit->icolval1 ) {
		FPrint(Stderr, "	Editing over: %s column = column range	%s	-> %s\n"
			, edit->column
			, edit->icolumn0
			, edit->icolumn1);
		FPrint(Stderr, "	Current range				%s	-> %s\n"
			, edit->icolval0
			, edit->icolval1);
	    } else {
		FPrint(Stderr, "	Editing over: %s column = column value	%s\n"
			, edit->column
			, edit->icolumn0);
		FPrint(Stderr, "	Current value				%s\n"
			, edit->icolval0);
	    }
	} else {
	    if ( edit->icolval1 ) {
		FPrint(Stderr, "	Editing over: %s column = static range	%s	-> %s\n"
			, edit->column
			, edit->icolval0
			, edit->icolval1);
	    } else {
		FPrint(Stderr, "	Editing over: %s column = static value %s\n"
			, edit->column
			, edit->icolval0);
	    }
	}

	FPrint(Stderr, "\n");
	FPrint(Stderr, "	Edits:\n");
	for ( i = 0; i < edit->nedit; i++ ) {
	    if ( edit->edits[i].value ) {
		FPrint(Stderr, "		%s	%s	= %s"
			, table_colnam(edit->search->THead, edit->edits[i].ocolumn)
			, table_colval(orow, edit->edits[i].ocolumn)
			, edit->edits[i].value);
	    } else {
		FPrint(Stderr, "		%s	%s	= %s	%s"
			, table_colnam(edit->search->THead, edit->edits[i].ocolumn)
			, table_colval(orow, edit->edits[i].ocolumn)
			, table_colnam(edit->ihead, edit->edits[i].icolumn)
			, table_colval(irow, edit->edits[i].icolumn));
	    }

	    FPrint(Stderr, "\n");
	}

	FPrint(Stderr, "\n");
	FPrint(Stderr, "edittable: edit this row? (y) ");
	ch = GetC(tty);

	Close(tty);

	if ( ch != 'y' && ch != '\n' ) 
	    return 0;
	else
	    return 1;
}

void ExecVerify(edit)
	Edits edit;
{
		File	tty;
		char	ch;
		int	i;

	if ( (tty = Open("/dev/tty", "r")) == NULL ) {
	    FPrint(Stderr, "edittable: can't open /dev/tty to verify execution\n");
	    exit(1);
	}

	FPrint(Stderr, "edittable: Verify execution\n");
	FPrint(Stderr, "	Editing file: %s\n", edit->search->Table);
	FPrint(Stderr, "\n");


	if ( edit->ihead ) {
	    if ( edit->icolval1 ) {
		FPrint(Stderr, "	Editing over: %s column = column range	%s	-> %s\n"
			, edit->column
			, edit->icolumn0
			, edit->icolumn1);
	    } else {
		FPrint(Stderr, "	Editing over: %s column = column value %s\n"
			, edit->column
			, edit->icolumn0);
	    }
	} else {
	    if ( edit->icolval1 ) {
		FPrint(Stderr, "	Editing over: %s column = static range	%s	-> %s\n"
			, edit->column
			, edit->icolval0
			, edit->icolval1);
	    } else {
		FPrint(Stderr, "	Editing over: %s column = static value %s\n"
			, edit->column
			, edit->icolval0);
	    }
	}

	FPrint(Stderr, "\n");
	FPrint(Stderr, "	Edits:\n");
	for ( i = 0; i < edit->nedit; i++ ) {
		FPrint(Stderr, "		%s	= %s\n"
			, table_colnam(edit->search->THead, edit->edits[i].ocolumn)
			, edit->edits[i].value 
				? edit->edits[i].value 
				: table_colnam(edit->ihead, edit->edits[i].icolumn));
	}

	FPrint(Stderr, "\n");
	FPrint(Stderr, "edittable: continue with edit? (y) ");
	ch = GetC(tty);

	Close(tty);

	if ( ch != 'y' && ch != '\n' ) exit(1);
}

void *edits(edit, start, end, orow)
	Edits		edit;
	off_t		start;
	off_t		end;
	TableRow	orow;
{
		TableRow irow = edit->irow;
		int	 i, j;

	if ( start != edit->here ) {
	    Flush(edit->table);
	    Seek(edit->table, start, 0);
	}

	if ( edit->verify ) {
	    if ( !EditVerify(edit, irow, orow) ) return edit;
	}

	for ( i = 0; i < edit->nedit; i++ ) {	
	    int 	 icol = edit->edits[i].icolumn;
	    int 	 ocol = edit->edits[i].ocolumn;
	    char 	*ival = edit->edits[i].value;
	    char 	*oval = table_colval(orow, ocol);

	    int		 ilen;
	    int		 olen;

	    if ( ival == NULL ) ival = table_colval(irow, icol);

	    ilen = strlen(ival);
	    olen = strlen(oval);

	    /* Trim the new value if we must 
	     */
	    while ( ilen > olen && ival[ilen - 1] == ' ' ) { ival[ilen - 1] = '\0'; ilen--; }
	    while ( ilen > olen && ival[       0] == ' ' ) { ival++               ; ilen--; }

	    if ( edit->output ) {
		;
	    } else {
		if ( ilen > olen ) {
		    FPrint(Stderr, "edittable: input value too large for space available in table:\n");
		    FPrint(Stderr, "	column: %s	old: %s	new: %s\n"
				    , table_colnam(edit->search->THead, ocol), oval, ival);
		    continue;
		}

		for ( j = 0; j < ilen; j++ ) oval[j] = ival[j];	/* Do a justification here ? */
		for (      ; j < olen; j++ ) oval[j] = ' ';
	    }
	}

	if ( !edit->output ) {
	    /* Re-Write the edited row
	     */
	    table_rowput(edit->table, edit->search->THead, orow, NULL, NULL, 0);
	    edit->here = end;
	    edit->edited++;
	}

	return edit;
}

int main(argc, argv)
	int	 argc;
	char	*argv[];
	
{
	Edits		 E;
	TableHead	 InTable = NULL;
	char		*column;
	int		 colnum;

	TableColArg	value[NCOLFLAGS];
	int		check[NCOLFLAGS];

	char	*p;

	int	 verify = 0;
	int	 execut = 1;

	int	i, j;

	int	icolumn, ocolumn;

	Calloc(E, sizeof(struct _Edits));

	while ( argc > 1 ) {
	    if ( argv[1][0] != '-' ) break;

	    switch ( argv[1][1] ) {
	     case 'o':  E->output = 1;  break;
	     case 'v':	   verify = 1;	break;
	     case 'V':	E->verify = 1;	break;
	     case 'x':	   execut = 0;	break;
	     default:	FPrint(Stderr, "edittable: unknown option: %s\n", argv[1]);
			exit(1);
	    }

	    argv++;
	    argc--;
	}

	if ( argc < 3 ) {
	    FPrint(Stderr, "usage: edittable [-vxV] table keycolumn[=value] [editcolumn[=value]] [< edits]\n");
	    exit(1);
	}

	NewString(column, argv[2]);
	E->column = column;

	if ( p = strchr(column, '=') ) *p = '\0';
	if ( p = strchr(column, ':') ) *p = '\0';

	if ( (E->search = table_search(argv[1], column, TSEARCH_OUTALL, NULL)) == NULL ) {
	    exit(1);
	}
	if ( (E->table   = Open(argv[1], "r+")) == NULL ) {
	    FPrint(Stderr, "edittable: can't open table for writing\n");
	    exit(1);
	}

	for ( j = 0; j < NCOLFLAGS; j++ ) check[j] = 0;

	table_colarg(NULL, argv[2], flag, form, value, check);

	if ( check[EQUAL] ) {				/* Static selection			*/
	    E->icolval0 = value[EQUAL].s;
	    if (  check[COLUMN0] ) {			 /* Static range			*/
		E->icolval1 = value[COLUMN0].s;
	    }
	} else {					/* Input driven selection		*/
	    if ( (InTable = table_header(Stdin, TABLE_PARSE)) == NULL ) {
		FPrint(Stderr, "edittable: can't read table from input\n");
		exit(1);
	    };

	    if ( check[COLUMN0] ) {			/* Key and input column different	*/
		E->icolumn0 = value[COLUMN0].s;

		if ( (E->icolnum0 = table_colnum(InTable,  value[COLUMN0].s)) == 0 ) {
		    FPrint(Stderr, "edittable: keycolumn %s not in input table\n"
			, value[COLUMN0].s);
		    exit(1);
		}
		if ( check[COLUMN1] ) {			/* Range column				*/
		    E->icolumn0 = value[COLUMN1].s;
		    if ( (E->icolnum1 = table_colnum(InTable, value[COLUMN1].s)) == 0 ) {
			FPrint(Stderr, "edittable: keycolumn %s not in input table\n"
			    , value[COLUMN1].s);
			exit(1);
		    }
		}
	    } else {					/* Key and input column same		*/
		E->icolumn0 = column;

		if ( (E->icolnum0 = table_colnum(InTable, column)) == 0 ) {
		    FPrint(Stderr, "edittable: keycolumn %s not in input table\n"
			, column);
		    exit(1);
		}
	    }
	}

	if ( argc == 3 ) {				/* Input driven edit on all columns	*/
	    if ( InTable == NULL ) {
		FPrint(Stderr, "edittable: no edits specified with static row selection.\n");
		exit(1);
	    }

	    for ( i = 1; i <= E->search->THead->header->ncol; i++ ) {
		if ( icolumn = table_colnum(InTable, table_colnam(E->search->THead, i)) )
		    AddEdit(E, icolumn, i, NULL);
	    }

	    if ( E->nedit == 0 ) {
		FPrint(Stderr, "edittable: no edit columns match input columns\n");
		exit(0);
	    }
	} else {
	    /* Options driven edit on some columns.
	     */
	    for ( i = 3; i < argc; i++ ) {
			int	ocolumn;

		for ( j = 0; j < NCOLFLAGS; j++ ) check[j] = 0;
		if ( (ocolumn = table_colarg(E->search->THead, argv[i], flag, form, value, check)) == 0 ) {
		    FPrint(Stderr, "edittable: edit column not in edit table: %s\n", argv[i]);
		    exit(1);
		}

		if ( check[EQUAL] ) {	/* Static edit				*/
		    AddEdit(E, 0, ocolumn, value[EQUAL].s);
		} else {		/* Input driven edit on this column	*/
		    if ( InTable == NULL ) {
			FPrint(Stderr, "edittable: input driven column edits can't be used with static selection\n");
			exit(1);
		    }

	            if ( check[COLUMN0] ) {			/* input and output column different	*/
			if ( (icolumn = table_colnum(InTable, value[COLUMN0].s)) == 0 ) {
			    FPrint(Stderr, "edittable: keycolumn not in input table: %s\n"
				    , value[COLUMN0].s);
			    exit(1);
			}
	            } else {					/* Key and input column same		*/

			if ( (icolumn = table_colnum(InTable, argv[i])) == 0 ) {
			    FPrint(Stderr, "edittable: edit column %s not in input table\n"
				, argv[i]);
			    exit(1);
			}
		    }

		    AddEdit(E, icolumn, ocolumn, NULL);
		}
	    }
	}

	E->ihead = InTable;

	if ( !execut ) {
	    ExecVerify(E);
	}

	{
		TableRow R    = table_rowloc(NULL, 0, 0);

	    if ( InTable ) {
		    TableRow IRow = table_rowloc(NULL, 0, 0);

		while ( E->irow = table_rowget(Stdin, InTable, IRow, NULL, NULL, 0) ) {
		    E->icolval0 =               table_colvals(E->irow, E->icolnum0);
		    E->icolval1 = E->icolnum1 ? table_colvals(E->irow, E->icolnum1) : NULL;

		    table_srange(E->search, E->icolval0, E->icolval1, edits, E, R);
		}

		table_rowfree(IRow);
	    } else {
		table_srange(E->search, E->icolval0, E->icolval1, edits, E, R);
	    }

	    table_rowfree(R);
	}

	if ( E->output ) {
	}
	if ( verify ) {
	    FPrint(Stdout, "Edited %d Rows\n", E->edited);
	}

	exit(0);
}
