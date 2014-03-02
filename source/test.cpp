#include "preHeader.h"
#include "Sampler.cpp"
void testTest() {
    cout << "worked!" << endl;
    return;
}

void testSampler(){
    Point UL  = Point(-1,  1, -1);
	Point UR  = Point( 1,  1, -1);
	Point LR  = Point( 1, -1, -1);
	Point LL  = Point(-1, -1, -1);
    Sampler sampler = Sampler(UL, UR, LR, LL, 50, 50);
    int i = 0;
    
}