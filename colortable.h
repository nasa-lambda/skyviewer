#ifndef COLORTABLE_H
#define COLORTABLE_H
/* ============================================================================
'colortable.h' defines a pair of classes used to maintain the color tables
required by the application.

Written by Nicholas Phillips.
QT4 adaption and Black/White color table by Michael R. Greason, ADNET,
	27 August 2007.
============================================================================ */
/*
			Fetch header files.
*/
#include <QString>
#include <QColor>
#include <QPixmap>
#include <vector>
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
class ColorTable
{
public:
	ColorTable();
	ColorTable(int id);
	QColor operator[](unsigned int ind) const;
	QColor operator[](float v) const;
	QColor operator()(float v) const;
	int getSize (void) const;
	QString getName() const;
	QPixmap getPixmap();
protected:
	QString name;			//!< Name of the table.
	unsigned int ncols;		//!< Number of colors in this table
	std::vector<QColor> table;	//!< The color table
private:
	void define_table (float intab[][3]);
};
/* ----------------------------------------------------------------------------
'getName' returns the name of the color table.

Arguments:
	None.

Returned:
	name - The name of the table.

Written by Michael R. Greason, ADNET, 27 August 2007.
---------------------------------------------------------------------------- */
inline QString ColorTable::getName() const
{
	return name;
}
/* ----------------------------------------------------------------------------
'getSize' returns the size of the color table.

Arguments:
	None.

Returned:
	size - The number of elements in the table.

Written by Michael R. Greason, ADNET, 27 August 2007.
---------------------------------------------------------------------------- */
inline int ColorTable::getSize(void) const
{
	return ncols;
}
/* ----------------------------------------------------------------------------
'operator[]' returns the indexed element in the table.

Arguments:
	ind - The index; it is forced to fall in bounds.

Returned:
	col - The desired color.

Written by Michael R. Greason, ADNET, 27 August 2007.
---------------------------------------------------------------------------- */
inline QColor ColorTable::operator[](unsigned int ind) const
{
	return table[(ind >= ncols) ? (ncols - 1) : ind];
}
/* ----------------------------------------------------------------------------
'operator()' returns the indexed element in the table.

Arguments:
	v - The 'index'; this is a value between 0 and 1 identifying the 
	    desired color as a fractional position in the table.

Returned:
	col - The desired color.

Written by Michael R. Greason, ADNET, 27 August 2007.
---------------------------------------------------------------------------- */
inline QColor ColorTable::operator()(float v) const
{
	return operator[](v);
}
/* ============================================================================
'ColorTableList' maintains a vector of color tables.
============================================================================ */
class ColorTableList : public std::vector<ColorTable *>
{
public:
	ColorTableList (void);
	~ColorTableList();
	bool set();
};

#endif
