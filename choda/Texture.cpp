#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glad/glad.h>
#include <iostream>
#include <string>

using namespace choda;

Texture::Texture() : id(0), path(""), type("")
{
}

Texture::Texture(const char* imgPath, const char* type) : id(0), path(imgPath), type(type)
{
}

unsigned int choda::Texture::GenerateFromFile(const char* path)
{
	unsigned int texId;
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


	int width, height, nChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* img = stbi_load(path, &width, &height, &nChannels, 0);
	if (img) {
		GLenum format;
		if (nChannels == 1) format = GL_RED;
		else if (nChannels == 3) format = GL_RGB;
		else if (nChannels == 4) format = GL_RGBA;

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format, GL_UNSIGNED_BYTE, img);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		throw std::string("Failed to load image ", path);
	stbi_image_free(img);

	return texId;
}