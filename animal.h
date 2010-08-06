#ifndef _NN_ANIMAL_H
#define _NN_ANIMAL_H

#include <vector>
#include "NEAT/organism.h"

class animal
{
public:
	std::vector<double> inputs;
	std::vector<double> outputs;
	double x, y, z;
	
	NEAT::Organism *organism;
	
	double direction;
	double speed;
	
	animal();
	~animal();
	
	void reset(NEAT::Organism *);
	void reset();
	void run();
};

#endif /* _NN_ANIMAL_H */
