#include "retangulo.hh"

retangulo::retangulo(float px, float py, float w, float h, float cr, float cg, float cb)
: px(px), py(py), largura(w), altura(h)
{
	cores = cor(cr,cg,cb);
}

retangulo::retangulo() : px(0), py(0), largura(0), altura(0)
{
	cores = cor(0,0,0);
}

void retangulo::draw()
{
	// Cores do retângulo
	glColor3f(cores.red, cores.green, cores.blue);

	// Desenha o retângulo
	glBegin(GL_POLYGON);
		glVertex3f(
			px-largura/2,
			py-altura/2,
			0.0);

		glVertex3f(
			px-largura/2,
			py+altura/2,
			0.0);

		glVertex3f(
			px+largura/2,
			py+altura/2,
			0.0);

		glVertex3f(
			px+largura/2,
			py-altura/2,
			0.0);

	glEnd();

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
