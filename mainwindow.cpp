/* ------------------------------------------------------------------------------------
The definition of the main window class.

Written by Nicholas Phillips.
QT4 implementation.  Michael R. Greason, ADNET, 23 August 2007.
------------------------------------------------------------------------------------ */
/*
			Fetch header files.
*/
#include <stdio.h>
#include <QMessageBox>
#include <QFileDialog>
#include <QFileInfo>
#include <QWhatsThis>
#include <QSize>
#include "mainwindow.h"
#include "controldialog.h"
#include "rangecontrol.h"
#include "debug.h"
#include "outlog.h"

using namespace std;
/*
			Constants.
*/
const double WhiteRiggingRadius = 0.99;
/* ====================================================================================
'mainWindow' defines the main window.  It descends from QMainWindow and from the 
Ui::MainWindow class that was created by QT Designer.

NOTE:  The SkyViewer widget, which descends from QGLViewer, is NOT inserted into
the window through QT Designer; it is explicitly inserted in the constructor of
this class.
==================================================================================== */
/* ------------------------------------------------------------------------------------
The class constructor.

Arguments:
	parent - The parent wiget/window.

Written by Nicholas Phillips.
QT4 implementation.  Michael R. Greason, ADNET, 23 August 2007.
------------------------------------------------------------------------------------ */
mainWindow::mainWindow(QWidget *parent) : QMainWindow(parent)
{
/*
			Initialize components.
*/
	map         = NULL;
	texture     = new SkyTexture;
	rigging     = new Rigging;
	whiterig    = new Rigging;
	polarsphere = new PolarArgLineSet;
/*
			Populate the window.
*/
	setupUi(this);

	viewer = new SkyViewer(centralwidget, this);
	viewer->setObjectName(QString::fromUtf8("viewer"));
	viewer->setGeometry(QRect(1, 0, 600, 591));
	viewer->setTexture(texture);
	viewer->setRigging(rigging);
	viewer->setWhiteRigging(whiterig);
	viewer->setPolarAngles(polarsphere);
	viewer->setFPSIsDisplayed(false);

	setWindowTitle(tr("Skyviewer"));

	filelabel = new QLabel("");
	filelabel->setMargin(2);
	filelabel->setMinimumWidth(150);
	filelabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	viewerStatus->addWidget(filelabel);

	projlabel = new QLabel("Projection: 3D Sphere");
	projlabel->setMargin(2);
	projlabel->setMinimumWidth(150);
	projlabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	viewerStatus->addWidget(projlabel);

	maplabel  = new QLabel("Map: Temperature");
	maplabel->setMargin(2);
	maplabel->setMinimumWidth(150);
	maplabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	viewerStatus->addWidget(maplabel);

	if (! showpolar)
	{
		actionPolarAnglesM->setDisabled(true);
		actionPolarAnglesM->setVisible(false);
		actionPolarAnglesTB->setDisabled(true);
		actionPolarAnglesTB->setVisible(false);
	}
/*
			Create the progress and range control windows.
*/

	ctl = new ControlDialog(this);
	rngctl = ctl->getRangeControl();
/*
			Define the connections.
*/
	connect(actionOpen,              SIGNAL(activated()), this,   SLOT(fileLoad()));
	connect(actionReload,            SIGNAL(activated()), this,   SLOT(fileReload()));
	connect(actionNew,               SIGNAL(activated()), this,   SLOT(fileNew()));
	connect(actionSnapshot,          SIGNAL(activated()), this,   SLOT(fileSnapshot()));
	connect(actionClose,             SIGNAL(activated()), this,   SLOT(fileClose()));
	connect(actionExit,              SIGNAL(activated()), this,   SLOT(fileExit()));

	connect(action3D_Sphere_tb,      SIGNAL(activated()), this,   SLOT(proj3Dsphere()));
	connect(action3D_Sphere_m,       SIGNAL(activated()), this,   SLOT(proj3Dsphere()));
	connect(actionMollweide_tb,      SIGNAL(activated()), this,   SLOT(projMollweide()));
	connect(actionMollweide_m,       SIGNAL(activated()), this,   SLOT(projMollweide()));

	connect(actionTemperature,       SIGNAL(activated()), this,   SLOT(mapTemperature()));
	connect(actionQ_Polarization,    SIGNAL(activated()), this,   SLOT(mapQPolarization()));
	connect(actionU_Polarization,    SIGNAL(activated()), this,   SLOT(mapUPolarization()));
	connect(actionP_Polarization,    SIGNAL(activated()), this,   SLOT(mapPPolarization()));
	connect(actionNum_Obs,           SIGNAL(activated()), this,   SLOT(mapNumObs()));

	connect(actionPolarAnglesTB,     SIGNAL(activated()), this,   SLOT(mapPolVectTB()));
	connect(actionPolarAnglesM,      SIGNAL(activated()), this,   SLOT(mapPolVectM()));


	connect(actionHelp,              SIGNAL(activated()), viewer, SLOT(help()));


	connect(rngctl,	SIGNAL(reTextureNeeded()),	this, SLOT(reTexture()));
	connect(rngctl,	SIGNAL(changeProjRigging()),	this, SLOT(newRigging()));
	connect(rngctl,	SIGNAL(changeFieldInfo()),	this, SLOT(newField()));
	connect(rngctl,	SIGNAL(changePolVect()),	this, SLOT(newPolVect()));

	connect(ctl,	SIGNAL(resetPixels(std::vector<int>)),	this,	SLOT(unselectPixels(std::vector<int>)));
	connect(ctl,	SIGNAL(recenterOnPixel(int)),		this,	SLOT(recenterOnPixel(int)));
/*
			Remaining initialization.
*/
	QSize sz;
	sz = viewer->size();
	delta = size() - sz;
	viewmoll = false;
	action3D_Sphere_tb->setChecked(! viewmoll); 
	action3D_Sphere_m->setChecked(! viewmoll);
	actionMollweide_tb->setChecked(viewmoll); 
	actionMollweide_m->setChecked(viewmoll);

	rigging->generate (rngctl->getRigging(), viewmoll);
	whiterig->generate(rngctl->getRigging(), viewmoll, WhiteRiggingRadius);
	ctl->clearStatus();
	ctl->show();
	
}
/* ------------------------------------------------------------------------------------
The class destructor.

Arguments:
	None.

Written by Michael R. Greason, ADNET, 23 August 2007.
------------------------------------------------------------------------------------ */
mainWindow::~mainWindow(void)
{
	if (map != NULL) delete map;
	map = NULL;
	if (texture != NULL) delete texture;
	texture = NULL;
	if (rigging != NULL) delete rigging;
	rigging = NULL;
	if (whiterig != NULL) delete whiterig;
	whiterig = NULL;
	if (polarsphere != NULL) delete polarsphere;
	polarsphere = NULL;
}
/* ------------------------------------------------------------------------------------
'resizeEvent' responds to a resize event, telling the QGLviewer to change its
size accordingly.

Arguments:
	event - The event to process.

Returned:
	Nothing.

Written by Michael R. Greason, ADNET, 27 August 2007.
------------------------------------------------------------------------------------ */
void mainWindow::resizeEvent(QResizeEvent *event)
{
/*
			Adjust the size of the viewer.
*/
	viewer->resize(event->size() - delta);	
/*
			Give the parent class its shot at the event.
*/
	QMainWindow::resizeEvent(event);
}
/* ------------------------------------------------------------------------------------
'readFile' reads a FITS file.  The name of the file is specified as either a null-
terminated strinkg or as a QSTRING.  The internal filename is set and the work is
passed off to the other version of this method that takes no arguments.

Arguments:
	file - The name of the file.

Returned:
	Nothing.

Written by Michael R. Greason, ADNET, 24 August 2007.
------------------------------------------------------------------------------------ */
void mainWindow::readFile (const char *file)
{
	if (file == NULL) return;
	QString tmp = file;
	readFile(tmp);
}
void mainWindow::readFile (const QString file)
{
	QString tmp;
	QFileInfo fi;
	if (file.length() <= 0) return;
	filename = file;
	fi.setFile(filename);
	tmp = fi.completeSuffix();
	tmp = (tmp.length() > 0) ? (fi.baseName() + tr(".") + tmp) : fi.baseName();
	filelabel->setText(tmp);
	ctl->startFile(tmp);
	readFile();
	//initMinMax();
}
/* ------------------------------------------------------------------------------------
'readFile' reads a FITS file.  The name of the file is supplied through the internal
filename variable.  This routine actually goes out and reads the file.

Arguments:
	None.

Returned:
	Nothing.

Written by Michael R. Greason, ADNET, 24 August 2007.
------------------------------------------------------------------------------------ */
void mainWindow::readFile ()
{
	if (filename.length() <= 0) return;
/*
			Allocate space for the map.
*/
	if (map == NULL) map = new HealpixMap();
	if (map == NULL) 
	{
		QMessageBox::critical(this, tr("Skyviewer"),
			tr("Unable to allocate memory for the map!"),
			QMessageBox::Ok);
		return;
	}
/*
			Read the map.
*/
	try {
		map->readFITS(filename, ctl);
	}
	catch (MapException &exc)
	{
		QString msg(tr("Unable to read map: "));
		QMessageBox::critical(this, tr("Skyviewer Load Error"),
			(msg + filename + "\nError: " + exc.Message()),
			QMessageBox::Ok);
		return;
	}
	ctl->init(map);
	setFieldEnables();
	
/*
			Create the texture and polarization angle vectors (if needed).
*/
	if (showtex)
	{
		try {
			texture->set(map, rngctl);
			viewer->update();
		}
		catch (MapException &exc)
		{
			QString msg(tr("Unable to read map: "));
			QMessageBox::critical(this, tr("Skyviewer"),
				(exc.Comment() == NULL) ? (msg + filename) : tr(exc.Comment()),
				QMessageBox::Ok);
			return;
		}
	}
	if (showpolar && map->has_Polarization())
	{
		if (polarsphere != NULL) polarsphere->set(map);
	}
/*
			Clear the list of selected pixels.  The window is taken care of at a lower
			level (as the file is being read).
*/
}
/* ------------------------------------------------------------------------------------
'undefined' is called when a response routine is currently not implemented.

Arguments:
	None.

Returned:
	Nothing.

Written by Nicholas Phillips.
QT4 implementation.  Michael R. Greason, ADNET, 23 August 2007.
------------------------------------------------------------------------------------ */
void mainWindow::undefined()
{
	QMessageBox::warning(this, tr("Skyviewer"),
		tr("This feature has not yet been defined!"),
		QMessageBox::Ok); 
}
/* ------------------------------------------------------------------------------------
'fileNew' prompts the user to specify a new file to be displayed.
	To be openned in a new viewer window.
	Not implemented yet.

Arguments:
	None.

Returned:
	Nothing.

Written by Nicholas Phillips.
------------------------------------------------------------------------------------ */
void mainWindow::fileNew()
{
	undefined();
}
/* ------------------------------------------------------------------------------------
'fileLoad' prompts the user to specify a new file to be displayed.

Arguments:
	None.

Returned:
	Nothing.

Written by Nicholas Phillips.
QT4 implementation.  Michael R. Greason, ADNET, 23 August 2007.
------------------------------------------------------------------------------------ */
void mainWindow::fileLoad()
{
	QString dir(".");
	if (filename.length() > 0)
	{
		QFileInfo fi(filename);
		dir = fi.absolutePath();
	}
	QString fil = QFileDialog::getOpenFileName(this, tr("Open FITS File"),
		dir, tr("FITS (*.fits);;Any(*)"));
	if (fil.length() <= 0) return;
	readFile(fil);
}
/* ------------------------------------------------------------------------------------
'fileReload' causes the current file to be reloaded and redisplayed.

Arguments:
	None.

Returned:
	Nothing.

Written by Nicholas Phillips.
QT4 implementation.  Michael R. Greason, ADNET, 23 August 2007.
------------------------------------------------------------------------------------ */
void mainWindow::fileReload()
{
	readFile();
}
/* ------------------------------------------------------------------------------------
'fileSnapshot' takes a snapshot of the current view.

Arguments:
	None.

Returned:
	Nothing.

Written by Nicholas Phillips.
QT4 implementation.  Michael R. Greason, ADNET, 23 August 2007.
------------------------------------------------------------------------------------ */
void mainWindow::fileSnapshot()
{
	//undefined();
	viewer->saveSnapshot(false);
}
/* ------------------------------------------------------------------------------------
'fileClose' closes the current main window; if others are still open the application
does not terminate.

Arguments:
	None.

Returned:
	Nothing.

Written by Nicholas Phillips.
QT4 implementation.  Michael R. Greason, ADNET, 23 August 2007.
------------------------------------------------------------------------------------ */
void mainWindow::fileClose()
{
	close();
}
/* ------------------------------------------------------------------------------------
'fileExit' shuts down the application.

Arguments:
	None.

Returned:
	Nothing.

Written by Nicholas Phillips.
QT4 implementation.  Michael R. Greason, ADNET, 23 August 2007.
------------------------------------------------------------------------------------ */
void mainWindow::fileExit()
{
	QApplication::exit(0);
}

//=====================================================================================
//                     Handle Changing the Projection/Rigging
//=====================================================================================
/* ------------------------------------------------------------------------------------
'proj3Dsphere' 
'projMollweide'
	responds to a change in choice of projection. rangeControl is told of the
	change. If it decides, it will signal a need to change the rigging


Written by Nicholas Phillips.
QT4 implementation.  Michael R. Greason, ADNET, 23 August 2007.
Modified by NGP, 8/12/08
------------------------------------------------------------------------------------ */
void mainWindow::proj3Dsphere()
{
	if( rngctl->getProjection() == Spherical ) {
		action3D_Sphere_tb->setChecked(true); 
		action3D_Sphere_m->setChecked(true);
		return;
	}
	rngctl->setProjection(Spherical);
}
void mainWindow::projMollweide()
{
	if( rngctl->getProjection() == Mollweide ) {
		actionMollweide_tb->setChecked(true); 
		actionMollweide_m->setChecked(true);
		return;
	}
	rngctl->setProjection(Mollweide);
}
/* ------------------------------------------------------------------------------------
'newRigging' updates the viewer when the user requests a change in the rigging size.

Arguments:
	None.

Returned:
	Nothing.

Written by Nicholas Phillips.
QT4 implementation.  Michael R. Greason, ADNET, 23 August 2007.
------------------------------------------------------------------------------------ */
void mainWindow::newRigging(void)
{
	try {
		viewmoll = rngctl->getProjection() == Mollweide;
		viewer->constrainMollweide(viewmoll);
		rigging->generate (rngctl->getRigging(), viewmoll);
		whiterig->generate(rngctl->getRigging(), viewmoll, WhiteRiggingRadius);
		polarsphere->setMollweide(viewmoll);
		viewer->update();
	}
	catch (MapException &exc)
	{
		const char *comm = exc.Comment();
		QMessageBox::critical(this, tr("Skyviewer"),
			tr((comm != NULL) ? comm : "newRigging error!"),
			QMessageBox::Ok);
	}
	// Update GUI.
	action3D_Sphere_tb->setChecked(! viewmoll); 
	action3D_Sphere_m->setChecked(! viewmoll);
	actionMollweide_tb->setChecked(viewmoll); 
	actionMollweide_m->setChecked(viewmoll);
	projlabel->setText(viewmoll ? "Projection: Mollweide" : "Projection: 3D Sphere");

}



//=====================================================================================
//                     Handle Changing the Display Map Field
//=====================================================================================
/* ------------------------------------------------------------------------------------
'setFieldEnables' Set the ToolBar's Field selections enabled according to new map
Arguments:
	None.
Written by Nicholas Phillips.
QT4 implementation.  Michael R. Greason, ADNET, 23 August 2007.
Updated by NGP, 8/13/08
------------------------------------------------------------------------------------ */
void mainWindow::setFieldEnables()
{
	actionTemperature->setEnabled(map->has_Temperature());
	actionQ_Polarization->setEnabled(map->has_Polarization());
	actionU_Polarization->setEnabled(map->has_Polarization());
	actionP_Polarization->setEnabled(map->has_Polarization());
	actionNum_Obs->setEnabled(map->has_Nobs());

	actionPolarAnglesTB->setEnabled(map->has_Polarization());
	actionPolarAnglesM->setEnabled(map->has_Polarization());

	return;
}
/* ------------------------------------------------------------------------------------
'mapTemperature'
'mapQPolarization()'
'mapUPolarization()'
'mapPPolarization()'
'mapNumObs()'
	Responds to changing the displayed map field action.
	rangeControl is told of the change and decides what to do.


Written by Nicholas Phillips.
QT4 implementation.  Michael R. Greason, ADNET, 23 August 2007.
------------------------------------------------------------------------------------ */
void mainWindow::mapTemperature()	{ rngctl->setField(I); }
void mainWindow::mapQPolarization() 	{ rngctl->setField(Q); }
void mainWindow::mapUPolarization() 	{ rngctl->setField(U); }
void mainWindow::mapPPolarization() 	{ rngctl->setField(P); }
void mainWindow::mapNumObs()		{ rngctl->setField(Nobs); }
/* ------------------------------------------------------------------------------------
'newField()'
	Update the GUI to reflect the state of the displayed field based on the
	state of rangeControl

Written by Nicholas Phillips, 8/13/08
------------------------------------------------------------------------------------ */
void mainWindow::newField()
{
	actionTemperature->setChecked(false);
	actionQ_Polarization->setChecked(false);
	actionU_Polarization->setChecked(false);
	actionP_Polarization->setChecked(false);
	actionNum_Obs->setChecked(false);
	switch( rngctl->getField() ) {
		case I:
			actionTemperature->setChecked(true);
			maplabel->setText(tr("Map: Temperature"));
			break;
		case Q:
			actionQ_Polarization->setChecked(true);
			maplabel->setText(tr("Map: Q Polarization"));
			break;
		case U:
			actionU_Polarization->setChecked(true);
			maplabel->setText(tr("Map: U Polarization"));
			break;
		case P:
			actionP_Polarization->setChecked(true);
			maplabel->setText(tr("Map: P Polarization"));
			break;
		case Nobs:
			actionNum_Obs->setChecked(true);
			maplabel->setText(tr("Map: Num Obs"));
			break;
	}
	return;
}
//=====================================================================================
/* ------------------------------------------------------------------------------------
'mapPolVectM' and 'mapPolVectTB' toggle the polarization vectors on and off.

Arguments:
	None.

Returned:
	Nothing.

Written by Nicholas Phillips.
QT4 implementation.  Michael R. Greason, ADNET, 30 August 2007.
------------------------------------------------------------------------------------ */
void mainWindow::mapPolVectM()
{
	rngctl->setPolarVect(actionPolarAnglesM->isChecked() ? On : Off );
}
void mainWindow::mapPolVectTB()
{
	rngctl->setPolarVect(actionPolarAnglesTB->isChecked() ? On : Off );
}
/* ------------------------------------------------------------------------------------
'newPolVect' Responds to rangeDialog indicating a change in state of displaying
		the polarization vectors.

Written by Nicholas Phillips, 8/13/08
------------------------------------------------------------------------------------ */
void mainWindow::newPolVect()
{
	bool b = rngctl->getPolVect() == On;
	actionPolarAnglesTB->setChecked(b);
	actionPolarAnglesM->setChecked(b);
	if (polarsphere != NULL) polarsphere->setOn(b);
}
//=====================================================================================
/* ------------------------------------------------------------------------------------
'reTexture'  updates the map texture and instructs the viewer to repaint itself.

This routine should be called when something occurs to modify the texture.

Arguments:
	None.

Returned:
	Nothing.

Written Michael R. Greason, ADNET, 23 August 2007.
------------------------------------------------------------------------------------ */
void mainWindow::reTexture(void)
{
	if (! showtex) return;
	try {
		texture->set(map, rngctl);
		viewer->update();
	}
	catch (MapException &exc)
	{
		const char *comm = exc.Comment();
		QMessageBox::critical(this, tr("Skyviewer"),
			tr((comm != NULL) ? comm : "reTextureOld error!"),
			QMessageBox::Ok);
	}
}
//=====================================================================================
/* ------------------------------------------------------------------------------------
'selectPixel' responds to the selection of a pixel in the viewer.  This version gets
the pixel to process from its arguments.

Arguments:
	pix - The pixel to process

Returned:
	pix - The pixel id.

Written by Michael R. Greason, ADNET, 29 August 2007.
------------------------------------------------------------------------------------ */
int mainWindow::selectPixel (int pix)
{
	if( ! ctl->selectPixel(pix,&((*map)[pix])) ) {
		texture->highlite(pix, 1.0);
	}
	if ( ctl->numselected() <= 0) {
		if (viewer->animationIsStarted()) viewer->stopAnimation();
	}
	else {
		if (! viewer->animationIsStarted()) viewer->startAnimation();
	}
	viewer->update();
	return pix;
}
/* ------------------------------------------------------------------------------------
'selectPixel' responds to the selection of a pixel in the viewer.  This version
converts the coordinates into a pixel id and passes execution to the other version.

Arguments:
	theta - The colatitude of the selected pixel, in radians, measured from the
	         North Pole.
	phi   - The longitude of the selected pixel, in radians.

Returned:
	pix   - The pixel id.

Written by Michael R. Greason, ADNET, 29 August 2007.
------------------------------------------------------------------------------------ */
int mainWindow::selectPixel (double theta, double phi)
{
	int pix;
	long p;

	//Convert the coordinate into a pixel number.
	map->angles2pixel(theta, phi, p);
	pix = int(p);

	// Process the pixel.
	return selectPixel(pix);
}
/* ------------------------------------------------------------------------------------
'highlightPixels' adjusts the opacity of the selected pixels.

Arguments:
	hlite - The opacity level to assign to the selected pixels.

Returned:
	Nothing.

Written by Michael R. Greason, ADNET, 29 August 2007.
------------------------------------------------------------------------------------ */
void mainWindow::highlightPixels (double hlite)
{
	for(int i = 0; i < ctl->numselected(); i ++ )
		texture->highlite(ctl->pixnum(i), float(hlite));
}
void mainWindow::unselectPixels(std::vector<int> pixs)
{
	for(uint i = 0; i < pixs.size(); i ++ )
		texture->highlite(pixs[i], 1.0);
	return;
}
/* ----------------------------------------------------------------------------
'recenterOnPixel' recenters the current view to be centered on the passed
	pixel number. Slot to receive signal from double-clicking on the list
	of selected pixels.

Arguments:
	int pixnum: Pixel number to center on, assumed to index into the
			current set sky map.

Returned:
	None.

Written by Nicholas Phillips, 10/14/08
---------------------------------------------------------------------------- */
void mainWindow::recenterOnPixel(int pixnum)
{
	double theta,phi;
	map->pixel2angles(pixnum, theta, phi);
	//viewer->recenterAt(cos(phi)*sin(theta),sin(phi)*sin(theta),cos(theta));
	viewer->recenterAt(theta,phi);
	return;
}
