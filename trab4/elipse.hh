#include "circulo.hh"

#ifndef _ELIPSE_HH_
#define _ELIPSE_HH_

class elipse : public circulo
{
public:
	float ry;

	elipse();

	elipse(float x, float y, float rx, float red, float gre, float blu, float ry);

	void draw();
};

#endif
