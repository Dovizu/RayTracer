CC = g++
RM = /bin/rm -f 
BUILD = build
SRC = source
LIB = libraries
CIMG = -I$(LIB)/CImg/ -lm -lpthread -I/opt/X11/include -L/opt/X11/lib -lm -lpthread -lX11


ifeq ($(shell sw_vers 2>/dev/null | grep Mac | awk '{ print $$2}'),Mac)
	CFLAGS = -g -std=c++11 -c -Wall -Wno-deprecated $(CIMG)
	LFLAGS = -std=c++11 -Wall $(CIMG)

else
	CFLAGS = -g -std=c++0x -c -Wall
	LFLAGS = -std=c++0x -Wall
endif

OBJS = $(BUILD)/raytracer.o

raytracer: raytracer.o
	$(CC) $(LFLAGS) $(OBJS) -o raytracer

raytracer.o: $(SRC)/raytracer.cpp $(SRC)/raytracer.h
	$(CC) $(CFLAGS) $(SRC)/raytracer.cpp -o $(BUILD)/raytracer.o

raytracer.h: $(LIB)/CImg/CImg.h

clean:
	$(RM) *.o raytracer