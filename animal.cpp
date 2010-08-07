#include <iostream>
#include <math.h>

#include "animal.h"
#include "config.h"
#include "NEAT/organism.h"
#include "NEAT/network.h"

using namespace std;

animal::animal()
{
	this->reset();
}

animal::~animal() {}

void animal::reset(NEAT::Organism *organism)
{
	this->organism	=	organism;
	this->reset();
}

void animal::reset()
{
	this->direction	=	0;
	this->speed		=	0;
}

void animal::run()
{
	unsigned int i;
	double inputs[this->inputs.size()];
	NEAT::Network *net;
	
	for(i = 0; i < this->inputs.size(); i++)
	{
		inputs[i]	=	this->inputs[i];
	}
	
	net	=	this->organism->net;
	net->load_sensors(inputs);
	net->activate();
	
	this->outputs.clear();
	for(i = 0; i < net->outputs.size(); i++)
	{
		this->outputs.push_back(net->outputs[i]->activation);
	}
	
	this->direction	+=	(2 * ANIMAL_MAX_TURN_ANGLE) * (this->outputs[0] - .5);
	//this->speed		=	this->outputs[1];
	this->speed	=	1;
	this->x			+=	cos(this->direction * (PI / 180)) * this->speed * .001;
	this->y			+=	sin(this->direction * (PI / 180)) * this->speed * .001;
	
	if(this->x > 1)		this->x	=	1;
	if(this->x < -1)	this->x	=	-1;
	if(this->y > 1)		this->y	=	1;
	if(this->y < -1)	this->y	=	-1;
	
	if(this->direction > 360)	this->direction	=	this->direction - 360;
	if(this->direction < 0)		this->direction	=	360 + this->direction;
}


