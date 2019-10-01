#ifndef object_H
#define object_H

#include <vector>
#include "graphics_headers.h"
#include <iostream>
#include <fstream>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/color4.h>

class object
{
  public:
    object(char* objectFilename);
    ~object();
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

#endif /* object_H */
