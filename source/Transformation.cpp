#ifndef TRANSFORM_CPP
#define TRANSFORM_CPP

#include "Ray.cpp"
#include "LocalGeo.cpp"

typedef enum {
    TransformationAffine,
    TransformationProjective
} TransformationType;

/**
 *  Transformation that uses Affine Transform & Projective Transform from Eigen
 */
class Transformation {
public:
    TransformationType type;
    Transform<float, 3, Affine> m, mInverseTranspose;
    Transform<float, 3, Projective> mProj, mInverseTransposeProj;
    /**
     *  Constructors
     */
    //Good-for-nothing transformation
    Transformation(){
    }
    //Rotation transformation
    //Transformation rotation(AngleAxisf(radians, vector.normalized()));
    Transformation(AngleAxisf t) {
        m = t;
        mInverseTranspose = m.linear().inverse().transpose();
        type = TransformationAffine;
    }
    //Translation transformation
    //Transformation translate(Translation3f(x,y,z));
    Transformation(Translation<float, 3> t) {
        m = t;
        mInverseTranspose = m.linear().inverse().transpose();
        type = TransformationAffine;
    }
    //Scaling transformation (fucking documentation)
    //Transformation scale(Scalingf(factor));
    Transformation(UniformScaling<float> t) {
        m = t;
        mInverseTranspose = m.linear().inverse().transpose();
        type = TransformationAffine;
    }
    Transformation(Scaling3f t) {
        m = t;
        mInverseTranspose = m.linear().inverse().transpose();
        type = TransformationAffine;
    }
    //General Affine transformation (assume <0,0,0,1> for last row)
    //Transformation compose(M3*M2*M1);
    Transformation(Transform<float, 3, Affine> t) {
        m = t;
        mInverseTranspose = m.linear().inverse().transpose();
        type = TransformationAffine;
    }
    //General Projective transformation (does not assume <0,0,0,1> for last row)
    Transformation(Transform<float, 3, Projective> t) {
        mProj = t;
        mInverseTranspose = mProj.linear().inverse().transpose();
        type = TransformationProjective;
    }
    
    /**
     *  Overloads
     *  Transform class goes left, vector/point goes right
     */
    //Transform Ray class
    Ray operator*(const Ray& other) const{
        Vector newPos = m*(other.position.matrix());
        Vector newVec = m.linear()*(other.direction);
        return Ray(newPos.array(), newVec, other.t_min, other.t_max);
    }
    //Transform Point
    Point operator*(const Point& other) const{
        return (m*(other.matrix())).array();
    }
    //Transform Vector
    Vector operator*(const Vector& other) const{
        return m.linear()*other;
    }
    //Transform a LocalGeo
    LocalGeo operator*(const LocalGeo& other) const{
        Vector newNormal = mInverseTranspose.linear()*other.surfaceNormal;
        Point newPosition = (m*(other.position.matrix())).array();
        newNormal.normalize();
        return LocalGeo(newPosition, newNormal);
    }
    //Transform a bounding box
    BoundingBox operator*(const BoundingBox& other) const{
        float maxX, maxY, maxZ;
        float minX, minY, minZ;
        minX = other.min(0);
        minY = other.min(1);
        minZ = other.min(2);
        maxX = other.max(0);
        maxY = other.max(1);
        maxZ = other.max(2);
        
        Point v1(minX, minY, minZ);
        Point v2(minX, minY, maxZ);
        Point v3(minX, maxY, minZ);
        Point v4(minX, maxY, maxZ);
        Point v5(maxX, minY, minZ);
        Point v6(maxX, minY, maxZ);
        Point v7(maxX, maxY, minZ);
        Point v8(maxX, maxY, maxZ);
        
        v1 = (*this)*v1;
        v2 = (*this)*v2;
        v3 = (*this)*v3;
        v4 = (*this)*v4;
        v5 = (*this)*v5;
        v6 = (*this)*v6;
        v7 = (*this)*v7;
        v8 = (*this)*v8;
        
        minX = minf(minf(minf(minf(minf(minf(minf(v1(0), v2(0)), v3(0)), v4(0)), v5(0)), v6(0)), v7(0)), v8(0));
        minY = minf(minf(minf(minf(minf(minf(minf(v1(1), v2(1)), v3(1)), v4(1)), v5(1)), v6(1)), v7(1)), v8(1));
        minZ = minf(minf(minf(minf(minf(minf(minf(v1(2), v2(2)), v3(2)), v4(2)), v5(2)), v6(2)), v7(2)), v8(2));
        
        maxX = maxf(maxf(maxf(maxf(maxf(maxf(maxf(v1(0), v2(0)), v3(0)), v4(0)), v5(0)), v6(0)), v7(0)), v8(0));
        maxY = maxf(maxf(maxf(maxf(maxf(maxf(maxf(v1(1), v2(1)), v3(1)), v4(1)), v5(1)), v6(1)), v7(1)), v8(1));
        maxZ = maxf(maxf(maxf(maxf(maxf(maxf(maxf(v1(2), v2(2)), v3(2)), v4(2)), v5(2)), v6(2)), v7(2)), v8(2));
        
        BoundingBox bb;
        bb.min = Point(minX, minY, minZ);
        bb.max = Point(maxX, maxY, maxZ);
        return bb;
    }
};

#endif