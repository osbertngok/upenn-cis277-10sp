/* the main window.  not much to do here.  */

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
#include <iostream>

#include "CIS277window.h"
#include "Dialogs.h"
#include "MyGLWidget.h"
#include "misc.h"
#include "algebra3.h"
#include "QListWidgetItemPlus.h"

using std::cout;
using std::endl;

CIS277window::CIS277window(QWidget *parent, Qt::WindowFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
  init();
}

CIS277window::~CIS277window()
{

}


void CIS277window::init()
{
  //Something to steal from ui
  QMenuBar* QMB = ui.menubar;
  
  //File Menu
  M_File = new QMenu(QMB);
  M_File->setObjectName(QString::fromUtf8("File"));
  M_File->setTitle(QApplication::translate("CIS277window", "&File", 0));
  QMB->addAction(M_File->menuAction());

  QAction* A_File_Exit = new QAction(this);
  A_File_Exit->setObjectName(QString::fromUtf8("action_File_Exit"));
  A_File_Exit->setText(QApplication::translate("CIS277window","E&xit", 0));
  M_File->addAction(A_File_Exit);
  QObject::connect(A_File_Exit, SIGNAL(triggered()), this, SLOT(close()));
  M_File_vec.push_back(A_File_Exit);

  //Preset Menu
  M_Preset = new QMenu(QMB);
  M_Preset->setObjectName(QString::fromUtf8("Preset"));
  M_Preset->setTitle(QApplication::translate("CIS277window", "&Preset", 0));
  QMB->addAction(M_Preset->menuAction());

  //More Actions in Preset Menu
  cubeDemoMapper = new QSignalMapper(this);
  for (unsigned int i = 0; i < 11; ++i)
  {
    QAction* QA = new QAction(this);
    QA->setObjectName(QString::fromUtf8(("action_Preset_"+to_string(i)).c_str()));
    QA->setText(QApplication::translate("CIS277window", ("Cube: Level "+to_string(i)).c_str(), 0));
    M_Preset->addAction(QA);
    M_Preset_vec.push_back(QA);

    //Signal vs. Slot
    cubeDemoMapper->setMapping(QA,i);
    QObject::connect(QA, SIGNAL(triggered()), cubeDemoMapper, SLOT(map()));
  }
  QObject::connect(cubeDemoMapper, SIGNAL(mapped(int)), ui.widget, SLOT(cubeDemo(int)));

  QObject::connect(this,SIGNAL(IWantAnUpdate()), ui.widget, SLOT(someoneWantsAnUpdate()));


  QObject::connect(ui.widget, SIGNAL(stuffReady(Stuff*)), this, SLOT(stuffReadyHandler(Stuff*)));
  // Faces Menu

  M_Faces = new Q_Face_List_Menu(QMB,this);
  M_Faces->setObjectName(QString::fromUtf8("Faces"));
  M_Faces->setTitle(QApplication::translate("CIS277window", "&Faces", 0));
  QMB->addAction(M_Faces->menuAction());

  // Vertices Menu

  M_Vertices = new Q_Vertex_List_Menu(QMB,this);
  M_Vertices->setObjectName(QString::fromUtf8("Vertices"));
  M_Vertices->setTitle(QApplication::translate("CIS277window", "&Vertices", 0));
  QMB->addAction(M_Vertices->menuAction());

  // HalfEdges Menu

  M_HalfEdges = new Q_HalfEdge_List_Menu(QMB,this);
  M_HalfEdges->setObjectName(QString::fromUtf8("HalfEdges"));
  M_HalfEdges->setTitle(QApplication::translate("CIS277window", "&HalfEdges", 0));
  QMB->addAction(M_HalfEdges->menuAction());

  //Others Functions Menu
  M_Others = new QMenu(QMB);
  M_Others->setObjectName(QString::fromUtf8("Others"));
  M_Others->setTitle(QApplication::translate("CIS277window", "&Others", 0));
  QMB->addAction(M_Others->menuAction());

  QAction* A_Split_Vertex = new QAction(this);
  A_Split_Vertex->setObjectName(QString::fromUtf8("action_Split_Vertex"));
  A_Split_Vertex->setText(QApplication::translate("CIS277window","Split Vertex into Two...", 0));
  M_Others->addAction(A_Split_Vertex);
  QObject::connect(A_Split_Vertex, SIGNAL(triggered()), this, SLOT(splitVertexHandler()));

  QAction* A_Split_Quad = new QAction(this);
  A_Split_Quad->setObjectName(QString::fromUtf8("action_Split_Quad"));
  A_Split_Quad->setText(QApplication::translate("CIS277window","Split Quad into Two...", 0));
  M_Others->addAction(A_Split_Quad);
  QObject::connect(A_Split_Quad, SIGNAL(triggered()), this, SLOT(splitQuadHandler()));
  return;
}

void CIS277window::stuffReadyHandler(Stuff* s)
{
  M_Faces->trashAllFaces();
  M_Faces->buildMenu(s);
  M_Vertices->trashAllVertices();
  M_Vertices->buildMenu(s);
  M_HalfEdges->trashAllHalfEdges();
  M_HalfEdges->buildMenu(s);
  return;
}

void CIS277window::faceColorHandler()
{
  Face* f = ((Q_Face_Menu*)((QAction*)sender())->parent())->CorrFace;
  vec3 Color = f->getColor();
  int R = int(Color[0]*100);
  int G = int(Color[1]*100);
  int B = int(Color[2]*100);
  ColorSelectionDialog CSD(this);
  CSD.setWindowTitle(QApplication::translate("ColorDialog", ("Color of Face "+to_string(f->getId())).c_str(), 0));

  //Sliders
  CSD.ui.RSlider->setMinimum(0);
  CSD.ui.RSlider->setMaximum(100);
  CSD.ui.RSlider->setValue(R);
  CSD.ui.GSlider->setMinimum(0);
  CSD.ui.GSlider->setMaximum(100);
  CSD.ui.GSlider->setValue(G);
  CSD.ui.BSlider->setMinimum(0);
  CSD.ui.BSlider->setMaximum(100);
  CSD.ui.BSlider->setValue(B);

  if (CSD.exec() == QDialog::Accepted )
  {
    R = CSD.ui.RSlider->value();
    G = CSD.ui.GSlider->value();
    B = CSD.ui.BSlider->value();
    f->setColor(vec3((double)R/100,(double)G/100,(double)B/100));
  }
  return;
}

void CIS277window::vertexCoordinateHandler()
{
  Vertex* v = ((Q_Vertex_Menu*)((QAction*)sender())->parent())->CorrVertex;
  int X = int(v->getX()*10);
  int Y = int(v->getY()*10);
  int Z = int(v->getZ()*10);
  PositionDialog PD(this);
  PD.setWindowTitle(QApplication::translate("PositionDialog", ("Position of Vertex "+to_string(v->getId())).c_str(), 0));

  //Sliders
  PD.ui.XSlider->setMinimum(-50);
  PD.ui.XSlider->setMaximum(50);
  PD.ui.XSlider->setValue(X);
  PD.ui.YSlider->setMinimum(-50);
  PD.ui.YSlider->setMaximum(50);
  PD.ui.YSlider->setValue(Y);
  PD.ui.ZSlider->setMinimum(-50);
  PD.ui.ZSlider->setMaximum(50);
  PD.ui.ZSlider->setValue(Z);

  if (PD.exec() == QDialog::Accepted )
  {
    X = PD.ui.XSlider->value();
    Y = PD.ui.YSlider->value();
    Z = PD.ui.ZSlider->value();
    v->setCoord(vec3((double)X/10,(double)Y/10,(double)Z/10));
  }
  return;
}

void CIS277window::vertexDeletionHandler()
{
  //Who is the unlucky Vertex?
  Vertex* v = ((Q_Vertex_Menu*)((QAction*)sender())->parent())->CorrVertex;
  //Let the Stuff handle the Stuff first
  ui.widget->Gift()->trashV(v);
  ui.widget->Gift()->emptyTrash();
  //Lord, we need to rebuild all menus...
  emit IWantAnUpdate();
}

void CIS277window::addVertexHandler()
{
  //Who is the unlucky HalfEdge?
  HalfEdge* he = ((Q_HalfEdge_Menu*)((QAction*)sender())->parent())->CorrHalfEdge;
  //Let the Stuff handle the Stuff first
  ui.widget->Gift()->addVertexToHalfEdge(he);
  //Lord, we need to rebuild all menus...
  emit IWantAnUpdate();
}

void CIS277window::fillFaceHandler()
{
  
  //Who is the unlucky HalfEdge?
  HalfEdge* he = ((Q_HalfEdge_Menu*)((QAction*)sender())->parent())->CorrHalfEdge;
  //Is it a boundary?
  if (he->isBoundary())
  {
    //Let the Stuff handle the Stuff first
    ui.widget->Gift()->fillFace(he,vec3(1,1,1));
    //Lord, we need to rebuild all menus...
    emit IWantAnUpdate();
  }
  else
  {
    QMessageBox::information(this,tr("Error!"),tr("This is not a boundary edge!"));
  }
  return;
}

void CIS277window::splitVertexHandler()
{
  SplitVertexDialog SVD(this);

  list<Face*>& f_vec = ui.widget->Gift()->Faces();
  unsigned int f1_counter = 0;
  for (list<Face*>::iterator i = f_vec.begin(); i != f_vec.end(); ++i)
  {
    QListWidgetItem* p = new QLWI_Face(QString::fromUtf8(("Face "+to_string((*i)->getId())).c_str()),SVD.ui.Face1List,*i);
    SVD.ui.Face1List->insertItem(++f1_counter, p);
  }

  unsigned int f2_counter = 0;
  for (list<Face*>::iterator i = f_vec.begin(); i != f_vec.end(); ++i)
  {
    QListWidgetItem* p = new QLWI_Face(QString::fromUtf8(("Face "+to_string((*i)->getId())).c_str()),SVD.ui.Face2List,*i);
    SVD.ui.Face1List->insertItem(++f2_counter, p);
  }

  list<Vertex*>& v_vec = ui.widget->Gift()->Vertices();
  unsigned int v_counter = 0;
  for (list<Vertex*>::iterator i = v_vec.begin(); i != v_vec.end(); ++i)
  {
    QListWidgetItem* p = new QLWI_Vertex(QString::fromUtf8(("Vertex "+to_string((*i)->getId())).c_str()),SVD.ui.VertexList,*i);
    SVD.ui.VertexList->insertItem(++v_counter, p);
  }

  if (SVD.exec() == QDialog::Accepted)
  {
    // Simple Validation: Did the user select the items?
    QListWidgetItem* a = SVD.ui.Face1List->currentItem();
    if (a == NULL)
    {
      QMessageBox::information(this,tr("Error!"),tr("Please select a Face!"));
      return;
    }
    QListWidgetItem* b = SVD.ui.Face2List->currentItem();
    if (b == NULL)
    {
      QMessageBox::information(this,tr("Error!"),tr("Please select a Face!"));
      return;
    }
    QListWidgetItem* c = SVD.ui.VertexList->currentItem();
    if (c == NULL)
    {
      QMessageBox::information(this,tr("Error!"),tr("Please select a Vertex!"));
      return;
    }
    // Everything seems to be fine. Then...
    bool result = ui.widget->Gift()->splitVertices(((QLWI_Face*)a)->_f,((QLWI_Face*)b)->_f,((QLWI_Vertex*)c)->_v);
    if (result)
      emit IWantAnUpdate(); //Lord, we need to rebuild all menus...
    else
      QMessageBox::information(this,tr("Error!"),tr("Your data don't match!"));
  }
}

void CIS277window::splitQuadHandler()
{
  SplitQuadDialog SQD(this);

  list<Face*>& f_vec = ui.widget->Gift()->Faces();
  unsigned int f_counter = 0;
  for (list<Face*>::iterator i = f_vec.begin(); i != f_vec.end(); ++i)
  {
    QListWidgetItem* p = new QLWI_Face(QString::fromUtf8(("Face "+to_string((*i)->getId())).c_str()),SQD.ui.FaceList,*i);
    SQD.ui.FaceList->insertItem(++f_counter, p);
  }

  list<Vertex*>& v_vec = ui.widget->Gift()->Vertices();
  unsigned int v_counter = 0;
  for (list<Vertex*>::iterator i = v_vec.begin(); i != v_vec.end(); ++i)
  {
    QListWidgetItem* p = new QLWI_Vertex(QString::fromUtf8(("Vertex "+to_string((*i)->getId())).c_str()),SQD.ui.VertexList,*i);
    SQD.ui.VertexList->insertItem(++v_counter, p);
  }

  if (SQD.exec() == QDialog::Accepted)
  {
    // Simple Validation: Did the user select the items?
    QListWidgetItem* a = SQD.ui.FaceList->currentItem();
    if (a == NULL)
    {
      QMessageBox::information(this,tr("Error!"),tr("Please select a Face!"));
      return;
    }
    QListWidgetItem* c = SQD.ui.VertexList->currentItem();
    if (c == NULL)
    {
      QMessageBox::information(this,tr("Error!"),tr("Please select a Vertex!"));
      return;
    }
    // Everything seems to be fine. Then...
    bool result = ui.widget->Gift()->splitQuad(((QLWI_Face*)a)->_f,((QLWI_Vertex*)c)->_v);
    if (result)
      emit IWantAnUpdate(); //Lord, we need to rebuild all menus...
    else
      QMessageBox::information(this,tr("Error!"),tr("Your data don't match!"));
  }
}
