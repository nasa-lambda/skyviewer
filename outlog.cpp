/* ============================================================================
'outlog.cpp' defines functions used to write error logs.
===============================================================================
			Fetch header files.
*/
#include <stdio.h>
/* ----------------------------------------------------------------------------
'writeOutLog' writes text to the end of a file.

Arguments:
	fname - The name of the file.
	text  - The text to write.

Returned:
	Nothing.

Written by Michael R. Greason, ADNET, 08 November 2007.
---------------------------------------------------------------------------- */
void writeOutLog (const char *fname, const char *text)
{
	FILE *fptr;
	if ((fptr = fopen(fname, "a")) == NULL) return;
	fprintf(fptr, "%s\n", text);
	fclose(fptr);
}
