CC = g++
RM = /bin/rm -f 
BUILD = build
SRC = source
LIB = libraries

CIMG = -I$(LIB)/CImg/ -lm -lpthread -I/opt/X11/include -L/opt/X11/lib -lm -lpthread -lX11
FIMG = -I$(LIB)/FreeImage/ -L$(LIB)/FreeImage/ -lfreeimage
EIGEN = -I$(LIB)/Eigen/
GF = -I$(LIB)/gflags/include -L$(LIB)/gflags/lib -lgflags
OBJLOADER = -I$(LIB)/TinyObjLoader/

ifeq ($(shell sw_vers 2>/dev/null | grep Mac | awk '{ print $$2}'),Mac)
	CFLAGS = -g -std=c++11 -c -Wall -Wno-deprecated $(EIGEN) $(GF)
	LFLAGS = -std=c++11 -Wall $(GF)

else
	CFLAGS = -g -std=c++0x -c -Wall $(EIGEN) $(GF)
	LFLAGS = -std=c++0x -Wall $(GF)
endif

OBJS = $(BUILD)/scene.o $(BUILD)/objLoader.o

scene: scene.o objLoader.o
	$(CC) $(LFLAGS) $(OBJS) $(FIMG) $(CIMG) $(GF) -o scene

scene.o: $(SRC)/Scene.cpp $(SRC)/Scene.h
	$(CC) $(CFLAGS) $(FIMG) $(CIMG) $(GF) $(OBJLOADER) $(SRC)/Scene.cpp -o $(BUILD)/scene.o
	
objLoader.o: $(LIB)/TinyObjLoader/tiny_obj_loader.cpp $(LIB)/TinyObjLoader/tiny_obj_loader.h
	$(CC) $(CFLAGS) $(LIB)/TinyObjLoader/tiny_obj_loader.cpp -o $(BUILD)/objLoader.o

clean:
	$(RM) *.o scene test build/scene.o build/test.o