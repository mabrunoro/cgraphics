#include <GL/glut.h>
#include <iostream>

using namespace std;

int keystat [256] = {};
GLfloat gx,gy;

void display(void)
{
    // Limpar todos os pixels
    glClear(GL_COLOR_BUFFER_BIT);
//     for(int i=1;1<=10;i++)
//     {
//         glColor3f(0.5,0,0);
//         glBegin(GL_POLYGON);
//             glVertex3f(0.1,0.1,0.0);
//             glVertex3f(0.9,0.1,0.0);
//             glVertex3f(0.9,0.9,0.0);
//             glVertex3f(0.1,0.9,0.0);
//         glEnd();
//     }
//     glColor3f(0.5,0,0);
//     glBegin(GL_POLYGON);
//         glVertex3f(0.1,0.1,0.0);
//         glVertex3f(0.9,0.1,0.0);
//         glVertex3f(0.9,0.9,0.0);
//         glVertex3f(0.1,0.9,0.0);
//     glEnd();
    //Desenhar um polígono branco (retângulo)
    glColor3f(0.5,0,0.3);
    glBegin(GL_POLYGON);
        glVertex3f(0.25+gx,0.25+gy,0.0);
        glVertex3f(0.75+gx,0.25+gy,0.0);
        glVertex3f(0.75+gx,0.75+gy,0.0);
        glVertex3f(0.25+gx,0.75+gy,0.0);
    glEnd();
    
    //Não esperar
//     glFlush();
    glutSwapBuffers();
}

void init(void)
{
    // Selecionar cor de fundo (preto)
    glClearColor(0.0,0.0,0.0,0.0);
    
    // Inicializar sistema de viz
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0,1.0,0.0,1.0,-1.0,1.0);
    // x->x, y->y, z->z
}

void keyup(unsigned char key, int x, int y)
{
    switch(key)
    {
        case 'w':
        case 'W':
//             gy += 0.01;
            keystat['w'] = 0;
            break;
        case 's':
        case 'S':
//             gy -= 0.01;
            keystat['s'] = 0;
            break;
        case 'a':
        case 'A':
//             gx -= 0.01;
            keystat['a'] = 0;
            break;
        case 'd':
        case 'D':
//             gx += 0.01;
            keystat['d'] = 0;
            break;
    }
//     glutPostRedisplay();
//     cout << "Tecla up: " << key << endl;
}

void keypress(unsigned char key, int x, int y)
{
    switch(key)
    {
        case 'w':
        case 'W':
//             gy += 0.01;
            keystat['w'] = 1;
            break;
        case 's':
        case 'S':
//             gy -= 0.01;
            keystat['s'] = 1;
            break;
        case 'a':
        case 'A':
//             gx -= 0.01;
            keystat['a'] = 1;
            break;
        case 'd':
        case 'D':
//             gx += 0.01;
            keystat['d'] = 1;
            break;
    }
//     glutPostRedisplay();
//     cout << "Tecla down: " << key << endl;
}

void idle(void)
{
     if(keystat['w'] == 1)
         gy += 0.01;
     if(keystat['s'] == 1)
         gy -= 0.01;
     if(keystat['a'] == 1)
         gx -= 0.01;
     if(keystat['d'] == 1)
         gx += 0.01;
     glutPostRedisplay();
}
 
void mouse(int button, int state, int x, int y)
{
	y = 250 - y;
	
	float xx = x / 250.0;
	float yy = y / 250.0;
	
	cout << "x: " << xx << ", y: " << yy << endl;
}

int main(int argc,char** argv)
{
    gx = 0;
    gy = 0;
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(250,250);
    glutInitWindowPosition(100,100);
    glutCreateWindow("hello");
    init();
    glutKeyboardFunc(keypress);
    glutKeyboardUpFunc(keyup);
    glutDisplayFunc(display);
		glutMouseFunc(mouse);
		glutIdleFunc(idle);
    glutMainLoop();
    
    //C ANSI requer que main retorne um inteiro.
    return 0;
}