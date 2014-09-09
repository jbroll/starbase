/* table_enum
 */
/* Starbase Data Tables - An Ascii Database for UNIX
 
   copyright 1991, 1993, 1995, 1999 John B. Roll jr.
 */


#include <xos.h>
#include <xfile.h>
#include <table.h>

#include <ctype.h>

char *strupr();

int table_enum(head, tab, value, here)
	TableHead	 head;
	TableEnum	*tab;
	char		*value;
	void		*here;
{
	int	*v = (int *) here;
	int	 n;

	if ( value == NULL ) {
		*v = tab->valu;
		return 1;
	}

	while ( *value == ' ' ) value++;

	if ( *value == '\0' ) {
		*v = tab->valu;
		return 1;
	}
	
	for ( ; tab->name != NULL; tab++ ) {
	    for ( n = 0; tab->name[n]; n++ ) {
		if ( !value[n] || toupper(value[n]) != tab->name[n] ) break;
	    }
	    if ( !tab->name[n] ) {
		*v = tab->valu;
		return 1;
	    }
	}

	tab--;
	*v = tab->valu;

	return 1;
}
