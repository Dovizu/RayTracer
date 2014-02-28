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
CImg is header-only, can be used directly from `libraries/CImg`.

###FreeImage
FreeImage requires compilation on each platform, Mac OS X 10.9 platform requires some modification for FreeImage to work (because FreeImage's OS X support is outdated), including inserting `#include <cstring>` in `Source/OpenEXR/IlmImf/ImfAutoArray.h` and a brand new Makefile. [(Reference)](http://stackoverflow.com/questions/19080303/how-to-compile-freeimage-on-mac-os-x-10-8). Compiled `libfreeimage.a` library archive plus `FreeImage.h` are used in main code.

When submitting the project, if we choose Linux platform, FreeImage needs to be compiled on Linux. Simply replacing Mac-compiled .a and .h files in our `libraries/FreeImage` with corresponding Linux version will work (Tested). FreeImage on Linux does not need further modifications.

Because FreeImage source + object files are 50MB+, they are not included in the repository. The included .a and .h files should work on all Intel Macs. I uploaded and compiled Linux copies of .a & .h to my account.

###PS: Notes
ImageMagick is apparently installed on instructional machines, too. I'm not sure whether we should use ImageMagick to output JPG though. Although ImageMagick requires no extra work in compilation and linking (FreeImage Mac vs Linux, argh) since it's already on instructional machines, but its [usage](http://www.imagemagick.org/Magick++/Image.html) seems fairly complex.

Project Documentation
=====================

To-Do
------

