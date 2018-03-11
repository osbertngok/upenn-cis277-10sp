/*  
	MyGLWidget extends the QGLWidget class, which is the Qt Widget with built in OpenGL functionality.
	It has 3 essential functions: initializeGL, paintGL, resizeGL
	initializeGL - can be used to call some one time initialization parameters.  In this example, the clear color is set.
	paintGL - is called every time the QGLWidget needs to be repainted.  This is where you will visualize your geometry
	resizeGL - is called every time the QGLWidget is resized, and once initially as well.

*/

#include "MyGLWidget.h"
#include <iostream>
#include <QtGui>

using std::cout;
using std::endl;

MyGLWidget::MyGLWidget(QWidget *parent)
: QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
		//default constructor, which will call the base constructor with some formatting parameters and the parent widget
    // prepare for key focus
    setFocusPolicy(Qt::StrongFocus);
    // initilize the camera and cube;
    
    //_primitive.toCube(100);
    _cam = Camera(vec4(0,0,6,0), vec4(0,1,0,0));
    _cam.setFrustumWidth(0.5);
    _cam.setFrustumFront(1);
    _cam.setFrustumBack(20);

    tick = new QTimer(this);
    connect(tick,SIGNAL(timeout()),this,SLOT(autoRotate()));
    tick->start(40);
}

void MyGLWidget::initializeGL(){
  glClearColor(1.0,0.0,0.0,1.0); //sets the background color to red
	glEnable(GL_DEPTH_TEST); //without this, order of drawing will determine which polygons are displayed
							// and not depth
	glDepthMask(GL_TRUE);
	//initial camera parameters could be stored here
	//remember not to update camera parameters here.  
	

}
void MyGLWidget::keyPressEvent(QKeyEvent *e){
	//a good place to process keystrokes
  if (e->key() == Qt::Key_Left)
  {
    _cam.AdjustHorizontalView(10);
  }
  if (e->key() == Qt::Key_Right)
  {
    _cam.AdjustHorizontalView(-10);
  }
  if (e->key() == Qt::Key_Up)
  {
    _cam.AdjustVerticalView(10);
  }
  if (e->key() == Qt::Key_Down)
  {
    _cam.AdjustVerticalView(-10);
  }
  if (e->key() == Qt::Key_PageUp)
  {
    _cam.AdjustZoom(0.91);
  }
  if (e->key() == Qt::Key_PageDown)
  {
    _cam.AdjustZoom(1.1);
  }
  QGLWidget::keyPressEvent(e);
  return;
}

//function for processing mouse press events in your GLWidget
//you will need QMouseEvent to determine the location of the click
void MyGLWidget::mousePressEvent(QMouseEvent *event){
  float x = event->x();
  float y = event->y();
  std::cout << "Someone clicked " << x << ", " << y << std::endl;
  // change to normalized device coordinates
  x = x / this->width();
  y = y / this->height();
  // coordinate of eye
  vec3 eye = vec3(_cam.getCameraVector());
  // up vector
  vec3 up = vec3(_cam.getUpVector());
  // get viewing vector
  vec3 center = vec3(0,0,0); // different if center moves
  vec3 G = center - eye;
  // half viewing angles
  float fi = atan(_cam.getFrustumWidth()/_cam.getFrustumFront());
  float theda = fi;
  vec3 A = G^up;
  vec3 B = A^G;
  vec3 M = eye + G;
  vec3 H = (A*(G.length()*tan(theda)))/A.length();
  vec3 V = (B*(G.length()*tan(fi)))/B.length();
  vec3 P = M + (2*x -1)*H + (1 - 2*y)*V;
  std::cout << "The point is " <<P[0]<<", "<<P[1]<<", "<<P[2]<< std::endl;
  // The ray is: where t goes from 0 to infinity
  // vec3 R = eye + t*(P - eye)/(P - eye).length();
  _cam.setRayStartVector(eye);
  _cam.setRayDirectVector(P - eye);
  _cam.setRayOn(true);
  // Select Faces
  list<Face*> F = _primitive.Faces();
  double mint = 100;
  list<Face*>::iterator mini = F.end();
  for (list<Face*>::iterator i = F.begin(); i != F.end(); ++i)
  {
    (*i)->unselect();
    if (!(*i)->isPlanar()) continue;
    double t = (*i)->rayIntersection(eye,P);
    if (!(t >= 0) && !(t < 0)) continue;
    if (t < mint)
    {
      mint = t;
      mini = i;
    }
  }
  if (mini != F.end())
    (*mini)->select();
  QGLWidget::mousePressEvent(event);
  return;
}

Stuff* MyGLWidget::Gift()
{
  return &_primitive;
}
/*
Draws a cube
It is probably easiest for you to build this cube so that it
is centered at the origin.  That will make rotation work much
more easily
*/
void MyGLWidget::drawStuff(){
  mat4 adhocmat = _cam.getCameraMatrix();
	_primitive.paintA(adhocmat);
}
//calls made here are made every time the widget's contents are repainted.  notice how we are redrawing everything.
void MyGLWidget::paintGL(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear the screen and depth buffer
	glLoadIdentity(); //loads the identity matrix into the OpenGL matrix stack.  

	//you may leave these lines in for debugging purposes.  However, it needs to be replaced by your own 
	//transformation code for the final version.  These commands can be useful to test to see if your transformations
	//do what you were hoping they would do.
	//glTranslatef(0.0,0.0,-6.0);
	//glRotatef(45.0,1.0,1.0,1.0);

	drawStuff();
  if (_cam.isRayOn())
    _cam.paintRayA(_cam.getCameraMatrix());
  return;

}

//this function is called every time the window is resized.  
void MyGLWidget::resizeGL(int width, int height){
	int side = qMin(width, height);  //finds the min of the width and height.  helpful for different distortions of the window
	glViewport((width - side) / 2, (height - side) / 2, side, side); //set the viewport

	glMatrixMode(GL_PROJECTION); //set the matrix mode to Projection
	glLoadIdentity(); //load the identity
	//sets up a frustum for use in perspective viewing
	//glFrustum(left edge, right edge, bottom edge, top edge, distance to near clipping plane, distance to far clipping plane
	//the last two parameters must be positive.  this is the distance from the origin (0,0,0) to the near/far clipping plane
	//also remember that OpenGL points the camera in the direction of -z, so the front clipping plane is a plane stationed at z=-1,
	//and the far clipping plane is a plane stationed at z=-20
	glFrustum(-_cam.getFrustumWidth(),_cam.getFrustumWidth(),-_cam.getFrustumWidth(),_cam.getFrustumWidth(),_cam.getFrustumFront(),_cam.getFrustumBack());
	glMatrixMode(GL_MODELVIEW); //change to the Modelview mode

}

//deconstructor that's empty
MyGLWidget::~MyGLWidget(){

}

void MyGLWidget::autoRotate()
{
  if (!_primitive.isDirty())
    update();
  else
    cout << "dirty!!" << endl;
  return;
}

void MyGLWidget::cubeDemo(int level)
{
  _primitive.toCube(level);
  emit stuffReady(&_primitive);
  return;
}

void MyGLWidget::someoneWantsAnUpdate()
{
  emit stuffReady(&_primitive);
  return;
}
