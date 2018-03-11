
#ifndef _DIALOGS_H_
#define _DIALOGS_H_

#include <QtGui/QDialog>
#include "ui_colorSelection.h"
#include "ui_positionSelection.h"
#include "ui_splitVertex.h"
#include "ui_splitQuad.h"

class CIS277window;

class ColorSelectionDialog : public QDialog
{
	Q_OBJECT

public:
	ColorSelectionDialog(QWidget *parent = 0, Qt::WFlags flags = 0);
	~ColorSelectionDialog();

private:
	Ui::ColorDialog ui;
  friend class CIS277window;
};

class PositionDialog : public QDialog
{
	Q_OBJECT

public:
	PositionDialog(QWidget *parent = 0, Qt::WFlags flags = 0);
	~PositionDialog();
  void init();

private:
	Ui::CoordinateDialog ui;
  friend class CIS277window;
};

class SplitVertexDialog : public QDialog
{
	Q_OBJECT

public:
	SplitVertexDialog(QWidget *parent = 0, Qt::WFlags flags = 0);
	~SplitVertexDialog();

private:
	Ui::SplitVDialog ui;
  friend class CIS277window;
};

class SplitQuadDialog : public QDialog
{
	Q_OBJECT

public:
	SplitQuadDialog(QWidget *parent = 0, Qt::WFlags flags = 0);
	~SplitQuadDialog();

private:
	Ui::SplitQDialog ui;
  friend class CIS277window;
};
#endif // _DIALOGS_H_
