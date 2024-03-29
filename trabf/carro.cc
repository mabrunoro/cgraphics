#include "carro.hh"

// Classe roda
carro::roda::roda() {}

carro::roda::roda(float raio, bool diresq) // true > roda direita | false > roda esquerda
{
	largura = 16*raio/RAIOP; // cada pneu tem largura/2 de largura
	altura = 12*raio/RAIOP;
	if(diresq)
	{
		eixo = retangulo(0,0,largura/2,raio/(RAIOP/3),3*raio/RAIOP,0.5,0.5,0.5);
		pneu = retangulo(0,0,largura/2,altura,0,0.3,0.3,0.3);
	}
	else
	{
		eixo = retangulo(0,0,largura/2,raio/(RAIOP/3),3*raio/RAIOP,0.5,0.5,0.5);
		pneu = retangulo(0,0,largura/2,altura,0,0.3,0.3,0.3);
	}
}

// vr = variável ranhura | va = variável ângulo
void carro::roda::draw(float pran, float va, bool de)
{
	glPushMatrix();
		glPushMatrix();
			if(de)
				glTranslatef(-0.5*eixo.largura,0,0);
			else
				glTranslatef(0.5*eixo.largura,0,0);

			eixo.draw(0);


		glPopMatrix();

		glRotatef(-90,0,1,0);
		glRotatef(va, 1, 0, 0);
		glTranslatef(0,0,-pneu.largura/2);
		//pneu.draw();
		glColor3f(0,0,0);
		// GLfloat mat_cor[] = {0,0,0,1.0};
		// glMaterialfv(GL_FRONT, GL_EMISSION, mat_cor);
		GLUquadric *quad = gluNewQuadric();
		gluQuadricOrientation(quad,GLU_OUTSIDE);
		gluDisk(quad, 0.0f, pneu.altura/2, 32, 1);
	  gluCylinder(quad,pneu.altura/2,pneu.altura/2,pneu.largura,32,32);
		glTranslatef(0,0,pneu.largura);
		gluDisk(quad, 0.0f, pneu.altura/2, 32, 1);
		// desenha as ranhuras
		// glLineWidth(1);
		// glColor3f(1,1,1);
		// if(pran >= 0)
		// {
		// 	glBegin(GL_LINES);
		// 		glVertex3f(-largura/4,-altura/2+pran*altura,0);
		// 		glVertex3f(largura/4,-altura/2+pran*altura,0);
		// 	glEnd();
		// }
		// // glBegin(GL_LINES);
		// // 	glVertex3f(-largura/4,-altura/4+pran*altura,0);
		// // 	glVertex3f(largura/4,-altura/4+pran*altura,0);
		// // glEnd();
		// glBegin(GL_LINES);
		// 	glVertex3f(-largura/4,pran*altura,0);
		// 	glVertex3f(largura/4,pran*altura,0);
		// glEnd();
		// // glBegin(GL_LINES);
		// // 	glVertex3f(-largura/4,altura/4+pran*altura,0);
		// // 	glVertex3f(largura/4,altura/4+pran*altura,0);
		// // glEnd();
		// if(pran <= 0)
		// {
		// 	glBegin(GL_LINES);
		// 		glVertex3f(-largura/4,altura/2+pran*altura,0);
		// 		glVertex3f(largura/4,altura/2+pran*altura,0);
		// 	glEnd();
		// }
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
	chassi = retangulo(0,0,24*r/RAIOP,42*r/RAIOP,10*r/RAIOP,1,0,0);
	cockpit = circulo(0, 0, chassi.largura/4, 0.3, 0.3, 0);
	cannon = retangulo(0,0,4*r/RAIOP,13*r/RAIOP,4*r/RAIOP,0,1,0);
	rdd = roda(r,true);
	rde = roda(r,false);
	rtd = roda(r,true);
	rte = roda(r,false);
}

carro::carro(float x, float y, float ro, float var, float vran, float del, int vac, float vel) : circulo(x,y,ro,0,0,0), varoda(var), pran(vran), delta(del), vacan(vac), velocidade(vel)
{
	chassi = retangulo(0,0,24*ro/RAIOP,42*ro/RAIOP,10*ro/RAIOP,1,0,0);
	cockpit = circulo(0, 0, chassi.largura/4, 1, 1, 1);
	cannon = retangulo(0,0,4*ro/RAIOP,13*ro/RAIOP,4*ro/RAIOP,0,1,0);
	rdd = roda(ro,true);
	rde = roda(ro,false);
	rtd = roda(ro,true);
	rte = roda(ro,false);
}

// dir = direção do movimento (-1 trás, 0 parado, 1 frente)
// vac = variável ângulo canhão
// varoda = variável ângulo roda
void carro::draw(bool luz)
{
	// GLfloat materialColorA[] = { float(0.5*chassi.cores.red), float(0.5*chassi.cores.green), float(0.5*chassi.cores.blue), 1};
	// GLfloat materialColorD[] = { float(0.8*chassi.cores.red), float(0.8*chassi.cores.green), float(0.8*chassi.cores.blue), 1};
	// GLfloat mat_specular[] = { float(0.5*chassi.cores.red), float(0.5*chassi.cores.green), float(0.5*chassi.cores.blue), 1};
	// GLfloat mat_shininess[] = { 80 };
	GLfloat mat_cor[] = {0.3, 0.3, 0.3, 1};

	glPushMatrix();
		glTranslatef(cx, cy, 0);
		glRotatef(delta, 0, 0, 1);
		glPushMatrix();
			glTranslatef(0, -chassi.altura/2, 0);
			// float aux = chassi.largura;
			// GLfloat light_position1[] = { aux, 0, -2*cannon.profundidade, 1.0 };
			// GLfloat light_position2[] = { -aux, 0, -2*cannon.profundidade, 1.0 };
			// GLfloat light_direction[] = { 0, -1, 0 };
			// GLfloat light_cutoff[] = { 45 };
			// glEnable(GL_LIGHTING);
			// glLightfv(GL_LIGHT2, GL_POSITION, light_position1);
			// glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, light_direction);
			// glLightfv(GL_LIGHT2, GL_SPOT_CUTOFF, light_cutoff);
			//
			// glLightfv(GL_LIGHT3, GL_POSITION, light_position2);
			// glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, light_direction);
			// glLightfv(GL_LIGHT3, GL_SPOT_CUTOFF, light_cutoff);
			// if(!luz)
			// {
			// 	glEnable(GL_LIGHT2);
			// 	glEnable(GL_LIGHT3);
			// }
			// else
			// {
			// 	glDisable(GL_LIGHT2);
			// 	glDisable(GL_LIGHT3);
			// }
			glRotatef(vacan, 0, 0, 1);
			glTranslatef(0, -cannon.altura/4, 0);
			cannon.draw(2*cannon.profundidade);
		glPopMatrix();

		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_cor);
		glMaterialf(GL_FRONT, GL_SHININESS, 10);
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_cor);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_cor);

		glPushMatrix();
			// glMaterialfv(GL_FRONT, GL_EMISSION, mat_cor);

			glTranslatef(chassi.largura/2+rdd.largura/2-raio/(RAIOP/2), -chassi.altura*0.4, -rdd.pneu.altura/2);
			// glRotatef(90,0,1,0);
			rdd.draw(pran,varoda,true);
		glPopMatrix();

		glPushMatrix();
			glTranslatef(-chassi.largura/2-rdd.largura/2+raio/(RAIOP/2), -chassi.altura*0.4, -rde.pneu.altura/2);
			// glRotatef(-90,0,1,0);
			rde.draw(pran,varoda,false);
		glPopMatrix();

		glPushMatrix();
			glTranslatef(chassi.largura/2+rdd.largura/2-raio/(RAIOP/2), chassi.altura*0.4, -rtd.pneu.altura/2);
			// glRotatef(90,0,1,0);
			rtd.draw(pran,0,true);
		glPopMatrix();

		glPushMatrix();
			glTranslatef(-chassi.largura/2-rdd.largura/2+raio/(RAIOP/2), chassi.altura*0.4, -rte.pneu.altura/2);
			// glRotatef(-90,0,1,0);
			rte.draw(pran,0,false);
		glPopMatrix();

		chassi.draw(chassi.profundidade);
		glMaterialf(GL_FRONT, GL_SHININESS, 80);
		cockpit.draw(1,2,1,chassi.profundidade*1.5);
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

float carro::pontax(float dx)
{
	return ((chassi.altura/2) * sin(delta*M_PI/180.0) + (cannon.altura + dx) * sin((vacan+delta) * M_PI/180.0) + cx);
}

float carro::pontay(float dy)
{
	return (cy - ((chassi.altura/2) * cos(delta*M_PI/180.0) + (cannon.altura + dy) * cos((vacan+delta) * M_PI/180.0)));
}

float carro::pontan()
{
	return (delta+vacan);
}

bool operator==(const carro& b,const carro& c)
{
	return (b.id == c.id);
}

float carro::getcx()
{
	return cx;
}

float carro::getcy()
{
	return cy;
}

float carro::getchassix(int i)
{
	return cx + i*(sin(delta*M_PI/180.0)*(chassi.altura/2));
}

float carro::getchassiy(int i)
{
	return cy - i*(cos(delta*M_PI/180.0)*(chassi.altura/2));
}

float carro::get3rdpx(int v)
{
	return cx - v*(sin(delta*M_PI/180.0)*(chassi.altura/2));
}

float carro::get3rdpy(int v)
{
	return cy + v*(cos(delta*M_PI/180.0)*(chassi.altura/2));
}

float carro::getcanx()
{
	return cx + (sin(delta*M_PI/180.0)*(chassi.altura/2 + cannon.altura/8));
}

float carro::getcany()
{
	return cy - (cos(delta*M_PI/180.0)*(chassi.altura/2 + cannon.altura/8));
}
