#include "retangulo.hh"
// #include <iostream>

retangulo::retangulo(int a, int l, int r, int g, int b) : altura(a), largura(l)
{
	fundo = cor(r,g,b);
}

retangulo::retangulo() : altura(0), largura(0)
{
	fundo = cor(0,0,0);
}