#pragma once

#include "utilities.h"
struct Shade;

class Material {
public:
    virtual vec3 getEmissiveLight(Shade &shade) { return vec3(); }
    virtual vec3 shade(Shade &shade) { return vec3(); }
    virtual vec3 areaLightShade(Shade &shade) { return vec3(); }
};