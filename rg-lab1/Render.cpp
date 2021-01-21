#include "Render.h"

#include <glut.h>
#include <math.h>
#include <glm/gtc/type_ptr.hpp>

static const double tangentScale = 1.0f;

void Render::DrawMeshOnSplineDCM(Mesh* mesh, Bspline* spline, double t)
{
	Vector3 position = spline->getPositionAtT(t);
	Vector3 tangent = spline->getTangentAtT(t);
	Vector3 secondDerivative = spline->getSecondDerivativeAtT(t);

	glm::vec3 z = tangent.asGLMVector();
	glm::vec3 y = secondDerivative.asGLMVector();
	double eps = 1e-12;
	//check if second derivative is zero
	if (std::abs(y.x - eps) < eps && std::abs(y.y - eps) < eps && std::abs(y.z - eps) < eps) {
		y = glm::vec3(0.0f, 0.0f, 1.0f);
	}
	glm::vec3 x = glm::cross(y, z); //normal
	//glm::vec3 v = glm::cross(x, z); //binormal
	glm::mat3x3 dcm = glm::mat3x3();
	dcm[0] = glm::normalize(x);
	dcm[1] = glm::normalize(y);
	dcm[2] = glm::normalize(z);
	glm::mat3x3 Idcm = dcm; //glm::inverse(dcm);

	glPushMatrix();
	//translate
	glTranslatef(position.x, position.y, position.z);

	//draw tangent
	glLineWidth(1.0f);
	glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(tangent.x * tangentScale, tangent.y * tangentScale, tangent.z * tangentScale);
	glEnd();

	//glPushMatrix();
	//glLoadMatrixf(glm::value_ptr(Idcm));
	//draw mesh
	glPushMatrix();
	DrawMesh(mesh, Idcm);
	glPopMatrix();

	//glPopMatrix();
	glPopMatrix();
}

void Render::DrawMeshOnSplineSE(Mesh* mesh, Bspline* spline, double t)
{
	Vector3 position = spline->getPositionAtT(t);
	Vector3 tangent = spline->getTangentAtT(t);

	glPushMatrix();
	//translate
	glTranslatef(position.x, position.y, position.z);

	//draw tangent
	glLineWidth(1.0f);
	glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(tangent.x * tangentScale, tangent.y * tangentScale, tangent.z * tangentScale);
	glEnd();

	//rotate
	glm::vec3 s = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 e = glm::normalize(tangent.asGLMVector());
	double angle = glm::dot(s, e); //cosine
	angle = std::acos(angle); //radians
	angle *= 180.0f / 3.141592653589793238463f; //degrees
	glm::vec3 axis = glm::cross(s, e); 
	glPushMatrix();
	glRotatef(angle, axis.x, axis.y, axis.z);
	//draw mesh
	glPushMatrix();
	DrawMesh(mesh);
	glPopMatrix();

	glPopMatrix();
	glPopMatrix();

}

void Render::DrawMesh(Mesh* mesh, glm::mat3x3 rotation)
{
	glBegin(GL_TRIANGLES);
	glColor3f(0.0f, 0.0f, 1.0f);
	for (int i = 0; i < (int)mesh->triangles.size(); i += 3) {
		glm::vec3 v1 = mesh->vertices[mesh->triangles[i]].asGLMVector();
		glm::vec3 v2 = mesh->vertices[mesh->triangles[i + 1]].asGLMVector();
		glm::vec3 v3 = mesh->vertices[mesh->triangles[i + 2]].asGLMVector();
		
		v1 = rotation * v1;
		v2 = rotation * v2;
		v3 = rotation * v3;
		
		glVertex3f(v1.x, v1.y, v1.z);
		glVertex3f(v2.x, v2.y, v2.z);
		glVertex3f(v3.x, v3.y, v3.z);
	}
	glEnd();
}

void Render::DrawMesh(Mesh* mesh) 
{
	glBegin(GL_TRIANGLES);
	glColor3f(0.0f, 0.0f, 1.0f);
	for (int i = 0; i < (int)mesh->triangles.size(); i += 3) {
		Vector3 v1 = mesh->vertices[mesh->triangles[i]];
		Vector3 v2 = mesh->vertices[mesh->triangles[i + 1]];
		Vector3 v3 = mesh->vertices[mesh->triangles[i + 2]];

		glVertex3f(v1.x, v1.y, v1.z);
		glVertex3f(v2.x, v2.y, v2.z);
		glVertex3f(v3.x, v3.y, v3.z);
	}
	glEnd();
}

void Render::DrawSpline(Bspline* spline)
{
	glLineWidth(1.0f);
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINE_STRIP);
	for (unsigned int i = 0; i < spline->points.size(); i++) {
		glVertex3f(spline->points[i].x, spline->points[i].y, spline->points[i].z);
	}
	glEnd();

	glColor3f(0.0f, 1.0f, 0.0f);
	for (unsigned int i = 0; i < spline->controlPoints.size(); i++) {
		glPushMatrix();
		glTranslatef(spline->controlPoints[i].x, spline->controlPoints[i].y, spline->controlPoints[i].z);
		glutSolidCube(0.2f);
		glPopMatrix();
	}
}

void Render::DrawMeshOnSpline(Mesh* mesh, Bspline* spline, double t, bool useDCM)
{
	if (useDCM) {
		DrawMeshOnSplineDCM(mesh, spline, t);
	}
	else {
		DrawMeshOnSplineSE(mesh, spline, t);
	}
}
