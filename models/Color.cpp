#include "Color.h"

Color::Color():r(0),g(0),b(0){} // default == black
Color::Color(float R,float G,float B):r(R),g(G),b(B){}
Color Color::clamped()const{ // normaliza os valores no intervalo [0,1], evitando overflow, visto que as variaveis float nao tem limite definido
    return{std::clamp(r,0.f,1.f),std::clamp(g,0.f,1.f),std::clamp(b,0.f,1.f)};
}
Color  Color::operator+ (const Color&o)const{return{r+o.r,g+o.g,b+o.b};} // soma de cores utilizada para definir sombras
Color  Color::operator* (float s)      const{return{r*s,g*s,b*s};} // escurecer ou clarear a cor de forma uniforme
Color  Color::operator* (const Color&o)const{return{r*o.r,g*o.g,b*o.b};} // trata da reflexao da luz na superficie, dependendo da cor da fonte de luz
Color& Color::operator+=(const Color&o){ // não retorna uma cor, modifica o proprio objeto
    r+=o.r; g+=o.g; b+=o.b;
    return *this;
}