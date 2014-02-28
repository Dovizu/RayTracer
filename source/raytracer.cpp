#include "raytracer.h"

int main(int argc, char *argv[]) {

    //Test CImg Library
    CImg<unsigned char> img(640,400,1,3);        // Define a 640x400 color image with 8 bits per color component.
    img.fill(0);                                 // Set pixel values to 0 (color : black)
    unsigned char purple[] = { 255,0,255 };      // Define a purple color
    img.draw_text(100,100,"Hello World",purple); // Draw a purple "Hello world" at coordinates (100,100).
//    img.display("My first CImg code");           // Display the image in a display window.
    img.save("TestImage.bmp");
    
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
    
    return 0;
}