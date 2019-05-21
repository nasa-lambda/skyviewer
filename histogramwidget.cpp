//
// C++ Implementation: histogramwidget
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
#include <QTimer>
#include "histogramwidget.h"
#include "healpixmap.h"

using namespace std;

/* ------------------------------------------------------------------------------------
'HistogramWidget' constructor

	Set defaults and connections

Written by Nicholas Phillips, UMCP, 6 August 2008.
------------------------------------------------------------------------------------ */
HistogramWidget::HistogramWidget(QWidget *parent) : QWidget(parent), minz(0.0001)
{

	setupUi(this);

	old_c = c = 0.5;
	old_z = z = 1;

	cztimer = new QTimer(this);
	connect(cztimer, SIGNAL(timeout()), this, SLOT(setNewCenterZoom()));
	
	lutimer = new QTimer(this);
	connect(lutimer, SIGNAL(timeout()), this, SLOT(setNewLowerUpper()));

	connect(this, SIGNAL(newCenterZoom(float,float)),histoView,SLOT(setCenterZoom(float,float)));

	setNewRange();
}


/* ------------------------------------------------------------------------------------
'set' 
	Setup the histoView and sliders for the given data vector
Arguments:
	x	The data to select a range for
Returned:
	Nothing

Written by Nicholas Phillips, UMCP, 6 August 2008.
------------------------------------------------------------------------------------ */
void HistogramWidget::set(vector<float> &x)
{	
	old_c = c = 0.5;
	old_z = z = 1;
	
	histogram.setup(x);

	minr = histogram.min();
	maxr = histogram.max();
	histogram.build(x, minr, maxr);
	histoView->set(&histogram);
	
	setComboBoxes();
	setNewRange();

	return;
}
	
//void HistogramWidget::set(const HealpixMap *map, Field fld)
void HistogramWidget::set(Skymap *map, Field fld)
{
	vector<float> x(map->n());
	switch( fld ) {
		case I: for(uint i = 0; i < map->n(); i++) x[i]= (*map)[i].T();
			break;
		case Q: for(uint i = 0; i < map->n(); i++) x[i]= (*map)[i].Q();
			break;
		case U: for(uint i = 0; i < map->n(); i++) x[i]= (*map)[i].U();
			break;
		case P: for(uint i = 0; i < map->n(); i++) x[i]= (*map)[i].Pmag();
			break;
		case Nobs: for(uint i = 0; i < map->n(); i++) x[i]= (*map)[i].Nobs();
			break;
	}
	
	histogram.setup(x);

	switch( fld ) {
		case I:
		case Q:
		case U:
			minr = -histogram.amax();
			maxr = +histogram.amax();
			break;
		case P:
		case Nobs:
			minr = 0;
			maxr = histogram.max();
			break;
	}
	
	histogram.build(x, minr, maxr);
	histoView->set(&histogram);
	
	setComboBoxes();
	setNewRange();

	return;
}
void HistogramWidget::set(ColorTable *ct)
{
	histoView->set(ct);
}
/* ------------------------------------------------------------------------------------
'setComboBoxes' 
	Set the fields for the Preset combo boxes based on the
	current min and max ranges
	x	The data to select a range for
Returned:
	Nothing

Written by Nicholas Phillips, UMCP, 6 August 2008.
------------------------------------------------------------------------------------ */
void HistogramWidget::setComboBoxes()
{	
	centerComboBox->clear();
	centerComboBox->addItem("<slider>");
	if( (minr < 0) && (maxr > 0) )
		centerComboBox->addItem("Zero");
	cmean = (histogram.mean()-minr)/(maxr-minr);
	centerComboBox->addItem("Mean");

	zoomComboBox->clear();
	zoomComboBox->addItem("<slider>");
	zstddev = histogram.stddev()/(maxr-minr);
	if( zstddev < 1 )
		zoomComboBox->addItem("1x Std Dev");
	if( 2*zstddev < 1 )
		zoomComboBox->addItem("2x Std Dev");
	if( 3*zstddev < 1 )
		zoomComboBox->addItem("3x Std Dev");
	zoomComboBox->addItem("Full");

	return;
}

//=====================================================================================
/* ------------------------------------------------------------------------------------
'on_zoomSlider_sliderPressed' 
'on_centerSlider_sliderPressed'

	When either zoom or center sliders are activated, start the associated timer
	so we sample them.

	Qt auto-connect has been assumed
Arguments:
	None
Returned:
	Nothing

Written by Nicholas Phillips, UMCP, 6 August 2008.
------------------------------------------------------------------------------------ */
void HistogramWidget::on_zoomSlider_sliderPressed() {
	cztimer->start(100);
}
void HistogramWidget::on_centerSlider_sliderPressed() {
	cztimer->start(100);
}
/* ------------------------------------------------------------------------------------
'on_zoomSlider_sliderReleased'
'on_centerSlider_sliderReleased'
	When either slider is released, stop sampling them and return them to their
	midpoints.
	
	Qt auto-connect has been assumed
Arguments:
	None.
Returned:
	Nothing

Written by Nicholas Phillips, UMCP, 6 August 2008.
------------------------------------------------------------------------------------ */
void HistogramWidget::on_zoomSlider_sliderReleased() {
	cztimer->stop();
	zoomSlider->setSliderPosition(50);
}
void HistogramWidget::on_centerSlider_sliderReleased() {
	cztimer->stop();
	centerSlider->setSliderPosition(50);
}
/* ------------------------------------------------------------------------------------
'setNewCenterZoom'
	Read the zoom/center sliders and update 'c' and 'z'.
	First convert the posistion into a delta. Change c and z
	accordingly and check the range. Update the displays.
	
Arguments:
	None.
Returned:
	Nothing.

Written by Nicholas Phillips, UMCP, 6 August 2008.
------------------------------------------------------------------------------------ */
void HistogramWidget::setNewCenterZoom()
{
	float dz=(zoomSlider->value()-50.)/100;
	dz = dz*dz*dz;
	z -= dz;
	float dc=(centerSlider->value()-50.)/100;
	dc = dc*dc*dc;
	c += dc;

	if( z < minz) z = minz;
	if( z > 1 ) z = 1;

	if( c+z/2 > 1 ) c =  1-z/2;
	if( c-z/2 < 0 ) c = z/2;

	setNewRange();

	return;
}
//=====================================================================================
/* ------------------------------------------------------------------------------------
'on_lowerSlider_sliderPressed' 
'on_upperSlider_sliderPressed'

	When either lower or upper sliders are activated, start the associated timer
	so we sample them.

	Qt auto-connect has been assumed
Arguments:
	None
Returned:
	Nothing

Written by Nicholas Phillips, UMCP, 6 August 2008.
------------------------------------------------------------------------------------ */
void HistogramWidget::on_lowerSlider_sliderPressed() {
	lutimer->start(100);
}
void HistogramWidget::on_upperSlider_sliderPressed() {
	lutimer->start(100);
}
/* ------------------------------------------------------------------------------------
'on_lowerSlider_sliderReleased'
'on_upperSlider_sliderReleased'
	When either slider is released, stop sampling them and return them to their
	midpoints.
	
	Qt auto-connect has been assumed
Arguments:
	None.
Returned:
	Nothing

Written by Nicholas Phillips, UMCP, 6 August 2008.
------------------------------------------------------------------------------------ */
void HistogramWidget::on_lowerSlider_sliderReleased() {
	lutimer->stop();
	lowerSlider->setSliderPosition(50);
}
void HistogramWidget::on_upperSlider_sliderReleased() {
	lutimer->stop();
	upperSlider->setSliderPosition(50);
}
/* ------------------------------------------------------------------------------------
'setNewLowerUpper'
	Read the lower/upper sliders and update 'c' and 'z'.
	First convert the posistion into a delta. Change c and z
	accordingly and check the range. Update the displays.
	
Arguments:
	None.
Returned:
	Nothing.

Written by Nicholas Phillips, UMCP, 6 August 2008.
------------------------------------------------------------------------------------ */
void HistogramWidget::setNewLowerUpper()
{
	float dl=(lowerSlider->value()-50.)/100;
	dl = dl*dl*dl;

	float du=(upperSlider->value()-50.)/100;
	du = du*du*du;

	float l = c-z/2;
	float u = c+z/2;

	l += dl;
	u += du;

	if( l < 0 ) l = 0;
	if( u > 1 ) u = 1;

	if( l >= u ) return;

	c = (u+l)/2;
	z = u-l;

	setNewRange();

	return;
}
//=====================================================================================

/* ------------------------------------------------------------------------------------
''on_zoomComboBox_activated' 
	Change c and z based on the preset selected' 
	
Arguments:
	None
Returned:
	Nothing

Written by Nicholas Phillips, UMCP, 6 August 2008.
------------------------------------------------------------------------------------ */
void HistogramWidget::on_zoomComboBox_activated(int /*index*/)
{
	bool changed=false;
	if( zoomComboBox->currentText() == "1x Std Dev" ) {
		old_z = z = zstddev;
		changed = true;
	}
	else if( zoomComboBox->currentText() == "2x Std Dev" ) {
		old_z = z = 2*zstddev;
		changed = true;
	}
	else if( zoomComboBox->currentText() == "3x Std Dev" ) {
		old_z = z = 3*zstddev;
		changed = true;
	}
	else if( zoomComboBox->currentText() == "Full" ) {
		old_z = z = 1;
		old_c = c = 0.5;
		changed = true;
	}
	if( changed ) {
		if( c+z/2 > 1 ) c =  1-z/2;
		if( c-z/2 < 0 ) c = z/2;
		setNewRange();
	}
	return;
}
//-------------------------------------------------------------
/* ------------------------------------------------------------------------------------
'on_centerComboBox_activated' 
	Change c and z based on the preset selected

Arguments:
	None
Returned:
	Nothing

Written by Nicholas Phillips, UMCP, 6 August 2008.
------------------------------------------------------------------------------------ */
void HistogramWidget::on_centerComboBox_activated(int /*index*/)
{
	bool changed=false;
	if( centerComboBox->currentText() == "Zero" ) {
		old_c = c = -minr/(maxr-minr);
		changed = true;
	}
	else if( centerComboBox->currentText() == "Mean" ) {
		old_c = c = cmean;
		changed = true;
	}
	if( changed ) {
		if( c + z/2 > 1 ) z = 2-2*c;
		if( c - z/2 < 0 ) z = 2*c;
		setNewRange();
	}
	return;
}
/* ------------------------------------------------------------------------------------
'setNewRange'
	Use the current 'c' and 'z' values to update the dispayed text. Reset any
	Comboboxes that need it. emit the new values so the associated histoView
	can update.
	
Arguments:
	None
Returned:
	Nothing

Written by Nicholas Phillips, UMCP, 6 August 2008.
------------------------------------------------------------------------------------ */
void HistogramWidget::setNewRange()
{
	lower = (maxr-minr)*(c - z/2)+minr;
	upper = (maxr-minr)*(c + z/2)+minr;
	float cv = (maxr-minr)*c+minr;
	if( cv < 1e-6*(maxr-minr) ) cv = 0;
 
	lowLabel->setText(QString("%1").arg(lower,9,'g',3));
	centerLabel->setText(QString("%1").arg(cv,9,'g',3));
	hiLabel->setText(QString("%1").arg(upper,9,'g',3));

	if( centerComboBox->currentIndex() != 0 && (c != old_c ) )
		centerComboBox->setCurrentIndex(0);
	if( zoomComboBox->currentIndex() != 0 && (z != old_z ) )
		zoomComboBox->setCurrentIndex(0);

	old_c = c;
	old_z = z;

	emit newCenterZoom(c, z);
	emit newRange(lower, upper);
}
