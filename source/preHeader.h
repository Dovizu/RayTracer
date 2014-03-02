#ifndef Assignment2_preHeader_h
#define Assignment2_preHeader_h

#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <algorithm>

#include <Eigen> //Eigen needs to go before CImg because X11 re-defines "Success" macro
#include <CImg.h>
#include <FreeImage.h>
//#include "

using namespace std;
using namespace Eigen;
using namespace cimg_library;

#include "utilities.cpp"

#ifndef DNDEBUG
#   define ASSERT(condition, message) \
do { \
if (! (condition)) { \
cerr << "Assertion `" #condition "` failed in " << __FILE__ \
<< " line " << __LINE__ << ": " << message << std::endl; \
exit(EXIT_FAILURE); \
} \
} while (false)
#else
#   define ASSERT(condition, message) do { } while (false)
#endif

#endif
