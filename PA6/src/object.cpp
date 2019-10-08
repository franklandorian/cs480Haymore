#include "object.h"
#include <stdlib.h>


object::object(char* filename)
{  
  Assimp::Importer importer;
  const aiScene *scene = importer.ReadFile(filename, aiProcess_Triangulate);
  
	meshNumber = scene->mNumMeshes;
	std::vector<Vertex> temp_vertices;
	std::vector<unsigned int> temp_indices;

  for(unsigned int iMesh = 0; iMesh < meshNumber; iMesh++){
    for(unsigned int iFaces = 0; iFaces < scene->mMeshes[iMesh]->mNumFaces; iFaces++){
			for(unsigned int index = 0; index < 3; index++){
        temp_indices.emplace_back(scene->mMeshes[iMesh]->mFaces[iFaces].mIndices[index]);
      }
    }
		for(unsigned int iVert = 0; iVert < scene->mMeshes[iMesh]->mNumVertices; iVert++){
      glm::vec3 temp_vertex(scene->mMeshes[iMesh]->mVertices[iVert].x,scene->mMeshes[iMesh]->mVertices[iVert].y,scene->mMeshes[iMesh]->mVertices[iVert].z);
      glm::vec3 temp_color(glm::vec3(0.0,0.0,0.0));
      glm::vec2 temp_tCoords(scene->mMeshes[iMesh]->mTextureCoords[0][iVert].x,scene->mMeshes[iMesh]->mTextureCoords[0][iVert].y);
      Vertex verts(temp_vertex, temp_color, glm::vec2(0,0));
      temp_vertices.emplace_back(verts);
    }
		meshes.push_back(temp_vertices);
		meshIndexes.push_back(temp_indices);
		VBS.push_back(iMesh);
		IBS.push_back(iMesh);
		temp_vertices.clear();
		temp_indices.clear();

    aiMaterial* mat = scene->mMaterials[iMesh];
    aiString tName;
    mat->Get(AI_MATKEY_NAME, tName);
    std::cout << tName.C_Str() << std::endl;
    Magick::Image *image = new Magick::Image(tName.C_Str());
    Magick::Blob blob;
    image->write(&blob, "RGBA");

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->columns(), image->rows(), 0, GL_RGBA, GL_UNSIGNED_BYTE, blob.data());
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    delete image;
  }


  

  setDefault();

  rotationAngle = revolveAngle = 0.0f;

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

	for(unsigned int i = 0; i < meshes.size(); i++){
     /*glBindBuffer(GL_ARRAY_BUFFER, VBS[i]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20);*/

		glGenBuffers(1, &VBS[i]);
    glBindBuffer(GL_ARRAY_BUFFER, VBS[i]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * meshes[i].size(), &meshes[i][0], GL_STATIC_DRAW);

    glGenBuffers(1, &IBS[i]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBS[i]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * meshIndexes[i].size(), &meshIndexes[i][0], GL_STATIC_DRAW);

		//glDrawElements(GL_TRIANGLES, meshIndexes[i].size(), GL_UNSIGNED_INT, 0);
	}
	
    /*glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);*/
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
  /*glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,color));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

  glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);*/

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    for (unsigned int i = 0 ; i < meshes.size() ; i++) {
        glBindBuffer(GL_ARRAY_BUFFER, VBS[i]);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBS[i]);

        glDrawElements(GL_TRIANGLES, meshIndexes[i].size(), GL_UNSIGNED_INT, 0);
    }

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);

}


