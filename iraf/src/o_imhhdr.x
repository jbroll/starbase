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
pointer	sp, template, image
bool	long_format, user_fields
int	imtopen(), imtgetim()
bool	clgetb()

begin
	call smark (sp)
	call salloc (image, SZ_FNAME, TY_CHAR)
	call salloc (template, SZ_LINE, TY_CHAR)

	call clgstr ("images", Memc[template], SZ_LINE)
	list = imtopen (Memc[template])

	long_format = clgetb ("longheader")
	user_fields = clgetb ("userfields")
	nimages = 0

	while (imtgetim (list, Memc[image], SZ_FNAME) != EOF) {
	    nimages = nimages + 1
	    if (long_format && nimages > 1)
		call putci (STDOUT, '\n')
	    iferr (call imphdr (STDOUT, Memc[image], long_format, user_fields))
		call erract (EA_WARN)
	    call flush (STDOUT)
	}

	call imtclose (list)

	call sfree (sp)
end


# IMPHDR -- Print the contents of an image header.

procedure imphdr (fd, image, long_format, user_fields)

int	fd
char	image[ARB]
bool	long_format
bool	user_fields

int	hi, i, len
pointer	im, sp, ctime, mtime, ldim, pdim, hgm, title, lbuf, ip, lp 
pointer	labbuf, hdrbuf, valbuf, delbuf, labptr, valptr, hdrptr, delptr 
pointer	delconst 
int	gstrcpy(), stropen(), getline(), strlen(), stridxs()
pointer	immap(), imgl1r()
errchk	im_fmt_dimensions, immap, access, stropen, getline
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

#	delptr=delconst
#	for( delptr=delconst; (delptr-delconst)<SZ_LINE; delptr=delptr+1)
#	{
#		Memc[delptr]='-'
#	}
	call strcpy("--------	",Memc[delconst],10)
	call printf("%s\n")
		call pargstr(Memc[delconst])

	call salloc (labbuf,  100*SZ_LINE,   TY_CHAR)
	call salloc (valbuf,  100*SZ_LINE,   TY_CHAR)
	call salloc (hdrbuf,  100*SZ_LINE,   TY_CHAR)
	call salloc (delbuf,  100*SZ_LINE,   TY_CHAR)
	labptr = labbuf
	valptr = valbuf
	hdrptr = hdrbuf
	delptr = delbuf

	call printf("delconst: %d delptr: %d delbuf: %d\n")
		call pargi(delconst)
		call pargi(delptr)
		call pargi(delbuf)
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
#	call fprintf (fd, "%s%s[%s]: %s\n")
#	    call pargstr (IM_NAME(im))
#	    call pargstr (Memc[ldim])
#	    call pargtype (IM_PIXTYPE(im))
#	call printf ("start labbuf\n")
	call sprintf (Memc[labptr], 10, "%s\t")
	    call pargstr ("TITLE")
	len=5
	labptr=labptr+len+1
#	call strcpy(Memc[delconst],Memc[deltemp],10)
#	Memc[deltemp+10]=EOS
	call printf("len: %d %s\n")
	    call pargi(len)
	    call pargstr(Memc[delconst])
	call printf("delconst: %d delptr: %d\n")
		call pargi(delconst)
		call pargi(delptr)
#        call sprintf( Memc[delptr],10,"%s\t")
#	    call pargstr(Memc[delconst])
	call strcat(Memc[delconst],Memc[delbuf],10)
	call printf("constant: %s delimiter: %s\n")
		call pargstr(Memc[delconst])
		call pargstr(Memc[delbuf])
#	call printf("after buf insert - delconst: %d delptr: %d delbuf: %d delstr: %s\n")
#		call pargi(delconst)
#		call pargi(delptr)
#		call pargi(delbuf)
#		call pargstr(Memc[delbuf])
#	delptr=delptr+10+1
	call printf("after inc - delconst: %d delptr: %d\n")
		call pargi(delconst)
		call pargi(delptr)
#	for( delptr=delconst; (delptr-delconst)<=len; delptr=delptr+1)
#	{
#		Memc[delptr]='-'
#	}
#	Memc[delptr+1]=EOS
#        call sprintf( Memc[delptr],10,"%s\t")
#	    call pargstr(Memc[deltemp])
	call sprintf (Memc[valptr], 66, "%s\t")
	    call pargstr (Memc[title])
	len=strlen(Memc[title])
	valptr=valptr+len+1

	# All done if not long format.
	if (! long_format)
	    goto done_

#	call fprintf (fd, "%4w%s bad pixels, %s histogram, min=%s, max=%s%s\n")
#	    if (IM_NBPIX(im) == 0)			# num bad pixels
#		call pargstr ("No")
#	    else
#		call pargl (IM_NBPIX(im))
#
#	    hgm = IM_HGM(im)				# is hgm valid?
#	    if (HGM_TIME(hgm) == 0)
#		call pargstr ("no")
#	    else if (HGM_TIME(hgm) < IM_MTIME(im))
#		call pargstr ("old")
#	    else
#		call pargstr ("valid")

#	    if (IM_LIMTIME(im) == 0) {			# min,max pixel values
#		do i = 1, 2
#		    call pargstr ("unknown")
#		call pargstr ("")
#	    } else {
#		call pargr (IM_MIN(im))
#		call pargr (IM_MAX(im))
#		if (IM_LIMTIME(im) < IM_MTIME(im))
#		    call pargstr (" (old)")
#		else
#		    call pargstr ("")
#	    }
#
#	call fprintf (fd,
#	    "%4w%s storage mode, physdim %s, length of user area %d s.u.\n")
#	    call pargstr ("Line")
#	    call pargstr (Memc[pdim])
#	    call pargi (IM_HDRLEN(im) - LEN_IMHDR)

#	call fprintf (fd, "%4wCreated %s, Last modified %s\n")
#	    call pargstr (Memc[ctime])			# times
#	    call pargstr (Memc[mtime])

#	iferr (lp = imgl1r (im))
#	    call strcpy ("[NO PIXEL FILE]", Memc[lbuf], SZ_LINE)
#	else
#	    call strcpy ("[ok]", Memc[lbuf], SZ_LINE)

	call sprintf (Memc[labptr], 10, "PIXFILE\t")
	len=7
	labptr=labptr+len+1 
#	call sprintf (Memc[valptr], 65, "%s\t")
#	    call pargstr (IM_PIXFILE(im))
#	len=strlen(IM_PIXFILE(im))
#	valptr=valptr+len+1
#
#	call strcpy(Memc[delconst],Memc[deltemp],10)
#	Memc[deltemp+10+1]=EOS

	call printf("len: %d %s\n")
	    call pargi(len)
	    call pargstr(Memc[delconst])
	call strcat(Memc[delconst],Memc[delbuf],10)
	call printf("constant: %s delimiter: %s\n")
		call pargstr(Memc[delconst])
		call pargstr(Memc[delbuf])
#        call sprintf( Memc[delptr],10,"%s\t")
#	    call pargstr(Memc[delconst])
#	delptr=delptr+10+1
	call printf("delconst: %d delptr: %d\n")
		call pargi(delconst)
		call pargi(delptr)
	call sprintf (Memc[valptr], 65, "%s\t")
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
#	    call imh_print_user_area (im,labbuf,valbuf,hdrbuf,delbuf,labptr,valptr,hdrptr,delptr,delconst)
	    call imh_print_user_area (im,labptr,valptr,hdrptr,delbuf,delconst)

	len=strlen(Memc[valbuf])
	if( Memc[valbuf+len-1] == '\t')
	    Memc[valbuf+len-1]=EOS	
	len=strlen(Memc[labbuf])
	if( Memc[labbuf+len-1] == '\t')
	    Memc[labbuf+len-1]=EOS	
	len=strlen(Memc[delbuf])
	if( Memc[delbuf+len-1] == '\t')
	    Memc[delbuf+len-1]=EOS	

	call printf ("hdr\n%s\n")
		call pargstr(Memc[hdrbuf])
	call printf ("labels\n%s\n")
		call pargstr(Memc[labbuf])
	call printf ("delimiter\n%s\n")
		call pargstr(Memc[delbuf])
	call printf ("values\n%s\n")
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

procedure imh_print_user_area (im,labptr,valptr,hdrptr,delbuf,delconst)

pointer	im		# i: image descriptor
#pointer	labbuf		# i: buffer of column labels
#pointer valbuf		# i: buffer of column values
#pointer hdrbuf		# i: buffer of hdr values
pointer labptr,valptr,hdrptr,delconst,delbuf

pointer	sp, lbuf, ip, jp 
int	in, ncols, min_lenuserarea, len 
int	stropen(), getline(), envgeti()
int	strlen()
int	strmatch()
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
##	call printf("labptr: %d len: %d\n")
##	    call pargi(labptr)
##	    call pargi(len)
	# Copy header records to the output, stripping any trailing
	# whitespace and clipping at the right margin.

	while (getline (in, Memc[lbuf]) != EOF) {
	#  First get the 8 character keyword
#	    for (ip=lbuf;  Memc[ip] != EOS && Memc[ip] != '\n' && ip-lbuf<8;  ip=ip+1)
#		;
	    for (ip=lbuf; Memc[ip] != EOS && Memc[ip] != '\n' && Memc[ip] != ' ' && Memc[ip] != '=';  ip=ip+1)
		;
#	    while (ip > lbuf && Memc[ip-1] == ' ')
#		ip = ip - 1

#	    Memc[ip] = '\n'
#	    Memc[ip-1] = EOS
	    if( lbuf-ip < 8)
	        Memc[ip] = EOS
#	    ip=max(ip+1,lbuf+8)	    
	    call strcpy(Memc[lbuf],temp,8)
## MC this doesn't seem to work
	    len=strmatch(temp,"COMMENT")
	    if( len == 0)
	        ip=max(ip,lbuf+9)	    
	    else
		ip=max(ip,lbuf+8)

	# Now get the "value" part of the header line
	    # Skip the beginning "=", string quote or space
	    while((Memc[ip] == '=') || (Memc[ip] == '\'') || (Memc[ip] == ' ') )
		ip=ip+1
	   # why do we need this?
#	    ip=ip+1
#  comments seem to be at 31 ,but one is at 30
	    for (jp=ip; Memc[jp] != '\n' && jp<(lbuf+30) && Memc[jp] != '\'';  jp=jp+1)
		;
#	    for (jp=ip; Memc[jp] != '\n' && Memc[jp] != '\'' && ;  jp=jp+1)
#		;
	    # remove trailing blanks
	    while (jp > ip && (Memc[jp-1] == ' ' || Memc[jp-1]=='\'') )
		jp = jp - 1
	    # remove "too long" fields
	    if (jp - ip > ncols)
		jp = ip + ncols 
#	    Memc[jp] = '\n'
#	    Memc[jp-1]=EOS
##	    call printf("after key: %s lbuf: %d ip: %d jp: %d\n")
##		call pargstr(Memc[ip])
##		call pargi(lbuf)
##		call pargi(ip)
##		call pargi(jp)
#	    if( jp >= ip+1)
		    Memc[jp]=EOS
#	    else
#		    Memc[lbuf+80]=EOS	    
#	    len=strmatch(Memc[lbuf],"COMMENT")
	    len=strmatch(temp,"COMMENT")
	    if( len==0 )  # no match
	    {
	        call sprintf(Memc[labptr],10,"%s\t")
#		    call pargstr(Memc[lbuf])
		    call pargstr(temp)
#		len=strlen(Memc[lbuf])
		len=strlen(temp)
#		call strcpy(Memc[delconst],Memc[deltemp],len)
#		Memc[deltemp+len]=EOS
#	        call sprintf( Memc[delptr],10,"%s\t")
#		    call pargstr(Memc[deltemp])
#		    call pargstr(Memc[delconst])
#		delptr=delptr+10+1
		call strcat(Memc[delconst],Memc[delbuf],10)
	call printf("constant: %s delimiter: %s\n")
		call pargstr(Memc[delconst])
		call pargstr(Memc[delbuf])
		labptr=labptr+len+1
##		call printf("key: %s len: %d labptr: %d\n")
##		    call pargstr(temp)
##		    call pargi(len)
##		    call pargi(labptr)
##		call printf("value: %s\n")
##		    call pargstr(Memc[ip])
    	        call sprintf (Memc[valptr], 65, "%s\t")
		    call pargstr(Memc[ip])
		len=strlen(Memc[ip])
		valptr=valptr+len+1
	    }
	    else
    	    {
	        call sprintf (Memc[hdrptr], 65, "\t%s\n")
		    call pargstr(Memc[ip])
		len=strlen(Memc[ip])
		hdrptr=hdrptr+len+2
	    }
#	    len=strlen(Memc[labbuf])
#	    call printf ("labelbuf: %s, lablen: %d labptr: %d\n")
#		call pargstr(Memc[labbuf])
#		call pargi(len)
#		call pargi(labptr)
#	    len=strlen(Memc[valbuf])
#	    call printf ("valuebuf: %s, vallen: %d valptr: %d\n")
#		call pargstr(Memc[valbuf])
#		call pargi(len)
#		call pargi(valptr)
#	    call putline (out, "    ")
#	    call putline (out, Memc[lbuf])
	}

	call close (in)
	call sfree (sp)
end
