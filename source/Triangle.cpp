#ifndef __Assignment2__Triangle__
#define __Assignment2__Triangle__

#include "preHeader.h"

class Triangle : public Shape {
public:
    Point A, B, C;
    
    Triangle() {
        A=Point(0,0,0);
        B=Point(0,0,0);
        C=Point(0,0,0);
    }
    Triangle(Point a, Point b, Point c) {
        A = a;
        B = b;
        C = c;
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
        Vector a = A.matrix();
        Vector b = B.matrix();
        Vector c = C.matrix();
        //Solve system of equations to get beta, gamma and t
        Matrix3f A_mat;
        Vector b_vec;
        A_mat <<    X(a)-X(b),  X(a)-X(c),  X(d),
                    Y(a)-Y(b),  Y(a)-Y(c),  Y(d),
                    Z(a)-Z(b),  Z(a)-Z(c),  Z(d);
        b_vec << X(a)-X(e), Y(a)-Y(e), Z(a)-Z(e);
        Vector sol = A_mat.colPivHouseholderQr().solve(b_vec);
        float beta = sol(0);
        float gamma = sol(1);
        *thit = sol(2);
        if (beta > 0.0 && gamma > 0.0 && beta + gamma < 1.0) {
            Point p;
            if (ray.valueAt(*thit, &p)) {
                Vector ab = makeVec(A, B);
                Vector ac = makeVec(A, C);
                local->surfaceNormal = ab.cross(ac);
                local->surfaceNormal.normalize();
                local->position = p;
                return true;
            }
        }
        return false;
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
        Vector a = A.matrix();
        Vector b = B.matrix();
        Vector c = C.matrix();
        //Solve system of equations to get beta, gamma and t
        Matrix3f A_mat;
        Vector b_vec;
        A_mat <<    X(a)-X(b),  X(a)-X(c),  X(d),
                    Y(a)-Y(b),  Y(a)-Y(c),  Y(d),
                    Z(a)-Z(b),  Z(a)-Z(c),  Z(d);
        b_vec << X(a)-X(e), Y(a)-Y(e), Z(a)-Z(e);
        Vector sol = A_mat.colPivHouseholderQr().solve(b_vec);
        float beta = sol(0);
        float gamma = sol(1);
        return (beta > 0.0 && gamma > 0.0 && beta + gamma < 1.0);
    }
};

#endif /* defined(__Assignment2__Triangle__) */
