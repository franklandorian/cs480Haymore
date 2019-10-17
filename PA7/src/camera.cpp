#include "camera.h"

Camera::Camera()
{

}

Camera::~Camera()
{

}

bool Camera::Initialize(int w, int h)
{
  //--Init the view and projection matrices
  //  if you will be having a moving camera the view matrix will need to more dynamic
  //  ...Like you should update it before you render more dynamic 
  //  for this project having them static will be fine
  setFocus(glm::vec3(0.0,0.0,0.0));
  update();

    // view = glm::lookAt( glm::vec3(0.0, 8.0, -16.0), //Eye Position
    //                   glm::vec3(0.0, 0.0, 0.0), //Focus point
    //                   glm::vec3(0.0, 1.0, 0.0)); //Positive Y is up

  projection = glm::perspective( 45.0f, //the FoV typically 90 degrees is good which is what this is set to
                                 float(w)/float(h), //Aspect Ratio, so Circles stay Circular
                                 0.01f, //Distance to the near plane, normally a small value like this
                                 200.0f); //Distance to the far plane, 
  return true;
}

glm::mat4 Camera::GetProjection()
{
  return glm::scale(projection, glm::vec3(2.0f,2.0f,2.0f));
}

glm::mat4 Camera::GetView()
{
  return view;
}

void Camera::setFocus(model* model){
  focusIsOnModel = true;
  modelFocus = model;
}

void Camera::setFocus(glm::vec3 position){
  focusIsOnModel = false;
  vec3Focus = position;
}

void Camera::update(){
  if(focusIsOnModel){
    glm::mat4 transformMatrix = modelFocus->GetModel();
    view = glm::lookAt(glm::vec3(0.0 + transformMatrix[3].x, 0.1 + transformMatrix[3].y, 0.1 + transformMatrix[3].z), glm::vec3(transformMatrix[3].x, transformMatrix[3].y, transformMatrix[3].z), glm::vec3(0.0,1.0,0.0));  
  }else{
    view = glm::lookAt(glm::vec3(0.0, 8.0, -16.0), vec3Focus, glm::vec3(0.0,1.0,0.0));  
  }
  
}


