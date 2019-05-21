/* ============================================================================
'pixel.cpp' defines the pixel class methods.  The classes are defined in 
'pixel.h'.

Written by Nicholas Phillips, December 2006
Broken out of 'skymap.h'.  MRG, ADNET, 23 January 2007.
============================================================================ */
#include <math.h>
#include "pixel.h"
/* ============================================================================
The BasePixel class provides a common denominator for the various types of
supported maps.  Up to six data elements per pixel are supported:
Stokes I, Q, U, NObs, and the polarization magnitude and angle wrt meridian.

The default data access functions are defined here.  They all throw exceptions
indicating that the data requested is not defined.  Child classes should 
overload the relevant functions to provide support for that data.
============================================================================ */
double BasePixel::ddummy  = 0.0;
/* ------------------------------------------------------------------------- */
double BasePixel::T() const
{
	throw MapException(MapException::Undefined);
	return 0;
}
/* ------------------------------------------------------------------------- */
double & BasePixel::T()
{
	throw MapException(MapException::Undefined);
	return ddummy;
}
/* ------------------------------------------------------------------------- */
double BasePixel::I() const
{
	throw MapException(MapException::Undefined);
	return 0;
}
/* ------------------------------------------------------------------------- */
double & BasePixel::I()
{
	throw MapException(MapException::Undefined);
	return ddummy;	// not that we will ever get here
}
/* ------------------------------------------------------------------------- */
double BasePixel::Q() const
{
	throw MapException(MapException::Undefined);
	return 0;
}
/* ------------------------------------------------------------------------- */
double & BasePixel::Q()
{
	throw MapException(MapException::Undefined);
	return ddummy;
}
/* ------------------------------------------------------------------------- */
double BasePixel::U() const
{
	throw MapException(MapException::Undefined);
	return 0;
}
/* ------------------------------------------------------------------------- */
double & BasePixel::U()
{
	throw MapException(MapException::Undefined);
	return ddummy;
}
/* ------------------------------------------------------------------------- */
double BasePixel::Nobs() const
{
	throw MapException(MapException::Undefined);
	return 0;
}
/* ------------------------------------------------------------------------- */
double & BasePixel::Nobs()
{
	throw MapException(MapException::Undefined);
	return ddummy;	// not that we will ever get here
}
/* ------------------------------------------------------------------------- */
double BasePixel::Pmag() const
{
	throw MapException(MapException::Undefined);
	return 0;
}
/* ------------------------------------------------------------------------- */
double & BasePixel::Pmag()
{
	throw MapException(MapException::Undefined);
	return ddummy;	// not that we will ever get here
}
/* ------------------------------------------------------------------------- */
double BasePixel::Pang() const
{
	throw MapException(MapException::Undefined);
	return 0;
}
/* ------------------------------------------------------------------------- */
double & BasePixel::Pang()
{
	throw MapException(MapException::Undefined);
	return ddummy;	// not that we will ever get here
}
/* ----------------------------------------------------------------------------
'copy' copies another pixel into this one.  This function captures and ignores
MapException exceptions; it does NOT rethrow them.

Arguments:
	&src - The pixel to copy.

Returned:
	Nothing.

Written by Michael R. Greason, ADNET, 27 March 2007.
---------------------------------------------------------------------------- */
void BasePixel::copy (BasePixel& src)
{
	try { this->T()    = src.T();    } catch (MapException &exc) { ; }
	try { this->Q()    = src.Q();    } catch (MapException &exc) { ; }
	try { this->U()    = src.U();    } catch (MapException &exc) { ; }
	try { this->Nobs() = src.Nobs(); } catch (MapException &exc) { ; }
	try { this->Pmag() = src.Pmag(); } catch (MapException &exc) { ; }
	try { this->Pang() = src.Pang(); } catch (MapException &exc) { ; }
}
/* ----------------------------------------------------------------------------
'clear' zeros the pixel out.  This function captures and ignores MapException
exceptions; it does NOT rethrow them.

Arguments:
	None.

Returned:
	Nothing.

Written by Michael R. Greason, ADNET, 27 March 2007.
---------------------------------------------------------------------------- */
void BasePixel::clear (void)
{
	try { this->T()    = 0.0; } catch (MapException &exc) { ; }
	try { this->Q()    = 0.0; } catch (MapException &exc) { ; }
	try { this->U()    = 0.0; } catch (MapException &exc) { ; }
	try { this->Nobs() = 0.0; } catch (MapException &exc) { ; }
	try { this->Pmag() = 0.0; } catch (MapException &exc) { ; }
	try { this->Pang() = 0.0; } catch (MapException &exc) { ; }
}
/* ============================================================================
The TPPixel class defines a Stokes I, Q, U pixel.  The polarization magnitude
and angle will be computed whenever Q and U are set.
============================================================================ */
/* ----------------------------------------------------------------------------
'computePolar' computes the polarization magnitude and angle.

Arguments:
	None.

Returned:
	Nothing.

Written by Michael R. Greason, ADNET, 30 August 2007.
---------------------------------------------------------------------------- */
void TPPixel::computePolar (void)
{
	Pmag_ = sqrt( (Q_ * Q_) + (U_ * U_) );
	Pang_ = atan2(U_, Q_) / 2.;
}
