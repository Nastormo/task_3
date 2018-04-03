#pragma once

#include "include/Mesh.h"
#include "include/Poligon.h"
#include <vector>

#include "externals/glm/glm.hpp"

//using namespace vector;

int size_vok_x = 1000;
int size_vok_y = 1000;
int size_vok_z = 1000;

class Scene
{
   std::vector<Triangle> triangle;
   Mesh house;
public:
   glm::vec3 min_point;
   glm::vec3 max_point;
   glm::vec3 step;
   std::vector<std::vector <std::vector <float> > > anten_signal{100, std::vector<std::vector<float> >(100, std::vector<float> (100, 0.0f))};

   Scene();
   Triangle get_triangle(int i) { return triangle[i]; };
   int size() { return triangle.size(); };

   //Set of meshes
};
