CC = g++
ifeq ($(shell sw_vers 2>/dev/null | grep Mac | awk '{ print $$2}'),Mac)
	CFLAGS = -g -std=c++11 -DGL_GLEXT_PROTOTYPES -I./include/ -I/usr/X11/include -DOSX -Wno-deprecated
	LDFLAGS = -framework GLUT -framework OpenGL \
    	-L"/System/Library/Frameworks/OpenGL.framework/Libraries" \
    	-lGL -lGLU -lm -lstdc++
else
	CFLAGS = -g -std=c++0x -DGL_GLEXT_PROTOTYPES -Iglut-3.7.6-bin
	LDFLAGS = -lglut -lGLU
endif
	
RM = /bin/rm -f 
all: main 
main: src/raytracer.o
	$(CC) $(CFLAGS) -o main src/raytracer.o $(LDFLAGS)
raytracer.o: src/raytracer.cpp
	$(CC) $(CFLAGS) -c src/raytracer.cpp -o src/raytracer.o
clean:
	$(RM) *.o raytracer