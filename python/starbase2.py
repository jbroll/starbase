
import sys,os,re, subprocess, tempfile


from threading import Thread

"""
        Add colnum
        Add insert header before, after other header value
        Add add/del header
        Add add/del row
        Add add/del col

        Add row formatting

"""


try:
    import starbase_data			# Try to load a Cython data reader.  Its Faster
    Starbase_readdata = starbase_data.readdata

except ImportError:
    def Starbase_readdata(file, types) :	# This is the same code just not Cython compiled
	    data = []

	    for row in file.readlines() :
		r = []
		for i, val in enumerate(row.rstrip("\r\n").split("\t")) :
		    try:
			value = types[i](val.strip())

		    except ValueError:
			value = val

		    r.append(value)

		data.append(r)

	    return data

Starbase_deftype = float
def Starbase_hdrtype(value) : 
    try: 		return Starbase_deftype(value)
    except ValueError:	return value

	
def Starbase_vector(value, type) : return value

try:
    import numpy as np
    Starbase_deftype = np.double
    def Starbase_vector(value, type) : return np.array(value, dtype=type)
except ImportError:
    pass
	

class Starbase(object):
    """
        # Starbase Data Tables in python
        # 
        # Read a table from disk:
        #
        >>> tab = Starbase("input.tab", deftype=int)

        #
        # A table may be read from a command pipeline by placing "|" as the 
        # first character in the filename:
        #
        >>> tab = Starbase('| row < input.tab "X > 3"', deftype=int)

        #
        # A table may be read from an open file descriptor:
        #
        >>> fp = open("input.tab")
        >>> tab = Starbase(fp, deftype=int)

        #
        # The data is stored as a list of lists.  It can be accessed directly
        # from the Starbase object.  Row and column indexing is zero based as
        # in python.  Columns may be indexed by integer or column name string:
        #
        >>> tab[0][0]                   # get value at 0, 0
        1

	#
	# A different default type can be set with the "deftype" keyword.
	#
        tab = Starbase("| jottable 10 x y z", deftype = float)

        x = tab[0].x + tab[0].y
        print x
        2

        >>> tab[2][1] = 5               # set value at 0, 0

        >>> tab[1]["Y"]                 # get value at row 1 col "Y"
        2

        >>> tab[4]["Y"] = 9             # set value at row 5 col "Y"

        #
        # Rows can be dotted too.
        #
        >>> tab[4].Y
        9

        >>> tab[4].Y = 8

        #
        # Table header values may be accessed by using python "dot" notation or
        # indexing the Starbase object with a string. Note that header values are
        # arrays and thus need to be indexed just like rows:
        #
        >>> tab.label = "label string"  # set header value

        >>> tab.label[0]                # or
        'label string'

        >>> tab["label"][0]
        'label string'

        >>> tab.label[0]
        'label string'

        #
        #
        # Iterating over the table returns each row of the table in turn:
        #
        >>> for row in tab :
        ...     print row
        [1, 1, 1]
        [2, 2, 2]
        [3, 5, 3]
        [4, 4, 4]
        [5, 8, 5]

        #
        # Rows can be sliced and iterated over:
        #
        >>> for row in tab[0:2] :
        ...     print row
        [1, 1, 1]
        [2, 2, 2]

        #
        # The row itself can be iterated of:
        #
        >>> for val in tab[2] :
        ...     print val
        3
        5
        3

        # 
        # Slice the rows of a data column.  This works as an a notation to 
        # select a column vector for input to numpy array().
        #
        >>> tab[:].X
        [1, 2, 3, 4, 5]

        # 
        # Or slice a few rows of the column:
        #
        >>> tab[0:2].X
        [1, 2]


	>>> print(tab[(1, 2)])				# doctest: +NORMALIZE_WHITESPACE
        2	2	2
	3	5	3

	>>> print(tab[1:3])				# doctest: +NORMALIZE_WHITESPACE
        2	2	2
	3	5	3

	>>> print(tab[1:3].X)
        [2, 3]

	>>> print tab[[1, 3]].X
        [2, 4]


	# Select a few columns from the data
	#
	>>> tab[["X", "Y"]]
	[[1, 1], [2, 2], [3, 5], [4, 4], [5, 8]]

	>>> tab[["X", "Z"]]
	[[1, 1], [2, 2], [3, 3], [4, 4], [5, 5]]

	# Two columns from a single row
	#
	>>> tab[1][["X", "Y"]]
	[2, 2]

	# Two columns from a slice of rows
	#
	>>> tab[2:4][["X", "Y"]]
	[[3, 5], [4, 4]]

	# Typical loop over rows in a table
	#
	>>> for (x, y) in tab[2:4][["X", "Y"]] :
	...  print x, y
	3 5
	4 4

        #
        # The values are stored as float by default.  An optional keyword
        # patameter "types" may be used to add data types to columns.  This 
        # makes using tables in expressions less painful.
        #
        >>> T = Starbase("| jottable 5 x y z", types = { "x" : float, "y" : int })
	>>> print T							# doctest: +NORMALIZE_WHITESPACE
	x	y	z
	-	-	-
	1.0	1	1.0
	2.0	2	2.0
	3.0	3	3.0
	4.0	4	4.0
	5.0	5	5.0



        #
        # Alternative "arrays" constructor can be used to create
        # a starbase object from a bunch of python list or numpy
        # array data.
        #
        >>> print Starbase.arrays("XXX", [1, 2, 3], "Y", [3, 4, 5])     # doctest: +NORMALIZE_WHITESPACE
        XXX     Y
        ---     -
        1       3
        2       4
        3       5

        # 
        # Keyword arguments are also supported, but the order of 
        # columns in the starbase table is determined by the 
        # python hash not the order passed to the constructor.
        #
        >>> print Starbase.arrays(X=[1, 2, 3], Y=[3, 4, 5])             # doctest: +NORMALIZE_WHITESPACE
        Y       X
        -       -
        3       1
        4       2
        5       3

        # 
        # The Starbase table may be printed directly.  This can safely be used
        # for "small" tables (less than several megabytes).  For truely huge
        # tables, the ">" operator will iterativly print the table to a file
        # descriptor and may be faster.
        #
        >>> print tab                   # print table                   # doctest: +NORMALIZE_WHITESPACE
        label   label string
        X       Y       Z
        -       -       -
        1       1       1
        2       2       2
        3       5       3
        4       4       4
        5       8       5

        >>> fp = open('/tmp/output', 'w')
        >>> print >> fp, tab            # print to open file fp
        
        #
        >>> tab > sys.stdout            # write table to sys.stdout     # doctest: +NORMALIZE_WHITESPACE
        label   label string
        X       Y       Z
        -       -       -
        1       1       1
        2       2       2
        3       5       3
        4       4       4
        5       8       5

        >>> tab > "output.tab"          # write table to file named "output.tab"

        #
        # If the output file name passed to ">" begins with "|<" the table will be
        # filtered through a shell pipeline:
        #
        >>> t = (Starbase("| jottable 10 x", deftype=int) > "|< row 'x <= 3'")
        >>> t > sys.stdout
        x
        -
        1.0
        2.0
        3.0

        #
        # Rows and header values have an independent existence and can be
        # selected and assigned:
        #
        >>> row = tab[4]

        >>> row[2] = 4                  # set column 3 of row 6 to 4

        >>> tab[0] = tab[3]             # copy row 3 to row 0

        # Union of two tables
        #
        >>> print (t + t)
        x
        -
        1.0
        2.0
        3.0
        1.0
        2.0
        3.0

        # Difference of two tables
        #
        >>> print (tab > "|< sorttable -u X") % "X=x" - t        # doctest: +NORMALIZE_WHITESPACE
        label   label string
        x
        -
        4.0
        5.0
        """


    class StarbaseHdr(object):
        """ Enables off by 1 access to the header value list reference. 
        """

        def __init__(self, data) :
            self.data = data

        def __getitem__(self, indx) :
            if ( type(indx) == slice ) :
                return self.data[1:][indx]

            return self.data[indx+1]
            
        def __setitem__(self, indx, value) :
            self.data[indx+1] = str(value)

        def __str__(self) :
            return "\t".join(self.data[1:])


    # StarbaseRow is a friend of Starbase and accesses its __Private members.
    #
    class StarbaseRow(object):
        """ Enables column lookup by string value.  Holds the reference to a row
          and the column dictionary.
        """

        def __init__(self, tabl, indx) :
            self.__tabl = tabl
            self.__indx = indx

            self.__initialized = 1

        def __str__(self) :
	    if ( type(self.__indx) == list or type(self.__indx) == tuple ) :
		rows = [self.__tabl._Starbase__data[row] for row in self.__indx]
	    else :
		rows =  self.__tabl._Starbase__data[self.__indx]

	    return "\n".join(("\t".join((str(item) for item in row)) for row in rows))

        def __getitem__(self, indx) :
            if ( type(indx) == str ) :
                indx = self.__tabl._Starbase__indx[indx]

	    if ( type(self.__indx) == int ) :
		if ( type(indx) == list or type(indx) == tuple ) :
		    return [self.__tabl._Starbase__data[self.__indx][self.__tabl._Starbase__indx[i]] for i in indx]
		else :
		    return self.__tabl._Starbase__data[self.__indx][indx]
	    else :
		if   ( type(self.__indx) == slice ) :
		    rows = self.__tabl._Starbase__data.__getitem__(self.__indx)
		elif ( type(self.__indx) == list or type(self.__indx) == tuple ) :
		    rows = [self.__tabl._Starbase__data[row] for row in self.__indx]
		else :
		    rows = None

		if ( type(indx) == list or type(indx) == tuple ) :
		    return [[row[self.__tabl._Starbase__indx[i]] for i in indx] for row in rows]
		else :
		    return [row[indx] for row in rows]
            
        def __setitem__(self, indx, value) :
            if ( type(indx) == str ) :
                indx = self.__tabl._Starbase__indx[indx]

            self.__tabl._Starbase__data[self.__indx][indx] = self.__tabl._Starbase__type[indx](value)

        def __getattr__(self, indx) :
            return self.__getitem__(indx)

        def __iter__(self) :
            return self.__tabl._Starbase__data[self.__indx].__iter__()

        def __setattr__(self, indx, value) :
            if ( not self.__dict__.has_key("_StarbaseRow__initialized") \
              or self.__dict__.has_key(indx) ) :
                self.__dict__[indx] = value
                return

            self.__setitem__(indx, value)

    class StarbasePipeWriter(Thread) :

        def __init__(self, table, write) :
            Thread.__init__(self)
            self.table = table
            self.write = write

        def run(self) :
            self.table > self.write
            self.write.close()


    def __init__(self, fp=None, deftype = Starbase_deftype, types = {}) :
        if ( fp == None ) :
            return

        if ( type(fp) == str ) :
            fp = open(fp, "r") if ( fp[0:1] != "|" ) else os.popen(fp[1:], "r")

        self.__head = {}
        self.__line = []
        self.__type = []

        self.__headline = fp.readline().rstrip().split("\t")
        self.__dashline = fp.readline().rstrip().split("\t")

        dashes = len([s for s in self.__dashline if re.match('^-+$' , s.strip())])

        # Read lines until the dashline is found
        #
        while ( not dashes                                                                      \
                 or dashes != len([s for s in self.__headline if re.match('\w+', s.strip())]) ) :
            if ( re.match('\w+', self.__headline[0].strip()) ) :
                self.__head[self.__headline[0].strip()] = len(self.__line)

            self.__line.append(self.__headline)

            self.__headline = self.__dashline
            self.__dashline = fp.readline().rstrip().split("\t")


            dashes = len([s for s in self.__dashline if re.match('^-+$' , s.strip())])

        i = 0
        self.__indx = {}
        for col in self.__headline :
            col = col.strip()

            self.__indx[col] = i
            self.__type.append(types[col] if ( types.has_key(col) ) else deftype)
            i += 1

        # Read the data in, converting to types
        #
        self.__data = Starbase_readdata(fp, self.__type)

        self.__initialized = 1


 
    @classmethod
    def arrays(self, *args, **kwargs) :
        self = Starbase()
        args = list(args)

        self.__head = {}
        self.__line = []
        self.__type = []
        self.__headline = []
        self.__dashline = []

        for col in kwargs :
            args.append(col)
            args.append(kwargs[col])

        i = 0
        vals = []
        self.__indx = {}
        while i < len(args) :
            col = args[i]
            val = args[i+1]

            self.__indx[col] = i
            self.__headline.append(col)
            self.__dashline.append("-" * len(col))

            vals.append(val)

            i += 2

        arry = [val for val in vals]
        self.__data = zip(*arry)

        self.__initialized = 1
        return self

    def __len__(self) :
	return len(self.__data)

    def __str__(self) :
        # Cast the table as a string.
        #
        return ( "\n".join(["\t".join(row) for row in self.__line]) + "\n" if self.__line else "" ) \
               + "\t".join(self.__headline) + "\n"                                              \
               + "\t".join(self.__dashline) + "\n"                                              \
               + "\n".join(("\t".join((str(item) for item in row)) for row in self.__data))

    def __iter__(self) :
        return self.__data.__iter__()

    def __contains__(self, indx):
        if ( type(indx) == str ) :
	    return indx in self.__head
	
    def __getitem__(self, indx) :
        if ( type(indx) == str ) :
	    if indx in self.__indx :
		return Starbase_vector(Starbase.StarbaseRow(self, slice(None))[indx]
			, self.__type[self.__indx[indx]])
	    else :
		return Starbase_vector(
			[Starbase_hdrtype(val) for val in Starbase.StarbaseHdr(self.__line[self.__head[indx]])]
			, Starbase_deftype
		    )

	if ( (type(indx) == list or type(indx) == tuple) and type(indx[0]) == str ) :
	    return Starbase.StarbaseRow(self, slice(None))[indx]
	    
        return Starbase.StarbaseRow(self, indx)

    def __setitem__(self, indx, value) :
        if ( type(indx) == str ) :
            if ( not self.__head.has_key(indx) ) :
                self.__head[indx] = len(self.__line)
                self.__line.append([indx])
    
            if ( type(value) == list ) :
                self.__line[self.__head[indx]][1:] = [str(v) for v in value]
            else :
                self.__line[self.__head[indx]][1:] = [str(value)]

            return

        if ( value.__class__.__name__ == 'StarbaseRow' ) :
            value = value._StarbaseRow__tabl.__data[value._StarbaseRow__indx]

        if ( type(value) != list ) :
            raise TypeError("Starbase set row expected list")

        if ( len(self.__headline) != len(value) ) :
            raise TypeError("Starbase set row expected list of " + str(len(self.__headline)) )
                
        self.__data[indx] = [typ(val) for (typ, val) in zip(self.__type, value)]

    def __getattr__(self, indx) :
	return self.__getitem__(indx)

    def __setattr__(self, indx, value) :
        if ( not self.__dict__.has_key("_Starbase__initialized")        \
          or self.__dict__.has_key(indx) ) :
            self.__dict__[indx] = value
            return

        self.__setitem__(indx, value)

    def __pow__(self, other):
	if other == "headline" : return self.__headline

	return None

    def __binop(self, other, command) : 
        fd, file1 = tempfile.mkstemp()
        os.close(fd)
        fd, file2 = tempfile.mkstemp()
        os.close(fd)

        self  > file1
        other > file2

        return Starbase(command + " " + file1 + " " + file2)

    def __add__(self, other) :
        return self.__binop(other, "|< uniontable")

    def __or__(self, other) :
        return self.__binop(other, "|< uniontable")

    def __sub__(self, other) :
        return self.__binop(other, "|< diffrtable")

    def __and__(self, other) :
        return self.__binop(other, "|< intertable")

    def __mod__(self, columns) :
        if ( type(columns) == list or type(columns) == tuple ) :
            columns = " ".join(columns)
        
        return (self > ("|< column " + columns))

    def __invert__(self) :
        return self > "|< transposetable"

    def __floordiv__(self, columns) :
        if ( type(columns) == list or type(columns) == tuple ) :
            columns = " ".join(columns)

        return self > ("|< sorttable -u " + columns)

    def __gt__(self, file) :
        if ( type(file) == str ) :
            if ( file[0:1] == "|" ) :
		if ( file[0:2] == "|<" ) :
		    read = True
		    file = file[1:]
		else :
		    read = False

		p = subprocess.Popen(file[1:], shell=True, bufsize=1        \
			, stdin=subprocess.PIPE                             \
			, stdout=subprocess.PIPE                    \
			, stderr=subprocess.STDOUT, close_fds=True)

		writer = Starbase.StarbasePipeWriter(self, p.stdin)

                writer.start()

                if read : reply = Starbase(p.stdout)
    		else    : reply = None

                writer.join()

                return reply

            file = open(file, "w")

        for line in self.__line :
            print >> file, "\t".join(line)

        print >> file, "\t".join(self.__headline)
        print >> file, "\t".join(self.__dashline)
        for row in self.__data :
            print >> file, "\t".join((str(item) for item in row))

        return None

    def __rshift__(self, file) :
        file = open(file, "a")

        self > file


#if __name__ == '__main__':
#    # jottable 5 X Y Z > input.tab
#    #
#    import doctest
#    doctest.testmod()

