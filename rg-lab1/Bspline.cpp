#include "Bspline.h"

#include <glm/glm.hpp>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

Bspline::Bspline()
{
	controlPoints = std::vector<Vector3>();
	points = std::vector<Vector3>();
}

void Bspline::loadFromFile(std::string filePath)
{
	std::ifstream stream;
	std::string line;
	stream.open(filePath);

	while (std::getline(stream, line)) {
		if (line[0] == 'v') {
			controlPointCount++;
		}
		else if (line[0] == 's') {
			break;
		}
	}

	controlPoints.resize(controlPointCount);

	stream.close();

	stream.open(filePath);
	int vi, ti;
	vi = ti = 0;
	while (std::getline(stream, line)) {
		if (line[0] == 'v') {
			std::string buffer;
			buffer = line.substr(2, line.size() - 2);
			std::stringstream ss(buffer);
			ss >> controlPoints[vi].x;
			ss >> controlPoints[vi].y;
			ss >> controlPoints[vi].z;
			vi++;
		}
		else if (line[0] == 's') {
			break;
		}
	}
	stream.close();

	std::cout << "Bspline loaded from file: " << filePath << "\n";
	std::cout << "Control point count: " << controlPointCount << "\n";

	/*for (int i = 0; i < controlPointCount; i++) {
		std::cout << controlPoints[i].x << " " << controlPoints[i].y << " " << controlPoints[i].z << " " << "\n";
	}*/
}

void Bspline::calculatePoints()
{
	for (int s = 1; s < controlPointCount - 2; s++) {
		for (int p = 0; p <= pointsPerSegment; p++) {
			double t = (double)p / pointsPerSegment;
			
			glm::vec3 r0, r1, r2, r3;
			r0 = controlPoints[s - 1].asGLMVector();
			r1 = controlPoints[s + 0].asGLMVector();
			r2 = controlPoints[s + 1].asGLMVector();
			r3 = controlPoints[s + 2].asGLMVector();
			//R matrix
			glm::mat4x3 RMatrix = glm::mat4x3(r0, r1, r2, r3);
			//t vector
			glm::vec4 vt = glm::vec4(t * t * t, t * t, t, 1);
			vt *= 1 / 6.0f;
			//t derivative vector
			glm::vec3 vdt = glm::vec3(t * t, t, 1);
			vdt *= 1 / 2.0f;
			//t dd vector
			glm::vec2 vddt = glm::vec2(t, 1);

			//B matrix
			glm::mat4x4 BMatrix =
				glm::mat4x4(
					-1, 3, -3, 1,
					3, -6, 3, 0,
					-3, 0, 3, 0,
					1, 4, 1, 0);
			//B d matrix
			glm::mat3x4 BdMatrix =
				glm::mat3x4(
					-1, 3, -3, 1,
					2, -4, 2, 0,
					-1, 0, 1, 0);
			//B dd matrix
			glm::mat2x4 Bddmatrix =
				glm::mat2x4(
					-1, 3, -3, 1,
					1, -2, 1, 0);

			//point
			glm::vec3 point = RMatrix * BMatrix * vt;
			points.push_back(Vector3(point.x, point.y, point.z));
			//first derivative
			point = RMatrix * BdMatrix * vdt;
			tangents.push_back(Vector3(point.x, point.y, point.z));
			//second derivative
			point = RMatrix * Bddmatrix * vddt;
			secondDerivatives.push_back(Vector3(point.x, point.y, point.z));
		}
	}

	std::cout << "Points on spline sampled, total points: " << points.size() << "\n";

	/*for (unsigned int i = 0; i < points.size(); i++) {
		std::cout << points[i].x << " " << points[i].y << " " << points[i].z << " " << "\n";
	}*/
}

int Bspline::getIndexAtT(double t) {
	double step = 1.0f / points.size();
	int index = (int)(t / step);
	index = std::max(0, std::min(index, (int)points.size() - 1));
	double remainder = t;
	while (remainder >= step) {
		remainder -= step;
	}
	//std::cout << "Step: " << step << " T: " << t << " Index: " << index << " remainder: " << remainder << "\n";
	return index;
}

Vector3 Bspline::getPositionAtT(double t)
{
	int index = getIndexAtT(t);
	return points[index];
}

Vector3 Bspline::getTangentAtT(double t)
{
	int index = getIndexAtT(t);
	return tangents[index];
}

Vector3 Bspline::getSecondDerivativeAtT(double t)
{
	int index = getIndexAtT(t);
	return secondDerivatives[index];
}
