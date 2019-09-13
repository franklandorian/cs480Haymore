#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <iostream>
using namespace std;

#include "graphics_headers.h"
#include "camera.h"
#include "shader.h"
#include "Cube.h"
#include "Moon.h"

class Graphics
{
  public:
    Graphics();
    ~Graphics();
    bool Initialize(int width, int height, char* vertexFilename, char* fragmentFilename);
    void Update(unsigned int dt);
    void Render();
    Cube* getCube();

  private:
    std::string ErrorString(GLenum error);

    Camera *m_camera;
    Shader *m_shader;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;

    Cube *m_cube;
    Moon *m_moon;
};

#endif /* GRAPHICS_H */
