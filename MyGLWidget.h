/* see the comments in the .cpp file for details about the functions */

#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QtOpenGL/QGLWidget>
#include <QSignalMapper>
#include "Camera.h"
#include "algebra3.h"
#include "Stuff.h"

class MyGLWidget: public QGLWidget{ //inherit from QGLWidget
	Q_OBJECT //tell Qt that this is a Qt object.  Don't erase this.

public:
	MyGLWidget(QWidget *parent = 0);
	~MyGLWidget();
	void initializeGL(); //initialize some parameters for OpenGL
	void paintGL(); //paints the scene
	void resizeGL(int, int); //sets the initial projection and handles
							//resizing
	void drawStuff(); //draws the stuff

	void keyPressEvent(QKeyEvent *e); //add code for processing keystrokes here.  keep it simple!
	void mousePressEvent(QMouseEvent *event);

  Stuff* Gift(); // Return the Mesh if MainWindow needs it

private:
  Stuff _primitive; // the Mesh
  Camera _cam;
  QTimer* tick;
signals:
  void stuffReady(Stuff* s);
public slots:
  void autoRotate(void);  // Now it just update the view.
  void cubeDemo(int level);
  void someoneWantsAnUpdate();
};

#endif //MYGLWIDGET_H
