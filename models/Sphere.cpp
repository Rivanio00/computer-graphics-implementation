#include "Sphere.h"
#include <cmath>

bool Sphere::intersect(const Raio& raio, float tMin, float tMax, HitRecord& hit) const
{
    Vector3 oc = raio.origem - centro_;
    float   hb = oc.dot(raio.direcao);
    float   c  = oc.dot(oc) - raio_ * raio_;
    float   discriminante = hb * hb - c;

    if (discriminante < 0.f)
        return false;

    float raizDisc = std::sqrt(discriminante);

    // testa a raiz menor primeiro (ponto mais proximo, "de frente");
    // se ela cair fora do intervalo [tMin, tMax], tenta a raiz maior
    // (ponto "de tras" da esfera) antes de desistir
    float t = -hb - raizDisc;
    if (t < tMin || t > tMax)
    {
        t = -hb + raizDisc;
        if (t < tMin || t > tMax)
            return false;
    }

    hit.t        = t;
    hit.point    = raio.origem + raio.direcao * t;
    hit.normal   = (hit.point - centro_) / raio_;
    hit.material = &material_;

    return true;
}