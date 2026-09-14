#pragma once
#include "Color.h"
#include "HitRecord.h"
#include "Raio.h"
#include "Scene.h"

class Renderer
{
public:
    // Acha, entre TODOS os objetos da cena, a intersecao de menor t dentro de [tMin, tMax]
    bool closestHit(const Scene& cena, const Raio& raio, float tMin, float tMax, HitRecord& hit) const;

    // Verifica se o ponto esta na sombra da fonte de luz
    bool isInShadow(const Scene& cena, const Vector3& ponto, const Vector3& normal, const PointLight& luz) const;

    // Calcula a cor final de um ponto de intersecao
    Color shade(const Scene& cena, const Raio& raio, const HitRecord& hit) const;

private:
    float eps_ = 0.001f;
};