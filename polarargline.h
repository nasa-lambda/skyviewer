#ifndef POLARARGLINE_H
#define POLARARGLINE_H
/* ============================================================================
'polarargline.h' defines the polarization angle vectors.

Written by Nicholas Phillips.
QT4 adaption by Michael R. Greason, ADNET, 27 August 2007
============================================================================ */
/*
			Fetch header files.
*/
#include <vector>
#include <QGLViewer/vec.h>
#include "healpixmap.h"
typedef unsigned char uchar;
/* ============================================================================
'PolarArgLine' manages a single polarization angle vector on the viewer.
============================================================================ */
class PolarArgLine
{
protected:
	bool on;
	bool moll;
	qglviewer::Vec v1;
	qglviewer::Vec v2;
	qglviewer::Vec col;
public:
	PolarArgLine (void);
	void set(double theta, double phi, double gamma, double size);
	void setColor(uchar r_ = 255, uchar g_ = 255, uchar b_ = 255);
	void setOn(bool b) { on = b;}
	bool isOn() const { return on; };
	void setMollweide (bool b) { moll = b; }
	void draw();
};
/* ============================================================================
'PolarArgLineSet' manages a set of polarization angle vectors.
============================================================================ */
class  PolarArgLineSet: public std::vector<PolarArgLine>
{
protected:
	bool on;
public:
	PolarArgLineSet (void);
	void set(HealpixMap *skymap);
	void setOn(bool b) { on = b;}
	bool isOn() const { return on; };
	void setMollweide (bool b);
	void draw();
};
#endif
