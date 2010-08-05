#include <GL/glut.h>

#include "draw.h"

draw::draw() {}
draw::~draw() {}

void draw::line(unsigned int x, unsigned int y, unsigned int x2, unsigned int y2, float r, float g, float b)
{
	glColor3f(r, g, b);
	glBegin(GL_LINES);
		glVertex2i(x, y);
		glVertex2i(x2, y2);
	glEnd();
}

void draw::line3(float x1, float y1, float z1, float x2, float y2, float z2, float r, float g, float b)
{
	glColor3f(r, g, b);
	glBegin(GL_LINES);
		glVertex3f(x1, y1, z1);
		glVertex3f(x2, y2, z2);
	glEnd();
}

void draw::square(unsigned int x, unsigned int y, unsigned int size, float r, float g, float b)
{
	int half	=	size / 2;
	glColor3f(r, g, b);
	glBegin(GL_POLYGON);
		glVertex2i(x - half, y - half);
		glVertex2i(x - half, y + half);
		glVertex2i(x + half, y + half);
		glVertex2i(x + half, y - half);
	glEnd();
}

void draw::square3(float x, float y, float z, float size, float r, float g, float b)
{
	float half	=	size / 2;
	glColor3f(r, g, b);
	
	glBegin(GL_QUADS);
	
	glVertex3f(x - half, y - half, z - half);
	glVertex3f(x + half, y - half, z - half);
	glVertex3f(x + half, y + half, z - half);
	glVertex3f(x - half, y + half, z - half);
	
	glVertex3f(x - half, y - half, z + half);
	glVertex3f(x + half, y - half, z + half);
	glVertex3f(x + half, y + half, z + half);
	glVertex3f(x - half, y + half, z + half);
	
	glVertex3f(x - half, y - half, z - half);
	glVertex3f(x + half, y - half, z - half);
	glVertex3f(x + half, y - half, z + half);
	glVertex3f(x - half, y - half, z + half);
	
	glVertex3f(x - half, y + half, z - half);
	glVertex3f(x + half, y + half, z - half);
	glVertex3f(x + half, y + half, z + half);
	glVertex3f(x - half, y + half, z + half);
	
	glVertex3f(x - half, y - half, z - half);
	glVertex3f(x - half, y + half, z - half);
	glVertex3f(x - half, y + half, z + half);
	glVertex3f(x - half, y - half, z + half);
	
	glVertex3f(x + half, y - half, z - half);
	glVertex3f(x + half, y + half, z - half);
	glVertex3f(x + half, y + half, z + half);
	glVertex3f(x + half, y - half, z + half);
	
	glEnd();
}

void draw::map_xy_xyz(int x, int y, float *pos)
{
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ;
	GLdouble posX, posY, posZ;

	glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
	glGetDoublev( GL_PROJECTION_MATRIX, projection );
	glGetIntegerv( GL_VIEWPORT, viewport );

	winX = (float)x;
	winY = (float)viewport[3] - (float)y;
	glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

	gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
	
	*pos	=	(float)posX;
	pos++;
	*pos	=	(float)posY;
	pos++;
	*pos	=	(float)posZ;
}
