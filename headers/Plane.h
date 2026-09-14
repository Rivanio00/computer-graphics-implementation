#pragma once
#include "Material.h"
#include "Object.h"
#include "Vector3.h"

class Plane : public Object
{
public:
    Plane(const Vector3& ponto, const Vector3& normal, const Material& material)
        : ponto_(ponto), normal_(normal.normalized()), material_(material)
    {
    }

    bool intersect(const Raio& raio, float tMin, float tMax, HitRecord& hit) const override;

private:
    Vector3  ponto_;   // um ponto qualquer conhecido do plano (P_pi)
    Vector3  normal_;  // normal unitaria do plano
    Material material_;
};