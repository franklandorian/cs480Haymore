#include <iostream>
#include <string>

#include "engine.h"

int main(int argc, char **argv)
{
	std::string vertexFile, fragmentFile;
	// Get command line arguments and pass into the engine
	// ./Tutorial -v <vertex_shader_file> -f <fragment_shader_file>
	for (int i = 1; i < argc; ++i)
	{
		if (std::strcmp(argv[i], "-v") == 0)
			vertexFile = argv[i+1]; 
		if (std::strcmp(argv[i], "-f") == 0)
			fragmentFile = argv[i+1];			
	}
	//std::cout << vertexFile << std::endl << fragmentFile << std::endl;
  // Start an engine and run it then cleanup after
  Engine *engine = new Engine("Ilya Kuznetsov PA2", 800, 800, vertexFile, fragmentFile);
  if(!engine->Initialize())
  {
    printf("The engine failed to start.\n");
    delete engine;
    engine = NULL;
    return 1;
  }
  engine->Run();
  delete engine;
  engine = NULL;
  return 0;
}
