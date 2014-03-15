#ifndef __Assignment2__Triangle__
#define __Assignment2__Triangle__

#include "preHeader.h"

class Triangle : public Shape {
public:
    Point A, B, C;
    Vector Anorm, Bnorm, Cnorm;
    bool predefinedNormal;
    Triangle() {
        A=Point(0,0,0);
        B=Point(0,0,0);
        C=Point(0,0,0);
        predefinedNormal = false;
    }
    Triangle(Point a, Point b, Point c) {
        A = a;
        B = b;
        C = c;
        predefinedNormal = false;
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
                Vector normal;
                if (!predefinedNormal) {
                    Vector ab = makeVec(A, B);
                    Vector ac = makeVec(A, C);
                    normal = ab.cross(ac);
                }else{
                    //normal = ((Anorm+Bnorm+Cnorm)/3.0).normalized();
                    Point bary;
                    getBarycentric(p,bary);
                    normal = (X(bary)*Anorm + Y(bary)*Bnorm + Z(bary)*Cnorm).normalized();
                }
                if (ray.direction.dot(normal) > 0.0) {
                    normal *= -1.0;
                }
                local->surfaceNormal = normal;
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
        if ((beta > 0.0 && gamma > 0.0 && beta + gamma < 1.0)) {
            Point p;
            float thit=0;
            if (ray.valueAt(thit, &p)) {
                return true;
            }
        }
        return false;
    }
    
    //returns barycentric coordinates of point in triangle
    void getBarycentric(Point p, Point &b){
        Vector v0, v1, v2;
        float d00,d01,d11,d20,d21, denom, u, v, w;
        v0 = makeVec(A,B); //B-A        maybe add this to parser
        v1 = makeVec(A,C); //C-A
        v2 = makeVec(A,p); //p-A
        d00 = v0.dot(v0);
        d01 = v0.dot(v1);
        d11 = v1.dot(v1);
        d20 = v2.dot(v0);
        d21 = v2.dot(v1);
        denom = d00*d11 - d01*d01;
        v = (d11 * d20 - d01 * d21) / denom;
        w = (d00 * d21 - d01 * d20) / denom;
        u = 1.0f - v - w;
        b(0) = u;
        b(1) = v;
        b(2) = w;
    }
};

#endif /* defined(__Assignment2__Triangle__) */
