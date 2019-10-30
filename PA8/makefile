# Check for OS Type
UNAME_S := $(shell uname -s)

# Linux
ifeq ($(UNAME_S), Linux)
	CC=g++
	LIBS=-lSDL2 -lGLEW -lGL -lassimp `Magick++-config --ldflags --libs` -I/usr/include/bullet -lBulletDynamics -lBulletSoftBody -lBulletCollision –lLinearMath
# Mac
else
	CC=clang++
	LIBS=-lSDL2 -framework OpenGL -framework Cocoa -lGLEW -stdlib=libc++ -I/usr/include/bullet -lBulletDynamics -lBulletSoftBody -lBulletCollision –lLinearMath
endif

# Paths, use this to set Include directories, or library directories; uncomment in all: if used
#PATHI=-I/usr/local/include/ 
#PATHL=-L/usr/local/lib/

#Compiler Flags
CXXFLAGS=-g -Wall -std=c++0x

# .o Compilation
O_FILES=main.o camera.o engine.o graphics.o object.o shader.o window.o

# Point to includes of local directories
INDLUDES=-I../include


all: $(O_FILES)
	$(CC) $(CXXFLAGS) -o HelloWorld $(O_FILES) $(LIBS) #$(PATHI) $(PATHL)

HelloWorld.o: HelloWorld.cpp
	$(CC) $(CXXFLAGS) -c HelloWorld.cpp -o HelloWorld.o $(INDLUDES)



clean:
	-@if rm *.o Tutorial 2>/dev/null || true; then echo "Main Removed"; else echo "No Main"; fi
