#include "retangulo.hh"

retangulo::retangulo(int a, int l, int r, int g, int b) : altura(a), largura(l)
{
	fundo = cor(r,g,b);
}

retangulo::retangulo() : altura(0), largura(0)
{
	fundo = cor(0,0,0);
}

void retangulo::criar(int x, int y)
{
	// Limpar todos os bits
    glClear(GL_COLOR_BUFFER_BIT);

    // Cores do retângulo
    glColor3f(fundo.red, fundo.green, fundo.blue);

    // Desenha o retângulo
    glBegin(GL_POLYGON);
        glVertex3f(
        	x-largura/2,
        	y-altura/2,
        	0.0);

        glVertex3f(
        	x+largura-largura/2,
        	y-altura/2,
        	0.0);

        glVertex3f(
        	x-largura/2,
        	y+altura-altura/2,
        	0.0);

        glVertex3f(
        	x+largura-largura/2,
        	y+altura-altura/2,
        	0.0);
    glEnd();
}