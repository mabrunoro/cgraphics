#ifndef MACLINUX
	#define MACLINUX false
#endif

#if MACLINUX
	#include <GLUT/glut.h>
#else
	#include <GL/glut.h>
#endif
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
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
#ifndef PI
	#define PI 3.1415926535897932384626433832795
#endif

// por padrão, modo debug é desligado
#ifndef DEBUG
	#define DEBUG false
#endif

// passada do círculo (velocidade)
#ifndef STEP
	#define STEP 1.0
#endif


/*		Funções, classes e variáveis					*/

// GLfloat gx, gy;
// GLfloat dx, dy;


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

	// void draw()
	// {
	//	 //static float angle;
	//	 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//	 glLoadIdentity();
	//	 glTranslatef(0, 0, -10);
	//	 int i, x, y;
	//	 double radius = 0.30;
	//	 glColor3ub(255, 0, 0);
	//	 double twicePi = 2.0 * 3.142;
	//	 x = 0, y = 0;
	//	 glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
	// 		glVertex2f(x, y); // center of circle
	// 		for (i = 0; i <= 20; i++)
	// 		{
	// 			glVertex2f (
	// 				(x + (radius * cos(i * twicePi / 20))), (y + (radius * sin(i * twicePi / 20))));
	// 		}
	//	 glEnd(); //END
	// }

	void draw()
	{
		// cor do círculo
		glColor3f(cores.red, cores.green, cores.blue);

		// desenha círculo
		glBegin(GL_POLYGON);
			//Change the 6 to 12 to increase the steps (number of drawn points) for a smoother circle
			//Note that anything above 24 will have little affect on the circles appearance
			//Value 1.5 - Draws Triangle
			//Value 2 - Draws Square
			//Value 3 - Draws Hexagon
			//Value 4 - Draws Octagon
			//Value 5 - Draws Decagon
			for(double i = 0; i < 2 * PI; i += PI / 24)
					glVertex3f(cx + (cos(i) * raio), cy + (sin(i) * raio), 0.0);
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
	}

	float rdistance(circulo c)
	{
		return (sqrt((c.cx-cx)*(c.cx-cx)+(c.cy-cy)*(c.cy-cy)) - c.raio - raio);
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
				px,
				py,
				0.0);

			glVertex3f(
				px,
				py+altura,
				0.0);

			glVertex3f(
				px+largura,
				py+altura,
				0.0);

			glVertex3f(
				px+largura,
				py,
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

/*---------------------------------------------------------------------*/
/*---------------------------------------------------------------------*/
janela			janarena;			// janela principal
retangulo		chegada;			// linha de chegada
circulo			jogador;			// circulo jogador
circulo			pistafora;			// circulo pista de fora
circulo			pistadentro;		// circulo da pista de dentro
vector<circulo>	inimigos;			// ponteiro do vetor com os inimigos
/*---------------------------------------------------------------------*/
map<char,bool>	keymap;
/*---------------------------------------------------------------------*/
/*---------------------------------------------------------------------*/


// Move para cima se possível (o eixo y é invertido)
void movw()
{
	if(keymap['s']) // se W e S estão pressionados, retorne
		return;
	// jogador.cy -= (STEP*1.1);
	int i;
	for(i = 0; i < 10; i++)
	{
		circulo n = circulo(jogador.cx,jogador.cy-STEP+i/10.0,jogador.raio,
			jogador.cores.red,jogador.cores.green,jogador.cores.blue);
		// jogador.cy += STEP*0.1;
		// testa distância com pista de dentro
		float aux = n.rdistance(pistadentro);
		if(aux <= 0)
		{
			// cout << "Erro fatal: jogador fora da pista (dentro)!" << endl;
			// jogador.cy -= STEP;
			// return;
			continue;
		}
		// if((aux < STEP) && (pistadentro.cy < jogador.cy)) // pára com distâncias menores que STEP
		// {
		// 	jogador.cy -= aux/2;
		// 	return;
		// }

		// testa distância com pista de fora
		aux = n.rdistance(pistafora);
		// if((aux > (-2*jogador.raio)) && (pistafora.cy > jogador.cy))
		if(aux >= (-2*n.raio))
		{
			// cout << "Erro fatal: jogador fora da pista (fora)!" << endl;
			// jogador.cy += STEP;
			// return;
			continue;
		}
		// if((aux > (-1)*(2*jogador.raio+STEP)) && (pistafora.cy > jogador.cy))
		// {
		// 	jogador.cy += (aux - (-1)*(2*jogador.raio+STEP))/2;
		// 	return;
		// }

		// testa distância com inimigos
		for(vector<circulo>::iterator it = inimigos.begin(); it != inimigos.end(); ++it)
		{
			aux = n.rdistance(*it);
			if(aux <= 0)
			{
				// cout << "Erro fatal: jogador dentro de inimigo!" << endl;
				// jogador.cy -= STEP;
				// return;
				goto LOOPW;
			}
			// if((aux < STEP) && (it->cy < jogador.cy)) // pára com distâncias menores que STEP
			// {
			// 	jogador.cy -= aux/2;
			// 	return;
			// }
		}
		// cout << "i: " << i << endl;
		jogador = n;
		return;
		LOOPW:;
	}
	// cout << "Erro loop W" << endl;
	// jogador.cy -= STEP;
}


// Move para baixo, se possível (o eixo y é invertido)
void movs()
{
	if(keymap['w']) // se W e S estão pressionados, retorne
		return;
	// jogador.cy += (STEP*1.1);
	int i;
	for(i = 0; i < 10; i++)
	{
		circulo n = circulo(jogador.cx,jogador.cy+STEP-i/10.0,jogador.raio,
			jogador.cores.red,jogador.cores.green,jogador.cores.blue);
		// jogador.cy -= STEP*0.1;
		// testa distância com pista de dentro
		float aux = n.rdistance(pistadentro);
		if(aux <= 0)
		{
			// cout << "Erro fatal: jogador fora da pista (dentro)!" << endl;
			// jogador.cy += STEP;
			// return;
			continue;
		}
		// if((aux < STEP) && (pistadentro.cy > jogador.cy)) // pára com distâncias menores que STEP
		// {
		// 	jogador.cy += aux/2;
		// 	return;
		// }

		// testa distância com pista de fora
		aux = n.rdistance(pistafora);
		// if((aux > (-2*jogador.raio)) && (pistafora.cy < jogador.cy))
		if(aux >= (-2*n.raio))
		{
			// cout << "Erro fatal: jogador fora da pista (fora)!" << endl;
			// jogador.cy -= STEP;
			// return;
			continue;
		}
		// if((aux > (-1)*(2*jogador.raio+STEP)) && (pistafora.cy < jogador.cy))
		// {
		// 	jogador.cy -= (aux - (-1)*(2*jogador.raio+STEP))/2;
		// 	return;
		// }

		// testa distância com inimigos
		for(vector<circulo>::iterator it = inimigos.begin(); it != inimigos.end(); ++it)
		{
			aux = n.rdistance(*it);
			if(aux <= 0)
			{
				// cout << "Erro fatal: jogador dentro de inimigo!" << endl;
				// jogador.cy += STEP;
				// return;
				goto LOOPS;
			}
			// if((aux < STEP) && (it->cy > jogador.cy)) // pára com distâncias menores que STEP
			// {
			// 	jogador.cy += aux/2;
			// 	return;
			// }
		}
		// cout << "i: " << i << endl;
		jogador = n;
		return;
		LOOPS:;
	}
	// cout << "Erro loop S" << endl;
	// jogador.cy += STEP;
}


// Move para direita, se possível
void movd()
{
	if(keymap['a']) // se A e D estão pressionados, retorne
		return;
	// jogador.cx += (STEP*1.1);
	int i;
	for(i = 0; i < 10; i++)
	{
		circulo n = circulo(jogador.cx+STEP-i/10.0,jogador.cy,jogador.raio,
			jogador.cores.red,jogador.cores.green,jogador.cores.blue);
		// jogador.cx -= STEP*0.1;
		// testa distância com pista de dentro
		float aux = n.rdistance(pistadentro);
		// cout << "aux1: " << aux << endl;
		if(aux <= 0)
		{
			// cout << "Erro fatal: jogador fora da pista (dentro)!" << endl;
			// jogador.cx += STEP;
			// return;
			continue;
		}
		// if((aux < STEP) && (pistadentro.cx > jogador.cx)) // pára com distâncias menores que STEP
		// {
		// 	jogador.cx += aux/2;
		// 	return;
		// }

		// testa distância com pista de fora
		aux = n.rdistance(pistafora);
		// cout << "aux2: " << aux << endl;
		// if((aux > (-2*jogador.raio)) && (pistafora.cx < jogador.cx))
		if(aux >= (-2*n.raio))
		{
			// cout << "Erro fatal: jogador fora da pista (fora)!" << endl;
			// jogador.cx -= STEP;
			// return;
			continue;
		}
		// if((aux > (-1)*(2*jogador.raio+STEP)) && (pistafora.cx < jogador.cx))
		// {
		// 	jogador.cx -= (aux - (-1)*(2*jogador.raio+STEP))/2;
		// 	return;
		// }

		// testa distância com inimigos
		for(vector<circulo>::iterator it = inimigos.begin(); it != inimigos.end(); ++it)
		{
			aux = n.rdistance(*it);
			// cout << "aux: " << aux << endl;
			if(aux <= 0)
			{
				// cout << "Erro fatal: jogador dentro de inimigo!" << endl;
				// jogador.cx += STEP;
				// return;
				goto LOOPD;
			}
			// if((aux < STEP) && (it->cx > jogador.cx)) // pára com distâncias menores que STEP
			// {
			// 	jogador.cx += aux/2;
			// 	return;
			// }
		}
		// cout << "i: " << i << endl;
		jogador = n;
		return;
		LOOPD:;
	}
	// cout << "Erro loop D" << endl;
	// jogador.cx += STEP;
}


// Move para esquerda, se possível
void mova()
{
	if(keymap['d']) // se A e D estão pressionados, retorne
		return;
	// jogador.cx -= (STEP*1.1);
	int i;
	for(i = 0; i < 10; i++)
	{
		circulo n = circulo(jogador.cx-STEP+i/10.0,jogador.cy,jogador.raio,
			jogador.cores.red,jogador.cores.green,jogador.cores.blue);
		// jogador.cx += STEP*0.1;
		// testa distância com pista de dentro
		float aux = n.rdistance(pistadentro);
		if(aux <= 0)
		{
			// cout << "Erro fatal: jogador fora da pista (dentro)!" << endl;
			// jogador.cx -= STEP;
			// return;
			continue;
		}
		// if((aux < STEP) && (pistadentro.cx < jogador.cx)) // pára com distâncias menores que STEP
		// {
		// 	jogador.cx -= aux/2;
		// 	return;
		// }

		// testa distância com pista de fora
		aux = n.rdistance(pistafora);
		// if((aux > (-2*jogador.raio)) && (pistafora.cx > jogador.cx))
		if(aux >= (-2*n.raio))
		{
			// cout << "Erro fatal: jogador fora da pista (fora)!" << endl;
			// jogador.cx += STEP;
			// return;
			continue;
		}
		// if((aux > (-1)*(2*jogador.raio+STEP)) && (pistafora.cx > jogador.cx))
		// {
		// 	jogador.cx += (aux - (-1)*(2*jogador.raio+STEP))/2;
		// 	return;
		// }

		// testa distância com inimigos
		for(vector<circulo>::iterator it = inimigos.begin(); it != inimigos.end(); ++it)
		{
			aux = n.rdistance(*it);
			if(aux <= 0)
			{
				// cout << "Erro fatal: jogador dentro de inimigo!" << endl;
				// jogador.cx -= STEP;
				// return;
				goto LOOPA;
			}
			// if((aux < STEP) && (it->cx < jogador.cx)) // pára com distâncias menores que STEP
			// {
			// 	jogador.cx -= aux/2;
			// 	return;
			// }
		}
		// cout << "i: " << i << endl;
		jogador = n;
		return;
		LOOPA:;
	}
	// cout << "Erro loop A" << endl;
	// jogador.cx -= STEP;
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
		// case 'v':
		// case 'V':
		// 	cout << jogador.rdistance(pistadentro) << '\t' << jogador.rdistance(pistafora) << endl;
		// 	break;
		case 27: // ESC
			exit(0);
			break;
		default:
			cout << "Botão pressionado: " << int(key) << endl;
	}
//	 glutPostRedisplay();
//	 cout << "Tecla down: " << key << endl;
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

	jogador.draw();

	janarena.atualizabuff();
}


void init(void)
{
	janarena.inicia();
}


void idle(void)
{
	if(keymap['w'])
	{
		movw();
		// jogador.cy += STEP;
	}
	if(keymap['s'])
	{
		movs();
		// jogador.cy -= STEP;
	}
	if(keymap['a'])
	{
		mova();
		// jogador.cx -= STEP;
	}
	if(keymap['d'])
	{
		movd();
		// jogador.cx += STEP;
	}
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


/*	Parte gráfica							*/

	glutInit(&argc,argv);

	init();

	glutKeyboardFunc(keypress);
	glutKeyboardUpFunc(keyup);

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
				jogador.cx = pelement->FloatAttribute("cx");
				jogador.cy = pelement->FloatAttribute("cy");
				jogador.raio = pelement->FloatAttribute("r");
				jogador.cores = cor(pelement->Attribute("fill"));
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