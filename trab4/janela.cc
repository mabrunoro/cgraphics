#include "janela.hh"

janela::janela() : altura(0.0), largura(0.0), vx(0.0), vy(0.0) {}

// prepara a câmera
void janela::inicia()
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
	glClear(GL_COLOR_BUFFER_BIT);
}

// atualiza imagem da tela
void janela::atualizabuff()
{
	glutSwapBuffers();
}

void janela::atualizadisp()
{
	glutPostRedisplay();
}
