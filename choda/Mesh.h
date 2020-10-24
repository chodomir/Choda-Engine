#pragma once

#include <glad/glad.h>
#include <vector>

namespace choda {

	class Mesh {
	public:
		~Mesh();

		void init();
		void draw();
	protected:
		std::vector<float> vertices;
		std::vector<unsigned int> indices;
		GLuint vao, vbo, ebo;
	};

}


