#include "object.h"
#include <stdlib.h>


void object::InitMesh(unsigned int Index, const aiMesh* paiMesh){
  std::vector<Vertex> temp_vertices;
  std::vector<unsigned int> temp_indices;

    for(unsigned int iFaces = 0; iFaces < paiMesh->mNumFaces; iFaces++){
      for(unsigned int index = 0; index < 3; index++){
        temp_indices.emplace_back(paiMesh->mFaces[iFaces].mIndices[index]);
      }
    }
    for(unsigned int iVert = 0; iVert < paiMesh->mNumVertices; iVert++){
      glm::vec3 temp_vertex(paiMesh->mVertices[iVert].x, paiMesh->mVertices[iVert].y,paiMesh->mVertices[iVert].z);
      glm::vec3 temp_color(glm::vec3(0.0,0.0,0.0));
      glm::vec2 temp_tCoords(0,0);
      if(paiMesh->HasTextureCoords(0)){
        temp_tCoords.x = paiMesh->mTextureCoords[0][iVert].x;
        temp_tCoords.y = paiMesh->mTextureCoords[0][iVert].y;
      }
      Vertex verts(temp_vertex, temp_color, temp_tCoords);
      temp_vertices.emplace_back(verts);
    }

    meshes.push_back(temp_vertices);
    meshIndexes.push_back(temp_indices);

    glGenBuffers(1, &VBS[Index]);
    glBindBuffer(GL_ARRAY_BUFFER, VBS[Index]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * meshes[Index].size(), &meshes[Index][0], GL_STATIC_DRAW);

    glGenBuffers(1, &IBS[Index]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBS[Index]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * meshIndexes[Index].size(), &meshIndexes[Index][0], GL_STATIC_DRAW);
}

object::object(char* filename)
{  
  Assimp::Importer importer;
  const aiScene *scene = importer.ReadFile(filename, aiProcess_Triangulate);
  
	meshNumber = scene->mNumMeshes;

  VBS.resize(meshNumber);
  IBS.resize(meshNumber);

	std::vector<Vertex> temp_vertices;
	std::vector<unsigned int> temp_indices;
  textures.resize(scene->mNumMaterials);
  std::cout << scene->mNumMaterials << std::endl;
  for(unsigned int iMesh = 0; iMesh < meshNumber; iMesh++){

    const aiMesh* paiMesh = scene->mMeshes[iMesh];
    InitMesh(iMesh, paiMesh);



    aiString tName;
    aiMaterial* mat = scene->mMaterials[iMesh+1];
    char textureFile[256]; 
    if(scene->mNumMaterials > 1 && mat->GetTextureCount(aiTextureType_DIFFUSE) > 0 && mat->GetTexture(aiTextureType_DIFFUSE, 0, &tName) == AI_SUCCESS){
      strcpy(textureFile,"../assets/");
      strcat(textureFile, tName.C_Str());
      Magick::Image *image = new Magick::Image(textureFile);
      Magick::Blob blob;
      image->write(&blob, "RGBA");

      glGenTextures(1, &textures[iMesh]);
      glBindTexture(GL_TEXTURE_2D, textures[iMesh]);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->columns(), image->rows(), 0, GL_RGBA, GL_UNSIGNED_BYTE, blob.data());
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      delete image;
    }
  }

  setDefault();
  rotationAngle = revolveAngle = 0.0f;

}

object::~object()
{
  meshes.clear();
  meshIndexes.clear();
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
    glEnableVertexAttribArray(2);

    for (unsigned int i = 0 ; i < meshes.size() ; i++) {
        glBindBuffer(GL_ARRAY_BUFFER, VBS[i]);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texture));

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBS[i]);

        // Bind Texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textures[i]);

        glDrawElements(GL_TRIANGLES, meshIndexes[i].size(), GL_UNSIGNED_INT, 0);
    }

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);

}


