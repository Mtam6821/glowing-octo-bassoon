#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include "Camera.h"
#include "RTScene.h"
#include "Image.h"
#include "Ray.h"
#include "Intersection.h"

#ifndef __RAYTRACER_H__
#define __RAYTRACER_H__

namespace RayTracer {
	const static int MAX_DEPTH = 3;		//max recursion depth

	void Raytrace(Camera cam, RTScene scene, Image& image); //page 9
	Ray RayThruPixel(Camera cam, int i, int j, int width, int height);//page 10, 18
	Intersection& Intersect(Ray ray, Triangle triangle); //page 30, 33
	Intersection& Intersect(Ray ray, RTScene scene); //page 11, 28, 31
	glm::vec3 FindColor(Intersection hit, RTScene& scene, int recursion_depth); //page 15
};

#endif 


