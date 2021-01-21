#ifndef BSPLINE_H
#define BSPLINE_H

#include "Vector3.h"

#include <string>
#include <vector>

class Bspline
{
private:
	int controlPointCount = 0;
	int pointsPerSegment = 100;
	int getIndexAtT(double t);

public:
	std::vector<Vector3> controlPoints;
	std::vector<Vector3> points;
	std::vector<Vector3> tangents;
	std::vector<Vector3> normals;
	std::vector<Vector3> binormals;
	std::vector<Vector3> secondDerivatives;

	Bspline();
	void loadFromFile(std::string filePath);
	void calculatePoints();
	Vector3 getPositionAtT(double t);
	Vector3 getTangentAtT(double t);
	Vector3 getSecondDerivativeAtT(double t);
};

#endif 