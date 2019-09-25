#include <iostream>

#include "engine.h"


int main(int argc, char **argv)
{
  // Start an engine and run it then cleanup after

  if(argc == 1){
  	printf("No shaders submitted, ex: ./PA3 -v <vertexFilename> -f <fragmentFilename>");
  	return 1;
  }
	char* vertexFilename;
	char* fragmentFilename;
  char* objectFilename;
	for(int i = 1; i < argc; i++){
		if(argv[i][0] == '-' && argv[i][1] == 'v'){
			vertexFilename = argv[i+1];
		}
		if(argv[i][0] == '-' && argv[i][1] == 'f'){
			fragmentFilename = argv[i+1];
		}
    if(argv[i][0] == '-' && argv[i][1] == 'o'){
      objectFilename = argv[i+1];
    }
	}
  
  Engine *engine = new Engine("Spinning loaded obj with random colors", 1366, 768);
  if(!engine->Initialize(vertexFilename, fragmentFilename, objectFilename))
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
