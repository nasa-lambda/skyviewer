#ifndef SKYMAP_HPP
#define SKYMAP_HPP
/* ============================================================================
'skymap.h' defines the pixel and skymap classes.  Non-inline methods and space
for static class members are defined in 'skymap.cpp'.

Written by Nicholas Phillips, December 2006
Adapted for WMAP.  FITS and copy operator added.  Michael R. Greason, ADNET,
	27 December 2006.
============================================================================ */
#include <fitsio.h>
#include <QString>
#include <string>
#include "pixel.h"
//#include "fileprogress.h"

class ControlDialog;
/* =============================================================================
The Skymap class defines a collection of pixels to represent a sky map.
This version allows for dynamically selecting how much information is stored 
for each pixel. Can be as simple as just the temperature or up to temperature, 
polarization and number of observations.  This class has no knowledge of the
pixel formatting; it is up to child classes to provide this information.

When set() is called, memory is allocated for the type of pixel requested.
The address is stored in the appropiate data points (tpix, tppix, etc) and
the member function pointer returnfct is set to point to the corresponding
indexing method (returnTpix, etc). Now when the indexing operator[] is called,
this is inlined de-referenced to the correct index method.  In turn, this method 
is just an inlined index into the corresponding data array.

A bunch of inlined syntax sugar so we can write 
map2[i].T() = map1[j].T()

There are four functions that are used to read and write the FITS headers.  The
two write routines exist to add keywords to the default headers.  These versions
do nothing; child classes should overload them as needed.
============================================================================= */
class Skymap
{
	public:
		// The possible data that can be stored per pixel
		enum Type { 	
			none,		// No type set 
			TPix, 		// Only temperature
			PPix, 		// Temperature and Polarization, i.e. Stoke's parameters
			TnobsPix, 	// Temperature and Number of observations
			TPnobsPix 	// Temperature, Polarization and Number of observations
		};

	protected:
		Type type_;						// The current data Type
		unsigned int n_;				// The current number of pixels
		TPixel       *tpix;				// When TPix, where to store data
		TPPixel      *tppix;			// When PPix, where to store data
		TnobsPixel   *tnobspix;			// When TnobsPix, where to store data
		TPnobsPixel  *tpnobspix;		// When TPnobsPix, where to store data

		TPnobsPixel   minpix;			// Minimum pixel values.
		TPnobsPixel   maxpix;			// Maximum pixel values.
		TPnobsPixel   avgpix;			// Mean pixel value.
		TPnobsPixel   stdpix;			// Std. dev. of the mean.

		// Dummy variable needed for completeness of returnnone()
		static BasePixel basedummy;

		// Set to zero size and no type
		virtual void init();
		// Allocate pixel memory.
		virtual void allocPixMemory(unsigned int n_in, Type type_in);
		// Free all heap memory.
		virtual void freeMemory();

		//------------------------------------------------------------------------
		// Versions of base indexing function to return a reference into this map
		// Used when the contents of the map are to be modified

		// method to call to return a refernece to an indexed pixel
		BasePixel& (Skymap::*returnfct)(unsigned int);
		
		// Index look-up method for when no data is allocated
		BasePixel& returnnone(unsigned int i);
		// Index look-up method for when type = TPix
		BasePixel& returnTpix(unsigned int i);
		// Index look-up method for when type = PPix
		BasePixel& returnTPpix(unsigned int i);
		// Index look-up method for when type = TnobsPix
		BasePixel& returnTnobspix(unsigned int i);
		// Index look-up method for when type = TPnobsPix
		BasePixel& returnTPnobspix(unsigned int i);


		// Functions to read/write the FITS headers.
		void writeHdrDate (fitsfile *fptr);
		virtual void readFITSPrimaryHeader    (fitsfile *fptr);
		virtual void writeFITSPrimaryHeader   (fitsfile *fptr);
		virtual void readFITSExtensionHeader  (fitsfile *fptr);
		virtual void writeFITSExtensionHeader (fitsfile *fptr);
	public:
		// Create with no data and Type
		Skymap();

		// Create with a selected size and Type
		Skymap(unsigned int n_in, Type type_in);

		// Done, call freeMemory()
		virtual ~Skymap();

		// Fundamental method to set size and type of data
		virtual void set(unsigned int n_in, Type type_in);

		// Clear stored memory and reset state
		virtual void clear() { set(0,none); }

		// Copy another map into this one.
		void copy (Skymap &imap);

		// Return the number of pixels
		unsigned int size() const { return n_; }

		// Return the number of pixels
		unsigned int n() const { return n_; }

		// Return the type of data stored at each pixel
		Type type() const { return type_; }
		bool has_Temperature (void) const;
		bool has_Polarization (void) const;
		bool has_Nobs (void) const;
		
		// Basic function to access a pixel
		BasePixel& operator[](unsigned int i);
		
		// Copy operator.
		Skymap& operator= (Skymap &imap);

		// Compute the polarization magnitude and angle for each pixel.
		virtual void computePolar (void);

		// Compute and return statistics on the map.
		virtual void calcStats (void);
		
		double getMeanT() const;
		double getStdDevT() const;
		double getMinT() const;
		double getMaxT() const;

		double getMeanQ() const;
		double getStdDevQ() const;
		double getMinQ() const;
		double getMaxQ() const;

		double getMeanU() const;
		double getStdDevU() const;
		double getMinU() const;
		double getMaxU() const;

		double getMeanNobs() const;
		double getStdDevNobs() const;
		double getMinNobs() const;
		double getMaxNobs() const;

		double getMeanPmag() const;
		double getStdDevPmag() const;
		double getMinPmag() const;
		double getMaxPmag() const;

		double getMeanPang() const;
		double getStdDevPang() const;
		double getMinPang() const;
		double getMaxPang() const;

		// Read a map from a FITS file.
		/*
		virtual void readFITS (const char* filename, fileProgress *progwin = NULL);
		virtual void readFITS (std::string filename, fileProgress *progwin = NULL);
		virtual void readFITS (QString filename, fileProgress *progwin = NULL);
		*/
		// Read a map from a FITS file.
		virtual void readFITS (const char* filename, ControlDialog *progwin = NULL);
		virtual void readFITS (std::string filename, ControlDialog *progwin = NULL);
		virtual void readFITS (QString filename, ControlDialog *progwin = NULL);

		// Write a map to a FITS file.
		virtual void writeFITS (const char* filename, char* tabname = NULL);
		virtual void writeFITS (std::string filename, char* tabname = NULL);
		virtual void writeFITS (QString filename, char* tabname = NULL);
};
//----------------------------------------------------------------------------------
inline BasePixel& Skymap::returnTpix(unsigned int i)       { return tpix[i]; }
inline BasePixel& Skymap::returnTPpix(unsigned int i)      { return tppix[i]; }
inline BasePixel& Skymap::returnTnobspix(unsigned int i)   { return tnobspix[i]; }
inline BasePixel& Skymap::returnTPnobspix(unsigned int i)  { return tpnobspix[i]; }
/* ----------------------------------------------------------------------------
'has_Temperature' returns true if the map contains Stokes I/temperature data.

Arguments:
	None.

Returned:
	true if the map contains the desired data.
---------------------------------------------------------------------------- */
inline bool Skymap::has_Temperature (void) const
{
	return (type() != none);
}
/* ----------------------------------------------------------------------------
'has_Polarization' returns true if the map contains Stokes Q and U data.

Arguments:
	None.

Returned:
	true if the map contains the desired data.
---------------------------------------------------------------------------- */
inline bool Skymap::has_Polarization (void) const
{
	return ((type() == PPix) || (type() == TPnobsPix));
}
/* ----------------------------------------------------------------------------
'has_Nobs' returns true if the map contains N_obs data.

Arguments:
	None.

Returned:
	true if the map contains the desired data.
---------------------------------------------------------------------------- */
inline bool Skymap::has_Nobs (void) const
{
	return ((type() == TnobsPix) || (type() == TPnobsPix));
}
/* ----------------------------------------------------------------------------
'operator[]' allows the sky map to be indexed as an array.

Arguments:
	i  -  The index into the map.

Returned:
	A reference to the pixel is returned.  An exception is thrown if the index is out of
	bounds.
---------------------------------------------------------------------------- */
inline BasePixel& Skymap::operator[](unsigned int i)
{ 
	if (i >= n_) throw MapException(MapException::Bounds);
	return (this->*returnfct)(i); 
}
/* ----------------------------------------------------------------------------
'operator=' copies another map into this one using the assignment operator.

Arguments:
	imap - The source map.

Returned:
	A reference to this map.
---------------------------------------------------------------------------- */
inline Skymap& Skymap::operator= (Skymap &imap)
{
	copy(imap);
	return *this;
}
/* ----------------------------------------------------------------------------
'getMeanT' returns the average temperature/Stokes I map value.  'calcStats'
should have been called since the last change to the map before trying to use 
this routine.

Arguments:
	None..

Returned:
	double - The mean value.
---------------------------------------------------------------------------- */
inline double Skymap::getMeanT(void) const
{
	if (! has_Temperature()) throw MapException(MapException::InvalidType);
	return avgpix.T();
}
/* ----------------------------------------------------------------------------
'getStdDevT' returns the standard deviation of the mean temperature/Stokes I 
map value.  'calcStats' should have been called since the last change to the 
map before trying to use this routine.

Arguments:
	None..

Returned:
	double - The standard deviation.
---------------------------------------------------------------------------- */
inline double Skymap::getStdDevT(void) const
{
	if (! has_Temperature()) throw MapException(MapException::InvalidType);
	return stdpix.T();
}
/* ----------------------------------------------------------------------------
'getMinT' returns the minimum temperature/Stokes I map value.  'calcStats'
should have been called since the last change to the map before trying to use 
this routine.

Arguments:
	None..

Returned:
	double - The minimum value.
---------------------------------------------------------------------------- */
inline double Skymap::getMinT(void) const
{
	if (! has_Temperature()) throw MapException(MapException::InvalidType);
	return minpix.T();
}
/* ----------------------------------------------------------------------------
'getMaxT' returns the maximum temperature/Stokes I map value.  'calcStats'
should have been called since the last change to the map before trying to use 
this routine.

Arguments:
	None..

Returned:
	double - The maximum value.
---------------------------------------------------------------------------- */
inline double Skymap::getMaxT(void) const
{
	if (! has_Temperature()) throw MapException(MapException::InvalidType);
	return maxpix.T();
}
/* ----------------------------------------------------------------------------
'getMeanQ' returns the average Stokes Q map value.  'calcStats' should have 
been called since the last change to the map before trying to use this routine.

Arguments:
	None..

Returned:
	double - The mean value.
---------------------------------------------------------------------------- */
inline double Skymap::getMeanQ(void) const
{
	if (! has_Polarization()) throw MapException(MapException::InvalidType);
	return avgpix.Q();
}
/* ----------------------------------------------------------------------------
'getStdDevQ' returns the standard deviation of the Stokes Q map value.  
'calcStats' should have been called since the last change to the map before 
trying to use this routine.

Arguments:
	None..

Returned:
	double - The standard deviation.
---------------------------------------------------------------------------- */
inline double Skymap::getStdDevQ(void) const
{
	if (! has_Polarization()) throw MapException(MapException::InvalidType);
	return stdpix.Q();
}
/* ----------------------------------------------------------------------------
'getMinQ' returns the minimum Stokes Q map value.  'calcStats' should have 
been called since the last change to the map before trying to use this routine.

Arguments:
	None..

Returned:
	double - The minimum value.
---------------------------------------------------------------------------- */
inline double Skymap::getMinQ(void) const
{
	if (! has_Polarization()) throw MapException(MapException::InvalidType);
	return minpix.Q();
}
/* ----------------------------------------------------------------------------
'getMaxQ' returns the maximum Stokes Q map value.  'calcStats' should have 
been called since the last change to the map before trying to use this routine.

Arguments:
	None..

Returned:
	double - The maximum value.
---------------------------------------------------------------------------- */
inline double Skymap::getMaxQ(void) const
{
	if (! has_Polarization()) throw MapException(MapException::InvalidType);
	return maxpix.Q();
}
/* ----------------------------------------------------------------------------
'getMeanU' returns the average Stokes U map value.  'calcStats' should have 
been called since the last change to the map before trying to use this routine.

Arguments:
	None..

Returned:
	double - The mean value.
---------------------------------------------------------------------------- */
inline double Skymap::getMeanU(void) const
{
	if (! has_Polarization()) throw MapException(MapException::InvalidType);
	return avgpix.U();
}
/* ----------------------------------------------------------------------------
'getStdDevU' returns the standard deviation of the Stokes U map value.  
'calcStats' should have been called since the last change to the map before 
trying to use this routine.

Arguments:
	None..

Returned:
	double - The standard deviation.
---------------------------------------------------------------------------- */
inline double Skymap::getStdDevU(void) const
{
	if (! has_Polarization()) throw MapException(MapException::InvalidType);
	return stdpix.U();
}
/* ----------------------------------------------------------------------------
'getMinU' returns the minimum Stokes U map value.  'calcStats' should have 
been called since the last change to the map before trying to use this routine.

Arguments:
	None..

Returned:
	double - The minimum value.
---------------------------------------------------------------------------- */
inline double Skymap::getMinU(void) const
{
	if (! has_Polarization()) throw MapException(MapException::InvalidType);
	return minpix.U();
}
/* ----------------------------------------------------------------------------
'getMaxU' returns the maximum Stokes U map value.  'calcStats' should have 
been called since the last change to the map before trying to use this routine.

Arguments:
	None..

Returned:
	double - The maximum value.
---------------------------------------------------------------------------- */
inline double Skymap::getMaxU(void) const
{
	if (! has_Polarization()) throw MapException(MapException::InvalidType);
	return maxpix.U();
}
/* ----------------------------------------------------------------------------
'getMeanNobs' returns the average Nobs map value.  'calcStats' should have 
been called since the last change to the map before trying to use this routine.

Arguments:
	None..

Returned:
	double - The mean value.
---------------------------------------------------------------------------- */
inline double Skymap::getMeanNobs(void) const
{
	if (! has_Nobs()) throw MapException(MapException::InvalidType);
	return avgpix.Nobs();
}
/* ----------------------------------------------------------------------------
'getStdDevNobs' returns the standard deviation of the Nobs map value.  
'calcStats' should have been called since the last change to the map before 
trying to use this routine.

Arguments:
	None..

Returned:
	double - The standard deviation.
---------------------------------------------------------------------------- */
inline double Skymap::getStdDevNobs(void) const
{
	if (! has_Nobs()) throw MapException(MapException::InvalidType);
	return stdpix.Nobs();
}
/* ----------------------------------------------------------------------------
'getMinNobs' returns the minimum Nobs map value.  'calcStats' should have 
been called since the last change to the map before trying to use this routine.

Arguments:
	None..

Returned:
	double - The minimum value.
---------------------------------------------------------------------------- */
inline double Skymap::getMinNobs(void) const
{
	if (! has_Nobs()) throw MapException(MapException::InvalidType);
	return minpix.Nobs();
}
/* ----------------------------------------------------------------------------
'getMaxNobs' returns the maximum Nobs map value.  'calcStats' should have 
been called since the last change to the map before trying to use this routine.

Arguments:
	None..

Returned:
	double - The maximum value.
---------------------------------------------------------------------------- */
inline double Skymap::getMaxNobs(void) const
{
	if (! has_Nobs()) throw MapException(MapException::InvalidType);
	return maxpix.Nobs();
}
/* ----------------------------------------------------------------------------
'getMeanPmag' returns the average polarization magnitude map value.  

'computePolar' and 'calcStats' should have been called since the last change 
to the map before trying to use this routine.

Arguments:
	None..

Returned:
	double - The mean value.
---------------------------------------------------------------------------- */
inline double Skymap::getMeanPmag(void) const
{
	if (! has_Polarization()) throw MapException(MapException::InvalidType);
	return avgpix.Pmag();
}
/* ----------------------------------------------------------------------------
'getStdDevPmag' returns the standard deviation of the polarization magnitude 
map value.   

'computePolar' and 'calcStats' should have been called since the last change 
to the map before trying to use this routine.

Arguments:
	None..

Returned:
	double - The standard deviation.
---------------------------------------------------------------------------- */
inline double Skymap::getStdDevPmag(void) const
{
	if (! has_Polarization()) throw MapException(MapException::InvalidType);
	return stdpix.Pmag();
}
/* ----------------------------------------------------------------------------
'getMinPmag' returns the minimum polarization magnitude map value.  

'computePolar' and 'calcStats' should have been called since the last change 
to the map before trying to use this routine.

Arguments:
	None..

Returned:
	double - The minimum value.
---------------------------------------------------------------------------- */
inline double Skymap::getMinPmag(void) const
{
	if (! has_Polarization()) throw MapException(MapException::InvalidType);
	return minpix.Pmag();
}
/* ----------------------------------------------------------------------------
'getMaxPmag' returns the maximum polarization magnitude map value.  

'computePolar' and 'calcStats' should have been called since the last change 
to the map before trying to use this routine.

Arguments:
	None..

Returned:
	double - The maximum value.
---------------------------------------------------------------------------- */
inline double Skymap::getMaxPmag(void) const
{
	if (! has_Polarization()) throw MapException(MapException::InvalidType);
	return maxpix.Pmag();
}
/* ----------------------------------------------------------------------------
'getMeanPang' returns the average polarization angle map value.  

'computePolar' and 'calcStats' should have been called since the last change 
to the map before trying to use this routine.

Arguments:
	None..

Returned:
	double - The mean value.
---------------------------------------------------------------------------- */
inline double Skymap::getMeanPang(void) const
{
	if (! has_Polarization()) throw MapException(MapException::InvalidType);
	return avgpix.Pang();
}
/* ----------------------------------------------------------------------------
'getStdDevPang' returns the standard deviation of the polarization angle 
map value.  

'computePolar' and 'calcStats' should have been called since the last change 
to the map before trying to use this routine.

Arguments:
	None..

Returned:
	double - The standard deviation.
---------------------------------------------------------------------------- */
inline double Skymap::getStdDevPang(void) const
{
	if (! has_Polarization()) throw MapException(MapException::InvalidType);
	return stdpix.Pang();
}
/* ----------------------------------------------------------------------------
'getMinPang' returns the minimum polarization angle map value.  

'computePolar' and 'calcStats' should have been called since the last change 
to the map before trying to use this routine.

Arguments:
	None..

Returned:
	double - The minimum value.
---------------------------------------------------------------------------- */
inline double Skymap::getMinPang(void) const
{
	if (! has_Polarization()) throw MapException(MapException::InvalidType);
	return minpix.Pang();
}
/* ----------------------------------------------------------------------------
'getMaxPang' returns the maximum polarization angle map value.  

'computePolar' and 'calcStats' should have been called since the last change 
to the map before trying to use this routine.

Arguments:
	None..

Returned:
	double - The maximum value.
---------------------------------------------------------------------------- */
inline double Skymap::getMaxPang(void) const
{
	if (! has_Polarization()) throw MapException(MapException::InvalidType);
	return maxpix.Pang();
}
#endif
