CC = g++
RM = /bin/rm -f 
BUILD = build
SRC = source
LIB = libraries

CIMG = -I$(LIB)/CImg/ -lm -lpthread -I/opt/X11/include -L/opt/X11/lib -lm -lpthread -lX11
FIMG = -I$(LIB)/FreeImage/ -L$(LIB)/FreeImage/ -lfreeimage
EIGEN = -I$(LIB)/Eigen/
GF = -I$(LIB)/gflags/include -L$(LIB)/gflags/lib -lgflags

ifeq ($(shell sw_vers 2>/dev/null | grep Mac | awk '{ print $$2}'),Mac)
	CFLAGS = -g -std=c++11 -c -Wall -Wno-deprecated $(EIGEN) $(GF)
	LFLAGS = -std=c++11 -Wall $(GF)

else
	CFLAGS = -g -std=c++0x -c -Wall $(EIGEN) $(GF)
	LFLAGS = -std=c++0x -Wall $(GF)
endif

OBJS = $(BUILD)/raytracer.o

raytracer: raytracer.o
	$(CC) $(LFLAGS) $(OBJS) $(FIMG) $(CIMG) $(GF) -o raytracer

raytracer.o: $(SRC)/raytracer.cpp $(SRC)/raytracer.h
	$(CC) $(CFLAGS) $(FIMG) $(CIMG) $(GF) $(SRC)/raytracer.cpp -o $(BUILD)/raytracer.o

raytracer.h: $(LIB)/CImg/CImg.h, $(LIB)/FreeImage/FreeImage.h

clean:
	$(RM) *.o raytracer build/raytracer.o