#ifndef CAMERA_CPP
#define CAMERA_CPP
#include "preHeader.h"
#include "Ray.cpp"
class Camera{
public:
    Point UL, UR, LR, LL, eye;
    Vector u,v,w, up;
    float l,r,t,b, planeWidth, planeHeight, xUnit, yUnit;
    int xPixels, yPixels;
public:
    Camera(Point ul, Point ur, Point lr, Point ll, Point eye, int xPix, int yPix);
    void generateRay(Sample& s, Ray* ray);
};

Camera::Camera (Point ul, Point ur, Point lr, Point ll, Point eyeCoordinate, int xPix, int yPix)
{
    UL = ul;
    UR = ur;
    LR = lr;
    LL = ll;
    eye = eyeCoordinate;
    u = makeVec(UL,UR);
    v = makeVec(LL, UL);
    l = UL(0);
    r = UR(0);
    t = UL(1);
    b = LL(1);
    xPixels = xPix;
    yPixels = yPix;
    planeWidth = u.norm();
    planeHeight = v.norm();
    //u.normalize();
    //v.normalize();
    xUnit = planeWidth/xPixels;
    yUnit = planeHeight/yPixels;
}


//Assumes that plane is orthogonal with z axis; Uses Perspective View from book(p.75)
void Camera::generateRay(Sample& s, Ray* ray)
{
    float xPix = s(0);
    float yPix = s(1);
    //ray->direction = LL+xUnit*xPix*u + yUnit*yPix*v;
//    float a = xUnit*xPix/xPixels;
//    float b = yUnit*yPix/xPixels;
    float a = xPix/xPixels;
    float b = yPix/yPixels;
    printf("a = %f, b = %f ", a, b);
    (ray->direction)(0) = (LL(0) + a*u(0) + b*v(0)) - eye(0);
    (ray->direction)(1) = (LL(1) + a*u(1) + b*v(1)) - eye(1);
    (ray->direction)(2) = (LL(2) + a*u(2) + b*v(2)) - eye(2);
    ray->position = eye;
    
}
#endif