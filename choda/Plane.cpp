#include "Plane.h"

choda::Plane::Plane() : Plane(20, 20)
{
}

choda::Plane::Plane(int xSegments, int ySegments)
{
	float dx = 2.0f / xSegments;
	float dy = 2.0f / ySegments;

	// vertex data
	for (int i = 0; i <= ySegments; i++) {
		for (int j = 0; j <= xSegments; j++) {
			// vertex positions
			vertices.push_back(dx * j - 1.0f);	// x
			vertices.push_back(dy * i - 1.0f);	// y
			vertices.push_back(0.0f);			// z

			// normals
			normals.push_back(0.0f);
			normals.push_back(0.0f);
			normals.push_back(1.0f);

			// texture coordinates
			texCoords.push_back(dx * j - 1.0f);
			texCoords.push_back(2.0f - dy * i);
		}
	}

	// indices
	for (int i = 0; i < ySegments; i++) {
		for (int j = 0; j < xSegments; j++) {
			int k1 = i * xSegments + j + i;
			int k2 = (i + 1) * xSegments + j + i + 1;
			// first triangle
			indices.push_back(k1);
			indices.push_back(k1 + 1);
			indices.push_back(k2);
			// second triangle
			indices.push_back(k2);
			indices.push_back(k2 + 1);
			indices.push_back(k1 + 1);
		}
	}
}
