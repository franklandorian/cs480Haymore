#ifndef CAMERA_H
#define CAMERA_H

#include "graphics_headers.h"
#include "model.h"

class Camera
{
  public:
    Camera();
    ~Camera();
    bool Initialize(int w, int h);
    glm::mat4 GetProjection();
    glm::mat4 GetView();
    void toggleFreeFly();
    void setKeyDown(bool state);
    void setFocus(model* model, glm::vec3 followDistance);
    void setFocus(glm::vec3 position, glm::vec3 followDistance);
    void update();
    void update(unsigned int DT);
    void mouseMovement(float deltaX, float deltaY, unsigned int DT);
    glm::vec3 getCameraPos();
		void printCameraPos();
      
  private:
    float yaw;
    float pitch;
  	bool focusIsOnModel;
    bool isFreeFly;
    float cameraSpeed;

    glm::mat4 projection;
    glm::mat4 view;
    model* modelFocus;
    glm::vec3 vec3Focus;
    glm::vec3 followDistance;


    glm::vec3 cameraPos;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;


};

#endif /* CAMERA_H */
