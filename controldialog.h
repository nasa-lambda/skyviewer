#ifndef CONTROLDIALOG_H
#define CONTROLDIALOG_H
/* ====================================================================================
The definition of the  control dialog class.

==================================================================================== */
/*
			Fetch header files.
*/
#include <vector>
#include "enums.h"
#include "ui_controldialog.h"
#include "selectedpixelmodel.h"

class RangeControl;
class Skymap;
//class SelectedPixelModel;

class ControlDialog : public QDialog, private Ui::ControlDialog
{
	Q_OBJECT
public:
	ControlDialog(QWidget *parent = 0);

	// Give mainWindow access to RangeControl Widget 
	RangeControl *getRangeControl() { return range; };

	void clearStatus();
	void loadFile(const char *);
	void loadFile(QString );
	void startFile(QString );
	void hasField(Field, bool);
	void loadField(Field);
	void loadNSide(int nside, int ordering);
	void finished(Skymap *);

	// initialize and associate with a given sky map
	void init(Skymap *map);

	bool selectPixel(int i, BasePixel *pix);
	int pixnum(int i);
	int numselected();

signals:
	void resetPixels(std::vector<int>);
	void recenterOnPixel(int pixnum);
	
private:
	SelectedPixelModel selectedpixels;
	QItemSelectionModel *clearSelection;
	SelectedPixelModel statspixels;
	SelectedPixelModel mapstats;

private slots:
	void on_clearsel_clicked();
	void on_clearall_clicked();
	void on_savelist_clicked();
	void updateClearSelection(const QItemSelection &selected, const QItemSelection &deselected);

	void doubleClicked(const QModelIndex & index );

	
};

inline int ControlDialog::numselected()
{
	return selectedpixels.size();
}

inline int ControlDialog::pixnum(int i)
{
	return selectedpixels.pixnum(i);
}
#endif
