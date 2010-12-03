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
	
	unsigned int num_ticks;
	unsigned int tick;
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
	void kill_current();
	void display();
	void pause(unsigned short int);
	
private:
	void epoch();
	std::vector<float> get_current_animals_closest_food_stats();
	std::vector<float> get_current_animals_closest_animals();
	float angle_diff(float, float, float, float, float);
};

#endif /* _NN_POPULATION_H */
