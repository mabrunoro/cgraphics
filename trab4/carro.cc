#include "carro.hh"

// Classe roda
carro::roda::roda() {}

carro::roda::roda(float raio, bool diresq) // true > roda direita | false > roda esquerda
{
	largura = 16*raio/RAIOP; // cada pneu tem largura/2 de largura
	altura = 12*raio/RAIOP;
	if(diresq)
	{
		eixo = retangulo(-3.5*raio/RAIOP,0,largura/2,raio/(RAIOP/3),0.5,0.5,0.5);
		pneu = retangulo(0,0,largura/2,altura,0.3,0.3,0.3);
	}
	else
	{
		eixo = retangulo(3.5*raio/RAIOP,0,largura/2,raio/(RAIOP/3),0.5,0.5,0.5);
		pneu = retangulo(0,0,largura/2,altura,0.3,0.3,0.3);
	}
}

// vr = variável ranhura | va = variável ângulo
void carro::roda::draw(float pran, float va)
{
	glPushMatrix();
		eixo.draw();
		glRotatef(va, 0, 0, 1);
		pneu.draw();
		// desenha as ranhuras
		glLineWidth(1);
		glColor3f(1,1,1);
		if(pran >= 0)
		{
			glBegin(GL_LINES);
				glVertex3f(-largura/4,-altura/2+pran*altura,0);
				glVertex3f(largura/4,-altura/2+pran*altura,0);
			glEnd();
		}
		// glBegin(GL_LINES);
		// 	glVertex3f(-largura/4,-altura/4+pran*altura,0);
		// 	glVertex3f(largura/4,-altura/4+pran*altura,0);
		// glEnd();
		glBegin(GL_LINES);
			glVertex3f(-largura/4,pran*altura,0);
			glVertex3f(largura/4,pran*altura,0);
		glEnd();
		// glBegin(GL_LINES);
		// 	glVertex3f(-largura/4,altura/4+pran*altura,0);
		// 	glVertex3f(largura/4,altura/4+pran*altura,0);
		// glEnd();
		if(pran <= 0)
		{
			glBegin(GL_LINES);
				glVertex3f(-largura/4,altura/2+pran*altura,0);
				glVertex3f(largura/4,altura/2+pran*altura,0);
			glEnd();
		}
	glPopMatrix();
}

void carro::atualizaran(int vr,GLdouble diff)
{
	pran += vr*RANSPEED*velocidade*diff;
	// std::cout << pran << ' ' << vr << std::endl;
	if((pran >= 0.5) || (pran <= -0.5))
		pran = 0;
}

carro::carro() {}

carro::carro(float r) : circulo(0,0,r,0,0,0), delta(0), pran(0.005), varoda(0), vacan(0), velocidade(0)
{
	chassi = retangulo(0,0,24*r/RAIOP,42*r/RAIOP,1,0,0);
	cockpit = elipse(0, 0, chassi.largura/4, 0.3, 0.3, 0, chassi.altura/4);
	cannon = retangulo(0,0,4*r/RAIOP,13*r/RAIOP,0,1,0);
	rdd = roda(r,true);
	rde = roda(r,false);
	rtd = roda(r,true);
	rte = roda(r,false);
}

carro::carro(float x, float y, float ro, float var, float vran, float del, int vac, float vel) : circulo(x,y,ro,0,0,0), varoda(var), pran(vran), delta(del), vacan(vac), velocidade(vel)
{
	chassi = retangulo(0,0,24*ro/RAIOP,42*ro/RAIOP,1,0,0);
	cockpit = elipse(0, 0, chassi.largura/4, 1, 1, 1, chassi.altura/4);
	cannon = retangulo(0,0,4*ro/RAIOP,13*ro/RAIOP,0,1,0);
	rdd = roda(ro,true);
	rde = roda(ro,false);
	rtd = roda(ro,true);
	rte = roda(ro,false);
}

// dir = direção do movimento (-1 trás, 0 parado, 1 frente)
// vac = variável ângulo canhão
// varoda = variável ângulo roda
void carro::draw()
{
	// std::cout << varoda << std::endl;
	glPushMatrix();
		glTranslatef(cx, cy, 0);
		glRotatef(delta, 0, 0, 1);
		glPushMatrix();
			glTranslatef(0, -chassi.altura/2, 0);
			glRotatef(vacan, 0, 0, 1);
			glTranslatef(0, -cannon.altura/4, 0);
			cannon.draw();
		glPopMatrix();

		glPushMatrix();
			glTranslatef(chassi.largura/2+rdd.largura/2-raio/(RAIOP/2), -chassi.altura*0.4, 0);
			rdd.draw(pran,varoda);
		glPopMatrix();

		glPushMatrix();
			glTranslatef(-chassi.largura/2-rdd.largura/2+raio/(RAIOP/2), -chassi.altura*0.4, 0);
			rde.draw(pran,varoda);
		glPopMatrix();

		glPushMatrix();
			glTranslatef(chassi.largura/2+rdd.largura/2-raio/(RAIOP/2), chassi.altura*0.4, 0);
			rtd.draw(pran,0);
		glPopMatrix();

		glPushMatrix();
			glTranslatef(-chassi.largura/2-rdd.largura/2+raio/(RAIOP/2), chassi.altura*0.4, 0);
			rte.draw(pran,0);
		glPopMatrix();

		chassi.draw();
		cockpit.draw();
	glPopMatrix();
}

int carro::atualizadel(int dir,GLdouble diff)
{
	if((varoda == 0) || (dir == 0))
		return delta;
	else if(varoda >= 45)
	{
		// if(dir > 0)
		// 	varoda = 45;
		// else
		varoda -= velocidade*diff;
		// std::cout << 'a' << varoda << std::endl;
		delta -= dir*velocidade*diff;
		return delta;
	}
	else if(varoda <= -45)
	{
		// if(dir > 0)
		// 	varoda = -45;
		// else
		varoda += velocidade*diff;
		// std::cout << 'b' << varoda << std::endl;
		delta += dir*velocidade*diff;
		return delta;
	}
	else if(varoda < 0)
	{
		if(varoda >= -1)
			varoda = 0;
		else
			varoda += velocidade*diff;
		// std::cout << 'c' << varoda << std::endl;
		delta += dir*velocidade*diff;
		return delta;
	}
	else if(varoda > 0)
	{
		if(varoda <= 1)
			varoda = 0;
		else
			varoda -= velocidade*diff;
		// std::cout << 'd' << varoda << std::endl;
		delta -= dir*velocidade*diff;
		return delta;
	}
	else
	{
		std::cout << "Erro atualizadel()" << std::endl;
		return delta = 0;
	}
}

void carro::mra()
{
	if(varoda <= -45)
		return;
	varoda -= 2;
}

void carro::mrd()
{
	if(varoda >= 45)
		return;
	varoda += 2;
}

void carro::movef(circulo pd,circulo pf,std::list<carro> im,GLdouble diff,carro jg)
{
	atualizadel(-1,diff);
	atualizaran(-1,diff);
	for(int i = 10; i > 0; i--)
	{
		// std::cout << velocidade << std::endl;
		circulo *n = new circulo(cx+sin(delta*M_PI/180.0)*(velocidade*diff*i/10.0), cy-cos(delta*M_PI/180.0)*(velocidade*diff*i/10.0),raio,0,0,0);
		// std::cout << '\t' << cx << ' ' << cy << std::endl;
		if(n->colide<carro>(pd,pf,im,jg))
		{
			delete n;
			continue;
		}
		else
		{
			delete n;
			cx = cx+sin(delta*M_PI/180.0)*(velocidade*diff*i/10.0);
			cy = cy-cos(delta*M_PI/180.0)*(velocidade*diff*i/10.0);
			// raio = raio;
			// varoda = varoda;
			// pran = pran;
			// delta = delta;
			// vacan = vacan;
			// velocidade = velocidade;
			return;
		}
	}
}

void carro::movef(circulo pd,circulo pf,std::list<carro> im,GLdouble diff)
{
	atualizadel(-1,diff);
	atualizaran(-1,diff);
	for(int i = 10; i > 0; i--)
	{
		// std::cout << velocidade << std::endl;
		circulo *n = new circulo(cx+sin(delta*M_PI/180.0)*(velocidade*diff*i/10.0), cy-cos(delta*M_PI/180.0)*(velocidade*diff*i/10.0),raio,0,0,0);

		if(n->colide<carro>(pd,pf,im))
		{
			delete n;
			continue;
		}
		else
		{
			delete n;
			cx = cx+sin(delta*M_PI/180.0)*(velocidade*diff*i/10.0);
			cy = cy-cos(delta*M_PI/180.0)*(velocidade*diff*i/10.0);
			// raio = raio;
			// varoda = varoda;
			// pran = pran;
			// delta = delta;
			// vacan = vacan;
			// velocidade = velocidade;
			return;
		}
	}
}

void carro::movet(circulo pd,circulo pf,std::list<carro> im,GLdouble diff)
{
	atualizadel(1,diff);
	atualizaran(1,diff);
	for(int i = 10; i > 0; i--)
	{
		// std::cout << velocidade << std::endl;
		circulo *n = new circulo(cx-sin(delta*M_PI/180.0)*(velocidade*diff*i/10.0), cy+cos(delta*M_PI/180.0)*(velocidade*diff*i/10.0),raio,0,0,0);

		if(n->colide<carro>(pd,pf,im))
		{
			delete n;
			continue;
		}
		else
		{
			delete n;
			cx = cx-sin(delta*M_PI/180.0)*(velocidade*diff*i/10.0);
			cy = cy+cos(delta*M_PI/180.0)*(velocidade*diff*i/10.0);
			// raio = raio;
			// varoda = varoda;
			// pran = pran;
			// delta = delta;
			// vacan = vacan;
			// velocidade = velocidade;
			return;
		}
	}
}

float carro::pontax()
{
	return ((chassi.altura/2) * sin(delta*M_PI/180.0) + cannon.altura * sin((vacan+delta) * M_PI/180.0) + cx);
}

float carro::pontay()
{
	return (cy - ((chassi.altura/2) * cos(delta*M_PI/180.0) + cannon.altura * cos((vacan+delta) * M_PI/180.0)));
}

float carro::pontan()
{
	return (delta+vacan);
}

bool operator==(const carro& b,const carro& c)
{
	return (b.id == c.id);
}
