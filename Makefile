CC = g++
RM = /bin/rm -f 
BUILD = build
SRC = source
LIB = libraries

CIMG = -I$(LIB)/CImg/ -I/opt/X11/include -L/opt/X11/lib -lX11 -I/usr/local/bin/ -lpthread
EIGEN = -I$(LIB)/Eigen/
OBJLOADER = -I$(LIB)/TinyObjLoader/

ifeq ($(shell sw_vers 2>/dev/null | grep Mac | awk '{ print $$2}'),Mac)
	CFLAGS = -g -std=c++11 -c -Wall -Wno-deprecated
	LFLAGS = -std=c++11 -Wall

else
	CFLAGS = -g -std=c++0x -c -Wall
	LFLAGS = -std=c++0x -Wall
endif

OBJS = $(BUILD)/scene.o $(BUILD)/objLoader.o

scene: scene.o objLoader.o
	$(CC) $(LFLAGS) $(OBJS) $(CIMG) -o scene

scene.o: $(SRC)/Scene.cpp
	$(CC) $(CFLAGS) $(EIGEN) $(CIMG) $(OBJLOADER) $(SRC)/Scene.cpp -o $(BUILD)/scene.o
	
objLoader.o: $(LIB)/TinyObjLoader/tiny_obj_loader.cpp $(LIB)/TinyObjLoader/tiny_obj_loader.h
	$(CC) $(CFLAGS) $(LIB)/TinyObjLoader/tiny_obj_loader.cpp -o $(BUILD)/objLoader.o

clean:
	$(RM) *.o scene test build/scene.o build/test.o