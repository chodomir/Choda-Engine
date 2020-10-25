#pragma once

#include <glad/glad.h>
#include <vector>

#include "Mesh.h"

namespace choda {
	
	class Sphere : public Mesh {
	public:
		Sphere(float radius = 1.0f, int stackCount = 18, int sectorCount = 36);
	private:
		int stackCount, sectorCount;
		float radius;
	};
}


