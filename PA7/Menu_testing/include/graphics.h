#ifndef GRAPHICS_H
#define GRAPHICS_H
#define GLM_ENABLE_EXPERIMENTAL

#include <iostream>
using namespace std;

#include "graphics_headers.h"
#include "camera.h"
#include "shader.h"
#include "object.h"

class Graphics
{
  public:
    Graphics();
		Graphics(string vShader, string fShader);
    ~Graphics();
    bool Initialize(int width, int height);
    void Update(unsigned int dt, int keyValue);
    void Render();

  private:
    std::string ErrorString(GLenum error);

    Camera *m_camera;
    Shader *m_shader;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;

    Object *m_cube;

		// String variables to pass in to the Shader object
		std::string m_Vertex;
		std::string m_Fragment;
};

#endif /* GRAPHICS_H */
