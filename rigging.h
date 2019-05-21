#ifndef RIGGING_H
#define RIGGING_H
/* ============================================================================
'rigging.h' defines the rigging of the viewer.

Written by Nicholas Phillips.
QT4 adaption by Michael R. Greason, ADNET, 27 August 2007
============================================================================ */
/*
			Fetch header files.
*/
#include <vector>
#include <QGLViewer/vec.h>
#include "face.h"
/* ============================================================================
'Rigging' manages the faces on the sky.
============================================================================ */
class Rigging
{
protected:
	bool viewmoll;
	int  nside;
	std::vector<Face> faces;
	typedef std::vector<Face>::iterator FI;
	std::vector<double> costhetas_np;	//!< theta values for North Pole Faces
	std::vector<double> costhetas_eq;	//!< theta values for Equatorial Pole Faces
	std::vector<double> costhetas_sp;	//!< theta values for South Pole Faces

	void drawFace(const int face);
	void setThetas();
public:
	Rigging();
	~Rigging();
	
	void draw();

	void generate(int ns, bool mp, double rad = 1.);

	bool mollweide (void) const;
	bool projectSelection (const qglviewer::Vec &o, const qglviewer::Vec &d,
		qglviewer::Vec &v) const;
	bool projectSelection (const qglviewer::Vec &o, const qglviewer::Vec &d,
		double &phi, double &lambda) const;
}; 
/* ----------------------------------------------------------------------------
'mollweide' indicates whether or not the rigging is configurated for a 
Mollweide projection.

Arguments:
	None.

Returned:
	viewmoll - The results of the test:  true if the current projection is
	           the Mollweide projection.

Written by Nicholas Phillips.
---------------------------------------------------------------------------- */
inline bool Rigging::mollweide (void) const
{
	return viewmoll;
}
#endif
