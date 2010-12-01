#include "config.h"

// ----------------------------------------
// graphics settings
// ----------------------------------------
unsigned int config::graphics::win_x		=	600;
unsigned int config::graphics::win_y		=	600;
float config::graphics::initial_x			=	0;
float config::graphics::initial_y			=	0;
float config::graphics::initial_z			=	-12 * 1.6;
float config::graphics::scale_x				=	.1;//300 / (float)config::graphics::bug_win_x;		// these two allow arbitrary scaling of the window size
float config::graphics::scale_y				=	.1;//300 / (float)config::graphics::bug_win_y;
float config::graphics::scale_z				=	.1;//300 / (float)config::graphics::bug_win_x;

// ----------------------------------------
// population settings
// ----------------------------------------
unsigned int config::population::num_animals		=	12;
unsigned int config::population::num_food			=	24;
unsigned int config::population::gen_before_famine	=	20;
float config::population::famine_prob				=	.1;
float config::population::famine_percent			=	.25;
unsigned int config::population::num_ticks			=	1600;
unsigned int config::population::delay				=	0;
bool config::population::parallel_process			=	true;

// ----------------------------------------
// animal settings
// ----------------------------------------
unsigned int config::animal::track_n_foods		=	4;
unsigned int config::animal::track_n_animals	=	4;
float config::animal::speed_constant			=	1;
float config::animal::max_food_ingestion		=	1;
unsigned int config::animal::chew_ticks			=	10;
float config::animal::fitness_run_penalty		=	0;
float config::animal::fitness_shock_penalty		=	.01;
bool config::animal::shocking					=	false;
float config::animal::shock_threshold			=	.95;
unsigned int config::animal::frozen_ticks		=	100;
float config::animal::max_turn_angle			=	30;
float config::animal::size						=	10;

// ----------------------------------------
// food settings
// ----------------------------------------
float config::food::size			=	config::animal::size / 2;
float config::food::start_amount	=	5;

// ----------------------------------------
// network settings
// ----------------------------------------
unsigned int config::network::num_inputs		=	1 + (3 * config::animal::track_n_foods) + (5 * config::animal::track_n_animals);
unsigned int config::network::num_hidden		=	config::network::num_inputs;
unsigned int config::network::num_outputs		=	3;
unsigned short int config::network::genome_type	=	2;
