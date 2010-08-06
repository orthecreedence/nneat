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
	double angle;
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
	
	this->direction	=	this->outputs[0] >= 0 ? this->outputs[0] : this->direction;
	this->speed		=	this->outputs[1] >= 0 ? this->outputs[1] : this->speed;
	
	//cout << this->direction << " " << this->speed << endl;
	
	angle	=	(this->direction * 360) / (180 / PI);
	this->x	+=	sin(angle) * this->speed * .1;
	this->y	+=	cos(angle) * this->speed * .1;
	
	if(this->x > 1)		this->x	=	1;
	if(this->x < -1)	this->x	=	-1;
	if(this->y > 1)		this->y	=	1;
	if(this->y < -1)	this->y	=	-1;
}


