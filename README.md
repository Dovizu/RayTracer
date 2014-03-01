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
Makefile compiles source files into target `raytracer`, links object files from `build/`, and compiles source code from `source/`. [(Makefile Tutorial)](https://www.cs.umd.edu/class/fall2002/cmsc214/Tutorial/makefile.html)

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
###X11
X11 is required by CImg. If your OS X is 10.8+, you need to install [X11](http://support.apple.com/kb/ht5293), which is no longer included in OS X automatically since 10.8 because Apple moved the project into open source development. 

X11 is installed on instructional Linux & Mac machines. (Tested)

###CImg
(Requires ImageMagick, X11)

[Documentation](http://cimg.sourceforge.net/reference/modules.html)

CImg is header-only, can be used directly from `libraries/CImg`.

###FreeImage
(Recompile for Linux)

[Documentation](https://inst.eecs.berkeley.edu/~cs184/fa09/resources/sec_UsingFreeImage.pdf)

FreeImage requires compilation on each platform, Mac OS X 10.9 platform requires some modification for FreeImage to work (because FreeImage's OS X support is outdated), including inserting `#include <cstring>` in `Source/OpenEXR/IlmImf/ImfAutoArray.h` and a brand new Makefile. [(Reference)](http://stackoverflow.com/questions/19080303/how-to-compile-freeimage-on-mac-os-x-10-8). Compiled `libfreeimage.a` library archive plus `FreeImage.h` are used in main code.

When submitting the project, if we choose Linux platform, FreeImage needs to be compiled on Linux. Simply replacing Mac-compiled .a and .h files in our `libraries/FreeImage` with corresponding Linux version will work (Tested). FreeImage on Linux does not need further modifications.

Because FreeImage source + object files are 50MB+, they are not included in the repository. The included .a and .h files should work on all Intel Macs. I uploaded and compiled Linux copies of .a & .h to my account.

###Eigen
[Documentation](http://eigen.tuxfamily.org/dox/index.html)

Eigen is installed and headers-only, can be used directly from `libraries/Eigen`. Eigen needs to be included before CImg because both define `Success` macro and Eigen will complain if CImg defines it before Eigen.

###PS: Mac vs Linux?
ImageMagick is apparently installed on instructional Linux machines, too. In fact, CImg supports PNG & JPG file saving as long as ImageMagick is installed. Therefore, technically we don't need FreeImage at all. But Mac instructional machines don't have ImageMagick. Therefore, we have two options:

1. Submit as Mac platform (fuji.eecs)
The only instructional Macs I know (fuji.eecs) don't have ImageMagick and don't support C++11. We need to talk to TA's to see if there is any instructional Macs that support C++11 (g++ version 4.3 or newer).

2. Submit as Linux platform (hive.eecs)
These machines support ImageMagick, but then we need to install ImageMagick on our Macs and our dev platform is Mac not Linux. So preferrably we want to develop and submit on Mac platform. 

Because we're not sure whether to stick with Linux or Mac, I'm keeping both FreeImage and CImg. 

##gflags
(Recompile for Linux)

[Documentation](https://gflags.googlecode.com/svn/trunk/doc/gflags.html)

gflags is installed.


Project Documentation
=====================

Currently running `raytracer` will automatically test CImg and FreeImage, and output FITest.png & test.bmp.

Data Structures
---------------

Vector (Eigen::Vector)

Point (Eigen::Array)

Ray

Matrix (Eigen::Matrix)

Transformation

Ray Tracer Classes
----------------

BRDF

Color

Sample

LocalGeo

Shape

Primitive

Intersection

GeometricPrimitive

AggregatePrimitive

Material

Sampler

Camera

RayTracer

Light

Film

Scene

