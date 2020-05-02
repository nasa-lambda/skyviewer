/* ====================================================================================
HEALPix-related local routines.
==================================================================================== */
/*
			Fetch header files.
*/
#include <math.h>
#include "heal.h"
#include "map_exception.h"

using namespace std;
/*
			Constants.
*/
#define NEED_MKXY2PIX 1
const double lambda0 = 0;	// offset

#ifdef NEED_MKXY2PIX
/* -----------------------------------------------------------------------------
 *
 *  Copyright (C) 1997-2010 Krzysztof M. Gorski, Eric Hivon,
 *                          Benjamin D. Wandelt, Anthony J. Banday, 
 *                          Matthias Bartelmann, 
 *                          Reza Ansari & Kenneth M. Ganga 
 *
 *
 *  This file is part of HEALPix.
 *
 *  HEALPix is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  HEALPix is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with HEALPix; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 *  For more information about HEALPix see http://healpix.jpl.nasa.gov
 *
 *----------------------------------------------------------------------------- */
/* Standard Includes */
#include <math.h>

void mk_xy2pix(int *x2pix, int *y2pix) {
  /* =======================================================================
   * subroutine mk_xy2pix
   * =======================================================================
   * sets the array giving the number of the pixel lying in (x,y)
   * x and y are in {1,128}
   * the pixel number is in {0,128**2-1}
   *
   * if  i-1 = sum_p=0  b_p * 2^p
   * then ix = sum_p=0  b_p * 4^p
   * iy = 2*ix
   * ix + iy in {0, 128**2 -1}
   * =======================================================================
   */
  int i, K,IP,I,J,ID;
  
  for(i = 0; i < 127; i++) x2pix[i] = 0;
  for( I=1;I<=128;I++ ) {
    J  = I-1;//            !pixel numbers
    K  = 0;//
    IP = 1;//
    truc : if( J==0 ) {
      x2pix[I-1] = K;
      y2pix[I-1] = 2*K;
    }
    else {
      ID = (int)fmod(J,2);
      J  = J/2;
      K  = IP*ID+K;
      IP = IP*4;
      goto truc;
    }
  }     
  
}
#endif
/* ----------------------------------------------------------------------------
'xy2pix' converts an x-y coordinate into a pixel number.

Arguments:
	ix,iy - The x-y coordinates.

Returned:
	pix - The pixel number.

Written by Nicholas Phillips.
---------------------------------------------------------------------------- */
long xy2pix(long ix, long iy)
{
	static int x2pix[128];
	static int y2pix[128];
	static bool xy2pix_called = false;
	if( ! xy2pix_called ) {
		mk_xy2pix(x2pix, y2pix);
	xy2pix_called = true;
	}
	//int ix_low = (int)fmod(ix,128);
	int ix_low = ix % 128;
	int ix_hi  = ix / 128;
	int iy_low = iy % 128;
	int iy_hi  = iy / 128;
	return (x2pix[ix_hi]+y2pix[iy_hi]) * (128 * 128)
			+ (x2pix[ix_low]+y2pix[iy_low]);
}
/* ----------------------------------------------------------------------------
'toMollweide' converts phi/lambda into x/y and theta, implementing a conversion
into the Molleweide projection.

Arguments:
	phi,lambda - The input coordinates (colatitude and longitude), in radians.
	x,y        - The output cartesian coordinates.

Returned:
	theta - The auxiliary angle.

Written by Nicholas Phillips.
---------------------------------------------------------------------------- */
double toMollweide(const double phi, const double lambda, double &x, double &y)
{
	const double r2 = sqrt(2.);
	double theta;
	x = phi;
	y = lambda;
/*
			Start by solving 2theta + sin(2theta) == pi sin(phi), for theta

				Upper limit.
*/
	if( fabs(x - M_PI/2) < 1e-6) {
		theta = M_PI/2;
	}
/*
				Lower limit
*/
	else if (fabs(x + M_PI/2) < 1e-6 ) {
		theta = -M_PI/2;
	}
	else {
/*
				Iterative computation using Newton's method.
*/
		const int nmax = 20;		// max number of iterations for Newton's method
		const double eps = 1e-6; 	// how well we solve for theta
		int i;
		double dtheta;
		double f,df;
		theta = x/2;									// The initial guess.
		f = 2*theta + sin(2*theta) - M_PI*sin(phi); 	// Solve for this function.
		for(i = 0; i < nmax; i++) {
			if( fabs(f) < eps)
				break;
			df = 2*(1+cos(2*theta));
			dtheta = -f/df;
			theta += dtheta;
			f = 2*theta + sin(2*theta) - M_PI*sin(x);
		}
	}
/*
			Now that we have the theta, we can compute the cartesian coordinates.
*/
	x = 2. * r2 * (y - lambda0) * cos(theta) / M_PI;
	y = r2 * sin(theta);
	
	return theta;
}
/* ----------------------------------------------------------------------------
'fromMollweide' converts x/y into phi/lambda, implementing a conversion from
the Molleweide projection.

Arguments:
	x,y        - The input cartesian coordinates.
	phi,lambda - The output coordinates (colatitude and longitude), in radians.

Returned:
	theta - The auxiliary angle.

Written by Michael R. Greason, ADNET, 05 September 2007.
---------------------------------------------------------------------------- */
double fromMollweide(const double x, const double y, double &phi, double &lambda)
{
	const double r2 = sqrt(2.);
	const double pi2 = M_PI / 2.;
	double theta;
	if (fabs(y) > r2) throw MapException(MapException::Other);
	theta  = asin(y / r2);
	phi    = pi2 - asin(((2. * theta) + sin(2. * theta)) / M_PI);
	lambda = lambda0 + ((M_PI * x) / (2. * r2 * cos(theta)));
	return theta;
}
