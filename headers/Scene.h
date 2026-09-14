#pragma once
#include <memory>
#include <vector>

#include "Color.h"
#include "Object.h"
#include "PointLight.h"

// Agrupa tudo que existe na cena (objetos/fontes de luz)
struct Scene
{
    std::vector<std::shared_ptr<Object>> objetos;
    PointLight                           luzPontual;
    Color                                luzAmbiente; // I_A
};