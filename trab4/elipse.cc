#include "elipse.hh"

elipse::elipse() {}

elipse::elipse(float x, float y, float rx, float red, float gre, float blu, float ry) : circulo(x,y,rx,red,gre,blu), ry(ry) {}

void elipse::draw()
{
	glPushMatrix();
		float rx = raio;
		int num_segments = 30;
		float theta = 2 * 3.1415926 / float(num_segments);
		float c = cosf(theta);//precalculate the sine and cosine
		float s = sinf(theta);
		float t;
		float y = 1; //we start at angle = 90
		float x = 0;
		glColor3f(cores.red, cores.green, cores.blue);
		glBegin(GL_POLYGON);
		for(int ii = 0; ii < num_segments; ii++)
		{
			//apply radius and offset
			glVertex2f(x * rx + cx, y * ry + cy);//output vertex
			//apply the rotation matrix
			t = x;
			x = c * x - s * y;
			y = s * t + c * y;
		}
		glEnd();
	glPopMatrix();
}
