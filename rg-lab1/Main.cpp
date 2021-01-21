#include "Vector3.h"
#include "Mesh.h"
#include "Bspline.h"
#include "Render.h"

#include <stdlib.h>
#include <glut.h>
#include <iostream>

void display();
void reshape(int w, int h);
void keyboard(unsigned char key, int mouseX, int mouseY);
void animate(int value);

void printRotationTip();

Vector3 O = Vector3(60, 5, 10);
Vector3 G = Vector3(5, 5, 30);

Bspline spline = Bspline();
Mesh mesh = Mesh();

double speed = 0.001f;
double currentT = 0.0f;

unsigned int deltaTimeMilis = 5;

bool useDCM = true;

int main(int argc, char* argv[]) {

	//bspline
	std::string splinePath = "../spiral.obj";
	spline.loadFromFile(splinePath);
	spline.calculatePoints();

	//3d object to display
	std::string objPath = "../f16.obj";
	mesh.loadFromFile(objPath);
	mesh.normalize();

	std::cout << "Press R to switch between DCM and axis-angle rotation" << "\n";
	printRotationTip();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Bspline");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutTimerFunc(deltaTimeMilis, animate, 0);
	glutMainLoop();

	return 0;
}

void display()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); //background color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //refresh buffers

	//render stuff
	Render::DrawSpline(&spline);
	Render::DrawMeshOnSpline(&mesh, &spline, currentT, useDCM);

	glutSwapBuffers();
}

void reshape(int w, int h) {
	glViewport(0, 0, w, h);	//window area

	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glMatrixMode(GL_PROJECTION); //activate projection matrix
	glLoadIdentity();
	gluPerspective(45.0, (float)w / h, 0.5, 80.0); //view angle, x/y, front and back plane
	glMatrixMode(GL_MODELVIEW); //activate modelview matrix
	glLoadIdentity();
	gluLookAt(O.x, O.y, O.z, G.x, G.y, G.z, 0, 1, 0);	//O(x,y,z), G(x,y,z) and up vector(x,y,z)
}

void keyboard(unsigned char key, int mouseX, int mouseY)
{
	switch (key)
	{
	case 'r':
		useDCM = !useDCM;

		printRotationTip();

		break;
	}
}

void animate(int value) {

	currentT += speed;

	if (currentT >= 1.0f || currentT <= 0.0f) {
		speed *= -1;
	}

	glutPostRedisplay();
	glutTimerFunc(deltaTimeMilis, animate, 0);
}

void printRotationTip() {
	if (useDCM) {
		std::cout << "Using DCM for rotation" << "\n";
	}
	else {
		std::cout << "Using axis-angle for rotation" << "\n";
	}
}