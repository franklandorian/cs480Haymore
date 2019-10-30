#ifndef MESH_ENTRY_H
#define MESH_ENTRY_H

#include <vector>
#include <iostream>
#include <cstdlib>
#include <time.h>
#include <btBulletDynamicsCommon.h>
#include "graphics_headers.h"

class meshEntry {
  private:
    // meshEntry();
    // ~meshEntry();
    glm::mat4 model;
    float angleRev, angleRotate;

		void moveForward();
		void moveLeft();
		void moveBack();
		void moveRight();
		void moveUp();
		void moveDown();

		float x = 0, y = 0, z = 0;

  public:

    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;

    unsigned int NumIndices;
    unsigned int MaterialIndex;

    bool Init(const std::vector<Vertex> incomingVertices, const std::vector<unsigned int> incomingIndices);
    glm::mat4 GetModel();

    void Update(unsigned int dt, int rotation = 0, float xPos = 0.0f, float yPos = 0.0f, float zPos = 0.0f );
		void buttonHandler(SDL_Keycode&);

		// getters for position of object
		float getX() const;
		float getY() const;
		float getZ() const;
		float getA() const;
};


#endif /*MESH_ENTRY_H*/
