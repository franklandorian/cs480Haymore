#ifndef MOON_H
#define MOON_H

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
#include "model.h"

class Moon
{
  private:
    std::vector<meshEntry> meshes;
    std::vector<GLuint> textures;
    unsigned int planetIndex;

    // Helper Method to initialize and bind each mesh
    void InitMesh(unsigned int Index, const aiMesh* paiMesh);

	public:
    model(std::string objectFilename);
    ~model();
    void Render();
    
    glm::mat4 GetModel();
  	void Update(unsigned int dt);
};

#endif /* MOON_H*/
