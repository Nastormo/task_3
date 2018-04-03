#pragma once

#include "externals/glm/glm.hpp"

class Ray
{
    glm::vec3 start;
    glm::vec3 dir;
public:
    Ray(glm::vec3 tstart, glm::vec3 tdir)
    {
      start = tstart;
      dir = tdir;
    };
    glm::vec3 get_start() { return start; };
    glm::vec3 get_dir() { return dir; };
};

