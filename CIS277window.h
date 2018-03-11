/* the main window.  */

#ifndef CIS277WINDOW_H
#define CIS277WINDOW_H

#include <QtWidgets/QMainWindow>
#include "ui_cis277_3d.h"
#include "Camera.h"
#include "Stuff.h"
#include "Menus.h"

class CIS277window : public QMainWindow
{
	Q_OBJECT

public:
	CIS277window(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~CIS277window();

  void init();

private:
	Ui::CIS277window ui;
  Camera _cam;

  //UI in details
  QMenu* M_File;
  QMenu* M_Preset;
  Q_Vertex_List_Menu* M_Vertices;
  Q_HalfEdge_List_Menu* M_HalfEdges;
  Q_Face_List_Menu* M_Faces;
  QMenu* M_Others;

  vector<QAction*> M_File_vec;
  vector<QAction*> M_Preset_vec;

  QSignalMapper* cubeDemoMapper;
signals:
  void IWantAnUpdate(); //See MyGLWidget slot "SomeoneWantsAUpdate"
public slots:
  //Menu Handlers
  void stuffReadyHandler(Stuff* s);
  void faceColorHandler();
  void vertexCoordinateHandler();
  void vertexDeletionHandler();
  void addVertexHandler();
  void fillFaceHandler();
  void splitVertexHandler();
  void splitQuadHandler();


};

#endif // CIS277WINDOW_H
