#include <string>
#include "cor.hh"

#if MACLINUX
	#include <GLUT/glut.h>
#else
	#include <GL/glut.h>
#endif

#ifndef _JANELA_HH_
#define _JANELA_HH_

class janela
{
public:
	// informação para tamanho da janela
	float altura;
	float largura;
	std::string titulo;

	// cor de fundo (mesmo que do círculo menor da pista)
	cor cores;

	// informação para posição da câmera
	float vx;
	float vy;

	janela();

	// prepara a câmera
	void inicia();

	// limpa a tela
	void limpa();

	// atualiza imagem da tela
	void atualizabuff();

	void atualizadisp();

	void redimensiona(int w, int h);

	float getcx();

	float getcy();
};

#endif
