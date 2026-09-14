#include "Renderer.h"
#include <algorithm>
#include <cmath>

bool Renderer::closestHit(const Scene& cena, const Raio& raio, float tMin, float tMax, HitRecord& hit) const
{
    bool  encontrou    = false;
    float maisProximo  = tMax;

    for (const auto& objeto : cena.objetos)
    {
        HitRecord hitAtual;
        if (objeto->intersect(raio, tMin, maisProximo, hitAtual))
        {
            encontrou   = true;
            maisProximo = hitAtual.t; // encolhe o intervalo
            hit         = hitAtual;
        }
    }

    return encontrou;
}

bool Renderer::isInShadow(const Scene& cena, const Vector3& ponto, const Vector3& normal, const PointLight& luz) const
{
    Vector3 paraLuz     = luz.posicao - ponto;
    float   distancia   = paraLuz.length();
    Vector3 direcaoLuz  = paraLuz / distancia;

    // desloca a origem um pouco na direcao da normal, pra nao achar
    // "intersecao" com o proprio objeto por erro de arredondamento
    Raio raioSombra{ ponto + normal * eps_, direcaoLuz };

    for (const auto& objeto : cena.objetos)
    {
        HitRecord hitSombra;
        if (objeto->intersect(raioSombra, eps_, distancia - eps_, hitSombra))
            return true; // algo esta no caminho ate a luz
    }

    return false;
}

Color Renderer::shade(const Scene& cena, const Raio& raio, const HitRecord& hit) const
{
    const Material& mat = *hit.material;
    Vector3         N   = hit.normal.normalized();

    // termo ambiente: sempre presente, mesmo na sombra
    Color cor = mat.ka * cena.luzAmbiente;

    if (!isInShadow(cena, hit.point, N, cena.luzPontual))
    {
        Vector3 L = (cena.luzPontual.posicao - hit.point).normalized();
        Vector3 V = (-raio.direcao).normalized();
        Vector3 R = (2.f * N.dot(L) * N - L).normalized();

        Color difusa    = (mat.kd * cena.luzPontual.intensidade) * std::max(0.f, N.dot(L));
        Color especular = (mat.ke * cena.luzPontual.intensidade)
                         * std::pow(std::max(0.f, V.dot(R)), mat.shininess);

        cor += difusa + especular;
    }

    return cor.clamped();
}