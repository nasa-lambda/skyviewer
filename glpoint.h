#ifndef GLPOINT_H
#define GLPOINT_H
/* ============================================================================
'glpoint.h' defines a class used to maintain vertices and associated texture
coordinates.

Written by Nicholas Phillips.
QT4 adaption by Michael R. Greason, ADNET, 27 August 2007.
============================================================================ */
/*
			Fetch header files.
*/
#include <vector>
#include <QGLViewer/qglviewer.h>

using namespace std;
/* ============================================================================
'GLPoint' maintains the an OpenGL vertex and its associated texture coordinates.
Note:  it is defined entirely inline.
============================================================================ */
class GLPoint 
{
public:
	float x,y,z;
	float s,t;
public:
	GLPoint(void);
	void setVertC(double x_, double y_, double z_);
	void setVertS(double theta, double phi, double rad = 1.);
	void setTex(double s_, double t_);
	void glSet();
};
/* ----------------------------------------------------------------------------
'GLPoint' defines an empty instance.

Arguments:
	None.

Returned:
	Nothing.

Written by Nicholas Phillips.
QT4 adaption by Michael R. Greason, ADNET, 27 August 2007.
---------------------------------------------------------------------------- */
inline GLPoint::GLPoint() : x(0.), y(0.), z(0.), s(0.), t(0.)
{ 
}
/* ----------------------------------------------------------------------------
'setVertC' defines a vertex's 3D coordinates.

Arguments:
	x,y,z - Its 3D world cartesian coordinates.

Returned:
	Nothing.

Written by Nicholas Phillips.
QT4 adaption by Michael R. Greason, ADNET, 27 August 2007.
---------------------------------------------------------------------------- */
inline void GLPoint::setVertC(double x_, double y_, double z_) 
{ 
	x = x_; 
	y = y_; 
	z = z_;
}
/* ----------------------------------------------------------------------------
'setVertS' defines a vertex's 3D coordinates.

Arguments:
	theta - The colatitude, measured from the North Pole in radians.
	phi   - The longitude in radians.
	rad   - The radius of the sphere.  Defaults to 1.

Returned:
	Nothing.

Written by Nicholas Phillips.
QT4 adaption by Michael R. Greason, ADNET, 27 August 2007.
---------------------------------------------------------------------------- */
inline void GLPoint::setVertS(double theta, double phi, double rad) 
{ 
	x = rad * cos(phi) * sin(theta); 
	y = rad * sin(phi) * sin(theta); 
	z = rad * cos(theta);
}
/* ----------------------------------------------------------------------------
'setTex' defines a vertex's texture coordinates.

Arguments:
	s,t - The texture coordinates.

Returned:
	Nothing.

Written by Nicholas Phillips.
QT4 adaption by Michael R. Greason, ADNET, 27 August 2007.
---------------------------------------------------------------------------- */
inline void GLPoint::setTex(double s_, double t_) 
{ 
	s = s_; 
	t = t_; 
}
/* ----------------------------------------------------------------------------
'glSet' inserts the vertex into the OpenGL system.

Arguments:
	None.

Returned:
	Nothing.

Written by Nicholas Phillips.
QT4 adaption by Michael R. Greason, ADNET, 27 August 2007.
---------------------------------------------------------------------------- */
inline void GLPoint::glSet()
{ 
	glTexCoord2f( s,t); 
	glVertex3f( x, y, z); 
}
/* ============================================================================
Associated typedefs.
============================================================================ */
typedef std::vector<GLPoint> GLVertices;
typedef GLVertices::iterator GLVertI;

typedef std::vector<GLVertices> GLVertVec;
typedef GLVertVec::iterator GLVertVI;

typedef GLVertVec GLVertList;
typedef GLVertVI GLVertLI;
#endif
