#pragma once
#include "HitRecord.h"
#include "Raio.h"
#include "Matrix4.h"

// qualquer objeto da cena tenha um metodo intersect() com a mesma assinatura
class Object
{
public:
    // destrutor virtual
    virtual ~Object() = default;

    // tMin/tMax delimitam o intervalo valido de "t" a considerar
    // Retorna true e preenche "hit" se houve intersecao valida nesse
    // intervalo; caso contrario, retorna false e "hit" fica inalterado.
    virtual bool intersect(const Raio& raio, float tMin, float tMax, HitRecord& hit) const = 0;

    virtual void applyTransform(const Matrix4& mat) = 0;
};