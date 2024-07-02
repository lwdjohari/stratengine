#include "stratengine/macro.h"
#include "glm/glm.hpp"

// cppcheck-suppress unknownMacro
STRATE_INNER_NAMESPACE(material)
STRATE_MAKE_NAMESPACE(v1)

struct Material{
    glm::vec3 ambient_color;
    glm::vec3 diffuse_color;
    float roughness;
    float ambient_strength;
    float diffuse_strength;
    float normal_strength;
    float lightmap_strength;


}

STRATE_END_NAMESPACE
STRATE_INNER_END_NAMESPACE