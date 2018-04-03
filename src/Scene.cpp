#include "include/Scene.h"
#include <stdio.h>

Scene::Scene(std::string str, int size_x = 200, int size_y = 200, int size_z = 100) : size_vok_x(size_x), size_vok_y(size_y), size_vok_z(size_z)
{
    house = Mesh(str);
    for (size_t s = 0; s < house._shapes.size(); s++) {
      // Loop over faces(polygon)
      size_t index_offset = 0;
      for (size_t f = 0; f < house._shapes[s].mesh.num_face_vertices.size(); f++) {
        int fv = house._shapes[s].mesh.num_face_vertices[f];
        // Loop over vertices in the face.
        glm::vec3 v0;
        glm::vec3 v1;
        glm::vec3 v2;
        for (size_t v = 0; v < fv; v++) {
          // access to vertex
          tinyobj::index_t idx = house._shapes[s].mesh.indices[index_offset + v];
          tinyobj::real_t vx = house._attrib.vertices[3*idx.vertex_index+0];
          tinyobj::real_t vy = house._attrib.vertices[3*idx.vertex_index+1];
          tinyobj::real_t vz = house._attrib.vertices[3*idx.vertex_index+2];
          tinyobj::real_t nx = house._attrib.normals[3*idx.normal_index+0];
          tinyobj::real_t ny = house._attrib.normals[3*idx.normal_index+1];
          tinyobj::real_t nz = house._attrib.normals[3*idx.normal_index+2];
          tinyobj::real_t tx = house._attrib.texcoords[2*idx.texcoord_index+0];
          tinyobj::real_t ty = house._attrib.texcoords[2*idx.texcoord_index+1];
          // Optional: vertex colors
          tinyobj::real_t red = house._attrib.colors[3*idx.vertex_index+0];
          tinyobj::real_t green = house._attrib.colors[3*idx.vertex_index+1];
          tinyobj::real_t blue = house._attrib.colors[3*idx.vertex_index+2];
          min_point.x = min_point.x > vx ? vx : min_point.x;
          min_point.y = min_point.y > vy ? vy : min_point.y;
          min_point.z = min_point.z > vz ? vz : min_point.z;
          max_point.x = max_point.x < vx ? vx : max_point.x;
          max_point.y = max_point.y < vy ? vy : max_point.y;
          max_point.z = max_point.z < vz ? vz : max_point.z;
          if(v == 0) {
              v0 = glm::vec3(vx, vy, vz);
          } else if (v == 1) {
              v1 = glm::vec3(vx, vy, vz);
          } else if (v == 2) {
              v2 = glm::vec3(vx, vy, vz);
          }
        }
        step = (max_point - min_point) / glm::vec3(size_vok_x, size_vok_y, size_vok_z);
        //Triangle tmp(v0, v1, v2);
        triangle.push_back(Triangle(v0, v1, v2));
        index_offset += fv;

        // per-face material
        house._shapes[s].mesh.material_ids[f];
      }
    }
}

Scene::Scene()
{
    for (size_t s = 0; s < house._shapes.size(); s++) {
      // Loop over faces(polygon)
      size_t index_offset = 0;
      for (size_t f = 0; f < house._shapes[s].mesh.num_face_vertices.size(); f++) {
        int fv = house._shapes[s].mesh.num_face_vertices[f];
        // Loop over vertices in the face.
        glm::vec3 v0;
        glm::vec3 v1;
        glm::vec3 v2;
        for (size_t v = 0; v < fv; v++) {
          // access to vertex
          tinyobj::index_t idx = house._shapes[s].mesh.indices[index_offset + v];
          tinyobj::real_t vx = house._attrib.vertices[3*idx.vertex_index+0];
          tinyobj::real_t vy = house._attrib.vertices[3*idx.vertex_index+1];
          tinyobj::real_t vz = house._attrib.vertices[3*idx.vertex_index+2];
          tinyobj::real_t nx = house._attrib.normals[3*idx.normal_index+0];
          tinyobj::real_t ny = house._attrib.normals[3*idx.normal_index+1];
          tinyobj::real_t nz = house._attrib.normals[3*idx.normal_index+2];
          tinyobj::real_t tx = house._attrib.texcoords[2*idx.texcoord_index+0];
          tinyobj::real_t ty = house._attrib.texcoords[2*idx.texcoord_index+1];
          // Optional: vertex colors
          tinyobj::real_t red = house._attrib.colors[3*idx.vertex_index+0];
          tinyobj::real_t green = house._attrib.colors[3*idx.vertex_index+1];
          tinyobj::real_t blue = house._attrib.colors[3*idx.vertex_index+2];
          min_point.x = min_point.x > vx ? vx : min_point.x;
          min_point.y = min_point.y > vy ? vy : min_point.y;
          min_point.z = min_point.z > vz ? vz : min_point.z;
          max_point.x = max_point.x < vx ? vx : max_point.x;
          max_point.y = max_point.y < vy ? vy : max_point.y;
          max_point.z = max_point.z < vz ? vz : max_point.z;
          if(v == 0) {
              v0 = glm::vec3(vx, vy, vz);
          } else if (v == 1) {
              v1 = glm::vec3(vx, vy, vz);
          } else if (v == 2) {
              v2 = glm::vec3(vx, vy, vz);
          }
        }
        step = (max_point - min_point) / glm::vec3(size_vok_x, size_vok_y, size_vok_z);
        //Triangle tmp(v0, v1, v2);
        triangle.push_back(Triangle(v0, v1, v2));
        index_offset += fv;

        // per-face material
        house._shapes[s].mesh.material_ids[f];
      }
    }

    //std::cout << vert0.size() << std::endl;
}

