#ifndef FACE_H
#define FACE_H
/* ============================================================================
'face.h' defines a Face class.

Written by Nicholas Phillips.
QT4 adaption by Michael R. Greason, ADNET, 27 August 2007
============================================================================ */
/*
			Fetch header files.
*/
#include "glpoint.h"
/* ============================================================================
'Face' maintains the contents of one block of the sky.
============================================================================ */
class Face 
{
private:
	int face;
	bool rigging_set;
	GLVertList quadList;
protected:
	void setRigging_NP(const int nside, std::vector<double> &costhetas, double rad = 1.);
	void setRigging_EQ(const int nside, std::vector<double> &costhetas, double rad = 1.);
	void setRigging_SP(const int nside, std::vector<double> &costhetas, double rad = 1.);
public:
	Face() : face(0), rigging_set(false) {};
	virtual ~Face() {};

	void faceNumber(const int face_) { face = face_; }
	void setRigging(const int nside, std::vector<double> &costhetas,
		bool viewmoll, double rad = 1.);
	void draw();
	void toMollweide(double rad = 1.);
	void toMollweideBackfaceSplit(void);
};
#endif
