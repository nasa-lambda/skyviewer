//
// C++ Interface: histogram
//
// Description: 
//
//
// Author: Nicholas Phillips <Nicholas.G.Phillips@nasa.gov>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include <iostream>
#include <iomanip>
#include <math.h>
#include "histogram.h"

using namespace std;

/* ------------------------------------------------------------------------------------
'setup' 
	- determine the min and max values in the data
	- calculate some stats of the data
	
Arguments:
	x: the vector of values to histogram
Returned:
	Nothing

Written by Nicholas Phillips, UMCP, 6 August 2008.
------------------------------------------------------------------------------------ */
void Histogram::setup(vector<float> &x)
{
	minv = maxv = x[0];
	amaxv = fabs(x[0]);
	double ttl = 0;
	double ttlsqr = 0;
	for(unsigned long i = 0; i < x.size(); i++) {
		if( x[i] > maxv ) maxv = x[i];
		if( x[i] < minv ) minv = x[i];
		if( fabs(x[i]) > amaxv ) amaxv = fabs(x[i]);
		ttl += x[i];
		ttlsqr += x[i]*x[i];
	}
	meanv = ttl/x.size();
	stddevv = ttlsqr/x.size() - meanv*meanv;
	stddevv = stddevv > 0 ? sqrt(stddevv) : 0;

	return;
} 
/* ------------------------------------------------------------------------------------
'set' 
	Using a fixed bin count, compute the histogram
	
Arguments:
	x: 	the vector of values to histogram
	minr:	The bottom value for  the histogram
	maxr:	The top value for  the histogram
Returned:
	Nothing

Written by Nicholas Phillips, UMCP, 6 August 2008.
------------------------------------------------------------------------------------ */
void Histogram::build(vector<float> &x, const float minr, const float maxr)
{
	nbin=2048;
	h.clear();
	h.resize(nbin);
	long n = x.size();

	hmax=0;
	for(long i = 0; i < n; i++) {
		long bin = (long)(nbin*(x[i]-minr)/(maxr-minr));
		if( (bin < 0) || (bin >= nbin) ) continue; // out of range
		h[bin]++;
		if( h[bin] > hmax ) hmax = h[bin];
	}

	return;
} 

/* ------------------------------------------------------------------------------------
'operator()' 
	
Arguments:
	x: bin value request, assumed 0 <= x < 1
Returned:
	bin value, as a value between 0 and 1

Written by Nicholas Phillips, UMCP, 6 August 2008.
------------------------------------------------------------------------------------ */
float Histogram::operator()(const float x) const
{
	long bin = (long)(nbin*x);
	if( (bin < 0) || (bin >= nbin) )
		return 0;
	return ((float)h[bin])/hmax;
}

/* ------------------------------------------------------------------------------------
'operator()' 
	
Arguments:
	x0: Lower limit of bin request range
	x1: Upper limit of bin request range
		both are assumed 0 <= x < 1
Returned:
	bin value, as a value between 0 and 1.
	If more than one bin, average of bin values in the range

Written by Nicholas Phillips, UMCP, 6 August 2008.
------------------------------------------------------------------------------------ */
float Histogram::operator()(const float x0, const float x1) const
{
	long bin0 = (long)(nbin*x0);
	long bin1 = (long)(nbin*x1);

	if( (bin0 < 0) || (bin1 >= nbin) )
		return 0;

	if( bin0 == bin1 ) return ((float)h[bin0])/hmax;

	float y = 0;
	for(long i = bin0; i <= bin1; i++)
		y += h[i];
	y /= bin1-bin0+1;
	return y/hmax;
}
