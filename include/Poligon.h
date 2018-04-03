#pragma once

#include "externals/glm/vec3.hpp"
#include "externals/glm/glm.hpp"

class Triangle {
    glm::vec3 vert0;
    glm::vec3 vert1;
    glm::vec3 vert2;
    glm::vec3 normal;
public:
    Triangle() : vert0(glm::vec3(0, 0, 0)), vert1(glm::vec3(0, 0, 0)), vert2(glm::vec3(0, 0, 0)) {
        normal = glm::normalize(glm::cross(vert1 - vert0, vert2 - vert0));
    };
    Triangle(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2) : vert0(v0), vert1(v1), vert2(v2) {
        normal = glm::normalize(glm::cross(vert1 - vert0, vert2 - vert0));
        //std::cout << v0.x << ' ' << v0.y << ' ' << v0.z << std::endl;
    };
    glm::vec3 get_vert0() { return vert0; };
    glm::vec3 get_vert1() { return vert1; };
    glm::vec3 get_vert2() { return vert2; };
    glm::vec3 get_normal() { return normal; };
};

