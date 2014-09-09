/*-
 * Copyright (c) 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef lint
static const char copyright[] =
"@(#) Copyright (c) 1993\n\
	The Regents of the University of California.  All rights reserved.\n";
#endif /* not lint */

#ifndef lint
#if 0
static char sccsid[] = "@(#)jot.c	8.1 (Berkeley) 6/6/93";
#endif
static const char rcsid[] =
	"$Id: _jot.c,v 1.2 2012/11/10 12:37:20 john Exp $";
#endif /* not lint */

/*
 * jot - print sequential or random data
 *
 * Author:  John Kunze, Office of Comp. Affairs, UCB
 */


#include <ctype.h>
/* #include <err.h> */
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#undef  __P
#define __P(x) ()


#define	REPS_DEF	100
#define	BEGIN_DEF	1
#define	ENDER_DEF	100
#define	STEP_DEF	1

#define	isdefault(s)	(strcmp((s), "-") == 0)

double	begin;
double	ender;
double	s;
long	reps;
int	randomize;
int	infinity;
int	boring;
int	prec;
int	dox;
int	chardata;
int	nofinalnl;
char	*sepstring = "\n";
char	format[BUFSIZ];

void	getargs __P((int, char *[]));
void	getformat __P((void));
int		getprec __P((char *));
void	putdata __P((double, long));
static void usage __P((void));

int
main(argc, argv)
	int argc;
	char *argv[];
{
	double	xd, yd;
	long	id;
	register double	*x = &xd;
	register double	*y = &yd;
	register long	*i = &id;

	getargs(argc, argv);
	if (randomize) {
		*x = (ender - begin) * (ender > begin ? 1 : -1);

		if (s == -1.0)
		    srandom(time(NULL) * getpid());
		else
		    srandom((unsigned long) s);

		for (*i = 1; *i <= reps || infinity; (*i)++) {
		    *y = (double) random() / LONG_MAX;
		    putdata(*y * *x + begin, reps - *i);
		}
	}
	else
	    for (*i = 1, *x = begin; *i <= reps || infinity; (*i)++, *x += s)
		putdata(*x, reps - *i);

	if (!nofinalnl)
		putchar('\n');
	exit(0);
}

void
getargs(ac, av)
	int ac;
	char *av[];
{
	register unsigned int	mask = 0;
	register int		n = 0;

	double	SAOstrtod();
	char	*ptr;

	while (--ac && **++av == '-' && !isdefault(*av))
		switch ((*av)[1]) {
		case 'r':
			randomize = 1;
			break;
		case 'c':
			chardata = 1;
			break;
		case 'n':
			nofinalnl = 1;
			break;
		case 'b':
			boring = 1;
		case 'w':
			if ((*av)[2])
				strcpy(format, *av + 2);
			else if (!--ac)
				{ fprintf(stderr, "need context word after -w or -b"); exit(1); }
			else
				strcpy(format, *++av);
			break;
		case 's':
			if ((*av)[2])
				sepstring = *av + 2;
			else if (!--ac)
				{ fprintf(stderr, "need string after -s"); exit(1); }
			else
				sepstring = *++av;
			break;
		case 'p':
			if ((*av)[2])
				prec = atoi(*av + 2);
			else if (!--ac)
				{ fprintf(stderr, "need number after -p"); exit(1); }
			else
				prec = atoi(*++av);
			if (prec <= 0)
				{ fprintf(stderr, "bad precision value"); exit(1); }
			break;
		default:
			usage();
		}

	switch (ac) {	/* examine args right to left, falling thru cases */
	case 4:
		if (!isdefault(av[3])) {
			s = SAOstrtod(av[3], &ptr);
			if ( *ptr )
				{ fprintf(stderr, "bad s value: %s", av[3]); exit(1); }
			mask |= 01;
		}
	case 3:
		if (!isdefault(av[2])) {
			ender = SAOstrtod(av[2], &ptr);
			if ( *ptr )
				ender = av[2][strlen(av[2])-1];
			mask |= 02;
			if (!prec)
				n = getprec(av[2]);
		}
	case 2:
		if (!isdefault(av[1])) {
			begin = SAOstrtod(av[1], &ptr);
			if ( *ptr )
				begin = av[1][strlen(av[1])-1];
			mask |= 04;
			if (!prec)
				prec = getprec(av[1]);
			if (n > prec)		/* maximum precision */
				prec = n;
		}
	case 1:
		if (!isdefault(av[0])) {
			reps = SAOstrtod(av[0], &ptr);
			if ( *ptr )
				{ fprintf(stderr, "bad reps value: %s", av[0]); exit(1); }
			mask |= 010;
		}
		break;
	case 0:
		usage();
	default:
		{ fprintf(stderr, "too many arguments. What do you mean by %s?", av[4]); exit(1); }
	}
	getformat();
	while (mask)	/* 4 bit mask has 1's where last 4 args were given */
		switch (mask) {	/* fill in the 0's by default or computation */
		case 001:
			reps = REPS_DEF;
			mask = 011;
			break;
		case 002:
			reps = REPS_DEF;
			mask = 012;
			break;
		case 003:
			reps = REPS_DEF;
			mask = 013;
			break;
		case 004:
			reps = REPS_DEF;
			mask = 014;
			break;
		case 005:
			reps = REPS_DEF;
			mask = 015;
			break;
		case 006:
			reps = REPS_DEF;
			mask = 016;
			break;
		case 007:
			if (randomize) {
				reps = REPS_DEF;
				mask = 0;
				break;
			}
			if (s == 0.0) {
				reps = 0;
				mask = 0;
				break;
			}
			reps = (ender - begin + s) / s;
			if (reps <= 0)
				{ fprintf(stderr, "impossible stepsize"); exit(1); }
			mask = 0;
			break;
		case 010:
			begin = BEGIN_DEF;
			mask = 014;
			break;
		case 011:
			begin = BEGIN_DEF;
			mask = 015;
			break;
		case 012:
			s = (randomize ? -1.0 : STEP_DEF);
			mask = 013;
			break;
		case 013:
			if (randomize)
				begin = BEGIN_DEF;
			else if (reps == 0)
				{ fprintf(stderr, "must specify begin if reps == 0"); exit(1); }
			else
				begin = ender - reps * s + s;
			mask = 0;
			break;
		case 014:
			s = (randomize ? -1.0 : STEP_DEF);
			mask = 015;
			break;
		case 015:
			if (randomize)
				ender = ENDER_DEF;
			else
				ender = begin + reps * s - s;
			mask = 0;
			break;
		case 016:
			if (randomize)
				s = -1.0;
			else if (reps == 0)
				{ fprintf(stderr, "infinite sequences cannot be bounded"); exit(1); }
			else if (reps == 1)
				s = 0.0;
			else
				s = (ender - begin) / (reps - 1);
			mask = 0;
			break;
		case 017:		/* if reps given and implied, */
			if (!randomize && s != 0.0) {
				long t = (ender - begin + s) / s;
				if (t <= 0)
					{ fprintf(stderr, "impossible stepsize"); exit(1); }
				if (t < reps)		/* take lesser */
					reps = t;
			}
			mask = 0;
			break;
		default:
			{ fprintf(stderr, "bad mask"); exit(1); }
		}
	if (reps == 0)
		infinity = 1;
}

void
putdata(x, notlast)
	double x;
	long notlast;
{
	long		d = x;
	register long	*dp = &d;

	if (boring)				/* repeated word */
		printf("%s", format);
	else if (dox)				/* scalar */
		printf(format, *dp);
	else					/* real */
		printf(format, x);
	if (notlast != 0)
		fputs(sepstring, stdout);
}

static void
usage()
{
	fprintf(stderr, "%s\n%s\n",
	"usage: jot [-cnr] [-b word] [-w word] [-s string] [-p precision]",
	"           [reps [begin [end [s]]]]");
	exit(1);
}

int
getprec(s)
	char *s;
{
	register char	*p;
	register char	*q;

	for (p = s; *p; p++)
		if (*p == '.')
			break;
	if (!*p)
		return (0);
	for (q = ++p; *p; p++)
		if (!isdigit(*p))
			break;
	return (p - q);
}

void
getformat()
{
	register char	*p;

	if (boring)				/* no need to bother */
		return;
	for (p = format; *p; p++)		/* look for '%' */
		if (*p == '%' && *(p+1) != '%')	/* leave %% alone */
			break;
	if (!*p && !chardata)
		sprintf(p, "%%.%df", prec);
	else if (!*p && chardata) {
		strcpy(p, "%c");
		dox = 1;
	}
	else if (!*(p+1))
		strcat(format, "%");		/* cannot end in single '%' */
	else {
		while (!isalpha(*p))
			p++;
		switch (*p) {
		case 'f': case 'e': case 'g': case '%':
			break;
		case 's':
			{ fprintf(stderr, "cannot convert numeric data to strings"); exit(1); }
			break;
		/* case 'd': case 'o': case 'x': case 'D': case 'O': case 'X':
		case 'c': case 'u': */
		default:
			dox = 1;
			break;
		}
	}
}
