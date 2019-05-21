/* ============================================================================
The MapException class defines exceptions thrown by the skymap-related classes.

Written by   Michael R. Greason, ADNET, 27 December 2006.
Broken out of 'skymap.h'.  MRG, ADNET, 23 January 2007.
============================================================================ */
#include <stdio.h>
#include <string.h>
#include "map_exception.h"
/* ----------------------------------------------------------------------------
'MapException' is the class constructor.

Arguments:
	code   - The error code.
	status - A status value associated with the exception.  Defaults to 0.
	comm   - A comment associated with the error.

Returned:
	N/A.
---------------------------------------------------------------------------- */
MapException::MapException (ErrCode code, int status, const char *comm)
{
	code_    = code;
	status_  = status;
	comment_ = (comm == NULL) ? NULL : strdup(comm);
}
/* ----------------------------------------------------------------------------
'MapException' is the class constructor.

Arguments:
	code   - The error code.
	status - A status value associated with the exception.  Defaults to 0.
	

Returned:
	N/A.
---------------------------------------------------------------------------- */
MapException::~MapException (void)
{ 
	if (comment_ != NULL) delete [] comment_; 
}
/* ----------------------------------------------------------------------------
'Message' returns an error message associated with the error code.

Arguments:
	None.

Returned:
	The error message.  This is written to internal static storage---be sure
	to do something with it before calling this function again.
---------------------------------------------------------------------------- */
const char* MapException::Message (void) const
{
	static char msg[48];
	switch (code_)
	{
		case Memory:
			strcpy(msg, "Memory allocation error!");
			break;
		case Bounds:
			strcpy(msg, "Map index out of bounds!");
			break;
		case Undefined:
			strcpy(msg, "Undefined pixel property!");
			break;
		case InvalidType:
			strcpy(msg, "Invalid pixel type!");
			break;
		case FITSError:
	    	if (status_ != 0) sprintf(msg, "FITS I/O Error:  %d", status_);
			             else strcpy(msg, "FITS I/O Error");
			break;
		default:
			*msg = '\0';
			break;
	}
	return msg;
}
