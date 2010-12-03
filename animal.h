#ifndef _NN_ANIMAL_H
#define _NN_ANIMAL_H

#include <vector>
#include "food.h"
#include "NEAT/organism.h"

class animal
{
public:
	std::vector<float> inputs;
	std::vector<float> outputs;
	float x, y, z;
	
	NEAT::Organism *organism;
	
	float direction;
	float speed;
	bool shock;
	unsigned int stunned;
	unsigned int chewing;
	
	animal();
	~animal();
	
	void reset(NEAT::Organism *);
	void reset();
	void eat(food *);
	void run();
};

#endif /* _NN_ANIMAL_H */
