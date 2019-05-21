/* ============================================================================
'rigging.cpp' defines the rigging of the viewer.

Written by Nicholas Phillips.
QT4 adaption by Michael R. Greason, ADNET, 27 August 2007
============================================================================ */
/*
			Fetch header files.
*/
#include <qdebug.h>
#include <math.h>
#include "rigging.h"
#include "heal.h"

using namespace std;
using namespace qglviewer;
/* ============================================================================
'Rigging' manages the faces on the sky.
============================================================================ */
/* ----------------------------------------------------------------------------
'Rigging' the class constuctor, it creates the faces.

Arguments:
	None.

Written by Nicholas Phillips.
---------------------------------------------------------------------------- */
Rigging::Rigging()
{
	viewmoll = false;
	faces.resize(12);
	for(int face = 0; face < 12; face++)
		faces[face].faceNumber(face);
	return;
}
/* ----------------------------------------------------------------------------
'Rigging' the class destuctor.

Arguments:
	None.

Written by Nicholas Phillips.
---------------------------------------------------------------------------- */
Rigging::~Rigging()
{
}
/* ----------------------------------------------------------------------------
'generate' creates the rigging given the size of the map being supported and
whether or not the map should be displayed as the sphere or as a mollweide
projection.

Arguments:
	ns  - The nside of the map.
	mp  - Mollweide projection?
	rad - The radius of the 'sphere' the map should be project onto.  Defaults
	      to 1.

Returned:
	Nothing.

Written by Nicholas Phillips.
---------------------------------------------------------------------------- */
void Rigging::generate(int ns, bool mp, double rad)
{
	viewmoll = mp;
	nside = ns;
	setThetas();
	for(int i = 0; i < 4; i++)
		faces[i].setRigging(nside,costhetas_np,mp,rad);
	for(int i = 4; i < 8; i++)
		faces[i].setRigging(nside,costhetas_eq,mp,rad);
	for(int i = 8; i < 12; i++)
		faces[i].setRigging(nside,costhetas_sp,mp,rad);
	return;
}
/* ----------------------------------------------------------------------------
'draw' paints all the faces.

Arguments:
	None.

Returned:
	Nothing.

Written by Nicholas Phillips.
---------------------------------------------------------------------------- */
void Rigging::draw()
{
	for(int i = 0; i < 12; i++)
		faces[i].draw();
	return;
}
/* ----------------------------------------------------------------------------
'setThetas' computes the theta values for all the faces.

Arguments:
	None.

Returned:
	Nothing.

Written by Nicholas Phillips.
---------------------------------------------------------------------------- */
void Rigging::setThetas()
{
	std::vector<double> thetas_np;	//!< theta values for North Pole Faces
	std::vector<double> thetas_eq;	//!< theta values for Equatorial Pole Faces
	std::vector<double> thetas_sp;	//!< theta values for South Pole Faces
	double theta,phi,costheta;
	int pix = 0;
	int dpix = 4;
	int npix = nside2npix(nside);
	const double eps = 0.128/double(nside);
	thetas_np.push_back(0.0);
	while( pix < npix ){
		pix2ang_ring(nside, pix,&theta,&phi);
		costheta = cos(theta);
		if( costheta > -eps )
			thetas_np.push_back(theta);
		if( fabs(costheta) <= (2./3.+eps) )
			thetas_eq.push_back(theta);
		if( costheta < eps )
			thetas_sp.push_back(theta);

		pix += dpix;
		if( costheta > 2./3.) {
			dpix += 4;
		}
		else if( costheta < -2./3. ) {
			dpix -= 4;
		}

	}
	thetas_sp.push_back(M_PI);

	unsigned int n = thetas_np.size();
	costhetas_np.resize(n);
	costhetas_eq.resize(n);
	costhetas_sp.resize(n);

	for(unsigned int i = 0; i < n; i++) {
		costhetas_np[i] = cos(thetas_np[n-i-1]);
		costhetas_eq[i] = cos(thetas_eq[n-i-1]);
		costhetas_sp[i] = cos(thetas_sp[n-i-1]);
	}
	return;
}
/* ----------------------------------------------------------------------------
'projectSelection' converts a selected point, identified by the vectors
returned by the QGLViewer convertClickToLine method, into a vector on the
projected surface.

Arguments:
	o - The location of the observer.
	d - A unit vector indicating the direction of the clicked pixel from
	    the observer's position.
	v - The projected vector.

Returned:
	flg - Was a pixel successfully found?

Written by Nicholas Phillips.
QR4 implementation.  Michael R. Greason, ADNET, 31 August 2007.
Mollweide support.  MRG, ADNET, 21 September 2007.
Discards out-of-bounds selections in the Mollweide projection.  MRG, ADNET,
	29 December 2008.
---------------------------------------------------------------------------- */
bool Rigging::projectSelection (const Vec &o, const Vec &d, Vec &v) const
{
	const float srad2 = 1.;						// Radius^2 of the 3D sphere.
	const float mdist = 0.; 					// Mollweide distance from origin.
	float t;
/*
			3D spherical projection.
*/
	if (! mollweide())
	{
		float sqrtterm, t1, t2, oo, od, dd;
		oo = o * o;
		od = o * d;
		dd = d * d;
		sqrtterm = (od * od) - (dd * (oo - srad2));
		if (sqrtterm < 0) return false;
		t1 = (-od - sqrt(sqrtterm))/(dd);
		t2 = (-od + sqrt(sqrtterm))/(dd);
		t = (t1 < t2) ? t1 : t2;
		v = o + t*d;
	}
/*
			Mollweide projection.
*/
	else
	{
		const float w = 2.0;
		const float h = 1.0;
		float l, dy, dz;
		t = (mdist - o.x) / d.x;
		v = o + t*d;
		dy = fabs(v.y);
		dz = fabs(v.z);
		if (dy >= w) return false;
		l = sqrt((1.0 - ((dy * dy) / (w * w))) * (h * h));
		if (dz >= l) return false;
	}
	return true;
}
/* ----------------------------------------------------------------------------
'projectSelection' converts a selected point, identified by the vectors
returned by the QGLViewer convertClickToLine method, into sky coordinates.

Arguments:
	o      - The location of the observer.
	d      - A unit vector indicating the direction of the clicked pixel from
	         the observer's position.
	phi    - The colatitude of the pixel.
	lambda - The longitude of the pixel.

Returned:
	flg - Was a pixel successful found?

Written by Nicholas Phillips.
QR4 implementation.  Michael R. Greason, ADNET, 31 August 2007.
Mollweide support.  MRG, ADNET, 21 September 2007.
---------------------------------------------------------------------------- */
bool Rigging::projectSelection (const Vec &o, const Vec &d, 
	double &phi, double &lambda) const
{
	const double r2 = sqrt(2.);
	Vec v;
	lambda = phi = 0.;
/*
			Determine where the vector intersects the projection in
			world coordinates.
*/
	if (! projectSelection(o, d, v)) return false;
/*
			3D spherical projection.
*/
	if (! mollweide())
	{
		phi    = acos(v.z);
		lambda = atan2(v.y, v.x);
	}
/*
			Mollweide projection.
*/
	else
	{
		v.y *= r2;
		v.z *= r2;
		try {
			fromMollweide(v.y, v.z, phi, lambda);
		}
		catch (...) { return false; }
	}
	return true;
}
