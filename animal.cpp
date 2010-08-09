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
	this->organism->fitness	=	0;
	this->reset();
}

void animal::reset()
{
	this->direction	=	RAND * 360;
	this->speed		=	0;
	this->shock		=	false;
	this->stunned	=	0;
}

void animal::run()
{
	unsigned int i;
	double inputs[this->inputs.size()];
	NEAT::Network *net;
	
	if(this->stunned > 0)
	{
		this->shock	=	false;
		this->stunned--;
		return;
	}
	
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
	
	this->direction	+=	(2 * config::animal::max_turn_angle) * (this->outputs[0] - .5);
	this->speed		=	(2 * this->outputs[1]) - 1;
	if(config::animal::shocking)
	{
		this->shock		=	(this->outputs[2] > config::animal::shock_threshold) ? true : false;
	}
	
	this->x			+=	cos(this->direction * (PI / 180)) * this->speed * .02;
	this->y			+=	sin(this->direction * (PI / 180)) * this->speed * .02;
	
	if(this->x > 1)		this->x	=	1;
	if(this->x < -1)	this->x	=	-1;
	if(this->y > 1)		this->y	=	1;
	if(this->y < -1)	this->y	=	-1;
	
	if(this->direction > 360)	this->direction	=	this->direction - 360;
	if(this->direction < 0)		this->direction	=	360 + this->direction;
	
	this->organism->fitness	-=	config::animal::fitness_run_penalty;
	if(this->shock)
	{
		this->organism->fitness	-=	config::animal::fitness_shock_penalty;
	}
	
	if(this->organism->fitness < 0)
	{
		this->organism->fitness	=	0;
	}
}


