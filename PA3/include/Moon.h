#ifndef Moon_H
#define Moon_H

#include <vector>
#include "graphics_headers.h"
#include "Cube.h"

class Moon
{
  public:
    Moon();
    Moon(Cube* planet);
    ~Moon();
    void Update(unsigned int dt);
    void Render();
    void mouseClick(SDL_MouseButtonEvent& mouseEvent);
    void buttonHandler(SDL_Keycode& keysym);

    glm::mat4 GetModel();

  private:
    void rotateCClockwise();
    void rotateClockwise();
    void revolveCClockwise();
    void revolveClockwise();
    void toggleMovement();
    void swapRotation();
    void swapRevolve();

    void setDefault();

    glm::mat4 model;
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;
    glm::mat4 translationMatrix;

    Cube* p_planet;

    bool moving;

    float revolveMod;
    int revolveDirection;
    int rotationMod;
    int rotationDirection;

    float scaleFactor;
    float revolveAngle;
    float rotationAngle;

};

#endif /* Moon_H */
