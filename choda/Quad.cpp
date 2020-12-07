#include "Quad.h"

choda::Quad::Quad()
{
	vertices = {
		-1.0f, -1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,

		1.0f, -1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f
	};

	texCoords = {
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 0.0f,

		1.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f
	};
}
