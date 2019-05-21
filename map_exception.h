#ifndef MAP_EXCEPTION_HPP
#define MAP_EXCEPTION_HPP
/* ============================================================================
'map_exception.h' defines the exceptions thrown by this library.

Written by   Michael R. Greason, ADNET, 27 December 2006.
Broken out of 'skymap.h'.  MRG, ADNET, 23 January 2007.
============================================================================ */
/* ============================================================================
The MapException class defines exceptions thrown by the skymap-related classes.
============================================================================ */
class MapException
{
	public:
// The possible exception codes.
		enum ErrCode {
			None,			// No error.
			Memory, 		// Memory allocation error.
			Bounds, 		// Array out of bounds error.
			Undefined,		// Requested element undefined.
			InvalidType,	// Invalid map type requested.
			FITSError,		// A FITS I/O error.
			Other			// Other error.
		};
	protected:
		ErrCode code_;
		int     status_;
		char   *comment_;
    public:
		MapException (ErrCode code, int status = 0, const char *comm = 0);
		virtual ~MapException (void);
		
		virtual const char* Message (void) const;
		virtual const char* Comment (void) const { return comment_; }
};
#endif
