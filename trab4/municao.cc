#include "municao.hh"

municao::municao() {}

municao::municao(float x, float y, float r, float a, float v, bool t) : circulo(x,y,r,1,0.2,0), angulo(a), velocidade(v), tiroinimigo(t) {}

void municao::draw()
{
	circulo::draw();
}

void municao::atualiza(GLdouble diff)
{
	cx += diff*velocidade*sin(angulo*M_PI/180.0);
	cy -= diff*velocidade*cos(angulo*M_PI/180.0);
}
