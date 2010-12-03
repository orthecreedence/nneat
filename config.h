#ifndef _NN_CONFIG_H
#define _NN_CONFIG_H

#include <stdlib.h>

// ----------------------------------------
// basic math/helpers
// ----------------------------------------
#define RAND (rand() / (double)RAND_MAX)
#define RAND_NEG ((rand() - (RAND_MAX / 2)) / (double)RAND_MAX) * 2
#define RAND_BIN (int)(rand() / (RAND_MAX / 2))
#define RAND_BIN_NEG (int)((RAND_BIN * 2) - 1)

#define PI			3.141592654

class config
{
public:
	// graphics settings
	struct graphics
	{
	public:
		static unsigned int win_x, win_y;
		static float initial_x, initial_y, initial_z;
	};
	
	// population settings
	struct population
	{
		static unsigned int num_animals;
		static unsigned int num_food;
		static unsigned int gen_before_famine;
		static float famine_prob;
		static float famine_percent;
		static unsigned int num_ticks;
		static unsigned int delay;
		static bool parallel_process;
	};
	
	// animal settings
	struct animal
	{
		static unsigned int track_n_foods;
		static unsigned int track_n_animals;

		static float speed_constant;
		
		static float max_food_ingestion;
		static unsigned int chew_ticks;
		
		static float fitness_run_penalty;
		static float fitness_shock_penalty;
		
		static bool shocking;
		static float shock_threshold;
		static unsigned int frozen_ticks;
		
		static float max_turn_angle;
		
		static float size;
		static float tail_size;
	};
	
	// food settings
	struct food
	{
		static float size;
		static float start_amount;
	};
	
	// network settings
	struct network
	{
		static unsigned int num_inputs;
		static unsigned int num_hidden;
		static unsigned int num_outputs;
		static unsigned short int genome_type;
	};
};

#endif /* _NN_CONFIG_H */
