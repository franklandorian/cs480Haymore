#ifndef MESH_ENTRY_H
#define MESH_ENTRY_H

#include <vector>
#include <iostream>
#include "graphics_headers.h"

class meshEntry {
  private:
    // meshEntry();
    // ~meshEntry();
    glm::mat4 model;
    float angle;

  public:

    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;

    unsigned int NumIndices;
    unsigned int MaterialIndex;

    bool Init(const std::vector<Vertex> incomingVertices, const std::vector<unsigned int> incomingIndices){

        Vertices = incomingVertices;
        Indices = incomingIndices;
        NumIndices = Indices.size();

        glGenBuffers(1, &VB);
        glBindBuffer(GL_ARRAY_BUFFER, VB);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

        glGenBuffers(1, &IB);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);
        
        angle = 0;

        return true;
    }

    glm::mat4 GetModel(){
        return model;
    }

    void Update(unsigned int dt){
        angle += dt * M_PI/1000;
        model = glm::translate(glm::mat4(1.0f), glm::vec3(8 * glm::sin(0.5 * angle), 0, 8 * glm::cos(0.5 * angle)));
        model = glm::rotate(model, (angle), glm::vec3(0.0, 1.0, 0.0));
    }
};

// meshEntry::meshEntry(){
//     NumIndices = 0;
// }

// meshEntry::~meshEntry(){

// }

#endif /*MESH_ENTRY_H*/