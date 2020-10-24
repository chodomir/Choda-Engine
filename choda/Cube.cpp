#include "Cube.h"

choda::Cube::Cube(float a) : Mesh(), a(a) {
	float v = a / 2;
	// top
	vertices.push_back(-v); vertices.push_back(v); vertices.push_back(v);
	vertices.push_back(v); vertices.push_back(v); vertices.push_back(v);
	vertices.push_back(v); vertices.push_back(v); vertices.push_back(-v);
	vertices.push_back(-v); vertices.push_back(v); vertices.push_back(-v);
	// bottom
	vertices.push_back(-v); vertices.push_back(-v); vertices.push_back(v);
	vertices.push_back(v); vertices.push_back(-v); vertices.push_back(v);
	vertices.push_back(v); vertices.push_back(-v); vertices.push_back(-v);
	vertices.push_back(-v); vertices.push_back(-v); vertices.push_back(-v);

	// top face
	indices.push_back(0); indices.push_back(1); indices.push_back(2);
	indices.push_back(2); indices.push_back(0); indices.push_back(3);
	// front face
	indices.push_back(0); indices.push_back(1); indices.push_back(4);
	indices.push_back(4); indices.push_back(5); indices.push_back(1);
	// back face
	indices.push_back(3); indices.push_back(2); indices.push_back(7);
	indices.push_back(7); indices.push_back(6); indices.push_back(2);
	// left face
	indices.push_back(3); indices.push_back(7); indices.push_back(0);
	indices.push_back(7); indices.push_back(4); indices.push_back(0);
	// right face
	indices.push_back(1); indices.push_back(5); indices.push_back(2);
	indices.push_back(5); indices.push_back(6); indices.push_back(2);
	// bottom face
	indices.push_back(4); indices.push_back(7); indices.push_back(6);
	indices.push_back(6); indices.push_back(4); indices.push_back(5);
}

float choda::Cube::getSideLenght() const {
	return a;
}
