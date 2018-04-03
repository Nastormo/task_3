#pragma once

#include "glm/glm.hpp"
#include "vector"

struct Ray
{
  glm::vec3 _start;
  glm::vec3 _dir;
};

/*struct Camera
{
  glm::vec3 m_pos;          // Camera position and orientation
  glm::vec3 m_forward;      // Orthonormal basis
  glm::vec3 m_right;
  glm::vec3 m_up;

  glm::vec2 m_viewAngle;    // View angles, rad
  glm::uvec2 m_resolution;  // Image resolution: w, h

  std::vector<glm::vec3> m_pixels;  // Pixel array
};*/

/*struct Mesh
{
  std::vector<glm::vec3> _vertices;  // vertex positions
  std::vector<glm::uvec3> _triangles;  // vetrex indices
};*/
