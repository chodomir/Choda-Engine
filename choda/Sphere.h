#pragma once

#include <glad/glad.h>
#include <vector>

namespace choda {
	
	class Sphere {
	public:
		Sphere(float radius, int stackCount, int sectorCount);
		Sphere();
		~Sphere();

		void init();
		void draw();
	private:
		std::vector<float> vertices;
		std::vector<unsigned int> indices;
		int stackCount, sectorCount;
		float radius;
		GLuint vao, vbo, ebo;
	};
}


