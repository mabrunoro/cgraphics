#include <iostream>
#include "cor.hh"

#if MACLINUX
	#include <GLUT/glut.h>
#else
	#include <GL/glut.h>
#endif

#ifndef _RETANGULO_HH_
#define _RETANGULO_HH_

class retangulo
{
public:
	float px;
	float py;
	float largura;
	float altura;
	cor cores;

	retangulo(float px, float py, float w, float h, float cr, float cg, float cb);

	retangulo();

	void draw();
};

#endif
