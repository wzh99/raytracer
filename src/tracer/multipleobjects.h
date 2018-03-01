#pragma once

#include "core/tracer.h"

class MultipleObjects : public Tracer {
public:
    MultipleObjects(World *w_ptr) : Tracer(w_ptr) {}
    virtual vec3 traceRay(const Ray &ray) const;
};
