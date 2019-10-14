#include <iostream>
#include <string>

#include "engine.h"

int main(int argc, char **argv)
{
  // Start an engine and run it then cleanup after

  if(argc == 1){
  	printf("No shaders submitted, ex: ./PA3 -v <vertexFilename> -f <fragmentFilename>");
  	return 1;
  }

  std::string configFileName(argv[1]);

  FILE * configFile = fopen(("../" + configFileName).c_str(), "r");

  if( configFile == NULL ){
    //printf(obj);
    printf("Impossible to open the file !\n");
    return false;
  }

  char vertexFilename[128];
  char fragmentFilename[128];
  char objectFilename[128];

  while( 1 ){
    
    char lineHeader[128];
    // read the first word of the line
    int res = fscanf(configFile, "%s", lineHeader);
    if (res == EOF)
        break; // EOF = End Of File. Quit the loop.

    // else : parse lineHeader
    if ( strcmp( lineHeader, "vs" ) == 0 ){
      fscanf(configFile, "%s", vertexFilename);
    } else if ( strcmp( lineHeader, "fs" ) == 0 ){
      fscanf(configFile, "%s", fragmentFilename);
    } else if ( strcmp( lineHeader, "obj" ) == 0 ){
      fscanf(configFile, "%s", objectFilename);
    }
  }

	// char* vertexFilename;
	// char* fragmentFilename;
 //  char* objectFilename;
	// for(int i = 1; i < argc; i++){
	// 	if(argv[i][0] == '-' && argv[i][1] == 'v'){
			
	// 		vertexFilename = argv[i+1];
	// 	}
	// 	if(argv[i][0] == '-' && argv[i][1] == 'f'){
	// 		fragmentFilename = argv[i+1];
	// 	}
 //    if(argv[i][0] == '-' && argv[i][1] == 'o'){
 //      objectFilename = argv[i+1];
 //    }
	// }
  
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
