#include "../headers/Cone.h"
#include <cmath>

Cone::Cone(Vector3 b, Vector3 d, double r, double h, Material m)
    : base_center(b), direction(d.normalized()), radius(r), height(h), material(m) {}

bool Cone::intersect(const Raio& raio, float tMin, float tMax, HitRecord& hit) const
{
    Vector3 apex = base_center + direction * height; // Vértice (ponta) do cone
    Vector3 v = raio.direcao;
    Vector3 o = raio.origem;
    Vector3 deltap = o - apex;

    // Fator m = (R^2 / H^2)
    double m = (radius * radius) / (height * height);
    
    // Vetor do vértice para a base
    Vector3 D_cone = direction * -1.0; 

    double v_dot_D = v.dot(D_cone);
    double dp_dot_D = deltap.dot(D_cone);

    double a = v.dot(v) - (1.0 + m) * v_dot_D * v_dot_D;
    double b = 2.0 * (v.dot(deltap) - (1.0 + m) * v_dot_D * dp_dot_D);
    double c = deltap.dot(deltap) - (1.0 + m) * dp_dot_D * dp_dot_D;

    double t_closest = -1.0;
    Vector3 normal_closest;

    // 1. Interseção com o corpo do Cone
    double delta = b * b - 4 * a * c;
    if (delta >= 0.0) {
        double sqrt_delta = std::sqrt(delta);
        double t1 = (-b - sqrt_delta) / (2 * a);
        double t2 = (-b + sqrt_delta) / (2 * a);

        for (double t : {t1, t2}) {
            if (t >= tMin && t <= tMax) {
                Vector3 p = o + v * t;
                double proj = (p - apex).dot(D_cone);
                
                if (proj > 0 && proj <= height) {
                    if (t_closest < 0 || t < t_closest) {
                        t_closest = t;
                        Vector3 pt_to_apex = p - apex;
                        Vector3 n = pt_to_apex - D_cone * ((1.0 + m) * pt_to_apex.dot(D_cone));
                        normal_closest = n.normalized();
                        tMax = t; 
                    }
                }
            }
        }
    }

    // 2. Interseção com a base do cone
    double denom = v.dot(direction * -1.0);
    if (std::abs(denom) > 1e-8) {
        double t = (base_center - o).dot(direction * -1.0) / denom;
        if (t >= tMin && t <= tMax) { // <-- Restrição de profundidade corrigida
            Vector3 p = o + v * t;
            if ((p - base_center).dot(p - base_center) <= radius * radius) {
                if (t_closest < 0 || t < t_closest) {
                    t_closest = t;
                    normal_closest = direction * -1.0;
                }
            }
        }
    }

    // 3. Verifica se houve colisão válida
    if (t_closest > 0) {
        hit.t = t_closest;
        hit.point = o + v * t_closest;
        hit.normal = normal_closest;
        hit.material = &material;
        return true;
    }
    return false;
}

void Cone::applyTransform(const Matrix4& mat) {
    base_center = mat.transformPoint(base_center);
    direction = mat.transformVector(direction).normalized();
}