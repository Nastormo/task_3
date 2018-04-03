#include "include/Tracer.h"


#define EPSILON 0.000001

using namespace glm;

glm::vec3 Tracer::ModelFong(Ray ray, Triangle pol, glm::vec3 baryCoord, glm::vec3 color)
{
    float alfa = 0.2f;
    float i_a = 0.1f;
    float i_d = 0.2f;
    float i_s = 1.0f;
    glm::vec3 RezultColor;
    glm::vec3 Ia = color * i_a * light.getLightColor();
    glm::vec3 point = (1 - baryCoord.x - baryCoord.y) * pol.get_vert0() + baryCoord.x * pol.get_vert1() + baryCoord.y * pol.get_vert2();
    glm::vec3 norm = pol.get_normal();
    bool check_intersect = 0;
    Ray rpoint(point, glm::normalize(light.getLightPosition() - point));
    for (int i = 0; i < scene.size(); i++) {
        Triangle poligon = scene.get_triangle(i);
        glm::vec3 baryPosition;
        if(intersectRayTriangle(rpoint.get_start(), glm::normalize(rpoint.get_dir()), poligon.get_vert0(), poligon.get_vert1(), poligon.get_vert2(), baryPosition)) {
            check_intersect = 1;
            break;
        }
    }
    float Id(0);
    if(!check_intersect) {
        Id = i_d * glm::dot(norm, glm::normalize(rpoint.get_dir()));
        Id = clamp(Id, 0.0f, 1.0f);
    }
    RezultColor = Ia + Id;
    float len = 0.0;
    float eps = 0.00001;
    while(1)
    {
        glm::vec3 Color;
        glm::vec3 tpoint = point + ray.get_dir() * -1.0f * len;
        if((tpoint.x < scene.min_point.x || tpoint.x > scene.max_point.x || abs(tpoint.x - scene.max_point.x) < eps || abs(tpoint.x - scene.min_point.x) < eps) ||
           (tpoint.y < scene.min_point.y || tpoint.y > scene.max_point.y || abs(tpoint.y - scene.max_point.y) < eps || abs(tpoint.y - scene.min_point.y) < eps) ||
           (tpoint.z < scene.min_point.z || tpoint.z > scene.max_point.z || abs(tpoint.z - scene.max_point.z) < eps || abs(tpoint.z - scene.min_point.z) < eps))
        {
            break;
        }
        glm::vec3 ind = (tpoint - scene.min_point) / scene.step;
        float d = scene.anten_signal[floor(ind.x)][floor(ind.y)][floor(ind.z)];
        Color.x = std::min(1.0f, 2.0f * d);
        Color.y = std::max(0.0f, -1.0f + 2 * d);
        Color.z = abs(d - 0.5);
        RezultColor = Color * alfa + RezultColor * (1 - alfa);
        len += glm::length(scene.step);
    }



    return RezultColor;
}

void Tracer::InitAntena()
{
    //antena.

    int num_ray = 10000;
    float max_len = antena.force * glm::length(scene.step);
    for(int i = 0; i < num_ray; i++) {
        float coord_x = (float)(rand() % 180) / 180 * M_PI;
        float coord_y = (float)(rand() % 360) / 180 * M_PI;
        std::cout << coord_x << ' ' << coord_y << std::endl;
        float x = sin(coord_x) * cos(coord_y);
        float y = sin(coord_x) * sin(coord_y);
        float z = cos(coord_x);
        std::cout << x << ' ' << y << ' ' << z << std::endl;
        glm::vec3 dir(x, y, z);
        Ray ray_antena(antena.getAntenaPosition(), glm::vec3(x, y, z));
        float len = 0.0;
        float eps = 0.00001;
        while(1)
        {
            glm::vec3 point = antena.getAntenaPosition() + dir * len;
            //std::cout << "OK" << std::endl;
            if((point.x < scene.min_point.x || point.x > scene.max_point.x || abs(point.x - scene.max_point.x) < eps || abs(point.x - scene.min_point.x) < eps) ||
               (point.y < scene.min_point.y || point.y > scene.max_point.y || abs(point.y - scene.max_point.y) < eps || abs(point.y - scene.min_point.y) < eps) ||
               (point.z < scene.min_point.z || point.z > scene.max_point.z || abs(point.z - scene.max_point.z) < eps || abs(point.z - scene.min_point.z) < eps))
            {
                break;
            }
            glm::vec3 ind = (point - scene.min_point) / scene.step;
            if(scene.anten_signal[floor(ind.x)][floor(ind.y)][floor(ind.z)] < 1 - ((len * len)/(max_len * max_len))) {
                //std::cout << ind.x << ' ' << ind.y << ' ' << ind.z << std::endl;
                scene.anten_signal[floor(ind.x)][floor(ind.y)][floor(ind.z)] = 1 - ((len * len)/(max_len * max_len));
            }
            len += glm::length(scene.step);
        }
        /*for(int i = 1; i < 99; i++) {
            for(int j = 1;j < 99; j++) {
                for(int q = 1; q < 99; q++) {
                    float sum = 0.0;
                    for(int x = -1; x <= 1; x++) {
                        for(int y = -1; y <= 1; y++) {
                            for(int z = -1; z <= 1; z++) {
                                sum += scene.anten_signal[i+x][j+y][q+z];
                            }
                        }

                    }
                    scene.anten_signal[i][j][q] = sum / 27;
                }
            }
        }*/
    }
}

Ray Tracer::MakeRay(glm::uvec2 pixelPos)
{
    float h = camera._resolution.y;
    float w = camera._resolution.x;
    float c = min(h / (2 * tan((camera._viewAngle.y / 2) / 180 * M_PI)), w / (2 * tan((camera._viewAngle.x / 2) / 180 * M_PI)));
    glm::vec3 up = glm::normalize(camera._up) * (h / 2);
    glm::vec3 right = glm::normalize(camera._right) * (w / 2);
    glm::vec3 ray_dir = glm::normalize(camera._forward) * c;
    ray_dir = ray_dir + right * (((pixelPos.x) + 0.5f) / w - 0.5f) + up * (((pixelPos.y) + 0.5f) / h - 0.5f);
    return Ray(camera._pos, ray_dir);
}

glm::vec3 Tracer::TraceRay(Ray ray)
{
    bool check_intersect = 0;
    glm::vec3 baryPosition;
    glm::vec3 min_baryPosition;
    glm::vec3 min_poligon_v0, min_poligon_v1, min_poligon_v2;
    float min_t;
    for (int i = 0; i < scene.size(); i++) {
        Triangle poligon = scene.get_triangle(i);
        if(intersectRayTriangle(ray.get_start(), glm::normalize(ray.get_dir()), poligon.get_vert0(), poligon.get_vert1(), poligon.get_vert2(), baryPosition)) {
            //glm::vec3 point = baryPosition.y * poligon.get_vert2() + baryPosition.z * poligon.get_vert0() + (1 - baryPosition.y - baryPosition.z) * poligon.get_vert1();
            //glm::vec3 point = (1 - baryCoord.x - baryCoord.y) * pol.get_vert0() + baryCoord.x * pol.get_vert1() + baryCoord.y * pol.get_vert2();
            if(check_intersect) {
                if(min_t > baryPosition.z) {
                    min_baryPosition = baryPosition;
                    min_poligon_v0 = poligon.get_vert0();
                    min_poligon_v1 = poligon.get_vert1();
                    min_poligon_v2 = poligon.get_vert2();
                    min_t = baryPosition.z;
                }
            } else {
                check_intersect = 1;
                min_baryPosition = baryPosition;
                min_poligon_v0 = poligon.get_vert0();
                min_poligon_v1 = poligon.get_vert1();
                min_poligon_v2 = poligon.get_vert2();
                min_t = baryPosition.z;
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
  InitAntena();
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


