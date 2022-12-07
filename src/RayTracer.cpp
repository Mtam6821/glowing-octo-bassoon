
#define GLM_SWIZZLE
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
	int w = image.width; int h = image.height;
	for (int j = 0; j < h; j++) {
		for (int i = 0; i < w; i++) {		
			glm::vec3 color = glm::vec3(0.0f);
			for (int k = 0; k < 10; k++) {		//sampling
				Ray ray = RayThruPixel(cam, i, j, w, h);
				Intersection hit = Intersect(ray, scene);
				color += FindColor(hit, scene, 1);
			}
			image.pixels[j * w + i] = (float)0.1 * color;
			
		}
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

	float a = width_ / height_;

	glm::vec3 w = glm::normalize(cam.eye - cam.target);
	glm::vec3 u = glm::cross(cam.up, w);
	glm::vec3 v = glm::cross(w, u);

	float angle = glm::tan(cam.fovy / 2);

	ray.dir = glm::normalize((alpha * a * angle * u) + (beta * angle * v) - w);

	return ray;
}//page 10, 18
Intersection& RayTracer::Intersect(Ray ray, Triangle triangle) {
	glm::mat4 tri_mat = glm::mat4(
		glm::vec4(triangle.P[0], 1.0f),
		glm::vec4(triangle.P[1], 1.0f),
		glm::vec4(triangle.P[2], 1.0f),
		glm::vec4(-ray.dir, 0.0f)
	);
	glm::vec4 ray_vec = glm::vec4(ray.dir, 1.0f);
	glm::vec4 bary_vec = glm::inverse(tri_mat) * ray_vec;

	Intersection ret;

	//return no hit
	for (int ind = 0; ind < 4; ind++) {
		if (bary_vec[ind] < 0) {
			ret.dist = INFINITY;
			return ret;
		}
	}

	Triangle* tri = new Triangle(triangle);

	ret.P = glm::vec3(bary_vec.x * triangle.P[0] + bary_vec.y * triangle.P[1] + bary_vec.z * triangle.P[2]);
	ret.N = glm::vec3(bary_vec.x * triangle.N[0] + bary_vec.y * triangle.N[1] + bary_vec.z * triangle.N[2]);
	ret.V = ray.dir;
	ret.triangle = tri;
	ret.dist = bary_vec[3];

	return ret;
} //page 30, 33

Intersection& RayTracer::Intersect(Ray ray, RTScene& scene) {
	float mindist = INFINITY;
	Intersection hit;  	
	for (int ind = 0; ind < scene.triangle_soup.size(); ind++) { // Find closest intersection; test all objects
		Triangle t = scene.triangle_soup[ind];
		Intersection& hit_temp = Intersect(ray, t);

		if (hit_temp.dist < mindist) { // closer than previous hit
			mindist = hit_temp.dist;
			hit = hit_temp;
		}
	}
	return hit;
} //page 11, 28, 31

glm::vec3 RayTracer::FindColor(Intersection hit, RTScene &scene, int recursion_depth) {
	
	glm::vec4 color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);     //sum of the color values for each light source

	glm::vec3 v_cam = glm::normalize(hit.V);
	glm::vec3 n_cam = glm::normalize(hit.N);
	
	if (hit.dist < INFINITY) {
		if (recursion_depth == MAX_DEPTH) {			
			Ray ray2;
			ray2.dir = 2 * glm::dot(n_cam, v_cam) * n_cam - v_cam;
			ray2.p0 = hit.P;

			Intersection hit2 = Intersect(ray2, scene);
			int next = recursion_depth++;

			glm::vec3 specular = hit.triangle->material->specular.xyz();
			color += glm::vec4(specular * FindColor(hit2, scene, next), 1.0f);
		}

		else {
			for (std::pair<std::string, Light*> entry : scene.light) {
				//get necessary points for calculating v,n,l

				glm::vec4 p_cam = glm::vec4(hit.P, 1.0f);     //position in camera coords
				glm::vec4 q_cam = (entry.second)->position;   //lightpos in camera coords

				glm::vec3 l_cam = glm::normalize((p_cam.w * q_cam.xyz()) - (q_cam.w * p_cam.xyz()));
				glm::vec3 h_cam = glm::normalize(v_cam + l_cam);

				//check for intersection between q_cam and the triangle (i.e. is hit.dist the same Intersect(l_ray, scene).dist) 
				Ray light_ray;
				light_ray.p0 = hit.P;
				light_ray.dir = glm::normalize(glm::vec3(q_cam) - light_ray.p0);

				Intersection light_hit = Intersect(light_ray, scene);
				if (light_hit.dist < glm::length(glm::vec3(q_cam) - light_ray.p0)) { continue; }

				//add ambient, diffuse, and specular
				glm::vec4 tempColor = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
				//tempColor += hit.triangle->material->ambient;
				tempColor += hit.triangle->material->diffuse * glm::max(0.0f, dot(l_cam, n_cam));
				//tempColor += hit.triangle->material->specular * pow(glm::max(0.0f, dot(n_cam, h_cam)), hit.triangle->material->shininess);

				//multiply by light color and add to sumColor
				color += (entry.second)->color * tempColor;
			}
		}
	}
	
	//color += hit.triangle->material->emision;
	
	return color;

} //page 15


