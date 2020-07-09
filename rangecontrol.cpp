/* ====================================================================================
The definition of the range control window class.

Written by Michael R. Greason, ADNET, 23 August 2007.
==================================================================================== */
/*
			Fetch header files.
*/
#include "rangecontrol.h"
#include "healpixmap.h"

using namespace std;
/* ------------------------------------------------------------------------------------
The class constructor.

Arguments:
	parent - The parent wiget/window.

Written by Michael R. Greason, ADNET, 23 August 2007.
------------------------------------------------------------------------------------ */
RangeControl::RangeControl(QWidget *parent) : QWidget(parent) 
{
	setupUi(this);

	//setWindowTitle(tr("Range Control"));

	//setVisible(false);

	map = 0;
	proj=Spherical;
	fld=I;
	rigging = riggingSelect->currentText().toInt();
	pv = Off;

	connect(histogramWidget, static_cast<void(HistogramWidget::*)(float,float)>(&HistogramWidget::newRange),
	        this,            static_cast<void(RangeControl::*)(float,float)>(&RangeControl::updateTexture));
};

/* ------------------------------------------------------------------------------------
'init'

Arguments:
	map:	Pointer to Healpix map to use

Written by Nicholas Phillips 8/11/08
------------------------------------------------------------------------------------ */
void RangeControl::init(Skymap *map_)
{
	map = map_;
	fieldSelect->clear();
	fields.clear();
	if( map->has_Temperature() ) {
		fieldSelect->addItem("Temperature");
		fields.push_back(I);
	}
	if( map->has_Polarization() ) {
		fieldSelect->addItem("Q Polarization");
		fields.push_back(Q);
		fieldSelect->addItem("U Polarization");
		fields.push_back(U);
		fieldSelect->addItem("P Polarization");
		fields.push_back(P);
	}
	else if( (fld==Q) || (fld==U) || (fld==P) )
		fld = I;
	if( map->has_Nobs() ) {
		fieldSelect->addItem("Num Obs");
		fields.push_back(Nobs);
	}
	else if( fld == Nobs )
		fld = I;

	polarVectorBox->setEnabled(map->has_Polarization());
	pv = Off;
	
	histogramWidget->set(map,fld);

	return;
}


/* ------------------------------------------------------------------------------------
'on_projectionSelect_activated'
	auto-connected slot to handle a change in the selected projection.
	All that happens is the change is recorded and signaled

Written by Nicholas Phillips 8/11/08
------------------------------------------------------------------------------------ */
void RangeControl::on_projectionSelect_activated(int)
{
	Projection newproj = projectionSelect->currentText() == "3D Sphere" ? Spherical : Mollweide;
	if( newproj == proj ) return;
	proj = newproj;
	emit changeProjRigging();
}
void RangeControl::setProjection(Projection newproj)
{
	if( newproj == proj ) return;
	proj = newproj;
	projectionSelect->setCurrentIndex(proj == Spherical ? 0 : 1);
	emit changeProjRigging();

}
/* ------------------------------------------------------------------------------------
'on_riggingSelect_activated'
	auto-connected slot to handle a change in the desired sphere rigging size
	All that happens is the change is recorded and signaled

Written by Nicholas Phillips 8/11/08
------------------------------------------------------------------------------------ */
void RangeControl::on_riggingSelect_activated(int)
{
	int newrigging = riggingSelect->currentText().toInt();
	if( newrigging == rigging ) return;
	rigging = newrigging;
	emit changeProjRigging();

}
/* ------------------------------------------------------------------------------------
'on_fieldSelect_activated'
	auto-connected slot to handle a change in the selected display field
Arguments:

Written by Nicholas Phillips 8/11/08
------------------------------------------------------------------------------------ */
void RangeControl::on_fieldSelect_activated(int i)
{
	if( fields[i] == fld )
		return;
	fld = fields[i];
	histogramWidget->set(map,fld);
	emit changeFieldInfo();
}
/* ------------------------------------------------------------------------------------
'setField'
	External request for a change in the selected map field. Assumes a valid field
	was requested.
Arguments:
	newfld:		The new display field to use

Written by Nicholas Phillips 8/11/08
------------------------------------------------------------------------------------ */
void RangeControl::setField(Field newfld)
{
	if( newfld == fld ) return;
	fld = newfld;
	for(int i = 0; i < fieldSelect->count();i++) {
		if( fields[i] == fld ) {
			fieldSelect->setCurrentIndex(i);
			break;
		}
	}
	histogramWidget->set(map,fld);
	emit changeFieldInfo();
}
/* ------------------------------------------------------------------------------------
'on_colorSelect_activated'
	auto-connected slot to handle a change in the selected color table
Arguments:

Written by Nicholas Phillips 8/11/08
------------------------------------------------------------------------------------ */
void RangeControl::on_colorSelect_activated(int)
{
	int i = colorSelect->currentIndex();
	histogramWidget->set(ctl[i]);
	emit reTextureNeeded();
}
/* ------------------------------------------------------------------------------------
'on_colorSelect_activated'
	auto-connected slot to handle a change in the selected color table
Arguments:

Written by Nicholas Phillips 8/11/08
------------------------------------------------------------------------------------ */
void RangeControl::on_polarVectorBox_clicked(bool v)
{
	pv = v ? On : Off;
	emit changePolVect();
}
/* ------------------------------------------------------------------------------------
'setPolarVect'
	External request for a change as to whether the polarization are shown.
Arguments:
	newpv:		The new state

Written by Nicholas Phillips 8/11/08
------------------------------------------------------------------------------------ */
void RangeControl::setPolarVect(PolVectors newpv)
{
	if( newpv == pv ) return;
	pv = newpv;
	polarVectorBox->setChecked(pv == On);
	emit changePolVect();
}
/* ------------------------------------------------------------------------------------
'updateTexture'
	change the state and signal a need to retexture
Arguments:

Written by Nicholas Phillips 8/11/08
------------------------------------------------------------------------------------ */
void RangeControl::updateTexture(float lower, float upper)
{
	minv = lower;
	maxv = upper;
	emit reTextureNeeded();
}
/* ------------------------------------------------------------------------------------
'getColorTable' returns the currently selected color table from its comboBox.

Arguments:
	None.

Returned:
	rigging - The current rigging.

Written by Michael R. Greason, ADNET, 23 August 2007.
------------------------------------------------------------------------------------ */
ColorTable* RangeControl::getColorTable (void) const
{
	int i = colorSelect->currentIndex();
	return ctl[i];
}

