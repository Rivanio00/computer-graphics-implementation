#include <cstdint>
#include <filesystem> 
#include <iostream>
#include <vector>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "Color.h"
#include "Vector3.h"

struct Raio // semirreta partindo da origem
{
    Vector3 origem;
    Vector3 direcao;
};

struct Intersecao
{
    bool houveHit; // true se huve intersecao
    float t; // dsitancia
};

// verifica se um raio intersecta (ou tangencia) uma esfera
Intersecao intersectaEsfera(const Raio& raio, const Vector3& centroEsfera, float rEsfera)
{
    Vector3 oc = raio.origem - centroEsfera;
    float   hb = oc.dot(raio.direcao);
    float   c  = oc.dot(oc) - rEsfera * rEsfera;
    float   discriminante = hb * hb - c;

    if (discriminante < 0.f)
        return Intersecao{false, 0.f};

    float t = -hb - std::sqrt(discriminante);

    return Intersecao{true, t};
}

// escreve uma cor (canais em 0..1) diretamente no arquivo
void escreverPixel(std::vector<std::uint8_t>& buffer, int indicePixel, const Color& cor)
{
    Color c = cor.clamped();
    buffer[indicePixel * 3 + 0] = static_cast<std::uint8_t>(255.f * c.r);
    buffer[indicePixel * 3 + 1] = static_cast<std::uint8_t>(255.f * c.g);
    buffer[indicePixel * 3 + 2] = static_cast<std::uint8_t>(255.f * c.b);
}

Color calcularCor(const Vector3& PI, const Vector3& N, const Vector3& direcaoDoRaio, const Vector3& P_F, const Color& I_F, const Color& K, float m)
{
    Vector3 L = (P_F - PI).normalized();
    Vector3 V = -direcaoDoRaio;
    Vector3 R = (2.f * N.dot(L) * N - L).normalized();

    Color I_d = (I_F * K) * std::max(0.f, N.dot(L));
    Color I_e = (I_F * K) * std::pow(std::max(0.f, V.dot(R)), m);

    return (I_d + I_e).clamped();
}

int main()
{
    // dimensoes da janela
    const float   wJanela = 2.0f;
    const float   hJanela = 2.0f;
    const float   dJanela = 1.0f;

    const Vector3 Eye(0.f, 0.f, 0.f);   // posicao do olho do pintor

    const float   rEsfera = 1.0f;   // raio da esfera
    const Vector3 centroEsfera(0.f, 0.f, -(dJanela + rEsfera) - 1.0f);  // centro da esfera

    const Vector3 P_F(0.f,5.f,0.f); // posicao da fonte de luz

    const Color I_F(0.7f,0.7f,0.7f); // cor da luz
    const Color esfColor(255.f / 255.f, 0.f / 255.f, 0.f / 255.f); // cor da esfera
    const Color K = esfColor;   // reflectancia do material
    const float m = 32.0f;      // expoente especular
    const Color bgColor(100.f / 255.f, 100.f / 255.f, 100.f / 255.f); // cor do background

    // resolucao do canvas
    const int nCol = 500;
    const int nLin = 500;
    const float Dx = wJanela / nCol;
    const float Dy = hJanela / nLin;

    // renderizacao: preenche um buffer em memoria, pixel a pixel
    std::vector<std::uint8_t> buffer(nCol * nLin * 3);


    for (int l = 0; l < nLin; ++l) // iterando sobre as linhas
    {
        float y = hJanela / 2.f - Dy / 2.f - l * Dy; // calcula a coordenada y (vertical) dessa linha

        for (int c = 0; c < nCol; ++c) // iterando sobre as colunas
        {
            float x = -wJanela / 2.f + Dx / 2.f + c * Dx; /// calcula a coordenada x (horizontal) dessa coluna

            Vector3 pontoJanela(x, y, -dJanela); /// ponto no plano da janela correspondente ao centro deste pixel
            Raio    raio{ Eye, (pontoJanela - Eye).normalized() }; // monta o raio que parte do olho em direcao ao ponto da janela

            Intersecao hit = intersectaEsfera(raio, centroEsfera, rEsfera); // verifica se o raio tangencia ou intersecta a esfera
            int  indicePixel = l * nCol + c;
            Color corFinal = bgColor; // valor padrao, caso nao haja hit

            if (hit.houveHit)
            {
                Vector3 PI = raio.origem + raio.direcao * hit.t;
                Vector3 N  = (PI - centroEsfera) / rEsfera;

                corFinal = calcularCor(PI, N, raio.direcao, P_F, I_F, K, m);
            }

            escreverPixel(buffer, indicePixel, corFinal); // salva no arquivo o novo pixel
        }
    }

    // salva em uma pasta separada da fonte, criando-a se necessario
    const std::filesystem::path pastaSaida = "output";
    std::filesystem::create_directories(pastaSaida);
    const std::filesystem::path caminhoImagem = pastaSaida / "task02.jpg";


    // qualidade JPEG de 0 a 100; 90 e um bom equilibrio qualidade/tamanho
    const int qualidadeJpg = 90;
    stbi_write_jpg(caminhoImagem.string().c_str(), nCol, nLin, 3, buffer.data(), qualidadeJpg);
 
    std::cout << "Imagem salva em " << caminhoImagem.string()
            << " (" << nCol << "x" << nLin << ")\n";

    std::string comandoAbrir = "xdg-open \"" + caminhoImagem.string() + "\" &";
    std::system(comandoAbrir.c_str());
 
    return 0;
}