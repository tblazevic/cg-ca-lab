#ifndef RENDER_H
#define RENDER_H

#include "Mesh.h"
#include "Vector3.h"
#include "Bspline.h"

#include <vector>

class Render
{
private:
	static void DrawMeshOnSplineDCM(Mesh* mesh, Bspline* spline, double t);
	static void DrawMeshOnSplineSE(Mesh* mesh, Bspline* spline, double t);
	static void DrawMesh(Mesh* mesh, glm::mat3x3 rotation);

public:
	static void DrawSpline(Bspline* spline);
	static void DrawMeshOnSpline(Mesh* mesh, Bspline* spline, double t, bool useDCM);
	static void DrawMesh(Mesh* mesh);
};

#endif