#ifndef BOUNDARY_H
#define BOUNDARY_H
/* ============================================================================
'boundary.h' defines a boundary class.

Written by Nicholas Phillips.
QT4 adaption by Michael R. Greason, ADNET, 27 August 2007
============================================================================ */
/*
			Fetch header files.
*/
/* ============================================================================
'Boundary' maintains the junctions between faces.
============================================================================ */
class Boundary {
private:
	double a;
	double b;
	bool   eq;
	int    face;
public:
	Boundary();
	virtual ~Boundary() {};
	void set_eq(const double z, const double phi, const int direction);
	void set_np(const double z, const double phi, const int face);
	void set_sp(const double z, const double phi, const int face);
	double operator()(const double z) const;
};
#endif
