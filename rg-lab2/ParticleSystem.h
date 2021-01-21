#ifndef PS_H
#define PS_H

#include "Particle.h"
#include <glm/glm.hpp>

class ParticleSystem
{
private:
	double frequencyOverflow = 0;
	Particle* particles;

	const glm::vec3 gravity = glm::vec3(0.0f, -9.81f, 0.0f);

	void ResetParticle(Particle* p);
	glm::vec3 GetRandomVelocity();

public:
	unsigned int maxParticles = 1000;
	unsigned int particlesPerSecond = 100;
	double time = 1;
	glm::vec3 position{ 0,0,0 };
	glm::vec3 offset{ 0,0,0 };
	glm::vec3 direction{ 0,1,0 };
	double magnitude = 1;

	glm::vec2 startSize{ 1,1 };
	glm::vec2 endSize{ 0,0 };
	glm::vec4 startColor{ 0,0,0,0 };
	glm::vec4 endColor{ 0,0,0,0 };

	bool useGravity;

	ParticleSystem(unsigned int maxParticles, unsigned int particlesPerSecond, double time, 
		glm::vec3 position, glm::vec3 offset,
		glm::vec3 direction, double magnitude, 
		glm::vec2 startSize, glm::vec2 endSize, 
		glm::vec4 startColor, glm::vec4 endColor, 
		bool useGravity = true);
	~ParticleSystem();

	void Update(double deltaTime);
	void Render(glm::vec3 cameraPosition, glm::vec3 cameraRight, glm::vec3 cameraUp);
};

#endif // !PS_H
