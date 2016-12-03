#include "janela.hh"

janela::janela(int a, int l, const char *t, int r, int g, int b) : retangulo(a,l,r,g,b), titulo(t) {}

janela::janela() : retangulo(0,0,0,0,0), titulo(nullptr) {}