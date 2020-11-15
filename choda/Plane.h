#pragma once
#include "Mesh.h"

namespace choda {

    class Plane : public Mesh
    {
    public:
        Plane();
        Plane(int xSegments, int ySegments);
    };
}

