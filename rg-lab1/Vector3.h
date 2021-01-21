#ifndef VECTOR3_H
#define VECTOR3_H

#include <glm/glm.hpp>

class Vector3
{
private:

public:
	float x, y, z;

	Vector3();
	Vector3(float x, float y, float z);
	glm::vec3 asGLMVector();
};

#endif