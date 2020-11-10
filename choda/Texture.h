#pragma once

#include <string>

namespace choda {

	class Texture
	{

	public:
		Texture();
		Texture(const char* imgPath, const char* type);

	public:
		static unsigned int GenerateFromFile(const char* path);

	public:
		unsigned int id;
		std::string type;
		std::string path;
	};

}


