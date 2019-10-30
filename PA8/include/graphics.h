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

class Graphics
{
  public:
    Graphics();
    ~Graphics();
    bool Initialize(int width, int height, char* vertexFilename, char* fragmentFilename, char* propertiesFilename, std::vector<std::string> allFiles);
    void Update(unsigned int dt);
		void Render();

    void processInput(unsigned int DT);
    void mouseMovement(float deltaX, float deltaY, unsigned int DT);
    void toggleFreeFly();
		void updateCamera();
   	void updateCamera(SDL_Keycode keycode);
		model* getModel(int objIndex);

  private:
    std::string ErrorString(GLenum error);

    Camera *m_camera;
    Shader *m_shader;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;

		vector<model*> m_objs;
		vector<objProp> m_properties; 

		unsigned int m_dt;

		// all our bullet stuff
		btBroadphaseInterface *broadphase;
		btDefaultCollisionConfiguration *collisionConfiguration;
		btCollisionDispatcher *dispatcher;
		btSequentialImpulseConstraintSolver *solver;
		btDiscreteDynamicsWorld *dynamicsWorld;

		// util functions
		void initProperties(char*);
};

#endif /* GRAPHICS_H */
