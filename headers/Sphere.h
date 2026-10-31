#pragma once
#include "Material.h"
#include "Object.h"
#include "Vector3.h"

class Sphere : public Object
{
public:
    Sphere(const Vector3& centro, float raio, const Material& material)
        : centro_(centro), raio_(raio), material_(material)
    {
    }

    bool intersect(const Raio& raio, float tMin, float tMax, HitRecord& hit) const override;

    void applyTransform(const Matrix4& mat) override;

private:
    Vector3  centro_;
    float    raio_;
    Material material_;
};