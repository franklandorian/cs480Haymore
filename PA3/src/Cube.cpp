#include "Cube.h"

Cube::Cube()
{  
  /*
    # Blender File for a Cube
    o Cube
    v 1.000000 -1.000000 -1.000000
    v 1.000000 -1.000000 1.000000
    v -1.000000 -1.000000 1.000000
    v -1.000000 -1.000000 -1.000000
    v 1.000000 1.000000 -0.999999
    v 0.999999 1.000000 1.000001
    v -1.000000 1.000000 1.000000
    v -1.000000 1.000000 -1.000000
    s off
    f 2 3 4
    f 8 7 6
    f 1 5 6
    f 2 6 7
    f 7 8 4
    f 1 4 8
    f 1 2 4
    f 5 8 6
    f 2 1 6
    f 3 2 7
    f 3 7 4
    f 5 1 8
  */

  Vertices = {
    {{1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, 0.0f}},
    {{1.0f, -1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
    {{-1.0f, -1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
    {{-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, 1.0f}},
    {{1.0f, 1.0f, -1.0f}, {1.0f, 1.0f, 0.0f}},
    {{1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 1.0f}},
    {{-1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0f}},
    {{-1.0f, 1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}}
  };

  Indices = {
    2, 3, 4,
    8, 7, 6,
    1, 5, 6,
    2, 6, 7,
    7, 8, 4,
    1, 4, 8,
    1, 2, 4,
    5, 8, 6,
    2, 1, 6,
    3, 2, 7,
    3, 7, 4,
    5, 1, 8
  };

  // The index works at a 0th index
  for(unsigned int i = 0; i < Indices.size(); i++)
  {
    Indices[i] = Indices[i] - 1;
  }

  moving = true;
  revolveDirection = 1;
  rotationAngle = revolveAngle = 0.0f;
  rotationDirection = 1;
  rotationMod = 4;

  glGenBuffers(1, &VB);
  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

  glGenBuffers(1, &IB);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);
  
}

Cube::~Cube()
{
  Vertices.clear();
  Indices.clear();
}
/*
 *	Cube::Update() currently makes the Cube revovle around the world center and then rotates about its own y axis.
 *	this is done by first rotating the Cube then translating it and then rotating again. The first rotation is so we get the revolving motion
 *	while the second rotation is so we get the spinning motion. 
 *	rotationMod is simple how fast and which direction the Cube is spinning
 */
void Cube::Update(unsigned int dt)
{
  revolveAngle +=  moving * revolveDirection * (int)dt * M_PI/2500;
  rotationAngle += moving * rotationDirection * rotationMod * (int)dt * M_PI/2500;
  tranlationMatrix = glm::translate(glm::mat4(1.0f), )
  model = glm::translate(glm::rotate(glm::mat4(1.0f), (revolveAngle), glm::vec3(0.0, 1.0, 0.0)), glm::vec3(7, 0.0, 0.0)) * glm::rotate(glm::mat4(1.0f), rotationAngle, glm::vec3(0.0, 1.0, 0.0));
}


void Cube::rotateCClockwise(){
  rotationDirection = -1;
}

void Cube::rotateClockwise(){
  rotationDirection = 1;
}

void Cube::revolveCClockwise(){
  revolveDirection = -1;
}

void Cube::revolveClockwise(){
  revolveDirection = 1;
}

void Cube::toggleMovement(){
  moving = !moving;
}

void Cube::swapRotation(){
  if(rotationDirection == 1){
    this->rotateCClockwise();
  }else{
    this->rotateClockwise();
  }
}

void Cube::swapRevolve(){
  if(revolveDirection == 1){
    this->revolveCClockwise();
  }else{
    this->revolveClockwise();
  }
}

void Cube::mouseClick(SDL_MouseButtonEvent& mouseEvent){
	switch(mouseEvent.button){
		case SDL_BUTTON_LEFT:
			swapRotation();
			break;
		case SDL_BUTTON_RIGHT:
			swapRevolve();
			break;
	}
}

glm::mat4 Cube::GetModel()
{
  return model;
}

void Cube::Render()
{
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,color));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

  glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
}

