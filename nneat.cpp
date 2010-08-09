#include <iostream>
#include <vector>
#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#include <pthread.h>

using namespace std;

#include "config.h"
#include "draw.h"
#include "population.h"

#define POP_CTRL_RESET		1
#define POP_CTRL_PAUSE		2
#define POP_CTRL_NEXT		3
#define POP_CTRL_KILL		4
#define POP_CTRL_FASTMODE	5

struct {
	float x, y, z;
	float lx, ly, lz;
	float anglex, angley, anglez;
} camera;

unsigned int psignal	=	0;
bool fast_wait	=	true;
pthread_mutex_t mtx_psignal		=	PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mtx_fast_wait	=	PTHREAD_MUTEX_INITIALIZER;

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
		case 'f':
			pthread_mutex_lock(&mtx_psignal);
			psignal	=	POP_CTRL_FASTMODE;
			pthread_mutex_unlock(&mtx_psignal);
			break;
		case 'R':
			// intentionally don't break so the screen resets
			pthread_mutex_lock(&mtx_psignal);
			psignal	=	POP_CTRL_RESET;
			pthread_mutex_unlock(&mtx_psignal);
		case 'r':
			// reset the view
			camera.x	=	config::graphics::initial_x;
			camera.y	=	config::graphics::initial_y;
			camera.z	=	config::graphics::initial_z;
			
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
		case 'k':
			// "kill" the current animal (set its fitness to 0 and give it no more iterations)
			pthread_mutex_lock(&mtx_psignal);
			psignal	=	POP_CTRL_KILL;
			pthread_mutex_unlock(&mtx_psignal);
			break;
		case 'n':
			pthread_mutex_lock(&mtx_psignal);
			psignal	=	POP_CTRL_NEXT;
			pthread_mutex_unlock(&mtx_psignal);
			break;
		case 'p':
			pthread_mutex_lock(&mtx_psignal);
			psignal	=	POP_CTRL_PAUSE;
			pthread_mutex_unlock(&mtx_psignal);
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
	
	pop.display();
	
	pthread_mutex_lock(&mtx_fast_wait);
	fast_wait	=	false;
	pthread_mutex_unlock(&mtx_fast_wait);
	
	// swap to our buffer and show the network we just built
	glutSwapBuffers();
	glutPostRedisplay();
}

void *pop_process(void *val)
{
	unsigned int l_psignal;
	bool l_fast	=	false;
	bool l_fast_wait;
	
	while(1)
	{
		if(!l_fast)
		{
			pthread_mutex_lock(&mtx_fast_wait);
			l_fast_wait	=	fast_wait;
			pthread_mutex_unlock(&mtx_fast_wait);
			
			if(l_fast_wait)
			{
				continue;
			}
			
			pthread_mutex_lock(&mtx_fast_wait);
			fast_wait	=	true;
			pthread_mutex_unlock(&mtx_fast_wait);
		}
		pop.step();
		
		pthread_mutex_lock(&mtx_psignal);
		l_psignal	=	psignal;
		pthread_mutex_unlock(&mtx_psignal);
		
		switch(l_psignal)
		{
			case POP_CTRL_RESET:
				pop.reset();
				break;
			case POP_CTRL_PAUSE:
				pop.pause(2);
				break;
			case POP_CTRL_NEXT:
				pop.tick	=	config::population::num_ticks + 1;
				break;
			case POP_CTRL_KILL:
				pop.kill_current();
				break;
			case POP_CTRL_FASTMODE:
				l_fast	=	l_fast ? false : true;
				break;
		}
		
		pthread_mutex_lock(&mtx_psignal);
		psignal	=	0;
		pthread_mutex_unlock(&mtx_psignal);
	}
}

int main(int argc, char ** argv)
{
	int win, bugwin, t;
	pthread_t process;
	
	camera.x	=	config::graphics::initial_x;
	camera.y	=	config::graphics::initial_y;
	camera.z	=	config::graphics::initial_z;
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(config::graphics::win_x, config::graphics::win_y);
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
	
	bugwin	=	glutCreateSubWindow(win, 0, 0, config::graphics::bug_win_x, config::graphics::bug_win_y);
	glClearColor(1, 1, 1, 0);
	glutDisplayFunc(display_cb_bug);
	
	pop.reset(config::population::num_animals, config::population::num_food, config::population::num_ticks);
	
	t	=	pthread_create(&process, NULL, pop_process, (void*)NULL);
	glutMainLoop();
	pthread_join(process, NULL);
	
	return 0;
}
