#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <unistd.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Entity.hpp"
#include <map>
using namespace std;
class Chest
{
	public:
	Chest(): attack(false), Sattack(false), open(false), ChestArea(false), WeaponArea(false), choice(false), YouRecived(false), currchoice(0), WeaponMSG(false), W_equipped(false), picked(false), overhead(false){};
	~Chest(){};
	void setOpen()
	{
		this->open = true;
	}
	bool isOpen()
	{
		return open;
	}
	vector<SDL_Texture *> ChestClosed;	
	bool ChestArea;
	bool WeaponArea;
	bool choice;
	bool YouRecived;
	bool WeaponMSG;
	bool W_equipped;
	bool picked;
	int currchoice;
	bool overhead;
	bool Sattack;
	bool attack;
	private:
	bool open;

};

typedef struct t_game
{
	Entity p;
	SDL_Texture *pidle0;
	SDL_Texture *pidle1;
	SDL_Texture *pidle2;
	SDL_Texture *pidle3;
	SDL_Texture *pidle4;
	Chest chest2;

	int pposx;
	int pposy;

	bool moveRight;
	bool moveUp;
	bool moveDown;
	bool moveLeft;
	bool idle;
	bool running;
	bool attack;
	bool Sattack;
	bool spaceKeyPressed;
	bool left;
	bool Hattk;
	bool Release;
	bool Hattak2;
	bool CurrentAttack;
	bool destroyed;
	bool Fly_left;
	bool takeDmg;
	bool FlyLightHit;
	bool interrupted;
	bool BatD;
	bool fSwordMSG;
	bool choice;
	bool chest1open;
	bool picked;
	bool SwordArea;
	bool YouRecived;
	bool overhead;
	bool launch;
	bool wand_cd;

	int currchoice;
	int fixedpos;
	
	bool W_equipped;
	bool Chest1Area;


	SDL_Texture* base_sword;
	SDL_Texture *base_wand;
	SDL_Texture* firstSwordMSG;	
	SDL_Texture *firstWandMSG;
	SDL_Texture *AcceptW;
	SDL_Texture *RefuseW;
	SDL_Texture *RecRustySword;
	SDL_Texture * RecOldRod;
	SDL_Texture * RoverR;
	SDL_Texture * RoverL;

	int randProp;
	int FlyHP;
	int CurrentProp;
	int PropToDes;
	int currFlyingX;
	int currFlyingY;
	vector<int> FlyPosx;
	vector<int> FlyPosy;
	int currLife;
	int currentFrame = 0; 
	int animationSpeed = 100;
	std::vector<vector<SDL_Texture *>> Animations[2];
	std::vector<SDL_Texture *> playeridleAnimation; 
	std::vector<SDL_Texture *> playeridleAnimationL;
	std::vector<SDL_Texture *> RunRAnim;
	std::vector<SDL_Texture *> RunLAnim;	
	std::vector<SDL_Texture *> AttackRAnim;	
	std::vector<SDL_Texture *> AttackLAnim;	
	std::vector<SDL_Texture *> HealthBar;	
	std::vector<SDL_Texture *> Floor;
	vector<SDL_Texture *> Heavy;
	vector<SDL_Texture *> HeavyL;
	vector<SDL_Texture *> TorchAnim;
	vector<SDL_Texture *> Props;
	vector<SDL_Texture *> Explosion;
	vector<SDL_Texture *>FlyingEnR;
	vector<SDL_Texture *>FlyingEnL;
	vector<SDL_Texture *> Hurt;
	vector<SDL_Texture * > FlyDeath;
	vector<SDL_Texture *> Smoke;
	vector<SDL_Texture *> SmokeG;
	vector<SDL_Texture *> Sprinkle;
	vector<SDL_Texture *> MagickAnimR;
	vector<SDL_Texture *> MagickAnimL;	
	map<int, bool> PropPos;
	map<int, int>PropCoord;
	bool want_atk = false;
	
} game;

class Render
{
	public:
	Render(const char * p_title, int p_w, int p_h);
	~Render();
	SDL_Texture *loadTexture(const char *fpath);
	 

	int getRefreshRate();
	void drawMap(Entity &p, Render &wind);
	void cleanUp(game *info);
	void clear();
	void render(Entity &ent);
	void display();
	void renderTexture(SDL_Texture* texture, int x, int y, bool left);
	void moveright(game *info, Entity p);
	void moveleft(game *info, Entity p);
	void movedown(game *info, Entity p);
	void moveup(game *info, Entity p);
	void renderToBuffer(Entity& en);
	SDL_Window  *getWindow();
	SDL_Texture *interpolateFrames(SDL_Texture* currentFrame, SDL_Texture* nextFrame, float progress);
	SDL_Renderer *renderer;
	private:
	SDL_Texture* buffer1;
	SDL_Texture* buffer2;
	SDL_Texture *buffer3;
	SDL_Texture* currentBuffer;
	SDL_Window *window;
};


class Enemy
{
	Enemy(){};
	~Enemy(){};
};


// DDOOOOORRRRR

class Doors
{
	public:
	Doors() : locked(true){};
	~Doors(){};
	vector<SDL_Texture *> DoorLocked;
	void setLock(bool open)
	{
		locked = open;
	}
	bool getLock()
	{
		return locked;
	};
	private:
	bool locked;
};




class Cooldown {
private:
    Uint32 lastActivationTime; 
	float sttime;		

public:
    Cooldown(Uint32 starttime) : lastActivationTime(0), sttime(starttime) {}

    bool isReady() const {
        Uint32 currentTime = SDL_GetTicks(); 
        float elapsedSeconds = (currentTime - lastActivationTime) / 1000.0f;
	  return elapsedSeconds >= 1.0f; 
    }
    float getTime()
    {
	Uint32 currentTime = SDL_GetTicks(); 
	float elapsedSeconds = (currentTime - lastActivationTime) / 1000.0f;
	return elapsedSeconds;

    }
    void activate() {
        lastActivationTime = SDL_GetTicks(); 
    }
	void reset()
	{
		this->lastActivationTime = 0;
	}
};

#endif