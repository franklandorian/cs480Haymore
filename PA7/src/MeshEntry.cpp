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
    // angleRev = ((float) rand() / (RAND_MAX)) * 360;
    // std::cout << angleRev << std::endl;
    angleRotate = 0;
    return true;
}

glm::mat4 meshEntry::GetModel(){
    return model;
}

void meshEntry::Update(unsigned int dt, int offset, float radius, float rotation, float revolution){
    angleRev += (dt) * M_PI/10000;
    if(rotation)
        model = glm::translate(glm::mat4(1.0f), glm::vec3( (3.0f + revolution) * (glm::sin(angleRev)), 0, (3.0f + revolution) * (glm::cos(angleRev)) ));
    else
        model = glm::translate(glm::mat4(1.0f), glm::vec3( revolution * (glm::sin(angleRev)), 0, revolution * (glm::cos(angleRev)) ));
    angleRotate += dt * M_PI/1000;
    model = glm::rotate(model, (rotation * angleRotate), glm::vec3(0.0, 1.0, 0.0));
	model = glm::scale(model, glm::vec3(radius, radius, radius));
}

void meshEntry::SetStart(float angle){
    angleRev = angle;
}