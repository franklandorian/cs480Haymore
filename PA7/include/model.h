#ifndef MODEL_H
#define MODEL_H

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


struct setting
{
	std::string name;
	int index;
	float radius;
	float rotation;
	float revolution;
};

class model
{
  private:
    std::vector<meshEntry> meshes;
    std::vector<GLuint> textures;

    unsigned int meshNumber;

    // Helper Method to initialize and bind each mesh
    void InitMesh(unsigned int Index, const aiMesh* paiMesh);

	setting m_setting;

	public:
    model(std::string objectFilename, setting set);
    ~model();
    void Render();
    
    glm::mat4 GetModel();
  	void Update(unsigned int dt, int offset);
};

#endif /* MODEL_H*/
