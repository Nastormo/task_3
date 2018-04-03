#include "include/Camera.h"

Camera::Camera()
{
    _pos = glm::vec3(0, 25, 0);
    _forward = glm::vec3(0, -1, 0);
    _right = glm::vec3(0, 0, 1);
    _up = glm::vec3(1, 0, 0);

    _viewAngle = glm::vec2(80, 80);
    _resolution = glm::uvec2(500, 500);
}

Camera::Camera (glm::vec3 pos, glm::vec3 dir, glm::vec3 right, glm::vec3 up, glm::vec2 view) {
    _pos = pos;
    _forward = dir;
    _right = right;
    _up = up;
    _viewAngle = view;
}
