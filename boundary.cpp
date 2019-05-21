/* ============================================================================
'boundary.cpp' defines a boundary class.

Written by Nicholas Phillips.
QT4 adaption by Michael R. Greason, ADNET, 27 August 2007
============================================================================ */
/*
			Fetch header files.
*/
#include <math.h>
#include "boundary.h"
#include "map_exception.h"

using namespace std;
/* ----------------------------------------------------------------------------
'Boundary' is the class constructor.

Arguments:
	None. 

Returned:
	Nothing.

Written by Nicholas Phillips.
QT4 adaption by Michael R. Greason, ADNET, 27 August 2007
---------------------------------------------------------------------------- */
Boundary::Boundary(void)
{
	a = b = 0.;
	face = 0;
 	eq = true;
	return;
}
/* ----------------------------------------------------------------------------
'set_eq' defines an equatorial boundary.

Arguments:
	z         - 
	phi       - 
	direction - 

Returned:
	Nothing.

Written by Nicholas Phillips.
QT4 adaption by Michael R. Greason, ADNET, 27 August 2007
---------------------------------------------------------------------------- */
void Boundary::set_eq(const double z, const double phi, const int direction)
{
	b = direction*0.375*M_PI;
	a = phi - b*z;
 	eq = true;
	return;
}
/* ----------------------------------------------------------------------------
'set_np' defines a north pole boundary.

Arguments:
	z     - 
	phi   - 
	face_ - 

Returned:
	Nothing.

Written by Nicholas Phillips.
QT4 adaption by Michael R. Greason, ADNET, 27 August 2007
---------------------------------------------------------------------------- */
void Boundary::set_np(const double z, const double phi, const int face_)
{
 	eq = false;
	face = face_;
	a = sqrt(1-z)*(0.5*M_PI*(face+1) - phi);
	return;
}
/* ----------------------------------------------------------------------------
'set_sp' defines a south pole boundary.

Arguments:
	z     - 
	phi   - 
	face_ - 

Returned:
	Nothing.

Written by Nicholas Phillips.
QT4 adaption by Michael R. Greason, ADNET, 27 August 2007
---------------------------------------------------------------------------- */
void Boundary::set_sp(const double z, const double phi, const int face_)
{
 	eq = false;
	face = face_;
	a = sqrt(1+z)*(phi - 0.5*M_PI*(face-8));
	return;
}
/* ----------------------------------------------------------------------------
'operator()' computes phi for a given z.

Arguments:
	z   - 

Returned:
	phi - 

Written by Nicholas Phillips.
QT4 adaption by Michael R. Greason, ADNET, 27 August 2007
---------------------------------------------------------------------------- */
double Boundary::operator()(const double z) const
{
	double phi;
	if( eq )
		phi = a + b*z;
	else if( face < 4) {
		phi = 0.5*M_PI*(face+1);
		if( z < 1 )
			phi += -a/sqrt(1-z);
	}
	else if( face > 7) {
		phi = 0.5*M_PI*(face-8);
		if( z > -1 )
			phi += a/sqrt(1+z);
	}
	else {
		phi = 0;
		throw MapException(MapException::Other, 0, 
			"Boundary::operator():Bad call");
	}
	return phi;
}

