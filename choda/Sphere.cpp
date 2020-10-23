#include "Sphere.h"
#include <cmath>
choda::Sphere::Sphere(float radius, int stackCount, int sectorCount) : radius(radius), stackCount(stackCount), sectorCount(sectorCount)
{
}

choda::Sphere::Sphere() : radius(1.0f), stackCount(18), sectorCount(36)
{
}

choda::Sphere::~Sphere()
{
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
	glDeleteVertexArrays(1, &vao);
}

void choda::Sphere::init() {
	float pi = 3.141592f;

	// calculate vertices
	float PI = 3.141592f;
	for (int i = 0; i <= stackCount; i++) {
		float phi = PI / 2 - i * PI / stackCount;
		float y = radius * std::sinf(phi); // r = 1
		float xz = radius * std::cosf(phi);
		for (int j = 0; j <= sectorCount; j++) {
			float theta = j * 2*PI / sectorCount;
			float x = xz * std::sinf(theta);
			float z = xz * std::cosf(theta);

			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(z);
		}
	}

	// triangulate calculated vertices
	for (int i = 0; i < stackCount; i++) {
		for (int j = 0; j < sectorCount; j++) {
			indices.push_back((i + 1) * (sectorCount + 1) + j);
			indices.push_back(i * (sectorCount + 1) + j);
			indices.push_back(i * (sectorCount + 1) + j + 1);

			indices.push_back((i + 1) * (sectorCount + 1) + j);
			indices.push_back((i + 1) * (sectorCount + 1) + j + 1);
			indices.push_back(i * (sectorCount + 1) + j + 1);
		}
	}

	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	glGenVertexArrays(1, &vao);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void choda::Sphere::draw()
{
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}
