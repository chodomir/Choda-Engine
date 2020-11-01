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
		std::vector<float> normals;
		std::vector<float> texCoords;
		GLuint vao, vbo, ebo;
	};

}


