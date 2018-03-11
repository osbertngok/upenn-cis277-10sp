#include "Menus.h"
Q_Face_List_Menu::Q_Face_List_Menu(QWidget* parent, QWidget* window)
:QMenu(parent),
_window(window)
{

}

void Q_Face_List_Menu::deleteAllFaces()
{
  clear();
  return;
}
void Q_Face_List_Menu::trashAllFaces()
{
  clear();
  return;
}
void Q_Face_List_Menu::buildMenu(Stuff* s)
{
  list<Face*>& f_vec = s->Faces();
  for (list<Face*>::iterator i = f_vec.begin(); i != f_vec.end(); ++i)
  {
    QMenu* p = new Q_Face_Menu(this, _window, *i);
    addAction(p->menuAction());
  }
  return;
}

void Q_Face_List_Menu::init(Stuff* s)
{
  trashAllFaces();
  buildMenu(s);
  return;
}

QWidget* Q_Face_List_Menu::getWinP() const
{
  return _window;
}

Q_Face_Menu::Q_Face_Menu(QWidget* parent, QWidget* window, Face* CFace)
:QMenu(parent),
CorrFace(CFace),
_window(window)
{
  setObjectName(QString::fromUtf8(("Face "+to_string(CFace->getId())).c_str()));
  setTitle(QApplication::translate("CIS277window", ("Face "+to_string(CFace->getId())).c_str(), 0));

  QAction* changeColor = new QAction(this);
  changeColor->setObjectName(QString::fromUtf8("changeColor"));
  changeColor->setText(QApplication::translate("CIS277window","Change Color", 0));
  addAction(changeColor);
  QObject::connect(changeColor, SIGNAL(triggered()), _window, SLOT(faceColorHandler()));
}

QWidget* Q_Face_Menu::getWinP() const
{
  return _window;
}


Q_Vertex_List_Menu::Q_Vertex_List_Menu(QWidget* parent, QWidget* window)
:QMenu(parent),
_window(window)
{

}

void Q_Vertex_List_Menu::deleteAllVertices()
{
  clear();
  return;
}
void Q_Vertex_List_Menu::trashAllVertices()
{
  clear();
  return;
}
void Q_Vertex_List_Menu::buildMenu(Stuff* s)
{
  list<Vertex*>& f_vec = s->Vertices();
  for (list<Vertex*>::iterator i = f_vec.begin(); i != f_vec.end(); ++i)
  {
    QMenu* p = new Q_Vertex_Menu(this, _window, *i);
    addAction(p->menuAction());
  }
  return;
}

void Q_Vertex_List_Menu::init(Stuff* s)
{
  trashAllVertices();
  buildMenu(s);
  return;
}

QWidget* Q_Vertex_List_Menu::getWinP() const
{
  return _window;
}

Q_Vertex_Menu::Q_Vertex_Menu(QWidget* parent, QWidget* window, Vertex* CVertex)
:QMenu(parent),
CorrVertex(CVertex),
_window(window)
{
  setObjectName(QString::fromUtf8(("Vertex "+to_string(CVertex->getId())).c_str()));
  setTitle(QApplication::translate("CIS277window", ("Vertex "+to_string(CVertex->getId())).c_str(), 0));

  //change Coordinate
  QAction* changeCoordinate = new QAction(this);
  changeCoordinate->setObjectName(QString::fromUtf8("changeCoordinate"));
  changeCoordinate->setText(QApplication::translate("CIS277window","Change Coordinate", 0));
  addAction(changeCoordinate);
  QObject::connect(changeCoordinate, SIGNAL(triggered()), _window, SLOT(vertexCoordinateHandler()));

  //delete Vertex
  QAction* delVertex = new QAction(this);
  delVertex->setObjectName(QString::fromUtf8("delVertex"));
  delVertex->setText(QApplication::translate("CIS277window","Delete", 0));
  addAction(delVertex);
  QObject::connect(delVertex, SIGNAL(triggered()), _window, SLOT(vertexDeletionHandler()));
}

QWidget* Q_Vertex_Menu::getWinP() const
{
  return _window;
}

Q_HalfEdge_List_Menu::Q_HalfEdge_List_Menu(QWidget* parent, QWidget* window)
:QMenu(parent),
_window(window)
{

}

void Q_HalfEdge_List_Menu::deleteAllHalfEdges()
{
  clear();
  return;
}
void Q_HalfEdge_List_Menu::trashAllHalfEdges()
{
  clear();
  return;
}
void Q_HalfEdge_List_Menu::buildMenu(Stuff* s)
{
  list<HalfEdge*>& f_vec = s->HalfEdges();
  for (list<HalfEdge*>::iterator i = f_vec.begin(); i != f_vec.end(); ++i)
  {
    QMenu* p = new Q_HalfEdge_Menu(this, _window, *i);
    addAction(p->menuAction());
  }
  return;
}

void Q_HalfEdge_List_Menu::init(Stuff* s)
{
  trashAllHalfEdges();
  buildMenu(s);
  return;
}

QWidget* Q_HalfEdge_List_Menu::getWinP() const
{
  return _window;
}

Q_HalfEdge_Menu::Q_HalfEdge_Menu(QWidget* parent, QWidget* window, HalfEdge* CHalfEdge)
:QMenu(parent),
CorrHalfEdge(CHalfEdge),
_window(window)
{
  setObjectName(QString::fromUtf8(("HalfEdge "+to_string(CHalfEdge->getId())).c_str()));
  setTitle(QApplication::translate("CIS277window", ("HalfEdge "+to_string(CHalfEdge->getId())).c_str(), 0));

  //addVertex
  QAction* addVertex = new QAction(this);
  addVertex->setObjectName(QString::fromUtf8("addVertex"));
  addVertex->setText(QApplication::translate("CIS277window","Add Vertex", 0));
  addAction(addVertex);
  QObject::connect(addVertex, SIGNAL(triggered()), _window, SLOT(addVertexHandler()));

  //fillFace
  if (CorrHalfEdge->isBoundary())
  {
    QAction* fillFace = new QAction(this);
    fillFace->setObjectName(QString::fromUtf8("fillFace"));
    fillFace->setText(QApplication::translate("CIS277window","Fill Face", 0));
    addAction(fillFace);
    QObject::connect(fillFace, SIGNAL(triggered()), _window, SLOT(fillFaceHandler()));
  }
}

QWidget* Q_HalfEdge_Menu::getWinP() const
{
  return _window;
}
