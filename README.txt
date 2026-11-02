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

    * Qt 5 (5.9 or later; tested with 5.15). Qt 6 is NOT supported.
      <https://www.qt.io/>
    * libQGLViewer 2.7.2, built against Qt 5
      <https://github.com/GillesDebunne/libQGLViewer>
    * CFITSIO >= 3.48
      <https://heasarc.gsfc.nasa.gov/fitsio/>
    * HEALPix C library (chealpix) >= 3.6
      <https://healpix.sourceforge.io/>

Only the HEALPix C library is needed, not the whole HEALPix package.

=> Download

Source code:

git clone https://github.com/nasa-lambda/skyviewer.git

=> Compiling

The build has three steps: install the dependencies, build and install
libQGLViewer 2.7.2 against Qt 5, then build Skyviewer.

IMPORTANT: everything must be built with the *Qt 5* qmake. Homebrew and
many Linux distributions now put Qt 6's qmake first on your PATH. If
libQGLViewer gets built with Qt 6, Skyviewer will either fail to build or
crash at startup because it loads both Qt 5 and Qt 6. Run "qmake -v" to see
which Qt a given qmake uses.

   macOS Build Instructions (Homebrew; tested on Apple Silicon with Qt 5.15):

1. Install the Xcode command line tools and Homebrew <https://brew.sh>,
   then the dependencies:

       xcode-select --install
       brew install qt@5 cfitsio healpix

   To save typing, keep the path to the Qt 5 qmake in a shell variable:

       QMAKE5=$(brew --prefix qt@5)/bin/qmake
       $QMAKE5 -v          # should report "Using Qt version 5.x"

2. Build libQGLViewer 2.7.2 and install it into ~/Library/Frameworks
   (this location does not need sudo):

       git clone https://github.com/GillesDebunne/libQGLViewer.git
       cd libQGLViewer
       git checkout v2.7.2
       cd QGLViewer
       $QMAKE5 LIB_DIR=$HOME/Library/Frameworks CONFIG+=sdk_no_version_check
       make
       make install
       cd ../..

   This creates ~/Library/Frameworks/QGLViewer.framework.

3. Build Skyviewer:

       cd skyviewer
       $QMAKE5
       make

   qmake prints the QGLViewer.framework it found. It searches
   ~/Library/Frameworks, /Library/Frameworks, /usr/local/lib and the
   Homebrew lib directory, in that order. If you installed the framework
   somewhere else, give qmake its directory:

       $QMAKE5 QGLVIEWER_DIR=/path/to/dir/containing/QGLViewer.framework

   The build automatically fixes the way skyviewer finds
   QGLViewer.framework at launch, so you do not need install_name_tool.

4. Run it:

       open skyviewer.app --args "$PWD/test_iqu.fits"
   or
       ./skyviewer.app/Contents/MacOS/skyviewer test_iqu.fits

   skyviewer.app uses the Homebrew Qt 5 and the QGLViewer.framework it
   was built against. Leave those installed, and keep the framework where
   it was when you built Skyviewer.

   Linux Build Instructions:

Install the Qt 5 development packages, CFITSIO, the HEALPix C library and
libQGLViewer for Qt 5. On Debian/Ubuntu, for example:

    sudo apt install build-essential qtbase5-dev libqt5opengl5-dev \
        libqglviewer-dev-qt5 libcfitsio-dev libchealpix-dev libglu1-mesa-dev

If your distribution has no Qt 5 libQGLViewer package, build v2.7.2 from
source as in step 2 of the macOS instructions, using the Qt 5 qmake
(often "qmake-qt5" or /usr/lib/qt5/bin/qmake) and "sudo make install"
(the default install location is /usr/local).

Then build Skyviewer:

    cd skyviewer
    qmake-qt5            # or the Qt 5 qmake on your system
    make
    ./skyviewer test_iqu.fits

If the libraries are under a prefix other than /usr/local, add
PREFIX=/that/prefix to the qmake command line. If libQGLViewer-qt5 is in a
nonstandard directory, also add QGLVIEWER_DIR=/path/to/its/lib.

A Linux build with Qt 5.9.7 and libQGLViewer 2.7.2 once failed to link
with an undefined reference to QApplication::clipboard(). The workaround
was to comment out the body of QGLViewer::snapshotToClipboard() in
libQGLViewer. With that change, copying a snapshot of the view to the
clipboard does not work.

   Microsoft Windows Build Instructions:

An older version of Skyviewer was built under Windows XP with an older Qt
and the MinGW environment. The current version has not been tested on
Windows. Install the required libraries, then follow the Linux
instructions.

   Notes:

More recent versions of HEALPix no longer include the mk_xy2pix function.
heal.cpp includes a copy from an earlier version, enabled by the
NEED_MKXY2PIX flag at the top of that file. It is on by default. If you do
not need it, remove the
#define NEED_MKXY2PIX 1
line from the top of heal.cpp.

Please let us know of any successes or failures you encounter:
http://lambda.gsfc.nasa.gov/contact/contact.cfm

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

November 2026
    * Simplified the build on modern macOS (Homebrew, Apple Silicon): skyviewer.pro now
      finds QGLViewer.framework, rejects Qt 6, and fixes the framework install name
      automatically. README has step-by-step install instructions.

July 9, 2020
    * Release 1.1
    * Built against libQGLViewer 2.7.2 and Qt 5.9. This required rewriting the Qt signal/slot connections.

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
