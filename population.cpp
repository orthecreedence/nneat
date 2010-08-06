#include <iostream>
#include <stdlib.h>
#include <math.h>

#include "population.h"
#include "draw.h"
#include "config.h"
#include "NEAT/population.h"
#include "NEAT/genome.h"

using namespace std;

population::population()
{
	srand(time(NULL));
	
	this->paused		=	false;
}

population::~population() {}

void population::reset(unsigned int num_animals, unsigned int num_food, unsigned int num_epochs)
{
	unsigned int i;
	animal a;
	food f;
	
	this->genome	=	new NEAT::Genome(NET_NUM_INPUTS, NET_NUM_OUTPUTS, 0, 0);
	this->pop		=	new NEAT::Population(this->genome, num_animals);
	
	this->num_animals	=	num_animals;
	this->num_food		=	num_food;
	this->cur_animal	=	0;
	
	this->generation	=	0;
	this->epoch			=	0;
	this->num_epochs	=	num_epochs;
	
	this->assign_animals();
	
	this->foods.clear();
	for(i = 0; i < num_food; i++)
	{
		f.reset();
		this->foods.push_back(f);
	}
}

void population::assign_animals()
{
	unsigned int i;
	vector<NEAT::Organism*>::iterator it;
	animal a;
	
	this->animals.clear();
	for(i = 0, it = this->pop->organisms.begin(); i < this->num_animals && it != this->pop->organisms.end(); i++, it++)
	{
		a.reset((*it));
		a.x	=	0;
		a.y	=	0;
		a.z	=	0;
		this->animals.push_back(a);
	}
}

void population::run()
{
	while(this->step() > 0) {}
}

unsigned int population::step()
{
	animal *a	=	&this->animals[this->cur_animal];
	vector<double> fstats;
	
	if(!this->paused)
	{
		fstats	=	this->get_current_animals_closest_food_stats();
		a->inputs.clear();
		//cout << fstats[0] << " " << fstats[1] << " " << (a->direction * 360) << endl;
		a->inputs.push_back(a->direction);
		a->inputs.push_back(fstats[0]);		// angle
		a->inputs.push_back(fstats[1]);		// distance
		a->run();
	}
	
	this->epoch++;
	
	if(this->epoch >= this->num_epochs)
	{
		this->epoch			=	0;
		this->cur_animal	=	(this->cur_animal + 1) % this->num_animals;
		
		if(this->cur_animal == 0)
		{
			this->generation++;
			this->pop->epoch(this->generation);
			this->assign_animals();
		}
	}
	
	return this->cur_animal;
}

void population::display()
{
	unsigned int i;
	animal *a;
	food *f;
	
	a	=	&this->animals[this->cur_animal];
	draw::square3(a->x, a->y, a->z, .04, 0, 0, 0);
	
	for(i = 0; i < this->num_food; i++)
	{
		f	=	&this->foods[i];
		draw::square3(f->x, f->y, f->z, .04, 1, 0, 0);
	}
}

void population::pause(unsigned short int pause)
{
	if((this->paused && pause == 2) || pause == 0)
	{
		this->paused		=	false;
	}
	else
	{
		this->paused		=	true;
	}
}

vector<double> population::get_current_animals_closest_food_stats()
{
	vector<double> output;
	unsigned int i, fidx = 0;
	int neg;
	double angle, tmpdist, dist	=	99999;
	animal *a	=	&this->animals[this->cur_animal];
	food *f;
	
	for(i = 0; i < this->num_food; i++)
	{
		f		=	&this->foods[i];
		tmpdist	=	pow(pow(a->x - f->x, 2) + pow(a->y - f->y, 2) + pow(a->z - f->z, 2), .5);
		
		if(tmpdist < .04)
		{
			f->reset();
			a->organism->fitness++;
			i--;
			continue;
		}
		
		if(tmpdist < dist)
		{
			dist	=	tmpdist;
			fidx	=	i;
		}
	}
	
	neg		=	a->y - this->foods[fidx].y < 0 ? -1 : 1;
	angle	=	asin(-(this->foods[fidx].x - a->x) / dist) * neg;
	angle	=	((angle * (180 / PI)) + 180);
	angle	=	angle / 360;
	
	output.push_back(angle);
	output.push_back(dist);
	
	return output;
}
