#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <iostream>
#include <vector>
#include <string>
using namespace std;

#include "graphics_headers.h"
#include "camera.h"
#include "shader.h"
#include "object.h"
#include "modelManager.h"

class Graphics
{
  public:
    Graphics();
    ~Graphics();
    bool Initialize(int width, int height, char* vertexFilename, char* fragmentFilename, std::string objectFilename, std::vector<std::string> allFiles);
    void Update(unsigned int dt);
    void Render();
    modelManager* getObject();

  private:
    std::string ErrorString(GLenum error);

    Camera *m_camera;
    Shader *m_shader;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;

    modelManager *m_object;
		vector<modelManager*> m_objs;
};

#endif /* GRAPHICS_H */
