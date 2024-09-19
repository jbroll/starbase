/* starbase - starbase data table C library.
 */

#ifndef _TABLE_H_
#define _TABLE_H_

/* These must be included with -I on the compile line
 */
#ifdef NO_XIO
#define PROTOTYPE(X)	 X
typedef void	*File;
typedef int     (*ifunct) ();
#else
#include <xos.h>
#include <xfile.h>
#endif

double 	SAOstrtod();
char	*trim();

#define TABLE_PARSE	 2	/* Parse the input rows, trimming up space */
/* #define TABLE_NOCASE	 4	/* Ignore case in header names.		   */
#define TABLE_JUSTIFY	 8	/* Justify output rows.			   */

#define TABLE_NICETAB	16	/* Parsing mode.			   */
#define TABLE_STRICTEOF	32	/* Parsing mode.			   */
#define TABLE_PARSBUFF	64	/* Parsing mode.			   */

#define TABLE_LINE	1024	/* Initial allocations for TableRows	*/
#define TABLE_NCOL	 128

#define TABLE_FILEBUF	1024*1024
#define TABLE_TRIMBUF	256


/* Row data structure.
 */
typedef struct _TableRow {
	char	 *buffer;	/* Text buffer for column values	*/
	int	  nbuf;		/* Used text				*/
	int	  abuf;		/* Size of allocated text		*/
	char	**column;	/* Pointer buffer for columns		*/
	int	  ncol;		/* Number of columns			*/
	int	  acol;		/* Number of pointers allocated		*/
	char	 *fields;	/* Pointer last char for columns	*/
} *TableRow;

typedef union _TableColArg {
	int	i;
	float	f;
	double	d;
	char	*s;
} TableColArg;

typedef struct _TableHead {
	TableRow header;	/* The header row in a TableRow buffer	*/
	TableRow dashes;	/* The dashline in a TableRow Buffer	*/
	char	*name;		/* The name of the table		*/


	int	*hwidth;	/* Widths of the header + widths rows	*/
	int	*mwidth;	/* Widths of the body of the table	*/
	int	*owidth;	/* Widths of the output columns 	*/
	int	*mprec;		/* Precision of the body of the table	*/

	char   **format;	/* Format of the output columns 	*/
	int	*ftype;		/* Format type of format		*/
	int	*fwidth;	/* Widths of the formatted spec 	*/
	int	*fprec;		/* Precision of formatted spec		*/
	int	*fflag;		/* Flag of formatted spec		*/

	int	 mode;		/* IO Mode				*/
	char	 ofs;
	char	 ors;

	TableRow*values;	/* Header values			*/
	int	*dirty;
	int	 avalue;
	int	 nvalue;	
} *TableHead;

typedef struct _TableEnum {
	char	*name;
	int	 valu;
} TableEnum;

/* Values for table_rowput justification array.
 */
#define TAB_DEFAULT	 1
#define TAB_LEFT	 2
#define TAB_CENTER	 4
#define TAB_RIGHT	 8
#define TAB_EXPAND	16
#define TAB_SKIP	32
#define TAB_NONE	64

/* Struct to control table_extract 
 */
typedef struct _TableSection {
	char	*sec;
	ifunct	 fun;
	void	*dat;
} *TableSection;

/* table_colval - get the value from the column.

    Return a pointer to the value of the table column for row #r.  It is a
    macro which returns a pointer to the actual column buffer.  The value
    should not be modified or freed.

 */
char	*table_colval		PROTOTYPE((
		  TableRow	r	/*  pointer to the table row. 		*/
		, int		c	/* The column number to get the value for. */
));

/* table_colvals - get the value from the column.

    Return a copy of the table column for row #r as a ascii string allocated
    with strdup.  The returned value may be freed with free(3) when it is no
    longer needed.

 */
char 	*table_colvals		PROTOTYPE((
		  TableRow	r	/*  pointer to the table row. 		*/
		, int		c	/* The column number to get the value for. */
));
/* table_colvali - get the value from the column.

    Return the value of the table column for row #r as an integer.
 */
int	 table_colvali		PROTOTYPE((
		  TableRow	r	/*  pointer to the table row. 		*/
		, int		c	/* The column number to get the value for. */
));
/* table_colvald - get the value from the column.

    return the value of the table column for row #r as a double.
 */
double   table_colvald		PROTOTYPE((
		  TableRow	r	/*  pointer to the table row. 		*/
		, int		c	/* The column number to get the value for. */
));
double   table_colvalr		PROTOTYPE((
		  TableRow	r	/*  pointer to the table row. 		*/
		, int		c	/* The column number to get the value for. */
));

/* Definitions for load/save access.
 */
#define TblOff(p_type,field) \
        ((int) (((char *) (&(((p_type)NULL)->field))) - ((char *) NULL)))

/* Value locations
 */
#define TblHdr	0x00010000

#define TblDef	0x00100000
#define TblUsr	0x00200000
#define TblBak	0x00400000

/* Value Masks
 */
#define TblType 0xF0000000
#define TblLen	0x000000FF	

/* Value Types
 */
#define TblInt	0x10000000
#define TblSht	0x20000000
#define TblDbl	0x30000000
#define TblFlt	0x40000000
#define TblStr	0x50000000
#define TblChr  0x60000000
#define TblByt  0x70000000
#define TblBuf  0x80000000
#define TblDup	0x90000000

typedef int	(*tblvect)();

typedef struct _TblRowDescrip {
	int	 type;
	char	*name;
	int	 offs;
	void	*data;
	tblvect  proc;
	int	 size;
	int	 cols;
} TblRowDescrip;

typedef TblRowDescrip *TblDescriptor;


/*#3 table_rowloc - low level table row descriptor allocation.

   ReAllocate the buffers for row to have nbuf characters in the buffer
   and ncol entries in the column vector.

RETURNS
   Returns the reallocated TableRow.
 #*/
TableRow table_rowloc		PROTOTYPE((
	  TableRow row 		/* the row to be reallocated. 		*/
	, int nbuf 		/* the new size of row buffer. 		*/
	, int ncol		/* the new size of column vector. 	*/
));

/*#3 table_parsline - parse a line from a table file.

   Parse a line from file and return a TableRow structure.
   If row is NULL a new TableRow structure will be allocated and
   returned to the user.  This is a low level routine used internally
   by libtab.  User programs should call #table_row.

   Returns a TableRow structure containing the parsed contents of the
   next line from file.
 #*/
TableRow table_parsline		PROTOTYPE((
	  File file 		/* the file to read the line from. 	*/
	, TableRow row		/* the row to fill in.			*/
	, int mode		/* the parsing mode			*/
	, int n			/* number of columns expected		*/
));
TableRow table_parsnice		PROTOTYPE((
	  File file 		/* the file to read the line from. 	*/
	, TableRow row		/* the row to fill in.			*/
	, int mode		/* the parsing mode			*/
	, int n			/* number of columns expected		*/
));

TableRow table_parsbuff		PROTOTYPE((
	  File     file		/* the file to read the line from. 	*/
	, TableRow row		/* the row to fill in.			*/
));

/* table_colpad - pad column output for justification.

   Output string to the file in a column of width characters.  If precision is
   non-zero, the output is right justified as a number with the "." precision
   characters from the right edge of the column.  Otherwise string is left
   justified.  The type may override the default justification with one of the
   following \#defined values:

	* TAB_DEFAULT
	* TAB_LEFT
	* TAB_RIGHT
	* TAB_CENTER
	* TAB_SKIP

   This is a low level routine to output one table column at a time with the 
   proper justification.  User programs should call #table_put.

 */

void  	 table_colpad		PROTOTYPE((
	  File file 		/* output file.				*/
	, char *string 		/* string to pad.			*/
	, int mwidth		/* width of output in characters.	*/
	, int owidth		/* width of output in characters.	*/
	, int precision 		/* precision of numeric output.		*/
	, int type		/* justification padding type.		*/
));

/* table_coladd - add a column to the table.

   Add name column to the table.

   Returns the column number of the new column.
 */
int table_coladd		PROTOTYPE((
	  TableHead table 	/* table to add the column to.		*/
	, char *name		/* name for the new column.		*/
));

/* table_colarg - parse a table program column argument.

DESCRIPTION
   #table_colarg parses any flags from the end of a column name argument given
   to a table program on the program command line and calls #table_col to look
   up the column in the table header.  #table_arg may also be used to parse
   column flags without looking up the column number.  If table is NULL
   #table_colnum is not called and the return value is 0.

RETURNS
   #table_colarg returns the column number of the column or 0 if the column is not in
   the table.
 */
int table_colarg		PROTOTYPE((
	  TableHead table	/* table to lookup column in.		*/
	, char *column 		/* column argument.			*/
	, char *flag 		/* string of allowed flags		*/
        , char *form[]		/* array of strings indicating flag types*/
	, TableColArg *val	/* array of returned flag values.	*/
	, int  *check		/* array of returned flag switched	*/
));

int table_colargpat		PROTOTYPE((
			TableHead	 tabl,
                        char            *col,
			char		*name,
			int		 leng,
                        char            *flag,
                        char            *form[],
                        TableColArg     *value,
                        int             *check,
			int		*list,
			int		 maxx
));


/* table_colnum - look up a column in a table header.

   Look up name in the table and return its column number.

   Returns the column number of name or 0 if name is not a column of table.
 */
int table_colnum		PROTOTYPE((
	  TableHead table  	/* table to lookup column in.		*/
	, char *name		/* name of the column to lookup.	*/
));
int table_colnummatch		PROTOTYPE((
     TableRow	 row,
     char	*text,
     int	 index,
     int	 mode
));

/* table_colnam - returns the name of the column number.
 
   Returns the column name or NULL if number is greater than ncol.
 */
char *table_colnam                PROTOTYPE((
          TableHead table         /* table to lookup column in.           */
        , int num                 /* number of the column to lookup.        */
));

/*#3 table_hdrfree - release memory of the table header structures when done.
 #*/
void table_hdrfree		PROTOTYPE((
	TableHead table		/* table to be done with.		*/
));

/*#3 table_hdrnth - returns the nth header keyword.

 #*/
TableRow table_hdrnth		PROTOTYPE((
          TableHead table         /* table to lookup keyword in.           */
        , int num                 /* number of the keyword to lookup.        */
));

/*#3 table_rowfree - release memory of the row structures when done.
 #*/
void table_rowfree		PROTOTYPE((
	TableRow row		/* table row to be done with.		*/
));

/* table_header -  read the table header from a file.

   table_header reads in a header from file and allocates the header data
   structures.  The mode parameter indicates how the table rows will be
   treated in later operations.  TAB_PARSE will cause table_row() to 
   trim extra spaces and record width and numeric precision of rows 
   as they are read.  TAB_JUSTIFY will cause table_put() to output the 
   rows with column justifications.  


    * mode == 0

		the rows of a table will pass through a program calling
		table_row() and table_put() without modification.  This is the
		behavoir of column with no argumants.

    * mode == TAB_PARSE

		the rows of a table will pass through a program calling
		table_row() and table_put() with all extra spaces trimmed.
		This is the behavoir ot trim with no arguments.

    * mode == TAB_PARSE | TAB_JUSTIFY

		the rows of a table will pass through a program calling
		table_row() and table_put() with default justifications.  This
		is the behavoir of justify with no arguments.

RETURNS
   #table_header returns a pointer to a table header data structure.
 */
TableHead table_header		PROTOTYPE((
	  File file 		/* file to read header from.		*/
	, int mode		/* parse mode.				*/
));

/* table_rowput - output a table row.

   The row is output on file with column selected and justified as indicated.

 */
void 	  table_rowput		PROTOTYPE((
	  File file 		/* output file.				*/
	, TableHead table 	/* table header.			*/
	, TableRow row	 	/* row to output.			*/
	, int *justify 		/* column justification flags.		*/
	, int *select 		/* column selection flags.		*/
	, int nsel		/* the number of selected columns.	*/
));

#define TBLHDR_VALUES		1
#define TBLHDR_TEXT		2
#define TBLHDR_HEADLINE		4
#define TBLHDR_DASHLINE		8
#define TBLHDR_ALL		(TBLHDR_VALUES|TBLHDR_TEXT|TBLHDR_HEADLINE|TBLHDR_DASHLINE)

/*#3 table_hdrput - output a table header.

DESCRIPTION
   The table header is output on file.

 #*/
void 	  table_hdrput		PROTOTYPE((
	  File file 		/* output file.				*/
	, TableHead table 	/* table header to output.		*/
	, int  flags		/* header output flags			*/
	, int *justify 		/* column justification flags.		*/
	, int *select 		/* column selection flags.              */
	, int nsel		/* the number of selected columns.      */
));

/* #table_rowget -  read the next row from a table file.

DESCRIPTION
   A row is read in from file.  If the row argumant is NULL a new
   row structure is allocated for the row.

   Returns a TableRow.
 */
TableRow  table_rowget		PROTOTYPE((
	  File file 		/* file to read from.			*/
	, TableHead table 	/* table header.			*/
	, TableRow row		/* row structure.			*/
	, int	 *justify	/* Optional justification flags for each column. */
	, int	 *select	/* Optional selection vector for each column. */
	, int	  nsel
));

#if 0
/* #table_setcol - set the value of a column in a row.

DESCRIPTION
   The column of row is set to value.
 #*/
int 	  table_setcol		PROTOTYPE((
	  TableHead table 	/* table header				*/
	, TableRow row 		/* row to set value in.			*/
	, int column 		/* column number to set.		*/
	, char *value 		/* new value for row-column.		*/
	, int length		/* length of the new value.		*/
));
#endif

/* #table_rowtrim -  trim blanks out of a row structure.

   Blank spaces in the values of the row are trimmed out inplace.
   The widths and numeric precision of the values in the row are
   recorded in the width and precision arrays. #table_trim is a 
   low level routine invoked by #table_row on each row of the
   table read when the table header mode was #TABLE_PARSE.
 */
void 	  table_rowtrim		PROTOTYPE((
	  TableRow row 		/* the row to trim.			*/
	, int *width 		/* width of each column.		*/
	, int *precision 	/* precision of each column.		*/
	, int  ncol		/* number of entries in width and precision */
	, int *justify		/* justification options for each col	*/
	, int *selection	/* selection of columns			*/
	, int  nsel		/* number of selections			*/
));

/* #table_hdrgetncol -  get the number of items in a header value.
 */
int 	  table_hdrgetncol		PROTOTYPE((
	  TableHead table	/* table header.			*/
	, char *name 		/* name of header value.		*/
	, int row		/* array index of value.		*/
));
/* #table_hdrget -  access a header value.

   Access the header value #name as an integer, double or string.  If the value 
   is an array the i'th value in the array is returned.

   #table_hdrget returns a pointer to the actual header buffer.  It should not
   be modified in any way.

 */
char 	 *table_hdrget		PROTOTYPE((
	  TableHead table	/* table header.			*/
	, char *name 		/* name of header value.		*/
	, int row		/* array index of value.		*/
	, int col		/* array index of value.		*/
));

/* #table_hdrgetncol returns the number of values in this header keyword.
 */
int	  table_hdrgetn		PROTOTYPE((
	  TableHead table       /* table header.                        */
        , char *name            /* name of header value.                */
	, int row		/* array index of value.		*/
));

/* #table_hdrgeti returns the header value an integer.

 */
int	  table_hdrgeti		PROTOTYPE((
	  TableHead table	/* table header.			*/
	, char *name 		/* name of header value.		*/
	, int row		/* array index of value.		*/
	, int col		/* array index of value.		*/
));

/* #table_hdrgetd the header value as a double.  The value is converted using #SAO_strtod
    and recoginises sexgasmal ascii formats (HH:MM:SS.SSSS).

 */
double	  table_hdrgetd		PROTOTYPE((
	  TableHead table		/* table header.			*/
	, char *name 		/* name of header value.		*/
	, int row		/* array index of value.		*/
	, int col		/* array index of value.		*/
));

/* #table_hdrgets returns a copy of the column value made with with #strdup.
   The user may modify this copy as needed and should free it with #free when
   the value is no longer needed.

 */
char 	 *table_hdrgets		PROTOTYPE((
	  TableHead table		/* table header.			*/
	, char *name 		/* name of header value.		*/
	, int row		/* array index of value.		*/
	, int col		/* array index of value.		*/
));

void 	 table_hdrset		PROTOTYPE((
	  TableHead table	/* table header.			*/
	, char *name 		/* name of header value.		*/
	, int row		/* row index of value.			*/
	, int col		/* col index of value.			*/
	, char *val		/* value.				*/
	, int	flag
));
void 	 table_hdrsetb		PROTOTYPE((
	  TableHead table	/* table header.			*/
	, char *name 		/* name of header value.		*/
	, int row		/* row index of value.			*/
	, int col		/* col index of value.			*/
	, int val		/* value.				*/
	, int	flag
));
void 	 table_hdrseti		PROTOTYPE((
	  TableHead table	/* table header.			*/
	, char *name 		/* name of header value.		*/
	, int row		/* row index of value.			*/
	, int col		/* col index of value.			*/
	, int val		/* value.				*/
	, char *format		/* *format;				*/
	, int	flag
));
void 	 table_hdrsetd		PROTOTYPE((
	  TableHead table	/* table header.			*/
	, char *name 		/* name of header value.		*/
	, int row		/* row index of value.			*/
	, int col		/* col index of value.			*/
	, double val		/* value.				*/
	, char	*format		/* *format;				*/
	, int	flag
));
void 	 table_hdrsets		PROTOTYPE((
	  TableHead table	/* table header.			*/
	, char *name 		/* name of header value.		*/
	, int row		/* row index of value.			*/
	, int col		/* col index of value.			*/
	, char *val		/* value.				*/
	, int	flag
));

/* #table_hdrV returns the row structure that holds the header value	
 */
TableRow	table_hdrfind	PROTOTYPE((
          TableHead table       /* table header.                        */
        , char *name            /* name of header value.                */
	, int row		/* row index of value.		*/
));

/* #table_extract -  extract a table from a multi table file.

   The named table is extracted from the multi table file using the tablex
   program.  Optionally the extracted table may be further processed by
   a filter given in command.

   #table_extract returns the file handled opened by popen().
 */
void	table_extract		PROTOTYPE((
	  File file 	/* table file name			*/
	, TableSection section /* Section descriptor		*/
	, int nsec	/* Number of seciton descriptors	*/
));

void	table_extractva		PROTOTYPE((
	  File file 	/* table file name			*/
	, ...
));

/* #table_load - load a table directly into a data structure.

   #table_load provides a top level routine for loading table data directly into
   a program data structure.  The data structure is described by the TblDescriptor #T.
 */
void	*table_load		PROTOTYPE((
	  File ifile		/* File pointer to read table from.	*/
	, TableHead t		/* Table header if already read in	*/
	, TblDescriptor T		/* Table column descriptor.		*/
	, void *hbase		/* Base pointer for header values.	*/
	, TableRow **rows		/* Returned row array.			*/
	, void *tbase		/* Base pointer for data structure array. */
	, size_t size		/* Size of one data structure.		*/
	, size_t *nrecs		/* Number of records read.		*/
));

/* #table_loadva - load a table directly into a data structure from varargs table description.

   #table_loadva constructs a TblDescriptor from its arguments and calls 
   #table_load.
 */
void	*table_loadva		PROTOTYPE((
	  File ifile		/* File pointer to read table from.     */ 
	, TableHead *t		/* Table header if already read in      */ 
	, void *hbase		/* Base pointer for header values.      */ 
	, TableRow **rows		/* Returned row array.                  */ 
	, void *tbase		/* Base pointer for data structure array. */ 
	, size_t size		/* Size of one data structure.          */ 
	, size_t *nrecs		/* Number of records read.              */
	, ...
));
TblDescriptor  table_create_descrip PROTOTYPE((size_t *nrecs,...));
int table_loadrow(
	File		ifile,
	TableHead	t,
	TableRow	r,
	TblDescriptor	T,
	void		*rec);
int table_loadmap PROTOTYPE((
                  TableHead	 t,
                  TblDescriptor	 T));


void	*table_loadva		PROTOTYPE((
	  File ifile		/* File pointer to read table from.     */ 
	, TableHead *t		/* Table header if already read in      */ 
	, void *hbase		/* Base pointer for header values.      */ 
	, TableRow **rows		/* Returned row array.                  */ 
	, void *tbase		/* Base pointer for data structure array. */ 
	, size_t size		/* Size of one data structure.          */ 
	, size_t *nrecs		/* Number of records read.              */
	, ...
));


TableRow table_value		PROTOTYPE((
	TableHead t, char *name
));

/* #table_mode - set the table i/o mode.

	The table mode may be set to TABLE_PARSE and
	TABLE_JUSTIFY.  See @table_header.
 */
void table_mode PROTOTYPE((TableHead t, int m, int x));

/* #table_ncol - return the number of column in a table row structure.
 */
int  table_ncol PROTOTYPE((TableHead t)); 

/* table_ofs - change the output field separator.

	The output field separator is a single character that @table_row
	places between the column values when it prints a row.  The
	previous output field separator is returned.
 */
int  table_ofs PROTOTYPE((
	  TableHead t	/* Table header pointer */
	, int newofs	/* New output field separator */
	));


/* #table_ors - change the output record separator.

	The output record separator is a single character that @table_row
	places after the last column value when it prints a row.  The
	previous output record separator is returned.
 */
int  table_ors PROTOTYPE((TableHead t, int newors));

TableRow table_colset PROTOTYPE((
	  TableRow	row	/* The row in which to change the column
				   value.				*/
	, int		col	/* The column to set.			*/
	, char	       *value	/* A character pointer to the new value */
	, int		length	/* The length of the new value		*/
	));

int table_isid PROTOTYPE((char *));


int table_hash PROTOTYPE((char *, int n));
int table_enum();

int table_colfmt PROTOTYPE((TableHead table, int col, char *format));
void table_setdash PROTOTYPE((TableHead table, int  col, int n));

int table_hdrfix(
	File	 ofile,
	char	*str,
	int	 n,
	int	 d
);

void table_dashes(
	File	file,
	int 	n,
	int	ch
);

void table_colseti(
	TableRow	 row,	/* The row in which to change the column
				   value.				*/
	int		 col,	/* The column to set.			*/
	int	         value,	/* A character pointer to the new value */
	char		*format
);

void table_colsetd(
	TableRow	 row,	/* The row in which to change the column
				   value.				*/
	int		 col,	/* The column to set.			*/
	double	         value,	/* A character pointer to the new value */
	char		*format
);

void table_colsets(
	TableRow	 row,	/* The row in which to change the column
				   value.				*/
	int		 col,	/* The column to set.			*/
	char	        *value	/* A character pointer to the new value */
);

void table_rowfmt(
	TableHead	table,
	TableRow	row,
	int		*select,
	int		nsel
);

void table_rowbuf_open(
	File	  file,
	TableHead table
);

void table_hdrdel(
	TableHead	table,
	char		*name,
	int		 row
);

int GetChar(File file);
int table_hdrnvalues(TableHead table);

/* Place nothing past here!! */
#endif
