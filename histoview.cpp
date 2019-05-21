//
// C++ Implementation: histoview
//
// Description: 
//
//
// Author: Nicholas Phillips <nicholas.G.Phillips@nasa.gov>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include <math.h>
#include <iostream>
#include <iomanip>
#include <QPainter>
#include "histogram.h"
#include "histoview.h"
#include "colortable.h"

using namespace std;

/* ------------------------------------------------------------------------------------
'HistoView'  constructor.

	Set the default size and background. Mark histogram as not set
	
Arguments:
	parent: who wanted me to exist. You take responsibility for killing me.

Written by Nicholas Phillips, UMCP, 6 August 2008.
------------------------------------------------------------------------------------ */
HistoView::HistoView(QWidget *parent) : QWidget(parent)
{
	histo = 0;	// So no segfaults while we wait for some data to paint
	ct = 0;

	h = 128;
	w = 256;
	w1=  64;
	/*
	h = 320;
	w = 480;
	w1= 80;
	*/
	setBackgroundRole(QPalette::Base);
	setAutoFillBackground(true);
}


/* ------------------------------------------------------------------------------------
'set'
	What histogram to display. It is assumed the passed Histogram exists as
	long as QPaintEvents may arrive.
	
Arguments:
	h	Pointer to the histogram to use
Returned:

Written by Nicholas Phillips, UMCP, 6 August 2008.
------------------------------------------------------------------------------------ */
void HistoView::set(const ColorTable *newct)
{
	ct = newct;
	update();
	return;
}
/* ------------------------------------------------------------------------------------
'set'
	What histogram to display. It is assumed the passed Histogram exists as
	long as QPaintEvents may arrive.
	
Arguments:
	h	Pointer to the histogram to use
Returned:

Written by Nicholas Phillips, UMCP, 6 August 2008.
------------------------------------------------------------------------------------ */
void HistoView::set(const Histogram *h)
{
	histo = h;
	setCenterZoom(0.5,1);
	return;
}
/* ------------------------------------------------------------------------------------
'setCenterZoom' 
	The new values are assumed to be within range.
	The hilighted region is taken to span [c-z/2,c+z/2]

Arguments:
	c_	New Center value
	z_	New Zoom value
Returned:
	Nothing
Written by Nicholas Phillips, UMCP, 6 August 2008.
------------------------------------------------------------------------------------ */
void HistoView::setCenterZoom( float c_, float z_)
{
	c = c_;
	z = z_;
	update();
	return;
}

/* ------------------------------------------------------------------------------------
'paintEvent' 
	Paint the actual histogram, hilighting the selected range as set by the
	most recent call of setCenterZoom.

	Assumes the histogram returns a value between 0 and 1

Arguments:
	<unused QPaintEvent>
Returned:
	Nothing.

Written by Nicholas Phillips, UMCP, 6 August 2008.
------------------------------------------------------------------------------------ */
void HistoView::paintEvent(QPaintEvent *)
{
	if( ! histo ) return;

	float a = c - (w1/(w-1.) + 0.5)*z;
	float b = z/(w-1.);

	QPainter painter(this);
	painter.setPen(QPen(QColor("black")));
	painter.drawLine(w1-1,   0, w1-1,   h);
	painter.drawLine(w+w1-1, 0, w+w1-1, h);

	QColor color;
	float x0,x1,r,cv;
	for(int p = 0; p < w+2*w1-1; p ++) {
		//cout << setw(5) << p;
		x0 = a + b*p;
		x1 = a + b*(p+1);

		if( (x0 <= 0) || (x1 >= 1) ) 
			r=0;
		else
			r = (*histo)(x0,x1);

		//cout << setw(3) << ((p < w1) || (p >= w+w1));

		if( (p < w1) || (p >= w+w1) )
			painter.setPen(QPen(QColor("black")));
		else {
			if( ct != NULL ) {
				cv = ((float)(p-w1))/(w-1);
				painter.setPen(QPen((*ct)(cv)));
			}
			else {
				color.setHsvF(((float)(p-w1))/(1.5*w),1,1);
			//cout << setw(10) << ((float)(p-w1))/(w-1);
				painter.setPen(QPen(color));
			}
		}
		//cout << endl;
		painter.drawLine(p,(int)(h*(1-r)),p,h);
	}
	return;
}
