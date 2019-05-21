/* ============================================================================
'healpixmap.cpp' extends the Skymap class to support HEALPIX maps.  The class
is defined in 'healpixmap.h'.

Written by Michael R. Greason, ADNET, 29 December 2006.
============================================================================ */
/*
			Fetch header files.  Wrap the call to chealpix.h in an 
			'extern "C"' construct to ensure that name mangling is turned off;
			the Gnu C++ compiler wouldn't link these functions in correctly
			without it.
*/
#include <iostream>
#include <string.h>
#include <stdio.h>
extern "C"
{
#include <chealpix.h>
};
#include "healpixmap.h"
#include "controldialog.h"
#include "str_funcs.h"

using namespace std;
/*
			Constants.
*/
static double pi      = 3.141592653589793;
static double deg2rad = pi / 180.0;
/* =============================================================================
The HealpixMap class defines a skymap that uses the HEALPIX pixelization scheme.
============================================================================= */
/* ----------------------------------------------------------------------------
'readFITSPrimaryHeader' reads information from the primary FITS header.

This routine assumes that the current HDU is the primary HDU.

An exception is thrown in the event of a FITS error.

Arguments:
	fitsfile - The handle to the currently open FITS file.

Returned:
	Nothing.
---------------------------------------------------------------------------- */
void HealpixMap::readFITSPrimaryHeader (fitsfile* fptr)
{
	char tmp[24], comm[80];
	int  status = 0;
	Skymap::readFITSPrimaryHeader(fptr);
	if (fits_read_keyword(fptr, "ORDERING", tmp, comm, &status) != 0)
		return;
	fits_str_cull(tmp);
	if (strncmp(tmp, "RING", 4) == 0) ordering_ = Ring;
	                             else ordering_ = Nested;
	return;
}
/* ----------------------------------------------------------------------------
'writeFITSPrimaryHeader' writes information to the primary FITS header.

This routine assumes that the current HDU is the primary HDU.

An exception is thrown in the event of a FITS error or if the ordering
scheme is undefined.

Arguments:
	fitsfile - The handle to the currently open FITS file.

Returned:
	Nothing.
---------------------------------------------------------------------------- */
void HealpixMap::writeFITSPrimaryHeader (fitsfile* fptr)
{
	char         stmp[80], comm[80];
	unsigned int itmp;
	int          status = 0;

	if (ordering_ == Undefined) throw MapException(MapException::Undefined);

	Skymap::writeFITSPrimaryHeader(fptr);

	strcpy(comm, "Pixel algorithm");
	strcpy(stmp, "HEALPIX");
	fits_write_key(fptr, TSTRING, "PIXTYPE", stmp, comm, &status);

	strcpy(comm, "Ordering scheme");
	strcpy(stmp, ordering());
	fits_write_key(fptr, TSTRING, "ORDERING", stmp, comm, &status);

	strcpy(comm, "Resolution index");
	itmp = NSide2Res(nside());
	fits_write_key(fptr, TUINT, "RESOLUTN", &itmp, comm, &status);

	strcpy(comm, "Resolution parameter");
	itmp = nside();
	fits_write_key(fptr, TUINT, "NSIDE", &itmp, comm, &status);

	strcpy(comm, "First pixel index (0 based)");
	itmp = 0;
	fits_write_key(fptr, TUINT, "FIRSTPIX", &itmp, comm, &status);

	strcpy(comm, "Last pixel index (0 based)");
	itmp = size() - 1;
	fits_write_key(fptr, TUINT, "LASTPIX", &itmp, comm, &status);

	return;
}
/* ----------------------------------------------------------------------------
'readFITSExtensionHeader' reads information from the extension FITS header.

This routine assumes that the current HDU is the correct extension HDU.

An exception is thrown in the event of a FITS error.

Arguments:
	fitsfile - The handle to the currently open FITS file.

Returned:
	Nothing.
---------------------------------------------------------------------------- */
void HealpixMap::readFITSExtensionHeader (fitsfile* fptr)
{
	char tmp[24], comm[80];
	int  status = 0;
	Skymap::readFITSExtensionHeader(fptr);
	if (fits_read_keyword(fptr, "ORDERING", tmp, comm, &status) != 0)
		return;
	fits_str_cull(tmp);
	if (strncmp(tmp, "RING", 4) == 0) ordering_ = Ring;
	                             else ordering_ = Nested;
}
/* ----------------------------------------------------------------------------
'writeFITSExtensionHeader' writes information to the primary FITS header.

This routine assumes that the current HDU is the correct extension HDU.

An exception is thrown in the event of a FITS error.

Arguments:
	fitsfile - The handle to the currently open FITS file.

Returned:
	Nothing.
---------------------------------------------------------------------------- */
void HealpixMap::writeFITSExtensionHeader (fitsfile* fptr)
{
	char         stmp[80], comm[80];
	unsigned int itmp;
	int          status = 0;

	Skymap::writeFITSExtensionHeader(fptr);

	strcpy(comm, "Pixel algorithm");
	strcpy(stmp, "HEALPIX");
	fits_write_key(fptr, TSTRING, "PIXTYPE", stmp, comm, &status);

	strcpy(comm, "Ordering scheme");
	strcpy(stmp, ordering());
	fits_write_key(fptr, TSTRING, "ORDERING", stmp, comm, &status);

	strcpy(comm, "Resolution index");
	itmp = NSide2Res(nside());
	fits_write_key(fptr, TUINT, "RESOLUTN", &itmp, comm, &status);

	strcpy(comm, "Resolution parameter");
	itmp = nside();
	fits_write_key(fptr, TUINT, "NSIDE", &itmp, comm, &status);

	strcpy(comm, "First pixel index (0 based)");
	itmp = 0;
	fits_write_key(fptr, TUINT, "FIRSTPIX", &itmp, comm, &status);

	strcpy(comm, "Last pixel index (0 based)");
	itmp = size() - 1;
	fits_write_key(fptr, TUINT, "LASTPIX", &itmp, comm, &status);

	return;
}
/* ----------------------------------------------------------------------------
'degrade_pixindex' computes a degraded pixel index for the current map type.
This calculation should always go from the higher to the lower resolution
direction because this results in a unique calculation; therefore there is
no 'upgrade_pixindex' function.

If an error occurs, a MapException will be thrown.

Arguments:
	i   - The pixel index to convert.
	nsi - The nside of the [high resolution] map corresponding to i.
	nso - The nside of the [low resolution] map to be computed to.

Returned:
	The converted pixel index.
---------------------------------------------------------------------------- */
unsigned int HealpixMap::degrade_pixindex (unsigned int i, unsigned int nsi,
	unsigned int nso)
{
	long ti, tj;
	double scl;
	if (ordering_ == Undefined) throw MapException(MapException::Undefined);
	if (ordering_ == Ring) ::ring2nest(long(nsi), long(i), &ti);
	                  else ti = long(i);
	scl = double(nso) / double(nsi);
	tj = long(double(ti) * scl * scl);
	if (ordering_ == Ring) ::nest2ring(long(nso), tj, &tj);
	return ((unsigned int) tj);
}
/* ----------------------------------------------------------------------------
'degrade_map' reduces the size of the map.

If an error occurs, a MapException will be thrown.

Arguments:
	ns - The new nsize.

Returned:
	Nothing.
---------------------------------------------------------------------------- */
void HealpixMap::degrade_map (unsigned int ns) 
{
	BasePixel *arr;
	unsigned int i, j;
	unsigned int nnew = NSide2NPix(ns);
	unsigned int *cnt;
/*
			Allocate space for the new array.
*/
	switch (type())
	{
		case TPix:
			arr = new TPixel[nnew];
			if (arr == NULL) throw MapException(MapException::Memory);
			break;
		case PPix:
			arr = new TPPixel[nnew];
			if (arr == NULL) throw MapException(MapException::Memory);
			break;
		case TnobsPix:
			arr = new TnobsPixel[nnew];
			if (arr == NULL) throw MapException(MapException::Memory);
			break;
		case TPnobsPix:
			arr = new TPnobsPixel[nnew];
			if (arr == NULL) throw MapException(MapException::Memory);
			break;
		default:
			throw MapException(MapException::InvalidType);
			break;
	}
	if ((cnt = new unsigned int[nnew]) == NULL)
		throw MapException(MapException::InvalidType);
	for (i = 0; i < nnew; i++) cnt[i] = 0;
/*
			Accumulate data into the new map.
*/
	for (j = 0; j < size(); j++)
	{
		i = degrade_pixindex(j, nside_, ns);
		if (i >= nnew) break;
		cnt[i] += 1;
		try { arr[i].T()    += (*this)[j].T();    } catch (MapException &exc) { ; }
		try { arr[i].Nobs() += (*this)[j].Nobs(); } catch (MapException &exc) { ; }
		try { arr[i].Q()    += (*this)[j].Q();    } catch (MapException &exc) { ; }
		try { arr[i].U()    += (*this)[j].U();    } catch (MapException &exc) { ; }
	}
/*
			Compute the mean temperature measurements.
*/
	for (i = 0; i < nnew; i++)
	{
		try { arr[i].T() /= double(cnt[i]); } catch (MapException &exc) { ; }
		try { arr[i].Q() /= double(cnt[i]); } catch (MapException &exc) { ; }
		try { arr[i].U() /= double(cnt[i]); } catch (MapException &exc) { ; }
	}
/*
			Destroy the old map array, assign the new one to the map,
			and update bookkeeping parameters.
*/
	delete [] cnt;
	switch (type())
	{
		case TPix:
			if (tpix != 0) delete [] tpix;
			tpix = (TPixel*) arr;
			break;
		case PPix:
			if (tppix != 0) delete [] tppix;
			tppix = (TPPixel*) arr;
			break;
		case TnobsPix:
			if (tnobspix != 0) delete [] tnobspix;
			tnobspix = (TnobsPixel*) arr;
			break;
		case TPnobsPix:
			if (tpnobspix != 0) delete [] tpnobspix;
			tpnobspix = (TPnobsPixel*) arr;
			break;
		default:
			throw MapException(MapException::InvalidType);
			break;
	}
	nside_ = ns;
	n_     = nnew;
	return;
}
/* ----------------------------------------------------------------------------
'upgrade_map' increases the size of the map.

If an error occurs, a MapException will be thrown.

Arguments:
	ns - The new nsize.

Returned:
	Nothing.
---------------------------------------------------------------------------- */
void HealpixMap::upgrade_map (unsigned int ns) 
{
	BasePixel *arr;
	unsigned int i, j;
	unsigned int nnew = NSide2NPix(ns);
	double       scl;
/*
			Allocate space for the new array.
*/
	switch (type())
	{
		case TPix:
			arr = new TPixel[nnew];
			if (arr == NULL) throw MapException(MapException::Memory);
			break;
		case PPix:
			arr = new TPPixel[nnew];
			if (arr == NULL) throw MapException(MapException::Memory);
			break;
		case TnobsPix:
			arr = new TnobsPixel[nnew];
			if (arr == NULL) throw MapException(MapException::Memory);
			break;
		case TPnobsPix:
			arr = new TPnobsPixel[nnew];
			if (arr == NULL) throw MapException(MapException::Memory);
			break;
		default:
			throw MapException(MapException::InvalidType);
			break;
	}
/*
			Accumulate data into the new map.
*/
	scl = double(nnew) / double(size());
	for (j = 0; j < nnew; j++)
	{
		i = degrade_pixindex(j, ns, nside_);
		i  = (unsigned int)(double(j) / scl);
		if (i >= size()) break;
		try { arr[j].T()	= (*this)[i].T();	 } catch (MapException &exc) { ; }
		try { arr[j].Nobs() = (*this)[i].Nobs(); } catch (MapException &exc) { ; }
		try { arr[j].Q()	= (*this)[i].Q();	 } catch (MapException &exc) { ; }
		try { arr[j].U()	= (*this)[i].U();	 } catch (MapException &exc) { ; }
	}
/*
			Destroy the old map array, assign the new one to the map,
			and update bookkeeping parameters.
*/
	switch (type())
	{
		case TPix:
			if (tpix != 0) delete [] tpix;
			tpix = (TPixel*) arr;
			break;
		case PPix:
			if (tppix != 0) delete [] tppix;
			tppix = (TPPixel*) arr;
			break;
		case TnobsPix:
			if (tnobspix != 0) delete [] tnobspix;
			tnobspix = (TnobsPixel*) arr;
			break;
		case TPnobsPix:
			if (tpnobspix != 0) delete [] tpnobspix;
			tpnobspix = (TPnobsPixel*) arr;
			break;
		default:
			throw MapException(MapException::InvalidType);
			break;
	}
	nside_ = ns;
	n_     = nnew;
	return;
}
/* ----------------------------------------------------------------------------
'HealpixMap' creates an empty instance of the class.

Arguments:
	None.

Returned:
	N/A.
---------------------------------------------------------------------------- */
HealpixMap::HealpixMap () : Skymap()
{
	nside_    = 0;
	ordering_ = Undefined;
}
/* ----------------------------------------------------------------------------
'HealpixMap' creates a blank map given descriptive information.

Arguments:
	n_in    - The number of pixels in the map.
	type_in - The type of map.
	ord     - The pixel ordering.  Defaults to Nested.

Returned:
	N/A.
---------------------------------------------------------------------------- */
HealpixMap::HealpixMap (unsigned int n_in, Type type_in, PixOrder ord) : 
	Skymap(n_in, type_in)
{
	nside_    = NPix2NSide(n_in);
	ordering_ = ord;
}
/* ----------------------------------------------------------------------------
'~HealpixMap' is the class destructor.

Arguments:
	None.

Returned:
	N/A.
---------------------------------------------------------------------------- */
HealpixMap::~HealpixMap ()
{
	;
}
/* ----------------------------------------------------------------------------
'copy' copies the contents of another map into this one.

Arguments:
	imap - The source map.

Returned:
	Nothing.
---------------------------------------------------------------------------- */
void HealpixMap::copy(HealpixMap &imap)
{
	Skymap::copy(imap);
	nside_    = imap.nside_;
	ordering_ = imap.ordering_;
	
}
/* ----------------------------------------------------------------------------
'ordering' reports the ordering scheme as a descriptive string.  Static
internal storage is used, so use the string before calling this routine
again.

Arguments:
	None.

Returned:
	The string:  NESTED or RING.
---------------------------------------------------------------------------- */
const char* HealpixMap::ordering () const
{
	static char str[12];
	if (ordering_ == Ring  ) strcpy(str, "RING");
	if (ordering_ == Nested) strcpy(str, "NESTED");
	                    else strcpy(str, "UNDEFINED");
	return str;
}
/* ----------------------------------------------------------------------------
'pixel2vector' converts a pixel number into a cartesian pointing vector.

If the pixel ordering of the map is undefined, then an Undefined MapException
is thrown.

Arguments:
	pix    - The pixel number.
	vector - The three-element cartesian pointing vector.

Returned:
	Nothing.
---------------------------------------------------------------------------- */
void HealpixMap::pixel2vector (long pix, double *vector)
{
	switch (ordering_)
	{
		case Nested:
			::pix2vec_nest(long(nside_), pix, vector);
			break;
		case Ring:
			::pix2vec_ring(long(nside_), pix, vector);
			break;
		default:
			throw MapException(MapException::Undefined);
			break;
	}
	return;
}
/* ----------------------------------------------------------------------------
'vector2pixel' converts a cartesian pointing vector into a pixel number.

If the pixel ordering of the map is undefined, then an Undefined MapException
is thrown.

Arguments:
	vector - The three-element cartesian pointing vector.
	pix    - The pixel number.

Returned:
	Nothing.
---------------------------------------------------------------------------- */
void HealpixMap::vector2pixel (double *vector, long &pix)
{
	pix = 0;
	switch (ordering_)
	{
		case Nested:
			::vec2pix_nest(long(nside_), vector, &pix);
			break;
		case Ring:
			::vec2pix_ring(long(nside_), vector, &pix);
			break;
		default:
			throw MapException(MapException::Undefined);
			break;
	}
	return;
}
/* ----------------------------------------------------------------------------
'pixel2angles' converts a pixel number into position angles on the sphere.

If the pixel ordering of the map is undefined, then an Undefined MapException
is thrown.

Arguments:
	pix   - The pixel number.
	theta - The colatitude in radians measured southward from the north pole
			(0--PI).
	phi   - The longitude in radians measured eastward from the X-axis
			(0--2PI).
	deg   - If nonzero return the angles in degrees instead of radians.
	     	Defaults to 0.

Returned:
	Nothing.
---------------------------------------------------------------------------- */
void HealpixMap::pixel2angles (long pix, double &theta, double &phi, int deg)
{
	theta = phi = 0.0;
	switch (ordering_)
	{
		case Nested:
			::pix2ang_nest(long(nside_), pix, &theta, &phi);
			break;
		case Ring:
			::pix2ang_ring(long(nside_), pix, &theta, &phi);
			break;
		default:
			throw MapException(MapException::Undefined);
			break;
	}
	if (deg != 0)
	{
		theta /= deg2rad;
		phi   /= deg2rad;
	}
	return;
}
/* ----------------------------------------------------------------------------
'angles2pixel' converts position angles on the sphere into a pixel number.

If the pixel ordering of the map is undefined, then an Undefined MapException
is thrown.

Arguments:
	theta - The colatitude in radians measured southward from the north pole
			(0--PI).
	phi   - The longitude in radians measured eastward from the X-axis
			(0--2PI).
	pix   - The pixel number.
	deg   - If nonzero the angles are in degrees instead of radians.
	     	Defaults to 0.

Returned:
	Nothing.
---------------------------------------------------------------------------- */
void HealpixMap::angles2pixel (double theta, double phi, long &pix, int deg)
{
	pix = 0;
	if (deg != 0)
	{
		theta *= deg2rad;
		phi   *= deg2rad;
	}
	switch (ordering_)
	{
		case Nested:
			::ang2pix_nest(long(nside_), theta, phi, &pix);
			break;
		case Ring:
			::ang2pix_ring(long(nside_), theta, phi, &pix);
			break;
		default:
			throw MapException(MapException::Undefined);
			break;
	}
	return;
}
/* ----------------------------------------------------------------------------
'pix2ordering' converts a pixel number for this map into a specified ordering 
scheme.

If either the pixel ordering of the map or the requested pixel ordering scheme
is undefined, then an Undefined MapException is thrown.

Arguments:
	ipix - The pixel number in this map's ordering scheme.
	dord - The desired ordering scheme.

Returned:
	The converted pixel number.
---------------------------------------------------------------------------- */
long HealpixMap::pix2ordering (long ipix, PixOrder dord)
{
	long opix = 0;
	if ((dord == Undefined) || (ordering_ == Undefined))
		throw MapException(MapException::Undefined);
	if (dord == ordering_) return ipix;
	switch (ordering_)
	{
		case Nested:
			::nest2ring(long(nside_), ipix, &opix);
			break;
		case Ring:
			::ring2nest(long(nside_), ipix, &opix);
			break;
		default:
			throw MapException(MapException::Undefined);
			break;
	}
	return opix;
}
/* ----------------------------------------------------------------------------
'resize' scales the map to a new nsize.

If an error occurs, a MapException will be thrown.

Arguments:
	ns - The new nsize.

Returned:
	Nothing.
---------------------------------------------------------------------------- */
void HealpixMap::resize (unsigned int ns) 
{
	if (ordering_ == Undefined) throw MapException(MapException::Undefined);
	if (ns == nside_) return;
	if (ns > nside_) upgrade_map(ns);
	            else degrade_map(ns);
	return;
}
/* ----------------------------------------------------------------------------
'pix2ordering' converts a pixel number for this map into a specified ordering 
scheme.

If either the pixel ordering of the map or the requested pixel ordering scheme
is undefined, then an Undefined MapException is thrown.

Arguments:
	ipix - The pixel number in this map's ordering scheme.
	dord - The desired ordering scheme.

Returned:
	The converted pixel number.
---------------------------------------------------------------------------- */
/* ----------------------------------------------------------------------------
'getPixel' retrieves a pixel based upon its position angles.

If the pixel ordering of the map is undefined, then an Undefined MapException
is thrown or if the resultant pixel number is out of bounds.

Arguments:
	theta - The colatitude in radians measured southward from the north pole
			(0--PI).
	phi   - The longitude in radians measured eastward from the X-axis
			(0--2PI).
	deg   - If nonzero the angles are in degrees instead of radians.
	     	Defaults to 0.

Returned:
	The pixel is returned.
---------------------------------------------------------------------------- */
BasePixel& HealpixMap::getPixel (double theta, double phi, int deg)
{
	long pix;
	angles2pixel(theta, phi, pix, deg);
	return (*this)[pix];
}
/* ----------------------------------------------------------------------------
'getPixel' retrieves a pixel based upon its cartesian pointing vector.

If the pixel ordering of the map is undefined, then an Undefined MapException
is thrown or if the resultant pixel number is out of bounds.

Arguments:
	vector - The three-element cartesian pointing vector.

Returned:
	The pixel is returned.
---------------------------------------------------------------------------- */
BasePixel& HealpixMap::getPixel (double *vector)
{
	long pix;
	vector2pixel(vector, pix);
	return (*this)[pix];
}
/* ----------------------------------------------------------------------------
'readFITS' fills the map from a FITS file. An exception is thrown in the event
of a FITS error or if the appropriate FITS table cannot be found.  There must
be a temperature column!

Arguments:
	filename - The name of the FITS file.
	progwin  - A pointer to the file load progress window.

Returned:
	Nothing.
---------------------------------------------------------------------------- */
/*
void HealpixMap::readFITS(const char* filename, fileProgress *progwin)
{
	Skymap::readFITS(filename, progwin);
	nside_ = NPix2NSide(size());
	if (progwin != NULL) progwin->loadNSide(nside_, ordering_);
}
void HealpixMap::readFITS(string filename, fileProgress *progwin)
{
	readFITS(filename.c_str(), progwin);
	if (progwin != NULL) progwin->loadNSide(nside_, ordering_);
}
void HealpixMap::readFITS(QString filename, fileProgress *progwin)
{
	readFITS(filename.toStdString(), progwin);
	if (progwin != NULL) progwin->loadNSide(nside_, ordering_);
}
*/
/* ----------------------------------------------------------------------------
'readFITS' fills the map from a FITS file. An exception is thrown in the event
of a FITS error or if the appropriate FITS table cannot be found.  There must
be a temperature column!

Arguments:
	filename - The name of the FITS file.
	progwin  - A pointer to the file load progress window.

Returned:
	Nothing.
---------------------------------------------------------------------------- */
void HealpixMap::readFITS(const char* filename, ControlDialog *progwin)
{
	Skymap::readFITS(filename, progwin);
	nside_ = NPix2NSide(size());
	if (progwin != NULL) progwin->loadNSide(nside_, ordering_);
}
void HealpixMap::readFITS(string filename, ControlDialog *progwin)
{
	readFITS(filename.c_str(), progwin);
	if (progwin != NULL) progwin->loadNSide(nside_, ordering_);
}
void HealpixMap::readFITS(QString filename, ControlDialog *progwin)
{
	readFITS(filename.toStdString(), progwin);
	if (progwin != NULL) progwin->loadNSide(nside_, ordering_);
}
