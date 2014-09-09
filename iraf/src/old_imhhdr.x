#Copyright(c) 1986 Association of Universities for Research in Astronomy Inc.

include	<error.h>
include	<ctype.h>
include	<imhdr.h>
include	<imio.h>
include	<time.h>

define	SZ_DIMSTR	(IM_MAXDIM*4)
define	SZ_MMSTR	40
define	USER_AREA	Memc[($1+IMU-1)*SZ_STRUCT + 1]
define	LMARGIN		4


# IMHEADER -- Read contents of an image header and print on STDOUT.

procedure t_imhhdr()

int	list, nimages
pointer	sp, template, image, outfile, fd
bool	long_format, user_fields
int	imtopen(), imtgetim(), open
bool	clgetb()

begin
	call smark (sp)
	call salloc (image, SZ_FNAME, TY_CHAR)
	call salloc (template, SZ_LINE, TY_CHAR)
	call salloc (outfile, SZ_LINE, TY_CHAR)

	call clgstr ("input", Memc[template], SZ_LINE)
	list = imtopen (Memc[template])
	call clgstr ("output", Memc[outfile], SZ_LINE)

	long_format = clgetb ("longheader")
	user_fields = clgetb ("userfields")
	nimages = 0

	fd = open(Memc[outfile],NEW_FILE,TEXT_FILE)
	while (imtgetim (list, Memc[image], SZ_FNAME) != EOF) {
	    nimages = nimages + 1
	    call fprintf(fd,"\n%s\n")
		call pargstr(Memc[image])
	    if (long_format && nimages > 1)
		call putci (STDOUT, '\n')
	    iferr (call imphdr (fd, Memc[image], long_format, user_fields, nimages))
		call erract (EA_WARN)
	    call flush (STDOUT)
	}

	call imtclose (list)

	call sfree (sp)
end


# IMPHDR -- Print the contents of an image header.

procedure imphdr (fd, image, long_format, user_fields, nimages)

int	fd
char	image[ARB]
bool	long_format
bool	user_fields
int	nimages

int	len, bufmax
pointer	im, sp, ctime, mtime, ldim, pdim, title, lbuf, ip 
pointer	labbuf, hdrbuf, valbuf, delbuf, labptr, valptr, hdrptr 
pointer	delconst 
int	gstrcpy(), strlen(), envgeti()
pointer	immap()
errchk	im_fmt_dimensions, immap, access 
define	done_ 91

begin
	# Allocate automatic buffers.
	call smark (sp)
	call salloc (ctime, SZ_TIME,   TY_CHAR)
	call salloc (mtime, SZ_TIME,   TY_CHAR)
	call salloc (ldim,  SZ_DIMSTR, TY_CHAR)
	call salloc (pdim,  SZ_DIMSTR, TY_CHAR)
	call salloc (title, SZ_LINE,   TY_CHAR)
	call salloc (lbuf,  SZ_LINE,   TY_CHAR)
	call salloc (delconst,  SZ_LINE, TY_CHAR)
#	call salloc (deltemp,  SZ_LINE, TY_CHAR)

	call strcpy("--------	",Memc[delconst],10)

	bufmax= min(100*SZ_LINE,envgeti("min_lenuserarea"))
	call salloc (labbuf,  bufmax,   TY_CHAR)
	call salloc (valbuf,  bufmax,   TY_CHAR)
	call salloc (hdrbuf,  bufmax,   TY_CHAR)
	call salloc (delbuf,  bufmax,   TY_CHAR)

	labptr = labbuf
	valptr = valbuf
	hdrptr = hdrbuf
#	delptr = delbuf

	im = immap (image, READ_ONLY, 0)

	# Format subscript strings, date strings, mininum and maximum
	# pixel values.

	call im_fmt_dimensions (im, Memc[ldim], SZ_DIMSTR, IM_LEN(im,1))
	call im_fmt_dimensions (im, Memc[pdim], SZ_DIMSTR, IM_PHYSLEN(im,1))
	call cnvtime (IM_CTIME(im), Memc[ctime], SZ_TIME)
	call cnvtime (IM_MTIME(im), Memc[mtime], SZ_TIME)

	# Strip any trailing whitespace from the title string.
	ip = title + gstrcpy (IM_TITLE(im), Memc[title], SZ_LINE) - 1
	while (ip >= title && IS_WHITE(Memc[ip]) || Memc[ip] == '\n')
	    ip = ip - 1
	Memc[ip+1] = EOS

	# Begin printing image header.
	call sprintf (Memc[labptr], 10, "%s\t")
	    call pargstr ("TITLE")
	len=5
	labptr=labptr+len+1
	call strcat(Memc[delconst],Memc[delbuf],bufmax)
	call sprintf (Memc[valptr], 70, "%s\t")
	    call pargstr (Memc[title])
	len=strlen(Memc[title])
	valptr=valptr+len+1

	# All done if not long format.
	if (! long_format)
	    goto done_

	call sprintf (Memc[labptr], 10, "PIXFILE\t")
	len=7
	labptr=labptr+len+1 

	call strcat(Memc[delconst],Memc[delbuf],bufmax)
	call sprintf (Memc[valptr], 70, "%s\t")
	    call pargstr (IM_PIXFILE(im))
	len=strlen(IM_PIXFILE(im))
	valptr=valptr+len+1

#	# Print the history records.
#	if (strlen (IM_HISTORY(im)) > 1) {
#	    hi = stropen (IM_HISTORY(im), ARB, READ_ONLY)
#	    while (getline (hi, Memc[lbuf]) != EOF) {
#		call putline (fd, "    ")
#		call putline (fd, Memc[lbuf])
#		if (stridxs ("\n", Memc[lbuf]) == 0)
#		    call putline (fd, "\n")
#	    }
#	    call close (hi)
#	}

	if (user_fields)
	    call imh_print_user_area (im,labptr,valptr,hdrptr,delbuf,delconst,bufmax)

	len=strlen(Memc[valbuf])
	if( Memc[valbuf+len-1] == '\t')
	    Memc[valbuf+len-1]=EOS	
	len=strlen(Memc[labbuf])
	if( Memc[labbuf+len-1] == '\t')
	    Memc[labbuf+len-1]=EOS	
	len=strlen(Memc[delbuf])
	if( Memc[delbuf+len-1] == '\t')
	    Memc[delbuf+len-1]=EOS	

#	    call printf ("hdr\n%s\n")
## MC - do we need this?
	call fprintf (fd,"%s\n")
		call pargstr(Memc[hdrbuf])
#	    call printf ("labels\n%s\n")
#        if( nimages == 1)
#	{
	    call fprintf (fd,"%s\n")
		call pargstr(Memc[labbuf])
#	    call printf ("delimiter\n%s\n")
	    call fprintf (fd,"%s\n")
		call pargstr(Memc[delbuf])
#	}
#	call printf ("values\n%s\n")
	call fprintf (fd,"%s\n\n")
		call pargstr(Memc[valbuf])
done_
	call imunmap (im)
	call sfree (sp)
end


# IM_FMT_DIMENSIONS -- Format the image dimensions in the form of a subscript,
# i.e., "[nx,ny,nz,...]".

procedure im_fmt_dimensions (im, outstr, maxch, len_axes)

pointer	im
char	outstr[ARB]
int	maxch, i, fd, stropen()
long	len_axes[ARB]
errchk	stropen, fprintf, pargl

begin
	fd = stropen (outstr, maxch, NEW_FILE)

	call fprintf (fd, "[%d")
	    call pargl (len_axes[1])

	do i = 2, IM_NDIM(im) {
	    call fprintf (fd, ",%d")
		call pargl (len_axes[i])
	}

	call fprintf (fd, "]")
	call close (fd)
end


# PARGTYPE -- Convert an integer type code into a string, and output the
# string with PARGSTR to FMTIO.

procedure pargtype (dtype)

int	dtype

begin
	switch (dtype) {
	case TY_UBYTE:
	    call pargstr ("ubyte")
	case TY_BOOL:
	    call pargstr ("bool")
	case TY_CHAR:
	    call pargstr ("char")
	case TY_SHORT:
	    call pargstr ("short")
	case TY_USHORT:
	    call pargstr ("ushort")
	case TY_INT:
	    call pargstr ("int")
	case TY_LONG:
	    call pargstr ("long")
	case TY_REAL:
	    call pargstr ("real")
	case TY_DOUBLE:
	    call pargstr ("double")
	case TY_COMPLEX:
	    call pargstr ("complex")
	case TY_POINTER:
	    call pargstr ("pointer")
	case TY_STRUCT:
	    call pargstr ("struct")
	default:
	    call pargstr ("unknown datatype")
	}
end


# IMH_PRINT_USER_AREA -- Print the user area of the image, if nonzero length
# and it contains only ascii values.

procedure imh_print_user_area (im,labptr,valptr,hdrptr,delbuf,delconst,bufmax)

pointer	im		# i: image descriptor
#pointer	labbuf		# i: buffer of column labels
#pointer valbuf		# i: buffer of column values
#pointer hdrbuf		# i: buffer of hdr values
pointer labptr,valptr,hdrptr,delconst,delbuf
int	bufmax		# i: max buffer length allocated

pointer	sp, lbuf, ip, jp 
int	in, ncols, min_lenuserarea, len 
int	getline(),stropen, strlen(), strmatch()
char	temp[20]

begin
	call smark (sp)
	call salloc (lbuf, SZ_LINE, TY_CHAR)

	# Open user area in header.
	min_lenuserarea = (LEN_IMDES + IM_LENHDRMEM(im) - IMU) * SZ_STRUCT - 1
	in = stropen (USER_AREA(im), min_lenuserarea, READ_ONLY)
#	ncols = envgeti ("ttyncols") - LMARGIN
	ncols = 80

	len=strlen(Memc[labptr])
	# Copy header records to the output, stripping any trailing
	# whitespace and clipping at the right margin.

	while (getline (in, Memc[lbuf]) != EOF) {
	    for (ip=lbuf; Memc[ip] != EOS && Memc[ip] != '\n' && Memc[ip] != ' ' && Memc[ip] != '=';  ip=ip+1)
		;

	    if( lbuf-ip < 8)
	        Memc[ip] = EOS
	    call strcpy(Memc[lbuf],temp,8)
	    len=strmatch(temp,"COMMENT")
	    if( len == 0)
	        ip=max(ip,lbuf+9)	    
	    else
		ip=max(ip,lbuf+8)

	    # Now get the "value" part of the header line
	    #     Treat strings different than others because term. different
	    if(Memc[lbuf+10] ==	'\'')
	    {
		ip=lbuf+10+1
		#  Look for "closing" quote
	        for (jp=ip; Memc[jp] != '\n' && Memc[jp] != '\'';  jp=jp+1)
		;
		jp=max(ip,jp-1) # Don't keep the quote
#  MC debugging
		    #Memc[jp+1]=EOS
	 	    #call printf("string len: %d string: %s\n")
		    #call pargi(jp-ip)
		    #call pargstr(Memc[ip])
	    }
	    else
	    {
		# Now get the non-string "value" part of the header line
	        while((Memc[ip] == '=') || (Memc[ip] == '\'') || (Memc[ip] == ' ') )
		    ip=ip+1
	        # Skip the beginning "=", string quote or space
	        #  look for end of line or / starting comment
	        for (jp=ip; Memc[jp] != '\n' && Memc[jp] != '/' ;  jp=jp+1)
		;
	    }
	    # remove trailing blanks
	    while (jp > ip && (Memc[jp-1] == ' ' || Memc[jp-1]=='\'') )
		jp = jp - 1
	    # remove "too long" fields
	    if (jp - ip > ncols)
		jp = ip + ncols 
#	    Memc[jp]=EOS
	    Memc[jp+1]=EOS
	    len=strmatch(temp,"COMMENT")
	    if( len==0 )  # no match
	    {
	        #call printf("%s\n")
		#    call pargstr(temp)
	        call sprintf(Memc[labptr],10,"%s\t")
		    call pargstr(temp)
		len=strlen(temp)
		call strcat(Memc[delconst],Memc[delbuf],bufmax)
		labptr=labptr+len+1
	        #call printf("%s\n")
		#    call pargstr(Memc[ip])
    	        call sprintf (Memc[valptr], 70, "%s\t")
		    call pargstr(Memc[ip])
		len=strlen(Memc[ip])
		valptr=valptr+len+1
	    }
	    else
    	    {
	        call sprintf (Memc[hdrptr], 70, "\t%s\n")
		    call pargstr(Memc[ip])
		len=strlen(Memc[ip])
		hdrptr=hdrptr+len+2
	    }
	}

	call close (in)
	call sfree (sp)
end
