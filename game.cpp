#include "game.hpp"
#include "Entity.hpp"
#include <chrono>
using namespace std;



Render::Render(const char * p_title, int p_w, int p_h) : window(NULL), renderer(NULL), buffer1(NULL), buffer2(NULL)
{
	window = SDL_CreateWindow(p_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, p_w, p_h, SDL_WINDOW_SHOWN);
	if (window == NULL)
		cout << "Window failed to init. Error: " << SDL_GetError() << endl;
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED && SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL)
		cout << "Renderer failed to init. Error: " << SDL_GetError() << endl;
    
    buffer1 = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, p_w, p_h);
    buffer2 = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, p_w, p_h);
    buffer3 = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, p_w, p_h);
    currentBuffer = buffer1; 
}

SDL_Texture *Render::loadTexture(const char *path)
{
	SDL_Texture *texture = NULL;
	texture = IMG_LoadTexture(renderer, path);
	if (texture == NULL)
		cout << "Failed to load texture. Error: " << SDL_GetError() << endl;
	return texture;
}

int Render::getRefreshRate()
{
	int displayIndex = SDL_GetWindowDisplayIndex(window);
	SDL_DisplayMode current;
	SDL_GetCurrentDisplayMode(displayIndex, &current);
	return current.refresh_rate;
}

void Render::drawMap(Entity &p, Render &wind)
{		
	for (vector<Entity>::iterator ite = p.walls.begin(); ite != p.walls.end(); ite++)
				wind.render(*ite);
	for (vector<Entity>::iterator it = p.floor.begin(); it != p.floor.end(); it++)
				wind.render(*it); 
}

void Render::cleanUp(game *info)
{
	SDL_DestroyRenderer(renderer);
}

void Render::clear()
{
    SDL_SetRenderTarget(renderer, currentBuffer);
	SDL_RenderClear(renderer);
}

void Render::render(Entity &en)
{
	SDL_Rect src;
	src.x = en.getCFr().x;
	src.y = en.getCFr().y;
	src.w = en.getCFr().w;
	src.h = en.getCFr().h;

	SDL_Rect dst;
	dst.x = en.getX();
	dst.y = en.getY();
	dst.w = en.getCFr().w;
	dst.h = en.getCFr().h;
    SDL_SetRenderTarget(renderer, currentBuffer);
	SDL_RenderCopy(renderer, en.getTex(), &src, &dst);
}

void Render::renderTexture(SDL_Texture* texture, int x, int y, bool left, game* info) {
    renderTexture(texture, x, y, left, info, 1.0f, 1.0f);
}

SDL_Renderer* Render::getRenderer() {
    return renderer;
}

SDL_Texture* Render::getCurrentBuffer() {
    return currentBuffer;
}


void Render::renderTexture(SDL_Texture* texture, int x, int y, bool left, game* info, float scaleX, float scaleY) {
    info->lastTextureRendered = texture;

    SDL_Rect destRect;
    destRect.x = x;
    destRect.y = y;

    int originalW, originalH;
    SDL_QueryTexture(texture, NULL, NULL, &originalW, &originalH);

    destRect.w = static_cast<int>(originalW * scaleX);
    destRect.h = static_cast<int>(originalH * scaleY);

    if (left)
        SDL_RenderCopyEx(renderer, texture, NULL, &destRect, 0, NULL, SDL_FLIP_HORIZONTAL);
    else
        SDL_RenderCopy(renderer, texture, NULL, &destRect);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
}



void Render::display()
{
   SDL_SetRenderTarget(renderer, NULL);  // Reset rendering target to the default window
    SDL_RenderCopy(renderer, currentBuffer, NULL, NULL);  // Render the current buffer to the window
    SDL_RenderPresent(renderer);
    if (currentBuffer == buffer1)
        currentBuffer = buffer2;
    else if (currentBuffer == buffer2)
        currentBuffer = buffer3;
    else
        currentBuffer = buffer1;
}

SDL_Window *Render::getWindow()
{
    return this->window;
}
Render::~Render()
{
    SDL_DestroyTexture(buffer1);
    SDL_DestroyTexture(buffer2);
    SDL_DestroyTexture(buffer3);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}