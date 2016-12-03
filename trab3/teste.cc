#ifndef MACLINUX
	#define MACLINUX false
#endif

// #if MACLINUX
// 	#include <GLUT/glut.h>
// #else
// 	#include <GL/glut.h>
// #endif
#include <GLUT/glut.h>
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;

// verifica se houver erro na leitura do XML ou SVG
#ifndef XMLCheckResult
	#define XMLCheckResult(a_eResult) if (a_eResult != XML_SUCCESS) { printf("Error: %i\n", a_eResult); return a_eResult; }
#endif

// typedef unsigned int uint;
#ifndef M_PI
	#define M_PI 3.1415926535897932384626433832795
#endif

// por padrão, modo debug é desligado
#ifndef DEBUG
	#define DEBUG false
#endif

// passada do círculo (velocidade)
#ifndef STEP
	#define STEP 1.0
#endif

#ifndef RANSPEED
	#define RANSPEED 0.05
#endif

#ifndef RAIOP
	#define RAIOP 22.0
#endif


/*		Funções, classes e variáveis					*/

int mousex;		// posição X do mouse
bool inicio;	// indica se o programa iniciou agora
float veltiro;	// velocidade do tiro
bool ifup;	// indica se o botão já foi solto ou não


int lerxml(char *argv);


class cor
{
public:
	float red;
	float green;
	float blue;

	cor(float r,float g,float b)
	: red(r), green(g), blue(b) {}

	cor() : cor(0,0,0) {}

	cor(const char* p)
	{
		string s = string(p);
		if(s == "black")
		{
			red = 0.0;
			green = 0.0;
			blue = 0.0;
		}
		else if(s == "red")
		{
			red = 1.0;
			green = 0.0;
			blue = 0.0;
		}
		else if(s == "green")
		{
			red = 0.0;
			green = 1.0;
			blue = 0.0;
		}
		else if(s == "blue")
		{
			red = 0.0;
			green = 0.0;
			blue = 1.0;
		}
		else if(s == "white")
		{
			red = 1.0;
			green = 1.0;
			blue = 1.0;
		}
		else if(s == "gray")
		{
			red = 0.3;
			green = 0.3;
			blue = 0.3;
		}
		else if(s == "yellow")
		{
			red = 1;
			green = 1;
			blue = 0.0;
		}
		else if(s == "cyan")
		{
			red = 0;
			green = 1;
			blue = 1;
		}
		else if(s == "magenta")
		{
			red = 1;
			green = 0;
			blue = 1;
		}
		else
		{
			cout << "Cor desconhecida: " << s << endl;
			red = 0.5;
			green = 0.5;
			blue = 0.0;
		}
	}
};

class circulo
{
public:
	float cx;
	float cy;
	float raio;
	cor cores;

	circulo(float cx, float cy, float raio, float cr, float cg, float cb)
	: cx(cx), cy(cy), raio(raio)
	{
		cores = cor(cr,cg,cb);
	}

	circulo() : cx(0), cy(0), raio(0)
	{
		cores = cor(0,0,0);
	}

	void draw()
	{
		glPushMatrix();
		// cor do círculo
		glColor3f(cores.red, cores.green, cores.blue);

		// desenha círculo
		glTranslatef(cx, cy, 0);
		glBegin(GL_POLYGON);
			//Change the 6 to 12 to increase the steps (number of drawn points) for a smoother circle
			//Note that anything above 24 will have little affect on the circles appearance
			//Value 1.5 - Draws Triangle
			//Value 2 - Draws Square
			//Value 3 - Draws Hexagon
			//Value 4 - Draws Octagon
			//Value 5 - Draws Decagon
			for(double i = 0; i < 2 * M_PI; i += M_PI / 24)
				glVertex3f((cos(i) * raio), (sin(i) * raio), 0.0);
		glEnd();

		#if DEBUG
			cout << "Debug círculo:" << endl << endl;
			cout << "\traio: " << raio << endl;
			cout << "\tcx: " << cx << endl;
			cout << "\tcy: " << cy << endl;
			cout << "\tcores: (" << cores.red << ','
				<< cores.green << ','
				<< cores.blue << ')' << endl;
		#endif
		glPopMatrix();
	}

	float rdistance(circulo c) const
	{
		return (sqrt((c.cx-cx)*(c.cx-cx)+(c.cy-cy)*(c.cy-cy)) - c.raio - raio);
	}

	bool colide(circulo pd, circulo pf, vector<circulo> ig) const
	{
		float aux = rdistance(pd);
		if(aux <= 0)
			return true;

		aux = rdistance(pf);
		if(aux >= -2*raio)
			return true;
		for(vector<circulo>::iterator it = ig.begin(); it != ig.end(); ++it)
		{
			aux = rdistance(*it);
			if(aux <= 0)
				return true;
		}
		// cout << "false" << endl;
		return false;
	}
};

class elipse : public circulo
{
public:
	float ry;

	elipse() {}

	elipse(float x, float y, float rx, float red, float gre, float blu, float ry) : circulo(x,y,rx,red,gre,blu), ry(ry) {}

	void draw()
	{
		glPushMatrix();
			float rx = raio;
			int num_segments = 30;
			float theta = 2 * 3.1415926 / float(num_segments);
			float c = cosf(theta);//precalculate the sine and cosine
			float s = sinf(theta);
			float t;
			float y = 1; //we start at angle = 90
			float x = 0;
			glColor3f(cores.red, cores.green, cores.blue);
			glBegin(GL_POLYGON);
			for(int ii = 0; ii < num_segments; ii++)
			{
				//apply radius and offset
				glVertex2f(x * rx + cx, y * ry + cy);//output vertex
				//apply the rotation matrix
				t = x;
				x = c * x - s * y;
				y = s * t + c * y;
			}
			glEnd();
		glPopMatrix();
	}
};

class retangulo
{
public:
	float px;
	float py;
	float largura;
	float altura;
	cor cores;

	retangulo(float px, float py, float w, float h, float cr, float cg, float cb)
	: px(px), py(py), largura(w), altura(h)
	{
		cores = cor(cr,cg,cb);
	}

	retangulo() : px(0), py(0), largura(0), altura(0)
	{
		cores = cor(0,0,0);
	}

	void draw()
	{
		// Cores do retângulo
		glColor3f(cores.red, cores.green, cores.blue);

		// Desenha o retângulo
		glBegin(GL_POLYGON);
			glVertex3f(
				px-largura/2,
				py-altura/2,
				0.0);

			glVertex3f(
				px-largura/2,
				py+altura/2,
				0.0);

			glVertex3f(
				px+largura/2,
				py+altura/2,
				0.0);

			glVertex3f(
				px+largura/2,
				py-altura/2,
				0.0);

		glEnd();

		#if DEBUG
			cout << "Debug retângulo:" << endl;
			cout << "\taltura: " << altura << endl;
			cout << "\tlargura: " << largura << endl;
			cout << "\tpx: " << px << endl;
			cout << "\tpy: " << py << endl;
			cout << "\tcores: (" << cores.red << ','
				<< cores.green << ','
				<< cores.blue << ')' << endl;
		#endif
	}
};

class janela
{
public:
	// informação para tamanho da janela
	float altura;
	float largura;
	string titulo;

	// cor de fundo (mesmo que do círculo menor da pista)
	cor cores;

	// informação para posição da câmera
	float vx;
	float vy;

	janela()
	: altura(0.0), largura(0.0), vx(0.0), vy(0.0) {}

	// prepara a câmera
	void inicia()
	{
		// Buffer duplo e RGB
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

		// Janela
		glutInitWindowSize(largura,altura);
		glutInitWindowPosition(100,100);
		glutCreateWindow(titulo.c_str());

		// Cor do fundo
		glClearColor(cores.red,cores.green,cores.blue,0.0);

		// Visão
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(vx,vx+largura,vy+altura,vy,-1.0,1.0);

		#if DEBUG
			cout << "Debug Janela:" << endl;
			cout << "\taltura: " << altura << endl;
			cout << "\tlargura: " << largura << endl;
			cout << "\tvx: " << vx << endl;
			cout << "\tvy: " << vy << endl;
			cout << "\ttitulo: " << titulo << endl;
		#endif
	}

	// limpa a tela
	void limpa()
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}

	// atualiza imagem da tela
	void atualizabuff()
	{
		glutSwapBuffers();
	}

	void atualizadisp()
	{
		glutPostRedisplay();
	}
};


class municao : public circulo
{
public:
	float angulo;
	float velocidade;

	municao() {}

	municao(float x, float y, float r, float a, float v) : circulo(x,y,r,1,0.2,0), angulo(a), velocidade(v) {}

	void draw()
	{
		circulo::draw();
	}

	void atualiza()
	{
		cx += sin(angulo*M_PI/180.0);
		cy -= cos(angulo*M_PI/180.0);
	}
};


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

		roda() {}

		roda(float raio, bool diresq) // true > roda direita | false > roda esquerda
		{
			largura = 16*raio/RAIOP; // cada pneu tem largura/2 de largura
			altura = 12*raio/RAIOP;
			if(diresq)
			{
				eixo = retangulo(-3.5*raio/RAIOP,0,largura/2,raio/(RAIOP/3),0.5,0.5,0.5);
				pneu = retangulo(0,0,largura/2,altura,0.3,0.3,0.3);
			}
			else
			{
				eixo = retangulo(3.5*raio/RAIOP,0,largura/2,raio/(RAIOP/3),0.5,0.5,0.5);
				pneu = retangulo(0,0,largura/2,altura,0.3,0.3,0.3);
			}
		}

		// vr = variável ranhura | va = variável ângulo
		void draw(float pran, float va)
		{
			glPushMatrix();
				eixo.draw();
				glRotatef(va, 0, 0, 1);
				pneu.draw();
				// desenha as ranhuras
				glLineWidth(1);
				glColor3f(1,1,1);
				if(pran >= 0)
				{
					glBegin(GL_LINES);
						glVertex3f(-largura/4,-altura/2+pran*altura,0);
						glVertex3f(largura/4,-altura/2+pran*altura,0);
					glEnd();
				}
				// glBegin(GL_LINES);
				// 	glVertex3f(-largura/4,-altura/4+pran*altura,0);
				// 	glVertex3f(largura/4,-altura/4+pran*altura,0);
				// glEnd();
				glBegin(GL_LINES);
					glVertex3f(-largura/4,pran*altura,0);
					glVertex3f(largura/4,pran*altura,0);
				glEnd();
				// glBegin(GL_LINES);
				// 	glVertex3f(-largura/4,altura/4+pran*altura,0);
				// 	glVertex3f(largura/4,altura/4+pran*altura,0);
				// glEnd();
				if(pran <= 0)
				{
					glBegin(GL_LINES);
						glVertex3f(-largura/4,altura/2+pran*altura,0);
						glVertex3f(largura/4,altura/2+pran*altura,0);
					glEnd();
				}
			glPopMatrix();
		}
	};

private:
	void atualizaran(int vr)
	{
		pran += vr*RANSPEED*velocidade;
		// cout << pran << ' ' << vr << endl;
		if((pran >= 0.5) || (pran <= -0.5))
			pran = 0;
	}

public:
	// Carro
	float delta; // angulo entre o norte e a frente do carro
	float velocidade;

	retangulo chassi;
	elipse cockpit;

	roda rdd; // dianteira direita
	roda rde; // dianteira esquerda
	roda rtd; // traseira direita
	roda rte; // traseira esquerda
	float pran; // posição da ranhura
	float varoda; // ângulo da roda em relação ao carro

	retangulo cannon;
	int vacan; // ângulo do canhão em relação ao carro

	carro() {}

	carro(float r) : circulo(0,0,r,0,0,0), delta(0), pran(0.005), varoda(0), vacan(0), velocidade(0)
	{
		chassi = retangulo(0,0,24*r/RAIOP,42*r/RAIOP,1,0,0);
		cockpit = elipse(0, 0, chassi.largura/4, 0.3, 0.3, 0, chassi.altura/4);
		cannon = retangulo(0,0,4*r/RAIOP,13*r/RAIOP,0,1,0);
		rdd = roda(r,true);
		rde = roda(r,false);
		rtd = roda(r,true);
		rte = roda(r,false);
	}

	carro(float x, float y, float ro, float var, float vran, float del, int vac, float vel) : circulo(x,y,ro,0,0,0), varoda(var), pran(vran), delta(del), vacan(vac), velocidade(vel)
	{
		chassi = retangulo(0,0,24*ro/RAIOP,42*ro/RAIOP,1,0,0);
		cockpit = elipse(0, 0, chassi.largura/4, 0.3, 0.3, 0, chassi.altura/4);
		cannon = retangulo(0,0,4*ro/RAIOP,13*ro/RAIOP,0,1,0);
		rdd = roda(ro,true);
		rde = roda(ro,false);
		rtd = roda(ro,true);
		rte = roda(ro,false);
	}

// dir = direção do movimento (-1 trás, 0 parado, 1 frente)
// vac = variável ângulo canhão
// varoda = variável ângulo roda
	void draw(int dir)
	{
		// cout << varoda << endl;
		atualizaran(dir);
		glPushMatrix();
			glTranslatef(cx, cy, 0);
			glRotatef(delta, 0, 0, 1);
			glPushMatrix();
				glTranslatef(0, -chassi.altura/2, 0);
				glRotatef(vacan, 0, 0, 1);
				glTranslatef(0, -cannon.altura/4, 0);
				cannon.draw();
			glPopMatrix();

			glPushMatrix();
				glTranslatef(chassi.largura/2+rdd.largura/2-raio/(RAIOP/2), -chassi.altura*0.4, 0);
				rdd.draw(pran,varoda);
			glPopMatrix();

			glPushMatrix();
				glTranslatef(-chassi.largura/2-rdd.largura/2+raio/(RAIOP/2), -chassi.altura*0.4, 0);
				rde.draw(pran,varoda);
			glPopMatrix();

			glPushMatrix();
				glTranslatef(chassi.largura/2+rdd.largura/2-raio/(RAIOP/2), chassi.altura*0.4, 0);
				rtd.draw(pran,0);
			glPopMatrix();

			glPushMatrix();
				glTranslatef(-chassi.largura/2-rdd.largura/2+raio/(RAIOP/2), chassi.altura*0.4, 0);
				rte.draw(pran,0);
			glPopMatrix();

			chassi.draw();
			cockpit.draw();
		glPopMatrix();
	}

	int atualizadel(int dir)
	{
		if((varoda == 0) || (dir == 0))
			return delta;
		else if(varoda >= 45)
		{
			if(dir > 0)
				varoda = 45;
			else
				varoda -= velocidade;
			// cout << 'a' << varoda << endl;
			delta -= dir*velocidade;
			return delta;
		}
		else if(varoda <= -45)
		{
			if(dir > 0)
				varoda = -45;
			else
				varoda += velocidade;
			// cout << 'b' << varoda << endl;
			delta += dir*velocidade;
			return delta;
		}
		else if(varoda < 0)
		{
			if(varoda >= -1)
				varoda = 0;
			else
				varoda -= dir*velocidade;
			// cout << 'c' << varoda << endl;
			delta += dir*velocidade;
			return delta;
		}
		else if(varoda > 0)
		{
			if(varoda <= 1)
				varoda = 0;
			else
				varoda += dir*velocidade;
			// cout << 'd' << varoda << endl;
			delta -= dir*velocidade;
			return delta;
		}
		else
		{
			cout << "Erro atualizadel()" << endl;
			return delta = 0;
		}
	}

	void mra()
	{
		if(varoda <= -45)
			return;
		varoda -= 2;
	}

	void mrd()
	{
		if(varoda >= 45)
			return;
		varoda += 2;
	}
};

/*---------------------------------------------------------------------*/
/*---------------------------------------------------------------------*/
janela			janarena;			// janela principal
retangulo		chegada;			// linha de chegada
carro			jogador;			// circulo jogador
circulo			pistafora;			// circulo pista de fora
circulo			pistadentro;		// circulo da pista de dentro
vector<circulo>	inimigos;			// ponteiro do vetor com os inimigos
list<municao>	tiros;				// vetor com os tiros atuais
/*---------------------------------------------------------------------*/
map<char,bool>	keymap;
/*---------------------------------------------------------------------*/
/*---------------------------------------------------------------------*/


// Move para cima se possível (o eixo y é invertido)
void movw()
{
	if(keymap['s']) // se W e S estão pressionados, retorne
		return;
	jogador.atualizadel(-1);
	int i;
	for(i = 0; i < 10; i++)
	{
		carro n = carro(jogador.cx+sin(jogador.delta*M_PI/180.0)*(jogador.velocidade+i/10.0),jogador.cy-cos(jogador.delta*M_PI/180.0)*(jogador.velocidade+i/10.0),jogador.raio, jogador.varoda,jogador.pran,jogador.delta,jogador.vacan,jogador.velocidade);

		// testa distância com pista de dentro
		// float aux = n.rdistance(pistadentro);
		// if(aux <= 0)
		// {
		// 	continue;
		// }
		//
		// // testa distância com pista de fora
		// aux = n.rdistance(pistafora);
		// if(aux >= (-2*n.raio))
		// {
		// 	continue;
		// }
		//
		// // testa distância com inimigos
		// for(vector<circulo>::iterator it = inimigos.begin(); it != inimigos.end(); ++it)
		// {
		// 	aux = n.rdistance(*it);
		// 	if(aux <= 0)
		// 	{
		// 		goto LOOPW;
		// 	}
		// }
		if(n.colide(pistadentro,pistafora,inimigos))
			continue;
		jogador = n;
		return;
		LOOPW:;
	}
}


// Move para baixo, se possível (o eixo y é invertido)
void movs()
{
	if(keymap['w']) // se W e S estão pressionados, retorne
		return;
	jogador.atualizadel(1);
	int i;
	for(i = 0; i < 10; i++)
	{
		carro n = carro(jogador.cx-sin(jogador.delta*M_PI/180.0)*(jogador.velocidade+i/10.0),jogador.cy+cos(jogador.delta*M_PI/180.0)*(jogador.velocidade+i/10.0),jogador.raio, jogador.varoda,jogador.pran,jogador.delta,jogador.vacan,jogador.velocidade);
		// testa distância com pista de dentro
		// float aux = n.rdistance(pistadentro);
		// if(aux <= 0)
		// {
		// 	continue;
		// }
		//
		// // testa distância com pista de fora
		// aux = n.rdistance(pistafora);
		// if(aux >= (-2*n.raio))
		// {
		// 	continue;
		// }
		//
		// // testa distância com inimigos
		// for(vector<circulo>::iterator it = inimigos.begin(); it != inimigos.end(); ++it)
		// {
		// 	aux = n.rdistance(*it);
		// 	if(aux <= 0)
		// 	{
		// 		goto LOOPS;
		// 	}
		// }
		if(n.colide(pistadentro, pistafora, inimigos))
			continue;
		jogador = n;
		return;
		LOOPS:;
	}
}


// Move para direita, se possível
void movd()
{
	if(keymap['a']) // se A e D estão pressionados, retorne
		return;
	jogador.mrd();
}


// Move para esquerda, se possível
void mova()
{
	if(keymap['d']) // se A e D estão pressionados, retorne
		return;
	jogador.mra();
}


void keyup(unsigned char key, int x, int y)
{
	switch(key)
	{
		case 'w':
		case 'W':
			keymap['w'] = false;
			break;
		case 's':
		case 'S':
			keymap['s'] = false;
			break;
		case 'a':
		case 'A':
			keymap['a'] = false;
			break;
		case 'd':
		case 'D':
			keymap['d'] = false;
			break;
	}
}


void keypress(unsigned char key, int x, int y)
{
	switch(key)
	{
		case 'w':
		case 'W':
			keymap['w'] = true;
			break;
		case 's':
		case 'S':
			keymap['s'] = true;
			break;
		case 'a':
		case 'A':
			keymap['a'] = true;
			break;
		case 'd':
		case 'D':
			keymap['d'] = true;
			break;
		case 27: // ESC
			exit(0);
			break;
		default:
			cout << "Botão pressionado: " << int(key) << endl;
	}
}


void mousepos(int x, int y)
{
	if(inicio)
	{
		mousex = ((((x*3)/10)*10)/3);
		inicio = false;
		return;
	}
	int i = x - mousex;
	// cout << i << endl;
	i = (i*3)/10;
	mousex += (i*10)/3;
	if((jogador.vacan >= 45) && (i > 0))
	{
		jogador.vacan = 45;
		return;
	}
	else if((jogador.vacan <= -45) && (i < 0))
	{
		jogador.vacan = -45;
		return;
	}
	else
		jogador.vacan += i;
}

void mouseclick(int butt, int st, int x, int y)
{
	if((butt == GLUT_LEFT_BUTTON) && (st == GLUT_DOWN) && (ifup))
	{
		float dx = (jogador.chassi.altura/2)*sin(jogador.delta*M_PI/180.0) + jogador.cannon.altura*sin((jogador.vacan+jogador.delta)*M_PI/180.0);
		float dy = (jogador.chassi.altura/2)*cos(jogador.delta*M_PI/180.0) + jogador.cannon.altura*cos((jogador.vacan+jogador.delta)*M_PI/180.0);
		municao a = municao(jogador.cx + dx, jogador.cy - dy, 2, jogador.delta+jogador.vacan, veltiro);
		tiros.push_back(a);
		ifup = false;
	}
	else if((butt == GLUT_LEFT_BUTTON) && (st == GLUT_UP))
		ifup = true;
}


void display(void)
{
	janarena.limpa();

	// desenha os itens estáticos
	pistafora.draw();
	pistadentro.draw();
	chegada.draw();

	for(vector<circulo>::iterator it = inimigos.begin(); it != inimigos.end(); ++it)
		it->draw();

	for(list<municao>::iterator it = tiros.begin(); it != tiros.end(); ++it)
		it->draw();

	if(keymap['w'] xor keymap['s'])
	{
		if(keymap['w'])
			jogador.draw(-1); // anda pra frente
		else
			jogador.draw(1); // anda pra trás
	}
	else
		jogador.draw(0);

	janarena.atualizabuff();
}


void init(void)
{
	janarena.inicia();
}


bool limpatiros(const municao &v)
{
	// cout << "limpou tiros" << endl;
	return v.colide(pistadentro,pistafora,inimigos);
}

void idle(void)
{
	if(keymap['w'])
		movw();

	if(keymap['s'])
		movs();

	if(keymap['a'])
		mova();

	if(keymap['d'])
		movd();

	for(list<municao>::iterator it = tiros.begin(); it != tiros.end(); ++it)
		it->atualiza();
	tiros.remove_if(limpatiros);

	janarena.atualizadisp();
}




/*		Main								*/

int main(int argc, char** argv)
{
	if(argc < 2)
	{
		std::cout << "Arquivo config.xml não fornecido."
		<< std::endl << "Saindo..." << std::endl;
		return 0;
	}

	int r = lerxml(argv[1]);
	if(r != 0)
		return r;

	#if DEBUG
		cout << "Modo DEBUG ativado." << endl;
	#endif


	keymap['a'] = false;
	keymap['w'] = false;
	keymap['s'] = false;
	keymap['d'] = false;

	mousex = 0;
	inicio = true;
	ifup = true;


/*	Parte gráfica							*/

	glutInit(&argc,argv);

	init();

	glutKeyboardFunc(keypress);
	glutKeyboardUpFunc(keyup);
	glutPassiveMotionFunc(mousepos);

	glutMouseFunc(mouseclick);

	glutDisplayFunc(display);
	glutIdleFunc(idle);

	glutMainLoop();

	return 0;
}


/*---------------------------------------------------------------------*/
/*---------------------------------------------------------------------*/
/*---------------------------------------------------------------------*/
/*---------------------------------------------------------------------*/
/*---------------------------------------------------------------------*/
/*---------------------------------------------------------------------*/
/*---------------------------------------------------------------------*/


/*		Código para leitura do XML			*/


int lerxml(char *argv)
{

	string config = string(argv) + "config.xml";


	XMLDocument xmldoc;
	XMLError eresult = xmldoc.LoadFile(config.c_str());
	XMLCheckResult(eresult);	// Imprime mensagem de erro se houver algo de errado


	XMLNode *proot = xmldoc.FirstChild();
	if(proot == nullptr)
	{
		cout << "Erro de leitura: config.xml" << endl;
		return XML_ERROR_FILE_READ_ERROR;
	}


/*		Código para parsing de arquivoDaArena	*/


	XMLElement *pelement = proot->FirstChildElement("arquivoDaArena");
	if(pelement == nullptr)
	{
		cout << "Erro parsing arquivoDaArena" << std::endl;
		return XML_ERROR_PARSING_ELEMENT;
	}

	string arena;

	const char* p = pelement->Attribute("caminho");
	if(p[0] == '~')
	{
		const char *hd;
		if((hd = getenv("HOME")) == nullptr)
		{
			hd = getpwuid(getuid())->pw_dir;
		}
		arena = string(hd)+(p+1);
	}
	else
		arena = string(p);

	p = pelement->Attribute("nome");
	arena = arena + p;

	p = pelement->Attribute("tipo");
	arena = arena + '.' + p;

	// cout << arena << endl;

	pelement = proot->FirstChildElement("carro");
	if(pelement == nullptr)
	{
		cout << "Erro parsing carro" << std::endl;
		return XML_ERROR_PARSING_ELEMENT;
	}
	float velcarro = pelement->FloatAttribute("velCarro");
	veltiro = pelement->FloatAttribute("velTiro");

/*		Abre arquivo da arena					*/


	eresult = xmldoc.LoadFile(arena.c_str());
	XMLCheckResult(eresult);	// Imprime mensagem de erro se houver algo de errado

	proot = xmldoc.FirstChild();
	if(proot == nullptr)
	{
		cout << "Erro de leitura: " << arena << endl;
		return XML_ERROR_FILE_READ_ERROR;
	}

	string nome;
	vector<circulo> vaux;

	for(pelement = proot->FirstChildElement(); pelement != nullptr; pelement = pelement->NextSiblingElement())
	{
		if(pelement == nullptr)
		{
			cout << "Erro parsing primeiro" << std::endl;
			return XML_ERROR_PARSING_ELEMENT;
		}

		nome = string(pelement->Value());

		if(nome == "circle")
		{
			nome = string(pelement->Attribute("id"));
			if(nome == "Inimigo")
			{
				circulo aux;
				aux.cx = pelement->FloatAttribute("cx");
				aux.cy = pelement->FloatAttribute("cy");
				aux.raio = pelement->FloatAttribute("r");
				aux.cores = cor(pelement->Attribute("fill"));

				inimigos.push_back(aux);
			}
			else if(nome == "Jogador")
			{
				float x = pelement->FloatAttribute("cx");
				float y = pelement->FloatAttribute("cy");
				float r = pelement->FloatAttribute("r");
				// cor c = cor(pelement->Attribute("fill"));
				jogador = carro(x,y,r,0,0,0,0,velcarro);
				// circulo a = circulo(x, y, r, c.red, c.green, c.blue);
				// inimigos.push_back(a);
			}
			else if(nome == "Pista")
			{
				circulo aux;
				aux.cx = pelement->FloatAttribute("cx");
				aux.cy = pelement->FloatAttribute("cy");
				aux.raio = pelement->FloatAttribute("r");
				aux.cores = cor(pelement->Attribute("fill"));

				vaux.push_back(aux);
				// string aux = string(pelement->Attribute("fill"));
				// if( aux == "blue") // Pista de fora
				// {
				// 	pistafora.cx = pelement->FloatAttribute("cx");
				// 	pistafora.cy = pelement->FloatAttribute("cy");
				// 	pistafora.raio = pelement->FloatAttribute("r");
				// 	pistafora.cores = cor(aux.c_str());
				// }
				// else// if( aux == "white") // Pista de dentro
				// {
				// 	pistadentro.cx = pelement->FloatAttribute("cx");
				// 	pistadentro.cy = pelement->FloatAttribute("cy");
				// 	pistadentro.raio = pelement->FloatAttribute("r");
				// 	pistadentro.cores = cor(aux.c_str());
				// }

				// else
				// {
				// 	cout << "Cor de pista desconhecido (nem azul, nem branco)" << endl;
				// }
			}
			else
			{
				cout << "Círculo desconhecido: "
					<< nome << endl;
				circulo aux;
				aux.cx = pelement->FloatAttribute("cx");
				aux.cy = pelement->FloatAttribute("cy");
				aux.raio = pelement->FloatAttribute("r");
				aux.cores = cor("cinza");

				inimigos.push_back(aux);
			}
		}
		else if(nome == "rect")
		{
			nome = string(pelement->Attribute("id"));
			if(nome == "LargadaChegada")
			{
				chegada.px = pelement->FloatAttribute("x");
				chegada.py = pelement->FloatAttribute("y");
				chegada.largura = pelement->FloatAttribute("width");
				chegada.altura = pelement->FloatAttribute("height");
				chegada.cores = cor(pelement->Attribute("fill"));
				chegada.px += chegada.largura/2;
				chegada.py += chegada.altura/2;
			}
			else
			{
				cout << "Retângulo desconhecido: "
					<< nome << endl;
				// chegada.px = pelement->FloatAttribute("x");
				// chegada.py = pelement->FloatAttribute("y");
				// chegada.largura = pelement->FloatAttribute("width");
				// chegada.altura = pelement->FloatAttribute("height");
				// chegada.cores = cor(pelement->Attribute("fill"));
			}
		}
		else
		{
			cout << "Tipo desconhecido (nem círculo, nem retângulo): "
				<< nome << endl;
		}
	}

	// Deve haver dois círculos no vetor vaux
	if(vaux[0].raio > vaux[1].raio)
	{
		pistafora = vaux[0];
		pistadentro = vaux[1];
	}
	else
	{
		pistafora = vaux[1];
		pistadentro = vaux[0];
	}

	// Cria objeto da janela
	janarena.altura = 2*pistafora.raio;
	janarena.largura = janarena.altura;
	janarena.titulo = "Trabalho CG 2";
	janarena.vx = pistafora.cx - pistafora.raio;
	janarena.vy = pistafora.cx - pistafora.raio;
	janarena.cores = pistadentro.cores;

	return 0;
}
