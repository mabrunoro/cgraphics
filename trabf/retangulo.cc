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

	GLfloat no_mat[] = {0.0,0.0,0.0,1.0};

	glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
	glMaterialfv(GL_FRONT, GL_SHININESS, no_mat);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, no_mat);

	glPushMatrix();

	glMaterialfv(GL_FRONT, GL_EMISSION, mat_cor);
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
