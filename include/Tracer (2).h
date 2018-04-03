#pragma once

#include "string"

#include "include/Ray.h"
#include "include/Scene.h"
#include "include/Camera.h"
#include "include/Light.h"

#include "externals/EasyBMP/include/EasyBMP.h"
#include "externals/glm/vec3.hpp"
#include "externals/glm/gtx/intersect.hpp"
#include "externals/glm/glm.hpp"
#include "externals/glm/gtx/rotate_vector.hpp"

class Tracer
{
    Camera camera;
    Scene scene;
    Light light;
public:
    Ray MakeRay(glm::uvec2 pixelPos);  // Create ray for specified pixel
    glm::vec3 TraceRay(Ray ray); // Trace ray, compute its color
    void RenderImage(int xRes, int yRes);
    void SaveImageToFile(std::string fileName);
    BMP* LoadImageFromFile(std::string fileName);

    glm::vec3 ModelFong(Ray ray, Triangle poligon, glm::vec3 baryPosition, glm::vec3 color);
    //glm::vec3 ModelFong(Ray ray, glm::vec3 point, glm::vec3 color);
};
