#ifndef PIXEL_HPP
#define PIXEL_HPP
/* ============================================================================
'pixel.h' defines the pixel classes.  Non-inline methods and space
for static class members are defined in 'pixel.cpp'.

Written by Nicholas Phillips, December 2006
Adapted for WMAP.  FITS and copy operator added.  Michael R. Greason, ADNET,
	27 December 2006.
Broken out of 'skymap.h'.  MRG, ADNET, 23 January 2007.
Polarization magnitude and angle support.  MRG, ADNET, 30 August 2007.
============================================================================ */
#include "map_exception.h"
/* ============================================================================
The BasePixel class provides a common denominator for the various types of
supported maps.  Up to six data elements per pixel are supported:
Stokes I, Q, and U, NObs, Pmag and Pang.

The contents may be accessed through the T/I, Q, U, Nobs, Pmag, and Pang
functions; these functions may be used on either side of the equals sign.
An exception is thrown if an undefined value is accessed.
============================================================================ */
class BasePixel
{
	protected:
		static double ddummy;
	public:
		BasePixel(){};
		virtual ~BasePixel() {};
		virtual double   T()    const;
		virtual double & T()         ;
		virtual double   I()    const;
		virtual double & I()         ;
		virtual double   Q()    const;
		virtual double & Q()         ;
		virtual double   U()    const;
		virtual double & U()         ;
		virtual double   Nobs() const;
		virtual double & Nobs()      ;
		virtual double   Pmag() const;
		virtual double & Pmag()      ;
		virtual double   Pang() const;
		virtual double & Pang()      ;
		
		virtual void copy (BasePixel& src);
		virtual void clear (void);
		
		BasePixel& operator= (BasePixel& src);
		double operator[](unsigned int i) const;
		double & operator[](unsigned int i);
		virtual unsigned int maxIndex (void) const;
		virtual void computePolar (void) { return; }
};
/* ----------------------------------------------------------------------------
'operator=' copies another pixel into this one.

Arguments:
	&src  - The pixel to copy.

Returned:
	*this - A reference to this pixel.
---------------------------------------------------------------------------- */
inline BasePixel& BasePixel::operator= (BasePixel& src)
{
	copy(src);
	return *this;
}
/* ----------------------------------------------------------------------------
'operator[]' allows the pixel to be indexed as an array.

Arguments:
	i  -  The index into the array:
			0 - Stokes I
			1 - Stokes Q
			2 - Stokes U
			3 - Nobs
			4 - Polarization magnitude
			5 - Polarization angle wrt the meridian

Returned:
	The associated value.  0 is returned if the pixel doesn't support the
	requested information; an exception is also thrown.  An exception is also
	thrown if the index is out of bounds.
---------------------------------------------------------------------------- */
inline double BasePixel::operator[](unsigned int i) const
{
	double rv = 0;
	switch( i ){
		case 0:
			rv = T();
			break;
		case 1:
			rv = Q();
			break;
		case 2: 
			rv = U();
			break;
		case 3: 
			rv = Nobs();
			break;
		case 4: 
			rv = Pmag();
			break;
		case 5: 
			rv = Pang();
			break;
		default:
			throw MapException(MapException::Undefined);
	}
	return rv;
}
inline double & BasePixel::operator[](unsigned int i)
{
	switch( i ){
		case 0:
			return T();
			break;
		case 1:
			return Q();
			break;
		case 2: 
			return U();
			break;
		case 3: 
			return Nobs();
			break;
		case 4: 
			return Pmag();
			break;
		case 5: 
			return Pang();
			break;
		default:
			throw MapException(MapException::Undefined);
	}
	return ddummy;
}
/* ----------------------------------------------------------------------------
'maxIndex' returns the maximum index number allowed by the pixel class when
indexing into it as an array.

Arguments:
	None.

Returned:
	The maximum index allowed by the pixel class.
---------------------------------------------------------------------------- */
inline unsigned int BasePixel::maxIndex(void) const
{
	throw MapException(MapException::Undefined);
	return 0;
}
/* ============================================================================
The TPixel class defines a temperature/Stokes I-only pixel.
============================================================================ */
class TPixel : public BasePixel
{
	protected:
		double T_;
	public:
		TPixel() : T_(0) {};		
		virtual ~TPixel() {};
		virtual double   T() const { return T_; };
		virtual double & T()       { return T_; };
		virtual double   I() const { return T_; };
		virtual double & I()       { return T_; };
		virtual unsigned int maxIndex (void) const { return 0; }
};
/* ============================================================================
The TPPixel class defines a Stokes I, Q, U pixel.  The polarization magnitude
and angle will be computed whenever Q and U are set.
============================================================================ */
class TPPixel : public TPixel
{
	protected:
		double Q_;
		double U_;
		double Pmag_;
		double Pang_;
	public:
		TPPixel() : TPixel(), Q_(0), U_(0), Pmag_(0), Pang_(0) {};
		virtual ~TPPixel() {};
		virtual double   Q()    const { return Q_;    };
		virtual double & Q()          { return Q_;    };
		virtual double   U()    const { return U_;    };
		virtual double & U()          { return U_;    };
		virtual double   Pmag() const { return Pmag_; };
		virtual double & Pmag()       { return Pmag_; };
		virtual double   Pang() const { return Pang_; };
		virtual double & Pang()       { return Pang_; };
		virtual unsigned int maxIndex (void) const { return 5; }
		virtual void computePolar (void);
};
/* ============================================================================
The TnobsPixel class defines a temperature/Stokes I-only pixel with an nobs 
component.
============================================================================ */
class TnobsPixel : public TPixel
{
	protected:
		double nobs_;
	public:
		TnobsPixel() : TPixel(), nobs_(0) {};		
		virtual ~TnobsPixel() {};
		virtual double    Nobs() const { return nobs_; };
		virtual double  & Nobs()       { return nobs_; };
		virtual unsigned int maxIndex (void) const { return 2; }
};
/* ============================================================================
The TPnobsPixel class defines a Stokes I, Q, U pixel with an nobs component.
============================================================================ */
class TPnobsPixel : public TPPixel
{
	protected:
		double nobs_;
	public:
		TPnobsPixel() : TPPixel(), nobs_(0) {};	
		virtual ~TPnobsPixel() {};
		virtual double    Nobs() const { return nobs_; };
		virtual double  & Nobs()       { return nobs_; };
		virtual unsigned int maxIndex (void) const { return 6; }
};
#endif
