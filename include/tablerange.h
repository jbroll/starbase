
void *table_rangerows(
	File	 	 file,
	TableHead	 table,
	TableRow	 R,
	Range		*range,
	void    	*(*func)(),
	void		*data
);

void table_blankrangerows(
        File     	 file,
        Range   	*range
);

void table_printrangerows(
	TableHead	 table,
        File     	 ifile,
        File     	 ofile,
        Range   	*range
);

Range *table_readrange(
	File	 file,
	int	 mode,		/* Union / Intersection mode	*/
	int	 starcol,
	int	 end_col
);
