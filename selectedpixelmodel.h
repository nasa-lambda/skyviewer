//
// C++ Interface: selectedpixelmodel
//
// Description: 
//
//
// Author: Nicholas Phillips <Nicholas.G.Phillips@nasa.gov>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SELECTEDPIXELMODEL_H
#define SELECTEDPIXELMODEL_H

#include <vector>
#include <QStringList>
#include <QAbstractTableModel>
#include "enums.h"
#include "pixel.h"

class Skymap;

class SelectedPixel
{
public:
	int i;
	BasePixel *p;
	QString label;
	SelectedPixel(int j, BasePixel *pp) : i(j), p(pp) {};
};
/*==================================================================
	@author Nicholas Phillips <Nicholas.G.Phillips@nasa.gov>
*/
class SelectedPixelModel : public QAbstractTableModel
{
public:
	SelectedPixelModel(QObject *parent = 0);


	// Overloaded methods to display content
	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
	virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
	virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	virtual QVariant headerData(const int i, Qt::Orientation o, int role = Qt::DisplayRole) const;

	void set(Skymap *map);
	void asStats(SelectedPixelModel *);
	void asStats(Skymap *map);

	void asStatus();
	void hasField(Field f, bool b);
	void loadField(Field f);

	bool operator()(int i, BasePixel *pix);

	int size() const { return pixs.size(); };
	int pixnum(int i) const { return pixs[i].i; };

	void clear();

	void updateStats();
	
	void writeListToFile (const QString &dst);
private:
	typedef std::vector<SelectedPixel> PixList;
	enum Modes { list, stats, status };

	Modes mode;

	int nrows;
	int ncols;
	PixList pixs;
	QStringList headers;
	std::vector<int> pidx;

	SelectedPixelModel *data4stats;
	QStringList statnames;
	int n;
};

#endif
