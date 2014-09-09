/* Access.c
 */
/* copyright 1991, 1993, 1995, 1999 John B. Roll jr.
 */


#include "xos.h"
#include "xfile.h"

char *strpbrk();
char *strstr();

static char     AccessName[1024 + 2];
static int      amparse();

char           *Access(name, mode, info)
    char           *name, *mode;
    FileInfo       *info;
{
    FileInfo        i;

    if (!info)
	info = &i;
    if (!Stat(name, info))
	return NULL;

    if (mode) {
	int             m = amparse(mode);

	if (getuid() == FileInfoUser((*info))) {
	    if (m & PERM_READ && !(FileInfoMode((*info)) & (PERM_READ << 6)))
		return NULL;
	    if (m & PERM_WRIT && !(FileInfoMode((*info)) & (PERM_WRIT << 6)))
		return NULL;
	    if (m & PERM_EXEC && !(FileInfoMode((*info)) & (PERM_EXEC << 6)))
		return NULL;
	} else if (getgid() == FileInfoGrop((*info))) {
	    if (m & PERM_READ && !(FileInfoMode((*info)) & (PERM_READ << 3)))
		return NULL;
	    if (m & PERM_WRIT && !(FileInfoMode((*info)) & (PERM_WRIT << 3)))
		return NULL;
	    if (m & PERM_EXEC && !(FileInfoMode((*info)) & (PERM_EXEC << 3)))
		return NULL;
	} else {
	    if (m & PERM_READ && !(FileInfoMode((*info)) & PERM_READ))
		return NULL;
	    if (m & PERM_WRIT && !(FileInfoMode((*info)) & PERM_WRIT))
		return NULL;
	    if (m & PERM_EXEC && !(FileInfoMode((*info)) & PERM_EXEC))
		return NULL;
	}
    }
    strcpy(AccessName, name);
    return name;
}


static int      amparse(mode)
    char           *mode;
{
    int             xmode = 0;

    xmode |= (strpbrk(mode, "r") != NULL ? PERM_READ : 0);
    xmode |= (strpbrk(mode, "w") != NULL ? PERM_WRIT : 0);
    xmode |= (strpbrk(mode, "x") != NULL ? PERM_EXEC : 0);
    xmode |= (strpbrk(mode, "f") != NULL ? 8 : 0);

    return xmode;
}

static char    *findpath(name, mode, path, info)
    char           *name, *mode, *path;
    FileInfo       *info;
{
    char            pathbuff[4096];
    char            namebuff[1024];
    char            backmode[10];

    char           *here, *found, *getenv(), *strtok();

    int             mark = 0;
    int             skip = strpbrk(mode, ">") != NULL;
    int             pick = strpbrk(mode, "<") != NULL;

    if (skip && pick)
	return NULL;

    if (!path || (name[0] == '.' && name[1] == '/') || name[0] == '/')
	return Access(name, mode, info);

    strcpy(pathbuff, path);
    path = pathbuff;

    if (here = strpbrk(pathbuff, " :;")) {
	mark = *here;
	*here++ = '\0';
    }
    while (path) {
	if (path[0] == '$') {
	    if (path = getenv(&path[1]))
		if (found = findpath(name, mode, path, info))
		    return found;
	} else {
	    if (!skip) {
		if (!strcmp(".", path))
		    path[0] = '\0';

		strcpy(namebuff, path);
		if (path[0] && path[strlen(path) - 1] != '/')
		    strcat(namebuff, "/");
		strcat(namebuff, name);

		if (found = Access(namebuff, mode, info))
		    return found;
	    }
	}

	if (mark == ';') {
	    if (skip) {
		skip = 0;
		/* Knock down the skip mode to select all directories in path
		 * after the first ";" */
		strcpy(backmode, mode);
		backmode[strlen(backmode) - 1] = '\0';
		mode = backmode;
	    }
	    if (pick)
		return NULL;
	}
	path = here;
	if (here && (here = strpbrk(here, " :;"))) {
	    mark = *here;
	    *here++ = '\0';
	}
    }

    return NULL;
}


char           *FindFile(name, mode, extn, path, info)
    char        *name, *mode, *extn, *path;
    FileInfo	*info;
{
    char            extnbuff[1024];
    char            namebuff[1024];
    char           *here, *found, *getenv();

    if (!extn)
	return findpath(name, mode, path, info);

    strcpy(extnbuff, extn);
    extn = extnbuff;

    if (here = strpbrk(extnbuff, " :;"))
	*here++ = '\0';

    while (extn) {
	if (extn[0] == '$') {
	    if (extn = getenv(&extn[1]))
		if (found = FindFile(name, mode, extn, path, info))
		    return found;
	} else {
	    char           *e = strstr(name, extn);

	    strcpy(namebuff, name);
	    if (!e || (e && *(e + strlen(extn))))
		strcat(namebuff, extn);

	    if ((found = findpath(namebuff, mode, path, info)))
		return found;

	}

	extn = here;
	if (here && (here = strpbrk(here, " :;")))
	    *here++ = '\0';
    }

    return NULL;
}
