#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include "graphics_headers.h"

class Object
{
  public:
    Object();
    ~Object();
    void Update(unsigned int dt);
    void Render();
    void spinCClockwise();
    void spinClockwise();
    void revolveCClockwise();
    void revolveClockwise();
    void toggleMovement();

    glm::mat4 GetModel();

  private:
    glm::mat4 model;
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;

    bool moving;

    int revolveDirection;
    int rotationMod;
    int rotationDirection;

    float revolveAngle;
    float rotationAngle;

};

#endif /* OBJECT_H */
