#ifndef MESH_ENTRY_H
#define MESH_ENTRY_H

#include <vector>
#include <iostream>
#include <cstdlib>
#include <time.h>
#include "graphics_headers.h"

class meshEntry {
  private:
    // meshEntry();
    // ~meshEntry();
    glm::mat4 model;
    float angleRev, angleRotate;
    bool isSun;

  public:

    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;

    unsigned int NumIndices;
    unsigned int MaterialIndex;

    bool Init(const std::vector<Vertex> incomingVertices, const std::vector<unsigned int> incomingIndices);
    void SetStart(float angle, int planetIndex);
    glm::mat4 GetModel();
    void Update(unsigned int dt, float radius, float revolution, float rotationSpeed, float orbitSpeed);
};


#endif /*MESH_ENTRY_H*/
