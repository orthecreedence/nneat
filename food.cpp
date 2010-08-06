#include "food.h"
#include "config.h"

food::food() {}

food::~food() {}

void food::reset()
{
	this->x	=	RAND * RAND_BIN_NEG;
	this->y	=	RAND * RAND_BIN_NEG;
	this->z	=	0;
}

