#ifndef CAMERA_CPP
#define CAMERA_CPP
#include "preheader.h"
#include "Ray.cpp"
class Camera{
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
    xUnit = planeWidth/xPixels;
    yUnit = planeHeight/yPixels;
}


//Assumes that plane is orthogonal with z axis; Uses Perspective View from book(p.75)
void Camera::generateRay(Sample& s, Ray* ray)
{
    float xPix = s(0);
    float yPix = s(1);
    //(*ray) = LL + xUnit*xPix + yUnit*yPix;
    ray->direction = LL+xUnit*xPix + yUnit*yPix;
    ray->position = eye;
    
}
#endif