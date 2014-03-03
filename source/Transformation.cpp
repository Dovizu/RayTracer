#ifndef TRANSFORM_CPP
#define TRANSFORM_CPP

class Transformation {
public:
    Matrix3f m, mInverceTranspose;
    
    Transformation(){
        
    }
    Ray operator*(const Ray& other) const{
        
    }
    Point operator*(const Point& other) const{
        
    }
    Vector operator*(const Vector& other) const{
        
    }
    LocalGeo operator*(const LocalGeo& other) const{
        
    }
};

#endif