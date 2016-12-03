#ifndef RETANGULO_HH
#define RETANGULO_HH

#include "cor.hh"
#include <GLUT/glut.h>

class retangulo
{
public:
	int altura;
	int largura;
	cor fundo;

	retangulo(int a, int l, int r, int g, int b);

	retangulo();

	void criar(int gx, int gy);
};

#endif