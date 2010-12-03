#include <GL/glut.h>
#include <iostream>

#include "config.h"
#include "draw.h"

using namespace std;

draw::draw() {}
draw::~draw() {}

void draw::line(float x, float y, float x2, float y2, float r, float g, float b)
{
	glColor3f(r, g, b);
	glBegin(GL_LINES);
		glVertex2f(x, y);
		glVertex2f(x2, y2);
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

void draw::square(float x, float y, float size, float r, float g, float b)
{
	float half	=	size / 2;
	glColor3f(r, g, b);
	glBegin(GL_POLYGON);
		glVertex2f(x - half, y - half);
		glVertex2f(x - half, y + half);
		glVertex2f(x + half, y + half);
		glVertex2f(x + half, y - half);
	glEnd();
}

void draw::square3(float x, float y, float z, float size, float r, float g, float b)
{
	float half_x	=	(size / 2);
	float half_y	=	(size / 2);
	float half_z	=	(size / 2);
	glColor3f(r, g, b);
	
	glBegin(GL_QUADS);
	
	glVertex3f(x - half_x, y - half_y, z - half_z);
	glVertex3f(x + half_x, y - half_y, z - half_z);
	glVertex3f(x + half_x, y + half_y, z - half_z);
	glVertex3f(x - half_x, y + half_y, z - half_z);
	
	glVertex3f(x - half_x, y - half_y, z + half_z);
	glVertex3f(x + half_x, y - half_y, z + half_z);
	glVertex3f(x + half_x, y + half_y, z + half_z);
	glVertex3f(x - half_x, y + half_y, z + half_z);
	
	glVertex3f(x - half_x, y - half_y, z - half_z);
	glVertex3f(x + half_x, y - half_y, z - half_z);
	glVertex3f(x + half_x, y - half_y, z + half_z);
	glVertex3f(x - half_x, y - half_y, z + half_z);
	
	glVertex3f(x - half_x, y + half_y, z - half_z);
	glVertex3f(x + half_x, y + half_y, z - half_z);
	glVertex3f(x + half_x, y + half_y, z + half_z);
	glVertex3f(x - half_x, y + half_y, z + half_z);
	
	glVertex3f(x - half_x, y - half_y, z - half_z);
	glVertex3f(x - half_x, y + half_y, z - half_z);
	glVertex3f(x - half_x, y + half_y, z + half_z);
	glVertex3f(x - half_x, y - half_y, z + half_z);
	
	glVertex3f(x + half_x, y - half_y, z - half_z);
	glVertex3f(x + half_x, y + half_y, z - half_z);
	glVertex3f(x + half_x, y + half_y, z + half_z);
	glVertex3f(x + half_x, y - half_y, z + half_z);
	
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

void draw::mode2d(bool on)
{
	if(on)
	{
		int vPort[4];

		glGetIntegerv(GL_VIEWPORT, vPort);

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();

		glOrtho(0, vPort[2], 0, vPort[3], -1, 1);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
	}
	else
	{
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();   
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();	
	}
}

void draw::text(float x, float y, string msg)
{
	draw::mode2d(true);

	glColor3f(0.0, 0.0, 0.0);
	glRasterPos2i((GLint)x, (GLint)((config::graphics::win_y - y)) - 13);		// subtraction because opengl is like an x/y plane (0,0 is lower left)
	
	void * font = GLUT_BITMAP_8_BY_13;
	for (string::iterator i = msg.begin(); i != msg.end(); ++i)
	{
		char c = *i;
		glutBitmapCharacter(font, c);
	}

	draw::mode2d(false);
}
