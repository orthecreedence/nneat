#include "food.h"
#include "config.h"

food::food() {}

food::~food() {}

void food::reset()
{
	this->x			=	RAND * config::graphics::win_x;
	this->y			=	RAND * config::graphics::win_y;
	this->z			=	0;
	this->amount	=	config::food::start_amount;
}

void food::reset(bool disabled)
{
	if(!disabled)
	{
		this->reset();
		return;
	}
	
	this->x			=	9999;
	this->y			=	9999;
	this->z			=	0;
	this->amount	=	0;
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
