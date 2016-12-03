#include "circulo.hh"

circulo::circulo(float cx, float cy, float raio, float cr, float cg, float cb)
: cx(cx), cy(cy), raio(raio)
{
	cores = cor(cr,cg,cb);
}

circulo::circulo() : cx(0), cy(0), raio(0)
{
	cores = cor(0,0,0);
}

void circulo::draw(float sx,float sy,float sz,float dz)
{
	GLfloat mat_cor[] = {cores.red, cores.green, cores.blue,1.0};

	GLfloat no_mat[] = {0.0,0.0,0.0,1.0};

	glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
	glMaterialfv(GL_FRONT, GL_SHININESS, no_mat);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, no_mat);
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_EMISSION, mat_cor);
	glColor3fv(mat_cor);
	// cor do círculo
	// glColor3f(cores.red, cores.green, cores.blue);

	// desenha círculo
	glTranslatef(cx, cy, -dz);
	glScalef(sx,sy,sz);
	glutSolidSphere(raio,32,32);
	// glBegin(GL_POLYGON);
	// 	//Change the 6 to 12 to increase the steps (number of drawn points) for a smoother circle
	// 	//Note that anything above 24 will have little affect on the circles appearance
	// 	//Value 1.5 - Draws Triangle
	// 	//Value 2 - Draws Square
	// 	//Value 3 - Draws Hexagon
	// 	//Value 4 - Draws Octagon
	// 	//Value 5 - Draws Decagon
	// 	for(double i = 0; i > -2 * M_PI; i -= M_PI / 24)
	// 		glVertex3f((cos(i) * raio), (sin(i) * raio), 0.0);
	// glEnd();

	#if DEBUG
		std::cout << "Debug círculo:" << std::endl << std::endl;
		std::cout << "\traio: " << raio << std::endl;
		std::cout << "\tcx: " << cx << std::endl;
		std::cout << "\tcy: " << cy << std::endl;
		std::cout << "\tcores: (" << cores.red << ','
			<< cores.green << ','
			<< cores.blue << ')' << std::endl;
	#endif
	glPopMatrix();
}

void circulo::pistdraw(bool df)
{

	draw(1,1,0,0);

	GLfloat no_mat[] = {0.0,0.0,0.0,1.0};

	glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
	glMaterialfv(GL_FRONT, GL_SHININESS, no_mat);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, no_mat);

	glPushMatrix();
		GLfloat mat_cor[] = {0, 0, 0,1.0};
		glMaterialfv(GL_FRONT, GL_EMISSION, mat_cor);
		GLUquadric *quad = gluNewQuadric();
		if(df)
			gluQuadricOrientation(quad,GLU_INSIDE);
		else
			gluQuadricOrientation(quad,GLU_OUTSIDE);

		glTranslatef(cx,cy,0);

		glScalef(1,1,-1);

		gluCylinder(quad,raio,raio,raio,32,32);
	glPopMatrix();
}

float circulo::rdistance(circulo c) const
{
	return (sqrt((c.cx-cx)*(c.cx-cx)+(c.cy-cy)*(c.cy-cy)) - c.raio - raio);
}

// template<typename T>
// bool circulo::colide(circulo pd, circulo pf, std::vector<T> ig) const
// {
// 	float aux = rdistance(pd);
// 	if(aux <= 0)
// 		return true;
//
// 	aux = rdistance(pf);
// 	if(aux >= -2*raio)
// 		return true;
//
// 	for(typename std::vector<T>::iterator it = ig.begin(); it != ig.end(); ++it)
// 	{
// 		aux = rdistance(*it);
// 		if(aux <= 0)
// 			return true;
// 	}
// 	// std::cout << "false" << std::endl;
// 	return false;
// }
