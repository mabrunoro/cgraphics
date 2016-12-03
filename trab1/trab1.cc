#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include "tinyxml2.h"
#include "retangulo.hh"
#include "janela.hh"

using namespace tinyxml2;

#ifndef XMLCheckResult
	#define XMLCheckResult(a_eResult) if (a_eResult != XML_SUCCESS) { printf("Error: %i\n", a_eResult); return a_eResult; }
#endif



/*		Funções	e variáveis					*/

janela jan;
retangulo quad;
GLfloat gx, gy; // centro do quadrado
GLfloat dx, dy; // diferença entre centro do quadrado e ponteiro do mouse

int estado; // 0 - estado inicial
			// 1 - cria o quadrado onde o mouse for clicado
			// 2 - quadrado pode ser arrastado ou deletado
			// 3 - quadrado sendo deletado

bool mover; // diz se o clique foi dentro do quadrado


int lerxml(char *argv);


void display(void)
{
	if((estado == 0) || (estado == 3)) // estado inicial
		glClear(GL_COLOR_BUFFER_BIT);

	else if((estado == 1) || ((estado == 2) && mover)) // desenha o quadrado
	{
		// Limpa a tela
		glClear(GL_COLOR_BUFFER_BIT);

	    // Cores do retângulo
	    glColor3f(quad.fundo.red, quad.fundo.green, quad.fundo.blue);

	    // Desenha o retângulo
	    glBegin(GL_POLYGON);
	        glVertex3f(
	        	gx+dx-quad.largura/2.0,
	        	gy+dy-quad.altura/2.0,
	        	0.0);

	        glVertex3f(
	        	gx+dx+quad.largura/2.0,
	        	gy+dy-quad.altura/2.0,
	        	0.0);

	        glVertex3f(
	        	gx+dx+quad.largura/2.0,
	        	gy+dy+quad.altura/2.0,
	        	0.0);

	        glVertex3f(
	        	gx+dx-quad.largura/2.0,
	        	gy+dy+quad.altura/2.0,
	        	0.0);

	    glEnd();
	}

	glutSwapBuffers();
}


void init(void)
{
	// Cor do fundo
	glClearColor(jan.fundo.red,jan.fundo.green,jan.fundo.blue,0.0);

	// Visão
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0,jan.largura,0.0,jan.altura,-1.0,1.0);
}


void mouse(int button, int state, int x, int y)
{
	y = jan.altura - y;

	if((estado == 0) && (state == GLUT_DOWN) && (button == GLUT_LEFT_BUTTON))
	{
		estado = 1;
		gx = x;
		gy = y;
		dx = dy = 0.0;
		mover = false;
	}

	else if((estado == 1) && (state == GLUT_UP) && (button == GLUT_LEFT_BUTTON))
	{
		estado = 2;
		mover = false;
		dx = dy = 0;
	}

	else if((estado == 2) && (state == GLUT_DOWN) && (button == GLUT_LEFT_BUTTON))
	{
		float aux = fabs(x - gx);
		if(aux > quad.largura/2.0)
		{
			mover = false;
			dx = dy = 0;
			return;
		}
		aux = fabs(y - gy);
		if(aux > quad.altura/2.0)
		{
			mover = false;
			dx = dy = 0;
			return;
		}
		dx = gx - x;
		dy = gy - y;
		gx = x;
		gy = y;
		mover = true;
	}
	else if((estado == 2) && (state == GLUT_UP) && (button == GLUT_LEFT_BUTTON))
	{
		if(mover)
		{
			gx = dx + x;
			gy = dy + y;
		}
		mover = false;
		dx = dy = 0;
	}
	else if((state == GLUT_DOWN) && (button == GLUT_RIGHT_BUTTON))
	{
		float aux = fabs(x - gx);
		if(aux > quad.largura/2.0)
			return;

		aux = fabs(y - gy);
		if(aux > quad.altura/2.0)
			return;
		
		estado = 3;
		mover = false;
		dx = dy = 0;
	}
	else if((estado == 3) && (state == GLUT_UP) && (button == GLUT_RIGHT_BUTTON))
	{
		estado = 0;
		mover = false;
		dx = dy = 0;
	}

	// std::cout << "x: " << x << " y: " << y << " gx: " << gx << " gy: " << gy << ' ';
	// std::cout << "Estado: " << estado << ' ' << mover << std::endl;
}


void idle(void)
{
	glutPostRedisplay();
}


// Atualiza a posição do mouse
void mousepos(int x, int y)
{
	// mx = x;
	// my = jan.altura - y;

	if((estado == 2) && mover)
	{
		gx = x;
		gy = jan.altura - y;
	}
	// std::cout << "x = " << x << "; y = " << y << std::endl;
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


/*	Parte gráfica							*/

	gx = gy = 0;
	dx = dy = 0;
	estado = false;

	glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	// Janela
	glutInitWindowSize(jan.largura,jan.altura);
    glutInitWindowPosition(100,100);
    glutCreateWindow(jan.titulo);
    init();

    glutDisplayFunc(display);
    glutIdleFunc(idle);
	glutMouseFunc(mouse);
	glutMotionFunc(mousepos);
    
    glutMainLoop();

    return 0;
}












/*		Código para leitura do XML			*/
int lerxml(char *argv)
{
    XMLDocument xmldoc;
    XMLError eresult = xmldoc.LoadFile(argv);
    XMLCheckResult(eresult);	// Imprime mensagem de erro se houver algo de errado
    
    XMLNode *proot = xmldoc.FirstChild();
	if(proot == nullptr)
	{
		std::cout << "Erro file read" << std::endl;
		return XML_ERROR_FILE_READ_ERROR;
	}




/*		Código para parsing da janela		*/

		
	XMLElement *pelement;
	XMLElement *plist = proot->FirstChildElement("janela");
	if(plist == nullptr)
	{
		std::cout << "Erro parsing janela" << std::endl;
		return XML_ERROR_PARSING_ELEMENT;
	}


		
/*		Lendo informações da janela			*/

	// Busca a largura
	pelement = plist->FirstChildElement("largura");
	if(pelement == nullptr)
	{
		std::cout << "Erro parsing largura" << std::endl;
		return XML_ERROR_PARSING_ELEMENT;
	}
	eresult = pelement->QueryIntText(&jan.largura);
	XMLCheckResult(eresult);

	
	// Busca altura
	pelement = plist->FirstChildElement("altura");
	if(pelement == nullptr)
	{
		std::cout << "Erro parsing altura" << std::endl;
		return XML_ERROR_PARSING_ELEMENT;
	}
	eresult = pelement->QueryIntText(&jan.altura);
	XMLCheckResult(eresult);


	// Busca cores RGB
	pelement = plist->FirstChildElement("fundo");
	if(pelement == nullptr)
	{
		std::cout << "Erro parsing fundo" << std::endl;
		return XML_ERROR_PARSING_ELEMENT;
	}

	eresult = pelement->QueryIntAttribute("corR", &jan.fundo.red);
	XMLCheckResult(eresult);

	
	eresult = pelement->QueryIntAttribute("corG", &jan.fundo.green);
	XMLCheckResult(eresult);
	
	
	eresult = pelement->QueryIntAttribute("corB", &jan.fundo.blue);
	XMLCheckResult(eresult);


	// Busca título
	pelement = plist->FirstChildElement("titulo");
	if(pelement == nullptr)
	{
		std::cout << "Erro parsing titulo" << std::endl;
		return XML_ERROR_PARSING_ELEMENT;
	}
	jan.titulo = pelement->GetText();
	// std::cout << jan.titulo << std::endl;
	


	// Teste
	// std::cout
	// 	<< jan.largura << ' '
	// 	<< jan.altura << ' '
	// 	<< jan.fundo.red << ' '
	// 	<< jan.fundo.green << ' '
	// 	<< jan.fundo.blue << ' '
	// 	<< jan.titulo << '.' << std::endl;


		

		
/*		Código para parsing do quadrado		*/

		
	pelement = proot->FirstChildElement("quadrado");
	if(pelement == nullptr)
	{
		std::cout << "Erro parsing quadrado" << std::endl;
		return XML_ERROR_PARSING_ELEMENT;
	}


		
/*		Lendo informações da janela			*/
	
	// Busca altura
	eresult = pelement->QueryIntAttribute("tamanho", &quad.altura);
	XMLCheckResult(eresult);
	

	// Busca largura
	eresult = pelement->QueryIntAttribute("tamanho", &quad.largura);
	XMLCheckResult(eresult);


	// Busca cores RGB
	eresult = pelement->QueryIntAttribute("corR", &quad.fundo.red);
	XMLCheckResult(eresult);

	
	eresult = pelement->QueryIntAttribute("corG", &quad.fundo.green);
	XMLCheckResult(eresult);
	
	
	eresult = pelement->QueryIntAttribute("corB", &quad.fundo.blue);
	XMLCheckResult(eresult);
	


	// Teste
	// std::cout
	// 	<< quad.largura << ' '
	// 	<< quad.altura << ' '
	// 	<< quad.fundo.red << ' '
	// 	<< quad.fundo.green << ' '
	// 	<< quad.fundo.blue << '.' << std::endl;

	return 0;
}