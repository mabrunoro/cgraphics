#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include <stdlib.h>


void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);

	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
}

void DrawAxes(double size)
{
	GLfloat mat_ambient_r[] = {1.0,0.0,0.0,1.0};
	GLfloat mat_ambient_g[] = {0.0,1.0,0.0,1.0};
	GLfloat mat_ambient_b[] = {0.0,0.0,1.0,1.0};

	GLfloat no_mat[] = {0.0,0.0,0.0,1.0};

	glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
	glMaterialfv(GL_FRONT, GL_SHININESS, no_mat);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, no_mat);

	glPushMatrix();
		glMaterialfv(GL_FRONT, GL_EMISSION, mat_ambient_r);
		glColor3fv(mat_ambient_g);
		glScalef(size, size*0.1, size*0.1);
		glTranslatef(0.5, 0, 0);
		glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
		glMaterialfv(GL_FRONT, GL_EMISSION, mat_ambient_g);
		glColor3fv(mat_ambient_b);
		glRotatef(90,0,0,1);
		glScalef(size,size*0.1,size*0.1);
		glTranslatef(0.5, 0, 0);
		glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
		glMaterialfv(GL_FRONT, GL_EMISSION, mat_ambient_b);
		glColor3fv(mat_ambient_r);
		glRotatef(-90,0,1,0);
		glScalef(size,size*0.1,size*0.1);
		glTranslatef(0.5, 0, 0);
		glutSolidCube(1.0);
	glPopMatrix();
}

void DrawObj(double size)
{
	GLfloat materialEmission[] = {0.0,0.0,0.0,1};
	GLfloat materialColor[] = {0.0,0.0,1.0,1.0};
	GLfloat materialColorAMB[] = {1.0,1.0,0.0,1.0};
	GLfloat mat_specular[] = {1.0,1.0,1.0,1.0};
	GLfloat mat_shininess[] = {128};
	glMaterialfv(GL_FRONT,GL_EMISSION,materialEmission);
	glMaterialfv(GL_FRONT,GL_AMBIENT,materialColor);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,materialColor);
	glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
	glMaterialfv(GL_FRONT,GL_SHININESS,mat_shininess);
	glColor3f(1,0,0);

	// glutSolidSphere(size, 100, 16);
	// glRotatef(45,0,1,0);

	glutSolidCube(size);

	// glTranslatef(-5,0,-5);
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if(w <= h)
	{
		// gluPerspective(90, (GLfloat)h/(GLfloat)w, 2, 15);
		glOrtho(-3, 3, -3*(GLfloat)h/(GLfloat)w, 3*(GLfloat)h/(GLfloat)w, 1.0, 15.0);
	}
	else
	{
		// gluPerspective(90, (GLfloat)w/(GLfloat)h, 2, 15);
		glOrtho(-3*(GLfloat)w/(GLfloat)h,3*(GLfloat)w/(GLfloat)h,-3,3,1.0,15.0);
	}
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0,2,5,0,0,0,0,1,0);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLfloat light_position[] = {0.0,5.0,2.0,1.0};
	glLightfv(GL_LIGHT0,GL_POSITION,light_position);

	DrawAxes(1.5);
	DrawObj(1.0);

	glFlush();
}

void keypress(unsigned char key, int x, int y)
{
	switch(key)
	{
	case 'q':
	case 'Q':
		exit(0);
	}
}

int main(int argc, char* argv[])
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500,500);
	glutInitWindowPosition(100,100);
	glutCreateWindow("lab5");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keypress);
	glutMainLoop();
	return 0;
}
