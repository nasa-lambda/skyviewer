#ifndef MAINWIINDOW_H
#define MAINWIINDOW_H
/* ====================================================================================
The definition of the main window class.

Written by Nicholas Phillips.
QT4 implementation.  Michael R. Greason, ADNET, 23 August 2007.
==================================================================================== */
/*
			Fetch header files.
*/
#include <vector>
#include <QComboBox>
#include <QString>
#include <QLabel>
#include <QSize>
#include <QResizeEvent> 
#include "ui_mainwindow.h"
#include "skyviewer.h"
#include "healpixmap.h"
#include "selectedpixelmodel.h"

class ControlDialog;
class RangeControl;

/*
class SelectedPixelModel : public QAbstractTableModel
{
};
*/
/* ====================================================================================
'mainWindow' defines the main window.  It descends from QMainWindow and from the 
Ui::MainWindow class that was created by QT Designer
==================================================================================== */
class mainWindow : public QMainWindow, private Ui::MainWindow
{
	Q_OBJECT
public slots:
	virtual void undefined ();
	virtual void fileLoad ();
	virtual void fileNew ();
	virtual void fileReload ();
	virtual void fileSnapshot ();
	virtual void fileClose ();
	virtual void fileExit ();

	virtual void proj3Dsphere ();
	virtual void projMollweide ();

	virtual void mapTemperature ();
	virtual void mapQPolarization ();
	virtual void mapUPolarization ();
	virtual void mapPPolarization ();
	virtual void mapNumObs ();

	virtual void mapPolVectM ();
	virtual void mapPolVectTB ();

	virtual void reTexture();
	virtual void newRigging(void);
	virtual void newField();
	virtual void newPolVect();

	virtual void unselectPixels(std::vector<int>);
	virtual void recenterOnPixel(int pixnum);

private:
	HealpixMap      *map;
	SkyTexture      *texture;
	Rigging         *rigging;
	Rigging         *whiterig;
	Rigging         *blackrig;
	PolarArgLineSet *polarsphere;

	bool viewmoll;

	SkyViewer    *viewer;
	ControlDialog *ctl;
	RangeControl *rngctl;

	QSize delta;

	QLabel     *filelabel;
	QString     filename;
	QLabel     *projlabel;
	QLabel     *maplabel;

	void setFieldEnables();

	void fileFileInfo (bool b);

protected:
	virtual void resizeEvent(QResizeEvent *event);
public:
	mainWindow(QWidget *parent = 0);
	virtual ~mainWindow (void);
	void readFile (const char *file);
	void readFile (const QString file);
	void readFile (void);

	bool currentMollweide (void) const { return viewmoll; }

	int  selectPixel (int pix);
	int  selectPixel (double phi, double lambda);
	void highlightPixels (double hlite);
};
#endif
