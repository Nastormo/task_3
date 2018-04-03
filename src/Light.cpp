#include "include/Light.h"

Light::Light () {
    position = glm::vec3(15,20,-10);
    //_pos = glm::vec3(40, -10, -10);
    color = glm::vec3(1,1,1);
}

Light::Light (glm::vec3 pos, glm::vec3 col) {
    position = pos;
    color = col;
}
