#ifndef _NN_FOOD_H
#define _NN_FOOD_H

class food
{
public:
	double amount;
	float x, y, z;
	
	food();
	~food();
	
	void reset();
};

#endif /* _NN_FOOD_H */
