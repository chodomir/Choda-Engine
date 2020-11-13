#include "Mesh.h"

choda::Mesh::Mesh(const std::vector<float>& vertices, const std::vector<float>& normals,
	const std::vector<float>& texCoords, const std::vector<unsigned int>& indices, const std::vector<Texture> &textures) 
	: vertices(vertices), normals(normals), texCoords(texCoords), indices(indices), textures(textures)
{
	init();
}

choda::Mesh::Mesh() 
{
}

choda::Mesh::~Mesh()
{
	for (int i = 0; i < textures.size(); i++)
		glDeleteTextures(1, &textures[i].id);

	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
	glDeleteVertexArrays(1, &vao);
}

void choda::Mesh::init()
{
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	glGenVertexArrays(1, &vao);

	// copying vertex data in one buffer
	std::vector<float> data;
	for (size_t i = 0; i < vertices.size(); i += 3) {
		// layout (location = 0)
		data.push_back(vertices[i]);
		data.push_back(vertices[i + 1]);
		data.push_back(vertices[i + 2]);

		// layout (location = 1)
		if (normals.size() > 0) {
			data.push_back(normals[i]);
			data.push_back(normals[i + 1]);
			data.push_back(normals[i + 2]);
		}

		// layout (location = 2)
		if (texCoords.size() > 0) {
			data.push_back(texCoords[i - i / 3]);
			data.push_back(texCoords[i - i / 3 + 1]);
		}
	}
	
	// loading vertex data
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
	if (indices.size() > 0) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	}

	// calculating stride and offset for glVertexAttribPointer
	int offset = 0;
	int stride = 3 * sizeof(float);
	if (normals.size() > 0) stride += 3 * sizeof(float);
	if (texCoords.size() > 0) stride += 2 * sizeof(float);

	// positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)(offset));
	glEnableVertexAttribArray(0);
	offset += 3 * sizeof(float);
	// normals
	if (normals.size() > 0) {
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(offset));
		glEnableVertexAttribArray(1);
		offset += 3 * sizeof(float);
	}
	if (texCoords.size() > 0) {
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(offset));
		glEnableVertexAttribArray(2);
		offset += 2 * sizeof(float);
	}

	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void choda::Mesh::draw(ShaderProgram& shader) {

	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	for (int i = 0; i < textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);

		// get the type of texture
		std::string number;
		if (textures[i].type == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (textures[i].type == "texture_specular")
			number = std::to_string(specularNr++);

		// set the texture in shader and bind it
		shader.setInt(("material." + textures[i].type + number).c_str(), i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}
	glActiveTexture(GL_TEXTURE0);


	// draw mesh
	glBindVertexArray(vao);
	if (indices.size() > 0) {
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	}
	else {
		glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 3);
	}
	glBindVertexArray(0);
}
