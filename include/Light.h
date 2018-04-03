#pragma once

#include "externals/glm/glm.hpp"

class Light {
    glm::vec3 position;
    glm::vec3 color;

    public:

    Light ();

    Light (glm::vec3 pos, glm::vec3 col);

    // method functions
    glm::vec3 getLightPosition () { return position; }
    glm::vec3 getLightColor () { return color; }

};

