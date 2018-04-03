#pragma once

#include "include/Mesh.h"
#include "include/Poligon.h"
#include <vector>

#include "externals/glm/glm.hpp"

//using namespace vector;



class Scene
{
   std::vector<Triangle> triangle;
   Mesh house;
public:
   int size_vok_x = 200;
   int size_vok_y = 200;
   int size_vok_z = 100;
   glm::vec3 min_point;
   glm::vec3 max_point;
   glm::vec3 step;
   std::vector<std::vector <std::vector <float> > > anten_signal{size_vok_x, std::vector<std::vector<float> >(size_vok_y, std::vector<float> (size_vok_z, 0.0f))};

   Scene();
   Scene(std::string str, int size_x, int size_y, int size_z);
   Triangle get_triangle(int i) { return triangle[i]; };
   int size() { return triangle.size(); };

   //Set of meshes
};
