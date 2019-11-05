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

  public:
    Physics();
    ~Physics();
    void Update(float dt, model *myModel, int index);
    void renderPlane();
    int addBody(btRigidBody * newBody);
    int createObject(objProp info);

    void Move(std::string command);
};

#endif /*PHYSICS_H*/
