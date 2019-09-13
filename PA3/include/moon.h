#ifndef Moon_H
#define Moon_H

#include <vector>
#include "graphics_headers.h"

class Moon
{
  public:
    Moon();
    ~Moon();
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

#endif /* Moon_H */
