#ifndef PHYSICS_H
#define PHYSICS_H

#include <iostream>
#include <vector>
#include <string>
#include <btBulletDynamicsCommon.h>

#include "model.h"

class Physics
{
  private:
    btBroadphaseInterface *broadphase;
	  btDefaultCollisionConfiguration *collisionConfiguration;
	  btCollisionDispatcher *dispatcher;
	  btSequentialImpulseConstraintSolver *solver;
	  btDiscreteDynamicsWorld *dynamicsWorld;

    std::vector<btRigidBody*> loadedBodies;
    void createFloor();
    void createWalls();
    void createCeiling();
    void createDiagonal();
    void createDivider();

  public:
    Physics();
    ~Physics();
    void Update(float dt, model *myModel, int index);
    void renderPlane();
    int addBody(btRigidBody * newBody);
    int createObject(objProp info, btTriangleMesh* objectTriangles);

    btTriangleMesh* triangleBoy;

    void Move(std::string command);
    void LaunchPlunger(float magnitude = 22.0f);
    void RestartBall(model *myModel);
};

#endif /*PHYSICS_H*/
