#include "Mesh.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

Mesh::Mesh()
{
	vertices = std::vector<Vector3>();
	triangles = std::vector<int>();
}

void Mesh::loadFromFile(std::string objPath)
{
	std::ifstream stream;
	std::string line;
	stream.open(objPath);

	while (std::getline(stream, line)) {
		if (line[0] == 'v') {
			vertexCount++;
		}
		else if (line[0] == 'f') {
			triangleCount++;
		}
	}

	vertices.resize(vertexCount);
	triangles.resize(triangleCount * 3);

	stream.close();

	stream.open(objPath);
	int vi, ti;
	vi = ti = 0;
	while (std::getline(stream, line)) {
		if (line[0] == 'v') {
			std::string buffer;
			buffer = line.substr(2, line.size() - 2);
			std::stringstream ss(buffer);
			ss >> vertices[vi].x;
			ss >> vertices[vi].y;
			ss >> vertices[vi].z;
			vi++;
		}
		else if (line[0] == 'f') {
			std::string buffer;
			buffer = line.substr(2, line.size() - 2);
			std::stringstream ss(buffer);
			ss >> triangles[ti];
			ss >> triangles[ti + 1];
			ss >> triangles[ti + 2];

			triangles[ti]--;
			triangles[ti + 1]--;
			triangles[ti + 2]--;

			ti += 3;
		}
	}
	stream.close();

	std::cout << "Mesh loaded from file: " << objPath << "\n";
	std::cout << "Vertex count: " << vertexCount << "\n";
	std::cout << "Triangle count: " << triangleCount << "\n";
}

void Mesh::normalize()
{
	float xmin, xmax, ymin, ymax, zmin, zmax;
	xmin = xmax = vertices[0].x;
	ymin = ymax = vertices[0].y;
	zmin = zmax = vertices[0].z;

	for (int i = 0; i < (int)vertices.size(); i++) {
		xmin = std::min(xmin, vertices[i].x);
		xmax = std::max(xmax, vertices[i].x);
		ymin = std::min(ymin, vertices[i].y);
		ymax = std::max(ymax, vertices[i].y);
		zmin = std::min(zmin, vertices[i].z);
		zmax = std::max(zmax, vertices[i].z);
	}

	float ax, ay, az, M;
	ax = (xmin + xmax) / 2;
	ay = (ymin + ymax) / 2;
	az = (zmin + zmax) / 2;
	M = std::max(xmax - xmin, std::max(ymax - ymin, zmax - zmin));

	for (int i = 0; i < (int)vertices.size(); i++) {
		vertices[i].x = (vertices[i].x - ax) * (2 / M);
		vertices[i].y = (vertices[i].y - ay) * (2 / M);
		vertices[i].z = (vertices[i].z - az) * (2 / M);
	}
}

