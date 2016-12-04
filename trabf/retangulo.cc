#include "retangulo.hh"

retangulo::retangulo(float px, float py, float w, float h,  float d, float cr, float cg, float cb)
: px(px), py(py), largura(w), altura(h), profundidade(d)
{
	cores = cor(cr,cg,cb);
}

retangulo::retangulo() : px(0), py(0), largura(0), altura(0), profundidade(0)
{
	cores = cor(0,0,0);
}

void retangulo::draw(float h)
{
	// Cores do retângulo
	glColor3f(cores.red, cores.green, cores.blue);

	// Desenha o retângulo
	GLfloat mat_cor[] = {cores.red, cores.green, cores.blue,1.0};

	glPushMatrix();

	// glMaterialfv(GL_FRONT, GL_EMISSION, mat_cor);
		GLfloat no_mat[] = {0,0,0,1};
		GLfloat materialColorA[] = { float(0.5*cores.red), float(0.5*cores.green), float(0.5*cores.blue), 1};
		GLfloat materialColorD[] = { float(0.8*cores.red), float(0.8*cores.green), float(0.8*cores.blue), 1};
		GLfloat mat_specular[] = { float(0.5*cores.red), float(0.5*cores.green), float(0.5*cores.blue), 1};
		GLfloat mat_shininess[] = { 50 };


		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
		glMaterialfv(GL_FRONT, GL_AMBIENT, materialColorA);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, materialColorD);
		glColor3fv(mat_cor);
		
		glTranslatef(px, py, -h);
		glScalef(largura, altura, profundidade);
		glutSolidCube(1.0);

	// glBegin(GL_POLYGON);
	// 	glVertex3f(
	// 		px-largura/2,
	// 		py-altura/2,
	// 		0.0);
	//
	// 	glVertex3f(
	// 		px+largura/2,
	// 		py-altura/2,
	// 		0.0);
	//
	// 	glVertex3f(
	// 		px+largura/2,
	// 		py+altura/2,
	// 		0.0);
	//
	// 	glVertex3f(
	// 		px-largura/2,
	// 		py+altura/2,
	// 		0.0);
	//
	// glEnd();


	glPopMatrix();

	#if DEBUG
		std::cout << "Debug retângulo:" << std::endl;
		std::cout << "\taltura: " << altura << std::endl;
		std::cout << "\tlargura: " << largura << std::endl;
		std::cout << "\tpx: " << px << std::endl;
		std::cout << "\tpy: " << py << std::endl;
		std::cout << "\tcores: (" << cores.red << ','
			<< cores.green << ','
			<< cores.blue << ')' << std::endl;
	#endif
}
