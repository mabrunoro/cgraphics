#ifndef JANELA_HH
#define JANELA_HH

#include "retangulo.hh"

class janela : public retangulo
{
public:
	const char *titulo;

	janela(int a, int l, const char *t, int r, int g, int b);

	janela();

	void criar();
};

#endif