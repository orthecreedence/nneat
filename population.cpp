#include <iostream>
#include <stdlib.h>
#include <math.h>

#include "population.h"
#include "draw.h"
#include "config.h"
#include "NEAT/population.h"
#include "NEAT/species.h"
#include "NEAT/genome.h"

using namespace std;

population::population()
{
	srand(time(NULL));
	
	this->paused		=	false;
}

population::~population() {}

void population::reset()
{
	delete this->pop;
	delete this->genome;
	
	this->reset(this->num_animals, this->num_food, this->num_epochs);
}

void population::reset(unsigned int num_animals, unsigned int num_food, unsigned int num_epochs)
{
	unsigned int i;
	animal a;
	food f;
	
	this->genome	=	new NEAT::Genome(NET_NUM_INPUTS, NET_NUM_OUTPUTS, NET_NUM_HIDDEN, NET_GENOME_TYPE);
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
	vector<NEAT::Species*>::iterator si;
	vector<NEAT::Organism*>::iterator oi;
	animal a;
	
	this->animals.clear();
	
	if(this->pop->species.size() > 0)
	{
		for(si = this->pop->species.begin(); si != this->pop->species.end(); si++)
		{
			if((*si)->organisms.size() == 0)
			{
				continue;
			}
			
			for(oi = (*si)->organisms.begin(); oi != (*si)->organisms.end(); oi++)
			{
				a.reset((*oi));
				a.x	=	0;
				a.y	=	0;
				a.z	=	0;
				this->animals.push_back(a);
			}
		}
	}
	else
	{
		this->reset();
	}
}

void population::run()
{
	while(this->step() > 0) {}
}

unsigned int population::step()
{
	unsigned int i;
	bool has_fitness;
	double angle_diff;
	animal *a	=	&this->animals[this->cur_animal];
	vector<double> fstats;
	vector<NEAT::Organism*>::iterator oi;
	
	if(!this->paused)
	{
		fstats	=	this->get_current_animals_closest_food_stats();
		a->inputs.clear();
		
		angle_diff		=	(fstats[0] - a->direction);
		
		// make angle diff never greater than 180
		if(angle_diff > 180)
		{
			angle_diff	-=	360;
		}
		else if(-angle_diff > 180)
		{
			angle_diff	+=	360;
		}
		
		//cout << angle_diff << endl;
		
		// make it between -1 and 1
		angle_diff	=	(angle_diff / 180);
		
		//cout << angle_diff << endl;
		
		a->inputs.push_back(angle_diff);
		//a->inputs.push_back(a->speed);		// distance
		a->run();
		
		this->epoch++;
	}
	
	if(this->epoch >= this->num_epochs)
	{
		this->epoch			=	0;
		this->cur_animal	=	(this->cur_animal + 1) % this->num_animals;
		
		if(this->cur_animal == 0)
		{
			has_fitness	=	false;
			for(oi = this->pop->organisms.begin(); oi != this->pop->organisms.end(); oi++)
			{
				if((*oi)->fitness >= 1)
				{
					has_fitness	=	true;
					break;
				}
			}
			
			if(!has_fitness)
			{
				cout << "\nAll organisms died, lol. Starting over...\n";
				this->reset();
				return 0;
			}
			
			this->pop->epoch(this->generation);
			
			if(this->pop->organisms.size() == 0)
			{
				this->reset();
			}
			
			this->assign_animals();
			this->generation++;
		}
	}
	
	for(i = 0; i < POP_DELAY; i++)
	{
		cout << "";
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
	draw::line3(a->x, a->y, a->z, a->x + (.1 * cos(a->direction * (PI / 180))), a->y + (.1 * sin(a->direction * (PI / 180))), 0, 0, 0, 0);
	
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
	int negx, negy;
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
	
	negx	=	a->x - this->foods[fidx].x < 0 ? -1 : 1;
	negy	=	a->y - this->foods[fidx].y < 0 ? -1 : 1;
	//angle	=	asin((this->foods[fidx].x - a->x) / dist) * neg;
	//angle	=	((angle * (180 / PI)));
	
	angle	=	atan((this->foods[fidx].y - a->y) / (this->foods[fidx].x - a->x));
	angle	=	angle * (180 / PI);
	
	// our angle needs to be adjusted between 0 and 360 vs -90 and 90. this is done by testing which quadrants
	// the food is in comparison to the animal. works well.
	if((angle < 0 && negy < 0) || (angle > 0 && negx > 0))
	{
		angle	=	angle + 180;
	}
	else if(angle < 0 && negy > 0)
	{
		angle	=	360 + angle;
	}
	
	output.push_back(angle);
	output.push_back(dist);
	
	return output;
}
