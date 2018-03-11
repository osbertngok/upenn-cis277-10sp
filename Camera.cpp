#include <cassert>
#include <cmath>
#include <QtOpenGL>
#include "algebra3.h"
#include "Camera.h"

Camera::Camera(void)
{
  _camera_vector = vec4(0,0,6,0);
  _up_vector = vec4(0,1,0,0);
  _ray_start_vector = vec3(0,0,0);
  _ray_direct_vector = vec3(0,0,-1);
  _ray_color = vec3(1,1,1);
  _ray_on = false;
  _f_width = 0.5;
  _f_front = 0.5;
  _f_back = 0.5;
}

Camera::Camera(const vec4& cameraVector, const vec4& upVector)
: _camera_vector(cameraVector),
  _up_vector(upVector)
{
  _ray_start_vector = vec3(0,0,0);
  _ray_direct_vector = vec3(0,0,-1);
  _ray_color = vec3(1,1,1);
  _ray_on = false;
  _f_width = 0.5;
  _f_front = 0.5;
  _f_back = 0.5;
}

void Camera::setCameraVector(const vec4& cameraVector)
{
  _camera_vector = cameraVector;
  return;
}

void Camera::setUpVector(const vec4& upVector)
{
  _up_vector = upVector;
  _up_vector.normalize();
  return;
}

void Camera::setRayStartVector(const vec3& rayStartVector)
{
  _ray_start_vector = rayStartVector;
  return;
}

void Camera::setRayDirectVector(const vec3& rayDirectVector)
{
  _ray_direct_vector = rayDirectVector;
  return;
}

void Camera::setRayColor(const vec3& rayColor)
{
  _ray_color = rayColor;
  return;
}

void Camera::setRayOn(bool on)
{
  _ray_on = on;
  return;
}

void Camera::AdjustHorizontalView(double degree)
{
  _camera_vector = rotation3D(_up_vector, degree, _camera_vector) * _camera_vector;
  return;
}

void Camera::AdjustVerticalView(double degree)
{
  vec3 Axis = vec3(_up_vector) ^ vec3(_camera_vector);
  _camera_vector = rotation3D(Axis, degree, _camera_vector) * _camera_vector;
  //_up_vector = rotation3D(Axis, degree, _up_vector) * _up_vector;
  _up_vector = vec4(vec3(_camera_vector) ^ Axis, 0);
  _up_vector.normalize();
  /*
  std::cout << "CV: ";
  for (unsigned int i = 0; i < 3 ; ++i)
    std::cout << _camera_vector[i] << ' ';
  std::cout << "UV: ";
  for (unsigned int i = 0; i < 3 ; ++i)
    std::cout << _up_vector[i] << ' ';
  std::cout << std::endl;
  */
  return;
}

void Camera::AdjustZoom(double scale)
{
  //_camera_vector = scale * _camera_vector;
  _f_width *= scale;
  _f_front /= scale;
  _f_back *= scale;
  glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	  glFrustum(-_f_width,_f_width,-_f_width,_f_width,_f_front,_f_back);
  glMatrixMode(GL_MODELVIEW);
  return;
}

vec4 Camera::getCameraVector(void) const
{
  return _camera_vector;
}

vec4 Camera::getUpVector(void) const
{
  return _up_vector;
}

mat4 Camera::getCameraMatrix(void) const
{
  //start with identity matrix;
  mat4 tmat = identity3D();
  
  // Test 1
  /*
  for (unsigned int i = 0; i < 4; ++i)
  {
    for (unsigned int j = 0; j < 4; ++j)
      std::cout << tmat[i][j] << ' ';
    std::cout << std::endl;
  }
  std::cout << std::endl;
  */

  //push the world along the view direction;
  tmat = translation3D(-1 * _camera_vector);
  // Test 2
  /*
  for (unsigned int i = 0; i < 4; ++i)
  {
    for (unsigned int j = 0; j < 4; ++j)
      std::cout << tmat[i][j] << ' ';
    std::cout << std::endl;
  }
  std::cout << std::endl;
  */
  //rotate zold to znew. znew is the cameraVector.

  vec4 zold = vec4(0,0,1,0);
  vec4 znew = _camera_vector;
  assert(abs(znew[3]) < 0.1);
  vec3 zRotateAxis = vec3(znew) ^ vec3(zold);

  // Test 3
  /*
  std::cout << "zRotateAxis:" << std:: endl;
  for (unsigned int i = 0; i < 3; ++i)
    std::cout << zRotateAxis[i] << ' ';
  std::cout << std::endl;
  std::cout << std::endl;
  */


  double zangle = acos((zold*znew)/zold.length()/znew.length());
  // Test 3.5
  /*
  std::cout << "zangle: " << zangle*180/3.14159 << std::endl;
  */

  mat4 zRotation = rotation3D(zRotateAxis, zangle/3.14159*180,vec3(znew));

  // Test 4
  /*
  for (unsigned int i = 0; i < 4; ++i)
  {
    for (unsigned int j = 0; j < 4; ++j)
      std::cout << zRotation[i][j] << ' ';
    std::cout << std::endl;
  }
  std::cout << std::endl;
  */

  tmat =zRotation * tmat;
  //rotate yold to ynew. ynew is the up vector.

  //vec4 yold = zRotation * vec4(0,1,0,0);
  //vec4 yold = rotation3D(zRotateAxis, zangle/3.14159*180,vec3(znew)) * vec4(0,1,0,0);
  vec4 yold = vec4(0,1,0,0);
  vec4 ynew = zRotation * _up_vector;
  //vec4 ynew = _up_vector;
  // Test 4.5
  /*
  for (unsigned int i = 0; i < 4; ++i)
    std::cout << yold[i] << ' ';
  std::cout << std::endl;
  
  for (unsigned int i = 0; i < 4; ++i)
    std::cout << ynew[i] << ' ';
  std::cout << std::endl;
  */
  vec3 yRotateAxis = vec3(ynew) ^ vec3(yold);
  // Test 5
  /*
  for (unsigned int i = 0; i < 3; ++i)
    std::cout << yRotateAxis[i] << ' ';
  std::cout << std::endl;
  std::cout << std::endl;
  */
  // double yangle = asin(yRotateAxis.length()/yold.length()/ynew.length());
  double yangle = acos((yold*ynew)/yold.length()/ynew.length());
  mat4 yRotation = rotation3D(yRotateAxis, yangle/3.14159*180,vec3(ynew));
  // Test 6
  /*
  for (unsigned int i = 0; i < 4; ++i)
  {
    for (unsigned int j = 0; j < 4; ++j)
      std::cout << yRotation[i][j] << ' ';
    std::cout << std::endl;
  }
  std::cout << std::endl;
  */
  
  tmat = yRotation * tmat;
  // Test 7
  /* 
  for (unsigned int i = 0; i < 4; ++i)
  {
    for (unsigned int j = 0; j < 4; ++j)
      std::cout << tmat[i][j] << ' ';
    std::cout << std::endl;
  }
  std::cout << std::endl;
  */

  return tmat;
}

vec3 Camera::getRayStartVector(void) const
{
  return _ray_start_vector;
}

vec3 Camera::getRayDirectVector(void) const
{
  return _ray_direct_vector;
}

vec3 Camera::getRayColor(void) const
{
  return _ray_color;
}

bool Camera::isRayOn(void) const
{
  return _ray_on;
}

bool Camera::paintRayA(const mat4& m) const
{
  if (!isRayOn()) return false;
  //Calcuate V1, V2
  vec3 _adhoc = _ray_direct_vector;
  vec3 v1 = m * _ray_start_vector;
  //vec3 v2 = m * (_ray_start_vector + 10 * _adhoc.normalize());
  vec3 v2 = m * (_ray_start_vector + _adhoc);
  glColor3f(_ray_color[0],_ray_color[1],_ray_color[2]);
  glBegin(GL_LINE_STRIP);
    glVertex3f(v1[0],v1[1],v1[2]);
    glVertex3f(v2[0],v2[1],v2[2]);
  glEnd();
  return true;
}

void Camera::setFrustumWidth(double width)
{
  _f_width = width;
  return;
}
void Camera::setFrustumFront(double front)
{
  _f_front = front;
  return;
}
void Camera::setFrustumBack(double back)
{
  _f_back = back;
  return;
}

double Camera::getFrustumWidth(void) const
{
  return _f_width;
}
double Camera::getFrustumFront(void) const
{
  return _f_front;
}
double Camera::getFrustumBack(void) const
{
  return _f_back;
}
