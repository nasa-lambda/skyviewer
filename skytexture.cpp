/* ============================================================================
'skytexture.cpp' defines the interface that converts a skymap into a texture
usable by the QGLViewer.

Written by Nicholas Phillips.
QT4 implementation.  Michael R. Greason, ADNET, 29 December 2006.
============================================================================ */
/*
			Fetch header files.
*/

#include "skytexture.h"
#include "rangecontrol.h"
#include "heal.h"
#include "map_exception.h"

using namespace std;


/* ----------------------------------------------------------------------------
'SkyViewer' is the class constructor.

Arguments:
	None.

Written by Nicholas Phillips.
QT4 implementation.  Michael R. Greason, ADNET, 28 August 2007.
---------------------------------------------------------------------------- */
SkyTexture::SkyTexture() : texture(0), texture_res(0), nside(0)
{
	hilite_level = 128;
	select_level =  64;
	order = HealpixMap::Undefined;
	timer = new QTimer(this);
	timer->setInterval(50);
	connect(timer, SIGNAL(timeout()), this, SLOT(reTexture()));

}
/* ----------------------------------------------------------------------------
'~SkyViewer' is the class destructor.

Arguments:
	None.

Written by Nicholas Phillips.
QT4 implementation.  Michael R. Greason, ADNET, 28 August 2007.
---------------------------------------------------------------------------- */
SkyTexture::~SkyTexture()
{
	if( texture ) delete [] texture;

}
/* ----------------------------------------------------------------------------
'buildLUT' builds the lookup table for a given map size and pixel ordering.

Arguments:
	ns       - The desired nside.
	ordering - The pixel ordering.

Returned:
	res      - Success?  true=yes.

Written by Nicholas Phillips.
QT4 implementation.  Michael R. Greason, ADNET, 28 August 2007.
---------------------------------------------------------------------------- */
bool SkyTexture::buildLUT(const int ns, HealpixMap::PixOrder ordering)
{
	if (ordering == HealpixMap::Undefined) return false;
	PixLUTCache &lut_cache = (ordering == HealpixMap::Ring)
	                       ? lut_cache_ring : lut_cache_nest;
	PixLUT &lut = lut_cache[ns];
	lut.resize(12*ns*ns);
	int  face = 0, x = 0, y = 0;
	long dy = 4*ns;
	long pix;
	long k = 0;
	for(face = 0; face < 12; face++) {
		long xo = ns*(face % 4);
		long yo = ns*(face / 4);
		long face_offset = face*ns*ns;
		for(y = 0; y < ns; y++) {
			for(x = 0; x < ns; x++) {
				k = x + xo + (y+yo)*dy;
				pix = xy2pix(x,y)  + face_offset;
				if (ordering == HealpixMap::Ring)
					nest2ring(ns,pix,&pix);
				lut[pix] = 4*k;
			}
		}
	}
	return true;
}
/* ----------------------------------------------------------------------------
'getLUT' returns the lookup table for a given map size and pixel ordering.  If
the table isn't defined, create it.

Arguments:
	nside_   - The desired nside.
	ordering - The pixel ordering.

Returned:
	luti     - The lookup table.

Written by Nicholas Phillips.
QT4 implementation.  Michael R. Greason, ADNET, 28 August 2007.
---------------------------------------------------------------------------- */
PixLUTCache::iterator SkyTexture::getLUT(const int ns, HealpixMap::PixOrder ordering)
{
	PixLUTCache::iterator luti;
	if (ordering == HealpixMap::Undefined) return luti;
	PixLUTCache &lut_cache = (ordering == HealpixMap::Ring)
	                       ? lut_cache_ring : lut_cache_nest;

	luti = lut_cache.find(ns);
	if( luti == lut_cache.end() ) buildLUT(ns, ordering);
	luti = lut_cache.find(ns);
	if( luti == lut_cache.end() ) {
		throw MapException(MapException::Other, 0, "SkyTexture::getLUT:Failed, aborting.");
	}
	return luti;
}
/* ----------------------------------------------------------------------------
'set' fills the texture from a skymap and the state of the range dialog.

Arguments:
	None.

Returned:
	Nothing.

Written by Nicholas Phillips.
QT4 implementation.  Michael R. Greason, ADNET, 28 August 2007.
---------------------------------------------------------------------------- */
void SkyTexture::set(HealpixMap *skymap_in, RangeControl *rangedialog)
{
/*
			If there is a painting going on, tell it to stop
			and wait for it to finish
*/
	if( isRunning() ) {
		restart = true;
		wait();
	}
/*
			Retrieve the lookup table.  If it doesn't bomb then assume
			things are fine.
*/
	skymap = skymap_in;
	order = skymap->pixordenum();
	lut = &(getLUT(skymap->nside(), skymap->pixordenum())->second);
/*
			Make sure texture buffer is correct size
*/
	if( skymap->nside() != (unsigned int) nside ) {
		nside = skymap->nside();
		texture_res = 4*nside;
		if( texture) delete[] texture;
		texture = new unsigned char[texture_res*texture_res*4];
	}
/*
			Retrieve the color table, the minimum and maximum, and the 
			map field to display.
*/
	// get fields from RangeDialog
	ct = rangedialog->getColorTable();
	dpyfield = rangedialog->getField();
	minv = rangedialog->getMinimum();
	maxv = rangedialog->getMaximum();
/*
			Start the construction the color table.
*/
	restart = false;
	update = true;
	timer->start();	// no harm if already running
	start();

	return;
}

/* ----------------------------------------------------------------------------
'run' fills the texture from a skymap, as a separate thread from the GUI


Arguments:
	None.

Returned:
	Nothing.

Written by Nicholas Phillips.
---------------------------------------------------------------------------- */
void SkyTexture::run()
{
	float v = 0.0;
	long texk = 0;
	QColor color, blank(255, 255, 255, 255);
	for(uint pix = 0; pix < skymap->size(); pix++) {
		switch (dpyfield)
		{
			case Nobs :			// N Obs
				v = (*skymap)[pix].Nobs();
				break;
			case P :			// P Polarization
				v = (*skymap)[pix].Pmag();
				break;
			case U :			// U Polarization
				v = (*skymap)[pix].U();
				break;
			case Q :			// Q Polarization
				v = (*skymap)[pix].Q();
				break;
			case I :			// Temperature
				v = (*skymap)[pix].T();
				break;
		}
		
		if (v < minv) v = minv;
		if (v > maxv) v = maxv;
		v = (v-minv)/(maxv-minv);
		color = (*ct)(v);
		texk = (*lut)[pix];
		texture[texk++] = color.red();
		texture[texk++] = color.green();
		texture[texk++] = color.blue();
		texture[texk++] = 255;
		if( restart ) {return;}
	}
	update = false;
	return;
}
/* ----------------------------------------------------------------------------
'glTexture' assigns the texture to the OpenGL system.

Arguments:
	None.

Returned:
	Nothing.

Written by Nicholas Phillips.
---------------------------------------------------------------------------- */
void SkyTexture::reTexture()
{
	glTexture();
	emit retextured();
	/*
	if( ! update )
		timer->stop();
	*/
	return;
}
void SkyTexture::glTexture()
{
	glTexImage2D(
		GL_TEXTURE_2D, 0, 4,
		texture_res, texture_res, 0,
		GL_RGBA, GL_UNSIGNED_BYTE,
		texture
	);
	return;
}
/* ----------------------------------------------------------------------------
'highlite' highlights a pixel by setting its transparency.

Arguments:
	pix   - The pixel to tweak.
	alpha - The opacity of the pixel.  1 indicates opaque; 0 indicates
	        transparent.

Returned:
	Nothing.

Written by Nicholas Phillips.
---------------------------------------------------------------------------- */
void SkyTexture::highlite (const int pix, float alpha)
{
	long texk = (*lut)[pix];
	if (alpha < 0.) alpha = 0.;
	if (alpha > 1.) alpha = 1.;
	texture[texk+3] = int(255. * alpha);
	glTexture();
}
