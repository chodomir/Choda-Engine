#pragma once

#include <glad/glad.h>
#include <vector>

#include "ShaderProgram.h"

namespace choda {

	class Mesh {
	public:
		Mesh(const std::vector<float> &vertices, const std::vector<float> &normals, const std::vector<float> &texCoords, const std::vector<unsigned int> &indices);
		Mesh();
		~Mesh();

		void init();
		void draw();
	protected:
		std::vector<float> vertices;
		std::vector<float> normals;
		std::vector<float> texCoords;
		std::vector<unsigned int> indices;
		GLuint vao, vbo, ebo;
	};

}


