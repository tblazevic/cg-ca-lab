#include "ParticleSystem.h"
#include <glut.h>
#include <algorithm>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>

ParticleSystem::ParticleSystem(unsigned int maxParticles, unsigned int particlesPerSecond, double time,
	glm::vec3 position, glm::vec3 offset,
	glm::vec3 direction, double magnitude,
	glm::vec2 startSize, glm::vec2 endSize,
	glm::vec4 startColor, glm::vec4 endColor,
	bool useGravity)
{
	this->maxParticles = maxParticles;
	this->useGravity = useGravity;

	this->particlesPerSecond = particlesPerSecond;
	this->frequencyOverflow = 0;

	this->time = time;
	this->position = position;
	this->offset = offset;
	this->direction = glm::normalize(direction);
	this->magnitude = magnitude;
	this->startSize = startSize;
	this->endSize = endSize;
	this->startColor = startColor;
	this->endColor = endColor;

	particles = (Particle*)malloc(sizeof(Particle) * maxParticles);
	if (particles == NULL) {
		exit(1);
	}

	srand(std::time(NULL));
}

ParticleSystem::~ParticleSystem()
{
	free(particles);
}

void ParticleSystem::Update(double deltaTime)
{
	const float localDt = (float)deltaTime;

	for (size_t i = 0; i < maxParticles; i++) {
		//decrease lifetime
		particles[i].lifeTime -= deltaTime;

		if (particles[i].lifeTime <= 0) {
			particles[i].isAlive = false;
			continue;
		}
		//update velocity - add gravity
		if (useGravity) {
			particles[i].velocity += localDt * gravity;
		}
		//update position
		particles[i].position += localDt * particles[i].velocity;

		//update color
		double t = particles[i].lifeTime / time;
		particles[i].color = glm::mix(endColor, startColor, t);

		//update size
		particles[i].size = glm::mix(endSize, startSize, t);
	}
	//instantiate next batch
	double targetParticleCount = particlesPerSecond * deltaTime;
	double newParticleCount = targetParticleCount + frequencyOverflow;
	if (newParticleCount < 1) {
		frequencyOverflow = newParticleCount;
		return;
	}
	for (size_t i = 0; i < maxParticles; i++) {
		if (!particles[i].isAlive) {
			ResetParticle(&particles[i]);
			newParticleCount--;
			if (newParticleCount < 1) {
				frequencyOverflow = newParticleCount;
				break;
			}
		}
	}
}

void ParticleSystem::Render(glm::vec3 cameraPosition, glm::vec3 cameraRight, glm::vec3 cameraUp)
{
	//calculate distance from camera
	for (size_t i = 0; i < maxParticles; i++) {
		if (particles[i].isAlive) {
			particles[i].cameraDistance = glm::length(cameraPosition - particles[i].position);
		}
	}
	//(reverse) sort particles by distance from camera for correct transparency
	std::sort(particles, &particles[maxParticles]);

	//draw all particles
	for (size_t i = 0; i < maxParticles; i++) {
		if (!particles[i].isAlive) {
			continue;
		}

		//calculate billboard parameters
		glm::vec3 width = cameraRight * particles[i].size.x / 2.0f;
		glm::vec3 height = -cameraUp * particles[i].size.y / 2.0f;		//is upside down without the minus

		//set tint color
		glm::vec4 color = particles[i].color;
		glColor4f(color.x, color.y, color.z, color.w);
		//draw quad
		glEnable(GL_TEXTURE_2D);
		glBegin(GL_QUADS);
		{
			glTexCoord2f(0.0, 0.0);
			glVertex3fv(glm::value_ptr(particles[i].position - width - height));
			glTexCoord2f(1.0, 0.0);
			glVertex3fv(glm::value_ptr(particles[i].position + width - height));
			glTexCoord2f(1.0, 1.0);
			glVertex3fv(glm::value_ptr(particles[i].position + width + height));
			glTexCoord2f(0.0, 1.0);
			glVertex3fv(glm::value_ptr(particles[i].position - width + height));
		}
		glEnd();
	}
}

void ParticleSystem::ResetParticle(Particle* p)
{
	p->isAlive = true;
	p->lifeTime = time;
	p->position = position + offset;
	p->velocity = GetRandomVelocity();
	p->size = startSize;
	p->color = startColor;
}

glm::vec3 ParticleSystem::GetRandomVelocity()
{
	float t1 = ((float)rand()) / RAND_MAX;
	float t2 = ((float)rand()) / RAND_MAX;
	float t3 = ((float)rand()) / RAND_MAX;

	float xt = -1 + t1 * 2;
	float zt = -1 + t2 * 2;

	glm::vec3 newDirection = glm::normalize(direction + 0.35f * t3 * glm::normalize(glm::vec3{ xt,0,zt }));

	return (float)magnitude * newDirection;
}
