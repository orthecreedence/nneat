#include "food.h"
#include "config.h"

food::food() {}

food::~food() {}

void food::reset()
{
	this->x	=	RAND * RAND_BIN_NEG;
	this->y	=	RAND * RAND_BIN_NEG;
	this->z	=	0;
	this->amount	=	1;
	//this->amount	=	RAND_BIN_NEG;
}

void food::remove()
{
	this->reset();
	/*
	this->x	=	9999;
	this->y	=	9999;
	this->z	=	0;
	*/
}
