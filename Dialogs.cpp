#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>
#include <QtWidgets/QDialog>
#include <iostream>

#include "Dialogs.h"
#include "misc.h"

using std::cout;
using std::endl;

ColorSelectionDialog::ColorSelectionDialog(QWidget *parent, Qt::WindowFlags flags)
	: QDialog(parent, flags)
{
	ui.setupUi(this);
}

ColorSelectionDialog::~ColorSelectionDialog()
{

}

PositionDialog::PositionDialog(QWidget *parent, Qt::WindowFlags flags)
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

SplitVertexDialog::SplitVertexDialog(QWidget *parent, Qt::WindowFlags flags)
	: QDialog(parent, flags)
{
	ui.setupUi(this);
}

SplitVertexDialog::~SplitVertexDialog()
{

}

SplitQuadDialog::SplitQuadDialog(QWidget *parent, Qt::WindowFlags flags)
	: QDialog(parent, flags)
{
	ui.setupUi(this);
}

SplitQuadDialog::~SplitQuadDialog()
{

}
