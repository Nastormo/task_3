#pragma once

#include "vector"

#include "externals/glm/glm.hpp"

class Camera {
public:
    glm::vec3 _pos;          // Camera position and orientation
    glm::vec3 _forward;      // Orthonormal basis
    glm::vec3 _right;
    glm::vec3 _up;

    glm::vec2 _viewAngle;    // View angles, rad
    glm::uvec2 _resolution;  // Image resolution: w, h

    std::vector<glm::vec3> _pixels;  // Pixel array

    Camera();
    Camera (glm::vec3 pos, glm::vec3 dir, glm::vec3 right, glm::vec3 up, glm::vec2 view);

    //Camera (Vect, Vect, Vect, Vect);

    // method functions
    //Vect getCameraPosition () { return campos; }
    //Vect getCameraDirection () { return camdir; }
    //Vect getCameraRight () { return camright; }
    //Vect getCameraDown () { return camdown; }

};
