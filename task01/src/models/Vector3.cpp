#include "Vector3.h"

Vector3::Vector3():x(0),y(0),z(0){} // vetor nulo

Vector3::Vector3(float X,float Y,float Z):x(X),y(Y),z(Z){}

float Vector3::length() const{return std::sqrt(x*x+y*y+z*z);} // distancia entre pontos com dimensao = 3

Vector3 Vector3::normalized() const{ // normaliza o resultado da distancia, aproximando pra (0,0,0) caso a distancia seja minuscula
    float  l = length();
    return l<1e-8f?Vector3():*this/l;
}

float Vector3::dot(const Vector3&o) const{ // produto escalar
    return x*o.x+y*o.y+z*o.z;
}

Vector3 Vector3::cross(const Vector3&o) const{ // produto vetorial
    return {y*o.z-z*o.y,z*o.x-x*o.z,x*o.y-y*o.x};
}

// combinando vetores
Vector3 Vector3::operator+(const Vector3&o)const{return{x+o.x,y+o.y,z+o.z};}
Vector3 Vector3::operator-(const Vector3&o)const{return{x-o.x,y-o.y,z-o.z};}

Vector3 Vector3::operator-()const{return{-x,-y,-z};} // aponta o vetor para a diração oposta, mas com mesmo comprimento

// escalam o vetor, esticando ou comprimindo, podendo mudar a direção caso "s" seja negativo
Vector3 Vector3::operator*(float s)const{return{x*s,y*s,z*s};}
Vector3 Vector3::operator/(float s)const{return{x/s,y/s,z/s};}

Vector3 operator*(float s,const Vector3&v){return v*s;} 