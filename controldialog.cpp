/* ====================================================================================
The definition of the range control window class.

Written by Michael R. Greason, ADNET, 23 August 2007.
==================================================================================== */
/*
			Fetch header files.
*/

#include <QMessageBox>
#include <QFileDialog>
#include <QString>
#include "skymap.h"
#include "healpixmap.h"
#include "selectedpixelmodel.h"
#include "controldialog.h"

using namespace std;
/* ------------------------------------------------------------------------------------
The class constructor.

Arguments:
	parent - The parent wiget/window.

NGP, 8/14/08
------------------------------------------------------------------------------------ */
ControlDialog::ControlDialog(QWidget *parent) : QDialog(parent) 
{
	setupUi(this);
	range->setVisible(true);
	pixlistview->setModel(&selectedpixels);
	clearSelection = pixlistview->selectionModel();

	connect (clearSelection, static_cast<void(QItemSelectionModel::*)(const QItemSelection &, const QItemSelection &)>(&QItemSelectionModel::selectionChanged),
	         this,           static_cast<void(ControlDialog::*)(const QItemSelection &, const QItemSelection &)>(&ControlDialog::updateClearSelection));
	connect (pixlistview, static_cast<void(QTreeView::*)(const QModelIndex &)>(&QTreeView::doubleClicked),
	         this,        static_cast<void(ControlDialog::*)(const QModelIndex &)>(&ControlDialog::doubleClicked));

	pixstatslistview->setModel(&statspixels);
	statslistview->setModel(&mapstats);

};

/* ------------------------------------------------------------------------------------
'clearStatus' Clear the total map status fields and set state to reflect no file

Nicholas Phillips, 8/14/08
------------------------------------------------------------------------------------*/
void ControlDialog::clearStatus()
{
	loading->setText("Waiting");
	filename->clear();
	nside->setText("Nside: --");
	ordering->setText("Ordering:  --");
	mapstats.asStatus();
	return;
}


/* ------------------------------------------------------------------------------------
'startFile' Set up to start receiving signals about loading a file

Nicholas Phillips, 8/14/08
------------------------------------------------------------------------------------*/
void ControlDialog::startFile(QString fname)
{
	clearStatus();
	loading->setText("Loading");
	filename->setText(fname);

};
void ControlDialog::loadFile(const char *fname)
{
	loadFile(QString(fname));
}
void ControlDialog::loadFile(QString fname)
{
	startFile( fname );
}

void ControlDialog::hasField(Field f, bool b)
{
	mapstats.hasField(f, b);
	return;
}
void ControlDialog::loadField(Field f)
{
	mapstats.loadField(f);
	return;
}
void ControlDialog::loadNSide(int ns, int ord)
{
	QString str;
	nside->setText(QString("Nside:  %1;").arg(ns));
	ordering->setText(str.FROMASCII((ord == 2) ? "Ordering:  RING" : "Ordering:  NESTED"));
}
/* ------------------------------------------------------------------------------------
'setStats' Display the stats for the given map

Nicholas Phillips, 8/14/08
------------------------------------------------------------------------------------*/
void ControlDialog::finished(Skymap *m)
{
	mapstats.asStats(m);

	return;
}

/* ------------------------------------------------------------------------------------*/
void ControlDialog::init(Skymap *map)
{
	range->init(map);

	selectedpixels.set(map);
	statspixels.asStats(&selectedpixels);
	//statspixels.updateStats();
	for(int i = 0; i < selectedpixels.columnCount(); i++) {
		pixlistview->setColumnWidth(i,85);
		pixstatslistview->setColumnWidth(i,85);
	}
	clearsel->setEnabled(false);
	clearall->setEnabled(false);
	savelist->setEnabled(false);
	return;
}
/* ------------------------------------------------------------------------------------*/
bool ControlDialog::selectPixel(int i, BasePixel *pix)
{
	bool set = selectedpixels(i,pix);
	clearall->setEnabled(selectedpixels.rowCount() > 0);
	savelist->setEnabled(selectedpixels.rowCount() > 0);
	statspixels.updateStats();
	return set;
}

/* ------------------------------------------------------------------------------------*/
void ControlDialog::on_clearsel_clicked()
{
	QModelIndexList indexes = clearSelection->selectedIndexes();
	QModelIndex index;

	// build the list of skymap pixel numbers to clear
	vector<int> clearpixs;
	foreach(index, indexes) {
		if( index.column() == 0 ) 
			clearpixs.push_back(selectedpixels.pixnum(index.row()));
	}
	// remove from the collection of selected pixels
	BasePixel dummy;
	for(uint i = 0; i < clearpixs.size(); i++)
		selectPixel(clearpixs[i],&dummy);
	clearsel->setEnabled(false);
	// tell the main window to reset their highlight
	emit resetPixels(clearpixs);
	return;
}
/* ------------------------------------------------------------------------------------*/
void ControlDialog::on_clearall_clicked()
{
	vector<int> clearpixs(selectedpixels.size());
	for(int i = 0; i < selectedpixels.size(); i++)
		clearpixs[i] = selectedpixels.pixnum(i);
	selectedpixels.clear();
	clearsel->setEnabled(false);
	clearall->setEnabled(false);
	savelist->setEnabled(false);
	emit resetPixels(clearpixs);
}
/* ------------------------------------------------------------------------------------*/
void ControlDialog::on_savelist_clicked()
{
	QString fname = QFileDialog::getSaveFileName(this, 
		tr("Save Pixel List"), tr("."), tr("*"));
	if (fname.isEmpty()) return;
	selectedpixels.writeListToFile(fname);
}
/* ------------------------------------------------------------------------------------*/
void ControlDialog::updateClearSelection(const QItemSelection& /*selected*/, const QItemSelection& /*deselected*/)
{
	clearsel->setEnabled( clearSelection->selectedIndexes().size() > 0 );
}


/* ------------------------------------------------------------------------------------*/
void ControlDialog::doubleClicked(const QModelIndex & index )
{
	QModelIndex idx = selectedpixels.index(index.row(),0);
	emit recenterOnPixel(selectedpixels.data(idx,Qt::DisplayRole).toInt());
	return;
}
