class Camera{
    Point UL, UR, LR, LL;

public:
    Camera(Point ul, Point, ur, point lr, point ll);
    void generateRay(Sample& sample, Ray* ray);
};

Camera::Camera (Point ul, Point, ur, point lr, point ll)
{
    UL = ul;
    UR = ur;
    LR = lr;
    LL = ll;
}