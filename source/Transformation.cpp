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
    Transformation(AngleAxisf t) {
        m = t;
        mInverseTranspose = m.linear().inverse().transpose();
        type = TransformationAffine;
    }
    //Translation transformation
    Transformation(Translation<float, 3> t) {
        m = t;
        mInverseTranspose = m.linear().inverse().transpose();
        type = TransformationAffine;
    }
    //Scaling transformation (fucking documentation)
    Transformation(UniformScaling<float> t) {
        m = t;
        mInverseTranspose = m.linear().inverse().transpose();
        type = TransformationAffine;
    }
    //General Affine transformation (assume <0,0,0,1> for last row)
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
};

#endif