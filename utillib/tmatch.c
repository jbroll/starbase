#include <xos.h>
#include <stdio.h>
#include <string.h>

char *strrev(string)
	char *string;
{
	char *here = &string[strlen(string) - 1];

    while ( here > string ) {
	int	ch = *here;

	  *here-- = *string;
	*string++ = ch;
    }

    return string;
}

char *treplace(string, pattern, replace, result, maxx)
	char	*string;
	char	*pattern;
	char	*replace;
	char	*result;
	int	 maxx;
{
	char	*here;
	int	 prefix;
	int	 suffix;

	char	buffer[512];

	strncpy(buffer, pattern, 512);
	pattern = buffer;

    if ( !(here = strchr(replace, '?')) ) return strncpy(result, replace, maxx); 

    *result = '\0';

    strncat(result, replace, Min(here - replace, maxx));

    prefix = strcspn(pattern, "*?[");
    strrev(pattern);
    suffix = strcspn(pattern, "*?]");
    
    strncat(result, &string[prefix], Min(strlen(string) - (prefix + suffix), maxx));

    return strncat(result, ++here, maxx);
}

