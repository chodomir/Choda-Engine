#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glad/glad.h>
#include <iostream>

using namespace choda;

unsigned short Texture::TextureUnitCnt = 0;

Texture::Texture(const char* imgPath, bool hasAlpha) : textureUnit(Texture::TextureUnitCnt++)
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


	int nChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* img = stbi_load(imgPath, &width, &height, &nChannels, 0);
	if (img) {
		if (hasAlpha) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
		}
		else {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cerr << "Failed to load image " << imgPath << "\n";
	}
	stbi_image_free(img);
}

void Texture::activate() const
{
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_2D, texture);
}

int Texture::getWidth() const
{
	return width;
}

int Texture::getHeight() const
{
	return height;
}

Texture::~Texture()
{
	glDeleteTextures(1, &texture);
}
