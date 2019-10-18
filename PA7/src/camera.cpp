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
  isFreeFly = false;
  focusIsOnModel = false;
  cameraPos = glm::vec3(0.0f,0.0f,3.0f);
  cameraFront = glm::vec3(0.0,0.0,-1.0);
  cameraUp = glm::vec3(0.0,-1.0,0.0);
  yaw = 0.0f;
  pitch = 0.0f;
  setFocus(glm::vec3(0.0,0.0,0.0), glm::vec3(0.0,-8.0,-16.0));
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

void Camera::toggleFreeFly(){
	isFreeFly = !isFreeFly;
	SDL_SetRelativeMouseMode(isFreeFly? SDL_TRUE : SDL_FALSE);
}

void Camera::setFocus(model* model, glm::vec3 followDistance){
  focusIsOnModel = true;
  this->followDistance = followDistance;
  modelFocus = model;
}

void Camera::setFocus(glm::vec3 position, glm::vec3 followDistance){
  focusIsOnModel = false;
  this->followDistance = followDistance;
  vec3Focus = position;
}

void Camera::update(unsigned int DT){
	const Uint8* keystate = SDL_GetKeyboardState(NULL);
	cameraSpeed = 2.5f * DT/1000;
	if(keystate[SDL_SCANCODE_W] == 1){
		cameraPos += cameraSpeed * cameraFront;
	}
	if(keystate[SDL_SCANCODE_S] == 1){
		cameraPos -= cameraSpeed * cameraFront;
	}
	if(keystate[SDL_SCANCODE_A] == 1){
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	if(keystate[SDL_SCANCODE_D] == 1){
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	if(keystate[SDL_SCANCODE_Q] == 1){
		cameraPos += cameraSpeed * cameraUp;
	}
	if(keystate[SDL_SCANCODE_E] == 1){
		cameraPos -= cameraSpeed * cameraUp;
	}
}

void Camera::update(){
  if(isFreeFly){
  	if(pitch > 89.0f) pitch = 89.0f;
  	if(pitch < -89.0f) pitch = -89.0f;
  	cameraFront.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
  	cameraFront.y = sin(glm::radians(pitch));
  	cameraFront.z = -(cos(glm::radians(pitch)) * sin(glm::radians(yaw)));
  	cameraFront = glm::normalize(cameraFront);
  	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
  }else{
	if(focusIsOnModel){
	glm::mat4 transformMatrix = modelFocus->GetModel();
	view = glm::lookAt(glm::vec3(followDistance.x + transformMatrix[3].x, followDistance.y + transformMatrix[3].y, followDistance.z + transformMatrix[3].z),
	                   glm::vec3(transformMatrix[3].x, transformMatrix[3].y, transformMatrix[3].z),
	                   cameraUp);  
	}else{
	  view = glm::lookAt(followDistance, vec3Focus, cameraUp);
	}
  }
}


void Camera::mouseMovement(float deltaX, float deltaY, unsigned int DT){
	float sensitivity = 0.1f;
	deltaX *= sensitivity;
	deltaY *= sensitivity;
	yaw += deltaX;
	pitch += deltaY;
}

glm::vec3 Camera::getCameraPos(){
	return cameraPos;
}



