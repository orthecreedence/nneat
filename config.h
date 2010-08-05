#ifndef _NN_CONFIG_H
#define _NN_CONFIG_H

#include <stdlib.h>

// ----------------------------------------
// graphics settings
// ----------------------------------------
#define G_WIN_X			1024		// x value for overall window size
#define G_WIN_Y			768			// y value for overall window size
#define G_SCR_X			8			// x value for operational size of network
#define G_SCR_Y			12			// y value for operational size of network
#define G_SCR_Z			0			// z value for screen
#define G_INITIAL_X		0
#define G_INITIAL_Y		0
#define G_INITIAL_Z		-G_SCR_Y * 1.6
#define G_NEURON_SIZE	.15			// size in pixels of neuron

#define G_BUG_WIN_X		300
#define G_BUG_WIN_Y		300

// ----------------------------------------
// population settings
// ----------------------------------------
#define POP_NUM_ANIMALS				1
#define POP_NUM_FOOD				1

// ----------------------------------------
// network settings
// ----------------------------------------

// ----------------------------------------
// neuron settings
// ----------------------------------------

// ----------------------------------------
// basic math/helpers
// ----------------------------------------
#define RAND (rand() / (double)RAND_MAX)
#define RAND_NEG ((rand() - (RAND_MAX / 2)) / (double)RAND_MAX) * 2
#define RAND_BIN (int)(rand() / (RAND_MAX / 2))
#define RAND_BIN_NEG (int)((RAND_BIN * 2) - 1)

#define PI			3.141592654

#endif /* _NN_CONFIG_H */
