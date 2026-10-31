#include "../headers/Cube.h"
#include <cmath>
#include <algorithm>

Cube::Cube(Vector3 centro_base, double aresta, Material m) : material(m) {
    // Calculando os limites a partir do centro da base e da aresta
    double half_a = aresta / 2.0;
    min_bounds = Vector3(centro_base.x - half_a, centro_base.y, centro_base.z - half_a);
    max_bounds = Vector3(centro_base.x + half_a, centro_base.y + aresta, centro_base.z + half_a);
}

bool Cube::intersect(const Raio& raio, float tMin, float tMax, HitRecord& hit) const {
    double txmin = (min_bounds.x - raio.origem.x) / raio.direcao.x;
    double txmax = (max_bounds.x - raio.origem.x) / raio.direcao.x;
    if (txmin > txmax) std::swap(txmin, txmax);

    double tymin = (min_bounds.y - raio.origem.y) / raio.direcao.y;
    double tymax = (max_bounds.y - raio.origem.y) / raio.direcao.y;
    if (tymin > tymax) std::swap(tymin, tymax);

    if ((txmin > tymax) || (tymin > txmax)) return false;
    if (tymin > txmin) txmin = tymin;
    if (tymax < txmax) txmax = tymax;

    double tzmin = (min_bounds.z - raio.origem.z) / raio.direcao.z;
    double tzmax = (max_bounds.z - raio.origem.z) / raio.direcao.z;
    if (tzmin > tzmax) std::swap(tzmin, tzmax);

    if ((txmin > tzmax) || (tzmin > txmax)) return false;
    if (tzmin > txmin) txmin = tzmin;
    if (tzmax < txmax) txmax = tzmax;

    double t_closest = txmin;
    if (t_closest < tMin || t_closest > tMax) {
        t_closest = txmax;
        if (t_closest < tMin || t_closest > tMax) return false;
    }

    // Calcular normal com base na face atingida
    Vector3 p = raio.origem + raio.direcao * t_closest;
    Vector3 n;
    double epsilon = 1e-4;
    
    if (std::abs(p.x - min_bounds.x) < epsilon) n = Vector3(-1, 0, 0);
    else if (std::abs(p.x - max_bounds.x) < epsilon) n = Vector3(1, 0, 0);
    else if (std::abs(p.y - min_bounds.y) < epsilon) n = Vector3(0, -1, 0);
    else if (std::abs(p.y - max_bounds.y) < epsilon) n = Vector3(0, 1, 0);
    else if (std::abs(p.z - min_bounds.z) < epsilon) n = Vector3(0, 0, -1);
    else if (std::abs(p.z - max_bounds.z) < epsilon) n = Vector3(0, 0, 1);

    hit.t = t_closest;
    hit.point = p;
    hit.normal = n;
    hit.material = &material;
    return true;
}

void Cube::applyTransform(const Matrix4& mat) {
    // Obter os 8 vértices originais da caixa
    Vector3 v[8];
    v[0] = mat.transformPoint(Vector3(min_bounds.x, min_bounds.y, min_bounds.z));
    v[1] = mat.transformPoint(Vector3(max_bounds.x, min_bounds.y, min_bounds.z));
    v[2] = mat.transformPoint(Vector3(min_bounds.x, max_bounds.y, min_bounds.z));
    v[3] = mat.transformPoint(Vector3(max_bounds.x, max_bounds.y, min_bounds.z));
    v[4] = mat.transformPoint(Vector3(min_bounds.x, min_bounds.y, max_bounds.z));
    v[5] = mat.transformPoint(Vector3(max_bounds.x, min_bounds.y, max_bounds.z));
    v[6] = mat.transformPoint(Vector3(min_bounds.x, max_bounds.y, max_bounds.z));
    v[7] = mat.transformPoint(Vector3(max_bounds.x, max_bounds.y, max_bounds.z));

    // Recalcular a nova caixa delimitadora AABB no espaço da câmara
    min_bounds = v[0];
    max_bounds = v[0];
    
    for (int i = 1; i < 8; ++i) {
        min_bounds.x = std::min(min_bounds.x, v[i].x);
        min_bounds.y = std::min(min_bounds.y, v[i].y);
        min_bounds.z = std::min(min_bounds.z, v[i].z);

        max_bounds.x = std::max(max_bounds.x, v[i].x);
        max_bounds.y = std::max(max_bounds.y, v[i].y);
        max_bounds.z = std::max(max_bounds.z, v[i].z);
    }
}