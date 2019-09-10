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
    void rotateCClockwise();
    void rotateClockwise();
    void revolveCClockwise();
    void revolveClockwise();
    void toggleMovement();
    void swapRotation();
    void swapRevolve();

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
