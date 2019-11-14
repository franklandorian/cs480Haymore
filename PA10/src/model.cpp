#include "model.h"
#include <stdlib.h>
#include <time.h>

model::model(std::string filename, objProp props)
{  
  Assimp::Importer importer;
  const aiScene *scene = importer.ReadFile(filename, aiProcess_Triangulate);
  
  // This tells us if we need the triangle mesh for bullet
  triangleFlag = (props.type == 2);
  // triangleFlag = false;
  if(!triangleFlag){
    triangleMesh = nullptr;
  }

  // set object properties
	m_Prop.name = props.name;
	m_Prop.type = props.type;
	m_Prop.shape = props.shape;
	m_Prop.startPos[0] = props.startPos[0];
	m_Prop.startPos[1] = props.startPos[1];
	m_Prop.startPos[2] = props.startPos[2];
  m_Prop.size = props.size;
  // std::cout << 	m_Prop.startPos[0] << props.startPos[1] << props.startPos[1];

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
      glm::vec3 temp_normals(paiMesh->mNormals[iVert].x, paiMesh->mNormals[iVert].y, paiMesh->mNormals[iVert].z);
      if(paiMesh->HasTextureCoords(0)){
        temp_tCoords.x = 1-paiMesh->mTextureCoords[0][iVert].x;
        temp_tCoords.y = paiMesh->mTextureCoords[0][iVert].y;
      }
      Vertex verts(temp_vertex, temp_color, temp_tCoords, temp_normals);
      temp_vertices.emplace_back(verts);
    }

    if(triangleFlag){
        btVector3 triangle[3];
        triangleMesh = new btTriangleMesh();
        for(unsigned int k = 0; k < paiMesh->mNumFaces; k++){
          aiFace * face = &paiMesh->mFaces[k];
          for(unsigned int l = 0; l < 3; l++){
            aiVector3D position = paiMesh->mVertices[face->mIndices[l]];
            triangle[l] = btVector3(position.x * m_Prop.size, position.y * m_Prop.size, position.z * m_Prop.size);
          }
        }
        triangleMesh->addTriangle(triangle[0], triangle[1], triangle[2]);
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
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    for (unsigned int i = 0 ; i < meshes.size() ; i++) {
        glBindBuffer(GL_ARRAY_BUFFER, meshes[i].VB);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texture));
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshes[i].IB);

        // Bind Texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textures[i]);

        glDrawElements(GL_TRIANGLES, meshes[i].NumIndices, GL_UNSIGNED_INT, 0);
    }

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);

}

glm::mat4 model::GetModel(){
  return meshes[0].GetModel();
}

void model::Update(unsigned int dt, int objType, float x, float y, float z){
	if (objType == 1)
	{
  	meshes[0].Update(dt, m_Prop, 1, m_Prop.startPos[0], m_Prop.startPos[1], m_Prop.startPos[2]);
	}
	else if (objType == 2)
	{
		meshes[0].Update(dt, m_Prop, 0, m_Prop.startPos[0], m_Prop.startPos[1], m_Prop.startPos[2]);
	}
}

void model::buttonHandler(SDL_Keycode& sym){
	meshes[0].buttonHandler(sym);  
}

/*
void model::setChild(model* child)
{
	m_children.push_back(child);
}

glm::mat4 model::GetChildModel(int i)
{
	return m_children[i]->GetModel();
}

model* model::getChild(int i)
{
	return m_children[i];
}
*/

std::string model::getObjName() const
{
	return m_Prop.name;
}

int model::getObjType() const
{
	return m_Prop.type;
}

int model::getObjShape() const
{
	return m_Prop.shape;
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
float model::getW() const
{
	return meshes[0].getA();
}

void model::SetModel(glm::mat4 newModel)
{
  meshes[0].SetModel(newModel);
}

btTriangleMesh* model::getTriangles()
{
  return triangleMesh;
}