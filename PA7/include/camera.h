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
    void setFocus(model* model);
    void setFocus(glm::vec3 position);
    void update();
      
  private:
  	bool focusIsOnModel;
    glm::mat4 projection;
    glm::mat4 view;
    model* modelFocus;
    glm::vec3 vec3Focus;
};

#endif /* CAMERA_H */
