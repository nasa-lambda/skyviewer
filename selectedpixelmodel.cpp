//
// C++ Implementation: selectedpixelmodel
//
// Description: 
//
//
// Author: Nicholas Phillips <Nicholas.G.Phillips@nasa.gov>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include <iostream>
#include <math.h>
#include <QFile>
#include <QTextStream>
#include "selectedpixelmodel.h"
#include "skymap.h"

using namespace std;

SelectedPixelModel::SelectedPixelModel(QObject *parent)
 : QAbstractTableModel(parent)
{
	ncols = 0;
	nrows = 0;
	mode = list;
	data4stats = 0;
	n = 0;
}

int SelectedPixelModel::rowCount(const QModelIndex& /*parent*/) const
{
	return pixs.size();
}

int SelectedPixelModel::columnCount(const QModelIndex& /*parent*/) const
{
	return ncols;
}

QVariant SelectedPixelModel::data(const QModelIndex &index, int role) const
{
	if( !index.isValid() )
		return QVariant();

	if( role==Qt::TextAlignmentRole ) {
		return int(Qt::AlignRight | Qt::AlignVCenter);
	}
	else if( role == Qt::DisplayRole ) {
		switch( mode ) {
			//----------------------------------------------------------------------
			case list:
				if( index.column() == 0 ) {
					return pixs[index.row()].i;
				}
				return (*pixs[index.row()].p)[pidx[index.column()-1]];
				break;
			//----------------------------------------------------------------------
			case stats:
				if( index.column() == 0 ) {
					return statnames[index.row()];
				}
				// There are no stats
				if( n == 0)
					return QVariant();
				// No Std Dev for a single data point
				if( (index.row() == 1) && (n == 1) )
					return QVariant();
				return (*pixs[index.row()].p)[pidx[index.column()-1]];
				break;
			//----------------------------------------------------------------------
			case status:
				int v = (int)(*pixs[index.row()].p)[pidx[index.column()]];
				if( ! v )
					return QVariant();
				if( index.row() == 0 ) {
					return v == 1 ? QString("Present") : QString("Not Present");
				}
				else {
					return pidx[index.column()] != 4 ? QString("Loaded") : QString("Computed");
				}
				break;
		}
	}

	return QVariant();
}

QVariant SelectedPixelModel::headerData(const int i, Qt::Orientation /*o*/, int role) const
{
	if( role==Qt::TextAlignmentRole ) {
		return int(Qt::AlignRight | Qt::AlignVCenter);
	}
	else if( role == Qt::DisplayRole ) {
		return headers[i];
	}
	return QVariant();
}

void SelectedPixelModel::set(Skymap *map)
{
	headers.clear();
	pidx.clear();
	ncols = 1;
	headers << "Pixel";
	if( map->has_Temperature( ) ) {
		headers << "Temp";
		pidx.push_back(0);
		ncols += 1;
	}
	if( map->has_Polarization( ) ) {
		headers << "Q Polar" << "U Polar" << "P Polar";
		pidx.push_back(1);
		pidx.push_back(2);
		pidx.push_back(4);
		ncols += 3;
	}
	if( map->has_Nobs( ) ) {
		headers << "Nobs";
		pidx.push_back(3);
		ncols += 1;
	}

	nrows=0;
	pixs.clear();

	reset();
	return;
}
/*
	return:
		true:	if pixel was set
		false:	if pixel was de-selected
*/
bool SelectedPixelModel::operator()(int i, BasePixel *pix)
{
	for(PixList::iterator pi = pixs.begin();pi != pixs.end(); pi++) {
		if( pi->i == i ) {
			pixs.erase(pi);
			reset();
			return false;
		}
	}
	SelectedPixel sp(i,pix);
	pixs.push_back(sp);
	reset();
	return true;
}
void SelectedPixelModel::clear()
{
	pixs.clear();
	reset();
}

//============================================================================
void SelectedPixelModel::asStats(SelectedPixelModel *data)
{
	mode = stats;
	data4stats = data;
	// Copy Header and indexing info
	headers = data4stats->headers;
	headers[0] = "Stat";
	pidx = data4stats->pidx;
	ncols=data4stats->ncols;
	statnames << "Mean" << "Std Dev" << "Min" << "Max";
	pixs.clear();
	TPnobsPixel *dummy;
	for(int i = 0; i < 4; i++) {
		dummy = new TPnobsPixel;
		SelectedPixel sp(1,dummy);
		pixs.push_back(sp);
	}
	nrows = 4;
	reset();
	//updateStats();
}


void SelectedPixelModel::updateStats()
{
	if( ! data4stats )
		return;
	n = data4stats->pixs.size();
	for(uint i = 0; i < pidx.size(); i++) {
		int j = pidx[i];
		(*pixs[2].p)[j] = (*data4stats->pixs[0].p)[j];
		(*pixs[3].p)[j] = (*data4stats->pixs[0].p)[j];
		double ttl = 0;
		double ttlsqr = 0;
		for(uint k = 0; k < (unsigned int) n; k++) {
			double x = (*data4stats->pixs[k].p)[j];
			if( x < (*pixs[2].p)[j]) (*pixs[2].p)[j] = x;
			if( x > (*pixs[3].p)[j]) (*pixs[3].p)[j] = x;
			ttl += x;
			ttlsqr += x*x;
		}
		(*pixs[0].p)[j] = ttl/n;
		if( n > 1 ) 
			(*pixs[1].p)[j] = sqrt(ttlsqr/n - (ttl/n)*(ttl/n));
	}
	reset();
	return;
}
//============================================================================
void SelectedPixelModel::asStatus()
{
	if( mode != status ) {
		headers.clear();
		pidx.resize(5);
		headers << "Temperature";
		pidx[0] = 0;
		headers << "Q Polar";
		pidx[1] = 1;
		headers << "U Polar";
		pidx[2] = 2;
		headers << "P Polar";
		pidx[3] = 4;
		headers << "Nobs";
		pidx[4] = 3;
		ncols = headers.size();
		pixs.clear();
		TPnobsPixel *dummy;
		for(int i = 0; i < 2; i++) {
			dummy = new TPnobsPixel;
			SelectedPixel sp(1,dummy);
			pixs.push_back(sp);
		}
		nrows = 2;
		mode = status;
	}
	for(int i = 0; i < 2; i++) {
		for(int k = 0; k <  ncols; k++)
			(*pixs[i].p)[pidx[k]] = 0;
	}
	reset();
	
	return;
}

void SelectedPixelModel::hasField(Field f, bool b)
{
	int i = 0;
	switch(f) {
		case I: i = 0;
			break;
		case Q: i = 1;
			break;
		case U: i = 2;
			break;
		case P: i = 3;
			break;
		case Nobs: i = 4;
			break;
	}
	(*pixs[0].p)[pidx[i]] = b ? 1 : -1;
	reset();
	return;
}
void SelectedPixelModel::loadField(Field f)
{
	int i = 0;
	switch(f) {
		case I: i = 0;
			break;
		case Q: i = 1;
			break;
		case U: i = 2;
			break;
		case P: i = 3;
			break;
		case Nobs: i = 4;
			break;
	}
	(*pixs[1].p)[pidx[i]] = 1;
	reset();
	return;
}
void SelectedPixelModel::asStats(Skymap *map)
{
	set(map);
	headers[0] = "Stat";
	statnames << "Mean" << "Std Dev" << "Min" << "Max";
	
	TPnobsPixel *dummy;
	for(int i = 0; i < 4; i++) {
		dummy = new TPnobsPixel;
		SelectedPixel sp(1,dummy);
		pixs.push_back(sp);
	}
	nrows = 4;
	n = map->size();
	for(uint i = 0; i < pidx.size(); i++) {
		int j = pidx[i];
		(*pixs[2].p)[j] = (*map)[0][j];
		(*pixs[3].p)[j] = (*map)[0][j];
		double ttl = 0;
		double ttlsqr = 0;
		for(uint k = 0; k < (unsigned int) n; k++) {
			double x = (*map)[k][j];
			if( x < (*pixs[2].p)[j]) (*pixs[2].p)[j] = x;
			if( x > (*pixs[3].p)[j]) (*pixs[3].p)[j] = x;
			ttl += x;
			ttlsqr += x*x;
		}
		(*pixs[0].p)[j] = ttl/n;
		if( n > 1 ) 
			(*pixs[1].p)[j] = sqrt(ttlsqr/n - (ttl/n)*(ttl/n));
	}
	
	mode = stats;

	reset();
}
//============================================================================
void SelectedPixelModel::writeListToFile (const QString &dst)
{
/*
		Open and configure the output stream.
*/
	int i, j, nr = pixs.size(), nc = ncols - 1;
	QFile file(dst);
	if (! file.open(QIODevice::WriteOnly))
	{
		return;
	}
	QTextStream out(&file);
	out.setFieldAlignment(QTextStream::AlignRight);
	out.setFieldWidth(18);
	out.setRealNumberNotation(QTextStream::ScientificNotation);
	out.setRealNumberPrecision(8);
/*
		List...

			Header.
*/
	out << "Pixel";
	for (i = 1; i < ncols; i++) out << headers[i];
	out << endl;
/*
			Pixels.
*/
	for (j = 0; j < nr; j++)
	{
		out << pixs[j].i;
		for (i = 0; i < nc; i++)
		{
			out << (*pixs[j].p)[pidx[i]];
		}
		out << endl;
	}
/*
			Done!
*/
	file.flush(); 
}
