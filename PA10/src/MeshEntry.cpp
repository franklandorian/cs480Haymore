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

    angleRotate = angleRev = 0;
    return true;
}

glm::mat4 meshEntry::GetModel(){
    return model;
}

void meshEntry::SetModel(glm::mat4 newModel){
    model = newModel;
}

void meshEntry::Update(unsigned int dt, objProp props, int rotation, float xPos, float yPos, float zPos){

    // Controlling speed of the sim
    dt *= 1;
	float scaleFactor = props.size;

	// glm::rotate(glm::mat4(1.0f), (revolveAngle), glm::vec3(0.0, 1.0, 0.0))

	// (dt) * M_PI/100000

	if(props.name.compare("Plunger") == 0){
		// 70.6
		model = glm::rotate(model, (glm::mediump_float) 80, glm::vec3(0.0, 1.0, 0.0));
	}

	if(props.name.compare("Backboard") == 0){
		model = glm::scale(model, glm::vec3(scaleFactor*1.15, scaleFactor/3, scaleFactor));
	} else if(props.name.compare("Plunger") == 0) {
		model = glm::scale(model, glm::vec3(scaleFactor/4.4, scaleFactor, scaleFactor));
	} else {
		model = glm::scale(model, glm::vec3(scaleFactor, scaleFactor, scaleFactor));
	}
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

