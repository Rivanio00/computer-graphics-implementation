#include "../headers/Matrix4.h"
#include <cmath>

Matrix4::Matrix4() {
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            m[i][j] = (i == j) ? 1.0 : 0.0;
}

Matrix4 Matrix4::operator*(const Matrix4& other) const {
    Matrix4 result;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            result.m[i][j] = 0;
            for (int k = 0; k < 4; ++k) {
                result.m[i][j] += m[i][k] * other.m[k][j];
            }
        }
    }
    return result;
}

Vector3 Matrix4::transformPoint(const Vector3& p) const {
    double x = m[0][0]*p.x + m[0][1]*p.y + m[0][2]*p.z + m[0][3];
    double y = m[1][0]*p.x + m[1][1]*p.y + m[1][2]*p.z + m[1][3];
    double z = m[2][0]*p.x + m[2][1]*p.y + m[2][2]*p.z + m[2][3];
    return Vector3(x, y, z);
}

Vector3 Matrix4::transformVector(const Vector3& v) const {
    double x = m[0][0]*v.x + m[0][1]*v.y + m[0][2]*v.z;
    double y = m[1][0]*v.x + m[1][1]*v.y + m[1][2]*v.z;
    double z = m[2][0]*v.x + m[2][1]*v.y + m[2][2]*v.z;
    return Vector3(x, y, z);
}

Matrix4 Matrix4::translation(double tx, double ty, double tz) {
    Matrix4 mat;
    mat.m[0][3] = tx;
    mat.m[1][3] = ty;
    mat.m[2][3] = tz;
    return mat;
}

Matrix4 Matrix4::scale(double sx, double sy, double sz) {
    Matrix4 mat;
    mat.m[0][0] = sx;
    mat.m[1][1] = sy;
    mat.m[2][2] = sz;
    return mat;
}

Matrix4 Matrix4::rotationX(double angle) {
    Matrix4 mat;
    double c = std::cos(angle);
    double s = std::sin(angle);
    mat.m[1][1] = c; mat.m[1][2] = -s;
    mat.m[2][1] = s; mat.m[2][2] = c;
    return mat;
}

Matrix4 Matrix4::rotationY(double angle) {
    Matrix4 mat;
    double c = std::cos(angle);
    double s = std::sin(angle);
    mat.m[0][0] = c; mat.m[0][2] = s;
    mat.m[2][0] = -s; mat.m[2][2] = c;
    return mat;
}

Matrix4 Matrix4::rotationZ(double angle) {
    Matrix4 mat;
    double c = std::cos(angle);
    double s = std::sin(angle);
    mat.m[0][0] = c; mat.m[0][1] = -s;
    mat.m[1][0] = s; mat.m[1][1] = c;
    return mat;
}

Matrix4 Matrix4::buildWorldToCamera(const Vector3& eye, const Vector3& at, const Vector3& up) {
    Vector3 w = (eye - at).normalized(); 
    Vector3 u = up.cross(w).normalized();
    Vector3 v = w.cross(u);

    Matrix4 R;
    R.m[0][0] = u.x; R.m[0][1] = u.y; R.m[0][2] = u.z; R.m[0][3] = 0;
    R.m[1][0] = v.x; R.m[1][1] = v.y; R.m[1][2] = v.z; R.m[1][3] = 0;
    R.m[2][0] = w.x; R.m[2][1] = w.y; R.m[2][2] = w.z; R.m[2][3] = 0;
    
    Matrix4 T = translation(-eye.x, -eye.y, -eye.z);

    return R * T;
}
