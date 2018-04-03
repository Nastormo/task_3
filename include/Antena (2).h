#pragma once

#include "externals/glm/glm.hpp"

class Antena {
    glm::vec3 position;
    glm::vec3 color;
public:
    float force;
    Antena () : position(glm::vec3(0, 3, 2)), color(glm::vec3(1, 1, 1)), force(100) {};

    Antena (glm::vec3 pos, glm::vec3 col);

    // method functions
    glm::vec3 getAntenaPosition () { return position; }
    glm::vec3 getAntenaColor () { return color; }
    //glm::vec3 getAntenaForce() { return force; }

};

