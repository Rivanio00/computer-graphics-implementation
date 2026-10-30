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

#include "Renderer.h"
#include "Scene.h"

void escreverPixel(std::vector<std::uint8_t>& buffer, int indicePixel, const Color& cor)
{
    Color c = cor.clamped();
    buffer[indicePixel * 3 + 0] = static_cast<std::uint8_t>(255.f * c.r);
    buffer[indicePixel * 3 + 1] = static_cast<std::uint8_t>(255.f * c.g);
    buffer[indicePixel * 3 + 2] = static_cast<std::uint8_t>(255.f * c.b);
}

int main()
{
    // ---------------- Janela e olho ----------------
    const float   wJanela = 60.f;
    const float   hJanela = 60.f;
    const float   dJanela = 30.f;
    const Vector3 Eye(0.f, 0.f, 0.f);

    // ---------------- Cena ----------------
    Scene cena;

    const float rEsfera = 40.f;
    Material    matEsfera(Color(0.7f, 0.2f, 0.2f), Color(0.7f, 0.2f, 0.2f), Color(0.7f, 0.2f, 0.2f), 10.f);
    cena.objetos.push_back(std::make_shared<Sphere>(Vector3(0.f, 0.f, -100.f), rEsfera, matEsfera));

    Material matChao(Color(0.2f, 0.7f, 0.2f), Color(0.2f, 0.7f, 0.2f), Color(0.f, 0.f, 0.f), 1.f);
    cena.objetos.push_back(
        std::make_shared<Plane>(Vector3(0.f, -rEsfera, 0.f), Vector3(0.f, 1.f, 0.f), matChao));

    Material matFundo(Color(0.3f, 0.3f, 0.7f), Color(0.3f, 0.3f, 0.7f), Color(0.f, 0.f, 0.f), 1.f);
    cena.objetos.push_back(
        std::make_shared<Plane>(Vector3(0.f, 0.f, -200.f), Vector3(0.f, 0.f, 1.f), matFundo));

    cena.luzPontual  = PointLight{ Vector3(0.f, 60.f, -30.f), Color(0.7f, 0.7f, 0.7f) };
    cena.luzAmbiente = Color(0.3f, 0.3f, 0.3f);

    const Color bgColor(0.f, 0.f, 0.f); // fallback; nesta cena, todo raio deve acertar algo

    // ---------------- Resolucao do canvas ----------------
    const int   nCol = 500;
    const int   nLin = 500;
    const float Dx   = wJanela / nCol;
    const float Dy   = hJanela / nLin;

    Renderer                   renderer;
    std::vector<std::uint8_t> buffer(nCol * nLin * 3);

    for (int l = 0; l < nLin; ++l)
    {
        float y = hJanela / 2.f - Dy / 2.f - l * Dy;

        for (int c = 0; c < nCol; ++c)
        {
            float x = -wJanela / 2.f + Dx / 2.f + c * Dx;

            Vector3 pontoJanela(x, y, -dJanela);
            Raio    raio{ Eye, (pontoJanela - Eye).normalized() };

            HitRecord hit;
            Color     corFinal = bgColor;

            if (renderer.closestHit(cena, raio, 0.001f, std::numeric_limits<float>::infinity(), hit))
            {
                corFinal = renderer.shade(cena, raio, hit);
            }

            escreverPixel(buffer, l * nCol + c, corFinal);
        }
    }

    const std::filesystem::path pastaSaida = "output";
    std::filesystem::create_directories(pastaSaida);
    const std::filesystem::path caminhoImagem = pastaSaida / "task03.jpg";

    stbi_write_jpg(caminhoImagem.string().c_str(), nCol, nLin, 3, buffer.data(), 90);
    std::cout << "Imagem salva em " << caminhoImagem.string() << " (" << nCol << "x" << nLin << ")\n";

    std::string comandoAbrir = "xdg-open \"" + caminhoImagem.string() + "\" &";
    if (std::system(comandoAbrir.c_str())) {}

    return 0;
}