#include "cor.hh"

cor::cor(float r,float g,float b) : red(r), green(g), blue(b) {}

cor::cor() : cor(0,0,0) {}

cor::cor(const char* p)
{
	std::string s = std::string(p);
	if(s == "black")
	{
		red = 0.0;
		green = 0.0;
		blue = 0.0;
	}
	else if(s == "red")
	{
		red = 1.0;
		green = 0.0;
		blue = 0.0;
	}
	else if(s == "green")
	{
		red = 0.0;
		green = 1.0;
		blue = 0.0;
	}
	else if(s == "blue")
	{
		red = 0.0;
		green = 0.0;
		blue = 1.0;
	}
	else if(s == "white")
	{
		red = 1.0;
		green = 1.0;
		blue = 1.0;
	}
	else if(s == "gray")
	{
		red = 0.3;
		green = 0.3;
		blue = 0.3;
	}
	else if(s == "yellow")
	{
		red = 1;
		green = 1;
		blue = 0.0;
	}
	else if(s == "cyan")
	{
		red = 0;
		green = 1;
		blue = 1;
	}
	else if(s == "magenta")
	{
		red = 1;
		green = 0;
		blue = 1;
	}
	else
	{
		std::cout << "Cor desconhecida: " << s << std::endl;
		red = 0.5;
		green = 0.5;
		blue = 0.0;
	}
}
