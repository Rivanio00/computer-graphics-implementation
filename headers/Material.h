#pragma once
#include "Color.h"
 
// Reflectividade de um objeto.
struct Material
{
    Color ka; // reflectividade do ambiente
    Color kd; // reflectiviade difusa
    Color ke; // reflectividade especular
    float shininess;
 
    Material()
        : ka(0.f, 0.f, 0.f), kd(0.f, 0.f, 0.f), ke(0.f, 0.f, 0.f), shininess(1.f)
    {
    }
 
    Material(const Color& a, const Color& d, const Color& e, float m)
        : ka(a), kd(d), ke(e), shininess(m)
    {
    }
};
