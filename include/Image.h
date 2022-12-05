#include <glm/glm.hpp>

#ifndef __IMAGE_H__
#define __IMAGE_H__

class Image {
private:
	unsigned int fbo; // framebuffer object
	unsigned int tbo; // texture buffer object

public:
	int width;
	int height;
	std::vector<glm::vec3> pixels; // RGB colors;

	Image(int width, int height) {
		Image::width = width;
		Image::height = height;
	}

	void Image::init() {
		int size = width * height;
		for (int ind = 0; ind < size; ind++) {
			pixels.push_back(glm::vec3(0.0f));
		}
		glGenFramebuffers(1, &fbo);
		glGenTextures(1, &tbo);
	}

	void Image::draw() {
		glBindTexture(GL_TEXTURE_2D, tbo);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, // load texture
			0, GL_RGB, GL_FLOAT, &pixels[0][0]);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
		glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_2D, tbo, 0);// attach texture and the read frame
		glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT,
			GL_NEAREST); // copy framebuffer from read to write
	}
};

#endif 