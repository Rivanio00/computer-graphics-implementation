#pragma once
#include "Vector3.h"

class Matrix4 {
public:
    double m[4][4];

    Matrix4();
    
    // Multiplicação de Matrizes
    Matrix4 operator*(const Matrix4& other) const;

    // Transformar Ponto (w = 1)
    Vector3 transformPoint(const Vector3& p) const;

    // Transformar Vetor (w = 0)
    Vector3 transformVector(const Vector3& v) const;

    // Utilitários de Transformação
    static Matrix4 translation(double tx, double ty, double tz);
    static Matrix4 scale(double sx, double sy, double sz);
    static Matrix4 rotationX(double angle);
    static Matrix4 rotationY(double angle);
    static Matrix4 rotationZ(double angle);
    
    // Construção da matriz World-to-Camera (M_wc)
    static Matrix4 buildWorldToCamera(const Vector3& eye, const Vector3& at, const Vector3& up);
};
