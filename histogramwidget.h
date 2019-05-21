//
// C++ Interface: histogramwidget
//
// Author: Nicholas Phillips <Nicholas.G.Phillips@nasa.gov>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef HISTOGRAMWIDGET_H
#define HISTOGRAMWIDGET_H

#include <vector>
#include <QWidget>

#include "enums.h"
#include "histogram.h"
#include "ui_histogramwidget.h"

//class HealpixMap;
class Skymap;
class ColorTable;

/*
	Main class for displaying and selecting the color table range
	baseded on a histogram of possible values.

	The widget needs a vector of values and display options TBD.
	It emits a newRange signal when there is a new range.

	There two basic parts to this widget. The top part is a custom
	HistoView widget. It has the responsibility of displaying the
	histogram and highlighting the current color range.

	The bottom part are the range control sliders and present ComboBoxes.
	They have been laid out via Qt's designer. See histogramwidget.ui.

	Finally, there are state variables including a Histogram object.


	A note on the sliders. They are 'rubber band' sliders, in that
	the distance of the slider from its mid-points determines the
	rate at which the value it controls changes. The further from
	the middle, the faster the value changes. Once the slider is
	released, it snaps back to the middle and the value stops changing.

	While the slider is held, we use QTimers to monitor its position
	and update the value it is controling. If the updated value is
	still within range, the new values are sent to the HistoView.
	Otherwise, no update. This way we do not have to worry about out
	of range values being reached. We also have 'page step' turned
	off for all sliders.

	Once the slider is released, the associated timer is stopped.

	@author Nicholas Phillips <Nicholas.G.Phillips@nasa.gov>
*/
class HistogramWidget : public QWidget, private Ui::HistogramWidget
{
	Q_OBJECT
public:
	HistogramWidget(QWidget *);

	// setup for the passed data
	void set(std::vector<float> &x);
	// Set based on a selected field of an Healpix Map
	void set(Skymap *map, Field fld);
	void set(ColorTable *);

signals:
	void newCenterZoom(float, float);
	void newRange(float, float);

private:
	const float minz;	// Min allowed zoom

	Histogram histogram;

	float z;		// Current zoom width
	float c;		// Current Center
	float old_c;		// Last valid center
	float old_z;		// Lasr valid zoom

	float lower;		// Lower limit of current range
	float upper;		// Upper limit of current range

	float minr;		// Min of range
	float maxr;		// Max of range
	float cmean;		// center for the data's mean
	float zstddev;		// zoom for the data's std dev
	

	QTimer *cztimer;	// Timer for sampling the center/zoom sliders
	QTimer *lutimer;	// Timer for sampling the lower/upper sliders

private:
	void setComboBoxes();
	void setNewRange();

private slots:
	void on_zoomSlider_sliderPressed();
	void on_zoomSlider_sliderReleased();
	void on_centerSlider_sliderPressed();
	void on_centerSlider_sliderReleased();

	void on_lowerSlider_sliderPressed();
	void on_lowerSlider_sliderReleased();
	void on_upperSlider_sliderPressed();
	void on_upperSlider_sliderReleased();

	void on_zoomComboBox_activated(int);
	void on_centerComboBox_activated(int);

	void setNewCenterZoom();
	void setNewLowerUpper();

};

#endif
