/* xawk.h
 */

#ifdef __STDC__
#define PROTOTYPE(X)	X
#else
#define PROTOTYPE(X)   ( )
#endif

/* This FBLOCK decliaration must exactally match the one in symtype.h 
   by having a copy of it here xawk.h is the only file that must be 
   included in the external function source code.
 */

typedef  struct FBLOCK {
	char *name ;
	void *code  ;
	Rettype (*addr)()  ;
	unsigned short nargs ;
	unsigned short nrets ;
	char *typev ;  /* array of size nargs holding types */
} FBLOCK ;   /* function block */


/* Stack access functions
 */

/* Return argument #n from the stack as an awk CELL pointer
 */
void 	*xawk_arg PROTOTYPE((void *sp, int nargs, int n));


/* Extract a number from an awk stack CELL pointer.
 */
double	 xawk_num PROTOTYPE((void *cp));

/* Extract a string from an awk stack CELL pointer.
 */
char 	*xawk_str PROTOTYPE((void *cp));


/* Function to get argument #n as a number or string.
 */
double	 xawk_numarg PROTOTYPE((void *sp, int nargs, int n));
char	*xawk_strarg PROTOTYPE((void *sp, int nargs, int n));


/* Functions to index a CELL pointer as an array with a number
   or a string.
 */
void	*xawk_stridx PROTOTYPE((void *ap, char *idx));
void 	*xawk_numidx PROTOTYPE((void *ap, double idx));

/* Function to push return values onto the stack.
 */
void 	*xawk_pshnum PROTOTYPE((void *sp, double d, int format));
void 	*xawk_pshstr PROTOTYPE((void *sp, char *str));
void	 xawk_return PROTOTYPE((void *sp, int n));


/* Functions to set values in CELL pointers returned by
   array index functions above.
 */
void	 xawk_setnum PROTOTYPE((void *cell, double d));
void	 xawk_setstr PROTOTYPE((void *cell, char *str));

