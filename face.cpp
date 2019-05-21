/* ============================================================================
'face.cpp' defines a Face class.

Written by Nicholas Phillips.
QT4 adaption by Michael R. Greason, ADNET, 27 August 2007
============================================================================ */
/*
			Fetch header files.
*/
#include "face.h"
#include "debug.h"
#include "boundary.h"
#include "heal.h"
#include "map_exception.h"

using namespace std;
/* ----------------------------------------------------------------------------
'zero' performs a zero check on a floating point value.

Arguments:
	x - The value to test.

Returned:
	res - The result.

Written by Nicholas Phillips.
---------------------------------------------------------------------------- */
inline float zero(float x)
{
	return fabs(x) < 1e-8 ? 0 : x;
}
/* ----------------------------------------------------------------------------
'min' returns the minimum of two values.

Arguments:
	a,b - The two values.

Returned:
	res - The result.

Written by Nicholas Phillips.
---------------------------------------------------------------------------- */
inline unsigned int min(const uint a, const uint b)
{
	return a < b ? a : b;
}
/* ----------------------------------------------------------------------------
'setRigging_NP' computes the cosine thetas (rigging) for a north-pole face.

Arguments:
	nside     - The size of the map.
	costhetas - The cosine thetas.
	rad       - The radius of the projection.  Defaults to 1.

Returned:
	Nothing.

Written by Nicholas Phillips.
---------------------------------------------------------------------------- */
void Face::setRigging_NP(const int nside, vector<double> &costhetas, double rad)
{
	double ds = 1./nside;
	double s = 0, t = 0;
	Boundary lower;
	Boundary boundry0;
	Boundary boundry1;

	quadList.resize(nside);
	GLVertVI qli = quadList.begin();
	lower.set_eq( 0, 0.5*M_PI*(face+0.5), 1);
	for(int i = 0; i < nside; i++) {
		t = 0;
		qli->resize(2*(nside+1));
		GLVertI pti = qli->begin();
		bool boundry0_reset = false;
		bool boundry1_reset = false;
		boundry0.set_eq(costhetas[i],   lower(costhetas[i]),   -1);
		boundry1.set_eq(costhetas[i+1], lower(costhetas[i+1]), -1);
		for(int j = i; j <= i+nside; j++) {
			pti->setVertS(acos(costhetas[j]),boundry0(costhetas[j]),rad);
			pti->setTex(s,t);
			++pti;

			pti->setVertS(acos(costhetas[j+1]),boundry1(costhetas[j+1]),rad);
			pti->setTex(s+ds,t);
			++pti;

			if( (costhetas[j] >= 2./3.) && ! boundry0_reset) {
				boundry0.set_np(costhetas[j],boundry0(costhetas[j]),face);
				boundry0_reset = true;
			}

			if( (costhetas[j+1 ]>= 2./3.) && ! boundry1_reset) {
				boundry1.set_np(costhetas[j+1],boundry1(costhetas[j+1]),face);
				boundry1_reset = true;
			}

			t += ds;
		}
		s += ds;
		++qli;
	}
	return;
}
/* ----------------------------------------------------------------------------
'setRigging_SP' computes the cosine thetas (rigging) for a south-pole face.

Arguments:
	nside     - The size of the map.
	costhetas - The cosine thetas.
	rad       - The radius of the projection.  Defaults to 1.

Returned:
	Nothing.

Written by Nicholas Phillips.
---------------------------------------------------------------------------- */
void Face::setRigging_SP(const int nside, vector<double> &costhetas, double rad)
{
	double ds = 1./nside;
	double s = 0, t = 0;
	double phi_boundry = 0.5*M_PI*(face-7);
	Boundary boundry0;
	Boundary boundry1;

	quadList.resize(nside);
	GLVertVI qli = quadList.begin();
	for(int i = 0; i < nside; i++) {
		t = 0;
		qli->resize(2*(nside+1));
		GLVertI pti = qli->begin();
		bool boundry0_reset = false;
		bool boundry1_reset = false;
		boundry0.set_sp(costhetas[i],   phi_boundry, face);
		boundry1.set_sp(costhetas[i+1], phi_boundry, face);
		for(int j = i; j <= i+nside; j++) {
			pti->setVertS(acos(costhetas[j]),boundry0(costhetas[j]),rad);
			pti->setTex(s,t);
			++pti;

			pti->setVertS(acos(costhetas[j+1]),boundry1(costhetas[j+1]),rad);
			pti->setTex(s+ds,t);
			++pti;

			if( (costhetas[j] > (-2./3. - 0.001)) && ! boundry0_reset) {
				boundry0.set_eq(costhetas[j],boundry0(costhetas[j]),-1);
				boundry0_reset = true;
			}
			if( (costhetas[j+1] > (-2./3. - 0.001)) && ! boundry1_reset) {
				boundry1.set_eq(costhetas[j+1],boundry1(costhetas[j+1]),-1);
				boundry1_reset = true;
			}

			t += ds;
		}
		s += ds;
		++qli;
	}
	return;
}
/* ----------------------------------------------------------------------------
'setRigging_EQ' computes the cosine thetas (rigging) for an equatorial face.

Arguments:
	nside     - The size of the map.
	costhetas - The cosine thetas.
	rad       - The radius of the projection.  Defaults to 1.

Returned:
	Nothing.

Written by Nicholas Phillips.
---------------------------------------------------------------------------- */
void Face::setRigging_EQ(const int nside, vector<double> &costhetas, double rad)
{
	double ds = 1./nside;
	double s = 0, t = 0;
	Boundary lower;
	lower.set_eq(-2./3., 0.5*M_PI*(face-4), 1);
	Boundary boundry0;
	Boundary boundry1;

	quadList.resize(nside);
	GLVertVI qli = quadList.begin();
	for(int i = 0; i < nside; i++) {
		t = 0;
		qli->resize(2*(nside+1));
		GLVertI pti = qli->begin();
		boundry0.set_eq(costhetas[i],   lower(costhetas[i]),   -1);
		boundry1.set_eq(costhetas[i+1], lower(costhetas[i+1]), -1);
		for(int j = i; j <= i+nside; j++) {
			pti->setVertS(acos(costhetas[j]),boundry0(costhetas[j]),rad);
			pti->setTex(s,t);
			++pti;

			pti->setVertS(acos(costhetas[j+1]),boundry1(costhetas[j+1]),rad);
			pti->setTex(s+ds,t);
			++pti;

			t += ds;
		}
		s += ds;
		++qli;
	}
	return;
}
/* ----------------------------------------------------------------------------
'setRigging' computes the cosine thetas (rigging) for a face.

Arguments:
	nside     - The size of the map.
	costhetas - The cosine thetas.
	viewmoll  - Projection to use:  true=mollweide, false=3D sphere.
	rad       - The radius of the projection.  Defaults to 1.

Returned:
	Nothing.

Written by Nicholas Phillips.
---------------------------------------------------------------------------- */
void Face::setRigging(const int nside, vector<double> &costhetas,
	bool viewmoll, double rad)
{
	double radius = viewmoll ? 1. : rad;
	if( face < 4)
		setRigging_NP(nside,costhetas,radius);
	else if( face > 7)
		setRigging_SP(nside,costhetas,radius);
	else
		setRigging_EQ(nside,costhetas,radius);
	rigging_set = true;

	GLVertVI qli = quadList.begin();
	for(int i = 0; i < nside; i++) {
		GLVertI pti = qli->begin();
		GLVertI pti_end = qli->end();
		for(; pti != pti_end; ++pti) {
			pti->s = 0.25*pti->s + 0.25*(face % 4);
			pti->t = 0.25*pti->t + 0.25*(face / 4);
		}
		++qli;
	}

	if (viewmoll || showmoll) toMollweide(rad);
	
	return;
}
/* ----------------------------------------------------------------------------
'draw' paints the face.

Arguments:
	None.

Returned:
	Nothing.

Written by Nicholas Phillips.
---------------------------------------------------------------------------- */
void Face::draw()
{
	if (! rigging_set) {
		throw (MapException(MapException::Other, 0,
			"Face::draw:Rigging not set, face = "));;
	}
	
	if( showface6 && face != 6 ) return;
	
	QColor color;
	GLVertVI qli = quadList.begin();
	GLVertVI qli_end = quadList.end();
	GLVertI pti;
	GLVertI pti_end;
	
	unsigned int list = 0;
	for(; qli !=  qli_end; ++qli) {
		pti_end = qli->end();
		
		if( true || (list==(quadList.size()/2)) ) {
			
			if( showrigging ) {
				glColor3f(1.0, 1.0, 1.0);
				glLineWidth(1.0);
				glBegin(GL_LINE_STRIP);
				for(pti = qli->begin(); pti != pti_end; ++pti) {
					pti->glSet();
				}
				
				glEnd();
			}
			if( showrigging ) {
				glLineWidth(3.0);
				glBegin(GL_LINE_STRIP);
			}
			else
				glBegin(GL_QUAD_STRIP);
			
			int pixel = 0;
			for(pti = qli->begin(); pti != pti_end; ++pti) {
				
				if( showrigging ) {
					color.setHsv((int)(360*pixel/(qli->size()-1.)), 255, 255);
					glColor3f(color.red()/255.,color.green()/255.,color.blue()/255.);
				}
				pti->glSet();
				pixel++;
			}
			glEnd();
			
		}
		list++;
	}
	return;
}
/* ----------------------------------------------------------------------------
'toMollweide' computes the vertices assuming a Molleweide projection.  If
the supplied radius is <1 then push the projection out of the screen so that
it lies behind a projection with radius 1, when viewed from x < 0.

Arguments:
	rad - The radius of the projection.  Defaults to 1.

Returned:
	Nothing.

Written by Nicholas Phillips.
---------------------------------------------------------------------------- */
void Face::toMollweide(double rad)
{
	double r = 1/sqrt(2.);
	double phi;
	double lambda;
	double x,y;
	
	int i = 0;
	for(GLVertVI qli = quadList.begin(); qli !=  quadList.end(); ++qli) {
		int j = 0;
		for(GLVertI pti = qli->begin(); pti != qli->end(); ++pti) {
			phi = asin(pti->z);
			lambda = atan2(pti->y,pti->x);
			::toMollweide(phi, lambda, x, y);
			if( (face == 2 || face == 10) && x > 0) x *= -1;
			pti->y = r*x;
			pti->z = r*y;
			pti->x = (rad < 1.) ? -(rad - 1.) : 0;
			j++;
		}
		i++;
	}
	if( face == 6 )
		toMollweideBackfaceSplit();

	return;
}
/* ----------------------------------------------------------------------------
'toMollweideBackfaceSplit' computes the vertices assuming a Molleweide
projection in the face that needs to be split.

Arguments:
	None.

Returned:
	Nothing.

Written by Nicholas Phillips.
---------------------------------------------------------------------------- */
void Face::toMollweideBackfaceSplit(void)
{
	if( face != 6 )
		return;
	
	// copy the current list to temp storage
	GLVertVec oldquads;
	oldquads.resize(quadList.size());
	GLVertVI srcqli = quadList.begin();
	GLVertVI destqli = oldquads.begin();
	for(uint i = 0; i < quadList.size(); i++) {
		destqli->resize(srcqli->size());
		GLVertI srcpti = srcqli->begin();
		GLVertI destpti = destqli->begin();
		for( uint j = 0; j < srcqli->size(); j++)
			*destpti++ = *srcpti++;
		srcqli++;
		destqli++;
	}
	
	// Now refill it, making the break along the back as needed
	srcqli = oldquads.begin();
	GLVertVI qli;
	quadList.resize(0);
	uint i = 0;
	for(srcqli = oldquads.begin(); srcqli != oldquads.end(); ++srcqli) {
		i++;
		
		bool patch = true;	// need to patch the end of the left and start of the right sides
		bool leftside = true;	// true while building the left side quadlist
		
		GLVertices newverts1;
		GLVertices newverts2;
		
		uint jbreak = srcqli->size(); // vertex number of switch between left and right side
		jbreak = 2*i-2;
		for(uint j = 0; j < srcqli->size(); j++ ){
			
			GLPoint p = (*srcqli)[j];
			if( leftside ) {
				p.y = -fabs(p.y);
				newverts1.push_back(p);
				leftside = j < jbreak;
			}
			else if( patch ) {
				
				p.y = -fabs(p.y);
				newverts1.push_back(p);
				
				GLPoint p2;
				GLVertVI qli = srcqli;
				qli++;
				if( qli != oldquads.end() ) {
					p2 = (*qli)[j+1];
					p2.y = -fabs(p2.y);
				}
				else {
					p2.y = -1.66597;
					p2.z =  0.553293;
					p2.s = 0.75;
					p2.t = 0.5;
				}
				newverts1.push_back(p2);
				newverts1.push_back(p);
				
				
				// Patch the beginning of the right side of the split face
				p = (*srcqli)[j+1];
				p.y = fabs(p.y);
				if( srcqli != oldquads.begin() ) {
					qli = srcqli;
					qli--;
					p2 = (*qli)[j];
					p2.y = fabs(p2.y);
				}
				else {
					p2.y =  1.66597;
					p2.z = -0.553293;
					p2.s = 0.5;
					p2.t = 0.25;
				}
				newverts2.push_back(p);
				newverts2.push_back(p2);
				
				patch = false;
			}
			else {
				p.y = fabs(p.y);
				newverts2.push_back(p);
			}
			
		}
		quadList.push_back(newverts1);
		quadList.push_back(newverts2);
	}
	return;
}
