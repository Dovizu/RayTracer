CC = g++
RM = /bin/rm -f 
BUILD = build
SRC = source
LIB = libraries

CIMG = -I$(LIB)/CImg/ -lm -lpthread -I/opt/X11/include -L/opt/X11/lib -lm -lpthread -lX11
FIMG = -I$(LIB)/FreeImage/ -L$(LIB)/FreeImage/ -lfreeimage

ifeq ($(shell sw_vers 2>/dev/null | grep Mac | awk '{ print $$2}'),Mac)
	CFLAGS = -g -std=c++11 -c -Wall -Wno-deprecated
	LFLAGS = -std=c++11 -Wall

else
	CFLAGS = -g -std=c++0x -c -Wall
	LFLAGS = -std=c++0x -Wall
endif

OBJS = $(BUILD)/raytracer.o

raytracer: raytracer.o
	$(CC) $(LFLAGS) $(OBJS) $(FIMG) $(CIMG) -o raytracer

raytracer.o: $(SRC)/raytracer.cpp $(SRC)/raytracer.h
	$(CC) $(CFLAGS) $(FIMG) $(CIMG) $(SRC)/raytracer.cpp -o $(BUILD)/raytracer.o

raytracer.h: $(LIB)/CImg/CImg.h, $(LIB)/FreeImage/FreeImage.h

clean:
	$(RM) *.o raytracer