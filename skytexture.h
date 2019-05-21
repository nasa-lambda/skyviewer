#ifndef SKYTEXTURE_H
#define SKYTEXTURE_H
/* ============================================================================
'skytexture.h' defines the interface that converts a skymap into a texture
usable by the QGLViewer.

Written by Nicholas Phillips.
QT4 implementation.  Michael R. Greason, ADNET, 29 December 2006.
============================================================================ */
/*
			Fetch header files.
*/
#include <vector>
#include <map>
#include <qtimer.h>
#include <qthread.h>
#include <QGLViewer/qglviewer.h>
#include "healpixmap.h"
#include "enums.h"

class ColorTable;
class RangeControl;
/*
			Typedefs.
*/
typedef std::vector<long> PixLUT;			// Skymap->texture lookup table.
typedef std::map<int, PixLUT > PixLUTCache; 		// Cache of computed texture look-up
							// tables.

/* ============================================================================
'SkyTexture' defines the interface that converts a skymap into a texture
usable by the QGLViewer.
============================================================================ */
class SkyTexture : public QThread
{
	Q_OBJECT
private:
	unsigned char  hilite_level;
	unsigned char  select_level;
	unsigned char *texture;			// Texture representation of skymap
	int            texture_res;		// Resolution per side of texture
	int            nside;			// HealPix resolution parameter for texture.
	HealpixMap::PixOrder order; 		// The HealPix ordering scheme.
	PixLUT        *lut;			// Pointer to current look-up table
	PixLUTCache    lut_cache_ring;		// Cache of ring-ordered look-up tables
	PixLUTCache    lut_cache_nest;		// Cache of nest-ordered look-up tables
	HealpixMap *skymap;				// Skymap to use
	ColorTable *ct;				// The color table to use
	int    dpyfieldold;			// The display fiels to use
	Field  dpyfield;			// The display fiels to use
	double minv;				// The minimum display value
	double maxv;				// The maximum display value
	bool restart;				// set when current repaint should stop

	QTimer *timer;				// Controls how often to update GL
	bool update;				// true while there is still a need to update

	bool buildLUT(const int ns, HealpixMap::PixOrder ordering);
	PixLUTCache::iterator getLUT(const int ns, HealpixMap::PixOrder ordering);

protected:
	void run();

protected slots:
	void reTexture(void);			// send the texture to GL

public :
	SkyTexture();
	virtual ~SkyTexture();

	void set(HealpixMap *skymap, RangeControl *rangecontrol);

	void glTexture();
	void highlite (const int pix, float alpha = 1.);

signals:
	void retextured();			// emitted every time the texture is sent to GL
};
#endif
