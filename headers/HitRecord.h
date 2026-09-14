#pragma once
#include "Material.h"
#include "Vector3.h"
 
// intersecao entre um raio e um objeto da cena
struct HitRecord
{
    float t = 0.f;              // distancia ao longo do raio ate a intersecao
    Vector3 point;               // PI: o ponto de intersecao em si
    Vector3 normal;               // N: normal da superficie nesse ponto
    const Material* material = nullptr; // material do objeto atingido
};