#include "Plane.h"
#include <cmath>

bool Plane::intersect(const Raio& raio, float tMin, float tMax, HitRecord& hit) const
{
    float denom = normal_.dot(raio.direcao);

    // raio (quase) paralelo ao plano -> não encontra
    if (std::fabs(denom) < 1e-6f)
        return false;

    float t = (ponto_ - raio.origem).dot(normal_) / denom;

    if (t < tMin || t > tMax)
        return false;

    hit.t        = t;
    hit.point    = raio.origem + raio.direcao * t;
    hit.normal   = normal_;
    hit.material = &material_;

    return true;
}

void Plane::applyTransform(const Matrix4& mat) {
    ponto_ = mat.transformPoint(ponto_);
    normal_ = mat.transformVector(normal_).normalized();
}