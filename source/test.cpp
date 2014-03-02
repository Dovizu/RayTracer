#include "preHeader.h"

#include "Sampler.cpp"

void testCImg() {
    print("===Testing CImg===");
    CImg<unsigned char> img(640,400,1,3);        // Define a 640x400 color image with 8 bits per color component.
    img.fill(0);                                 // Set pixel values to 0 (color : black)
    unsigned char purple[] = { 255,0,255 };      // Define a purple color
    img.draw_text(100,100,"Hello World",purple); // Draw a purple "Hello world" at coordinates (100,100).
    img.display("My first CImg code");           // Display the image in a display window.
    img.save("TestImage.bmp");
}

void testFreeImage() {
    print("===Testing FreeImage===");
    //Test FreeImage library
    FreeImage_Initialise();
    cout << "FreeImage " << FreeImage_GetVersion() << endl;
    cout << FreeImage_GetCopyrightMessage() << "\n\n";
    
    //Test FreeImage file saving function
    FIBITMAP *bitmap = FreeImage_Allocate(800, 600, 24);
    RGBQUAD color;
    if (!bitmap)
        exit(1);
    for (int i=0; i<800; i++) {
        for (int j=0; j<600; j++) {
            color.rgbRed = 0;
            color.rgbGreen = (double) i/800 * 255.0;
            color.rgbBlue = (double) j/600 * 255.0;
            FreeImage_SetPixelColor(bitmap, i, j, &color);
        }
    }
    if (FreeImage_Save(FIF_PNG, bitmap, "FITest.png", 0))
        cout << "Image Successfully Saved!" << endl;
    
    FreeImage_DeInitialise();
}

void testEigen() {
    //Test Eigen
    print("===Testing Eigen===");
    MatrixXd m(2,2);
    m(0,0) = 3;
    m(1,0) = 2.5;
    m(0,1) = -1;
    m(1,1) = m(1,0) + m(0,1);
    std::cout << m << std::endl;
}

void testMakeVec() {
    print("===Testing makeVec");
    Point p1(3, 2, 1);
    Point p2(1, 2, 3);
    Vector vec = makeVec(p1, p2);
    cout << vec << endl;
}

void testSampler(){
    printf("===Testing Sampler===\n \n");
    Sampler sampler = Sampler(4, 4);
    //int i = 0;
    Sample s = Sample(0,0);
    while(sampler.getSample(&s))
    {
        printf("%f, %f \n", s(0), s(1));
    }
}

void testAll() {
    print("===Begin Comprehensive Testing===");
    testCImg();
    testFreeImage();
    testEigen();
    testMakeVec();
    testSampler();
}
