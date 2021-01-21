#ifndef MESH_H
#define MESH_H

#include "Vector3.h"

#include <string>
#include <vector>

class Mesh
{
private:
	int vertexCount = 0;
	int triangleCount = 0;

public:
	std::vector<Vector3> vertices;
	std::vector<int> triangles;

	Mesh();
	void loadFromFile(std::string objPath);
	void normalize();
};

#endif