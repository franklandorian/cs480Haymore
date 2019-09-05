#include <iostream>

#include "engine.h"


int main(int argc, char **argv)
{
  // Start an engine and run it then cleanup after

  if(argc == 1){
  	printf("No shaders submitted, ex: ./Tutorial -v <vertexFilename> -f <fragmentFilename>");
  	return 1;
  }
	char* vertexFilename;
	char* fragmentFilename;
	for(int i = 1; i < argc; i++){
		if(argv[i][0] == '-' && argv[i][1] == 'v'){
			vertexFilename = argv[i+1];
		}
		if(argv[i][0] == '-' && argv[i][1] == 'f'){
			fragmentFilename = argv[i+1];
		}
	}
  Engine *engine = new Engine("Revolving Spinning Cube", 1366, 768);
  if(!engine->Initialize(vertexFilename, fragmentFilename))
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
