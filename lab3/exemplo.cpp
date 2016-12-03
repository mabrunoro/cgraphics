#include <GL/glut.h>
#include <iostream>
#include <cmath>

using namespace std;

#define paddleHeight 80
#define paddleWidth 10
#define baseHeight 40
#define baseWidth 100
#define radiusWheel 30
#define zAxis 0

// const int Width = 700;
// const Height 700;

int keystat [256] = {};
GLfloat gx,gy;
GLfloat tr,t1,t2,t3;


void drawrectangle(int altura, int largura, float r, float g, float b)
{
    glColor3f(r,g,b);
    
    glBegin(GL_QUADS);
        glVertex2f(-largura/2.0, 0.0);
        glVertex2f(-largura/2.0, altura);
        glVertex2f( largura/2.0, altura);
        glVertex2f( largura/2.0, 0.0);
    glEnd();
}

void drawwheel(GLfloat raio, float r, float g, float b)
{
    glColor3f(r,g,b);
    glPointSize(3);
    int i;
    glBegin(GL_POINTS);
        for(i=0;i<=360;i+=20)
        {
            float x = raio*cos(M_PI*i/180);
            float y = raio*sin(M_PI*i/180);
            glVertex3f(x,y,0);
        }
    glEnd();
}

void drawrobot(GLfloat x, GLfloat y, GLfloat tr, GLfloat t1, GLfloat t2, GLfloat t3)
{
    glPushMatrix();
        glTranslatef(gx,gy,0.0);
        drawrectangle(baseHeight, baseWidth, 1.0, 0.0, 0.0);
    glPushMatrix();
        glTranslatef(-baseWidth/2.0,0.0,0.0);
        drawwheel(radiusWheel,0,1,0);
    glPushMatrix();
    glPopMatrix();
        glTranslatef(baseWidth/2.0,0.0,0.0);
        drawwheel(radiusWheel,0,1,0);
    glPopMatrix();
    glPopMatrix();
}

void display(void)
{
    // Limpar todos os pixels
    glClear(GL_COLOR_BUFFER_BIT);
    //Desenhar um polígono branco (retângulo)
    /*glColor3f(0.5,0,0.3);
    glBegin(GL_POLYGON);
        glVertex3f(0.25+gx,0.25+gy,0.0);
        glVertex3f(0.75+gx,0.25+gy,0.0);
        glVertex3f(0.75+gx,0.75+gy,0.0);
        glVertex3f(0.25+gx,0.75+gy,0.0);
    glEnd();*/
    
    drawrobot(gx,gy,tr,t1,t2,t3);
    
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
    glOrtho(-250.0,250.0,-250.0,250.0,100.0,-100.0);
    // x->x, y->y, z->z
}

void keyup(unsigned char key, int x, int y)
{
    switch(key)
    {
        case 'w':
        case 'W':
//             gy += 0.1;
            keystat['w'] = 0;
            break;
        case 's':
        case 'S':
//             gy -= 0.1;
            keystat['s'] = 0;
            break;
        case 'a':
        case 'A':
//             gx -= 0.1;
            keystat['a'] = 0;
            break;
        case 'd':
        case 'D':
//             gx += 0.1;
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
//             gy += 0.1;
            keystat['w'] = 1;
            break;
        case 's':
        case 'S':
//             gy -= 0.1;
            keystat['s'] = 1;
            break;
        case 'a':
        case 'A':
//             gx -= 0.1;
            keystat['a'] = 1;
            break;
        case 'd':
        case 'D':
//             gx += 0.1;
            keystat['d'] = 1;
            break;
    }
//     glutPostRedisplay();
//     cout << "Tecla down: " << key << endl;
}

void idle(void)
{
     if(keystat['w'] == 1)
         gy += 5;
     if(keystat['s'] == 1)
         gy -= 5;
     if(keystat['a'] == 1)
         gx -= 5;
     if(keystat['d'] == 1)
         gx += 5;
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
    gx = 0.0;
    gy = 0.0;
    tr = 0.0;
    t1 = 0.0;
    t2 = 0.0;
    t3 = 0.0;
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