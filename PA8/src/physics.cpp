#include "physics.h"

Physics::Physics()
{
    // This creates the broadphase
    broadphase = new btDbvtBroadphase();

    // Create a collision configuration
	collisionConfiguration = new btDefaultCollisionConfiguration();

    // Create a dispatcher
	dispatcher = new btCollisionDispatcher(collisionConfiguration);

    // Create a sovler
	solver = new btSequentialImpulseConstraintSolver;

    // Set the world
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0, -9.81, 0));

    // We need a floor lmao
    createFloor();
}

Physics::~Physics()
{
    // Cleanup Time
    // for(int i=0; i<loadedBodies.size(); i++)
    // {
    //     dynamicsWorld->removeCollisionObject(loadedBodies[i]);
    //     btMotionState * motionState = loadedBodies[i]->getMotionState();
    //     btCollisionShape * collisionShape = loadedBodies[i]->getCollisionShape();
    //     delete loadedBodies[i];
    //     loadedBodies[i] = nullptr;
    //     delete motionState;
    //     delete collisionShape;
    // }

    // Delete the world along with everything
    delete dynamicsWorld;
	delete solver;
	delete dispatcher;
	delete collisionConfiguration;
	delete broadphase;
    
    dynamicsWorld = nullptr;
    solver = nullptr;
    dispatcher = nullptr;
    collisionConfiguration = nullptr;
    broadphase = nullptr;
}

void Physics::Update(float dt, model *myModel,int index)
{
    dynamicsWorld->stepSimulation(1/600.0f, 10);

    btTransform trans;
    btScalar m[16];

    loadedBodies[index]->getMotionState()->getWorldTransform(trans);
    trans.getOpenGLMatrix(m);
    myModel->SetModel(glm::make_mat4(m));
}

int Physics::createObject(objProp info)
{
    btCollisionShape *shape;

    // Change the collision shape based on whats being loaded
    if(info.name.compare("Board") == 0){
        shape = new btStaticPlaneShape (btVector3(0,1,0), 0);
    } else {
        shape = new btBoxShape (btVector3(info.size,info.size,info.size));
    }

    btDefaultMotionState *shapeMotionState = NULL;
    // Start at the given positions
    shapeMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 1, 0), btVector3(info.startPos[0], info.startPos[1], info.startPos[2])));
    
    btScalar mass;
    // If the object is dynamic then mass is nonzero
    (info.type != 1) ? mass = 0 : mass = 1;

    // std::cout << "hey" << info.size << std::endl;
    
    btVector3 inertia(0,0,0);
    shape->calculateLocalInertia(mass, inertia);
    btRigidBody::btRigidBodyConstructionInfo shapeRigidBodyCI(mass, shapeMotionState, shape, inertia);
    btRigidBody *rigidBody = new btRigidBody(shapeRigidBodyCI);

    rigidBody->setActivationState(DISABLE_DEACTIVATION);
    addBody(rigidBody);
}

int Physics::addBody(btRigidBody * newBody)
{
    // Add the rigid body and save the rigid body into a vector
    dynamicsWorld->addRigidBody(newBody);
    loadedBodies.emplace_back(newBody);
    return loadedBodies.size();
}

void Physics::createFloor()
{
    // This basically just creates the floor
    btTransform transform[1];
    transform[0].setIdentity();
    transform[0].setOrigin(btVector3(0,-1,0));
    btStaticPlaneShape *floor = new btStaticPlaneShape(btVector3(0.0,1,0.0), 0);
    btMotionState *motionFloor = new btDefaultMotionState(transform[0]);
    btRigidBody::btRigidBodyConstructionInfo floorInfo(0, motionFloor, floor);
    btRigidBody * floorPlane = new btRigidBody(floorInfo);

    int flags = floorPlane->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT;
    floorPlane->setCollisionFlags(flags);
    floorPlane->setActivationState(DISABLE_DEACTIVATION);

    // Don't add to loadedBodies but make sure that there is floor collision
    dynamicsWorld->addRigidBody(floorPlane);
}

void Physics::Move(std::string command)
{
    float magnitude = 1.0f;

    // The following applies a force to the cube to make it move
    if (command.compare("right") == 0){
        loadedBodies[1]->applyCentralImpulse(btVector3(-magnitude,0.0,0.0));
    } else if (command.compare("left") == 0){
        loadedBodies[1]->applyCentralImpulse(btVector3(magnitude,0.0,0.0));
    } else if (command.compare("up") == 0){
        loadedBodies[1]->applyCentralImpulse(btVector3(0.0,0.0,magnitude));
    } else {
        loadedBodies[1]->applyCentralImpulse(btVector3(magnitude,0.0,-magnitude));
    }

    
}