#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <sstream>
#include <btBulletDynamicsCommon.h>
using namespace std;

#include "graphics_headers.h"
#include "camera.h"
#include "shader.h"
#include "model.h"
#include "physics.h"

class Graphics
{
  public:
    Graphics();
	  Graphics(Physics * physicsWorld);
    ~Graphics();
    bool Initialize(int width, int height, char* vertexFilename, char* fragmentFilename, char* propertiesFilename, std::vector<std::string> allFiles);
    void Update(unsigned int dt);
	  void Render();

    void processInput(unsigned int DT);
    void mouseMovement(float deltaX, float deltaY, unsigned int DT);
    void toggleFreeFly();
	  void updateCamera();
    void updateCamera(SDL_Keycode keycode);

	  void MoveCube(std::string command);
	  model* getModel(int objIndex);

    bool SwapShaders();

    void IncreaseAmbience();
    void DecreaseAmbience();
    void IncreaseSpecular();
    void DecreaseSpecular();
    void ChangeColorFilter();

    void LaunchPlunger();

  private:
    std::string ErrorString(GLenum error);

    Camera *m_camera;
    Shader *m_shader;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;

    GLint m_AmbientProduct;
    GLint m_DiffuseProduct;
    GLint m_SpecularProduct;
    GLint m_LightPosition;
    GLint m_Shininess;

    glm::vec4 ambient;
    glm::vec4 specular;
    
	  vector<model*> m_objs;
	  vector<objProp> m_properties;

    vector<glm::vec4> rainbowColors;
    int currentColor;

    bool shaderFlag;

	  unsigned int m_dt;

	  Physics *physicsWorld;

    // util functions
    void initProperties(char*);
};

#endif /* GRAPHICS_H */
