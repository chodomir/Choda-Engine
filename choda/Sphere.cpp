#include "Sphere.h"
#include <cmath>
choda::Sphere::Sphere(int stackCount, int sectorCount) : stackCount(stackCount), sectorCount(sectorCount)
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


	for (unsigned int y = 0; y <= stackCount; ++y)
	{
		for (unsigned int x = 0; x <= sectorCount; ++x)
		{
			float xSegment = (float)x / (float)sectorCount;
			float ySegment = (float)y / (float)stackCount;
			float xPos = std::cos(xSegment * 2*pi) * std::sin(ySegment * pi); // TAU is 2PI
			float yPos = std::cos(ySegment * pi);
			float zPos = std::sin(xSegment * 2*pi) * std::sin(ySegment * pi);

			vertices.push_back(xPos);
			vertices.push_back(yPos);
			vertices.push_back(zPos);
		}
	}

	for (int y = 0; y < stackCount; ++y)
	{
		for (int x = 0; x < sectorCount; ++x)
		{
			indices.push_back((y + 1) * (sectorCount + 1) + x);
			indices.push_back(y * (sectorCount + 1) + x);
			indices.push_back(y * (sectorCount + 1) + x + 1);

			indices.push_back((y + 1) * (sectorCount + 1) + x);
			indices.push_back(y * (sectorCount + 1) + x + 1);
			indices.push_back((y + 1) * (sectorCount + 1) + x + 1);
		}
	}

	//// calculate vertices
	//float PI = 3.141592f;
	//for (int i = 0; i < stackCount; i++) {
	//	float phi = PI / 2 - i * PI / stackCount;
	//	float y = std::sinf(phi); // r = 1
	//	float xz = std::cosf(phi);
	//	for (int j = 0; j < sectorCount; j++) {
	//		float theta = j * PI / sectorCount;
	//		float x = xz * std::sinf(theta);
	//		float z = xz * std::cosf(theta);

	//		vertices.push_back(x);
	//		vertices.push_back(y);
	//		vertices.push_back(z);
	//	}
	//}

	//// triangulate calculated vertices
	//for (int i = 0; i < stackCount; i++) {
	//	int column = i * sectorCount;
	//	for (int j = 0; j < sectorCount; j++) {
	//		indices.push_back(column + j);
	//		indices.push_back(column + j + 1);
	//		indices.push_back(column + sectorCount + j);

	//		indices.push_back(column + sectorCount + j + 1);
	//		indices.push_back(column + j + 1);
	//		indices.push_back(column + sectorCount + j);
	//	}
	//}

	//vertices.push_back(-0.5f); vertices.push_back(0.5f); vertices.push_back(0.0f);
	//vertices.push_back(-0.5f); vertices.push_back(-0.5f); vertices.push_back(0.0f);
	//vertices.push_back(0.5f); vertices.push_back(0.5f); vertices.push_back(0.0f);
	//vertices.push_back(0.5f); vertices.push_back(-0.5f); vertices.push_back(0.0f);

	//indices.push_back(0); indices.push_back(1); indices.push_back(2);
	//indices.push_back(2); indices.push_back(3); indices.push_back(1);

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
