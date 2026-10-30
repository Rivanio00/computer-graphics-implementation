#pragma once
#include "Object.h"
#include "Vector3.h"
#include "Material.h"

class Cube : public Object {
public:
    Vector3 min_bounds;
    Vector3 max_bounds;
    Material material;

    // Construtor recebe o centro da base e o tamanho da aresta
    Cube(Vector3 centro_base, double aresta, Material m);
    bool intersect(const Raio& raio, float tMin, float tMax, HitRecord& hit) const override;
};