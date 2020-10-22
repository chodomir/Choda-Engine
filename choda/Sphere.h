#pragma once

#include <glad/glad.h>
#include <vector>

namespace choda {
	
	class Sphere {
	public:
		Sphere(int stackCount, int sectorCount);
		~Sphere();

		void init();
		void draw();
	private:
		std::vector<float> vertices;
		std::vector<unsigned int> indices;
		int stackCount, sectorCount;
		GLuint vao, vbo, ebo;
	};
}


