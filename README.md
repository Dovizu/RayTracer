Project Identity
================
###Names
- Leo Colobong
- Donny Reynolds

###Platform
`OS X`

###Location
`./`

###Submitted by
First Last

Support Documentation
=====================

Makefile
--------
Makefile compiles things source into target `raytracer`, links object files from `build/`, and compiles source code from `source`. [Makefile Tutorial](https://www.cs.umd.edu/class/fall2002/cmsc214/Tutorial/makefile.html)

Xcode
--------
    Xcode Command   Cmd Line Equivalent
    Product->Build  make
    Product->Clean  make clean
    Source Control  git
    Scheme          arguments

Libraries
---------
All libraries are stored under `libraries/`

###CImg
CImg is header-only, can be used directly

###FreeImage
FreeImage requires compilation on each platform, Mac OS X 10.9 platform requires some modification for FreeImage to work (because FreeImage's OS X support is outdated), including [inserting `#include <cstring>` in `Source/OpenEXR/IlmImf/ImfAutoArray.h` and a brand new Makefile](http://stackoverflow.com/questions/19080303/how-to-compile-freeimage-on-mac-os-x-10-8). Compiled `libfreeimage.a` library archive plus `FreeImage.h` are used in main code.

When submitting the project, if we choose Linux platform, FreeImage needs to be compiled on Linux. Simply replacing Mac-compiled .a and .h files in our `libraries/FreeImage` with corresponding Linux version will work (Tested). FreeImage on Linux does not need further modifications.

Project Documentation
=====================

To-Do
------
Integrate Eigen
