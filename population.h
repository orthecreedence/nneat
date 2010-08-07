#ifndef _NN_POPULATION_H
#define _NN_POPULATION_H

#include <vector>

#include "animal.h"
#include "food.h"
#include "NEAT/population.h"
#include "NEAT/genome.h"

class population
{
public:
	unsigned int num_animals;
	unsigned int cur_animal;
	unsigned int num_food;
	
	unsigned int num_epochs;
	unsigned int epoch;
	unsigned int generation;
	
	NEAT::Genome *genome;
	NEAT::Population *pop;
	
	std::vector<animal> animals;
	std::vector<food> foods;
	
	bool paused;
	
	population();
	~population();
	
	void reset(unsigned int, unsigned int, unsigned int);
	void reset();
	void assign_animals();
	void run();
	unsigned int step();
	void display();
	void pause(unsigned short int);
	
private:
	std::vector<double> get_current_animals_closest_food_stats();
};

#endif /* _NN_POPULATION_H */
