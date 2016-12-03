#include <cmath>
#include "circulo.hh"

#if MACLINUX
	#include <GLUT/glut.h>
#else
	#include <GL/glut.h>
#endif

#ifndef _MUNICAO_HH_
#define _MUNICAO_HH_

class municao : public circulo
{
public:
	float angulo;
	float velocidade;
	bool tiroinimigo;

	municao();

	municao(float x, float y, float r, float a, float v,bool t);

	void draw();

	void atualiza(GLdouble d);
};

#endif
