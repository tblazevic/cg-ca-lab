#ifndef PARTICLE_H
#define PARTICLE_H

#include <glm/glm.hpp>

class Particle
{
private:

public:
	bool isAlive = false;
	double lifeTime;
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec4 color;
	glm::vec2 size;
	float cameraDistance;

	bool operator<(Particle& that) const
	{
		return this->cameraDistance > that.cameraDistance; //back to front first
	}
};

#endif // !PARTICLE_H
