#ifndef PRE_CPP
#define PRE_CPP

#include "preHeader.h"

#include "Ray.cpp"
#include "Sampler.cpp"
#include "Camera.cpp"
#include "Raytracer.cpp"
#include "Film.cpp"
#include "Primitive.cpp"
#include "GeometricPrimitive.cpp"
#include "AggregatePrimitive.cpp"
#include "Shape.cpp"
#include "Sphere.cpp"
#include "Triangle.cpp"
#include "Parser.cpp"
#include "Light.cpp"
#include "AABBNode.cpp"

void testCImg() {
    println("===Testing CImg===");
    CImg<unsigned char> img(640,400,1,3);        // Define a 640x400 color image with 8 bits per color component.
    img.fill(0);                                 // Set pixel values to 0 (color : black)
    unsigned char purple[] = { 255,0,255 };      // Define a purple color
    img.draw_text(100,100,"Hello World",purple); // Draw a purple "Hello world" at coordinates (100,100).
    img.display("My first CImg code");           // Display the image in a display window.
    img.save("TestImage.bmp");
}

void testEigen() {
    //Test Eigen
    println("===Testing Eigen===");
    MatrixXd m(2,2);
    m(0,0) = 3;
    m(1,0) = 2.5;
    m(0,1) = -1;
    m(1,1) = m(1,0) + m(0,1);
    cout << m << endl;
}

void testMakeVec() {
    println("===Testing makeVec");
    Point p1(3, 2, 1);
    Point p2(1, 2, 3);
    Vector vec = makeVec(p1, p2);
    cout << vec << endl;
}

void testSampler(){
    printf("===Testing Sampler===\n \n");
    Sampler sampler = Sampler(2, 2);
    Sample s = Sample(0,0);
    while(sampler.getSample(&s))
    {
        printf("%f, %f \n", s(0), s(1));
    }
}

void testCamera(){
    printf("===Testing Camera====\n \n");
    Point UL  = Point(-1,  1, 0);
	Point UR  = Point( 1,  1, -2);
	Point LR  = Point( 1, -1, -2);
	Point LL  = Point(-1, -1, -1);
    Camera c = Camera(UL, UR, LR, LL, Point(0,0,0), 4, 4);
    Sampler sampler = Sampler(4,4);
    Sample s = Sample(0,0);
    while(sampler.getSample(&s))
    {
        Ray r = Ray();
        c.generateRay(s, &r);
        printf("Direction: (%f, %f, %f) \n", r.direction[0], r.direction[1], r.direction[2]);
    }
}

void testFilm(){
    printf("===Testing Film=== \n \n");
    Film film = Film(50,50);
     Sampler sampler = Sampler(50,50);
    Sample s = Sample(0,0);
    Color c = Color (0.8,0.6,0.4);
    while (sampler.getSample(&s)) {
        film.commit(s, c);
    }
    film.writeImage("FilmTest.png");
}

bool _testLoadObj(const char* filename, const char* basepath = NULL) {
    cout << "Loading " << filename << endl;
    
    vector<shape_t> shapes;
    string err = LoadObj(shapes, filename, basepath);
    
    if (!err.empty()) {
        cerr << err << endl;
        return false;
    }
    
    cout << "# of shapes : " << shapes.size() << endl;
    
    for (size_t i = 0; i < shapes.size(); i++) {
        printf("shape[%ld].name = %s\n", i, shapes[i].name.c_str());
        printf("shape[%ld].indices: %ld\n", i, shapes[i].mesh.indices.size());
        assert((shapes[i].mesh.indices.size() % 3) == 0);
        for (size_t f = 0; f < shapes[i].mesh.indices.size(); f++) {
            printf("  idx[%ld] = %d\n", f, shapes[i].mesh.indices[f]);
        }
        
        printf("shape[%ld].vertices: %ld\n", i, shapes[i].mesh.positions.size());
        assert((shapes[i].mesh.positions.size() % 3) == 0);
        for (size_t v = 0; v < shapes[i].mesh.positions.size() / 3; v++) {
            printf("  v[%ld] = (%f, %f, %f)\n", v,
                   shapes[i].mesh.positions[3*v+0],
                   shapes[i].mesh.positions[3*v+1],
                   shapes[i].mesh.positions[3*v+2]);
        }
        printf("shape[%ld].material.name = %s\n", i, shapes[i].material.name.c_str());
        printf("  material.Ka = (%f, %f ,%f)\n", shapes[i].material.ambient[0], shapes[i].material.ambient[1], shapes[i].material.ambient[2]);
        printf("  material.Kd = (%f, %f ,%f)\n", shapes[i].material.diffuse[0], shapes[i].material.diffuse[1], shapes[i].material.diffuse[2]);
        printf("  material.Ks = (%f, %f ,%f)\n", shapes[i].material.specular[0], shapes[i].material.specular[1], shapes[i].material.specular[2]);
        printf("  material.Tr = (%f, %f ,%f)\n", shapes[i].material.transmittance[0], shapes[i].material.transmittance[1], shapes[i].material.transmittance[2]);
        printf("  material.Ke = (%f, %f ,%f)\n", shapes[i].material.emission[0], shapes[i].material.emission[1], shapes[i].material.emission[2]);
        printf("  material.Ns = %f\n", shapes[i].material.shininess);
        printf("  material.Ni = %f\n", shapes[i].material.ior);
        printf("  material.map_Ka = %s\n", shapes[i].material.ambient_texname.c_str());
        printf("  material.map_Kd = %s\n", shapes[i].material.diffuse_texname.c_str());
        printf("  material.map_Ks = %s\n", shapes[i].material.specular_texname.c_str());
        printf("  material.map_Ns = %s\n", shapes[i].material.normal_texname.c_str());
        map<string, string>::iterator it(shapes[i].material.unknown_parameter.begin());
        map<string, string>::iterator itEnd(shapes[i].material.unknown_parameter.end());
        for (; it != itEnd; it++) {
            printf("  material.%s = %s\n", it->first.c_str(), it->second.c_str());
        }
        printf("\n");
    }
    
    return true;
}

void testTinyObjLoader() {
    _testLoadObj("test/cube.obj", "test/");
}

void testSphereIntersection() {
    Sphere ball = Sphere(1.0, 0.0, 0.0, -3.0);
    Ray ray(Point(0,0,0), Vector(0,0,-1), 0.0, 100.0);
    float thit;
    LocalGeo local;
    if (ball.intersect(ray, &thit, &local)) {
        cout << "Intersected at: " << local.position << " with Surface Normal: " << local.surfaceNormal;
    }
}

void testTriangleIntersection() {
    Triangle tri = Triangle(Point(-1.0,0.0,-3.2), Point(1,1,-3.2), Point(1,-1,-3.2));
    Ray ray(Point(0,0,0), Vector(0,0,-1), 0.0, 100.0);
    float thit;
    LocalGeo local;
    if (tri.intersect(ray, &thit, &local)) {
        cout << "Intersected at: " << local.position << " with Surface Normal: " << local.surfaceNormal;
    }
}


void testTransformation() {
    cout << "Testing Transformation of all kinds" << endl;
    
    cout << "Transformation: Rotation" << endl;
    AngleAxisf rotation(0.5*M_PI, Vector::UnitX());
    Ray ray(Point(0,0,0), Point(0,1,0), 0, 1);
    cout << "Rotation of Ray->0,1,0 by 90 deg about x-axis" << endl;
    Transformation t(rotation);
    ray = t*ray;
    cout << "Should be: 0,0,1" << endl << "Got: " << ray.direction << endl;
    cout << "May have slight error margin" << endl;
    rotation = AngleAxisf(-0.25*M_PI, Vector::UnitZ());
    ray = Ray(Point(0,0,0), Point(0,1,0), 0, 1);
    cout << "Rotation of Ray->0,1,0 by -45 deg about z-axis" << endl;
    t = Transformation(rotation);
    ray = t*ray;
    cout << "Should be: x,y,0, where x~=y" << endl << "Got: " << ray.direction << endl;
    
    println("Transformation: Translation");
    t = Transformation(Translation3f(1,2,3));
    Point p(0,0,0);
    println("Translate point(0,0,0) by (1,2,3)");
    p = t*p;
    println("Should be: 1,2,3");
    cout << "Got: " << p << endl;
    
    println("Transformation: Scaling");
    t = Transformation(Scalingf(3));
    p = Point(2,3,4);
    println("Scaling point(2,3,4) by factor 3");
    p = t*p;
    println("Should be: 6, 9, 12");
    cout << "Got: " << p << endl;

    println("Transformation: Composed Transform");
    t = AngleAxisf(0.25*M_PI, Vector::UnitZ())*Translation3f(1,2,3)*Scalingf(3);
    p = Point(1,1,1);
    println("Composed Transform: ScalingBy3->TranslateBy(1,2,3)->RotateBy(-pi/2,z-axis)");
    p = t*p;
    println("Should be: rotation*(4,5,6), which is some negative x, positive y, and z=6");
    cout << "Got: " << p << endl;
    
    println("Transformation: Non-uniform scaling");
    t = Transformation(Scaling3f(3, 2, 1));
    cout << t.m.matrix() << endl;
}

void testParser(string basePath) {
    vector<string> filenames;
    map<string, string> tMap;
    getFileNamesOfDirectory(basePath, filenames, tMap);
    for (auto & name : filenames) {
        println(name);
        println(tMap[fileNameWithoutExt(name)]);
    }
    string args = "0.5 1.0 0.9";
    Color kr = getKr(args);
    cout << kr;
    
    AggregatePrimitive aggregate;
    vector<Light> lights;
    Point eye;
    vector<Point> plane;
    parseObjectFiles(aggregate, lights, basePath, &eye, plane);
    vector<Primitive*> blah = aggregate.primList; //break here
    
    println("Testing Parsing Transform Files");
    Transformation *transform;
    parseTransformFile(aggregate, lights, "testFolder/cube.param", &transform, &eye, plane);
    cout << transform->m.matrix() << endl;
    println("Demonstrate empty trasnform");
    cout << IdentityTransform().matrix() << endl;
    
    println("Demonstrate eye and plane transform");
    cout << eye << endl;
    printf("UL: %f, %f, %f\n", plane[0](0), plane[0](1), plane[0](2));
    printf("UR: %f, %f, %f\n", plane[1](0), plane[1](1), plane[1](2));
    printf("LR: %f, %f, %f\n", plane[2](0), plane[2](1), plane[2](2));
    printf("LL: %f, %f, %f\n", plane[3](0), plane[3](1), plane[3](2));
    
    //println("Demonstratte sphere & ellipsoid");
    //break here
}

void testAggregatePrimitive(){
//    Sphere ball = Sphere(1.0, 0.0, 0.0, -3.0);
//    Ray ray(Point(0,0,0), Vector(0,0,-1), 0.0, 100.0);
//    float thit;
//    LocalGeo local;
}

void testDisplay()
{
//    Sampler sampler = Sampler(100,100);
    
}

void testAll() {
    println("===Begin Comprehensive Testing===");
    testCImg();
    testEigen();
    testMakeVec();
    testSampler();
    testCamera();
    testTransformation();
}

#endif
