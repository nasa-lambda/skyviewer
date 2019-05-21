//
// C++ Interface: histoview
//
// Description: 
//
//
// Author: Nicholas Phillips <nicholas.G.Phillips@nasa.gov>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef HISTOVIEW_H
#define HISTOVIEW_H

#include <QWidget>

class Histogram;
class ColorTable;
/*
	Paint a histogram.

	Once we have a histogram, allows the painting of it via a
	QPaintEvent request. When it is painted, 'c' and 'z' control
	the center and zoom of the region to be highlighted. Both
	c-z/2 and c+z/2 are assumed to be between 0 and 1.

	@author Nicholas Phillips <nicholas.G.Phillips@nasa.gov>
*/
class HistoView : public QWidget
{
	Q_OBJECT
public:
	HistoView(QWidget *parent = 0);
	
	// Associate with given histogram data
	void set(const Histogram *h);
	
	// Use this color table
	void set(const ColorTable *ct);
	
	// This is a fixed sized widget at this point
	QSize minimumSizeHint() const { 
		return QSize(w+2*w1, h); }
	QSize sizeHint() const { 
		return QSize(w+2*w1, h); }


public slots:
	void setCenterZoom(float, float);
	
protected:
	void paintEvent(QPaintEvent *event);

	int h;	// Height of view window
	int w;	// Width of selected range
	int w1;	// Width of border region on each side of the selected range
	
	float c;	// Current center mark
	float z;	// Current zoom width

	const Histogram *histo;	// pointer to histogram data
	const ColorTable *ct;
};

#endif
