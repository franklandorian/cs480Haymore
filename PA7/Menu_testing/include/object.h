#ifndef OBJECT_H
#define OBJECT_H
#define GLM_ENABLE_EXPERIMENTAL

#include <vector>
#include "graphics_headers.h"

class Object
{
  public:
    Object();
    ~Object();
    void Update(unsigned int dt, int keyValue);
    void Render();

    glm::mat4 GetModel();

  private:
    glm::mat4 model;
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;

    float angleOrbit;
		float angleRotate;

		// conditionals for orbit and rotation
		bool orbit = true;			// true = clockwise, false = counterclock
		bool rotation = true;		// true = clockwise, false = counterclock
		bool pauseOrbit = false;			
		bool pauseRotate = false;
};

#endif /* OBJECT_H */
