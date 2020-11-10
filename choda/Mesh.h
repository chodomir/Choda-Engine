#pragma once

#include <glad/glad.h>
#include <vector>

#include "ShaderProgram.h"
#include "Texture.h"

namespace choda {

	class Mesh {
	public:
		Mesh(const std::vector<float> &vertices, const std::vector<float> &normals, const std::vector<float> &texCoords, 
				const std::vector<unsigned int> &indices, const std::vector<Texture> &textures);
		Mesh();

		~Mesh();

		void init();
		void draw(ShaderProgram &shader);
	protected:
		std::vector<float> vertices;
		std::vector<float> normals;
		std::vector<float> texCoords;
		std::vector<unsigned int> indices;
		std::vector<Texture> textures;
		GLuint vao, vbo, ebo;
	};

}


