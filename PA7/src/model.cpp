#include "model.h"
#include <stdlib.h>
#include <time.h>

model::model(std::string filename, setting set)
{  
  Assimp::Importer importer;
  const aiScene *scene = importer.ReadFile(filename, aiProcess_Triangulate);
  
  // Count and allocate proper memory for the meshes
	meshNumber = scene->mNumMeshes;
  meshes.resize(meshNumber);
  textures.resize(scene->mNumMaterials);

  // Loop throw through the number of meshes
  for(unsigned int iMesh = 0; iMesh < meshNumber; iMesh++){

    const aiMesh* paiMesh = scene->mMeshes[iMesh];
    InitMesh(iMesh, paiMesh);

    // Grab the texture names and run them
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

	// fill settings
	m_setting.name = set.name ;
	m_setting.index = set.index;
	m_setting.radius = set.radius/2; // Since this is radius, we half the current value
	m_setting.rotationSpeed = set.rotationSpeed;
	m_setting.orbitSpeed = set.orbitSpeed;
	m_setting.revolution = set.revolution;
 	m_setting.start = set.start;
	m_setting.numMoons = set.numMoons;
	m_setting.moon = set.moon;


 	meshes[0].SetStart(m_setting.start, m_setting.index, m_setting.moon);

	// srand(time(NULL));
}

void model::InitMesh(unsigned int Index, const aiMesh* paiMesh){
  // Temporary Storage
  std::vector<Vertex> temp_vertices;
  std::vector<unsigned int> temp_indices;

    // Store the faces
    for(unsigned int iFaces = 0; iFaces < paiMesh->mNumFaces; iFaces++){
      for(unsigned int index = 0; index < 3; index++){
        temp_indices.emplace_back(paiMesh->mFaces[iFaces].mIndices[index]);
      }
    }

    // Store the Vertices
    for(unsigned int iVert = 0; iVert < paiMesh->mNumVertices; iVert++){
      glm::vec3 temp_vertex(paiMesh->mVertices[iVert].x, paiMesh->mVertices[iVert].y,paiMesh->mVertices[iVert].z);
      glm::vec3 temp_color(glm::vec3(0.0,0.0,0.0));
      glm::vec2 temp_tCoords(0,0);
      if(paiMesh->HasTextureCoords(0)){
        temp_tCoords.x = 1-paiMesh->mTextureCoords[0][iVert].x;
        temp_tCoords.y = paiMesh->mTextureCoords[0][iVert].y;
      }
      Vertex verts(temp_vertex, temp_color, temp_tCoords);
      temp_vertices.emplace_back(verts);
    }

    // Store into mesh
    meshes[Index].Init(temp_vertices, temp_indices);

		srand( time(0) );
}

model::~model()
{
  // meshes.clear();
  // meshIndexes.clear();
}

void model::Render()
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
        glBindBuffer(GL_ARRAY_BUFFER, meshes[i].VB);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texture));

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshes[i].IB);

        // Bind Texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textures[i]);

        glDrawElements(GL_TRIANGLES, meshes[i].NumIndices, GL_UNSIGNED_INT, 0);
    }

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);

}

glm::mat4 model::GetModel(){
  return meshes[0].GetModel();
}

void model::Update(unsigned int dt, float revOffset, float x, float y, float z, std::string name){
	if (name.compare("Space") == 0)
	{
		//std::cout << x << " " << y << " " << z << "\n";
		meshes[0].Update(dt, m_setting.radius, m_setting.revolution, m_setting.rotationSpeed, m_setting.orbitSpeed, revOffset, x, y, z);		// space tether overload
	}
	else if (name.compare("moon") == 0)
	{
			//std::cout << m_setting.radius << std::endl;
			if (m_setting.radius > 0.08f)
				meshes[0].Update(dt, m_setting.radius/6.0f,	m_setting.revolution, m_setting.rotationSpeed, m_setting.orbitSpeed, revOffset , x, y, z);		// moon overload
			else
				meshes[0].Update(dt, m_setting.radius/0.5f,	m_setting.revolution, m_setting.rotationSpeed, m_setting.orbitSpeed, revOffset , x, y, z);		// moon overload
	}
	else
  	meshes[0].Update(dt, m_setting.radius, m_setting.revolution, m_setting.rotationSpeed, m_setting.orbitSpeed, revOffset);

}

void model::setMoon(model* moon)
{
	m_moonObj = moon;
	m_moons.push_back(m_moonObj);
}

void model::moonUpdates(unsigned int dt, float planetOffset, int i)
{
	// assign some y offset for moons to stagger around planets and look better
	float y = i%2==0 ?  getY()+m_setting.radius / (i+1):  getY()-m_setting.radius / (i+1);
	m_moons[i]->Update(dt, planetOffset, getX(), y, getZ(), "moon"); 
}

glm::mat4 model::GetMoonModel(int i)
{
	return m_moons[i]->GetModel();
}

model* model::getMoon(int i)
{
	return m_moons[i];
}

// getters for position of object
float model::getX() const
{
	return meshes[0].getX();
}
float model::getY() const
{
	return meshes[0].getY();
}
float model::getZ() const
{
	return meshes[0].getZ();
}

int model::isMoon() const
{
	return m_setting.moon;
}

int model::getIndex() const
{
	return m_setting.index;
}

std::string model::getName() const
{
	return m_setting.name;
}

void model::speedUp(){
  // Doesn't matter what mesh, we just need to change speed of ALL
  meshes[0].speedUp();
}

void model::speedDown(){
  // Doesn't matter what mesh, we just need to change speed of ALL
  meshes[0].speedDown();
}

float model::getRadius() const
{
	return m_setting.radius;
}

float model::getNumMoons() const
{
	return m_setting.numMoons;
}

void model::reset()
{
	//meshes[0]->
}
