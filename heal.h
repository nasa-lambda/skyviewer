#ifndef HEAL_H
#define HEAL_H
/* ====================================================================================
HEALPix-related local routine declarations.
==================================================================================== */
extern "C"
{
#include <chealpix.h>
};

long xy2pix(long ix, long iy);
double toMollweide(const double phi, const double lambda, double &x, double &y);
double fromMollweide(const double x, const double y, double &phi, double &lambda);
#endif

