#pragma once

#include "externals/glm/glm.hpp"

class Antena {
    glm::vec3 position;
    glm::vec3 color;
    float radius;
public:
    float force;
    Antena () : position(glm::vec3(0, 4, 2)), color(glm::vec3(1, 1, 1)), force(100), radius(0.5f) {};

    Antena (glm::vec3 pos, glm::vec3 col, float f) {
        position = pos;
        color = col;
        force = f;
        radius = 0.5f;
    };

    // method functions
    glm::vec3 getAntenaPosition () { return position; }
    glm::vec3 getAntenaColor () { return color; }
    float getRadius() { return radius; }
    //glm::vec3 getAntenaForce() { return force; }

};

