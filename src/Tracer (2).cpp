#include "include/Tracer.h"
#include "math.h"

#define EPSILON 0.000001

using namespace glm;

glm::vec3 Tracer::ModelFong(Ray ray, Triangle pol, glm::vec3 baryCoord, glm::vec3 color)
{
    float i_a = 0.1f;
    float i_d = 0.1f;
    float i_s = 1.0f;
    glm::vec3 Ia = color * i_a * light.getLightColor();

    glm::vec3 norm = pol.get_normal();
    bool check_intersect = 0;
    glm::vec3 point = ray.get_start() + glm::normalize(ray.get_start()) * baryCoord.z;
    std::cout << ray.get_start().x << ' ' << ray.get_start().y << ' ' << ray.get_start().z << std::endl;
    std::cout << ray.get_dir().x << ' ' << ray.get_dir().y << ' ' << ray.get_dir().z << std::endl;
    std::cout << point.x << ' ' << point.y << ' ' << point.z << std::endl;
    std::cout << pol.get_vert0().x << ' ' << pol.get_vert0().y << ' ' << pol.get_vert0().z << std::endl;
    std::cout << pol.get_vert1().x << ' ' << pol.get_vert1().y << ' ' << pol.get_vert1().z << std::endl;
    std::cout << pol.get_vert2().x << ' ' << pol.get_vert2().y << ' ' << pol.get_vert2().z << std::endl;
    Ray rpoint(point, glm::normalize(light.getLightPosition() - point));
    for (int i = 0; i < scene.size(); i++) {
        Triangle poligon = scene.get_triangle(i);
        glm::vec3 baryPosition;
        if(intersectRayTriangle(rpoint.get_start(), rpoint.get_dir(), poligon.get_vert0(), poligon.get_vert1(), poligon.get_vert2(), baryPosition)) {
            check_intersect = 1;
            //if(baryPosition == baryCoord)
            //    std::cout << "NO" << std::endl;
            break;
        }
    }
    float Id = 0;
    if(!check_intersect) {
        //std::cout << "OK" << std::endl;
        Id = i_d * glm::dot(norm, rpoint.get_dir());
        Id = clamp(Id, 0.0f, 1.0f);
        std::cout << Id << std::endl;
    }
    //float I = 0.299f * color.x + 0.587f * color.y + 0.114 * color.z;
    //float Ir = Ia + Id;
    //std::cout << I << ' ' << Ir << std::endl;
    return Ia;
}

Ray Tracer::MakeRay(glm::uvec2 pixelPos)
{
    float h = camera._resolution.y;
    float w = camera._resolution.x;
    float c = min(h / (2 * tan((camera._viewAngle.x / 2) / 180 * M_PI)), w / (2 * tan((camera._viewAngle.x / 2) / 180 * M_PI)));
    //std::cout << tan((camera._viewAngle.x / 2) / 180 * M_PI) << ' ' << w / (2 * tan((camera._viewAngle.x / 2) / 180 * M_PI)) << std::endl;
    glm::vec3 up = glm::normalize(camera._up) * (h / 2);
    glm::vec3 right = glm::normalize(camera._right) * (w / 2);
    glm::vec3 ray_dir = glm::normalize(camera._forward) * c;
    ray_dir = ray_dir + right * (((pixelPos.x - w / 2) + 0.5f) / w - 0.5f) + up * (((pixelPos.y - h / 2) + 0.5f) / h - 0.5f);
    return Ray(camera._pos, ray_dir);

    /*float angle_step_x = camera._viewAngle.x/camera._resolution.x;
    float angle_step_y = camera._viewAngle.y/camera._resolution.y;
    glm::vec3 m_dir = rotate(camera._forward, (angle_step_x * ((float)pixelPos.x - camera._resolution.x/2)), camera._up);
    glm::vec3 new_right = rotate(camera._right, (angle_step_x * ((float)pixelPos.x - camera._resolution.x/2)), camera._up);
    m_dir = rotate(m_dir, (angle_step_y * ((float)pixelPos.y - camera._resolution.y/2)), new_right);
    return Ray(camera._pos, m_dir);*/
}

glm::vec3 Tracer::TraceRay(Ray ray)
{
    bool check_intersect = 0;
    //double min_t, min_u, min_v;
    glm::vec3 baryPosition;
    glm::vec3 min_baryPosition;
    glm::vec3 min_poligon_v0, min_poligon_v1, min_poligon_v2;
    for (int i = 0; i < scene.size(); i++) {
        Triangle poligon = scene.get_triangle(i);
        if(intersectRayTriangle(ray.get_start(), ray.get_dir(), poligon.get_vert0(), poligon.get_vert1(), poligon.get_vert2(), baryPosition)) {
            //std::cout << "OK" << std::endl;
            if(check_intersect) {
                if(min_baryPosition.z > baryPosition.z) {
                    min_baryPosition = baryPosition;
                    min_poligon_v0 = poligon.get_vert0();
                    min_poligon_v1 = poligon.get_vert1();
                    min_poligon_v2 = poligon.get_vert2();
                }
            } else {
                check_intersect = 1;
                min_baryPosition = baryPosition;
                min_poligon_v0 = poligon.get_vert0();
                min_poligon_v1 = poligon.get_vert1();
                min_poligon_v2 = poligon.get_vert2();
            }
        }
    }
    if(check_intersect) {
        Triangle poligon(min_poligon_v0, min_poligon_v1, min_poligon_v2);
        return ModelFong(ray, poligon, min_baryPosition, glm::vec3(1, 1, 1));
    } else {
        return vec3(0, 0, 0);
    }
}

void Tracer::RenderImage(int xRes, int yRes)
{
  // Rendering
  camera._resolution = uvec2(xRes, yRes);
  camera._pixels.resize(xRes * yRes);

  for(int i = 0; i < yRes; i++)
    for(int j = 0; j < xRes; j++)
    {
      Ray ray = MakeRay(uvec2(j, i));
      camera._pixels[i * xRes + j] = TraceRay(ray);
    }
}

void Tracer::SaveImageToFile(std::string fileName)
{
    BMP image;

    int width = camera._resolution.x;
    int height = camera._resolution.y;

    image.SetSize(width, height);
    RGBApixel pix;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            vec3 color = camera._pixels[width * i + j];
            pix.Red=color.r * 255.0;
            pix.Green=color.g * 255.0;
            pix.Blue=color.b * 255.0;
            image.SetPixel(j, i, pix);
        }
    }
    if (!image.WriteToFile(fileName.c_str()))
        std::cout << "Error writing file " << fileName;
}

BMP* Tracer::LoadImageFromFile(std::string fileName)
{
  BMP* Image = new BMP;
  if(Image->ReadFromFile(fileName.c_str()))
      return Image;
  else
  {
      std::cout << "Error reading file "  << fileName;
      delete Image;
      return NULL;
  }
}


