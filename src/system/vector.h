#ifndef VECTOR_H
#define VECTOR_H

typedef struct
{
	float x,y;
}Vec2;

inline Vec2 Vec2_Add(const Vec2* a, const Vec2* b)
{
	return (Vec2){ a->x + b->x, a->y + b->y };
}

#endif