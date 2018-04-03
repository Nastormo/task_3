#define TINYOBJLOADER_IMPLEMENTATION

#include "include/Mesh.h"

Mesh::Mesh()
{
    std::string err;
    bool ret = tinyobj::LoadObj(&_attrib, &_shapes, &_materials, &err, inputfile.c_str());

    if (!err.empty()) {
        std::cerr << err << std::endl;
    }
    if (!ret) {
        //std::cout << "Error in Mesh" << std::endl;
        exit(1);
    }
}

Mesh::Mesh(std::string str)
{
    std::string err;
    inputfile = str;
    bool ret = tinyobj::LoadObj(&_attrib, &_shapes, &_materials, &err, inputfile.c_str());

    if (!err.empty()) {
        std::cerr << err << std::endl;
    }
    if (!ret) {
        //std::cout << "Error in Mesh" << std::endl;
        exit(1);
    }
}

#undef TINYOBJLOADER_IMPLEMENTATION
