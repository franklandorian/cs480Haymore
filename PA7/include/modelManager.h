#ifndef MODEL_MANAGER_H
#define MODEL_MANAGER_H

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/color4.h>
#include <Magick++.h>

#include "graphics_headers.h"
#include "MeshEntry.h"

class modelManager
{
  public:
    modelManager(char* objectFilename);
    ~modelManager();
    void Render();

    
    glm::mat4 GetModel();
  	void Update(unsigned int dt);


  private:
    std::vector<meshEntry> meshes;
    std::vector<GLuint> textures;

    unsigned int meshNumber;

    // Helper Method to initialize and bind each mesh
    void InitMesh(unsigned int Index, const aiMesh* paiMesh);
};

#endif /* MODEL_MANAGER_H*/
