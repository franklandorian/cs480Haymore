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
	dynamicsWorld->setGravity(btVector3(0, -9.81, -10.0));

    // We need a floor lmao
    createFloor();
    createWalls();
    createCeiling();
}

Physics::~Physics()
{
    // Cleanup Time
    for(int i=0; i<loadedBodies.size(); i++)
    {
        dynamicsWorld->removeCollisionObject(loadedBodies[i]);
        btMotionState * motionState = loadedBodies[i]->getMotionState();
        btCollisionShape * collisionShape = loadedBodies[i]->getCollisionShape();
        delete loadedBodies[i];
        loadedBodies[i] = nullptr;
        delete motionState;
        delete collisionShape;
    }

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

int Physics::createObject(objProp info, btTriangleMesh* objectTriangles)
{
    btCollisionShape *shape;

    if(objectTriangles){
        triangleBoy = objectTriangles;
    }

    // Change the collision shape based on whats being loaded
    if(info.name.compare("Board") == 0){
        // shape = new btStaticPlaneShape (btVector3(0,1,0), 0);
        // shape = new btBvhTriangleMeshShape(objectTriangles, true);
        shape = new btScaledBvhTriangleMeshShape(new btBvhTriangleMeshShape(triangleBoy, true), btVector3(info.size*100, info.size*100, info.size*100));
        
        std::cout  << info.size << " " << !!triangleBoy << std::endl;
    } else if(info.name.compare("Flipper") == 0){
        shape = new btBoxShape (btVector3(info.size,2*info.size,2*info.size));
    } else if(info.shape == 3 ){
        shape = new btBoxShape (btVector3(info.size,2*info.size,info.size));
    } else{
        shape = new btBoxShape (btVector3(info.size,info.size,info.size));
    }

    btDefaultMotionState *shapeMotionState;
    // Start at the given positions
    shapeMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 1, 0), btVector3(info.startPos[0], info.startPos[1], info.startPos[2])));

    // If the object is dynamic then mass is nonzero
    btScalar mass;
    (info.type != 1) ? mass = 0 : mass = 1;
    
    btVector3 inertia(0,0,0);
    shape->calculateLocalInertia(mass, inertia);
    btRigidBody::btRigidBodyConstructionInfo shapeRigidBodyCI(mass, shapeMotionState, shape, inertia);
    btRigidBody *rigidBody = new btRigidBody(shapeRigidBodyCI);

    if(info.name.compare("Board") == 0){
        std::cout << "hewwo, this is the: " << info.name << "\n";
        int flags = rigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT;
        rigidBody->setCollisionFlags(flags);
    }

    rigidBody->setActivationState(DISABLE_DEACTIVATION);
    addBody(rigidBody);

    if(info.name.compare("Board") == 0){
    }
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
    btTransform transform;
    transform.setIdentity();
    transform.setOrigin(btVector3(0,0.5,0));
    btStaticPlaneShape *floor = new btStaticPlaneShape(btVector3(0.0,1,0.0), 0);
    btMotionState *motionFloor = new btDefaultMotionState(transform);
    btRigidBody::btRigidBodyConstructionInfo floorInfo(0, motionFloor, floor);
    btRigidBody * floorPlane = new btRigidBody(floorInfo);

    int flags = floorPlane->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT;
    floorPlane->setCollisionFlags(flags);
    floorPlane->setActivationState(DISABLE_DEACTIVATION);

    // Don't add to loadedBodies but make sure that there is floor collision
    dynamicsWorld->addRigidBody(floorPlane);
}

void Physics::createCeiling()
{
    // This basically just creates the ceiling
    btTransform transform;
    transform.setIdentity();
    transform.setOrigin(btVector3(0,3.5,0));
    btStaticPlaneShape *ceiling = new btStaticPlaneShape(btVector3(0.0,-1,0.0), 0);
    btMotionState *motionCeiling = new btDefaultMotionState(transform);
    btRigidBody::btRigidBodyConstructionInfo ceilingInfo(0, motionCeiling, ceiling);
    btRigidBody * ceilingPlane = new btRigidBody(ceilingIfnfo);

    int flags = ceilingPlane->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT;
    floorPlane->setCollisionFlags(flags);
    floorPlane->setActivationState(DISABLE_DEACTIVATION);

    // Don't add to loadedBodies but make sure that there is floor collision
    dynamicsWorld->addRigidBody(floorPlane);
}

void Physics::createWalls()
{
    // This basically just creates the walls (very similar to how floor is made)
    int numberOfWalls = 4;
    btTransform transform[numberOfWalls];

    // Set indenity
    for(int i = 0; i < numberOfWalls; i++)
    {
        transform[i].setIdentity();
    }

    //backwall
    transform[0].setOrigin(btVector3(0,0,8.5));
    //frontwall
    transform[1].setOrigin(btVector3(0,0,-8.5));
    //leftside
    transform[2].setOrigin(btVector3(6.1,0,0));
    //rightside
    transform[3].setOrigin(btVector3(-5.2,0,0));

    std::vector<btStaticPlaneShape *> walls;

    for(int i = 0; i < numberOfWalls; i++)
    {
        btVector3 temp;

        switch(i){
            case 0:
                temp = btVector3(0.0,0.0,-1.0);
                break;
            case 1:
                temp = btVector3(0.0,0.0,1.0);
                break;
            case 2:
                temp = btVector3(-1.0,0.0,0.0);
                break;
            case 3:
                temp = btVector3(1.0,0.0,0.0);
                break;
        }

        btStaticPlaneShape *tempWall = new btStaticPlaneShape(temp, 1);
        walls.emplace_back(tempWall);
    }

    std::vector<btMotionState *> wallMotions;
    std::vector<btRigidBody::btRigidBodyConstructionInfo> wallInfo;
    std::vector<btRigidBody *> planes;

    for(int i = 0; i < numberOfWalls; i++)
    {
        btMotionState *tempMotion = new btDefaultMotionState(transform[i]);
        wallMotions.emplace_back(tempMotion);

        btRigidBody::btRigidBodyConstructionInfo temp3(0, wallMotions[i], walls[i]);
        btRigidBody * tempPlane = new btRigidBody(temp3);
        planes.emplace_back(tempPlane);
    }

    int flags = planes[0]->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT;

    for(int i = 0; i < numberOfWalls; i++)
    {
        planes[i]->setCollisionFlags(flags);
        planes[i]->setActivationState(DISABLE_DEACTIVATION);

        dynamicsWorld->addRigidBody(planes[i]);
    }
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
        loadedBodies[1]->applyCentralImpulse(btVector3(0.0,0.0,-magnitude));
    }
}

void Physics::LaunchPlunger(float magnitude)
{
    // Apply an upwards force on ball
    loadedBodies[1]->applyCentralImpulse(btVector3(0.0,0.0,magnitude));

    // Set game to on

}