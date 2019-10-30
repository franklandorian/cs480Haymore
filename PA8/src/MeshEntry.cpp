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

void meshEntry::Update(unsigned int dt, float xPos, float yPos, float zPos ){

    // Controlling speed of the sim
    dt *= 1;

		// I'm pretty sure we'll have to do all the physics accounting here
		// So I'm gonna save these functions for now

		
    angleRev += (dt) * M_PI/1000;;
    model = glm::translate(glm::mat4(1.0f), glm::vec3(xPos,yPos, zPos));
		model = glm::rotate(model, (angleRev), glm::vec3(0.0, 1.0, 0.0));
		model = glm::scale(model, glm::vec3(0.5,0.5,0.5));
	/*
    // Next rotate the actual object
    angleRotate += (dt) * M_PI/100000 * rotationSpeed;
    model = glm::rotate(model, (angleRotate), glm::vec3(0.0, 1.0, 0.0));

    // Now scale it to the appropiate size
		model = glm::scale(model, glm::vec3(radius, radius, radius)); */

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

