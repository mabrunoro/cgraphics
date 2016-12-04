// por padrão, modo debug é desligado
#ifndef DEBUG
	#define DEBUG false
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
#include <list>
#include <map>
#include <chrono>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include "tinyxml2.h"
#include "cor.hh"
#include "circulo.hh"
#include "elipse.hh"
#include "retangulo.hh"
#include "janela.hh"
#include "municao.hh"
#include "carro.hh"

using namespace std;
using namespace chrono;
using namespace tinyxml2;

// verifica se houver erro na leitura do XML ou SVG
#ifndef XMLCheckResult
	#define XMLCheckResult(a_eResult) if (a_eResult != XML_SUCCESS) { printf("Error: %i\n", a_eResult); return a_eResult; }
#endif

typedef high_resolution_clock clk;


/*		Funções e variáveis					*/

int lerxml(char *argv);


/*---------------------------------------------------------------------*/
/*---------------------------------------------------------------------*/
janela				janarena;			// janela principal
retangulo			chegada;			// linha de chegada
carro					jogador;			// circulo jogador
circulo				pistafora;		// circulo pista de fora
circulo				pistadentro;	// circulo da pista de dentro
list<carro>		inimigos;			// ponteiro do vetor com os inimigos
list<municao>	tiros;				// vetor com os tiros atuais
/*---------------------------------------------------------------------*/
map<char,bool>	keymap;				// valores booleanos para teclado
clk::time_point		t1;				// tempo 1
clk::time_point		t2;				// tempo 2
static char str[100];				// string do cronômetro
void *font = GLUT_BITMAP_9_BY_15;	// fonte do texto do cronômetro
int mousex;							// posição X do mouse
bool inicio;						// indica se o programa iniciou agora
float veltiroj;						// velocidade do tiro
float veltiroi;						// velocidade do tiro inimigo
float freqtiroi;					// freqüência do tiro inimigo
bool ifup;							// indica se o botão já foi solto ou não
int id;								// id para os carros
bool jogando;						// indica se o jogador está jogando
bool ganhou;						// indica se o jogador venceu
int laps;							// número de voltas do jogador
int tipocamera;				// tipo da câmera utilizada
bool movecam3;				// indica se é para mover o ângulo da câmera 3
float camxy;
float camxz;
float lastcamx;
float lastcamy;
/*---------------------------------------------------------------------*/
/*---------------------------------------------------------------------*/

void encerrajogo(bool i)
{
	jogando = false;
	ganhou = i;
}

// Move para cima se possível (o eixo y é invertido)
void movw(GLdouble d)
{
	if(keymap['s']) // se W e S estão pressionados, retorne
		return;
	int y = floor(jogador.cy);
	jogador.movef(pistadentro,pistafora,inimigos,d);
	if((floor(jogador.cy) < floor(chegada.py+chegada.altura/2))
		&& (y >= floor(chegada.py+chegada.altura/2))
		&& (jogador.cx > pistadentro.cx))
		laps++;
	if((floor(jogador.cy) > floor(chegada.py+chegada.altura/2))
		&& (y <= floor(chegada.py+chegada.altura/2))
		&& (jogador.cx > pistadentro.cx))
		laps--;
}


// Move para baixo, se possível (o eixo y é invertido)
void movs(GLdouble d)
{
	if(keymap['w']) // se W e S estão pressionados, retorne
		return;
	float y = floor(jogador.cy);
	jogador.movet(pistadentro,pistafora,inimigos,d);
	if((floor(jogador.cy) < floor(chegada.py+chegada.altura/2))
		&& (y >= floor(chegada.py+chegada.altura/2))
		&& (jogador.cx > pistadentro.cx))
		laps++;
	if((floor(jogador.cy) > floor(chegada.py+chegada.altura/2))
		&& (y <= floor(chegada.py+chegada.altura/2))
		&& (jogador.cx > pistadentro.cx))
		laps--;
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
		case '1':
			tipocamera = 1;
			break;
		case '2':
			tipocamera = 2;
			break;
		case '3':
			tipocamera = 3;
			break;
		case '4':
			tipocamera = 4;
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
		jogador.vacan = 45;
	else if((jogador.vacan <= -45) && (i < 0))
		jogador.vacan = -45;
	else
		jogador.vacan += i;
}

void mousecam(int x, int y)
{
	if(!movecam3)
	return;

	camxy += x - lastcamx;
	camxz += y - lastcamy;

	if(camxy > 180)
	camxy = 180;
	else if(camxy < -180)
	camxy = -180;

	if(camxz > 90)
	camxz = 90;
	else if(camxz < -90)
	camxz = -90;

	lastcamx = x;
	lastcamy = y;
}

void mouseclick(int butt, int st, int x, int y)
{
	if((butt == GLUT_LEFT_BUTTON) && (st == GLUT_DOWN) && (ifup))
	{
		float dx = jogador.pontax(0);
		float dy = jogador.pontay(0);
		float an = jogador.pontan();

		municao a = municao(dx, dy, RTIRO, an, veltiroj,false);
		tiros.push_back(a);
		ifup = false;
	}
	else if((butt == GLUT_LEFT_BUTTON) && (st == GLUT_UP))
		ifup = true;

	if((butt == GLUT_RIGHT_BUTTON) && (st == GLUT_DOWN))
	{
		movecam3 = true;
		lastcamx = x;
		lastcamy = y;
	}
	else if((butt == GLUT_RIGHT_BUTTON) && (st == GLUT_UP))
	{
		movecam3 = false;
		// lastcamx += jogador.delta;
	}

	// cout << '\t' << movecam3 << endl;
}


void printcrono(GLfloat x, GLfloat y)
{
	char *tmp;
	int i = TGAME - duration_cast<seconds>(t2 - t1).count();
	if((i <= 0) || (!jogando))
	{
		if((jogando) && (i <= 0))
			encerrajogo(false);
		else if(ganhou)
			sprintf(str,"You win!");
		else
			sprintf(str,"You just lost the game!");
	}
	else
		sprintf(str,"Tempo restante: %d",i);
	tmp = str;
	glDisable(GL_LIGHTING);
	glPushMatrix();
		glColor3f(0,0,0);
		glRasterPos2f(x,y);
		while(*tmp)
			glutBitmapCharacter(font,*tmp++);
	glPopMatrix();
	glEnable(GL_LIGHTING);
}

void RasterChars(GLfloat x, GLfloat y, GLfloat z, const char * text, double r, double g, double b)
{
	//Push to recover original attributes
	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	//Draw text in the x, y, z position
	glColor3f(r,g,b);
	glRasterPos3f(x, y, z);
	const char* tmpStr;
	tmpStr = text;
	while( *tmpStr ){
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *tmpStr);
		tmpStr++;
	}
	glPopAttrib();
}

void PrintText(GLfloat x, GLfloat y, double r, double g, double b)
{
	char *tmp;
	int i = TGAME - duration_cast<seconds>(t2 - t1).count();
	if((i <= 0) || (!jogando))
	{
		if((jogando) && (i <= 0))
			encerrajogo(false);
		else if(ganhou)
			sprintf(str,"You win!");
		else
			sprintf(str,"You just lost the game!");
	}
	else
		sprintf(str,"Tempo restante: %d",i);
	tmp = str;
	//Draw text considering a 2D space (disable all 3d features)
	glMatrixMode (GL_PROJECTION);
	//Push to recover original PROJECTION MATRIX
	glPushMatrix();
	glLoadIdentity ();
	glOrtho (0, 1, 0, 1, -1, 1);
	RasterChars(x, y, 0, tmp, r, g, b);
	glPopMatrix();
	glMatrixMode (GL_MODELVIEW);
}


void display(void)
{
	janarena.limpa();
	glViewport(0,janarena.altura-200,janarena.largura,200);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45,1.0,1,600);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(jogador.getchassix(-1), jogador.getchassiy(-1), -30,
						jogador.get3rdpx(4), jogador.get3rdpy(4),-40,0,0,-1);

	GLfloat light_position[] = { pistadentro.cx, pistadentro.cy, -200, 1.0 };
	GLfloat light_direction[] = { 0, 0, 1 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	pistafora.pistdraw(true);
	pistadentro.pistdraw(false);

	for(list<carro>::iterator it = inimigos.begin(); it != inimigos.end(); ++it)
		it->draw();

	for(list<municao>::iterator it = tiros.begin(); it != tiros.end(); ++it)
		it->draw(2*jogador.cannon.profundidade);

	jogador.draw();

	chegada.draw(1);

	// cout << janarena.altura << ' ' << janarena.largura << endl;

	glViewport(0,0,janarena.largura,janarena.altura-200);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if(tipocamera == 4)
		glOrtho(-janarena.largura/2,janarena.largura/2,-janarena.altura/2,janarena.altura/2,-100,100);
		// gluPerspective(90, 1.0, 1, 500);
	else
		gluPerspective(90, 1.0, 1, 600);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	PrintText(0.05, 0.05, 1,0.5,0);
	switch(tipocamera)
	{
		case 1:
			gluLookAt(jogador.getchassix(1), jogador.getchassiy(1), -25,
								jogador.getcanx(), jogador.getcany(), -24, 0,0,-1);
			break;

		case 2:
			gluLookAt(jogador.pontax(0),jogador.pontay(0), -2.5*jogador.cannon.profundidade,
								jogador.pontax(4),jogador.pontay(4), -2.5*jogador.cannon.profundidade, 0,0,-1);
			break;

		case 3:
			gluLookAt(jogador.get3rdpx(4), jogador.get3rdpy(4), -80,
							jogador.getcx(), jogador.getcy(), -60, 0,0,-1);
			glTranslatef(jogador.cx,jogador.cy,0);
			glRotatef(camxy,0,0,1);
			// glRotatef(camxz,1,0,0);
			glTranslatef(-jogador.cx,-jogador.cy,0);
			break;

		case 4:
			gluLookAt(janarena.getcx(), janarena.getcy(), -100,
								janarena.getcx(), janarena.getcy(), 100, 0,-1,0);
			break;
	}

	// GLfloat light_position[] = { pistadentro.cx, pistadentro.cy, -100, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	// desenha os itens
	// glDisable(GL_LIGHTING);

	pistafora.pistdraw(true);
	pistadentro.pistdraw(false);


	for(list<carro>::iterator it = inimigos.begin(); it != inimigos.end(); ++it)
		it->draw();

	for(list<municao>::iterator it = tiros.begin(); it != tiros.end(); ++it)
		it->draw(2*jogador.cannon.profundidade);

	jogador.draw();

	chegada.draw(1);

	// printcrono(pistafora.cx + pistafora.raio/4, pistafora.cy - pistafora.raio + 10);
	// glEnable(GL_LIGHTING);

	janarena.atualizabuff();
	// cout << "Lap: " << laps << endl;
}


void init(void)
{
	janarena.inicia();
}


bool limpatiros(const municao &v)
{
	int j;
	bool i = v.colide<carro>(pistadentro,pistafora,inimigos,jogador,v.tiroinimigo,&j);
	if(i)
	{
		// cout << j << endl;
		if((j == jogador.id) && v.tiroinimigo)
			encerrajogo(false);
		else if(!v.tiroinimigo)
		{
			for(list<carro>::iterator it = inimigos.begin(); it != inimigos.end(); ++it)
			{
				if(it->id == j)
				{
					// cout << "vai remover" << endl;
					inimigos.remove(*it);
					break;
				}
			}
		}
	}
	return i;
}

void idle(void)
{
	static GLdouble previous = 0;
	static float vtiro = 0.0;
	GLdouble current;
	GLdouble difference;

	if(!jogando)
	{
		janarena.atualizadisp();
		return;
	}

	if(laps > 0)
	{
		encerrajogo(true);
		janarena.atualizadisp();
		return;
	}

	current = glutGet(GLUT_ELAPSED_TIME);
	difference = current - previous;
	previous = current;

	if(keymap['w'])
		movw(difference);

	if(keymap['s'])
		movs(difference);

	if(keymap['a'])
		mova();

	if(keymap['d'])
		movd();

	for(list<municao>::iterator it = tiros.begin(); it != tiros.end(); ++it)
		it->atualiza(difference);
	tiros.remove_if(limpatiros);

	for(list<carro>::iterator it = inimigos.begin(); it != inimigos.end(); ++it)
	{
		it->mra();
		it->movef(pistadentro,pistafora,inimigos,difference,jogador);
	}


	vtiro += freqtiroi*difference;
	if(vtiro >= 1.0)
	{
		for(list<carro>::iterator it = inimigos.begin(); it != inimigos.end(); ++it)
		{
			float dx = it->pontax(0);
			float dy = it->pontay(0);
			float an = it->pontan();

			municao a = municao(dx, dy, RTIRO, an, veltiroi,true);
			tiros.push_back(a);
		}
		vtiro--;
	}

	t2 = clk::now();

	janarena.atualizadisp();
}

void reshape(int w, int h)
{
	janarena.redimensiona(w,h);
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
	id = 0;
	jogando = true;
	ganhou = false;
	laps = 0;
	tipocamera = 1;
	movecam3 = false;

	// cout << "Jogador: " << jogador.id << endl;
	// for(list<carro>::iterator it = inimigos.begin(); it != inimigos.end(); ++it)
	// 	cout << "Inimigo: " << it->id << endl;


/*	Parte gráfica							*/

	glutInit(&argc,argv);

	init();

	glutKeyboardFunc(keypress);
	glutKeyboardUpFunc(keyup);
	glutPassiveMotionFunc(mousepos);
	glutMotionFunc(mousecam);

	glutMouseFunc(mouseclick);

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	glutIdleFunc(idle);

	t2 = clk::now();
	t1 = clk::now();

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

//--------------------------------------------------

	pelement = proot->FirstChildElement("carro");
	if(pelement == nullptr)
	{
		cout << "Erro parsing carro" << std::endl;
		return XML_ERROR_PARSING_ELEMENT;
	}
	float velcarroj = pelement->FloatAttribute("velCarro");
	veltiroj = pelement->FloatAttribute("velTiro");

//--------------------------------------------------

	pelement = proot->FirstChildElement("carroInimigo");
	if(pelement == nullptr)
	{
		cout << "Erro parsing carroInimigo" << std::endl;
		return XML_ERROR_PARSING_ELEMENT;
	}
	freqtiroi = pelement->FloatAttribute("freqTiro");
	float velcarroi = pelement->FloatAttribute("velCarro");
	veltiroi = pelement->FloatAttribute("velTiro");

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
				float x = pelement->FloatAttribute("cx");
				float y = pelement->FloatAttribute("cy");
				float r = pelement->FloatAttribute("r");
				carro ig = carro(x, y, r, 0, 0, 0, 0, velcarroi);
				ig.id = id++;
				ig.chassi.cores = cor(pelement->Attribute("fill"));
				ig.cannon.cores = ig.chassi.cores;
				// ig.cannon.cores.blue = 1;
				// ig.chassi.cores.blue = 0.3;
				// ig.chassi.cores.red = 0.3;
				inimigos.push_back(ig);
			}
			else if(nome == "Jogador")
			{
				float x = pelement->FloatAttribute("cx");
				float y = pelement->FloatAttribute("cy");
				float r = pelement->FloatAttribute("r");
				jogador = carro(x,y,r,0,0,0,0,velcarroj);
				jogador.id = id++;
				jogador.chassi.cores = cor(pelement->Attribute("fill"));
				jogador.cannon.cores = jogador.chassi.cores;
			}
			else if(nome == "Pista")
			{
				circulo aux;
				aux.cx = pelement->FloatAttribute("cx");
				aux.cy = pelement->FloatAttribute("cy");
				aux.raio = pelement->FloatAttribute("r");
				aux.cores = cor(pelement->Attribute("fill"));

				vaux.push_back(aux);
			}
			else
			{
				cout << "Círculo desconhecido: "
					<< nome << endl;
				carro aux;
				aux.cx = pelement->FloatAttribute("cx");
				aux.cy = pelement->FloatAttribute("cy");
				aux.raio = pelement->FloatAttribute("r");
				// aux.cores = cor("cinza");

				inimigos.push_back(aux);
			}
		}
		else if(nome == "rect")
		{
			nome = string(pelement->Attribute("id"));
			if(nome == "LargadaChegada")
			{
				float px = pelement->FloatAttribute("x");
				float py = pelement->FloatAttribute("y");
				float largura = pelement->FloatAttribute("width");
				float altura = pelement->FloatAttribute("height");
				float profundidade = 2;
				cor cores = cor(pelement->Attribute("fill"));
				px += largura/2;
				py += altura/2;

				chegada = retangulo(px ,py,  largura, altura, profundidade, cores.red, cores.green, cores.blue);

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

	vaux.clear();

	// Cria objeto da janela
	janarena.altura = 2*pistafora.raio;
	janarena.largura = 2*pistafora.raio;
	janarena.titulo = "Trabalho CG 2";
	janarena.vx = pistafora.cx - pistafora.raio;
	janarena.vy = pistafora.cx - pistafora.raio;
	janarena.cores = pistadentro.cores;

	return 0;
}
