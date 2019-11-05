#include "MeshEntry.h"

bool meshEntry::Init(const std::vector<Vertex> incomingVertices, const std::vector<unsigned int> incomingIndices){
    Vertices = incomingVertices;
    Indices = incomingIndices;
    NumIndices = Indices.size();
    glGenBuffers(1, &VB);
    glBindBuffer(GL_ARRAY_BUFFER, VB);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);
    glGenBuffers(1, &IB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);
    
    srand(time(NULL));

    angleRotate = angleRev =  0;
    return true;
}

glm::mat4 meshEntry::GetModel(){
    return model;
}

void meshEntry::SetModel(glm::mat4 newModel){
    model = newModel;
}

void meshEntry::Update(unsigned int dt, int rotation, float xPos, float yPos, float zPos, float scaleFactor ){

    // Controlling speed of the sim
    dt *= 1;

	// I'm pretty sure we'll have to do all the physics accounting here
	// So I'm gonna save these functions for now
		
    // angleRev += (dt) * M_PI/1000;;
    // model = glm::translate(glm::mat4(1.0f), glm::vec3(xPos + x,yPos + y, zPos + z));
	// model = glm::rotate(model, (angleRev * rotation), glm::vec3(0.0, 1.0, 0.0));
	model = glm::scale(model, glm::vec3(scaleFactor, scaleFactor, scaleFactor));

	// std::cout << scaleFactor << std::endl;

	/*
    // Next rotate the actual object
    angleRotate += (dt) * M_PI/100000 * rotationSpeed;
    model = glm::rotate(model, (angleRotate), glm::vec3(0.0, 1.0, 0.0));

    // Now scale it to the appropiate size
	model = glm::scale(model, glm::vec3(radius, radius, radius)); */
}

void meshEntry::buttonHandler(SDL_Keycode& sym){
	switch (sym)
	{
		  case SDLK_i:
		    moveForward();
		    break;
		  case SDLK_j:
		    moveLeft();
		    break;
		  case SDLK_k:
		    moveBack();
		    break;
		  case SDLK_l:
		    moveRight();
		    break;
			case SDLK_u:
		    moveUp();
		    break;
			case SDLK_o:
		    moveDown();
		    break;
		  default:
				break;
		}
}

void meshEntry::moveForward()
{	
	z += 0.18f;
}

void meshEntry::moveLeft()
{
	x -= 0.18f;
}

void meshEntry::moveBack()
{
	z -= 0.18f;
}

void meshEntry::moveRight()
{
	x += 0.18f;
}

void meshEntry::moveUp()
{
	y -= 0.18f;
}

void meshEntry::moveDown()
{
	y += 0.18f;
}

// getters for position of object
float meshEntry::getX() const
{
	return model[3][0];
}
float meshEntry::getY() const
{
	return model[3][1];
}
float meshEntry::getZ() const
{
	return model[3][2];
}
float meshEntry::getA() const
{
	return model[3][3];
}

