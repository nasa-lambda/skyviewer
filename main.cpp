#include <qapplication.h>
#include "mainwindow.h"
/* ------------------------------------------------------------------------------------
The main program.

Written by Nicholas Phillips.
QT4 implementation.  Michael R. Greason, ADNET, 23 August 2007.
------------------------------------------------------------------------------------ */
int main( int argc, char ** argv )
{
	QApplication app( argc, argv );
	mainWindow *w = new mainWindow();
	w->show();
	app.connect( &app, SIGNAL( lastWindowClosed() ), &app, SLOT( quit() ) );
	if (argc > 1) w->readFile(argv[argc-1]);
	return app.exec();
}
