#include <iostream>
#include <list>
#include "circulo.hh"
#include "retangulo.hh"
#include "elipse.hh"

#if MACLINUX
	#include <GLUT/glut.h>
#else
	#include <GL/glut.h>
#endif

#ifndef _CARRO_HH_
#define _CARRO_HH_

class carro : public circulo
{
	// Classe roda
	class roda
	{
	public:
		retangulo eixo;
		retangulo pneu;
		float largura;
		float altura;

		roda();

		roda(float raio, bool diresq); // true > roda direita | false > roda esquerda

		// vr = variável ranhura | va = variável ângulo
		void draw(float pran, float va, bool de);
	};

private:
	void atualizaran(int vr,GLdouble diff);

public:
	int id;
	// Carro
	float delta; // angulo entre o norte e a frente do carro
	float velocidade;

	retangulo chassi;
	circulo cockpit;

	roda rdd; // dianteira direita
	roda rde; // dianteira esquerda
	roda rtd; // traseira direita
	roda rte; // traseira esquerda
	float pran; // posição da ranhura
	float varoda; // ângulo da roda em relação ao carro

	retangulo cannon;
	int vacan; // ângulo do canhão em relação ao carro

	carro();

	carro(float r);

	carro(float x, float y, float ro, float var, float vran, float del, int vac, float vel);

// dir = direção do movimento (-1 trás, 0 parado, 1 frente)
// vac = variável ângulo canhão
// varoda = variável ângulo roda
	void draw(bool luz);

	int atualizadel(int dir,GLdouble diff);

	void mra();

	void mrd();

	// mover carro inimigo para frente (considera jogador)
	void movef(circulo pd,circulo pf,std::list<carro> im,GLdouble diff,carro jo);

	// move jogador para frente
	void movef(circulo pd,circulo pf,std::list<carro> im,GLdouble diff);

	void movet(circulo pd,circulo pf,std::list<carro> im,GLdouble diff);

	float pontax(float dx);

	float pontay(float dy);

	float pontan();

	float getcx();

	float getcy();

	float getchassix(int i);

	float getchassiy(int i);

	float get3rdpx(int v);

	float get3rdpy(int v);

	float getcanx();

	float getcany();
};

bool operator==(const carro& b,const carro& c);

#endif
