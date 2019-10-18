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
	float rotationSpeed;
  float orbitSpeed;
	float revolution;
  float start;
	int moon;
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
  	void Update(unsigned int dt, float revOffset, float x = 0.0f, float y = 0.0f, float z = 0.0f, std::string objName = "");

		// getters for position of object
		float getX() const;
		float getY() const;
		float getZ() const;

		int isMoon() const;
		int getIndex() const;
		std::string getName() const;
		float getRadius() const;
};

#endif /* MODEL_H*/
