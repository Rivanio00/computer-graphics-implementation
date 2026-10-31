#pragma once
#include "Object.h"
#include "Vector3.h"
#include "Material.h"

class Cone : public Object {
public:
    Vector3 base_center;
    Vector3 direction;
    double radius;
    double height;
    Material material;

    Cone(Vector3 b, Vector3 d, double r, double h, Material m);
    bool intersect(const Raio& raio, float tMin, float tMax, HitRecord& hit) const override;

    void applyTransform(const Matrix4& mat) override;
};