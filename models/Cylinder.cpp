#include "../headers/Cylinder.h"
#include <cmath>

Cylinder::Cylinder(Vector3 b, Vector3 d, double r, double h, Material m)
    : base_center(b), direction(d.normalized()), radius(r), height(h), material(m) {}

bool Cylinder::intersect(const Raio& raio, float tMin, float tMax, HitRecord& hit) const
{
    Vector3 v = raio.direcao;
    Vector3 o = raio.origem;
    Vector3 deltap = o - base_center;

    double v_dot_d = v.dot(direction);
    double dp_dot_d = deltap.dot(direction);

    Vector3 v_ortho = v - direction * v_dot_d;
    Vector3 dp_ortho = deltap - direction * dp_dot_d;

    double a = v_ortho.dot(v_ortho);
    double b = 2.0 * v_ortho.dot(dp_ortho);
    double c = dp_ortho.dot(dp_ortho) - radius * radius;

    double t_closest = -1.0;
    Vector3 normal_closest;

    // 1. Interseção com o corpo do cilindro
    double delta = b * b - 4 * a * c;
    if (delta >= 0.0 && std::abs(a) > 1e-8) {
        double sqrt_delta = std::sqrt(delta);
        double t1 = (-b - sqrt_delta) / (2 * a);
        double t2 = (-b + sqrt_delta) / (2 * a);

        for (double t : {t1, t2}) {
            if (t >= tMin && t <= tMax) {
                Vector3 p = o + v * t;
                double h_proj = (p - base_center).dot(direction);
                if (h_proj >= 0 && h_proj <= height) {
                    t_closest = t;
                    Vector3 proj_pt = base_center + direction * h_proj;
                    normal_closest = (p - proj_pt).normalized();
                    tMax = t;
                    break;
                }
            }
        }
    }
        

    // 2. Função auxiliar para interseção com a base e topo
    auto check_cap = [&](const Vector3& center, const Vector3& n) {
        double denom = v.dot(n);
        if (std::abs(denom) > 1e-8) {
            double t = (center - o).dot(n) / denom;
            if (t >= tMin && t <= tMax) { // <-- Restrição de profundidade corrigida
                Vector3 p = o + v * t;
                if ((p - center).dot(p - center) <= radius * radius) {
                    if (t_closest < 0 || t < t_closest) {
                        t_closest = t;
                        normal_closest = n;
                    }
                }
            }
        }
    };

    Vector3 top_center = base_center + direction * height;
    check_cap(base_center, direction * -1.0); // Tampa de baixo
    check_cap(top_center, direction);         // Tampa de cima

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

void Cylinder::applyTransform(const Matrix4& mat) {
    base_center = mat.transformPoint(base_center);
    direction = mat.transformVector(direction).normalized();
}