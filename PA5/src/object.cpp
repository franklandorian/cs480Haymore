#include "object.h"
#include <stdlib.h>
#include <time.h>


object::object(char* filename)
{  
  Assimp::Importer importer;
  srand(time(NULL));
  const aiScene *scene = importer.ReadFile(filename, aiProcess_Triangulate);
  aiMesh **meshes = scene->mMeshes;
  for(unsigned int iMesh = 0; iMesh < scene->mNumMeshes; iMesh++){
    aiFace *faces = meshes[iMesh]->mFaces;
    aiVector3D *vertices = meshes[iMesh]->mVertices;
    for(unsigned int iFaces = 0; iFaces < meshes[iMesh]->mNumFaces; iFaces++){
			for(unsigned int index = 0; index < 3; index++){
        Indices.push_back(faces[iFaces].mIndices[index]);
      }
    }
	  for(unsigned int iVert = 0; iVert < meshes[iMesh]->mNumVertices; iVert++){
      glm::vec3 temp_vertex(vertices[iVert].x, vertices[iVert].y, vertices[iVert].z);
      glm::vec3 temp_color(glm::vec3(0.5,0.5,0.5));
      Vertex verts(temp_vertex, temp_color);
      Vertices.push_back(verts);
    }
    std::cout << iMesh << std::endl;
  }

  // reading input filename
  /*FILE * file = fopen(filename, "r");
  srand(time(NULL));
  if(file == NULL){
    printf("Impossible to open the file. Check file location/name.\n");
    throw "Filename mismatch";
  }
  while(1){
    char header[128];
    int res = fscanf(file, "%s", header);
    if(res == EOF){ break;}

    if(strcmp(header, "v") == 0){
      glm::vec3 temp_vertex, temp_color;
      Vertex vertex(temp_vertex, temp_color);
      fscanf(file, "%f %f %f\n", &vertex.vertex.x, &vertex.vertex.y, &vertex.vertex.z);
      vertex.color.r = rand()%100/100.0;
      vertex.color.g = rand()%100/100.0;
      vertex.color.b = rand()%100/100.0;
      Vertices.push_back(vertex);
    }else if(strcmp(header, "f") == 0){
      unsigned int index[3];
      fscanf(file, "%u %u %u\n", &index[0], &index[1], &index[2]);
      for(int i = 0; i < 3; i++){
        Indices.push_back(index[i]);
      }
    }
  }*/

  // The index works at a 0th index
  /*for(unsigned int i = 0; i < Indices.size(); i++)
  {
    Indices[i] = Indices[i] - 1;
  }*/

  setDefault();

  rotationAngle = revolveAngle = 0.0f;

  glGenBuffers(1, &VB);
  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

  glGenBuffers(1, &IB);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);
  
}

object::~object()
{
  Vertices.clear();
  Indices.clear();
}
/*
 *	object::Update() currently makes the object revovle around the world center and then rotates about its own y axis.
 *	this is done by first rotating the object then translating it and then rotating again. The first rotation is so we get the revolving motion
 *	while the second rotation is so we get the spinning motion. 
 *	rotationMod is simple how fast and which direction the object is spinning
 */
void object::Update(unsigned int dt)
{
  revolveAngle +=  moving * revolveDirection * revolveMod * (int)dt * M_PI/2500;
  rotationAngle += moving * rotationDirection * rotationMod * (int)dt * M_PI/2500;
  translationMatrix = glm::rotate(glm::translate(glm::rotate(glm::mat4(1.0f), (revolveAngle), glm::vec3(0.0, 1.0, 0.0)), glm::vec3(5.0, 0.0, 0.0)), (-revolveAngle), glm::vec3(0.0,1.0,0.0));
  model = glm::scale(translationMatrix * glm::rotate(glm::mat4(1.0f), rotationAngle, glm::vec3(0.0, 1.0, 0.0)), glm::vec3(scaleFactor,scaleFactor,scaleFactor));
}

void object::buttonHandler(SDL_Keycode& sym){
  switch (sym){
    case SDLK_q:
      rotateClockwise();
      break;
    case SDLK_e:
      rotateCClockwise();
      break;
    case SDLK_a:
      revolveClockwise();
      break;
    case SDLK_d:
      revolveCClockwise();
      break;
    case SDLK_SPACE:
      toggleMovement();
      break;
    case SDLK_w:
      scaleFactor += 0.1f;
      break;
    case SDLK_s:
      scaleFactor -= 0.1f;
      break;
    case SDLK_UP:
      rotationMod += 1;
      break;
    case SDLK_DOWN:
      rotationMod -= 1;
      break;
    case SDLK_RIGHT:
      revolveMod += 0.5f;
      break;
    case SDLK_LEFT:
      revolveMod -= 0.5f;
      break;
    case SDLK_r:
      setDefault();
      break;
  }
}

void object::rotateCClockwise(){
  rotationDirection = -1;
}

void object::rotateClockwise(){
  rotationDirection = 1;
}

void object::revolveCClockwise(){
  revolveDirection = -1;
}

void object::revolveClockwise(){
  revolveDirection = 1;
}

void object::toggleMovement(){
  moving = !moving;
}

void object::swapRotation(){
  if(rotationDirection == 1){
    this->rotateCClockwise();
  }else{
    this->rotateClockwise();
  }
}

void object::swapRevolve(){
  if(revolveDirection == 1){
    this->revolveCClockwise();
  }else{
    this->revolveClockwise();
  }
}

void object::setDefault(){
  moving = true;
  revolveDirection = 1;
  revolveMod = 1;
  scaleFactor = 1.0f;
  rotationDirection = 1;
  rotationMod = 4;
}

void object::mouseClick(SDL_MouseButtonEvent& mouseEvent){
	switch(mouseEvent.button){
		case SDL_BUTTON_LEFT:
			swapRotation();
			break;
		case SDL_BUTTON_RIGHT:
			swapRevolve();
			break;
	}
}

glm::mat4 object::GetModel()
{
  return model;
}

glm::mat4 object::GetTranslationM(){
  return translationMatrix;
}

void object::Render()
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


