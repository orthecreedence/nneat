#include <iostream>
#include <vector>
#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>

using namespace std;

#include "config.h"
#include "draw.h"
#include "population.h"

struct {
	float x, y, z;
	float lx, ly, lz;
	float anglex, angley, anglez;
} camera;

population pop;

void key_cb(unsigned char key, int x, int y)
{
	int amount	=	1;
	
	if(glutGetModifiers() == GLUT_ACTIVE_SHIFT)
	{
		amount	=	5;
	}
	
	switch(key)
	{
		case 'w':
		case 'W':
			camera.y	-=	amount;
			break;
		case 's':
		case 'S':
			camera.y	+=	amount;
			break;
		case 'a':
		case 'A':
			camera.x	+=	amount;
			break;
		case 'd':
		case 'D':
			camera.x	-=	amount;
			break;
		case '-':
		case '_':
			camera.z	-=	amount;
			break;
		case '=':
		case '+':
			camera.z	+=	amount;
			break;
		case 'q':
			// quit
			exit(0);
			break;
		case 'R':
			// intentionally don't break so the screen resets
		case 'r':
			// reset the view
			camera.x	=	G_INITIAL_X;
			camera.y	=	G_INITIAL_Y;
			camera.z	=	G_INITIAL_Z;
			
			glLoadIdentity();
			
			glTranslatef(camera.x, camera.y, camera.z);
			
			glRotatef(camera.anglex, 1, 0, 0);
			glRotatef(camera.angley, 0, 1, 0);
			glRotatef(camera.anglez, 0, 0, 1);
			
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			gluLookAt(
				0.0, 0.0, 10.0, 
				0.0, 0.0, -1.0,
				0.0, 1.0, 0.0
			);
			break;
		case 'p':
			break;
	}
}

void key_sp_cb(int key, int x, int y)
{
	int amount	=	5;
	
	if(glutGetModifiers() == GLUT_ACTIVE_SHIFT)
	{
		amount	=	1;
	}
	
	switch(key)
	{
		case GLUT_KEY_UP:
			camera.anglex	+=	amount;
			break;
		case GLUT_KEY_DOWN:
			camera.anglex	-=	amount;
			break;
		case GLUT_KEY_LEFT:
			camera.angley	+=	amount;
			break;
		case GLUT_KEY_RIGHT:
			camera.angley	-=	amount;
			break;
	}
}

void resize_cb(int w, int h)
{
	// Prevent a divide by zero, when window is too short
	if(h == 0)
	{
		h = 1;
	}

	float ratio	=	1.0* w / h;

	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45,ratio,1,1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		0.0, 0.0, 10.0, 
		0.0, 0.0, -1.0,
		0.0, 1.0, 0.0
	);
}

void mouse_cb(int button, int state, int x, int y)
{
	if(button != GLUT_LEFT_BUTTON)
	{
		return;
	}
	
	if(state == GLUT_DOWN)
	{
		float *pos;
		pos	=	new float[3];
		
		draw::map_xy_xyz(x, y, pos);
	}
	else
	{
	}
}

void mouse_drag_cb(int x, int y)
{
}

void idle_cb()
{
	glutPostRedisplay();
}

void display_net_cb()
{
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glLoadIdentity();
	
	glTranslatef(camera.x, camera.y, camera.z);
	
	glRotatef(camera.anglex, 1, 0, 0);
	glRotatef(camera.angley, 0, 1, 0);
	glRotatef(camera.anglez, 0, 0, 1);
	
	//pop.animals[0].brain.display();
	
	// swap to our buffer and show the network we just built
	glutSwapBuffers();
	glutPostRedisplay();
}

void display_cb_bug()
{
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glLoadIdentity();
	
	// run the network (updates all neurons and draws them)
	pop.step();
	pop.display();
	
	//cout << pop.epoch << " " << pop.cur_animal << "\n";
	
	// swap to our buffer and show the network we just built
	glutSwapBuffers();
	glutPostRedisplay();
}

int main_(int argc, char ** argv)
{
	//NEAT::Genome *g		=	new NEAT::Genome(0, 3, 2, 1, 32, true, 1);
	NEAT::Genome *g		=	new NEAT::Genome(3, 2, 0, 0);
	NEAT::Population *p	=	new NEAT::Population(g, 6);
	NEAT::Network *cur_n;
	
	double inputs[]	=	{.4, .5, .9};
	unsigned int i;
	
	cur_n	=	p->organisms[0]->net;
	cur_n->load_sensors(inputs);
	cur_n->activate();
	
	for(i = 0; i < cur_n->outputs.size(); i++)
	{
		cout << "Output [" << i << "] = " << cur_n->outputs[i]->activation << "\n";
	}
	
	delete g;
	delete p;
	
	return 0;
}


int main(int argc, char ** argv)
{
	int win, bugwin;
	
	camera.x	=	G_INITIAL_X;
	camera.y	=	G_INITIAL_Y;
	camera.z	=	G_INITIAL_Z;
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(G_WIN_X, G_WIN_Y);
	glutInitWindowPosition(20, 20);
	
	glutIdleFunc(idle_cb);
	
	win		=	glutCreateWindow("rotate");
	glClearColor(1, 1, 1, 1);
	glutDisplayFunc(display_net_cb);
	glutKeyboardFunc(key_cb);
	glutSpecialFunc(key_sp_cb);
	glutMouseFunc(mouse_cb);
	glutMotionFunc(mouse_drag_cb);
	glutReshapeFunc(resize_cb);
	glTranslatef(camera.x, camera.y, camera.z);
	//gluOrtho2D(0, G_WIN_X, 0, G_WIN_Y);
	
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	
	//glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
	//glEnable(GL_COLOR_MATERIAL);
	//glEnable(GL_LIGHTING);
	
	bugwin	=	glutCreateSubWindow(win, 0, 0, G_BUG_WIN_X, G_BUG_WIN_Y);
	glClearColor(1, 1, 1, 0);
	glutDisplayFunc(display_cb_bug);
	
	pop.reset(POP_NUM_ANIMALS, POP_NUM_FOOD, POP_NUM_EPOCHS);
	
	glutMainLoop();
	
	return 0;
}
