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
#include <btBulletDynamicsCommon.h>

#include "graphics_headers.h"
#include "MeshEntry.h"

class model
{
  private:
    std::vector<meshEntry> meshes;
    std::vector<GLuint> textures;
    unsigned int meshNumber;

    // Helper Method to initialize and bind each mesh
    void InitMesh(unsigned int Index, const aiMesh* paiMesh);
	objProp m_Prop;

	btTriangleMesh* triangleMesh;
	bool triangleFlag;

	// keeping this in case we have to add some bullshit
	//std::vector<model*> m_children;

	public:
    model(std::string objectFilename, objProp props);	// setting set
    ~model();
    void Render();

    glm::mat4 GetModel();
	void SetModel(glm::mat4 newModel);

  	void Update(unsigned int dt, int objType = 0, float x = 0.0f, float y = 0.0f, float z = 0.0f);

	void buttonHandler(SDL_Keycode&);
	btTriangleMesh* getTriangles();

    void MoveLeftFlipper();
    void MoveRightFlipper();

	std::string getObjName() const;
	int getObjType() const;
	int getObjShape() const;

	// getters for position of object
	float getX() const;
	float getY() const;
	float getZ() const;
	float getW() const;
};

#endif /* MODEL_H*/
