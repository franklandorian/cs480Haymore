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
    void mouseClick(SDL_MouseButtonEvent& mouseEvent);
    void buttonHandler(SDL_Keycode& sym);

    glm::mat4 GetModel();
    glm::mat4 GetTranslationM();

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
    glm::mat4 translationMatrix;
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;

    bool moving;

    float revolveMod;
    int revolveDirection;
    int rotationMod;
    int rotationDirection;

    float scaleFactor;
    float revolveAngle;
    float rotationAngle;

};

#endif /* Cube_H */