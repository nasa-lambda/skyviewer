/* ============================================================================
"str_funcs.cpp" defines various string manipulation functions:
	fits_str_cull - Removes leading and trailing quotes from a FITS
	                header card string.
============================================================================ */
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
/* ----------------------------------------------------------------------------
'fits_str_cull' Removes leading and trailing quotes from a FITS header
card string.  It also removes leading and trailing white space from the string.

Arguments:
	str - The string to process.  It IS modified by this function.

Returned:
	The processed string.

Written by Michael R. Greason, ADNET, 08 January 2007.
---------------------------------------------------------------------------- */
char *fits_str_cull (char *str)
{
	char *p, *d;
	int  flg, n;
/*
			Trivial case---NULL or empty string.
*/
	if ((str == NULL) || ((n = strlen(str)) <= 0)) return str;
/*
			Search for a leading quote.
*/
	flg = 0;
	p = str;
	while (*p != '\0')
	{
		if ((*p == '\"') || (*p == '\''))
		{
			flg = 1;
			*p  = ' ';
			break;
		}
		++p;
	}
/*
			Remove trailing white space.  At the same time and if a
			leading quote was found, remove a trailing quote.
*/
	p = str + n - 1;
	while (--n >= 0)
	{
		if (isspace(*p) != 0)
		{
			*(p--) = '\0';
			continue;
		}
		if ((flg != 0) && ((*p == '\"') || (*p == '\'')))
		{
			*(p--) = '\0';
			continue;
		}
		break;
	}
/*
			Remove leading white space.
*/
	n = 0;
	d = p = str;
	while ((*p != '\0') && (isspace(*p) != 0)) ++p;
	if (*p == '\0') return str;
	while (*p != '\0') *d++ = *p++;
	*d = '\0';

	return str;
}
