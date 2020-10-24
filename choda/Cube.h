#pragma once

#include "Mesh.h"

namespace choda {

	class Cube : public Mesh {
	public:
		Cube(float a);

		// getter & setters
		float getSideLenght() const;
	private:
		float a;
	};
	
}


