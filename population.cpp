#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <list>

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
	
	this->reset(this->num_animals, config::population::num_food, this->num_ticks);
}

void population::reset(unsigned int num_animals, unsigned int num_food, unsigned int num_ticks)
{
	unsigned int i;
	animal a;
	food f;
	
	this->genome	=	new NEAT::Genome(config::network::num_inputs, config::network::num_outputs, config::network::num_hidden, config::network::genome_type);
	this->pop		=	new NEAT::Population(this->genome, num_animals);
	
	this->num_animals	=	num_animals;
	//this->num_food		=	(int)(num_food * ((RAND * .9) + .1));
	this->num_food		=	num_food;
	this->cur_animal	=	0;
	
	this->generation	=	0;
	this->tick			=	0;
	this->num_ticks		=	num_ticks;
	
	this->assign_animals();
	
	this->foods.clear();
	for(i = 0; i < this->num_food; i++)
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
	this->num_animals	=	0;
	
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
				a.x	=	RAND * RAND_BIN_NEG;
				a.y	=	RAND * RAND_BIN_NEG;
				a.z	=	0;
				this->animals.push_back(a);
				this->num_animals++;
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
	unsigned int i, f;
	animal *a;
	vector<double> fstats, astats;
	
	if(this->paused)
	{
		return this->cur_animal;
	}
	
	for(i = 0; i < this->animals.size(); i++)
	{
		a	=	&this->animals[this->cur_animal];
		
		fstats	=	this->get_current_animals_closest_food_stats();
		astats	=	this->get_current_animals_closest_animals();
		
		a->inputs.clear();
		for(f = 0; f < config::animal::track_n_foods; f++)
		{
			//cout << this->cur_animal << ": " << fstats[(f * 3)] << " " << fstats[f + 1] << " " << fstats[f + 2] << endl;
			
			a->inputs.push_back(fstats[(f * 3)]);
			a->inputs.push_back(fstats[(f * 3) + 1]);		// distance
			a->inputs.push_back(fstats[(f * 3) + 2]);		// food amount (-1 .. 1)
		}
		
		for(f = 0; f < config::animal::track_n_animals; f++)
		{
			//cout << this->cur_animal << ": " << astats[(f * 3)] << " " << astats[f + 1] << " " << astats[f + 2] << " " << astats[f + 3] << endl;
			
			a->inputs.push_back(astats[(f * 3)]);
			a->inputs.push_back(astats[(f * 3) + 1]);		// distance
			a->inputs.push_back(astats[(f * 3) + 2]);		// neighbor traveling angle
			a->inputs.push_back(astats[(f * 3) + 3]);		// neighbor speed
			a->inputs.push_back(astats[(f * 3) + 4]);		// is neighbor "shocking"?
		}
		
		//cout << "----\n";
		a->run();
		
		if(!config::population::parallel_process)
		{
			break;
		}
		
		this->cur_animal	=	(this->cur_animal + 1) % this->num_animals;
	}
	this->tick++;
	
	if(this->tick >= this->num_ticks)
	{
		if(config::population::parallel_process)
		{
			this->epoch();
		}
		else
		{
			this->cur_animal	=	(this->cur_animal + 1) % this->num_animals;
			
			if(this->cur_animal == 0)
			{
				this->epoch();
			}
		}
		
		this->tick	=	0;
	}
	
	return this->cur_animal;
}

void population::epoch()
{
	unsigned int i;
	bool has_fitness	=	false;
	vector<NEAT::Organism*>::iterator oi;
	
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
		cout << "All organisms died, lol. Starting over...\n";
		this->reset();
		return;
	}
	
	this->pop->epoch(this->generation);
	
	if(this->pop->organisms.size() == 0)
	{
		this->reset();
	}
	
	for(i = 0; i < this->num_food; i++)
	{
		this->foods[i].reset();
	}
	
	this->assign_animals();
	this->generation++;
}

vector<double> population::get_current_animals_closest_food_stats()
{
	vector<double> output;
	unsigned int i, track_n_foods, fidx = 0;
	double f_amount;
	double angle_diff, tmpdist, ldist = 0, dist = 99999;
	animal *a	=	&this->animals[this->cur_animal];
	food *f;
	pair<double, unsigned int> distpair;
	list< pair<double, unsigned int> > nearby;
	list< pair<double, unsigned int> >::iterator nearby_i;
	
	track_n_foods	=	config::animal::track_n_foods > this->num_food ? this->num_food : config::animal::track_n_foods;
	
	nearby.clear();
	for(i = 0; i < this->num_food; i++)
	{
		f		=	&this->foods[i];
		tmpdist	=	pow(pow(a->x - f->x, 2) + pow(a->y - f->y, 2) + pow(a->z - f->z, 2), .5);
		
		if(tmpdist < config::animal::size)
		{
			f->remove();
			a->organism->fitness	+=	f->amount;
			i--;
			continue;
		}
		
		distpair.first	=	tmpdist;
		distpair.second	=	i;
		nearby.push_back(distpair);
		
		if(tmpdist > ldist)
		{
			ldist	=	tmpdist;
		}
	}
	
	nearby.sort();
	for(i = 0, nearby_i = nearby.begin(); i < track_n_foods && nearby_i != nearby.end(); i++, nearby_i++)
	{
		distpair	=	(*nearby_i);
		fidx		=	distpair.second;
		dist		=	distpair.first;
		f_amount	=	this->foods[fidx].amount;
		
		angle_diff	=	this->angle_diff(this->foods[fidx].x, this->foods[fidx].y, a->x, a->y, a->direction);
		
		// get the distance relative to the furthest target
		dist	=	dist / ldist;
		
		output.push_back(angle_diff);
		output.push_back(dist / ldist);
		output.push_back(f_amount);
	}
	
	return output;
}

vector<double> population::get_current_animals_closest_animals()
{
	vector<double> output;
	unsigned int i, track_n_animals, aidx = 0;
	double a_direction;
	double angle_diff, aangle_diff, tmpdist, ldist = 0, dist = 99999;
	animal *a	=	&this->animals[this->cur_animal];
	animal *pal;
	pair<double, unsigned int> distpair;
	list< pair<double, unsigned int> > nearby;
	list< pair<double, unsigned int> >::iterator nearby_i;
	
	track_n_animals	=	config::animal::track_n_animals > this->num_animals ? this->num_animals : config::animal::track_n_animals;
	
	nearby.clear();
	for(i = 0; i < this->num_animals; i++)
	{
		if(i == this->cur_animal)
		{
			// don't count current animal
			continue;
		}
		
		pal		=	&this->animals[i];
		tmpdist	=	pow(pow(a->x - pal->x, 2) + pow(a->y - pal->y, 2) + pow(a->z - pal->z, 2), .5);
		
		if(tmpdist < config::animal::size && a->shock)
		{
			pal->stunned	=	config::animal::frozen_ticks;
			continue;
		}
		
		distpair.first	=	tmpdist;
		distpair.second	=	i;
		nearby.push_back(distpair);
		
		if(tmpdist > ldist)
		{
			ldist	=	tmpdist;
		}
	}
	
	nearby.sort();
	for(i = 0, nearby_i = nearby.begin(); i < track_n_animals && nearby_i != nearby.end(); i++, nearby_i++)
	{
		distpair	=	(*nearby_i);
		aidx		=	distpair.second;
		dist		=	distpair.first;
		a_direction	=	this->animals[aidx].direction;
		
		angle_diff	=	this->angle_diff(this->animals[aidx].x, this->animals[aidx].y, a->x, a->y, a->direction);
		aangle_diff	=	this->angle_diff(a->x, a->y, this->animals[aidx].x, this->animals[aidx].y, this->animals[aidx].direction);
		
		// get the distance relative to the furthest target
		dist	=	dist / ldist;
		
		output.push_back(angle_diff);
		output.push_back(aangle_diff);
		output.push_back(dist / ldist);
		output.push_back(this->animals[aidx].speed);
		output.push_back(this->animals[aidx].shock ? 1 : 0);
	}
	
	return output;
}

double population::angle_diff(float x1, float y1, float x2, float y2, double direction)
{
	int negx, negy;
	double angle, angle_diff;
	
	negx	=	x2 - x1 < 0 ? -1 : 1;
	negy	=	y2 - y1 < 0 ? -1 : 1;
	
	angle	=	atan((y1 - y2) / (x1 - x2));
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
	
	angle_diff		=	(angle - direction);
	if(angle_diff > 180)
	{
		angle_diff	-=	360;
	}
	else if(-angle_diff > 180)
	{
		angle_diff	+=	360;
	}
	angle_diff	=	(angle_diff / 180);
	
	return angle_diff;
}

void population::kill_current()
{
	this->animals[this->cur_animal].organism->fitness	=	0;
	
	if(config::population::parallel_process)
	{
		this->epoch();
	}
	else
	{
		this->cur_animal	=	(this->cur_animal + 1) % this->num_animals;
		
		if(this->cur_animal == 0)
		{
			this->epoch();
		}
	}
	
	this->tick	=	0;
}

void population::display()
{
	unsigned int i;
	animal *a;
	food *f;
	float r, g, b;
	
	if(config::population::parallel_process)
	{
		for(i = 0; i < this->animals.size(); i++)
		{
			a	=	&this->animals[i];
			g	=	a->shock ? 1 : 0;
			b	=	a->stunned ? 1 : 0;
			draw::square3(a->x, a->y, a->z, config::animal::size, 0, g, b);
			draw::line3(a->x, a->y, a->z, a->x + (.1 * cos(a->direction * (PI / 180))), a->y + (.1 * sin(a->direction * (PI / 180))), 0, 0, g, b);
		}
	}
	else
	{
		a	=	&this->animals[this->cur_animal];
		g	=	a->shock ? 1 : 0;
		b	=	a->stunned ? 1 : 0;
		draw::square3(a->x, a->y, a->z, config::animal::size, 0, g, b);
		draw::line3(a->x, a->y, a->z, a->x + (.1 * cos(a->direction * (PI / 180))), a->y + (.1 * sin(a->direction * (PI / 180))), 0, 0, g, b);
	}
	
	for(i = 0; i < this->num_food; i++)
	{
		f	=	&this->foods[i];
		
		if(f->amount < 0)
		{
			g	=	f->amount * -1;
			r	=	0;
		}
		else
		{
			r	=	f->amount;
			g	=	0;
		}
		
		draw::square3(f->x, f->y, f->z, config::food::size, r, g, 0);
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

