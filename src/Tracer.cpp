#include "include/Tracer.h"


#define EPSILON 0.000001

using namespace glm;

/*Matrix<double> kernel = {{ -1.0/6,  -2.0/3,  -1.0/6},
                             { -2.0/3,  13.0/3,  -2.0/3},
                             {-1.0/6, -2.0/3, -1.0/6}};*/

void Tracer::autocontrast(double fraction) {
    uint min_y = 255;
    uint max_y = 0;
    for(int i = 0; i < yRes; i++) {
        for(int j = 0; j < xRes; j++) {
            float r = camera._pixels[i * xRes + j].x * 255;
            float g = camera._pixels[i * xRes + j].y * 255;
            float b = camera._pixels[i * xRes + j].z * 255;
            float Y = 0.2125 * r + 0.7154 * g + 0.0721 * b;
            uint cur_val = floor(Y);
            if(min_y > cur_val)
                min_y = cur_val;
            if(max_y < cur_val)
                max_y = cur_val;
        }
    }
    uint dif = max_y - min_y;
    min_y += fraction * dif;
    max_y -= fraction * dif;
    for(int i = 0; i < yRes; i++) {
        for(int j = 0; j < xRes; j++) {
            float r = camera._pixels[i * xRes + j].x * 255;
            float g = camera._pixels[i * xRes + j].y * 255;
            float b = camera._pixels[i * xRes + j].z * 255;
            float Y = 0.2125 * r + 0.7154 * g + 0.0721 * b;
            float new_Y = (Y - min_y) * (255.0 / (max_y - min_y));
            float coef = new_Y / Y;
            r = r * coef > 255 ? 255 : (r * coef < 0 ? 0 : r * coef);
            g = g * coef > 255 ? 255 : (g * coef < 0 ? 0 : g * coef);
            b = b * coef > 255 ? 255 : (b * coef < 0 ? 0 : b * coef);
            camera._pixels[i * xRes + j] = glm::vec3(r / 255.0f, g / 255.0f, b / 255.0f);
        }
    }
}

glm::vec3 Tracer::ModelFong(Ray ray, glm::vec3 point_intersect, glm::vec3 normal, glm::vec3 color)
{
    float i_a = 0.1f;
    float i_d = 0.2f;
    float i_s = 0.25f;
    glm::vec3 RezultColor;
    glm::vec3 Ia = color * i_a * light.getLightColor();
    glm::vec3 point = point_intersect;
    glm::vec3 norm = normal;
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
    float Is(0);
    if(!check_intersect) {
        glm::vec3 ref = glm::reflect(glm::normalize(point - light.getLightPosition()), norm);
        glm::vec3 v = glm::normalize(camera._pos - point);
        Is = i_s * std::pow(glm::dot(norm, v), 4);
        //std::cout << Is << std::endl;
    }
    RezultColor = Ia + Id + Is;
    return RezultColor;
}

glm::vec3 Tracer::ShowSignal(Ray ray, glm::vec3 point_intersect, glm::vec3 normal, glm::vec3 color)
{
    float alfa = 0.02f;
    glm::vec3 RezultColor = color;
    //glm::vec3 point = (1 - baryCoord.x - baryCoord.y) * pol.get_vert0() + baryCoord.x * pol.get_vert1() + baryCoord.y * pol.get_vert2();
    glm::vec3 point = point_intersect;
    glm::vec3 tpoint = point;
    while(1)
    {
        glm::vec3 Color;
        glm::vec3 ind = (tpoint - scene.min_point) / scene.step;
        if(ind.x < 0 || scene.size_vok_x <= ind.x || ind.y < 0 || scene.size_vok_y <= ind.y || ind.z < 0 || scene.size_vok_z <= ind.z) break;
        float d = scene.anten_signal[floor(ind.x)][floor(ind.y)][floor(ind.z)];
        if(d > 0.0f) {
            //std::cout << "l" << d << ' ' << d / antena.force << std::endl;
            d = d / antena.force;
            Color.x = std::min(1.0f, 2.0f * d);
            Color.y = std::max(0.0f, -1.0f + 2.0f * d);
            Color.z = abs(d - 0.5);
            RezultColor = Color * alfa + RezultColor * (1.0f - alfa);
        }
        tpoint += glm::normalize(ray.get_dir()) * -1.0f * length(scene.step);
    }
    return RezultColor;
}

bool Tracer::SetValVok(glm::vec3 pos, float val)
{
    pos = (pos - scene.min_point) / scene.step;
    int x = floor(pos.x);
    int y = floor(pos.y);
    int z = floor(pos.z);
    //std::cout << x << ' ' << y << ' ' << z << std::endl;
    if(0 <= x && x < scene.size_vok_x && 0 <= y && y < scene.size_vok_y && 0 <= z && z <= scene.size_vok_z) {
        if(scene.anten_signal[x][y][z] < val)
            scene.anten_signal[x][y][z] = val;
        return 1;
    } else {
        return 0;
    }
}

glm::vec3 Tracer::InitSignal(Ray ray)
{
    int refl = 0;
    bool end_flag = true;
    float power = antena.force, dist = 0;

    while(end_flag and refl < 5) {
        Triangle poligon, tmp_poligon;
        float minDistance = 100000;
        double eps = 0.01;
        vec3 point;
        bool check_intersect = false;
        #pragma omp parallel
        {
            #pragma omp for
            for(int i = 0; i < scene.size(); i++) {
                tmp_poligon = scene.get_triangle(i);
                vec3 pos;
                if(glm::intersectRayTriangle(ray.get_start(), ray.get_dir(), tmp_poligon.get_vert0(), tmp_poligon.get_vert1(), tmp_poligon.get_vert2(), pos)) {
                    if(pos.z < minDistance) {
                        minDistance = pos.z;
                        pos = ray.get_start() + ray.get_dir() * pos.z;
                        poligon = scene.get_triangle(i);
                        point = pos;
                        check_intersect = true;
                    }
                }
            }
        }
        if(check_intersect) {
            float end = length(ray.get_start() - point);
            vec3 tmp = ray.get_start();
            while(length(tmp - ray.get_start()) < end) {
                power = antena.force - eps * dist * dist;
                std::cout << power << std::endl;
                if(power < 0) power = 0;
                if(!(end_flag = SetValVok(tmp, power))) {
                    break;
                }
                dist += 1;
                tmp += ray.get_dir() * (glm::length(scene.step));
            }
            vec3 normal = glm::triangleNormal(poligon.get_vert0(), poligon.get_vert1(), poligon.get_vert2());
            ray = Ray(point, glm::reflect(ray.get_dir(), normal));
        } else {
            vec3 tmp = ray.get_start();
            while(tmp.x < scene.max_point.x && tmp.y < scene.max_point.y && tmp.z < scene.max_point.z &&
                  tmp.x > scene.min_point.z && tmp.y > scene.min_point.y && tmp.z > scene.min_point.z ) {
                power = antena.force - eps * dist * dist;
                std::cout << power << std::endl;
                if(power < 0) power = 0;
                if(!(end_flag = SetValVok(tmp, power))) {
                    break;
                }
                dist += 1;
                tmp += ray.get_dir() * (glm::length(scene.step));
            }
            end_flag = false;
        }
        refl++;
    }
}

void Tracer::InitAntena()
{
    int num_ray = 200000;
    #pragma omp parallel
    {
        #pragma omp for
        for(int i = 0; i < num_ray; i++) {
            Ray ray_antena(antena.getAntenaPosition(), glm::sphericalRand(1.0f));
            InitSignal(ray_antena);
        }
    }
    float eps = 0.00001;
    #pragma omp parallel
    {
        #pragma omp for
        for(int i = 1; i < scene.size_vok_x - 1; i++) {
            for(int j = 1;j < scene.size_vok_y - 1; j++) {
                for(int q = 1; q < scene.size_vok_z - 1; q++) {
                    float sum = 0.0;
                    int count = 0;
                    for(int x = -1; x <= 1; x++) {
                        for(int y = -1; y <= 1; y++) {
                            for(int z = -1; z <= 1; z++) {
                                if(scene.anten_signal[i+x][j+y][q+z] > 0.0f) {
                                    sum += scene.anten_signal[i+x][j+y][q+z];
                                    count++;
                                }
                            }
                        }
                    }
                    if(scene.anten_signal[i][j][q] > 0.0f) {
                        scene.anten_signal[i][j][q] = std::max(sum / count, scene.anten_signal[i][j][q]);
                    } else {
                        scene.anten_signal[i][j][q] = sum / 27;
                    }
                }
            }
        }
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

std::vector<Ray> Tracer::MakeRayAnt(glm::uvec2 pixelPos)
{
    std::vector<Ray> vec_ray;
    float h = camera._resolution.y;
    float w = camera._resolution.x;
    float c = min(h / (2 * tan((camera._viewAngle.y / 2) / 180 * M_PI)), w / (2 * tan((camera._viewAngle.x / 2) / 180 * M_PI)));
    glm::vec3 up = glm::normalize(camera._up) * (h / 2);
    glm::vec3 right = glm::normalize(camera._right) * (w / 2);
    glm::vec3 ray_dir = glm::normalize(camera._forward) * c;
    for(int i = 0; i <= 1; i++) {
        for(int j = 0; j <= 1; j++) {
            glm::vec3 tmp_dir = ray_dir + right * (((pixelPos.x) + i) / w - 0.5f) + up * (((pixelPos.y) + j) / h - 0.5f);
            vec_ray.push_back(Ray(camera._pos, tmp_dir));
            //std::cout << "l" << tmp_dir.x << ' ' << tmp_dir.y << ' ' << tmp_dir.z << std::endl;
        }
    }
    return vec_ray;
}

glm::vec3 Tracer::TraceRay(Ray ray)
{
    bool check_intersect = 0;
    glm::vec3 baryPosition;
    glm::vec3 min_baryPosition;
    glm::vec3 min_poligon_v0, min_poligon_v1, min_poligon_v2;
    glm::vec3 point;
    glm::vec3 norm;
    float min_t;
    for (int i = 0; i < scene.size(); i++) {
        Triangle poligon = scene.get_triangle(i);
        if(intersectRayTriangle(ray.get_start(), glm::normalize(ray.get_dir()), poligon.get_vert0(), poligon.get_vert1(), poligon.get_vert2(), baryPosition)) {
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
    point = (1 - min_baryPosition.x - min_baryPosition.y) * min_poligon_v0 + min_baryPosition.x * min_poligon_v1 + min_baryPosition.y * min_poligon_v2;
    norm = triangleNormal(min_poligon_v0, min_poligon_v1, min_poligon_v2);
    glm::vec3 pos;
    glm::vec3 normal;
    if(intersectRaySphere(ray.get_start(), glm::normalize(ray.get_dir()), antena.getAntenaPosition(), antena.getRadius(), pos, normal)) {
        //std::cout << pos.x << ' ' << pos.y << ' ' << pos.z << std::endl;
        //std::cout << normal.x << ' ' << normal.y << ' ' << normal.z << std::endl;
        point = pos;
        norm = normal;
        check_intersect = 1;
    }
    if(check_intersect) {
        glm::vec3 l =  ModelFong(ray, point, norm, glm::vec3(1, 1, 1));
        //glm::vec3 rez = ShowSignal(ray, point, norm, l);
        return l;
    } else {
        return vec3(0, 0, 0);
    }
}

void Tracer::RenderImage()
{
  camera._resolution = uvec2(xRes, yRes);
  camera._pixels.resize(xRes * yRes);
  //InitAntena();
    #pragma omp parallel
    {
        #pragma omp for
        for(int i = 0; i < yRes; i++)
            for(int j = 0; j < xRes; j++)
            {
              Ray ray = MakeRay(uvec2(j, i));
              glm::vec3 Color = TraceRay(ray);
              if(anti_aliasing)
              {
                  std::vector<Ray> vec = MakeRayAnt(uvec2(j, i));
                  for(int i = 0; i < 4; i++) {
                    Color += TraceRay(vec[i]);
                  }
                  Color /= 5.0f;
              }
              camera._pixels[i * xRes + j] = Color;
            }
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


