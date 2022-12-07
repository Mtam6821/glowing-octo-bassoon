#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#ifndef __INTERSECTION_H__
#define __INTERSECTION_H__

class Intersection {
public:
	glm::vec3 P; // position of the intersection
	glm::vec3 N; // surface normal
	glm::vec3 V; // direction to incoming ray
	Triangle* triangle = nullptr; // pointer to geometric primitive (and material info)
	float dist = INFINITY; // distance to the source of ray
};
#endif 