#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMessageBox>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QWidget>
#include <QtGui/QDialog>
#include <iostream>

#include "Dialogs.h"
#include "misc.h"

using std::cout;
using std::endl;

ColorSelectionDialog::ColorSelectionDialog(QWidget *parent, Qt::WFlags flags)
	: QDialog(parent, flags)
{
	ui.setupUi(this);
}

ColorSelectionDialog::~ColorSelectionDialog()
{

}

PositionDialog::PositionDialog(QWidget *parent, Qt::WFlags flags)
	: QDialog(parent, flags)
{
	ui.setupUi(this);
  init();
}

PositionDialog::~PositionDialog()
{

}

void PositionDialog::init()
{
  
  QObject::connect(ui.XSlider, SIGNAL(valueChanged(int)), ui.XLabel, SLOT(setNum(int)));
  QObject::connect(ui.YSlider, SIGNAL(valueChanged(int)), ui.YLabel, SLOT(setNum(int)));
  QObject::connect(ui.ZSlider, SIGNAL(valueChanged(int)), ui.ZLabel, SLOT(setNum(int)));
  return;
}

SplitVertexDialog::SplitVertexDialog(QWidget *parent, Qt::WFlags flags)
	: QDialog(parent, flags)
{
	ui.setupUi(this);
}

SplitVertexDialog::~SplitVertexDialog()
{

}

SplitQuadDialog::SplitQuadDialog(QWidget *parent, Qt::WFlags flags)
	: QDialog(parent, flags)
{
	ui.setupUi(this);
}

SplitQuadDialog::~SplitQuadDialog()
{

}
