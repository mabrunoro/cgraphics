#include <iostream>
#include <list>
#include <cmath>
#include "cor.hh"

#if MACLINUX
	#include <GLUT/glut.h>
#else
	#include <GL/glut.h>
#endif

#ifndef _CIRCULO_HH_
#define _CIRCULO_HH_

class circulo
{
public:
	float cx;
	float cy;
	float raio;
	cor cores;

	circulo(float cx, float cy, float raio, float cr, float cg, float cb);

	circulo();

	void draw();

	float rdistance(circulo c) const;

	template<typename T>
	bool colide(circulo pd, circulo pf, std::list<T> ig) const
	{
		float aux = rdistance(pd);
		// std::cout << aux << std::endl;
		if(aux <= 0)
			return true;

		aux = rdistance(pf);
		// std::cout << aux << std::endl;
		if(aux >= -2*raio)
			return true;

		for(typename std::list<T>::iterator it = ig.begin(); it != ig.end(); ++it)
		{
			aux = rdistance(*it);
			// std::cout << aux << std::endl;
			if(aux <= 0)
				return true;
		}
		// std::cout << "false" << std::endl;
		return false;
	}

	template<typename T>
	bool colide(circulo pd, circulo pf, std::list<T> ig,T jg) const
	{
		float aux = rdistance(pd);
		// std::cout << aux << std::endl;
		if(aux <= 0)
			return true;

		aux = rdistance(pf);
		// std::cout << aux << std::endl;
		if(aux >= -2*raio)
			return true;

		aux = rdistance(jg);
		if(aux <= 0)
			return true;

		for(typename std::list<T>::iterator it = ig.begin(); it != ig.end(); ++it)
		{
			aux = rdistance(*it);
			// std::cout << aux << std::endl;
			if((aux <= 0) && (aux > (-1)*raio))
				return true;
		}
		// std::cout << "false" << std::endl;
		return false;
	}

	template<typename T>
	bool colide(circulo pd, circulo pf, std::list<T> ig,T jg,bool ti,int *j) const
	{
		float aux = rdistance(pd);
		// std::cout << aux << std::endl;
		if(aux <= 0)
		{
			*j = -1;
			return true;
		}

		aux = rdistance(pf);
		// std::cout << aux << std::endl;
		if(aux >= -2*raio)
		{
			*j = -1;
			return true;
		}

		aux = rdistance(jg);
		if(aux <= 0)
		{
			if(ti) //tiro veio do inimigo
			*j = jg.id;
			return true;
		}

		for(typename std::list<T>::iterator it = ig.begin(); it != ig.end(); ++it)
		{
			aux = rdistance(*it);
			// std::cout << aux << std::endl;
			if((aux <= 0) && (aux > (-1)*raio))
			{
				if(!ti)
					*j = it->id;
				return true;
			}
		}
		// std::cout << "false" << std::endl;
		*j = -2;
		return false;
	}
};

#endif
