/* ============================================================================
'polarargline.cpp' defines the polarization vectors.

Written by Nicholas Phillips.
QT4 adaption by Michael R. Greason, ADNET, 27 August 2007
============================================================================ */
/*
			Fetch header files.
*/
#include "polarargline.h"
#include "heal.h"

using namespace std;
using namespace qglviewer;

const double boost = 0.001; 	// Distance to assign to a vector to get it
								// above the sphere.
/* ----------------------------------------------------------------------------
'spinVector' computes the spin vector of a vector given a base vector and
a rotation angle.

Arguments:
	v0   - The base vector.
	vin  - The other vector
	psi  - A rotation angle

Returned:
	vout - The spin vector.

Written by Nicholas Phillips.
---------------------------------------------------------------------------- */
Vec spinVector(const Vec &v0, const Vec &vin, double psi)
{
	double e1,e2,e3;
	double A[3][3];

	e1 = v0[0];	e2 = v0[1];	e3 = v0[2];
	double cosp = cos(psi);
	double sinp = sin(psi);

	A[0][0] = cosp + e1*e1*(1-cosp); 	A[0][1] = e1*e2*(1-cosp)+e3*sinp;	A[0][2] = e1*e3*(1-cosp)-e2*sinp;
	A[1][0] = e1*e2*(1-cosp)-e3*sinp;	A[1][1] = cosp+e2*e2*(1-cosp);		A[1][2] = e2*e3*(1-cosp)+e1*sinp;
	A[2][0] = e1*e3*(1-cosp)+e2*sinp;	A[2][1] = e2*e3*(1-cosp)-e1*sinp;	A[2][2] = cosp+e3*e3*(1-cosp);

	Vec vout;
	for(int i = 0; i < 3; i++) {
		vout[i] = 0;
		for(int j = 0; j < 3; j++)
			vout[i] += A[i][j]*vin[j];
	}
	return vout;
}
/* ----------------------------------------------------------------------------
'toMollweide' converts a vector from a spherical to Mollweide projection.

Arguments:
	v   - The vector to transform.  Assume that it is a unit vector.
	hgt - The height above the screen/plane to assign to the new vector.

Returned:
	t   - The transformed vector.

Written by Nicholas Phillips.
---------------------------------------------------------------------------- */
Vec toMollweide (Vec v, double hgt)
{
	Vec t;
	double rad, phi, lambda, x, y;
	rad = sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
	phi    = asin(v.z / rad);
	lambda = atan2(v.y, v.x);
	toMollweide(phi, lambda, x, y);
	t.x = hgt;
	t.y = x / sqrt(2.);
	t.z = y / sqrt(2.);
	return t;
}
/* ============================================================================
'PolarArgLine' manages a single polarization angle vector on the viewer.
============================================================================ */
/* ----------------------------------------------------------------------------
'PolarArgLine' is the class constructor.

Arguments:
	None.

Written by Nicholas Phillips.
---------------------------------------------------------------------------- */
PolarArgLine::PolarArgLine (void)
{
	on   = false;
	moll = false;
}
/* ----------------------------------------------------------------------------
'set' defines the vector.

Arguments:
	theta - The colatitude in radians measured southward from the north pole
			(0--PI).
	phi   - The longitude in radians measured eastward from the X-axis
			(0--2PI).
	gamma - The polarization angle, in radians, from the meridian.
	size  - Essentially theta for pixel 0 over two--a scaling factor.

Returned:
	Nothing.

Written by Nicholas Phillips.
---------------------------------------------------------------------------- */
void PolarArgLine::set(double theta, double phi, double gamma, double size)
{
/*
			Compute the vector we will rotate about: the pixel center
*/
	Vec v0(cos(phi)*sin(theta),sin(phi)*sin(theta),cos(theta));
/*
			Compute the vector we will rotate: points to the North Pole, Q=1, U=0
*/
	theta -= M_PI/2;
	Vec vin(cos(phi)*sin(theta),sin(phi)*sin(theta),cos(theta));
/*
			Compute the end points: either end of a headless (spin-2) vector
*/
	v1 = (1+boost)*v0 + size*spinVector(v0,vin,gamma);
	v2 = (1+boost)*v0 + size*spinVector(v0,vin,gamma+M_PI);
/*
			Done!
*/
	setColor();
	setOn(true);
	return;
}
/* ----------------------------------------------------------------------------
'setColor' assigns a color to the vector.

Arguments:
	r - The red component of the color.
	g - The green component of the color.
	b - The blue component of the color.

Returned:
	Nothing.

Written by Nicholas Phillips.
---------------------------------------------------------------------------- */
void PolarArgLine::setColor(uchar r, uchar g, uchar b)
{
	col.setValue(r/255.,g/255.,b/255.);
	return;
}
/* ----------------------------------------------------------------------------
'draw' draws the vector on the viewer.

Arguments:
	None.

Returned:
	Nothing.

Written by Nicholas Phillips.
---------------------------------------------------------------------------- */
void PolarArgLine::draw()
{
	Vec a, b;
	if (moll)
	{
		double l;
		a = toMollweide(v1, boost);
		b = toMollweide(v2, boost);
		l = sqrt(((a.x - b.x) * (a.x - b.x))
		       + ((a.y - b.y) * (a.y - b.y))
		       + ((a.z - b.z) * (a.z - b.z)));
		if (l > 2.2) return;
	}
	else
	{
		a = v1;
		b = v2;
	}

	glColor3fv(col);
	glBegin(GL_LINES);
	glVertex3fv(a);
	glVertex3fv(b);
	glEnd();

	return;
}
/* ============================================================================
'PolarArgLineSet' manages a set of polarization angle vectors.
============================================================================ */
/* ----------------------------------------------------------------------------
'PolarArgLineSet' is the class constructor.

Arguments:
	None.

Written by Nicholas Phillips.
---------------------------------------------------------------------------- */
PolarArgLineSet::PolarArgLineSet (void)
{
	on   = false;
}
/* ----------------------------------------------------------------------------
'set' defines the vectors from a sky map.

Arguments:
	skymap - The map, with polarization, to compute the vectors from.

Returned:
	Nothing.

Written by Nicholas Phillips.
---------------------------------------------------------------------------- */
void PolarArgLineSet::set(HealpixMap *skymap)
{
	long   i, nsiz, npix;
	double theta,phi;
	double pixsize;
	if (! (skymap->has_Polarization() && skymap->has_Nobs())) return;
/*
			Start assuming the entire map.  Discard pixels with no observations.
*/
	nsiz = npix = skymap->size();
	for (i = 0; i < nsiz; i++)
	{
		if ((*skymap)[i].Nobs() <= 0) npix--;
	}
	resize(npix);
	if (npix <= 0) return;
/*
			Fill the vector list.
*/
	iterator it = begin();
	pixsize = (sqrt(M_PI / 3.) / skymap->nside()) / 2.;
	for (i = 0; i < nsiz; i++)
	{
		if ((*skymap)[i].Nobs() <= 0) continue;
		skymap->pixel2angles(i, theta, phi);
		it->set(theta, phi, (*skymap)[i].Pang(), pixsize);
		++it;
	}
	return;
}
/* ----------------------------------------------------------------------------
'setMollweide' sets the Mollweide projection flag.

Arguments:
	b - Indicates whether (true) or not (false) to display the vectors in a
	    Mollweide projection.

Returned:
	Nothing.

Written by Michael R. Greason, ADNET, 05 September 2007.
---------------------------------------------------------------------------- */
void PolarArgLineSet::setMollweide (bool b)
{
	for (iterator it = begin(); it != end(); it++)
	{
		it->setMollweide(b);
	}
}
/* ----------------------------------------------------------------------------
'draw' draws the vectors on the viewer.

Arguments:
	None.

Returned:
	Nothing.

Written by Nicholas Phillips.
---------------------------------------------------------------------------- */
void PolarArgLineSet::draw()
{
	if( ! isOn() )
		return;

	for( iterator it = begin(); it != end(); ++it){
		if( it->isOn() )
			it->draw();
	}
	return;
}
