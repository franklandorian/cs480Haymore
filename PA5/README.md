# PA4: Model Loading and stuff

# Dependencies, Building, and Running

## Dependency Instructions
For both of the operating systems to run this project installation of these three programs are required [GLEW](http://glew.sourceforge.net/), [GLM](http://glm.g-truc.net/0.9.7/index.html), and [SDL2](https://wiki.libsdl.org/Tutorials).

This project uses OpenGL 3.3. Some computers, such as virtual machines in the ECC, can not run this version. In in order to run OpenGL 2.7 follow the instructions at [Using OpenGL 2.7](https://github.com/HPC-Vis/computer-graphics/wiki/Using-OpenGL-2.7)

### Ubuntu/Linux
```bash
sudo apt-get install libglew-dev libglm-dev libsdl2-dev
```

## Building and Running
To build this project there are two options. One is to use CMake which makes including new libraries easier, and handles new files added automatically to the src and include directory. CMake is a small new learning curve but makes things easier in the future.
The second option is to use the provided Makefile which is used as usual.

Running the make in a separate directory will allow easy cleanup of the build data, and an easy way to prevent unnecessary data to be added to the git repository.  

### CMake Instructions
The building of the project is done using CMake, installation with apt-get or brew may be necessary. Later use with CMake and Shader files will be require the copy of a directory where those files are stored (ex. shaders). To do this in the ```add_custom_target``` function place 
```cmake
COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/shaders/ ${CMAKE_CURRENT_BINARY_DIR}/shaders
```

```bash
mkdir build
cd build
cmake ..
make
./PA5 -v ../shaders/vertexShader.txt -f ../shaders/fragmentShader.txt -o ../assets/tray.obj
```
shader files are located in ../PA4/shaders/ they are called "vertexShader.txt" and "fragmentShader.txt".
object files are in ../PA4/obj/ the included one is ../obj.tray.obj
after running cmake and make...
-v specifies the vertex shader
-f specififes the fragment shader
-o specifies the object to be loaded



### Controls

The controls are fairly simple:

q: causes the object to rotate counter clockwise.

e: causes the object to rotate clockwise.

a: causes the object to revolve counter clockwise.

d: causes the object to revolve clockwise.

w: causes the object to increase in size.

s: causes the object to decrease in size.

r: resets the object to default settings.

↑: increases rotation speed.

↓: decreases rotation speed.

←: decreases revolution speed.

→: increases revolution speed.

spacebar: causes all motion to stop.

left click: toggles rotation direction.

right click: toggles revolution direction.
