#ifndef RANGEDIALOG_H
#define RANGEDIALOG_H
/* ====================================================================================
The definition of the range control window class.

==================================================================================== */
/*
			Fetch header files.
*/
#include <vector>
#include "colortable.h"
#include "ui_rangecontrol.h"
#include "enums.h"

class Skymap;

class RangeControl : public QWidget, private Ui::RangeControl
{
	Q_OBJECT
public:
	RangeControl (QWidget *parent = 0);

	// initialize and associate with a given sky map
	void init(Skymap *map_);

	// Let others know the current state
	Projection getProjection() const { return proj; };
	Field getField() const { return fld; };
	PolVectors getPolVect() const { return pv; };
	int getMapIndex(void) const;
	float getMinimum() { return minv; };
	float getMaximum() { return maxv; };
	int getRigging() const { return rigging; };
	ColorTable *getColorTable() const;

	void setProjection(Projection);
	void setField(Field);
	void setPolarVect(PolVectors);

signals:
	// Need to retexture the sphere
	void reTextureNeeded();
	// Update the rigging
	void changeProjRigging();
	// Update the displayed field info
	void changeFieldInfo();
	// Update showing the polarization vectors
	void changePolVect();

private:
	// state variables
	Projection proj;
	Field fld;
	int rigging;
	PolVectors pv;

	float minv;
	float maxv;

	ColorTableList ctl; 			// List of supported color tables.

	Skymap *map;
	std::vector<Field> fields;

private slots:
	// auto-connect the ComboBoxes with their responses
	void on_projectionSelect_activated(int);
	void on_fieldSelect_activated(int);
	void on_riggingSelect_activated(int);
	void on_colorSelect_activated(int);
	void on_polarVectorBox_clicked(bool);
	// need to update the texture
	void updateTexture(float l, float u);
};
#endif
