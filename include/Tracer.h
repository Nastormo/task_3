#pragma once

#include "string"
#include "math.h"
#include "cmath"
#include "vector"

#include "include/Ray.h"
#include "include/Scene.h"
#include "include/Camera.h"
#include "include/Light.h"
#include "include/Antena.h"

#include "externals/EasyBMP/include/EasyBMP.h"
#include "externals/glm/vec3.hpp"
#include "externals/glm/gtx/intersect.hpp"
#include "externals/glm/glm.hpp"
#include "externals/glm/geometric.hpp"
#include "externals/glm/gtc/random.hpp"
#include "externals/glm/gtx/normal.hpp"

class Tracer
{

public:
    Camera camera;
    Scene scene;
    Light light;
    Antena antena;
    int xRes;
    int yRes;
    bool anti_aliasing = false;


    Ray MakeRay(glm::uvec2 pixelPos);  // Create ray for specified pixel
    glm::vec3 TraceRay(Ray ray); // Trace ray, compute its color
    void RenderImage();
    void SaveImageToFile(std::string fileName);
    BMP* LoadImageFromFile(std::string fileName);
    std::vector<Ray> MakeRayAnt(glm::uvec2 pixelPos);

    void InitAntena();

    glm::vec3 ModelFong(Ray ray, glm::vec3 point_intersect, glm::vec3 normal, glm::vec3 color);
    bool SetValVok(glm::vec3 pos, float val);
    glm::vec3 InitSignal(Ray ray);
    glm::vec3 ShowSignal(Ray ray, glm::vec3 point_intersect, glm::vec3 normal, glm::vec3 color);
    void autocontrast(double fraction);
};
