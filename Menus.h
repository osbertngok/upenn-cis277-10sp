#ifndef _MENUS_H_
#define _MENUS_H_
#include <QMenu>
#include <QAction>
#include <QApplication>
#include "Stuff.h"
#include "misc.h"

class Q_Face_List_Menu;
class Q_Face_Menu;

class Q_Vertex_List_Menu;
class Q_Vertex_Menu;

class Q_HalfEdge_List_Menu;
class Q_HalfEdge_Menu;

class Q_Face_List_Menu : public QMenu
{
  Q_OBJECT
  public:
    Q_Face_List_Menu(QWidget* parent, QWidget* window);
    void deleteAllFaces();
    void trashAllFaces();
    void buildMenu(Stuff* s);
    void init(Stuff* s);
    QWidget* getWinP() const;
  signals:
    void deleteFace(Face* p);
  public slots:

  private:
    QWidget* _window;
    
};

class Q_Face_Menu : public QMenu
{
  Q_OBJECT
  public:
    Q_Face_Menu(QWidget * parent, QWidget* window, Face * CFace);
    Face* CorrFace;
    QWidget* getWinP() const;
  private:
    QWidget* _window;
};

class Q_Vertex_List_Menu : public QMenu
{
  Q_OBJECT
  public:
    Q_Vertex_List_Menu(QWidget* parent, QWidget* window);
    void deleteAllVertices();
    void trashAllVertices();
    void buildMenu(Stuff* s);
    void init(Stuff* s);
    QWidget* getWinP() const;
  signals:
    void deleteVertex(Vertex* p);
  public slots:

  private:
    QWidget* _window;
    
};

class Q_Vertex_Menu : public QMenu
{
  Q_OBJECT
  public:
    Q_Vertex_Menu(QWidget * parent, QWidget* window, Vertex * CVertex);
    Vertex* CorrVertex;
    QWidget* getWinP() const;
  private:
    QWidget* _window;
};

class Q_HalfEdge_List_Menu : public QMenu
{
  Q_OBJECT
  public:
    Q_HalfEdge_List_Menu(QWidget* parent, QWidget* window);
    void deleteAllHalfEdges();
    void trashAllHalfEdges();
    void buildMenu(Stuff* s);
    void init(Stuff* s);
    QWidget* getWinP() const;
  signals:
    void deleteHalfEdge(HalfEdge* p);
  public slots:

  private:
    QWidget* _window;
    
};

class Q_HalfEdge_Menu : public QMenu
{
  Q_OBJECT
  public:
    Q_HalfEdge_Menu(QWidget * parent, QWidget* window, HalfEdge * CHalfEdge);
    HalfEdge* CorrHalfEdge;
    QWidget* getWinP() const;
  private:
    QWidget* _window;
};
#endif

