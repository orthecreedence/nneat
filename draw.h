#ifndef _AGL_DRAW_H
#define _AGL_DRAW_H

#include <iostream>

class draw
{
public:
	draw();
	~draw();
	static void line(float, float, float, float, float, float, float);
	static void line3(float, float, float, float, float, float, float, float, float);
	static void square(float, float, float, float, float, float);
	static void square3(float, float, float, float, float, float, float);
	static void map_xy_xyz(int, int, float *);
	static void mode2d(bool);
	static void text(float, float, std::string);
};

#endif /* _AGL_DRAW_H */
