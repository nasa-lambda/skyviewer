/* ============================================================================
'skymap.cpp' defines the pixel and skymap class methods.  The classes are
defined in 'skymap.h'.

Written by Nicholas Phillips, December 2006
Adapted for WMAP.  FITS and copy operator added.  Michael R. Greason, ADNET,
	27 December 2006.
readFITS modified to support multiple values in a single table cell.
    MRG, ADNET, 13 January 2009.
============================================================================ */
#include <math.h>
#include <stdio.h>
#include "skymap.h"
#include "controldialog.h"
#include "enums.h"

#ifndef HEALPIX_NULLVAL
#define HEALPIX_NULLVAL (-1.6375e30)
#endif

using namespace std;
/*
			Constants.
*/
char  ICOLNAME[]  = "TEMPERATURE";
char  ICOLNAMEA[] = "I_STOKES";
char  ICOLNAMEB[] = "I_Stokes";
char  QCOLNAME[]  = "Q_POLARISATION";
char  QCOLNAMEA[] = "QPOLARISATION";
char  QCOLNAMEB[] = "Q_POLARIZATION";
char  QCOLNAMEC[] = "QPOLARIZATION";
char  QCOLNAMED[] = "Q_STOKES";
char  QCOLNAMEE[] = "Q_Stokes";
char  UCOLNAME[]  = "U_POLARISATION";
char  UCOLNAMEA[] = "UPOLARISATION";
char  UCOLNAMEB[] = "U_POLARIZATION";
char  UCOLNAMEC[] = "UPOLARIZATION";
char  UCOLNAMED[] = "U_STOKES";
char  UCOLNAMEE[] = "U_Stokes";
char  NCOLNAME[]  = "N_OBS";
char  NCOLNAMEA[] = "HITS";

char  DEFTABLE[]  = "Sky Maps";
char  DEFTUNIT[]  = "mK";
char  DEFNUNIT[]  = "counts";
char  DEFFORM[]   = "E";

const int maxcols = 5;
/* ============================================================================
The Skymap class defines a collection of pixels to represent a sky map.
This version allows for dynamically selecting how much information is stored 
for each pixel. Can be as simple as just the temperature or up to temperature, 
polarization and number of observations.
============================================================================ */
BasePixel Skymap::basedummy;
/* ----------------------------------------------------------------------------
'Skymap' is the class constructor that defines an empty map.

Arguments:
	None.

Returned:
	N/A.
---------------------------------------------------------------------------- */
Skymap::Skymap()
{
	init();
}
/* ----------------------------------------------------------------------------
'Skymap' is the class constructor that defines a blank map of a given size and
type.

Arguments:
	n_in    - The number of pixels in the map.
	type_in - The type of map.

Returned:
	N/A.
---------------------------------------------------------------------------- */
Skymap::Skymap(unsigned int n_in, Type type_in)
{
	init();
	set(n_in, type_in );
}
/* ----------------------------------------------------------------------------
'~Skymap' is the class destructor.

Arguments:
	None.

Returned:
	N/A.
---------------------------------------------------------------------------- */
Skymap::~Skymap()
{
	freeMemory();
}
/* ----------------------------------------------------------------------------
'returnnone' throws an exception indicating that an empty map was accessed.

Arguments:
	i - The index into the map.

Returned:
	A dummy pixel is returned..
---------------------------------------------------------------------------- */
BasePixel& Skymap::returnnone(unsigned int /*i*/)
{
	throw MapException(MapException::InvalidType);
	return basedummy;
}
/* ----------------------------------------------------------------------------
'init' initializes an empty map.

Arguments:
	None.

Returned:
	Nothing.
---------------------------------------------------------------------------- */
void Skymap::init()
{
	type_     = none;
	n_        = 0;
	tpix      = 0;
	tppix     = 0;
	tnobspix  = 0;
	tpnobspix = 0;
	returnfct = &Skymap::returnnone;

	minpix.clear();
	maxpix.clear();
	avgpix.clear();
	stdpix.clear();

	return;
}
/* ----------------------------------------------------------------------------
'allocPixMemory' allocates the storage for a blank map.  If an invalid map 
type is supplied an exception is thrown.

Arguments:
	n_in    - The number of pixels in the map.
	type_in - The type of map.

Returned:
	Nothing.
---------------------------------------------------------------------------- */
void Skymap::allocPixMemory(unsigned int n_in, Type type_in)
{
	if( n_in == n() && type_in == type() )
		return;
	if( n_in == 0 || type_in == none )
		return;
		
	type_ = type_in;
	if (tpix       ) { delete[] tpix;       tpix       = 0; }
	if (tppix      ) { delete[] tppix;      tppix      = 0; }
	if (tnobspix   ) { delete[] tnobspix;   tnobspix   = 0; }
	if (tpnobspix  ) { delete[] tpnobspix;  tpnobspix  = 0; }
	n_ = n_in;
	switch( type() ){
		case TPix:
			tpix = new TPixel[n()];
			if (tpix == NULL) throw MapException(MapException::Memory);
			returnfct = &Skymap::returnTpix;
			break;
		case PPix:
			tppix = new TPPixel[n()];
			if (tppix == NULL) throw MapException(MapException::Memory);
			returnfct = &Skymap::returnTPpix;
			break;
		case TnobsPix:
			tnobspix = new TnobsPixel[n()];
			if (tnobspix == NULL) throw MapException(MapException::Memory);
			returnfct = &Skymap::returnTnobspix;			
			break;
		case TPnobsPix:
			tpnobspix = new TPnobsPixel[n()];
			if (tpnobspix == NULL) throw MapException(MapException::Memory);
			returnfct = &Skymap::returnTPnobspix;
			break;
		default:
			throw MapException(MapException::InvalidType);
	}
	return;
}
/* ----------------------------------------------------------------------------
'freeMemory' releases the memory associated with the map and resets its
internal storage to show an empty map.

Arguments:
	None.

Returned:
	Nothing.
---------------------------------------------------------------------------- */
void Skymap::freeMemory()
{
	if (tpix       ) { delete[] tpix;       tpix       = 0; }
	if (tppix      ) { delete[] tppix;      tppix      = 0; }
	if (tnobspix   ) { delete[] tnobspix;   tnobspix   = 0; }
	if (tpnobspix  ) { delete[] tpnobspix;  tpnobspix  = 0; }
	init();
	return;
}
/* ----------------------------------------------------------------------------
'writeHdrDate' writes a DATE card to the header of the current HDU.

An exception is thrown in the event of a FITS error.

Arguments:
	fitsfile - The handle to the currently open FITS file.

Returned:
	Nothing.
---------------------------------------------------------------------------- */
void Skymap::writeHdrDate (fitsfile* fptr)
{
	char str[48], comm[48], key[] = {"DATE"};
	int  flg = 0, status = 0;
	if (fits_get_system_time(str, &flg, &status) != 0)
		throw MapException(MapException::FITSError, status);
	if (flg == 0) strcpy(comm, "File creation date (YYYY-MM-DDThh:mm:ss UT)");
	         else strcpy(comm, "File creation date (YYYY-MM-DDThh:mm:ss)");
	if (fits_update_key(fptr, TSTRING, key, str, comm, &status) != 0)
		throw MapException(MapException::FITSError, status);
	return;
}
/* ----------------------------------------------------------------------------
'readFITSPrimaryHeader' reads information from the primary FITS header.

This routine assumes that the current HDU is the primary HDU.

This routine does nothing; child classes should overload this function as
appropriate.

An exception is thrown in the event of a FITS error.

Arguments:
	fitsfile - The handle to the currently open FITS file.

Returned:
	Nothing.
---------------------------------------------------------------------------- */
void Skymap::readFITSPrimaryHeader (fitsfile* /*fptr*/)
{
	return;
}
/* ----------------------------------------------------------------------------
'writeFITSPrimaryHeader' writes information to the primary FITS header.

This routine assumes that the current HDU is the primary HDU.

This routine simply writes a DATE card to the header; child classes should
overload this function as appropriate.

An exception is thrown in the event of a FITS error.

Arguments:
	fitsfile - The handle to the currently open FITS file.

Returned:
	Nothing.
---------------------------------------------------------------------------- */
void Skymap::writeFITSPrimaryHeader (fitsfile* fptr)
{
	writeHdrDate(fptr);
	return;
}
/* ----------------------------------------------------------------------------
'readFITSExtensionHeader' reads information from the extension FITS header.

This routine assumes that the current HDU is the correct extension HDU.

This routine does nothing; child classes should overload this function as
appropriate.

An exception is thrown in the event of a FITS error.

Arguments:
	fitsfile - The handle to the currently open FITS file.

Returned:
	Nothing.
---------------------------------------------------------------------------- */
void Skymap::readFITSExtensionHeader (fitsfile* /*fptr*/)
{
	return;
}
/* ----------------------------------------------------------------------------
'writeFITSExtensionHeader' writes information to the primary FITS header.

This routine assumes that the current HDU is the correct extension HDU.

This routine simply writes a DATE card to the header; child classes should
overload this function as appropriate.

An exception is thrown in the event of a FITS error.

Arguments:
	fitsfile - The handle to the currently open FITS file.

Returned:
	Nothing.
---------------------------------------------------------------------------- */
void Skymap::writeFITSExtensionHeader (fitsfile* fptr)
{
	writeHdrDate(fptr);
	return;
}
/* ----------------------------------------------------------------------------
'set' creates the storage for a blank map.  If an invalid map type is supplied
an exception is thrown.

Arguments:
	n_in    - The number of pixels in the map.
	type_in - The type of map.

Returned:
	Nothing.
---------------------------------------------------------------------------- */
void Skymap::set(unsigned int n_in, Type type_in)
{
	if( n_in == n() && type_in == type() ) return;
	freeMemory();
	allocPixMemory(n_in, type_in);
	return;
}
/* ----------------------------------------------------------------------------
'copy' copies the contents of another map into this one.

Arguments:
	imap - The source map.

Returned:
	Nothing.
---------------------------------------------------------------------------- */
void Skymap::copy(Skymap &imap)
{
	unsigned int i;
	set(imap.size(), imap.type());
	for (i = 0; i < size(); i++)
	{
		switch (type())
		{
		  case TnobsPix:
			(*this)[i].T()    = imap[i].T();
			(*this)[i].Nobs() = imap[i].Nobs();
			break;
		  case TPnobsPix:
			(*this)[i].Nobs() = imap[i].Nobs();
		  case PPix:
			(*this)[i].Q()    = imap[i].Q();
			(*this)[i].U()    = imap[i].U();
			(*this)[i].Pmag() = imap[i].Pmag();
			(*this)[i].Pang() = imap[i].Pang();
		  case TPix:
			(*this)[i].T() = imap[i].T();
			break;
		  default:
			throw MapException(MapException::InvalidType);
		}
	}
	minpix = imap.minpix;
	maxpix = imap.maxpix;
	avgpix = imap.avgpix;
	stdpix = imap.stdpix;
	return;
}
/* ----------------------------------------------------------------------------
'computePolar' computes the polarization magnitude and angle for each pixel.

This routine should be called anytime the map is modified before the routines
that provide access to these values are called.  It is called at the end
of the 'readFITS' routine.

A MapException is thrown in the event of an error.

Arguments:
	None.

Returned:
	Nothing.
---------------------------------------------------------------------------- */
void Skymap::computePolar(void)
{
	unsigned int i;
	if (! has_Polarization()) return;
	for (i = 0; i < size(); i++)
	{
		(*this)[i].computePolar();
	}
}
/* ----------------------------------------------------------------------------
'calcStats' computes the statistics of the map:  the minimum, maximum, mean,
and standard deviation pixel values.

This routine must be called anytime the map is modified before the routines
that provide access to these statistics are called.  It is called at the end
of the 'readFITS' routine.

A MapException is thrown in the event of an error.

Arguments:
	None.

Returned:
	Nothing.
---------------------------------------------------------------------------- */
void Skymap::calcStats(void)
{
	unsigned int i;
	double       dn, dns;
/*
			First pixel.
*/
	i = 0;
	switch (type())
	{
	  case TnobsPix:
		avgpix.T()    = maxpix.T()    = minpix.T()    = (*this)[i].T();
		avgpix.Nobs() = maxpix.Nobs() = minpix.Nobs() = (*this)[i].Nobs();
		stdpix.T()    = (*this)[i].T()    * (*this)[i].T();
		stdpix.Nobs() = (*this)[i].Nobs() * (*this)[i].Nobs();
		break;
	  case TPnobsPix:
		avgpix.Nobs() = maxpix.Nobs() = minpix.Nobs() = (*this)[i].Nobs();
		stdpix.Nobs() = (*this)[i].Nobs() * (*this)[i].Nobs();
	  case PPix:
		avgpix.Q()    = maxpix.Q()    = minpix.Q()    = (*this)[i].Q();
		avgpix.U()    = maxpix.U()    = minpix.U()    = (*this)[i].U();
		avgpix.Pmag() = maxpix.Pmag() = minpix.Pmag() = (*this)[i].Pmag();
		avgpix.Pang() = maxpix.Pang() = minpix.Pang() = (*this)[i].Pang();
		stdpix.Q()    = (*this)[i].Q()    * (*this)[i].Q();
		stdpix.U()    = (*this)[i].U()    * (*this)[i].U();
		stdpix.Pmag() = (*this)[i].Pmag() * (*this)[i].Pmag();
		stdpix.Pang() = (*this)[i].Pang() * (*this)[i].Pang();
	  case TPix:
		avgpix.T() = maxpix.T() = minpix.T() = (*this)[i].T();
		stdpix.T() = (*this)[i].T() * (*this)[i].T();
		break;
	  default:
		throw MapException(MapException::InvalidType);
	}
/*
			Accumulate over the rest of the pixels.
*/
	for (i = 1; i < size(); i++)
	{
		switch (type())
		{
		  case TnobsPix:
		  	avgpix.T() += (*this)[i].T();
			stdpix.T() += (*this)[i].T() * (*this)[i].T();
			if ((*this)[i].T() < minpix.T()) minpix.T() = (*this)[i].T();
			if ((*this)[i].T() > maxpix.T()) maxpix.T() = (*this)[i].T();

		  	avgpix.Nobs() += (*this)[i].Nobs();
			stdpix.Nobs() += (*this)[i].Nobs() * (*this)[i].Nobs();
			if ((*this)[i].Nobs() < minpix.Nobs()) minpix.Nobs() = (*this)[i].Nobs();
			if ((*this)[i].Nobs() > maxpix.Nobs()) maxpix.Nobs() = (*this)[i].Nobs();
			break;
		  case TPnobsPix:
		  	avgpix.Nobs() += (*this)[i].Nobs();
			stdpix.Nobs() += (*this)[i].Nobs() * (*this)[i].Nobs();
			if ((*this)[i].Nobs() < minpix.Nobs()) minpix.Nobs() = (*this)[i].Nobs();
			if ((*this)[i].Nobs() > maxpix.Nobs()) maxpix.Nobs() = (*this)[i].Nobs();
		  case PPix:
		  	avgpix.Q() += (*this)[i].Q();
			stdpix.Q() += (*this)[i].Q() * (*this)[i].Q();
			if ((*this)[i].Q() < minpix.Q()) minpix.Q() = (*this)[i].Q();
			if ((*this)[i].Q() > maxpix.Q()) maxpix.Q() = (*this)[i].Q();

		  	avgpix.U() += (*this)[i].U();
			stdpix.U() += (*this)[i].U() * (*this)[i].U();
			if ((*this)[i].U() < minpix.U()) minpix.U() = (*this)[i].U();
			if ((*this)[i].U() > maxpix.U()) maxpix.U() = (*this)[i].U();

		  	avgpix.Pmag() += (*this)[i].Pmag();
			stdpix.Pmag() += (*this)[i].Pmag() * (*this)[i].Pmag();
			if ((*this)[i].Pmag() < minpix.Pmag()) minpix.Pmag() = (*this)[i].Pmag();
			if ((*this)[i].Pmag() > maxpix.Pmag()) maxpix.Pmag() = (*this)[i].Pmag();

		  	avgpix.Pang() += (*this)[i].Pang();
			stdpix.Pang() += (*this)[i].Pang() * (*this)[i].Pang();
			if ((*this)[i].Pang() < minpix.Pang()) minpix.Pang() = (*this)[i].Pang();
			if ((*this)[i].Pang() > maxpix.Pang()) maxpix.Pang() = (*this)[i].Pang();
		  case TPix:
		  	avgpix.T() += (*this)[i].T();
			stdpix.T() += (*this)[i].T() * (*this)[i].T();
			if ((*this)[i].T() < minpix.T()) minpix.T() = (*this)[i].T();
			if ((*this)[i].T() > maxpix.T()) maxpix.T() = (*this)[i].T();
			break;
		  default:
			throw MapException(MapException::InvalidType);
		}
	}
/*
			Compute means and standard deviations.
*/
	dn  = double(size());
	dns = dn * (dn - 1.0);
	switch (type())
	{
	  case TnobsPix:
		stdpix.T() = sqrt(((dn * stdpix.T()) - (avgpix.T() * avgpix.T())) / dns);
		avgpix.T() /= dn;

		stdpix.Nobs() = sqrt(((dn * stdpix.Nobs()) - (avgpix.Nobs() * avgpix.Nobs())) / dns);
		avgpix.Nobs() /= dn;
		break;
	  case TPnobsPix:
		stdpix.Nobs() = sqrt(((dn * stdpix.Nobs()) - (avgpix.Nobs() * avgpix.Nobs())) / dns);
		avgpix.Nobs() /= dn;
	  case PPix:
		stdpix.Q() = sqrt(((dn * stdpix.Q()) - (avgpix.Q() * avgpix.Q())) / dns);
		avgpix.Q() /= dn;

		stdpix.U() = sqrt(((dn * stdpix.U()) - (avgpix.U() * avgpix.U())) / dns);
		avgpix.U() /= dn;

		stdpix.Pmag() = sqrt(((dn * stdpix.Pmag()) - (avgpix.Pmag() * avgpix.Pmag())) / dns);
		avgpix.Pmag() /= dn;

		stdpix.Pang() = sqrt(((dn * stdpix.Pang()) - (avgpix.Pang() * avgpix.Pang())) / dns);
		avgpix.Pang() /= dn;
	  case TPix:
		stdpix.T() = sqrt(((dn * stdpix.T()) - (avgpix.T() * avgpix.T())) / dns);
		avgpix.T() /= dn;
		break;
	  default:
		throw MapException(MapException::InvalidType);
	}
	return;
}
/* ----------------------------------------------------------------------------
'readFITS' fills the map from a FITS file. An exception is thrown in the event
of a FITS error or if the appropriate FITS table cannot be found.  There must
be a temperature column!

Once the map has been read, 'calcStats' is called to compute its statistics.

Arguments:
	filename - The name of the FITS file.  It may be supplied as a char* string,
	           string, or QString.
	progwin  - A pointer to the file load progress window.

Returned:
	Nothing.
---------------------------------------------------------------------------- */
#ifdef NONE
void Skymap::readFITS(const char* filename, fileProgress *progwin)
{
	fitsfile *fptr;
	int      status = 0, icol = 0, qcol = 0, ucol = 0, ncol = 0, scol = 0;
	int      i, t, hducnt, hdutype, naxis;
	long     numrow, numcol, numpix, naxes[2];
	Type     maptyp;
	float    *tmp = NULL, nul = -999.;
/*
			Initialize.
*/
	clear();		// Empty the map.
	if (progwin != NULL) progwin->loadFile(filename);
/*
			Try to open and prepare the FITS file for reading.  Find the
			HDU that contains a valid skymap.  Read the headers for 
			interesting information.
*/
	fits_open_file(&fptr, filename, READONLY, &status);
	fits_get_num_hdus(fptr, &hducnt, &status);
	if ((status != 0) || (hducnt <= 1))
		throw MapException(MapException::FITSError, status);

	readFITSPrimaryHeader(fptr);

	for (i = 1; ((i <= hducnt) && (icol == 0)); i++)
	{
		status = 0;
		if (fits_movabs_hdu(fptr, 2, &hdutype, &status) != 0)
			throw MapException(MapException::FITSError, status);
		if (hdutype != BINARY_TBL) continue;
		fits_get_colnum(fptr, CASEINSEN, ICOLNAME, &t, &status);
		if (status != 0) continue;
		icol = t;
	}
	if (icol == 0) throw MapException(MapException::InvalidType);

	readFITSExtensionHeader(fptr);
/*
			Determine the size of the map.
*/
	fits_read_tdim(fptr, icol, 2, &naxis, naxes, &status);
	if (status != 0) throw MapException(MapException::FITSError, status);
	if (naxis > 1)
	{
		numrow = naxes[1];
		numcol = naxes[0];
		numpix = naxes[1] * naxes[0];
	}
	else
	{
		fits_get_num_rows(fptr, &numrow, &status);
		if (status != 0) throw MapException(MapException::FITSError, status);
		numcol = 1;
		numpix = numrow;
	}
/*
			Identify the map.  Both Q and U must be supplied for polarization
			data to be stored.  Allocate space.
*/
	if (fits_get_colnum(fptr, CASEINSEN, NCOLNAME, &t, &status) == 0) ncol = t;

	if (fits_get_colnum(fptr, CASEINSEN, QCOLNAME, &t, &status) == 0) qcol = t;
	if ((qcol == 0) && (fits_get_colnum(fptr, CASEINSEN, QCOLNAMEA, &t, &status) == 0)) qcol = t;
	if ((qcol == 0) && (fits_get_colnum(fptr, CASEINSEN, QCOLNAMEB, &t, &status) == 0)) qcol = t;
	if ((qcol == 0) && (fits_get_colnum(fptr, CASEINSEN, QCOLNAMEC, &t, &status) == 0)) qcol = t;

	if (fits_get_colnum(fptr, CASEINSEN, UCOLNAME, &t, &status) == 0) ucol = t;
	if ((ucol == 0) && (fits_get_colnum(fptr, CASEINSEN, UCOLNAMEA, &t, &status) == 0)) ucol = t;
	if ((ucol == 0) && (fits_get_colnum(fptr, CASEINSEN, UCOLNAMEB, &t, &status) == 0)) ucol = t;
	if ((ucol == 0) && (fits_get_colnum(fptr, CASEINSEN, UCOLNAMEC, &t, &status) == 0)) ucol = t;

	if ((qcol == 0) || (ucol == 0)) qcol = ucol = scol = 0;

	if      ((ncol != 0) && (qcol != 0)) 				maptyp = TPnobsPix;
	else if  (qcol != 0)                  				maptyp = PPix;
	else if  (ncol != 0)                  				maptyp = TnobsPix;
	else                                 				maptyp = TPix;
	allocPixMemory(numpix, maptyp);
	status = 0;
/*
			Fill the columns.
*/
	if ((tmp = new float[numpix]) == NULL) throw MapException(MapException::Memory);
/*
				Stokes I/temperature.
*/
	if (icol != 0)
	{
		if (progwin != NULL) progwin->loadTemperature();
		if (fits_read_col(fptr, TFLOAT, icol, 1, 1, numpix, &nul, tmp, &t, &status) != 0)
			throw MapException(MapException::FITSError, status);
		for (i = 0; i < numpix; i++) (*this)[i].T() = tmp[i];
	}
/*
				Stokes Q.
*/
	if (qcol != 0)
	{
		if (progwin != NULL) progwin->loadQPolarization();
		if (fits_read_col(fptr, TFLOAT, qcol, 1, 1, numpix, &nul, tmp, &t, &status) != 0)
			throw MapException(MapException::FITSError, status);
		for (i = 0; i < numpix; i++) (*this)[i].Q() = tmp[i];
	}
/*
				Stokes U.
*/
	if (ucol != 0)
	{
		if (progwin != NULL) progwin->loadUPolarization();
		if (fits_read_col(fptr, TFLOAT, ucol, 1, 1, numpix, &nul, tmp, &t, &status) != 0)
			throw MapException(MapException::FITSError, status);
		for (i = 0; i < numpix; i++) (*this)[i].U() = tmp[i];
	}
/*
				N_Obs.
*/
	if (ncol != 0)
	{
		if (progwin != NULL) progwin->loadNumObs();
		if (fits_read_col(fptr, TFLOAT, ncol, 1, 1, numpix, &nul, tmp, &t, &status) != 0)
			throw MapException(MapException::FITSError, status);
		for (i = 0; i < numpix; i++) (*this)[i].Nobs() = tmp[i];
	}
/*
			Done!  Close the file and compute statistics.
*/
	fits_close_file(fptr, &status);
	if (tmp != NULL) delete [] tmp;
	if ((qcol != 0) && (ucol != 0) && (progwin != NULL)) progwin->loadPPolarization();
	computePolar();
	calcStats();
	if (progwin != NULL) progwin->finished(this);
	return;
}
void Skymap::readFITS(string filename, fileProgress *progwin)
{
	readFITS(filename.c_str(), progwin);
}
void Skymap::readFITS(QString filename, fileProgress *progwin)
{
	readFITS(filename.toStdString(), progwin);
}
#endif
/* ----------------------------------------------------------------------------
'writeFITS' writes the map to a FITS file. An exception is thrown in the event
of a FITS error.

Arguments:
	filename - The name of the FITS file.  It may be supplied as a char* string,
	           string, or QString.
	tabname  - The name to assign the binary FITS table.  Defaults to NULL,
	           which corresponds to DEFTABLE.

Returned:
	Nothing.
---------------------------------------------------------------------------- */
void Skymap::writeFITS(const char* filename, char* tabname)
{
	char        *ttype[maxcols], *tform[maxcols], *tunit[maxcols];
	fitsfile    *fptr;
	int          status = 0;
	int          ncol, col;
	unsigned int i;
	float       *tmp = NULL;
/*
			Initialize.
*/
	if (strlen(tabname) <= 0) tabname = NULL;
	if (tabname == NULL) tabname = DEFTABLE;
	ncol = 0;
	ttype[ncol] = ICOLNAME; tform[ncol] = DEFFORM; tunit[ncol++] = DEFTUNIT;
	if ((type() == PPix) || (type() == TPnobsPix))
	{
		ttype[ncol] = QCOLNAME; tform[ncol] = DEFFORM; tunit[ncol++] = DEFTUNIT;
		ttype[ncol] = UCOLNAME; tform[ncol] = DEFFORM; tunit[ncol++] = DEFTUNIT;
	}
	if ((type() == TnobsPix) || (type() == TPnobsPix))
	{
		ttype[ncol] = NCOLNAME; tform[ncol] = DEFFORM; tunit[ncol++] = DEFNUNIT;
	}
/*
			Try to open and prepare the FITS file for writing.

				Try to create a new file.  If it exists, simply open it.  In either
				case, write additions to the primary header.
*/
	if (fits_create_file(&fptr, filename, &status) == 0)
	{
		fits_create_img(fptr, SHORT_IMG, 0, NULL, &status); // Create an empty primary HDU.
	}
	else
	{
		status = 0;
		fits_open_file(&fptr, filename, READWRITE, &status);
	}
	if (status != 0) throw MapException(MapException::FITSError, status);
	writeFITSPrimaryHeader(fptr);
/*
				Search the file for named table.  If found, remove it.
				Create a new binary table HDU and extend its header.
*/
	fits_movnam_hdu(fptr, BINARY_TBL, tabname, 0, &status);
	if ((status != 0) && (status != BAD_HDU_NUM)) throw MapException(MapException::FITSError, status);
	if (status == 0)
	{
		fits_delete_hdu(fptr, NULL, &status);
		if (status != 0) throw MapException(MapException::FITSError, status);
	}
	status = 0;
	if (fits_create_tbl(fptr, BINARY_TBL, 0, ncol, ttype, tform, tunit, tabname, &status) != 0)
		throw MapException(MapException::FITSError, status);
	writeFITSExtensionHeader(fptr);
/*
			Fill the columns.
*/
	if ((tmp = new float[size()]) == NULL) throw MapException(MapException::Memory);
    col = 0;
/*
				Stokes I/temperature.
*/
    col++;
	for (i = 0; i < size(); i++)
	{
		tmp[i] = (*this)[i].T();
	}
	if (fits_write_col(fptr, TFLOAT, col, 1, 1, size(), tmp, &status) != 0)
		throw MapException(MapException::FITSError, status);
/*
				Stokes Q.
*/
	if ((type() == PPix) || (type() == TPnobsPix))
	{
		col++;
		for (i = 0; i < size(); i++)
		{
			tmp[i] = (*this)[i].Q();
		}
		if (fits_write_col(fptr, TFLOAT, col, 1, 1, size(), tmp, &status) != 0)
			throw MapException(MapException::FITSError, status);
/*
				Stokes U.
*/
		col++;
		for (i = 0; i < size(); i++)
		{
			tmp[i] = (*this)[i].U();
		}
		if (fits_write_col(fptr, TFLOAT, col, 1, 1, size(), tmp, &status) != 0)
			throw MapException(MapException::FITSError, status);
	}
/*
				N_Obs.
*/
	if ((type() == TnobsPix) || (type() == TPnobsPix))
	{
		col++;
		for (i = 0; i < size(); i++)
		{
			tmp[i] = (*this)[i].Nobs();
		}
		if (fits_write_col(fptr, TFLOAT, col, 1, 1, size(), tmp, &status) != 0)
			throw MapException(MapException::FITSError, status);
	}
/*
			Done!
*/
	fits_close_file(fptr, &status);
	if (tmp != NULL) delete [] tmp;
	return;
}
void Skymap::writeFITS(string filename, char* tabname)
{
  writeFITS(filename.c_str(), tabname);
}
void Skymap::writeFITS(QString filename, char* tabname)
{
  writeFITS(filename.toStdString(), tabname);
}
/* ----------------------------------------------------------------------------
'readFITS' fills the map from a FITS file. An exception is thrown in the event
of a FITS error or if the appropriate FITS table cannot be found.  There must
be a temperature column!

Once the map has been read, 'calcStats' is called to compute its statistics.

Arguments:
	filename - The name of the FITS file.  It may be supplied as a char* string,
	           string, or QString.
	progwin  - A pointer to the file load progress window.

Returned:
	Nothing.
---------------------------------------------------------------------------- */
void Skymap::readFITS(const char* filename, ControlDialog *progwin)
{
    fitsfile *fptr;
    int      status = 0, icol = 0, qcol = 0, ucol = 0, ncol = 0, scol = 0, planck_map = 0;
    int      i, t, hducnt, hdutype, naxis, bstatus=0;
    long     numrow, numcol, numpix, naxes[2];
    char     comment[FLEN_COMMENT];
    double   badvalue;
    float    fbadvalue=HEALPIX_NULLVAL;
    Type     maptyp;
    float    *tmp = NULL, nul = -999.;
/*
			Initialize.
*/
	clear();		// Empty the map.
	if (progwin != NULL) progwin->loadFile(filename);
/*
			Try to open and prepare the FITS file for reading.  Find the
			HDU that contains a valid skymap.  Read the headers for 
			interesting information.
*/
	fits_open_file(&fptr, filename, READONLY, &status);
	fits_get_num_hdus(fptr, &hducnt, &status);
	if ((status != 0) || (hducnt <= 1))
		throw MapException(MapException::FITSError, status);

	readFITSPrimaryHeader(fptr);

	for (i = 1; ((i <= hducnt) && (icol == 0)); i++)
	{
		status = 0;
		if (fits_movabs_hdu(fptr, 2, &hdutype, &status) != 0)
			throw MapException(MapException::FITSError, status);
		if (hdutype != BINARY_TBL) continue;
		fits_get_colnum(fptr, CASEINSEN, ICOLNAME, &t, &status);
        if (status != 0)
        {
            status = 0;
            fits_get_colnum(fptr, CASEINSEN, ICOLNAMEA, &t, &status);
            planck_map = 1;
        }
        if (status != 0)
        {
            status = 0;
            fits_get_colnum(fptr, CASEINSEN, ICOLNAMEB, &t, &status);
            planck_map = 1;
        }
		if (status != 0) continue;
		icol = t;
	}
	if (icol == 0) throw MapException(MapException::InvalidType);

	readFITSExtensionHeader(fptr);
/*
			Determine the size of the map.
*/
	fits_read_tdim(fptr, icol, 2, &naxis, naxes, &status);
    if (planck_map)
    {
        fits_read_key_dbl(fptr, "BAD_DATA", &badvalue, comment, &bstatus);
        if (bstatus==0) fbadvalue=(float) badvalue;
    }
	fits_get_num_rows(fptr, &numrow, &status);
	if (status != 0) throw MapException(MapException::FITSError, status);
	numcol = (naxes[0] > 1) ? naxes[0] : 1;
	numpix = numcol * numrow;
/*
			Identify the map.  Both Q and U must be supplied for polarization
			data to be stored.  Allocate space.
*/
    fits_get_colnum(fptr, CASEINSEN, NCOLNAME, &t, &status);
    if (status == 0) ncol = t;
    if ((ncol==0) && (status > 0)) {status = 0; fits_get_colnum(fptr, CASEINSEN, NCOLNAMEA, &t, &status);};
    if (status == 0) ncol = t; else status = 0;

    fits_get_colnum(fptr, CASEINSEN, QCOLNAME, &t, &status);
    if (status == 0) qcol = t;
    if ((qcol==0) && (status > 0)) {status = 0; fits_get_colnum(fptr, CASEINSEN, QCOLNAMEA, &t, &status);};
    if (status == 0) qcol = t;
    if ((qcol==0) && (status > 0)) {status = 0; fits_get_colnum(fptr, CASEINSEN, QCOLNAMEB, &t, &status);};
    if (status == 0) qcol = t;
    if ((qcol==0) && (status > 0)) {status = 0; fits_get_colnum(fptr, CASEINSEN, QCOLNAMEC, &t, &status);};
    if (status == 0) qcol = t;
    if ((qcol==0) && (status > 0)) {status = 0; fits_get_colnum(fptr, CASEINSEN, QCOLNAMED, &t, &status);};
    if (status == 0) qcol = t;
    if ((qcol==0) && (status > 0)) {status = 0; fits_get_colnum(fptr, CASEINSEN, QCOLNAMEE, &t, &status);};
    if (status == 0) qcol = t; else status = 0;

    fits_get_colnum(fptr, CASEINSEN, UCOLNAME, &t, &status);
    if (status == 0) ucol = t;
    if ((ucol==0) && (status > 0)) {status = 0; fits_get_colnum(fptr, CASEINSEN, UCOLNAMEA, &t, &status);};
    if (status == 0) ucol = t;
    if ((ucol==0) && (status > 0)) {status = 0; fits_get_colnum(fptr, CASEINSEN, UCOLNAMEB, &t, &status);};
    if (status == 0) ucol = t;
    if ((ucol==0) && (status > 0)) {status = 0; fits_get_colnum(fptr, CASEINSEN, UCOLNAMEC, &t, &status);};
    if (status == 0) ucol = t;
    if ((ucol==0) && (status > 0)) {status = 0; fits_get_colnum(fptr, CASEINSEN, UCOLNAMED, &t, &status);};
    if (status == 0) ucol = t;
    if ((ucol==0) && (status > 0)) {status = 0; fits_get_colnum(fptr, CASEINSEN, UCOLNAMEE, &t, &status);};
    if (status == 0) ucol = t;

	if ((qcol == 0) || (ucol == 0)) qcol = ucol = scol = 0;

	if      ((ncol != 0) && (qcol != 0)) 				maptyp = TPnobsPix;
	else if  (qcol != 0)                  				maptyp = PPix;
	else if  (ncol != 0)                  				maptyp = TnobsPix;
	else                                 				maptyp = TPix;
	allocPixMemory(numpix, maptyp);
	status = 0;

	// Let the progwin know what fields we have
	if( progwin != NULL ) {
		progwin->hasField(I, icol != 0 );
		progwin->hasField(Q, qcol != 0 );
		progwin->hasField(U, ucol != 0 );
		progwin->hasField(P, (ucol != 0) || (qcol != 0) );
		progwin->hasField(Nobs, ncol != 0 );
	}
/*
			Fill the columns.
*/
	if ((tmp = new float[numpix]) == NULL) throw MapException(MapException::Memory);
/*
				Stokes I/temperature.
*/
	if (icol != 0)
	{
		if (progwin != NULL) progwin->loadField(I);
		if (fits_read_col(fptr, TFLOAT, icol, 1, 1, numpix, &nul, tmp, &t, &status) != 0)
			throw MapException(MapException::FITSError, status);
        for (i = 0; i < numpix; i++)
        {
            if (tmp[i] == fbadvalue)
            {
                (*this)[i].T() = 0.0;
            }
            else
            {
                (*this)[i].T() = tmp[i];
            };
         }
	}
/*
				Stokes Q.
*/
	if (qcol != 0)
	{
		if (progwin != NULL) progwin->loadField(Q);
		if (fits_read_col(fptr, TFLOAT, qcol, 1, 1, numpix, &nul, tmp, &t, &status) != 0)
			throw MapException(MapException::FITSError, status);
        for (i = 0; i < numpix; i++)
        {
            if (tmp[i] == fbadvalue)
            {
                (*this)[i].Q() = 0.0;
            }
            else
            {
                (*this)[i].Q() = tmp[i];
            };
        }
	}
/*
				Stokes U.
*/
	if (ucol != 0)
	{
		if (progwin != NULL) progwin->loadField(U);
		if (fits_read_col(fptr, TFLOAT, ucol, 1, 1, numpix, &nul, tmp, &t, &status) != 0)
			throw MapException(MapException::FITSError, status);
        for (i = 0; i < numpix; i++)
        {
            if (tmp[i] == fbadvalue)
            {
                (*this)[i].U() = 0.0;
            }
            else
            {
                (*this)[i].U() = tmp[i];
            };
        }
	}
/*
				N_Obs.
*/
	if (ncol != 0)
	{
		if (progwin != NULL) progwin->loadField( Nobs );
		if (fits_read_col(fptr, TFLOAT, ncol, 1, 1, numpix, &nul, tmp, &t, &status) != 0)
			throw MapException(MapException::FITSError, status);
		for (i = 0; i < numpix; i++) (*this)[i].Nobs() = tmp[i];
	}
/*
			Done!  Close the file and compute statistics.
*/
	fits_close_file(fptr, &status);
	if (tmp != NULL) delete [] tmp;
	if ((qcol != 0) && (ucol != 0) && (progwin != NULL)) progwin->loadField(P);
	computePolar();
	calcStats();
	if (progwin != NULL) progwin->finished(this);
	return;
}
void Skymap::readFITS(string filename, ControlDialog *progwin)
{
	readFITS(filename.c_str(), progwin);
}
void Skymap::readFITS(QString filename, ControlDialog *progwin)
{
	readFITS(filename.toStdString(), progwin);
}
