Skyviewer

This is an OpenGL based program to display HEALPix -based skymaps,
saved in FITS format files. The loaded skymaps can be
viewed either on a 3D sphere or as a Mollweide projection. In either
case, realtime panning and zooming are supported, along with rotations
for the 3D sphere view, assuming you have a strong enough graphics card.

    * Features
    * Requirements
    * Download
    * Compiling
    * Usage
    * Change Log
    * To Do
    * Credits

=> Features

    * Realtime rotation, zooming and panning of map
    * Choice of 3D sphere or 2D Mollweide projection
    * Pixel Texture aligns with HealPix pixel boundries
    * Select pixel and get information for the pixel
    * Adjustable mapping from pixel value to color table range
    * View Temperature, Polarization or NumObs field
    * Save screen shots to image file
    * Contextual Help
    * OS agnostic


=> Requirements

    * Qt <http://www.trolltech.com/products/qt/index.html>, >= v3.3.6; < 4
        Note: This version of skyviewer doesn't work with Qt 4.
    * CFITSIO
      <http://legacy.gsfc.nasa.gov/docs/software/fitsio/fitsio.html>, A
      FITS File Subroutine Library, >= v2.440
    * HEALPix <http://www.eso.org/science/healpix/>, >= v2.01
    * libQGLViewer
      <http://artis.imag.fr/Members/Gilles.Debunne/QGLViewer/>,
      >= v2.2.4

*Note*: For HEALPix, I do not need the entire package, just the C
library. So enter the directory /path/to/Healpix_2.01/src/C/subs/ and
make and make install. This should be enough. Not to say the rest of the
HEALPix package isn't worth building, just that is all I need.


=> Download

Source code:

http://lambda.gsfc.nasa.gov/toolbox/skyviewer/skyviewer-0.2.4.tar.gz

=> Compiling

I have developed this on a Fedora Core 6 system, but this should work with
Windows and Mac OSX as well. At least all the above libraries claim to
be able to be built on them. Let me know if you have any success.

Ungzip and untar the file in a convenient location, qmake and make:

tar xfz skyviewer-0.2.4.tar.gz
cd skyviewer-0.2.4/
qmake
make

If everything goes as it should, you should have the binary *skyviewer*.
Put it where you like to keep your binaries and enjoy looking at skymaps
in full 3D glory. Please email us with any successes or failures you
encounter: Suggestions
<http://lambda.gsfc.nasa.gov/contact/contact.cfm>. We would be delighted
to hear of anyone's success in making builds under either Windows or Mac
OSX.

Can be generated from source via Doxygen (http://www.stack.nl/~dimitri/doxygen/index.html)


=> Usage

Using this program is straightforward. After starting, you are prompted
to select a FITS to load. Once the map is loaded, use the mouse to
rotate/pan/zoom and select display options from the buttons on the left.
Use the *Help*, , and *WhatsThis*, , buttons for further help.

To select pixel, "Shift" + "Left-Mouse-Button". Information will appear
on the left, under the "Sphere Resolution" button.

=> Change Log

Jan 04, 2006

    * Release 0.2.4
    * Fixed jagged edge bug in Mollweide projection

Feb 22, 2005

    * Release: 0.2.2
    * Initial LAMBDA release
    * Minor documentation fixes

Oct 6, 2004

    * Release: 0.2.1
    * "Reload" added
    * Bugfix for identifying Polarization data

Sept 25, 2004
	* version 0.2
	* Texture "aligns" with HealPix pixel boundries
            I have changed the texturing code so the OpenGL texture now
            maps one-to-one with the underlying HealPix pixelization
            scheme. This means each HealPix pixel is now its own
            color-mapped pixel when displayed.
	* For 3D Sphere mode, can select pixels and get information
            Now that each image pixel is a HealPix pixel, I can now map
            the cursor location to the corresponding map pixel. I use
            this to allow the selection of a pixel and then list the
            information of the pixel. Currently, this only works while
            in the 3D Sphere mode. To use, "Shift" + "Left-Mouse-Button".
	* Reload for current map

Mar 30, 2004
    * Initial public release: 0.1


=> To Do

Planning on adding:

    * Overlay coordinate grid
    * Load multiple files, one per tab

Please let us know what you think of this viewer, including suggestions
for improvements and complaints over its use. This is a new project and
we are by no means done.

Suggestions http://lambda.gsfc.nasa.gov/contact/contact.cfm for
Nicholas Phillips



Nicholas Phillips, phillips@friday.gsfc.nasa.gov
UMCP/NASA/GSFC, Greenbelt MD.
