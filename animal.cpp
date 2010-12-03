#include <iostream>
#include <math.h>

#include "animal.h"
#include "food.h"
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
	this->x			=	RAND * (float)config::graphics::win_x;
	this->y			=	RAND * (float)config::graphics::win_y;
	this->z			=	0;

	this->direction	=	RAND * 360;
	this->speed		=	0;
	this->shock		=	false;
	this->stunned	=	0;
	this->chewing	=	0;
}

void animal::eat(food *f)
{
	float f_diff;
	if(this->chewing > 0)
	{
		return;
	}
	
	f_diff			=	f->amount > config::animal::max_food_ingestion ? config::animal::max_food_ingestion : f->amount;
	f->amount		-=	f_diff;
	
	this->chewing	=	config::animal::chew_ticks;
	this->organism->fitness	+=	f_diff;
}

void animal::run()
{
	unsigned int i;
	float inputs[this->inputs.size()];
	NEAT::Network *net;
	
	if(this->chewing > 0)
	{
		this->chewing--;
	}
	
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
	net->load_sensors((double *)inputs);
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
	
	this->x			+=	cos(this->direction * (PI / 180)) * this->speed * config::animal::speed_constant;
	this->y			+=	sin(this->direction * (PI / 180)) * this->speed * config::animal::speed_constant;
	
	if(this->x > config::graphics::win_x)	this->x	=	config::graphics::win_x;
	if(this->x < 0)							this->x	=	0;
	if(this->y > config::graphics::win_y)	this->y	=	config::graphics::win_y;
	if(this->y < 0)							this->y	=	0;
	
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


