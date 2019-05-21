#ifndef HEALPIXMAP_HPP
#define HEALPIXMAP_HPP
/* ============================================================================
'healpixmap.h' extends the Skymap class to support HEALPIX maps.  The non-
inline methods are defined in 'healpixmap.cpp'.

Written by Michael R. Greason, ADNET, 29 December 2006.
============================================================================ */
#include <math.h>
#include "skymap.h"

class ControlDialog;
/* =============================================================================
The HealpixMap class defines a skymap that uses the HEALPIX pixelization scheme.
============================================================================= */
class HealpixMap : public Skymap
{
	public:
		enum PixOrder {
			Undefined,				// Hasn't been set.
			Nested, 				// Nested pixel ordering.
			Ring					// Ring pixel ordering.
		};

		// Healpix utilities.
		static unsigned int NSide2NPix (unsigned int ns);
		static unsigned int NPix2NSide (unsigned int np);
		static unsigned int Res2NSide  (unsigned int res);
		static unsigned int Res2NPix   (unsigned int res);
		static unsigned int NSide2Res  (unsigned int ns);
		static unsigned int NPix2Res   (unsigned int np);
	protected:
		PixOrder     ordering_;		// Pixel ordering scheme.
		unsigned int nside_;		// Map resolution parameter.

		// Functions to read/write the FITS headers.
		virtual void readFITSPrimaryHeader    (fitsfile *fptr);
		virtual void writeFITSPrimaryHeader   (fitsfile *fptr);
		virtual void readFITSExtensionHeader  (fitsfile *fptr);
		virtual void writeFITSExtensionHeader (fitsfile *fptr);

		unsigned int degrade_pixindex (unsigned int i, unsigned int nsi, unsigned int nso);
		void degrade_map (unsigned int ns);
		void upgrade_map (unsigned int ns);
	public:
		// Constructors and destructor.
		HealpixMap ();
		HealpixMap (unsigned int n_in, Type type_in, PixOrder ord = Undefined);
		virtual ~HealpixMap();

		void copy (HealpixMap &imap);

		// Return ordering and resolution.
		unsigned int nside      () const { return nside_; }
		PixOrder     pixordenum () const { return ordering_; }
		unsigned int pixorder   () const { return ordering_; }
		const char*  ordering   () const;
		
		// Pixel coordinate conversions.
		void pixel2vector (long pix, double *vector);
		void vector2pixel (double *vector, long &pix);

		void pixel2angles (long pix, double &theta, double &phi, int deg = 0);
		void angles2pixel (double theta, double phi, long &pix, int deg = 0);
		
		long pix2ordering (long ipix, PixOrder dord);

		// Resize.
		void resize (unsigned int ns);
		
		// Copy operator.
		HealpixMap& operator= (HealpixMap &imap);

		// Pixel access.
		BasePixel& getPixel (double theta, double phi, int deg = 0);
		BasePixel& getPixel (double *vector);

		// FITS I/O.
		/*
		virtual void readFITS (const char* filename, fileProgress *progwin = NULL);
		virtual void readFITS (std::string filename, fileProgress *progwin = NULL);
		virtual void readFITS (QString filename, fileProgress *progwin = NULL);
		*/
		// FITS I/O.
		virtual void readFITS (const char* filename, ControlDialog *progwin = NULL);
		virtual void readFITS (std::string filename, ControlDialog *progwin = NULL);
		virtual void readFITS (QString filename, ControlDialog *progwin = NULL);
};
/* ----------------------------------------------------------------------------
'NSide2NPix' computes the number of pixels from NSide.

Static function.

Arguments:
	ns - NSide.

Returned:
	The number of pixels.
---------------------------------------------------------------------------- */
inline unsigned int HealpixMap::NSide2NPix (unsigned int ns)
{
	return 12 * ns * ns;
}
/* ----------------------------------------------------------------------------
'NPix2NSide' computes the NSide parameter from a number of pixels.  It assumes
that the math will work out evenly.

Static function.

Arguments:
	np - The number of pixels.

Returned:
	NSide.
---------------------------------------------------------------------------- */
inline unsigned int HealpixMap::NPix2NSide (unsigned int np)
{
	return (unsigned int) (sqrt(double(np) / 12.0) + 0.4);
}
/* ----------------------------------------------------------------------------
'Res2NSide' computes the number of pixels from NSide.

Static function.

Arguments:
	res - The map resolution.

Returned:
	NSide.
---------------------------------------------------------------------------- */
inline unsigned int HealpixMap::Res2NSide (unsigned int res)
{
	unsigned int i = 1;
	return i << res;
}
/* ----------------------------------------------------------------------------
'Res2NPix' computes the NSide parameter from a number of pixels.  It assumes
that the math will work out evenly.

Static function.

Arguments:
	res - The map resolution.

Returned:
	The number of pixels.
---------------------------------------------------------------------------- */
inline unsigned int HealpixMap::Res2NPix (unsigned int res)
{
	return NSide2NPix(Res2NSide(res));
}
/* ----------------------------------------------------------------------------
'NSide2Res' computes the resolution from NSide.

Static function.

Arguments:
	ns - NSide.

Returned:
	The map resolution.
---------------------------------------------------------------------------- */
inline unsigned int HealpixMap::NSide2Res (unsigned int ns)
{
	return (unsigned int) (0.4 + (log(double(ns)) / log(2.0)));
}
/* ----------------------------------------------------------------------------
'NPix2Res' computes the resolution from the number of pixels.

Static function.

Arguments:
	np - The number of pixels.

Returned:
	The map resolution.
---------------------------------------------------------------------------- */
inline unsigned int HealpixMap::NPix2Res (unsigned int np)
{
	return NSide2Res(NPix2NSide(np));
}
/* ----------------------------------------------------------------------------
'operator=' copies another map into this one using the assignment operator.

Arguments:
	imap - The source map.

Returned:
	A reference to this map.
---------------------------------------------------------------------------- */
inline HealpixMap& HealpixMap::operator= (HealpixMap &imap)
{
	copy(imap);
	return *this;
}
#endif


