#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "algebra3.h"

class Camera
{
  public:
    
    Camera(void); //default constructor
    Camera(const vec4& cameraVector, const vec4& upVector);

    //Setting Functions
    void setCameraVector(const vec4& cameraVector);
    void setUpVector(const vec4& upVector);

    //Ray Setup Function (Not related to our final project)
    void setRayStartVector(const vec3& rayStartVector);
    void setRayDirectVector(const vec3& rayDirectVector);
    void setRayColor(const vec3& rayColor);
    void setRayOn(bool on);

    //The following functions need to be changed to gluLookat
    void AdjustHorizontalView(double degree);
    void AdjustVerticalView(double degree);
    void AdjustZoom(double scale);

    //get functions
    vec4 getCameraVector(void) const;
    vec4 getUpVector(void) const;
    mat4 getCameraMatrix(void) const;

    // Again, not related to our project
    vec3 getRayStartVector(void) const;
    vec3 getRayDirectVector(void) const;
    vec3 getRayColor(void) const;
    bool isRayOn(void) const;
    bool paintRayA(const mat4& m) const;

    // Set Functions for Frustum
    void setFrustumWidth(double width);
    void setFrustumFront(double front);
    void setFrustumBack(double back);

    // Get Functions for Frustum
    double getFrustumWidth(void) const;
    double getFrustumFront(void) const;
    double getFrustumBack(void) const;

  private:
    vec4 _camera_vector;  //the eye vector
    vec4 _up_vector;  // the up vector
    // we need the center vector right here
    double _f_width; //width for frustum
    double _f_front; //front value for frustum
    double _f_back;  //back value for frustum

    vec3 _ray_start_vector;
    vec3 _ray_direct_vector;
    vec3 _ray_color;
    bool _ray_on;
};

#endif

