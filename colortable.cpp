/* ============================================================================
'colortable.cpp' defines a pair of classes used to maintain the color tables
required by the application.

Written by Nicholas Phillips.
QT4 adaption and Black/White color table by Michael R. Greason, ADNET,
	27 August 2007.
============================================================================ */
/*
			Fetch header files.
*/
#include <iostream>
#include "colortable.h"
#include "define_colortable.h"

using namespace std;
/* ============================================================================
'ColorTable' maintains a single color table.  Each element represents one
usable color; it may be accessed either by indexing it directly or by supplying
a value from 0--1.

The color tables currently supported are staticly defined in 
'define_colortable.h'.

TBD:
	- Allow the setting of color tables from image files.
	- Return a pixmap of the color table.
============================================================================ */
/* ----------------------------------------------------------------------------
'ColorTable' is the default class constructor; it defines the default color
table in the new instance.

Arguments:
	None.

Written by Michael R. Greason, ADNET, 27 August 2007.
---------------------------------------------------------------------------- */
ColorTable::ColorTable()
{
	name = default_ct_name;
	define_table(default_colortable);
	return;
}
/* ----------------------------------------------------------------------------
'ColorTable' is a class constructor; it defines the 'indexed' color table from
'define_colortable.h'

Arguments:
	id - The color table in the file to define:
			0 - default
			1 - black-white

Written by Michael R. Greason, ADNET, 27 August 2007.
---------------------------------------------------------------------------- */
ColorTable::ColorTable(int id)
{
	switch (id)
	{
	  case 1:
		name = blkwht_ct_name;
		define_table(blkwht_colortable);
		break;
	  default:
		name = default_ct_name;
		define_table(default_colortable);
		break;
	}
	return;
}
/* ----------------------------------------------------------------------------
'define_table' performs the work in defining a color table from
'define_colortable.h'.  This routine does NOT define the name of the table.

Arguments:
	intab - The desired color table as a float array.

Returned:
	Nothing.

Written by Michael R. Greason, ADNET, 27 August 2007.
---------------------------------------------------------------------------- */
void ColorTable::define_table (float intab[][3])
{
	for(ncols = 0; intab[ncols][0] != -1; ncols++);
	table.resize(ncols);
	for(uint i = 0; i < ncols; i++)
		table[i].setRgbF(intab[i][0], intab[i][1], intab[i][2]);
}
/* ----------------------------------------------------------------------------
'operator[]' returns the indexed element in the table.

Arguments:
	v - The 'index'; this is a value between 0 and 1 identifying the 
	    desired color as a fractional position in the table.

Returned:
	col - The desired color.

Written by Michael R. Greason, ADNET, 27 August 2007.
---------------------------------------------------------------------------- */
QColor ColorTable::operator[](float v) const
{
	v = v < 0 ? 0 : v;
	v = v > 1 ? 1 : v;
	uint idx = (uint)(v*(ncols-1));
	//cout << idx;
	return table[idx];
}
/* ----------------------------------------------------------------------------
'getPixmap' returns a pixmap of the color table.

NOT CURRENTLY DEFINED

Arguments:
	None.

Returned:
	pm - The pixmap.

Written by Michael R. Greason, ADNET, 27 August 2007.
---------------------------------------------------------------------------- */
QPixmap ColorTable::getPixmap()
{
	cout << "ColorTable::getPixmap(): Not implemented yet!" << endl;
	return QPixmap();
}
/* ============================================================================
'ColorTableList' maintains a vector of color tables.
============================================================================ */
/* ----------------------------------------------------------------------------
'ColorTableList' is the default class constructor, defining all the color
tables defined in 'define_colortable.h'.

Arguments:
	None.

Written by Michael R. Greason, ADNET, 27 August 2007.
---------------------------------------------------------------------------- */
ColorTableList::ColorTableList(void)
{
	set();
}
/* ----------------------------------------------------------------------------
'~ColorTableList' is class destructor, destroying the color tables before
this instance is destroyed.

Arguments:
	None.

Written by Michael R. Greason, ADNET, 27 August 2007.
---------------------------------------------------------------------------- */
ColorTableList::~ColorTableList()
{
	for( iterator cti = begin(); cti != end(); ++cti){
		delete *cti;
	}
	return;
}
/* ----------------------------------------------------------------------------
'set' defines all the color tables defined in 'define_colortable.h'.

Arguments:
	None.

Returned:
	flg - true if successful.

Written by Michael R. Greason, ADNET, 27 August 2007.
---------------------------------------------------------------------------- */
bool ColorTableList::set()
{
	ColorTable *ct;
	clear();
	for (int i = 0; i < colortable_count; i++)
	{
		ct = new ColorTable(i);
		push_back(ct);
	}
	return true;
}
