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
	void reset(bool disabled);
	void remove();
};

#endif /* _NN_FOOD_H */
