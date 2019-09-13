#ifndef Cube_H
#define Cube_H

#include <vector>
#include "graphics_headers.h"

class Cube
{
  public:
    Cube();
    ~Cube();
    void Update(unsigned int dt);
    void Render();
    void rotateCClockwise();
    void rotateClockwise();
    void revolveCClockwise();
    void revolveClockwise();
    void toggleMovement();
    void swapRotation();
    void swapRevolve();
    void mouseClick(SDL_MouseButtonEvent& mouseEvent);

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

#endif /* Cube_H */
