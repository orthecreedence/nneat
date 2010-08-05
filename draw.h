#ifndef _AGL_DRAW_H
#define _AGL_DRAW_H

class draw
{
public:
	draw();
	~draw();
	static void line(unsigned int, unsigned int, unsigned int, unsigned int, float, float, float);
	static void line3(float, float, float, float, float, float, float, float, float);
	static void square(unsigned int, unsigned int, unsigned int, float, float, float);
	static void square3(float, float, float, float, float, float, float);
	static void map_xy_xyz(int, int, float *);
};

#endif /* _AGL_DRAW_H */
