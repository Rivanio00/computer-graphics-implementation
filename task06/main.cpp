#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <limits>
#include <vector>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "Color.h"
#include "Vector3.h"
#include "Matrix4.h"

#include "Material.h"
#include "Plane.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "Cone.h"
#include "Cube.h"

#include "Renderer.h"
#include "Scene.h"

void escreverPixel(std::vector<std::uint8_t>& buffer, int indicePixel, const Color& cor) {
    Color c = cor.clamped();
    buffer[indicePixel * 3 + 0] = static_cast<std::uint8_t>(255.f * c.r);
    buffer[indicePixel * 3 + 1] = static_cast<std::uint8_t>(255.f * c.g);
    buffer[indicePixel * 3 + 2] = static_cast<std::uint8_t>(255.f * c.b);
}

int main() {
    // ---------------- 1. Parâmetros da Câmara ----------------
    const float wJanela = 60.f;
    const float hJanela = 60.f;
    const float dJanela = 30.f;
    
    // Câmara horizontal (Eye.y == At.y) para evitar a distorção da AABB do Cubo
    Vector3 Eye(0.f, -20.f, 220.f); 
    Vector3 At(0.f, -20.f, -200.f);  
    Vector3 Up(0.f, 1.f, 0.f);    

    Scene cena;
    float brilho = 10.f;

    // ---------------- 2. Montagem do Cenário (Coordenadas do Mundo) ----------------

    // --- Galpão ---
    Material matChao(Color(0.4f, 0.2f, 0.1f), Color(0.4f, 0.2f, 0.1f), Color(0.4f, 0.2f, 0.1f), brilho);
    cena.objetos.push_back(std::make_shared<Plane>(Vector3(0.f, -150.f, 0.f), Vector3(0.f, 1.f, 0.f), matChao));

    Material matParede(Color(0.686f, 0.933f, 0.933f), Color(0.686f, 0.933f, 0.933f), Color(0.686f, 0.933f, 0.933f), brilho);
    cena.objetos.push_back(std::make_shared<Plane>(Vector3(200.f, -150.f, 0.f), Vector3(-1.f, 0.f, 0.f), matParede));  // Direita
    cena.objetos.push_back(std::make_shared<Plane>(Vector3(200.f, -150.f, -400.f), Vector3(0.f, 0.f, 1.f), matParede)); // Frontal
    cena.objetos.push_back(std::make_shared<Plane>(Vector3(-200.f, -150.f, 0.f), Vector3(1.f, 0.f, 0.f), matParede)); // Esquerda

    Material matTeto(Color(0.933f, 0.933f, 0.933f), Color(0.933f, 0.933f, 0.933f), Color(0.933f, 0.933f, 0.933f), brilho);
    cena.objetos.push_back(std::make_shared<Plane>(Vector3(0.f, 150.f, 0.f), Vector3(0.f, -1.f, 0.f), matTeto));

    // --- Mesa no Centro ---
    Material matMesaTampo(Color(0.55f, 0.27f, 0.07f), Color(0.55f, 0.27f, 0.07f), Color(0.55f, 0.27f, 0.07f), brilho);
    
    // Tampo da mesa mais fino e elegante (Espessura = 5)
    auto tampoMesa = std::make_shared<Cube>(Vector3(0.f, -62.5f, -200.f), 1.0f, matMesaTampo); 
    tampoMesa->min_bounds = Vector3(-70.f, -65.f, -280.f); 
    tampoMesa->max_bounds = Vector3( 70.f, -60.f, -120.f);
    cena.objetos.push_back(tampoMesa);

    // Pés da mesa a ligar o chão (Y=-150) ao tampo (Y=-65)
    Material matPeMesa(Color(0.35f, 0.18f, 0.05f), Color(0.35f, 0.18f, 0.05f), Color(0.35f, 0.18f, 0.05f), brilho);
    cena.objetos.push_back(std::make_shared<Cylinder>(Vector3(-60.f, -150.f, -130.f), Vector3(0.f, 1.f, 0.f), 4.f, 85.f, matPeMesa));
    cena.objetos.push_back(std::make_shared<Cylinder>(Vector3( 60.f, -150.f, -130.f), Vector3(0.f, 1.f, 0.f), 4.f, 85.f, matPeMesa));
    cena.objetos.push_back(std::make_shared<Cylinder>(Vector3(-60.f, -150.f, -270.f), Vector3(0.f, 1.f, 0.f), 4.f, 85.f, matPeMesa));
    cena.objetos.push_back(std::make_shared<Cylinder>(Vector3( 60.f, -150.f, -270.f), Vector3(0.f, 1.f, 0.f), 4.f, 85.f, matPeMesa));

    // --- Árvore de Natal (Assente perfeitamente no topo da mesa Y = -60) ---
    Material matCilindroArvore(Color(0.5f, 0.35f, 0.2f), Color(0.5f, 0.35f, 0.2f), Color(0.5f, 0.35f, 0.2f), brilho);
    cena.objetos.push_back(std::make_shared<Cylinder>(Vector3(0.f, -60.f, -200.f), Vector3(0.f, 1.f, 0.f), 6.f, 40.f, matCilindroArvore));

    Material matConeArvore(Color(0.f, 0.6f, 0.3f), Color(0.f, 0.6f, 0.3f), Color(0.f, 0.6f, 0.3f), brilho);
    cena.objetos.push_back(std::make_shared<Cone>(Vector3(0.f, -20.f, -200.f), Vector3(0.f, 1.f, 0.f), 60.f, 120.f, matConeArvore));

    Material matEsfera(Color(1.0f, 0.84f, 0.0f), Color(1.0f, 0.84f, 0.0f), Color(1.0f, 0.84f, 0.0f), brilho);
    cena.objetos.push_back(std::make_shared<Sphere>(Vector3(0.f, 100.f, -200.f), 5.f, matEsfera));

    // --- Fontes de Luz ---
    cena.luzPontual = PointLight{ Vector3(-100.f, 140.f, -20.f), Color(0.7f, 0.7f, 0.7f) };
    cena.luzAmbiente = Color(0.3f, 0.3f, 0.3f);

    // ---------------- 3. Aplicação da Matriz World-to-Camera (M_wc) ----------------
    Matrix4 M_wc = Matrix4::buildWorldToCamera(Eye, At, Up);

    for (auto& obj : cena.objetos) {
        obj->applyTransform(M_wc);
    }

    cena.luzPontual.posicao = M_wc.transformPoint(cena.luzPontual.posicao);

    const Vector3 cameraOrigin(0.f, 0.f, 0.f);
    const Color bgColor(0.f, 0.f, 0.f);

    // ---------------- 4. Ray-Casting ----------------
    const int nCol = 500;
    const int nLin = 500;
    const float Dx = wJanela / nCol;
    const float Dy = hJanela / nLin;

    Renderer renderer;
    std::vector<std::uint8_t> buffer(nCol * nLin * 3);

    for (int l = 0; l < nLin; ++l) {
        float y = hJanela / 2.f - Dy / 2.f - l * Dy;

        for (int c = 0; c < nCol; ++c) {
            float x = -wJanela / 2.f + Dx / 2.f + c * Dx;

            Vector3 pontoJanela(x, y, -dJanela);
            Raio    raio{ cameraOrigin, (pontoJanela - cameraOrigin).normalized() };

            HitRecord hit;
            Color     corFinal = bgColor;

            if (renderer.closestHit(cena, raio, 0.001f, std::numeric_limits<float>::infinity(), hit)) {
                corFinal = renderer.shade(cena, raio, hit);
            }

            escreverPixel(buffer, l * nCol + c, corFinal);
        }
    }

    const std::filesystem::path pastaSaida = "output";
    std::filesystem::create_directories(pastaSaida);
    const std::filesystem::path caminhoImagem = pastaSaida / "task06.jpg";

    stbi_write_jpg(caminhoImagem.string().c_str(), nCol, nLin, 3, buffer.data(), 90);
    std::cout << "Imagem salva em " << caminhoImagem.string() << " (" << nCol << "x" << nLin << ")\n";

    if (std::system(("xdg-open \"" + caminhoImagem.string() + "\" &").c_str())) {}

    return 0;
}