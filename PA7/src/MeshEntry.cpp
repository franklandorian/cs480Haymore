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

    angleRotate = 0;
    return true;
}

glm::mat4 meshEntry::GetModel(){
    return model;
}

void meshEntry::Update(unsigned int dt, float radius, float revolution, float rotationSpeed, float orbitSpeed, float xPos, float yPos, float zPos ){

    // First where it should translate to
    dt *= 20;

		if (isMoon){
        // 32/5190 is the scaling factor I used to scale the distances, yeah it's hard coded. I'm sorry
        // Also the orbit speed is relative to the revolution
        angleRev += (dt) * M_PI/100000 * (orbitSpeed / revolution * 32 / 5910);
        model = glm::translate(glm::mat4(1.0f), glm::vec3( xPos + ((0.223f + revolution) * (glm::sin(angleRev))), yPos + 0, zPos + ((0.223ff + revolution) * (glm::cos(angleRev))) ));
		}
    else if(!isSun){
        // 32/5190 is the scaling factor I used to scale the distances, yeah it's hard coded. I'm sorry
        // Also the orbit speed is relative to the revolution
        angleRev += (dt) * M_PI/100000 * (orbitSpeed / revolution * 32 / 5910);
        model = glm::translate(glm::mat4(1.0f), glm::vec3( xPos + ((3.0f + revolution) * (glm::sin(angleRev))), yPos + 0, zPos + ((3.0f + revolution) * (glm::cos(angleRev))) ));
    } else {
        angleRev += (dt) * M_PI/100000 * (orbitSpeed);
        model = glm::translate(glm::mat4(1.0f), glm::vec3( revolution * (glm::sin(angleRev)), 0, revolution * (glm::cos(angleRev)) ));
    }

    // Next rotate the actual object
    angleRotate += (dt) * M_PI/100000 * rotationSpeed;
    model = glm::rotate(model, (angleRotate), glm::vec3(0.0, 1.0, 0.0));

    // Now scale it to the appropiate size
	model = glm::scale(model, glm::vec3(radius, radius, radius));

}

void meshEntry::SetStart(float angle, int planetIndex, int moon){
    angleRev = angle;
    isSun = (planetIndex == 0 || planetIndex >= 10); // Checks if this is not space or the sun
		isMoon = moon;
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
