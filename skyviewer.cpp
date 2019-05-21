/* ============================================================================
'skyviewer.cpp' extends the QGLViewer class.
============================================================================ */
/*
			Fetch header files.
*/
#include <iostream>
#include <QFile>
#include <QTextStream>
#include "skyviewer.h"
#include "mainwindow.h"
#include "heal.h"
#include "debug.h"

using namespace qglviewer;
using namespace std;
/*
			Constants.
*/
static const char stateFile[] = {".skyviewer.xml"};
/* ============================================================================
'SkyViewer' extends the QGLViewer class.
============================================================================ */
/* ----------------------------------------------------------------------------
'SkyViewer' is the class constructor.

Arguments:
	parent - The parent widget.
	name   - The name of this widget.

Written by Nicholas Phillips.
QT4 implementation.  Michael R. Greason, ADNET, 28 August 2007.
---------------------------------------------------------------------------- */
SkyViewer::SkyViewer (QWidget *parent, mainWindow *mw) : 
	QGLViewer(parent), whitecolor(255,255,255,255), blackcolor(0,0,0,255)
{
	pulseflg  = true;
	mollview  = false;
	mwin      = mw;
	texture   = NULL;
	rigging   = NULL;
	whiterig  = NULL;
	polar     = NULL;
	hlite     =  1.0;
	delhlite  = -0.1;
	constraint = NULL;
	setStateFileName(stateFile);
}
/* ----------------------------------------------------------------------------
'~SkyViewer' is the class destructor.  It does NOT delete the rigging, texture,
or polar lines members.

Arguments:
	None.

Written by Nicholas Phillips.
QT4 implementation.  Michael R. Greason, ADNET, 28 August 2007.
---------------------------------------------------------------------------- */
SkyViewer::~SkyViewer()
{
	saveStateToFile();
	texture  = NULL;
	rigging  = NULL;
	whiterig = NULL;
	polar    = NULL;
	if (constraint != NULL) delete constraint;
	constraint = NULL;
}
/* ----------------------------------------------------------------------------
'setTexture' assigns the texture to be displayed.

Arguments:
	t - The new texture.

Returned:
	None.

Written by Nicholas Phillips.
QT4 implementation.  Michael R. Greason, ADNET, 28 August 2007.
---------------------------------------------------------------------------- */
void SkyViewer::setTexture(SkyTexture *t)
{
	if( texture )
		disconnect( texture, SIGNAL(retextured()), this, SLOT( updateGL() ) );
	
	texture = t;
	connect( texture, SIGNAL(retextured()), this, SLOT( updateGL() ) );
}
/* ----------------------------------------------------------------------------
'init' is called the first time this widget is displayed, initializing the
widget.

Arguments:
	None.

Returned:
	None.

Written by Nicholas Phillips.
QT4 implementation.  Michael R. Greason, ADNET, 28 August 2007.
---------------------------------------------------------------------------- */
void SkyViewer::init(void)
{
/*
			General configuration.
*/
	setBackgroundColor(QColor(0,0,0));	// Black background.
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//setFPSIsDisplayed(true);
/*
			Define the lighting.
*/
	GLfloat light_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_zero[] = { 0.0, 0.0, 0.0, 1.0 };
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_zero);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_zero);
/*
			Enable GL textures.
*/
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glEnable( GL_TEXTURE_2D );
			// Nice texture coordinate interpolation
	glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
/*

			Configure the camera.
*/
	camera()->setPosition( Vec(3.0,0,0) );
	camera()->lookAt( Vec(0,0,0) );
	camera()->setUpVector(Vec(0,0,1));
	if (constraint != NULL) delete constraint;
	constraint = new CameraConstraint(camera());
	constraint->setRotationConstraintType(AxisPlaneConstraint::FREE);
	camera()->frame()->setConstraint(constraint);
/*
			Configure animation.
*/
	setAnimationPeriod(200);
/*
			Recover the state from a prior run.
*/
	restoreStateFromFile();	
}
/* ----------------------------------------------------------------------------
'draw' is called the whenever the widget needs to be drawn on the screen.

Arguments:
	None.

Returned:
	None.

Written by Nicholas Phillips.
QT4 implementation.  Michael R. Greason, ADNET, 28 August 2007.
---------------------------------------------------------------------------- */
void SkyViewer::draw(void)
{
	if (showrigging) {
		glDisable( GL_TEXTURE_2D );
		rigging->draw();
	}
	if (showtex) {
		bool sr = showrigging;
		showrigging = false;
		if (whiterig != NULL)
		{
			glDisable(GL_TEXTURE_2D);
			if (pulseflg)
			{
				glColor4f(whitecolor.redF(), whitecolor.greenF(), whitecolor.blueF(), 1.);
			}
			else
			{
				glColor4f(blackcolor.redF(), blackcolor.greenF(), blackcolor.blueF(), 1.);
			}
			whiterig->draw();
		}
		if (rigging != NULL)
		{
			glColor4f(1., 1., 1., 1.);
			if (texture != NULL) glEnable( GL_TEXTURE_2D );
			rigging->draw();
		}
		showrigging = sr;
	}
	if ((polar != NULL) && (polar->isOn()) && showpolar) {
		glDisable( GL_TEXTURE_2D );
		polar->draw();
	}
}
/* ----------------------------------------------------------------------------
'animate' is called to perform the next step in animating the viewer.

Arguments:
	None.

Returned:
	None.

Written by Nicholas Phillips.
QT4 implementation.  Michael R. Greason, ADNET, 28 August 2007.
---------------------------------------------------------------------------- */
void SkyViewer::animate (void)
{
	const double llim = 0.3, ulim = 0.9;
	hlite += delhlite;
	if (hlite <= llim)
	{
		hlite    = llim;
		delhlite = -delhlite;
	}
	if (hlite >= ulim)
	{
		hlite    = ulim;
		delhlite = -delhlite;
		pulseflg = pulseflg ? false : true;
	}
	mwin->highlightPixels(hlite);
}
/* ----------------------------------------------------------------------------
'postSelection' is called at the end of the selection process by the QGLViewer
library.

Arguments:
	pt - The point in the widget where the selection took place.

Returned:
	None.

Written by Nicholas Phillips.
QT4 implementation.  Michael R. Greason, ADNET, 30 August 2007.
---------------------------------------------------------------------------- */
void SkyViewer::postSelection (const QPoint &pt)
{
	Vec orig, dir;
	double lambda, phi;
	camera()->convertClickToLine(pt, orig, dir);
	if (! rigging->projectSelection(orig, dir, phi, lambda)) return;
	mwin->selectPixel(phi, lambda);
}
/* ----------------------------------------------------------------------------
'constrainMollweide' constrains the camera if entering a Mollweide projection.

Arguments:
	b - Entering Mollweide?.

Returned:
	None.

Written by Nicholas Phillips.
QT4 implementation.  Michael R. Greason, ADNET, 31 August 2007.
---------------------------------------------------------------------------- */
void SkyViewer::constrainMollweide (bool b)
{
/*
			Mollweide.
*/
	mollview = b;
	if( mollview )
	{
		camera()->setPosition( Vec(-3.0,0,0) );
		camera()->lookAt( Vec(0,0,0) );
		camera()->setUpVector(Vec(0,0,1));
		constraint->setRotationConstraintType(AxisPlaneConstraint::FORBIDDEN);
		camera()->frame()->setConstraint(constraint);
	}
/*
			3D sphere.
*/
	else
	{
		constraint->setRotationConstraintType(AxisPlaneConstraint::FREE);
		constraint->setTranslationConstraintType(AxisPlaneConstraint::FREE);
		camera()->frame()->setConstraint(constraint);
	}
}
/* ----------------------------------------------------------------------------
'helpString' defines the general help string for the application.  Used by
QGLViewer to generate the help window.

Arguments:
	None.

Returned:
	None.

Written by Nicholas Phillips.
QT4 implementation.  Michael R. Greason, ADNET, 30 August 2007.
---------------------------------------------------------------------------- */
QString SkyViewer::helpString () const
{
	QString line, contents;
/*
			Read the file.
*/
	QFile qf(":/general.txt");
	qf.open(QIODevice::ReadOnly | QIODevice::Text);
	QTextStream in(&qf);
	while (!in.atEnd())
	{
		line = in.readLine();
		contents += line;
	}
	qf.close();
/*
			Write its contents.
*/
	return contents;
}
/* ----------------------------------------------------------------------------
'recenterAt' recenters the current view to be centered on the passed
	pixel coordinates. 

Arguments:
	x,y,z: coords of selected pixel
Returned:
	None.

Written by Nicholas Phillips, 10/14/08
---------------------------------------------------------------------------- */
void SkyViewer::recenterAt(const double x_, const double y_, const double z_)
{
	cout << "SkyViewer::recenterAt" << endl;

	cout << "pos:  " << camera()->position();
	cout << "\tlookat:  " << camera()->viewDirection();
	cout << "\tup:  " << camera()->upVector() << endl;

	Vec p(x_,y_,z_);
	Vec z(0,0,1);
	Vec d = z-(p*z)*p;


	cout << "new pos: " << p;
	p *= camera()->position().norm();
	cout << " -> " << p << endl;

	camera()->setPosition( p );
	camera()->setViewDirection(-p);
	camera()->setUpVector( d );
	
	cout << "pos:  " << camera()->position();
	cout << "\tlookat:  " << camera()->viewDirection();
	cout << "\tup:  " << camera()->upVector() << endl;

	cout << "d: " << d << ", d.p: "<< d*p << ",  d.z:  " << d*z << endl;
	cout << endl;

	return;
}
/* ----------------------------------------------------------------------------
'recenterAt' recenters the current view to be centered on the passed
	pixel coordinates. 

Arguments:
	x,y,z: coords of selected pixel
Returned:
	None.

Written by Nicholas Phillips, 10/14/08
---------------------------------------------------------------------------- */
void SkyViewer::recenterAt(const double theta, const double phi)
{
	Vec pos;	// Camera position
	Vec view;	// View direction
	Vec up;		// Up direction for camera
	
	if( mollview ) {
		double phi = asin(cos(theta));
		double lambda = atan2(sin(phi)*sin(theta),cos(phi)*sin(theta));
		double x,y;
		::toMollweide(phi, lambda, x, y);
		//x *= -1;
		::toMollweide(phi,theta,x,y);
		x /= sqrt(2.0);
		y /= sqrt(2.0);
		cout << camera()->position() << "\t" << x << ", "<< y <<endl;
		pos = Vec(camera()->position()[0],x,y);
		view = Vec(-1,0,0);
		up = Vec(0,0,1);
	}
	else {
		Vec z(0,0,1);
		pos = Vec(cos(phi)*sin(theta),sin(phi)*sin(theta),cos(theta));
		up = z-(pos*z)*pos;	// Up direction
		view = -pos;
		pos *= camera()->position().norm();	// Same zoom as before
	}
	camera()->setPosition( pos );
	camera()->setViewDirection( view );
	camera()->setUpVector( up );

	return;
}
