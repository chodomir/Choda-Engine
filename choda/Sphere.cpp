#include "Sphere.h"
#include <cmath>
choda::Sphere::Sphere(float radius, int stackCount, int sectorCount) : Mesh(), radius(radius), stackCount(stackCount), sectorCount(sectorCount) {
	calculate();
}

choda::Sphere::Sphere() : Mesh(), radius(1.0f), stackCount(18), sectorCount(36) {
	calculate();
}

void choda::Sphere::calculate() {
	float pi = 3.141592f;

	// calculate vertices
	float PI = 3.141592f;
	for (int i = 0; i <= stackCount; i++) {
		float phi = PI / 2 - i * PI / stackCount;
		float y = radius * std::sinf(phi); // r = 1
		float xz = radius * std::cosf(phi);
		for (int j = 0; j <= sectorCount; j++) {
			float theta = j * 2 * PI / sectorCount;
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
}
