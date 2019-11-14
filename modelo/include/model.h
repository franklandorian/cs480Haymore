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

	/*
	void setChild(model *child);
	glm::mat4 GetChildModel(int i);
	model* getChild(int i);*/

	std::string getObjName() const;
	int getObjType() const;
	int getObjShape() const;

	// getters for position of object
	float getX() const;
	float getY() const;
	float getZ() const;
	float getW() const;
};

    // btCollisionShape *shape;

    // // Change the collision shape based on whats being loaded
    // if(info.name.compare("Board") == 0){
    //     shape = new btStaticPlaneShape (btVector3(0,1,0), 0);
    //     std::cout << "hewwo" << !!objectTriangles << "\n";
    //     // shape = new btBvhTriangleMeshShape(objectTriangles, true);
	// 	btTransform transform;
	// 	transform.setIdentity();
	// 	transform.setOrigin(btVector3(0,-1,0));
	// 	btStaticPlaneShape *floor = new btStaticPlaneShape(btVector3(0.0,1,0.0), 0);
	// 	btMotionState *motionFloor = new btDefaultMotionState(transform);
	// 	btRigidBody::btRigidBodyConstructionInfo floorInfo(0, motionFloor, floor);
	// 	btRigidBody * floorPlane = new btRigidBody(floorInfo);

	// 	int flags = floorPlane->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT;
	// 	floorPlane->setCollisionFlags(flags);
	// 	floorPlane->setActivationState(DISABLE_DEACTIVATION);

	// 	// Don't add to loadedBodies but make sure that there is floor collision
	// 	addBody(floorPlane);
	// } else {
	// 	if(info.shape == 3 ){
    //     	shape = new btBoxShape (btVector3(3.75*info.size,3.75*info.size,3.75*info.size));
	// 	} else {
	// 		shape = new btBoxShape (btVector3(info.size,info.size,info.size));
	// 	}
	// 		btDefaultMotionState *shapeMotionState = NULL;
	// 		// Start at the given positions
	// 		shapeMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 1, 0), btVector3(info.startPos[0], info.startPos[1], info.startPos[2])));
			
	// 		btScalar mass;
	// 		// If the object is dynamic then mass is nonzero
	// 		(info.type != 1) ? mass = 0 : mass = 1;

	// 		// std::cout << "hey" << info.size << std::endl;
			
	// 		btVector3 inertia(0,0,0);
	// 		shape->calculateLocalInertia(mass, inertia);
	// 		btRigidBody::btRigidBodyConstructionInfo shapeRigidBodyCI(mass, shapeMotionState, shape, inertia);
	// 		btRigidBody *rigidBody = new btRigidBody(shapeRigidBodyCI);

	// 		rigidBody->setActivationState(DISABLE_DEACTIVATION);
	// 		addBody(rigidBody);
	// }




#endif /* MODEL_H*/
