# PA1: Spinning Cube

# Dependencies, Building, and Running

## Dependency Instructions
For both of the operating systems to run this project installation of these three programs are required [GLEW](http://glew.sourceforge.net/), [GLM](http://glm.g-truc.net/0.9.7/index.html), and [SDL2](https://wiki.libsdl.org/Tutorials).

This project uses OpenGL 3.3. Some computers, such as virtual machines in the ECC, can not run this version. In in order to run OpenGL 2.7 follow the instructions at [Using OpenGL 2.7](https://github.com/HPC-Vis/computer-graphics/wiki/Using-OpenGL-2.7)

### Ubuntu/Linux
```bash
sudo apt-get install libglew-dev libglm-dev libsdl2-dev
```

### Mac OSX
Installation of brew is suggested to easily install the libs. Ensure that the latest version of the Developer Tools is installed.
```bash
brew install glew glm sdl2
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
```


## Ubuntu.cse.unr.edu
OpenGL 3.3 will run on the [ubuntu.cse.unr.edu](https://ubuntu.cse.unr.edu/) website. To do so follow the build instructions, but when running the Tutorial executable use this line to execute.
```bash
/usr/NX/scripts/vgl/vglrun ./PA2 -v <vertex_shader_file> -f <fragment_shader_file>
```

### Running instructions for TAs
General executable format for PA2
* The '-v' and '-f' order is interchangeable
```bash
./PA2 -v <vertex_shader_file> -f <fragment_shader_file>
```
Specific executable instructions for PA2
```bash
./PA2 -v ../shaders/vShader.txt -f ../shaders/fShader.txt
```

### Guide for Button and Mouse Controls
<table>
	<tr>
		<th>Action</th>
		<th>Command</th>
	</tr>
	<tr>
		<td>Reverse the orbit</td>
		<td><kbd>q</kbd> OR <kbd>Left Mouse Button</kbd></td>
	</tr>
	<tr>
		<td>Reverse the rotation</td>
		<td><kbd>w</kbd> OR <kbd>Right Mouse Button</kbd></td>
	</tr>
	<tr>
		<td>Pause the orbit</td>
		<td><kbd>e</kbd></td>
	</tr>
	<tr>
		<td>Pause the rotation</td>
		<td><kbd>r</kbd></td>
	</tr>
</table>
