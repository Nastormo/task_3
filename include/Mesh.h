#pragma once

#include "iostream"

#include "externals/tinyobjloader/tiny_obj_loader.h"

class Mesh
{
    std::string inputfile = "../models/house.obj";
public:
    tinyobj::attrib_t _attrib;
    std::vector<tinyobj::shape_t> _shapes;
    std::vector<tinyobj::material_t> _materials;
    Mesh();
    Mesh(std::string str);

};


