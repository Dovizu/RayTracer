#ifndef __Assignment2__Sphere__
#define __Assignment2__Sphere__

#include "preHeader.h"

class Sphere : Shape {
public:
    float radius;
    Point center;
    
    Sphere() {
        radius = 1.0;
        center = Point(0,0,0);
    }
    Sphere(float radius, float x, float y, float z) {
        center = Point(x,y,z);
        this->radius = radius;
    }
    
    /**
     *  Calculate intersection point and LocalGeo
     *
     *  @param ray   ray shot
     *  @param thit  pointer to float t at which ray hits
     *  @param local localGeo which contains intersection point info
     *
     *  @return whether the intersection happens
     */
    bool intersect(Ray& ray, float* thit, LocalGeo* local) {
        Vector d = ray.direction;
        Vector e = ray.position.matrix();
        Vector c = center.matrix();
        
        //Computer quadratic elements
        float A = d.dot(d);
        float B = d.dot(e-c)*2;
        float C = (e-c).dot(e-c)-sqr(radius);
        //Check discriminant
        float dscmnt = sqr(B)-4*A*C;
        if (dscmnt < 0.0) {
            return false;
        }else if (dscmnt == 0.0){
            //grazes the sphere, one point
            *thit = (-d.dot(e-c))/(d.dot(d));
            Point p;
            if (ray.valueAt(*thit, &p)) {
                Vector surfaceNormal = makeVec(center, p);
                local->surfaceNormal = surfaceNormal;
                local->position = p;
                return true;
            }
            return false;
        }else{
            //passes thru two points
            float t1 = (-d.dot(e-c)+sqrt(sqr(d.dot(e-c))-(d.dot(d))*((e-c).dot(e-c))-sqr(radius)))/(d.dot(d));
            float t2 = (-d.dot(e-c)-sqrt(sqr(d.dot(e-c))-(d.dot(d))*((e-c).dot(e-c))-sqr(radius)))/(d.dot(d));
            *thit = (t1 < t2) ? t1 : t2; //smaller of two
            Point p;
            if (ray.valueAt(*thit, &p)) {
                Vector surfaceNormal = makeVec(center, p);
                local->surfaceNormal = surfaceNormal;
                local->position = p;
                return true;
            }
            return false;
        }
    }
    
    /**
     *  Just returns whether the ray intersects with this shape or not
     *
     *  @param ray ray shot
     *
     *  @return whether it intersects
     */
    bool intersectP(Ray& ray) {
        Vector d = ray.direction;
        Vector e = ray.position.matrix();
        Vector c = center.matrix();
        float A = d.dot(d);
        float B = d.dot(e-c)*2;
        float C = (e-c).dot(e-c)-sqr(radius);
        return (sqr(B)-4*A*C) >= 0.0;
    }
};

#endif /* defined(__Assignment2__Sphere__) */