#include "janela.hh"

janela::janela() : altura(0.0), largura(0.0), vx(0.0), vy(0.0) {}

// prepara a câmera
void janela::inicia()
{
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);

	// Janela
	glutInitWindowSize(largura,altura);
	glutInitWindowPosition(100,100);
	glutCreateWindow(titulo.c_str());

	// Cor do fundo
	glClearColor(cores.red,cores.green,cores.blue,0.0);

	// Visão
	// glMatrixMode(GL_PROJECTION);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);

	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	// glMatrixMode(GL_PROJECTION);
	// glLoadIdentity();
	// gluPerspective (90, 1.0, 10, 400);
	// // glOrtho(vx,vx+largura,vy+altura,vy,-1.0,50.0);
	// glMatrixMode(GL_MODELVIEW);
	// gluLookAt(vx+largura/2, vy+altura/2, -300, vx+largura/2, vy+altura/2, 100, 0,-1,0);

	#if DEBUG
		std::cout << "Debug Janela:" << std::endl;
		std::cout << "\taltura: " << altura << std::endl;
		std::cout << "\tlargura: " << largura << std::endl;
		std::cout << "\tvx: " << vx << std::endl;
		std::cout << "\tvy: " << vy << std::endl;
		std::cout << "\ttitulo: " << titulo << std::endl;
	#endif
}

// limpa a tela
void janela::limpa()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

// atualiza imagem da tela
void janela::atualizabuff()
{
	glutSwapBuffers();
	// gluLookAt(500,500,-1, 0,0,1, 1,0,0);
	// glFlush();
}

void janela::atualizadisp()
{
	glutPostRedisplay();
}

void janela::redimensiona(int w, int h)
{
	// glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	// glMatrixMode(GL_PROJECTION);
	// glLoadIdentity();
	// glOrtho(vx,vx+largura,vy+altura,vy,-1.0,1.0);
	// if (w <= h)
	// 	gluPerspective (45, (GLfloat)h/(GLfloat)w, 10, 100);
	// else
	// 	gluPerspective (45, (GLfloat)w/(GLfloat)h, 10, 100);
	// glMatrixMode(GL_MODELVIEW);
	// gluLookAt(vx+largura/2, vy+altura/2, -20, vx+largura/2, vy+altura/2 ,100, 0,1,0);
}

float janela::getcx()
{
	return vx + largura/2;
}

float janela::getcy()
{
	return vy + altura/2;
}
