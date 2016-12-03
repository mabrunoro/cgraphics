/*
* Copyright (c) 1993-1997, Silicon Graphics, Inc.
* ALL RIGHTS RESERVED
* Permission to use, copy, modify, and distribute this software for
* any purpose and without fee is hereby granted, provided that the above
* copyright notice appear in all copies and that both the copyright notice
* and this permission notice appear in supporting documentation, and that
* the name of Silicon Graphics, Inc. not be used in advertising
* or publicity pertaining to distribution of the software without specific,
* written prior permission.
*
* THE MATERIAL EMBODIED ON THIS SOFTWARE IS PROVIDED TO YOU "AS-IS"
* AND WITHOUT WARRANTY OF ANY KIND, EXPRESS, IMPLIED OR OTHERWISE,
* INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY OR
* FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL SILICON
* GRAPHICS, INC.  BE LIABLE TO YOU OR ANYONE ELSE FOR ANY DIRECT,
* SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY
* KIND, OR ANY DAMAGES WHATSOEVER, INCLUDING WITHOUT LIMITATION,
* LOSS OF PROFIT, LOSS OF USE, SAVINGS OR REVENUE, OR THE CLAIMS OF
* THIRD PARTIES, WHETHER OR NOT SILICON GRAPHICS, INC.  HAS BEEN
* ADVISED OF THE POSSIBILITY OF SUCH LOSS, HOWEVER CAUSED AND ON
* ANY THEORY OF LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE
* POSSESSION, USE OR PERFORMANCE OF THIS SOFTWARE.
*
* US Government Users Restricted Rights
* Use, duplication, or disclosure by the Government is subject to
* restrictions set forth in FAR 52.227.19(c)(2) or subparagraph
* (c)(1)(ii) of the Rights in Technical Data and Computer Software
* clause at DFARS 252.227-7013 and/or in similar or successor
* clauses in the FAR or the DOD or NASA FAR Supplement.
* Unpublished-- rights reserved under the copyright laws of the
* United States.  Contractor/manufacturer is Silicon Graphics,
* Inc., 2011 N.  Shoreline Blvd., Mountain View, CA 94039-7311.
*
* OpenGL(R) is a registered trademark of Silicon Graphics, Inc.
*/

/*
*  light.c
*  This program demonstrates the use of the OpenGL lighting
*  model.  A sphere is drawn using a grey material characteristic.
*  A single light source illuminates the object.
*/
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include <stdlib.h>
#include <math.h>

void init(void)
{
	glClearColor (0.0, 0.0, 0.0, 0.0);
	//    glShadeModel (GL_FLAT);
	glShadeModel (GL_SMOOTH);
	glEnable(GL_LIGHTING);

	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
}

void DrawAxes(double size)
{
	GLfloat mat_ambient_r[] = { 1.0, 0.0, 0.0, 1.0 };
	GLfloat mat_ambient_g[] = { 0.0, 1.0, 0.0, 1.0 };
	GLfloat mat_ambient_b[] = { 0.0, 0.0, 1.0, 1.0 };
	GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE,
	no_mat);
	glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
	glMaterialfv(GL_FRONT, GL_SHININESS, no_mat);

	//x axis
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_EMISSION, mat_ambient_r);
	glColor3fv(mat_ambient_r);
	glScalef(size, size*0.1, size*0.1);
	glTranslatef(0.5, 0, 0); // put in one end
	glutSolidCube(1.0);
	glPopMatrix();

	//y axis
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_EMISSION, mat_ambient_g);
	glColor3fv(mat_ambient_g);
	glRotatef(90,0,0,1);
	glScalef(size, size*0.1, size*0.1);
	glTranslatef(0.5, 0, 0); // put in one end
	glutSolidCube(1.0);
	glPopMatrix();

	//z axis
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_EMISSION, mat_ambient_b);
	glColor3fv(mat_ambient_b);
	glRotatef(-90,0,1,0);
	glScalef(size, size*0.1, size*0.1);
	glTranslatef(0.5, 0, 0); // put in one end
	glutSolidCube(1.0);
	glPopMatrix();

}

GLfloat p1[] = {3, 3, 3};
GLfloat p2[] = {2, 2, 4};
GLfloat p3[] = {4, 1, 2};
void DrawPoints(double size,
GLdouble ax, GLdouble ay, GLdouble az,
GLdouble bx, GLdouble by, GLdouble bz,
GLdouble cx, GLdouble cy, GLdouble cz)
{

	glPushAttrib(GL_LIGHTING_BIT);
	glDisable(GL_LIGHTING);

	glColor3f(1,0,0);
	glPushMatrix();
	glTranslatef(ax, ay, az);
	glutSolidSphere (size, 100, 16);
	glPopMatrix();

	glColor3f(0, 1, 0);
	glPushMatrix();
	glTranslatef(bx, by, bz);
	glutSolidSphere (size, 100, 16);
	glPopMatrix();

	glColor3f(0,0,1);
	glPushMatrix();
	glTranslatef(cx, cy, cz);
	glutSolidSphere (size, 100, 16);
	glPopMatrix();
	glPopAttrib();
}

void normalize(float a[3])
{
	double norm = sqrt(a[0]*a[0]+a[1]*a[1]+a[2]*a[2]);
	a[0] /= norm;
	a[1] /= norm;
	a[2] /= norm;
}
void cross(float a[3], float b[3], float c[3])
{
	c[0] = a[1]*b[2] - a[2]*b[1];
	c[1] = a[2]*b[0] - a[0]*b[2];
	c[2] = a[0]*b[1] - a[1]*b[0];
}

void MygluLookAt(GLdouble eyex, GLdouble eyey, GLdouble eyez, GLdouble centerx,
GLdouble centery, GLdouble centerz, GLdouble upx, GLdouble upy,
GLdouble upz)
{
	float forward[3], side[3], up[3];
	//column-major order
	GLfloat m[4][4] = { 1,0,0,0,
	0,1,0,0,
	0,0,1,0,
	0,0,0,1};

	forward[0] = centerx - eyex;
	forward[1] = centery - eyey;
	forward[2] = centerz - eyez;

	up[0] = upx;
	up[1] = upy;
	up[2] = upz;

	normalize(forward);

	/* Side = forward x up */
	cross(forward, up, side);
	normalize(side);

	/* Recompute up as: up = side x forward */
	cross(side, forward, up);
	//column-major
	//C  L
	m[0][0] = side[0];
	m[1][0] = side[1];
	m[2][0] = side[2];
	//C  L
	m[0][1] = up[0];
	m[1][1] = up[1];
	m[2][1] = up[2];
	//C  L
	m[0][2] = -forward[0];
	m[1][2] = -forward[1];
	m[2][2] = -forward[2];

	glMultMatrixf(&m[0][0]);
	glTranslated(-eyex, -eyey, -eyez);
}

void ChangeCoordSys(
GLdouble ax, GLdouble ay, GLdouble az,
GLdouble bx, GLdouble by, GLdouble bz,
GLdouble cx, GLdouble cy, GLdouble cz)
{
	float z[3], side[3], up[3];
	GLfloat m[4][4] = { 1,0,0,0,
	0,1,0,0,
	0,0,1,0,
	0,0,0,1};

	z[0] = (ax - bx);
	z[1] = (ay - by);
	z[2] = (az - bz);

	up[0] = (cx - bx);
	up[1] = (cy - by);
	up[2] = (cz - bz);

	normalize(z);

	cross(up, z, side);
	normalize(side);

	/* Recompute up as: up = side x forward */
	cross(z, side, up);

	m[0][0] = side[0];
	m[0][1] = side[1];
	m[0][2] = side[2];

	m[1][0] = up[0];
	m[1][1] = up[1];
	m[1][2] = up[2];

	m[2][0] = z[0];
	m[2][1] = z[1];
	m[2][2] = z[2];

	m[3][0] = bx;
	m[3][1] = by;
	m[3][2] = bz;

	glMultMatrixf(&m[0][0]);
}

double camDist=10;
double camXYAngle=0;
double camXZAngle=0;
int toggleCam = 2;
void display(void)
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	if (toggleCam == 1){
		glTranslatef(0,0,-camDist);
		glRotatef(camXZAngle,1,0,0);
		glRotatef(camXYAngle,0,1,0);
	} else if (toggleCam == 2){
		gluLookAt(6,10,10, 0,0,0, 1,0,0);
	} else if (toggleCam == 3){
		MygluLookAt(5,10,10, 0,0,0, 1,0,0);
	}

	GLfloat light_position[] = { 0.0, 0.0, 5.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	DrawAxes(1.5);
	DrawPoints (
	0.3,
	p1[0],p1[1],p1[2],
	p2[0],p2[1],p2[2],
	p3[0],p3[1],p3[2]);
	glPushMatrix();
	//Coloca em p2, com z alinhado com p2->p1 e orientado por p2->p3
	ChangeCoordSys(
	p1[0],p1[1],p1[2],
	p2[0],p2[1],p2[2],
	p3[0],p3[1],p3[2]);
	DrawAxes(1.5);
	glPopMatrix();

	glFlush ();
}

void reshape (int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	if (w <= h)
		gluPerspective (45, (GLfloat)h/(GLfloat)w, 2, 50);
	else
		gluPerspective (45, (GLfloat)w/(GLfloat)h, 2, 50);
	glMatrixMode(GL_MODELVIEW);
}

int lastX = 0;
int lastY = 0;
int buttonDown=0;
void mouse_callback(int button, int state,
int x, int y)
{
	if (button == GLUT_LEFT_BUTTON &&
	state == GLUT_DOWN) {
		lastX = x;
		lastY = y;
		buttonDown = 1;
	}
	if (button == GLUT_LEFT_BUTTON &&
	state == GLUT_UP) {
		buttonDown = 0;
	}
	glutPostRedisplay();
}
void mouse_motion(int x, int y)
{
	if (!buttonDown)
	return;

	camXYAngle += x - lastX;
	camXZAngle += y - lastY;

	camXYAngle = (int)camXYAngle % 360;
	camXZAngle = (int)camXZAngle % 360;

	lastX = x;
	lastY = y;
	glutPostRedisplay();
}
void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case '1':
		toggleCam = 1;
		break;
	case '2':
		toggleCam = 2;
		break;
	case '3':
		toggleCam = 3;
		break;
	case 27:
		exit(0);
		break;
	}
}
void idle()
{
	glutPostRedisplay();
}
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize (500, 500);
	glutInitWindowPosition (100, 100);
	glutCreateWindow (argv[0]);
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMotionFunc(mouse_motion);
	glutMouseFunc(mouse_callback);
	glutIdleFunc(idle);
	glutMainLoop();
	return 0;
}
