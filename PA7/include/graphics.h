#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <sstream>
using namespace std;

#include "graphics_headers.h"
#include "camera.h"
#include "shader.h"
#include "object.h"
#include "model.h"

class Graphics
{
  public:
    Graphics();
    ~Graphics();
    bool Initialize(int width, int height, char* vertexFilename, char* fragmentFilename, char* settingFilename, std::vector<std::string> allFiles);
    void Update(unsigned int dt);
    void processInput(unsigned int DT);
    void updateCamera();
    void mouseMovement(float deltaX, float deltaY, unsigned int DT);
    void toggleFreeFly();
    void updateCamera(SDL_Keycode keycode);
    void increaseSpeed();
    void decreaseSpeed();
    void Render();

  private:
    std::string ErrorString(GLenum error);

    Camera *m_camera;
    Shader *m_shader;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;

	vector<model*> m_objs;
	vector<setting> m_settings; 

	// util functions
	void initSetting(char* settingFilename);

	unsigned int m_dt;
};

#endif /* GRAPHICS_H */
