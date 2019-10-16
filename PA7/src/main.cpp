#include <iostream>
#include <string>

#include "engine.h"

int main(int argc, char **argv)
{
  // Check for config file, if not there, then throw an error
  if(argc == 1){
  	printf("No shaders submitted, ex: ./PA7 config.txt");
  	return 1;
  }

  // Grab the file name and open it
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
	char settingFilename[128];
  std::vector<std::string> objectFilenames;

  // Parse through the file
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
      std::string temp(objectFilename);
      objectFilenames.emplace_back(temp);
    }
		else if ( strcmp( lineHeader, "set" ) == 0 ){
      fscanf(configFile, "%s", settingFilename);
    }
  }
  
  // Start an engine and run it then cleanup after
  Engine *engine = new Engine("Spinning loaded obj with random colors", 1366, 768);
  if(!engine->Initialize(vertexFilename, fragmentFilename, settingFilename, objectFilenames))
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
