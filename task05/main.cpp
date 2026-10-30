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
#include "Material.h"
#include "Plane.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "Cone.h"
#include "Cube.h" // Novo
#include "Renderer.h"
#include "Scene.h"

void escreverPixel(std::vector<std::uint8_t>& buffer, int indicePixel, const Color& cor) {
    Color c = cor.clamped();
    buffer[indicePixel * 3 + 0] = static_cast<std::uint8_t>(255.f * c.r);
    buffer[indicePixel * 3 + 1] = static_cast<std::uint8_t>(255.f * c.g);
    buffer[indicePixel * 3 + 2] = static_cast<std::uint8_t>(255.f * c.b);
}

int main() {
    const float wJanela = 60.f;
    const float hJanela = 60.f;
    const float dJanela = 30.f;
    const Vector3 Eye(0.f, 0.f, 0.f);

    Scene cena;

    // Propriedade genérica de especularidade para todos os materiais
    float brilho = 10.f;

    // --- Planos ---
    // Chão (Textura de madeira pendente)
    Material matChao(Color(0.4f, 0.2f, 0.1f), Color(0.4f, 0.2f, 0.1f), Color(0.4f, 0.2f, 0.1f), brilho);
    cena.objetos.push_back(std::make_shared<Plane>(Vector3(0.f, -150.f, 0.f), Vector3(0.f, 1.f, 0.f), matChao));

    // Paredes
    Material matParede(Color(0.686f, 0.933f, 0.933f), Color(0.686f, 0.933f, 0.933f), Color(0.686f, 0.933f, 0.933f), brilho);
    cena.objetos.push_back(std::make_shared<Plane>(Vector3(200.f, -150.f, 0.f), Vector3(-1.f, 0.f, 0.f), matParede)); // Parede Direita
    cena.objetos.push_back(std::make_shared<Plane>(Vector3(200.f, -150.f, -400.f), Vector3(0.f, 0.f, 1.f), matParede)); // Parede Frontal
    cena.objetos.push_back(std::make_shared<Plane>(Vector3(-200.f, -150.f, 0.f), Vector3(1.f, 0.f, 0.f), matParede)); // Parede Esquerda

    // Teto
    Material matTeto(Color(0.933f, 0.933f, 0.933f), Color(0.933f, 0.933f, 0.933f), Color(0.933f, 0.933f, 0.933f), brilho);
    cena.objetos.push_back(std::make_shared<Plane>(Vector3(0.f, 150.f, 0.f), Vector3(0.f, -1.f, 0.f), matTeto));

    // --- Cilindro ---
    Material matCilindro(Color(0.824f, 0.706f, 0.549f), Color(0.824f, 0.706f, 0.549f), Color(0.824f, 0.706f, 0.549f), brilho);
    cena.objetos.push_back(std::make_shared<Cylinder>(Vector3(0.f, -150.f, -200.f), Vector3(0.f, 1.f, 0.f), 5.f, 90.f, matCilindro));

    // --- Cone ---
    Material matCone(Color(0.f, 1.f, 0.498f), Color(0.f, 1.f, 0.498f), Color(0.f, 1.f, 0.498f), brilho);
    cena.objetos.push_back(std::make_shared<Cone>(Vector3(0.f, -60.f, -200.f), Vector3(0.f, 1.f, 0.f), 90.f, 150.f, matCone));

    // --- Cubo ---
    Material matCubo(Color(1.f, 0.078f, 0.576f), Color(1.f, 0.078f, 0.576f), Color(1.f, 0.078f, 0.576f), brilho);
    cena.objetos.push_back(std::make_shared<Cube>(Vector3(0.f, -150.f, -165.f), 40.f, matCubo));

    // --- Esfera ---
    Material matEsfera(Color(0.854f, 0.647f, 0.125f), Color(0.854f, 0.647f, 0.125f), Color(0.854f, 0.647f, 0.125f), brilho);
    cena.objetos.push_back(std::make_shared<Sphere>(Vector3(0.f, 95.f, -200.f), 5.f, matEsfera));

    // --- Luzes ---
    cena.luzPontual = PointLight{ Vector3(-100.f, 140.f, -20.f), Color(0.7f, 0.7f, 0.7f) };
    cena.luzAmbiente = Color(0.3f, 0.3f, 0.3f);

    const Color bgColor(0.f, 0.f, 0.f);

    // --- Renderizacao ---
    const int nCol = 500;
    const int nLin = 500;
    const float Dx = wJanela / nCol;
    const float Dy = hJanela / nLin;

    Material matMadeiraClara(Color(0.5f, 0.3f, 0.15f), Color(0.5f, 0.3f, 0.15f), Color(0.5f, 0.3f, 0.15f), brilho);
    Material matMadeiraEscura(Color(0.3f, 0.15f, 0.05f), Color(0.3f, 0.15f, 0.05f), Color(0.3f, 0.15f, 0.05f), brilho);

    Renderer renderer;
    std::vector<std::uint8_t> buffer(nCol * nLin * 3);

    for (int l = 0; l < nLin; ++l) {
        float y = hJanela / 2.f - Dy / 2.f - l * Dy;
        for (int c = 0; c < nCol; ++c) {
            float x = -wJanela / 2.f + Dx / 2.f + c * Dx;
            Vector3 pontoJanela(x, y, -dJanela);
            Raio raio{ Eye, (pontoJanela - Eye).normalized() };

            HitRecord hit;
            Color corFinal = bgColor;

            if (renderer.closestHit(cena, raio, 0.001f, std::numeric_limits<float>::infinity(), hit)) {
                // HACK: Textura procedural de tacos de madeira para o chão
                // Se colidiu no chão (Normal apontando para cima e Y próximo a -150)
                if (hit.normal.y > 0.9f && std::abs(hit.point.y - (-150.f)) < 0.1f) {
                    
                    // Divide o chão em blocos de 20x20 cm
                    int x = static_cast<int>(std::floor(hit.point.x / 20.0f));
                    int z = static_cast<int>(std::floor(hit.point.z / 20.0f));
                    
                    // Alterna os materiais criando um xadrez com base na soma das coordenadas
                    if (std::abs(x + z) % 2 == 0) {
                        hit.material = &matMadeiraClara;
                    } else {
                        hit.material = &matMadeiraEscura;
                    }
                }

                corFinal = renderer.shade(cena, raio, hit);
            }

            escreverPixel(buffer, l * nCol + c, corFinal);
        }
    }

    const std::filesystem::path pastaSaida = "output";
    std::filesystem::create_directories(pastaSaida);
    const std::filesystem::path caminhoImagem = pastaSaida / "task05.jpg";

    stbi_write_jpg(caminhoImagem.string().c_str(), nCol, nLin, 3, buffer.data(), 90);
    std::cout << "Imagem salva em " << caminhoImagem.string() << " (" << nCol << "x" << nLin << ")\n";

    if (std::system(("xdg-open \"" + caminhoImagem.string() + "\" &").c_str())) {}

    return 0;
}