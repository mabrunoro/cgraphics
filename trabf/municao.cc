#include "municao.hh"

municao::municao() {}

municao::municao(float x, float y, float r, float a, float v, bool t) : circulo(x,y,r,0.5,0.5,0.5), angulo(a), velocidade(v), tiroinimigo(t) {}

void municao::draw(float dz)
{
	circulo::draw(1,1,1,dz);
}

void municao::atualiza(GLdouble diff)
{
	cx += diff*velocidade*sin(angulo*M_PI/180.0);
	cy -= diff*velocidade*cos(angulo*M_PI/180.0);
}
