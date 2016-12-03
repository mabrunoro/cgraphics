#include <string>
#include <iostream>

#ifndef _COR_HH_
#define _COR_HH_

class cor
{
public:
	float red;
	float green;
	float blue;

	cor(float r,float g,float b);

	cor();

	cor(const char* p);
};

#endif
