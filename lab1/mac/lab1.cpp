#include <GLUT/glut.h>
#include <iostream>

using namespace std;

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
    glColor3f(1.0,1.0,1.0);
    glBegin(GL_POLYGON);
        glVertex3f(0.25+gx,0.25+gy,0.0);
        glVertex3f(0.75+gx,0.25+gy,0.0);
        glVertex3f(0.75+gx,0.75+gy,0.0);
        glVertex3f(0.25+gx,0.75+gy,0.0);
    glEnd();
    
    //Não esperar
    glFlush();
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

void keypress(unsigned char key, int x, int y)
{
    switch(key)
    {
        case 'w':
        case 'W':
            gy += 0.01;
            break;
        case 's':
        case 'S':
            gy -= 0.01;
            break;
        case 'a':
        case 'A':
            gx -= 0.01;
            break;
        case 'd':
        case 'D':
            gx += 0.01;
            break;
    }
    glutPostRedisplay();
    cout << "Tecla: " << key << endl;
}

int main(int argc,char** argv)
{
    gx = 0;
    gy = 0;
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(250,250);
    glutInitWindowPosition(100,100);
    glutCreateWindow("hello");
    init();
    glutKeyboardFunc(keypress);
    glutDisplayFunc(display);
    glutMainLoop();
    
    //C ANSI requer que main retorne um inteiro.
    return 0;
}