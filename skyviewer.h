#ifndef SKYVIEWER_H
#define SKYVIEWER_H
/* ============================================================================
'skyviewer.h' extends the QGLViewer class.

Written by Nicholas Phillips.
QT4 implementation.  Michael R. Greason, ADNET, 29 December 2006.
============================================================================ */
/*
			Fetch header files.
*/
#include <QGLViewer/qglviewer.h>
#include <QGLViewer/manipulatedCameraFrame.h>
#include <QMouseEvent>
#include "skytexture.h"
#include "rigging.h"
#include "polarargline.h"

class mainWindow;
/* ============================================================================
'SkyViewer' extends the QGLViewer class.
============================================================================ */
class SkyViewer : public QGLViewer
{
    Q_OBJECT
protected:
	qglviewer::CameraConstraint *constraint;

	mainWindow *mwin;

	float hlite;
	float delhlite;

	SkyTexture      *texture;
	Rigging         *rigging;
	Rigging         *whiterig;
	PolarArgLineSet *polar;

	QColor whitecolor, blackcolor;
	bool   pulseflg;

	bool mollview;

	virtual void init(void);
	virtual void draw(void);
	virtual void animate (void);

	virtual void postSelection (const QPoint &pt);
public :
	SkyViewer(QWidget *parent, mainWindow *mw);
	virtual ~SkyViewer ();

	void setTexture      (SkyTexture      *t);
	void setRigging      (Rigging         *r);
	void setWhiteRigging (Rigging         *r);
	void setPolarAngles  (PolarArgLineSet *p);

	void constrainMollweide (bool b);

	void recenterAt(const double theta, const double phi); 
	void recenterAt(const double x, const double y, const double z); 

	virtual QString helpString () const;

};
/* ----------------------------------------------------------------------------
'setRigging' assigns the rigging to be used.

Arguments:
	r - The new rigging.

Returned:
	None.

Written by Nicholas Phillips.
QT4 implementation.  Michael R. Greason, ADNET, 28 August 2007.
---------------------------------------------------------------------------- */
inline void SkyViewer::setRigging(Rigging *r)
{
	rigging = r;
}
/* ----------------------------------------------------------------------------
'setWhiteRigging' assigns the rigging of the underlying white image to be used.

Arguments:
	r - The new rigging.

Returned:
	None.

Written by Nicholas Phillips.
QT4 implementation.  Michael R. Greason, ADNET, 28 August 2007.
---------------------------------------------------------------------------- */
inline void SkyViewer::setWhiteRigging(Rigging *r)
{
	whiterig = r;
}
/* ----------------------------------------------------------------------------
'setPolarAngles' assigns the polarization angles to be displayed.

Arguments:
	p - The polarization angles.

Returned:
	None.

Written by Nicholas Phillips.
QT4 implementation.  Michael R. Greason, ADNET, 28 August 2007.
---------------------------------------------------------------------------- */
inline void SkyViewer::setPolarAngles(PolarArgLineSet *p)
{
	polar = p;
}
#endif
