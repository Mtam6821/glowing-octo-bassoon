
#include "RayTracer.h"

inline float random_float() {
	// Returns a random real in [0,1).
	return rand() / (RAND_MAX + 1.0);
}

inline float random_float(float min, float max) {
	// Returns a random real in [min,max).
	return min + (max - min) * random_float();
}

inline static glm::vec3 random_vec(float min, float max) {
	return glm::vec3(random_float(min, max), random_float(min, max), random_float(min, max));
}


void RayTracer::Raytrace(Camera cam, RTScene& scene, Image& image) {
	std::cout << cam.eye[0] << ' ' << cam.eye[1] << ' ' << cam.eye[2] << std::endl;
	int w = image.width; int h = image.height;
	for (int j = 0; j < h; j++) {
		for (int i = 0; i < w; i++) {	

			/*			glm::vec3 color = glm::vec3(0.0f);
			for (int k = 0; k < 10; k++) {		//sampling
				Ray ray = RayThruPixel(cam, i, j, w, h);
				Intersection hit = Intersect(ray, scene);
				color += FindColor(hit, scene, 1);
			}
			image.pixels[j * w + i] = (float)0.1 * color;*/
			
				
			Ray ray = RayThruPixel(cam, i, j, w, h);
			Intersection hit = Intersect(ray, scene);
			glm::vec3 color = FindColor(hit, scene, 1);
			
			image.pixels[j * w + i] = (color == glm::vec3(0.0f)) ? glm::vec3(0.1f,0.2f,0.3f) : color;

			
		}
		std::cout << "row " << j << " out of " << h << std::endl;
	}
}//page 9

Ray RayTracer::RayThruPixel(Camera cam, int i, int j, int width, int height) {
	Ray ray;
	ray.p0 = cam.eye;
	float i_ = (float)i;
	float j_ = (float)j;
	float width_ = (float)width;
	float height_ = (float)height;

	float alpha = 2 * ((i_ + 0.5) / width_) - 1;
	float beta = 1 - 2 * ((j_ + 0.5) / height_);

	//float alpha = 2 * ((i_ + random_float()) / width_) - 1;
	//float beta = 1 - 2 * ((j_ + random_float()) / height_);

	float a = width_ / height_;

	glm::vec3 w = glm::normalize(cam.eye - cam.target);
	glm::vec3 u = glm::cross(cam.up, w);
	glm::vec3 v = glm::cross(w, u);

	float angle = glm::tan(cam.fovy / 2);

	ray.dir = glm::normalize((alpha * a * angle * u) + (beta * angle * v) - w);

	return ray;
}//page 10, 18
Intersection& RayTracer::Intersect(Ray ray, Triangle& triangle) {
	glm::mat4 tri_mat = glm::mat4(
		glm::vec4(triangle.P[0], 1.0f),
		glm::vec4(triangle.P[1], 1.0f),
		glm::vec4(triangle.P[2], 1.0f),
		glm::vec4(-ray.dir, 0.0f)
	);
	glm::vec4 ray_pos = glm::vec4(ray.p0, 1.0f);
	glm::vec4 bary_vec = glm::inverse(tri_mat) * ray_pos;

	Intersection ret;

	//return no hit
	for (int ind = 0; ind < 4; ind++) {
		if (bary_vec[ind] < 0) {
			ret.dist = INFINITY;
			return ret;
		}
	}

	ret.P = glm::vec3(bary_vec.x * triangle.P[0] + bary_vec.y * triangle.P[1] + bary_vec.z * triangle.P[2]);
	ret.N = glm::vec3(bary_vec.x * triangle.N[0] + bary_vec.y * triangle.N[1] + bary_vec.z * triangle.N[2]);
	ret.V = -ray.dir;		
	ret.triangle = &triangle;
	ret.dist = bary_vec[3];

	return ret;
} //page 30, 33

Intersection& RayTracer::Intersect(Ray ray, RTScene& scene) {
	float mindist = INFINITY;
	Intersection hit;  	
	for (int ind = 0; ind < scene.triangle_soup.size(); ind++) { // Find closest intersection; test all objects
		Triangle& t = scene.triangle_soup[ind];
		Intersection& hit_temp = Intersect(ray, t);

		if (hit_temp.dist != INFINITY) { assert(hit_temp.triangle != nullptr); }
		else { assert(hit_temp.triangle == nullptr); }

		if (hit_temp.dist < mindist) { // closer than previous hit
			mindist = hit_temp.dist;
			hit = hit_temp;
		}
	}
	return hit;
} //page 11, 28, 31

glm::vec3 RayTracer::FindColor(Intersection hit, RTScene &scene, int recursion_depth) {
	assert(recursion_depth <= MAX_DEPTH);
	glm::vec3 color = glm::vec3(0.0f);     //sum of the color values for each light source

	if (hit.dist < INFINITY) {
		glm::vec3 v_vec = glm::normalize(hit.V);					//incoming ray
		glm::vec3 n_vec = glm::normalize(hit.N);					//hit normal

		for (std::pair<std::string, Light*> entry : scene.light) {
			//get necessary points for calculating v,n,l
			std::cout << "here" << std::endl;
			glm::vec3 p_pos= hit.P;									 //hit position
			glm::vec3 q_pos = glm::vec3((entry.second)->position);   //lightpos
			glm::vec3 l_vec = glm::normalize(q_pos - p_pos);		 //vec to light from hitpos

			//check for intersection between light and triangle 
			//i.e. shadow
			Ray light_ray;			//new ray
			light_ray.p0 = hit.P;
			light_ray.dir = l_vec;
			Intersection light_hit = Intersect(light_ray, scene);

			glm::vec3 tempColor = glm::vec3(0.0f);
			if (!(light_hit.dist < glm::length(q_pos - light_ray.p0))) {
				//add diffuse
				std::cout << "diffuse" << std::endl; 
				tempColor += glm::vec3(hit.triangle->material->diffuse) * glm::max(0.0f, dot(l_vec, n_vec));
				color += glm::vec3((entry.second)->color) * tempColor;
			}

			//recursive specular
			if (recursion_depth < MAX_DEPTH) {
				std::cout << "spec " << recursion_depth << std::endl;

				Ray ray2;	//specular refl ray
				ray2.dir = (2 * glm::dot(n_vec, v_vec) * n_vec) - v_vec;
				//glm::vec3 jitter = (hit.N * 0.1f);
				ray2.p0 = hit.P;
				Intersection& hit2 = Intersect(ray2, scene);

				if (hit2.dist < INFINITY) {
					glm::vec3 specular = glm::vec3(hit2.triangle->material->specular);
					color += specular * FindColor(hit2, scene, ++recursion_depth);
				}
			}
		}
		color += glm::vec3(hit.triangle->material->emision);
	}
	return color;

} //page 15


