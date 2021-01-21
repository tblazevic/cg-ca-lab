#include <iostream>
#include <glut.h>
#include <cstring>
#include <vector>
#include <chrono>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "ParticleSystem.h"

void display();
void reshape(int w, int h);
void keyboard(unsigned char key, int mouseX, int mouseY);
void idle();

glm::vec3 eye = { 0,5,-20 };
glm::vec3 lookAt = { 0,5,0 };
glm::vec3 viewUp = { 0,1,0 };

glm::mat4 viewMatrix = glm::lookAt(eye, lookAt, viewUp);
glm::vec3 cameraRight = { viewMatrix[0][0], viewMatrix[1][0], viewMatrix[2][0] };
glm::vec3 cameraUp = { viewMatrix[0][1], viewMatrix[1][1], viewMatrix[2][1] };

std::vector<ParticleSystem*> systems;

GLuint textureId[] = { 0, 0, 0 };
GLuint tex_2d;

bool paused = false;
double currentTime = 0;

int main() {
	std::cout << "Press P to pause/unpause\n";
	//textures
	const char* t1 = "../textures/snow.bmp";
	const char* t2 = "../textures/balloon.bmp";

	//load data
	int width1, height1, channels1;
	unsigned char* data1 = stbi_load(t1, &width1, &height1, &channels1, 0);
	int width2, height2, channels2;
	unsigned char* data2 = stbi_load(t2, &width2, &height2, &channels2, 0);

	//setup particle system 1
	ParticleSystem ps1 = ParticleSystem(10000, 100, 5.0f, //max number of particles, frequency and duration
		glm::vec3{ -5,0,10 }, glm::vec3{ 0,0,0 }, //position and offset
		glm::vec3{ 0,1,0 }, 20, //velocity direction and magnitude
		glm::vec2{ 2,2 }, glm::vec2{ 0,0 }, //size
		glm::vec4{ 0,0,1,0 }, glm::vec4{ 0,1,1,0.8f }); //rgba color
	systems.push_back(&ps1);

	//setup particle system 2
	ParticleSystem ps2 = ParticleSystem(10000, 50, 3.0f, //max number of particles, frequency and duration
		glm::vec3{ 10,0,10 }, glm::vec3{ 0,0,0 }, //position and offset
		glm::vec3{ 0,1,0 }, 16, //velocity direction and magnitude
		glm::vec2{ 4,4 }, glm::vec2{ 1.5f,1.5f }, //size
		glm::vec4{ 1,0,0,0 }, glm::vec4{ 0,1,0,0.8f }); //rgba color
	systems.push_back(&ps2);

	//setup opengl
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1080, 720);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Particle systems");

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//setup opengl textures
	glEnable(GL_TEXTURE_2D);
	glGenTextures(2, textureId);
	//bind texture 1
	glBindTexture(GL_TEXTURE_2D, textureId[0]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width1, height1, 0, GL_RGB, GL_UNSIGNED_BYTE, data1);
	//bind texture 2
	glBindTexture(GL_TEXTURE_2D, textureId[1]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width2, height2, 0, GL_RGB, GL_UNSIGNED_BYTE, data2);

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);
	glutMainLoop();

	stbi_image_free(data1);
	stbi_image_free(data2);

	return 0;
}


void display()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); //background color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //refresh buffers

	//render stuff
	for (size_t i = 0; i < systems.size(); i++) {
		glBindTexture(GL_TEXTURE_2D, textureId[i % 2]);
		systems[i]->Render(eye, cameraRight, cameraUp);
	}

	glutSwapBuffers();
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);	//window area

	glMatrixMode(GL_PROJECTION); //activate projection matrix
	glLoadIdentity();
	gluPerspective(60.0, (float)w / h, 0.5, 100.0); //view angle, x/y, front and back plane
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(glm::value_ptr(viewMatrix));
}

void keyboard(unsigned char key, int mouseX, int mouseY)
{
	switch (key)
	{
	case 'p':
		paused = !paused;
		break;
	}
}

void idle()
{
	static bool first = true;
	static std::chrono::steady_clock::time_point timeLast;
	if (first) {
		timeLast = std::chrono::high_resolution_clock::now();
		first = false;
	}
	auto timeNow = std::chrono::high_resolution_clock::now();

	double deltaTimeMiliSeconds = std::chrono::duration<double, std::milli>(timeNow - timeLast).count();
	double deltaTimeSeconds = deltaTimeMiliSeconds / 1e3;

	timeLast = timeNow;

	if (paused) return;

	//create offset
	currentTime += deltaTimeSeconds;
	double sin = std::sin(currentTime);
	double cos = std::cos(currentTime);

	systems[0]->offset = (5.0f * glm::vec3{ cos,cos,sin });
	systems[1]->offset = (10.0f * glm::vec3{ 0,cos,0 });

	//update particle systems
	for (size_t i = 0; i < systems.size(); i++) {
		systems[i]->Update(deltaTimeSeconds);
	}

	glutPostRedisplay();
}