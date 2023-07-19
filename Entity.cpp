#include "Entity.hpp"

Entity::Entity(float x_, float y_, SDL_Texture *tex_) : x(x_), y(y_), tex(tex_)
{
	currFr.x = 0;
	currFr.y = 0;
	currFr.w = 64;
	currFr.h = 64;
}

Entity::Entity()
{
	return ;
}
float Entity::getX()
{
	return this->x;
}

float Entity::getY()
{
	return this->y;
}

SDL_Texture *Entity::getTex()
{
	return this->tex;
}

SDL_Rect Entity::getCFr()
{
	return currFr;
}

Entity::~Entity()
{
	return ;
}