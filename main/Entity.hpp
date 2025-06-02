#ifndef ENTITY_H
#define ENTITY_H
#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
using namespace std;

class Entity
{
	public:
		Entity(float x_, float y_, SDL_Texture *tex);
		Entity();
		~Entity();
		void init();
		float getX();
		float getY();
		SDL_Texture * getTex();
		SDL_Rect getCFr();
		vector<Entity> floor;
		vector<Entity> walls;
		vector <Entity> pg;

	private:
		float x;
		float y;
		SDL_Rect currFr;
		SDL_Texture *tex;

};

#endif