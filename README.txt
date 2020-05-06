Skyviewer

This is an OpenGL based program to display HEALPix -based skymaps, saved
in FITS format files. The loaded skymaps can be viewed either on a 3D
sphere or as a Mollweide projection. In either case, realtime panning and
zooming are supported, along with rotations for the 3D sphere view,
assuming you have a strong enough graphics card.

This version of Skyviewer represents a major redesign and improvement
over previous versions.  Any comments or suggestions will be greatly
appreciated!  Comments can be submitted through:
http://lambda.gsfc.nasa.gov/contact/contact.cfm

    * Features
    * Requirements
    * Download
    * Compiling
    * Usage
    * Change Log
    * Known Bugs
    * To Do
    * Credits

=> Features

    * Realtime rotation, zooming and panning of map
    * Choice of 3D sphere or 2D Mollweide projection
    * Pixel Texture aligns with HealPix pixel boundaries
    * Select pixel and get information for the pixel
    * Adjustable mapping from pixel value to color table range
    * View Temperature, Polarization or NumObs field
    * View polarization direction vectors
    * Save screen shots to image file
    * Contextual Help
    * OS agnostic


=> Requirements

    * Qt <http://www.trolltech.com/products/qt/index.html>, >= v4.4.3
        Note: This version of skyviewer does not work with earlier versions of QT
	(e.g., QT3).
    * CFITSIO
      <http://legacy.gsfc.nasa.gov/docs/software/fitsio/fitsio.html>, A
      FITS File Subroutine Library, >= v2.440
    * HEALPix <http://www.eso.org/science/healpix/>, >= v2.01
    * libQGLViewer
      <http://artis.imag.fr/Members/Gilles.Debunne/QGLViewer/>,
      >= v2.2.4

*Note*: The entire HEALPix package is not needed, just the C library.
Enter the directory /path/to/Healpix_2/src/C/subs/ and make and make
install. This should be enough. Not to say the rest of the HEALPix
package isn't worth building, just that is all that is needed here.

This version has been successfully built against libQGLViewer 2.5.1.

=> Download

Source code:

http://lambda.gsfc.nasa.gov/toolbox/skyviewer/skyviewer-1.0.1.tar.gz

=> Compiling

This version was developed this on a RedHat Enterprise Linux system and
it has been successfully built on a Microsoft Windows XP system and on
a Mac OSX 10.6 system.

   Unix Build Instructions:

Ungzip and untar the file in a convenient location, qmake and make:

tar xfz skyviewer-1.0.1.tar.gz
cd skyviewer-1.0.1/
qmake
make

More recent versions of HEALPIX no longer include the mk_xy2pix function.
It has been included in the heal.cpp file from an earlier version and a 
flag has created at the top of the file (NEED_MKXY2PIX); this is enabled
by default. If not needed or wanted, remove the 
#define NEED_MKXY2PIX 1
line from the top of this file.

If everything goes as it should, you should have the binary *skyviewer*.
Put it where you like to keep your binaries and enjoy looking at skymaps
in full 3D glory. 

   Microsoft Windows Build Instructions:

The first step is to install the various required libraries.  Then unpack
skyviewer and follow the Unix build instructions.

The LAMBDA team has built Skyviewer under Windows and is making this
executable available to any that would like to use it.  This executable was
built with the following tools (built and/or installed in this order):

Microsoft Windows XP SP3
Open Source Qt version 2009.1.10, also installing the bundled MinGW
libQGLViewer 2.3.1
CFITSIO 3.130
HEALPix 2.10
Skyviewer 1.0.0

Several DLL's from Qt and libQGLViewer are also included; these should be
stored either in the same directory as the executable or in the directory:
c:\Windows\System32


Please let us know of any successes or failures you encounter: 
http://lambda.gsfc.nasa.gov/contact/contact.cfm

Mac OSX

Use Fink to install the qt4-mac package.

Install the libGLViewer package normally.  The libQGLViewer library will
be installed as a framework.

Download and build cfitsio and HEALPix.  Be sure to specify the '-arch i386'
in the compile and link commands.

Download and unpack skyviewer.  After executing the qmake command it will
be necessary to edit the Makefile to replace the '-lQGLViewer' link option
with '-framework QGLViewer'.  At this point, the make command should 
successfully build the application.

=> Usage

The program can be started with a FITS filename as an argument, which
will be opened and displayed. Or if no filename is given, a File Dialog
with open which can be used to select a file to view. Two windows will be
present: the main Skyviewer window and a  Control/Information window. The
top menu of the Skyviewer window has a "Help" button that will provide
information on how to use the mouse and keyboard to navigate the view.
Also present are buttons for loading a new file, reloading the current
file, saving an image file of the current view, changing the projection,
help and exiting.

The Control/Information window is used for adjusting how the skymap is
displayed, displaying information of selected Healpix pixels and
information for the loaded file.

The Range tab on the Control/Information window is one of the main
changes for this release. It controls how the selected skymap field is
mapped to the selected Color Map. The top of the window shows a histogram
of the values in the selected field, colored as they appear in the
Skyview window. Vertical lines at either end of the window show the lower
and upper limits of the current range. The current range can be changed
via the "Lower", "Upper", "Zoom" and "Center" sliders. There are also
presets for both the Zoom and Center, available via drop-down menus.

The sliders work by pulling them away from their center positions. The
further from the center they are pulled, the quicker the selected value
changes. As soon as any slider is released, the value it controls stops
changing. This way, both small and large value changes are readily made.

From the Range tab, the projection, display field and color map can
be selected. The fineness of the displayed sphere's polygon is controlled
with the "Rigging" menu. The display of Polarization Vectors presently
only works with the 3D Sphere projection.

The Selected tab for the Control/Information window shows the pixel
values for those pixels that have been selected in the Skyview window. To
select a pixel in the Skyviewer window, use "Shift-Left click" on the
mouse. The selected pixel will then be high-lighted and added to the list
of pixels. "Shift-Left click" on a selected will remove it from the
list. Also displayed on the Selected tab are common statistics for the
list of selected pixels.

When viewing the list of selected pixels, individual pixels can be
selected with the mouse. This list has the usual "Shift-click" to select
a range of values and "Ctrl-click" to add/remove from the selection set
behavior. The selected entries in the list can then be removed from the
list. To have the Skyviewer window center on one of the selected
pixels,just "Double-Left-click"on the entry in the list of selected
pixels.

The list of selected pixel values can be saved to an ASCII file as 
multi-column list, with headers.

=> Change Log

July 3, 2014
    * Release 1.0.1
    * Builds against libQGLViewer 2.5.1; this required adding an explicit reference to this library's
      manipulatedCameraFrame.h header file.
    * Builds against HEALpix 3.1.1. This required adding mk_xy2pix from an earlier version (see above).

Mar 12, 2009
    * Microsoft Windows XP Port

Feb 06, 2009
    * Release 1.0.0
    * Conversion to QT 4
    * Multiple pixel selection
    * Complete redesign of the user interface

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
	* Texture "aligns" with HealPix pixel boundaries
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


=> Known Bugs



=> To Do

Planning on adding:

    * Overlay coordinate grid
    * Load multiple files
    * Tie the views of multiple files together using one range
      selection widget and one camera.

Please let us know what you think of this viewer, including suggestions
for improvements and complaints over its use.

Suggestions http://lambda.gsfc.nasa.gov/contact/contact.cfm



Nicholas G. Phillips
UMCP/NASA/GSFC, Greenbelt MD.

Michael R. Greason
ADNET Systems/NASA/GSFC, Greenbelt MD.
