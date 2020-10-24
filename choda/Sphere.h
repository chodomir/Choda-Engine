#pragma once

#include <glad/glad.h>
#include <vector>

#include "Mesh.h"

namespace choda {
	
	class Sphere : public Mesh {
	public:
		Sphere(float radius, int stackCount, int sectorCount);
		Sphere();
	private:
		void calculate();
	private:
		int stackCount, sectorCount;
		float radius;
	};
}


