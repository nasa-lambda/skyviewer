//
// C++  Interfacen: histogram
//
//
// Author: Nicholas Phillips <nicholas.G.Phillips@nasa.gov>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <vector>

/*
	Used for computing and storing histogram data.

	Once the histogram has been computed, it can be accessed
	via given it values between 0 and 1, corresponding to the
	lowest and highest values found/used. The returned value
	is between 0 and 1. Use the operator()
	methods.

	Also can provide stats on the underlying data that was binned.

	@author Nicholas Phillips <Nicholas.G.Phillips@nasa.gov>
*/
class Histogram
{
public:
	// Compute stats
	void setup(std::vector<float> &x);
	// Compute the histogram
	void build(std::vector<float> &, const float, const float );

	// return some pre-computed stats
	float min()	const	{ return minv; };
	float max()	const	{ return maxv; };
	float amax()	const	{ return amaxv; };
	float mean()	const	{ return meanv; };
	float stddev()	const	{ return stddevv; };
	//float () const { return v; };

	// Access the histogram
	float operator()(const float x) const;
	float operator()(const float x0,const float x1) const;
protected:
	std::vector<int> h;	// The stored histogram
	long nbin;		// Number of bins
	int hmax;		// Largest bin value
	//float minr;		// Bottom of histogram range
	//float maxr;		// Top of histogram range
	float minv;		// Smallest value in the input vector
	float maxv;		// Largest  value in the input vector
	float amaxv;		// Max absolute value of input values
	float meanv;		// Mean of input vector of values
	float stddevv;		// Standard Deviation of input vector of values
};

#endif
