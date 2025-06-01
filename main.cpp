#include "game.hpp"
#include "Entity.hpp"
#include "utils.hpp"
#include <random>

using namespace std;


struct Projectile {
    int x, y;
    int cast = 0;
    bool hit = false;
    int frame = 0;
    int frameDelay = 0;
    bool directionRight;  
};


void LFProp(game *info)
{
	map<int, bool>::iterator it = info->PropPos.begin();
	for (; it != info->PropPos.end(); it++)
	{	
		if (!info->left)
		{
			if (info->pposx > it->first - 64 && info->pposx < it->first)
			{	
				info->CurrentProp = it->first;
				info->PropCoord.insert(std::make_pair(it->first, 0));
				break ;
			}
		}
		else
		{
			if (info->pposx - 64  < it->first && (info->pposy >= it->first - 64 && info->pposy <= it->first))
			{
				info->CurrentProp = it->first;
				info->PropCoord[it->first] = info->pposy; 
				break ;
			}
		}

	}
}

int	RanGen(int min, int max)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> distribution(min, max);
	int random = distribution(gen);
	return (random);
}


void renderDestroyables(game *game, Render *wind)
{

	map<int, bool>::iterator it = game->PropPos.begin();
	vector<SDL_Texture *>::iterator k = game->Props.begin();
	k++;
	for (; it != game->PropPos.end(); it++, k++)
	{
		if (!it->second)
			wind->renderTexture(*k, it->first, it->first, false);
		else
		{
			game->PropPos.erase(it);
			game->Props.erase(k);
		}
		
	}

}

void renderFlyEnemies(game *game, Render *wind, int i)
{	
	static Cooldown qCooldown(1000);
	vector<int>::iterator it = game->FlyPosx.begin();
	vector<int>::iterator ite =game->FlyPosy.begin();
	for (; it != game->FlyPosx.end(); it++, ite++)
	{
		if (*it > game->pposx)
			game->Fly_left = false;
		else
		game->Fly_left = true;
	
	
		if (game->currLife == 0)
			exit(printf("GAME OVER\n"));
		if (game->Fly_left)
		wind->renderTexture(game->FlyingEnR[i], *it, *ite, false);
		else
		wind->renderTexture(game->FlyingEnL[i], *it, *ite, false);
		
		if (*it > game->pposx + 64 && *it < game->pposx)
		{
			*it -= 3;
			*it += 4;
			*it -= 2;
		}
		if (*it < game->pposx + 64 && *it > game->pposx)
		{			
			*it += 3;
			*it -= 4;
			*it += 2;
			
		}
		if (game->FlyPosx.size() >= 10 && game->FlyPosx.size() <= 20)
		{
			*ite += 1;
			*it += 1;
		}
		if (game->FlyPosx.size() >= 100)
		{
			*ite += 3;
			*it += 3;
		}
		if ((int)*it % 2 == 0)
		{
			if (*it > game->pposx + 64)
				*it -= 1;
			if (*ite < game->pposy)
				*ite += 1;
			if (*it < game->pposx - 64)
				*it += 1;
			if (*ite > game->pposy)
				*ite -= 1;
		}
		else
		{
			if (*it > game->pposx + 48)
				*it -= 1;
			if (*ite < game->pposy)
				*ite += 1;
			if (*it < game->pposx - 48)
				*it += 1;
			if (*ite > game->pposy)
				*ite -= 1;

		}
		i++;
		if (i == game->FlyingEnR.size())
			i = 0;
	}

}


void renderStartingWeapon(game *game, Render *wind, Chest &chest1, SDL_KeyboardEvent* event)
{	
	
	if (chest1.isOpen())
	{
		static int i;
		static int k;
		if (i < 30)
		{
			if (i % 2 == 0)
			wind->renderTexture(chest1.ChestClosed[0], 348, 380, false);
			else
			wind->renderTexture(chest1.ChestClosed[0], 352, 380, false);

		}
		i++;
		if (i >= 30 && i <= 40)
		wind->renderTexture(chest1.ChestClosed[8], 352, 380, false);
		if (i >= 41 && i <= 44)
		{
			static int lol;
			wind->renderTexture(game->Sprinkle[lol], 352, 380, false);
			wind->renderTexture(game->Sprinkle[lol], 352, 380, false);	
			wind->renderTexture(game->Sprinkle[lol], 352, 380, false);
			lol++;
		}
		if (i >= 45)
		{
			wind->renderTexture(game->Smoke[k], 342, 360, false);
			k++;
			if (k == 19)
				k = 0;

			wind->renderTexture(game->base_sword, 350, 400, false);
			game->picked = false;
		}
	}

}

void renderStartingWeapon2(game *game, Render *wind, Chest &chest1, SDL_KeyboardEvent* event)
{	
	
	if (game->chest2.isOpen())
	{
		static int i;
		static int k;
		if (i < 30)
		{
			if (i % 2 == 0)
			wind->renderTexture(chest1.ChestClosed[0], 548, 380, false);
			else
			wind->renderTexture(chest1.ChestClosed[0], 552, 380, false);

		}
		i++;
		if (i >= 30 && i <= 40)
		wind->renderTexture(chest1.ChestClosed[8], 552, 380, false);
		if (i >= 41 && i <= 44)
		{
			static int lol;
			wind->renderTexture(game->Sprinkle[lol], 552, 380, false);
			wind->renderTexture(game->Sprinkle[lol], 552, 380, false);	
			wind->renderTexture(game->Sprinkle[lol], 552, 380, false);
			lol++;
		}
		if (i >= 45)
		{
			wind->renderTexture(game->SmokeG[k], 510, 290, false);
			k++;
			if (k == 19)
				k = 0;

			wind->renderTexture(game->base_wand, 550, 400, false);
			game->chest2.picked = false;
		}
	}

}

void renderThings(game *game, int Tf, SDL_Texture *currentTexture, Render *wind, int fps, Doors door1, Chest &chest1, SDL_KeyboardEvent *event)
{
	bool magickAnim = (currentTexture == game->MagickAnimR[0] ||
                   currentTexture == game->MagickAnimR[1] ||
                   currentTexture == game->MagickAnimR[2]);

	if (game->left && magickAnim)
	{
		wind->renderTexture(currentTexture, game->pposx, game->pposy, true);
	}
	else
	{
		wind->renderTexture(currentTexture, game->pposx, game->pposy, false);
	}
	wind->renderTexture(game->HealthBar[game->currLife], 0, 0, false);
	wind->renderTexture(game->TorchAnim[Tf], 430, 0, false);
	wind->renderTexture(game->TorchAnim[Tf], 530, 0, false);
	wind->renderTexture(game->TorchAnim[Tf], 530, 898, false);
	wind->renderTexture(game->TorchAnim[Tf], 130, 898, false);
	wind->renderTexture(game->Props[0], 480, 8, false);

	if (door1.getLock())
		wind->renderTexture(door1.DoorLocked[0], 580, 0, false);
	if (!chest1.isOpen())
	{
		static int l;
		static int ll;
		if (ll < 100)
		{
			wind->renderTexture(chest1.ChestClosed[0], 350, 380, false);
			ll++;
		}
		else
		{
			wind->renderTexture(chest1.ChestClosed[l], 350, 380, false);
			l++;
			if (l == chest1.ChestClosed.size() - 1)
			{
		 		l = 0;
				ll = 0;
			}
		}
	}
	if (!game->chest2.isOpen())
	{
				static int l;
		static int ll;
		if (ll < 100)
		{
			wind->renderTexture(chest1.ChestClosed[0], 550, 380, false);
			ll++;
		}
		else
		{
			wind->renderTexture(chest1.ChestClosed[l], 550, 380, false);
			l++;
			if (l == chest1.ChestClosed.size() - 1)
			{
		 		l = 0;
				ll = 0;
			}
		}

	}
	if (game->destroyed)
	{
		
		static int k;
		wind->renderTexture(game->Explosion[k], game->PropToDes, game->PropToDes, false);
		k++;
		if (k == 7)
		{
			game->CurrentProp = game->PropToDes;
			game->destroyed = false;
			game->CurrentProp = 0;
			k = 0;
		}
	}
	if (!game->W_equipped)
		renderStartingWeapon(game, wind, chest1, event);
	if (!game->chest2.W_equipped)
		renderStartingWeapon2(game, wind, chest1, event);
	renderDestroyables(game, wind);
	renderFlyEnemies(game, wind, fps);
	if (game->fSwordMSG)
		wind->renderTexture(game->firstSwordMSG, 350 , 450, false); 
	//CHEST 2
	if (game->chest2.WeaponMSG)
		wind->renderTexture(game->firstWandMSG, 350 , 450, false);
	if (game->choice)
	{
		if (game->currchoice == 0)
		wind->renderTexture(game->AcceptW, 350 , 450, false);
		else
		wind->renderTexture(game->RefuseW, 350, 450, false);
	}
	//CHEST 2
	if (game->chest2.choice)
	{
		if (game->chest2.currchoice == 0)
		wind->renderTexture(game->AcceptW, 350 , 450, false);
		else
		wind->renderTexture(game->RefuseW, 350, 450, false);
	}
	if (game->YouRecived)
	{
		game->overhead = true;
		wind->renderTexture(game->RecRustySword, 350, 450, false);
	}
	//CHEST 2
	if (game->chest2.YouRecived)
	{
		game->chest2.overhead = true;
		wind->renderTexture(game->RecOldRod, 350, 450, false);
	} 
	if (game->overhead)
	{	
		static int o = 255;
		if (o > 120)
			o-= 10;
		else
		o -= 2;
		if (game->left)
		{
			SDL_SetTextureAlphaMod(game->RoverL, o);
				wind->renderTexture(game->RoverR, game->pposx, game->pposy - 60, false);	
		}
		else
		{
			SDL_SetTextureAlphaMod(game->RoverR, o);
			wind->renderTexture(game->RoverL, game->pposx, game->pposy - 60, false);
		}

		if (o <= 0)
			game->overhead = false;
	}

}

void Collisions(game *info, std::vector<Projectile>& projectiles)
{
    // Teletrasporto
    if (info->pposx >= 525 && info->pposx <= 640 && info->pposy >= 0 && info->pposy <= 64)
    {
        int step = info->running ? 16 : 8;
        info->pposx += (info->pposx <= 528) ? -step : step;
        info->pposy += step;
    }

    // Chest 1
    if (info->pposx >= 300 && info->pposx <= 400 && info->pposy >= 350 && info->pposy <= 435 && !info->W_equipped)
    {
        if (!info->chest1open)
            info->Chest1Area = true;
        else
            info->SwordArea = true;

        int step = info->running ? 16 : 8;
        info->pposx += (info->pposx <= 305) ? -step : (info->pposx >= 400) ? step : 0;
        info->pposy += (info->pposy >= 400 && info->pposy <= 440) ? step : (info->pposy >= 344) ? -step : 0;
    }
    else if (info->pposx < 280 || info->pposx > 420 || info->pposy < 330 || info->pposy > 455)
    {
        info->Chest1Area = false;
    }

    // Chest 2
    if (info->pposx >= 500 && info->pposx <= 600 && info->pposy >= 350 && info->pposy <= 435 && !info->chest2.W_equipped)
    {
        if (!info->chest2.isOpen())
            info->chest2.ChestArea = true;
        else
            info->chest2.WeaponArea = true;

        int step = info->running ? 16 : 8;
        info->pposx += (info->pposx <= 505) ? -step : (info->pposx >= 600) ? step : 0;
        info->pposy += (info->pposy >= 400 && info->pposy <= 440) ? step : (info->pposy >= 344) ? -step : 0;
    }
    else if (info->pposx < 480 || info->pposx > 620 || info->pposy < 330 || info->pposy > 455)
    {
        info->chest2.ChestArea = false;
    }

    // Collisioni proiettili
    for (auto& p : projectiles)
    {
        if (p.hit) continue; 

        int px = p.x + p.cast;
        int py = p.y;


		int totx = (640 * 2);
		int toty = (480 * 2);
        bool hitWall = (px < 0 || px >= totx - 70 || py < 0 || py > toty);
        if (hitWall)
        {
            p.hit = true;
            p.frame = 7;
            p.frameDelay = 0;
            continue; 
        }

        bool chest1Closed = !info->chest1open && (px >= 300 && px <= 400 && py >= 350 && py <= 435);
        bool chest2Closed = !info->chest2.isOpen() && (px >= 500 && px <= 600 && py >= 350 && py <= 435);


        bool hitFly = false;
        {
            auto itx = info->FlyPosx.begin();
            auto ity = info->FlyPosy.begin();
            for (; itx != info->FlyPosx.end() && ity != info->FlyPosy.end(); ++itx, ++ity)
            {
                int flyx = *itx;
                int flyy = *ity;
                if (abs(px - flyx) <= 20 && abs(py - flyy) <= 20)
                {
                    hitFly = true;
					info->FlyLightHit = true;
                    break;
                }
            }
        }

        if (chest1Closed || chest2Closed || hitFly)
        {
            p.hit = true;
            p.frame = 7;
            p.frameDelay = 0;
            continue;
        }


		hitWall = (px < 0 || px >= totx - 70 || py < 0 || py > toty);

        if (hitWall)
        {
            p.hit = true;
            p.frame = 7;
            p.frameDelay = 0;
            continue;
        }
    }
	for (auto& p : projectiles)
	{
		if (p.hit) continue;

		int px = p.x + p.cast;  
		int py = p.y;           

		const int radius = 60;
		const int radiusSquared = radius * radius;

		for (const auto& prop : info->PropCoord)  
		{
			int dx = px - prop.first;
			int dy = py - prop.second;

			int distSquared = dx * dx + dy * dy;

			if (distSquared <= radiusSquared)
			{
				p.hit = true;
				p.frame = 7;
				p.frameDelay = 0;
				break;
			}
		}
	}
}


void Collisions(game *info)
{
	static std::vector<Projectile> dummy;
	Collisions(info, dummy);
}


void wand_anim(game *info, SDL_Texture **currentTexture)
{
	static Cooldown attack_cd(1000);
	static bool attack_anim_active = false;
	static int i = 0;

	if (!info->wand_cd && info->chest2.attack && info->idle)
	{
		attack_cd.activate();
		info->wand_cd = true;
		attack_anim_active = true;

		i = 0;
	}
	if (attack_anim_active)
	{
		if (i == 10) {
			info->launch = true;
			info->chest2.attack = false;
			info->fixedpos = info->pposy;
		}
		
		if (attack_cd.getTime() >= 0.5 && i > 10) {
			attack_anim_active = false;
			info->wand_cd = false;
			attack_cd.reset();
			i = 0;
			return;
		}
		else {
			if (i < 3)
				*currentTexture = info->MagickAnimR[i];
			else if (i >= 4 && i <= 9)
				*currentTexture = info->MagickAnimR[2];

			i++;
		}
	}
	else
	{
		return;
	}
}


SDL_Texture*Animations(SDL_Texture *currentTexture, game *info, int currentFrame, int &as)
{	

	bool colliding = false;
	static int tookDmg;
	LFProp(info);


	if (info->pposx >= 0 && info->pposx <= 182 && info->pposy >= 0 && info->pposy <= 48)
		SDL_SetTextureAlphaMod(info->HealthBar[info->currLife], 100);
	else
		SDL_SetTextureAlphaMod(info->HealthBar[info->currLife], 255);
	if (info->pposx >= 430 && info->pposx <= 500 && info->pposy >= 0 && info->pposy <= 64)
		SDL_SetTextureAlphaMod(info->Props[0], 100);
	else
	SDL_SetTextureAlphaMod(info->Props[0], 255);
	
	Collisions(info);

	if (info->idle && info->Hattk && info->W_equipped)
	{	
		if (!info->left)
		{
			static int j;
			if (!info->Release)
			{
				currentTexture = info->Heavy[j];
				j++;
				if (j == 3)
					j = 0;
			}
		}
		else
		{
			static int j;
			if (!info->Release)
			{
				currentTexture = info->HeavyL[j];
				j++;
				if (j == 3)
					j = 0;
			}
		}
		

	}
	if (info->idle && !info->Hattk && info->Hattak2 && info->W_equipped)
	{	
		if (!info->left)
		{
			static int j = 3;
			currentTexture = info->Heavy[j];
			j++;
			if (j == 6)
			{
				j = 3;
				info->Hattak2 = false;
			}
		}
		else
		{
			static int j = 3;
			currentTexture = info->HeavyL[j];
			j++;
			if (j == 6)
			{
				j = 3;
				info->Hattak2 = false;
			}
		}
	}

	if (info->attack && info->W_equipped)
	{	
		if (!info->left)
		{
		static int i = 2;
		currentTexture = info->AttackRAnim[i];
		i++;
		if ( i == 4 )
			i = 2;
		}
		else
		{
			static int l;
			currentTexture = info->AttackLAnim[l];
			l++;
			if ( l == 2 )
				l = 0;
		}
	} 
	if (info->idle == true && info->left == false && info->attack == false && !info->Hattk && !info->Hattak2 && !info->chest2.attack)
		currentTexture = info->playeridleAnimation[currentFrame];
	if (info->idle == true && info->left == true && info->attack == false && !info->Hattk && !info->Hattak2 && !info->chest2.attack)
		currentTexture = info->playeridleAnimationL[currentFrame];
	if (info->moveRight == true)
	{	
		info->idle = false;
		currentTexture = info->RunRAnim[currentFrame];
		if (info->pposx >= (info->CurrentProp - 60) && info->pposx <= (info->CurrentProp + 50) && info->pposy + 40 > info->CurrentProp && info->pposy - 40 < info->CurrentProp)
			colliding = true;
		if (info->pposx < 1280 - 128 && info->running == false && !colliding)
			info->pposx += 8;
		else if (info->pposx < 1280 - 128 && info->running == true && !colliding)
			info->pposx += 16;
	}
	if (info->moveLeft)
	{	
		info->idle = false;
		currentTexture = info->RunLAnim[currentFrame];
		if (info->pposx <= (info->CurrentProp + 60) && info->pposx > (info->CurrentProp - 50) && info->pposy >= (info->CurrentProp - 40) && info->pposy <= (info->CurrentProp + 40)) 
			colliding = true;
		if (info->pposx > 64 && info->running == false && !colliding)
			info->pposx -= 8;
		if (info->pposx > 64 && info->running == true && !colliding)
			info->pposx -= 16;
	}
	if (info->moveUp == true && info->left == true)
	{
		info->idle = false;
		currentTexture = info->RunLAnim[currentFrame];
		if (info->pposy - 60 < info->CurrentProp && info->pposy - 44 > info->CurrentProp && info->pposx >= (info->CurrentProp - 40) && info->pposx <= (info->CurrentProp + 40))
			colliding = true;
		if (info->pposy > 12 && info->running == false && !colliding)
			info->pposy -= 8;
		if (info->pposy > 12 && info->running == true && !colliding)
			info->pposy -= 16;
	}
	if (info->moveUp == true && info->left == false)
	{
		info->idle = false;
		currentTexture = info->RunRAnim[currentFrame];
		if (info->pposy - 60 < info->CurrentProp && info->pposy - 44 > info->CurrentProp && info->pposx >= (info->CurrentProp - 40) && info->pposx <= (info->CurrentProp + 40))
			colliding = true;
		if (info->pposy > 12 && info->running == false && !colliding)
			info->pposy -= 8;
		if (info->pposy > 12 && info->running == true && !colliding)
			info->pposy -= 16;
	}
	if (info->moveDown == true && info->left == true)
	{
		info->idle = false;
		currentTexture = info->RunLAnim[currentFrame];
		if (info->pposy + 64 > info->CurrentProp && info->pposy - 44 < info->CurrentProp && info->pposx >= (info->CurrentProp - 40) && info->pposx <= (info->CurrentProp + 40))
			colliding = true;

		if (info->pposy < 960 - 128 && info->running == false && !colliding)
			info->pposy += 8;
		if (info->pposy < 960 - 128 && info->running == true && !colliding)
			info->pposy += 16;
	}
	if (info->moveDown == true && info->left == false)
	{		
		info->idle = false;			
		currentTexture = info->RunRAnim[currentFrame];
		if (info->pposy + 64 > info->CurrentProp && info->pposy - 44 < info->CurrentProp && info->pposx >= (info->CurrentProp - 40) && info->pposx <= (info->CurrentProp + 48))
			colliding = true;
		if (info->pposy < 960 - 128 && info->running == false && !colliding)
			info->pposy += 8;
		if (info->pposy < 960 - 128 && info->running == true && !colliding)
			info->pposy += 16;
	}

	
	if (info->takeDmg)
	{

		tookDmg++;
		if (tookDmg >= 1 && tookDmg <= 15 && !info->left)
			currentTexture = info->Hurt[2];
		if (tookDmg >= 16 && tookDmg <= 30 && !info->left)
			currentTexture = info->Hurt[0];
		if (tookDmg >= 31 && tookDmg <= 40 && !info->left)
			currentTexture = info->Hurt[2];
		if (tookDmg >= 41 && tookDmg <= 50 && !info->left)
			currentTexture = info->Hurt[1];
		if (tookDmg >= 31 && tookDmg <= 50 && !info->left)
			currentTexture = info->Hurt[2];
			//LEFT
		if (tookDmg >= 1 && tookDmg <= 15 && info->left)
			currentTexture = info->Hurt[5];
		if (tookDmg >= 16 && tookDmg <= 30 && info->left)
			currentTexture = info->Hurt[3];
		if (tookDmg >= 31 && tookDmg <= 40 && info->left)
			currentTexture = info->Hurt[5];
		if (tookDmg >= 41 && tookDmg <= 50 && info->left)
			currentTexture = info->Hurt[4];
		if (tookDmg >= 41 && tookDmg <= 50 && info->left)
			currentTexture = info->Hurt[5];

		if (tookDmg % 8 == 0)
		SDL_SetTextureAlphaMod(currentTexture,  0);
		else 
		SDL_SetTextureAlphaMod(currentTexture,  255);			
		if (tookDmg == 51)
		{
			info->takeDmg = false;
			tookDmg = 0;
		}
	}
	else
		SDL_SetTextureAlphaMod(currentTexture,  255);
	if (info->chest2.W_equipped)
	{
		wand_anim(info, &currentTexture);
	}

	return (currentTexture);

}


void PopUpMSG(game *info, Render *wind, SDL_KeyboardEvent* event)
{

	info->fSwordMSG = true;

}


void pickWeaponLogickp2(game *info, SDL_KeyboardEvent *event, Render *wind)
{
	if (info->chest2.choice)
	{
		if (event->keysym.sym == SDLK_RIGHT)
		{
			info->chest2.choice = true;
			info->chest2.currchoice = 1;
		}
		if (event->keysym.sym == SDLK_LEFT)
		{
			info->chest2.choice = true;
			info->chest2.currchoice = 0;
		}
		if (event->keysym.sym == SDLK_RETURN)
		{
			info->chest2.choice = false;
			info->chest2.WeaponMSG = false;
			info->chest2.ChestArea = false;
			if (info->chest2.currchoice == 0)
				info->chest2.setOpen();
		}
	}
	if (info->chest2.WeaponMSG)
		info->chest2.choice = true;

	if (info->chest2.ChestArea)
	{	
		if (event->keysym.sym == SDLK_e)
			info->chest2.WeaponMSG = true;
		else
		info->chest2.WeaponMSG = false;
	}
	else
	info->chest2.choice = false;
	if (info->chest2.WeaponArea && !info->chest2.picked && !info->chest2.W_equipped)
	{
		if (event->keysym.sym == SDLK_e)
			info->chest2.picked = true;

	}
	else if (!info->chest2.ChestArea)
		info->chest2.WeaponArea = false;
	if (info->chest2.picked)
	{
		info->chest2.W_equipped = true;
		info->chest2.YouRecived = true;
	}
	if (info->chest2.YouRecived)
	{
		static int k;
		k++;
		if (event->keysym.sym == SDLK_e)
		{
			if (k > 1)
				info->chest2.YouRecived = false;

			info->chest2.picked = false;
		}
	}

}

void pickWeaponLogick(game *info, SDL_KeyboardEvent *event, Chest &chest1, Render *wind)
{
	if (info->choice)
	{
		if (event->keysym.sym == SDLK_RIGHT)
		{
			info->choice = true;
			info->currchoice = 1;
		}
		if (event->keysym.sym == SDLK_LEFT)
		{
			info->choice = true;
			info->currchoice = 0;
		}
		if (event->keysym.sym == SDLK_RETURN)
		{
			info->choice = false;
			info->fSwordMSG = false;
			info->Chest1Area = false;
			if (info->currchoice == 0)
			{
				chest1.setOpen();
				info->chest1open = true;
			}
		}
	}
	if (info->fSwordMSG)
		info->choice = true;


	if (info->Chest1Area)
	{	
		if (event->keysym.sym == SDLK_e)
			PopUpMSG(info, wind, event);
		else
		info->fSwordMSG = false;
	}
	else
	info->choice = false;
	if (info->SwordArea && !info->picked && !info->W_equipped)
	{
		if (event->keysym.sym == SDLK_e)
			info->picked = true;
	}
	else if (!info->SwordArea)
		info->SwordArea = false;
	if (info->picked)
	{
		info->W_equipped = true;
		info->YouRecived = true;
	}
	if (info->YouRecived)
	{
		static int k;
		k++;
		if (event->keysym.sym == SDLK_e)
		{
			if (k > 1)
				info->YouRecived = false;

			info->picked = false;
		}
	}
	pickWeaponLogickp2(info, event, wind);


}


void WandAttack(game *info, SDL_KeyboardEvent* event, const Uint8 *currentKeyState, Uint32 &spaceKeyDownTime, float elapsedSeconds, Uint32 currentTime, SDL_Texture * currentTexture)
{
	if (event->keysym.sym == SDLK_SPACE && info->chest2.W_equipped) 
	{
		if (!currentKeyState[SDL_SCANCODE_SPACE])
		{

		// Space key was released
		spaceKeyDownTime = 0; // Reset the space key down time
		} 
		else 
		{	
			if (elapsedSeconds < 1 && currentTime / 1000.0f > 1  && info->launch && info->idle)
			{	
				info->chest2.Sattack = true;
				info->fixedpos = info->pposy;
				elapsedSeconds = 0;
				spaceKeyDownTime = 0;
			}
			else
			{
				if (!info->wand_cd)
					info->chest2.attack = true;
				spaceKeyDownTime = 0;
				elapsedSeconds = 0;
			} 
		// Space key is still held down
		if (spaceKeyDownTime == 0) {
			spaceKeyDownTime = currentTime; // Set the space key down time
		}
		}
	}
}


void manageKEYBOARD(SDL_KeyboardEvent* event, Render* wind, game* info, int& animationSpeed, Chest &chest1, SDL_Texture * currentTexture) 
{
const Uint8* currentKeyState = SDL_GetKeyboardState(nullptr);
static Uint32 spaceKeyDownTime = 0; // Track the time when the space key was pressed
static Uint32 qKeyDt = 0;
static Cooldown qCooldown(1000);
static Cooldown IdleCooldown(1000);
static int reset = 0;

Uint32 currentTime = SDL_GetTicks(); // Get the current time in milliseconds
float elapsedSeconds = (currentTime - spaceKeyDownTime) / 1000.0f; // Calculate the elapsed time since the space key was pressed
float elapseQ = (currentTime - qKeyDt) / 1000.0f;
if (event->type == SDL_KEYDOWN) {	
	if (event->keysym.sym == SDLK_ESCAPE) {
		wind->cleanUp(info);
		exit(0);
	}
	if (event->keysym.sym == SDLK_d && !info->CurrentAttack) {
		info->moveRight = true;
		info->moveLeft = false;
		info->idle = false;
		info->left = false;
		animationSpeed = 60;
		info->fSwordMSG = false;
		info->chest2.WeaponMSG = false;
		IdleCooldown.activate();
		
	}
	if (event->keysym.sym == SDLK_a && !info->CurrentAttack) {
		info->moveLeft = true;
		info->moveRight = false;
		info->idle = false;
		info->left = true;
		animationSpeed = 60;
		info->fSwordMSG = false;
		info->chest2.WeaponMSG = false;
		IdleCooldown.activate();
	}
	if (event->keysym.sym == SDLK_w && !info->CurrentAttack) {
		info->moveUp = true;
		info->moveDown = false;
		info->idle = false;
		animationSpeed = 60;
		info->fSwordMSG = false;
		info->chest2.WeaponMSG = false;
		IdleCooldown.activate();		
	}
	if (event->keysym.sym == SDLK_s && !info->CurrentAttack) {
		info->moveDown = true;
		info->moveUp = false;
		info->idle = false;
		animationSpeed = 60;
		info->fSwordMSG = false;
		info->chest2.WeaponMSG = false;	
		IdleCooldown.activate();	
	}
	if (!info->moveDown && !info->moveLeft && !info->moveRight && !info->moveUp  && info->W_equipped && event->keysym.sym == SDLK_q)
	{
		if (qCooldown.isReady())
		{		
			if (!currentKeyState[SDL_SCANCODE_Q])
			{	
				qCooldown.activate();
				qKeyDt = 0;
				info->Release = true;
			}
			else
			{
				if (elapseQ <= 2.5 && currentTime / 1000.0f > 1)
				{	
					info->Hattk = true;
					info->Hattak2 = true;
					elapseQ = 0;
					qKeyDt = 0;
				}
				else
				{
					info->Hattk = false;
					qKeyDt = 0;
					elapseQ = 0;
				}
				if (qKeyDt == 0)
					qKeyDt = currentTime;
			}
		}

	}
	if (event->keysym.sym == SDLK_SPACE && info->W_equipped) 
	{
		if (!currentKeyState[SDL_SCANCODE_SPACE])
		{

		// Space key was released
		spaceKeyDownTime = 0; // Reset the space key down time
		} 
		else 
		{	
			if (elapsedSeconds < 1 && currentTime / 1000.0f > 1)
			{	
				info->Sattack = true;
				elapsedSeconds = 0;
				spaceKeyDownTime = 0;
			}
			else
			{
				info->attack = true;
				spaceKeyDownTime = 0;
				elapsedSeconds = 0;
			} 
		// Space key is still held down
		if (spaceKeyDownTime == 0) {
			spaceKeyDownTime = currentTime; // Set the space key down time
		}
		}
	}
	pickWeaponLogick(info, event, chest1, wind);
	WandAttack(info, event, currentKeyState, spaceKeyDownTime, elapsedSeconds, currentTime, currentTexture);
	if (currentKeyState[SDL_SCANCODE_LSHIFT]) {
		animationSpeed = 60;
		info->running = true;
	} else {
		info->idle = true;
	}
}

if (event->type == SDL_KEYUP) {
	if (info->W_equipped && event->keysym.sym == SDLK_q && IdleCooldown.getTime() > 0  && !info->moveDown && !info->moveLeft && !info->moveRight && !info->moveUp)
	{
 		if (info->idle && qCooldown.getTime() >= 2.5)
			info->Release = true;
		qCooldown.activate();
		currentKeyState = SDL_GetKeyboardState(nullptr);
	}
	if (event->keysym.sym == SDLK_d)
	{
		info->moveRight = false;
		info->fSwordMSG = false;
	}
	if (event->keysym.sym == SDLK_a)
	{
		info->moveLeft = false;
		info->fSwordMSG = false;
	}
	if (event->keysym.sym == SDLK_w)
	{
		info->moveUp = false;
		info->fSwordMSG = false;
	}
	if (event->keysym.sym == SDLK_s)
	{
		info->moveDown = false;
		info->fSwordMSG = false;
	}
	
	info->idle = true;
	info->running = false;
	info->attack = false;
	animationSpeed = 70;
	info->Sattack = false;
	info->Hattk = false;
	spaceKeyDownTime = 0; // Reset the space key down time
}
}


void moreEvents(game *info, Render *wind, SDL_Texture * currentTexture)
{
    static std::vector<Projectile> projectiles;

    if (info->chest2.W_equipped)
    {
        if (info->launch)
        {
			projectiles.push_back({info->pposx, info->pposy, 0, false, 3, 0, !info->left});
            info->launch = false;
        }

        for (size_t idx = 0; idx < projectiles.size(); )
        {
            auto& p = projectiles[idx];

            int animFrame;
            if (p.hit)
                animFrame = std::min(p.frame, 11);
            else
                animFrame = 3 + ((p.frame - 3) % 4);

            wind->renderTexture(info->MagickAnimR[animFrame], p.x + p.cast, p.y + 10, !p.directionRight);

            p.frameDelay++;
            if (p.frameDelay >= 2) {
                p.frame++;
                p.frameDelay = 0;
            }

            if (!p.hit)
            {

                if (p.directionRight)
                    p.cast += 25;  
                else
                    p.cast -= 25; 
            }

            if ((p.hit && p.frame > 11) || (!p.hit && (p.cast > 1500 || p.cast < -1500)))
                projectiles.erase(projectiles.begin() + idx);
            else
                ++idx;
        }

        Collisions(info, projectiles);
    }
}

void checkForEvents(game *info, Render *wind, int &As, SDL_Texture * currentTexture)
{
	map<int, bool>::iterator it = info->PropPos.begin();
	map<int, bool>::iterator i = info->PropPos.find(info->CurrentProp);
	if (info->CurrentProp == 0)
		info->CurrentProp = it->first;
	if (info->idle && info->Release)
	{	

		if (!info->left)
		{	
			for (; it != info->PropPos.end(); it++)
			{
				if ((it->first - info->pposx >= 0 && it->first - info->pposx <= 200) && ((it->first - info->pposy >= 0 && it->first - info->pposy <= 64) || (info->pposy - it->first >= 0 && info->pposy - it->first <= 64)))
				{
						i = info->PropPos.find(it->first);
						info->PropToDes = it->first;
						info->destroyed = true;
						i->second = true;
				
				}
			}
		}
		else
		{
			for (; it != info->PropPos.end(); it++)
			{
				if ((info->pposx - it->first >= 0 && info->pposx - it->first <= 200) && ((it->first - info->pposy >= 0 && it->first - info->pposy <= 64) || (info->pposy - it->first >= 0 && info->pposy - it->first <= 64)))
				{
					i = info->PropPos.find(it->first);
					info->PropToDes = it->first;
					info->destroyed = true;
					i->second = true;	
				}
			}

		}
	}

	// PLAYER DAMAGES FLYING ENEMY
	if (info->FlyLightHit)
	{	
		info->FlyPosx.push_back(RanGen(0, 1280));
		info->FlyPosy.push_back(RanGen(0, 960));
		static int k;
		if (k > 0)
		wind->renderTexture(info->FlyDeath[k], info->currFlyingX, info->currFlyingY, false);
		k++;
		if (k == info->FlyDeath.size())
		{	
			info->FlyLightHit = false;
			k = 0;
		}
	}
	moreEvents(info, wind, currentTexture);
}

int main(int ac, char **av)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) > 0)
	{
		cout << "SDL_Init Error: " << SDL_GetError() << endl;
		return 1;
	}
	if (!IMG_Init(IMG_INIT_PNG))
	{
		cout << "IMG_Init Error: " << SDL_GetError() << endl;
		SDL_Quit();
		return 1;
	}
	Render wind("Game", 640 * 2, 480 * 2);
	int windowREfreshRate = wind.getRefreshRate();
	// INIT TEXTURES
/* 		SDL_Texture *floorTex = wind.loadTexture("img/bg/floor_1.png");  */
	SDL_Texture *wallTex = wind.loadTexture("img/bg/wall_1.png");
	SDL_Texture *wall2Tex = wind.loadTexture("img/bg/wall_2.png");
	SDL_Texture *wall3Tex = wind.loadTexture("img/bg/wall_3.png");
	
	// OBJECTS And STRUCTURE
	game game;
	Doors door1;
	Chest chest1;
	
	//FLYING ENEMIS COORDINATES

	
	for (int i = 0; i < 3; i++)
	{
		game.FlyPosx.push_back(RanGen(0, 1280));
		if (i % 2 == 0)
		game.FlyPosy.push_back(RanGen (0, 24));
		else
		game.FlyPosy.push_back(RanGen (900, 960));
	}



	game.pposx = 128;
	game.pposy = 128;
	game.CurrentProp = 0;
	game.left = false;
	game.moveLeft = false;
	game.moveRight = false;
	game.moveUp = false;
	game.moveDown = false;
	game.idle = true;
	game.running = false;
	game.attack = false;
	game.Sattack = false;
	game.Hattk = false;
	game.Release = false;
	game.Hattak2 = false;
	game.destroyed = false;
	game.Fly_left = false;
	game.takeDmg = false;
	game.FlyLightHit = false;
	game.W_equipped = false;
	game.Chest1Area = false;
	game.BatD = false;
	game.currchoice = 0;
	game.choice = false;
	game.fSwordMSG = false;
	game.chest1open = false;
	game.picked = false;
	game.SwordArea = false;
	game.overhead = false;
	game.YouRecived = false;
	int rand2 = RanGen(201, 300);
	game.randProp = RanGen(701, 850);
	game.currLife = 5;
	game.fixedpos = 2000;
	game.launch = false;
	game.wand_cd = false;


	// PROP POSITION INSERT
	game.PropPos.insert(make_pair(game.randProp, false));
	game.PropPos.insert(make_pair(rand2, false));
	game.PropCoord.insert(make_pair(0, 0));

//LOAD Hurt frame
	game.Hurt.push_back(wind.loadTexture("img/pl/right/hurt1.png"));
	game.Hurt.push_back(wind.loadTexture("img/pl/right/hurt.png"));
	game.Hurt.push_back(wind.loadTexture("img/pl/right/hurt2.png"));
	game.Hurt.push_back(wind.loadTexture("img/pl/left/hurt1L.png"));
	game.Hurt.push_back(wind.loadTexture("img/pl/left/hurtL.png"));
	game.Hurt.push_back(wind.loadTexture("img/pl/left/hurt2L.png"));
//load Floor
	game.Floor.push_back(wind.loadTexture("img/bg/floor_1.png"));
	game.Floor.push_back(wind.loadTexture("img/bg/floor_2.png"));
	game.Floor.push_back(wind.loadTexture("img/bg/floor_3.png"));
	game.Floor.push_back(wind.loadTexture("img/bg/floor_4.png"));	
// Load the player idle animation textures
	game.playeridleAnimation.push_back(wind.loadTexture("img/pl/right/pidle0.png"));
	game.playeridleAnimation.push_back(wind.loadTexture("img/pl/right/pidle0.png"));
	game.playeridleAnimation.push_back(wind.loadTexture("img/pl/right/pidle1.png"));
	game.playeridleAnimation.push_back(wind.loadTexture("img/pl/right/pidle1.png"));
	game.playeridleAnimation.push_back(wind.loadTexture("img/pl/right/pidle2.png"));
	game.playeridleAnimation.push_back(wind.loadTexture("img/pl/right/pidle2.png"));	
	game.playeridleAnimation.push_back(wind.loadTexture("img/pl/right/pidle5.png"));
	game.playeridleAnimation.push_back(wind.loadTexture("img/pl/right/pidle5.png"));
	game.playeridleAnimation.push_back(wind.loadTexture("img/pl/right/pidle4.png"));
	game.playeridleAnimation.push_back(wind.loadTexture("img/pl/right/pidle4.png"));
//Load the player idle left animation textures

	game.playeridleAnimationL.push_back(wind.loadTexture("img/pl/left/pidle0l.png"));
	game.playeridleAnimationL.push_back(wind.loadTexture("img/pl/left/pidle0l.png"));
	game.playeridleAnimationL.push_back(wind.loadTexture("img/pl/left/pidle1l.png"));
	game.playeridleAnimationL.push_back(wind.loadTexture("img/pl/left/pidle1l.png"));
	game.playeridleAnimationL.push_back(wind.loadTexture("img/pl/left/pidle2l.png"));
	game.playeridleAnimationL.push_back(wind.loadTexture("img/pl/left/pidle2l.png"));
	game.playeridleAnimationL.push_back(wind.loadTexture("img/pl/left/pidle5l.png"));
	game.playeridleAnimationL.push_back(wind.loadTexture("img/pl/left/pidle5l.png"));
	game.playeridleAnimationL.push_back(wind.loadTexture("img/pl/left/pidle4l.png"));
	game.playeridleAnimationL.push_back(wind.loadTexture("img/pl/left/pidle4l.png"));

// Load the player running right animation textures
	game.RunRAnim.push_back(wind.loadTexture("img/pl/right/runR0.png"));
	game.RunRAnim.push_back(wind.loadTexture("img/pl/right/runR1.png"));
	game.RunRAnim.push_back(wind.loadTexture("img/pl/right/runR1.png"));
	game.RunRAnim.push_back(wind.loadTexture("img/pl/right/runR2.png"));
	game.RunRAnim.push_back(wind.loadTexture("img/pl/right/runR2.png"));
	game.RunRAnim.push_back(wind.loadTexture("img/pl/right/runR3.png"));
	game.RunRAnim.push_back(wind.loadTexture("img/pl/right/runR3.png"));
	game.RunRAnim.push_back(wind.loadTexture("img/pl/right/runR4.png"));
	game.RunRAnim.push_back(wind.loadTexture("img/pl/right/runR4.png"));
	game.RunRAnim.push_back(wind.loadTexture("img/pl/right/runR0.png"));
// Load the player running left animation textures
	game.RunLAnim.push_back(wind.loadTexture("img/pl/left/runL0.png"));
	game.RunLAnim.push_back(wind.loadTexture("img/pl/left/runL1.png"));
	game.RunLAnim.push_back(wind.loadTexture("img/pl/left/runL1.png"));
	game.RunLAnim.push_back(wind.loadTexture("img/pl/left/runL2.png"));
	game.RunLAnim.push_back(wind.loadTexture("img/pl/left/runL2.png"));
	game.RunLAnim.push_back(wind.loadTexture("img/pl/left/runL3.png"));
	game.RunLAnim.push_back(wind.loadTexture("img/pl/left/runL3.png"));
	game.RunLAnim.push_back(wind.loadTexture("img/pl/left/runL4.png"));
	game.RunLAnim.push_back(wind.loadTexture("img/pl/left/runL4.png"));
	game.RunLAnim.push_back(wind.loadTexture("img/pl/left/runL0.png"));
// Load the player attacking to the right animation textures
	game.AttackRAnim.push_back(wind.loadTexture("img/pl/right/attack00R.png"));
	game.AttackRAnim.push_back(wind.loadTexture("img/pl/right/attack02R.png"));	
	game.AttackRAnim.push_back(wind.loadTexture("img/pl/right/attack01R.png"));
	game.AttackRAnim.push_back(wind.loadTexture("img/pl/right/attack0R.png"));
	game.AttackRAnim.push_back(wind.loadTexture("img/pl/right/attack1R.png"));
	game.AttackRAnim.push_back(wind.loadTexture("img/pl/right/a2R.png"));
	game.AttackRAnim.push_back(wind.loadTexture("img/pl/right/attack3R.png"));
	game.AttackRAnim.push_back(wind.loadTexture("img/pl/right/attack1R.png"));	
	game.AttackRAnim.push_back(wind.loadTexture("img/pl/right/attack3R.png"));
// Load the player attacking to the left animation textures
	game.AttackLAnim.push_back(wind.loadTexture("img/pl/left/attack0L.png"));
	game.AttackLAnim.push_back(wind.loadTexture("img/pl/left/attack01L.png"));
	game.AttackLAnim.push_back(wind.loadTexture("img/pl/left/attack02L.png"));
	game.AttackLAnim.push_back(wind.loadTexture("img/pl/left/attack03L.png"));
	game.AttackLAnim.push_back(wind.loadTexture("img/pl/left/attack04L.png"));

// Load the player healthbar
	game.HealthBar.push_back(wind.loadTexture("img/bg/health_ui0H.png"));	
	game.HealthBar.push_back(wind.loadTexture("img/bg/health_ui1H.png"));
	game.HealthBar.push_back(wind.loadTexture("img/bg/health_ui2H.png"));
	game.HealthBar.push_back(wind.loadTexture("img/bg/health_ui3H.png"));
	game.HealthBar.push_back(wind.loadTexture("img/bg/health_ui4H.png"));
	game.HealthBar.push_back(wind.loadTexture("img/bg/health_ui.png"));
//Load heavy strike
	game.Heavy.push_back(wind.loadTexture("img/pl/right/attack00R.png"));
	game.Heavy.push_back(wind.loadTexture("img/pl/right/Charge0.png"));
	game.Heavy.push_back(wind.loadTexture("img/pl/right/Charge1.png"));
	game.Heavy.push_back(wind.loadTexture("img/pl/right/attack01R.png"));
	game.Heavy.push_back(wind.loadTexture("img/pl/right/attack01R.png"));
	game.Heavy.push_back(wind.loadTexture("img/pl/right/CHARGE4.png"));
	game.Heavy.push_back(wind.loadTexture("img/pl/right/CHARGE4.png"));
	game.Heavy.push_back(wind.loadTexture("img/pl/right/CHARGE4.png"));
	game.Heavy.push_back(wind.loadTexture("img/pl/right/CHARGE5.png"));
	game.Heavy.push_back(wind.loadTexture("img/pl/right/CHARGE5.png"));
	game.Heavy.push_back(wind.loadTexture("img/pl/right/CHARGE6.png"));
//LOAD heave LEFT strike
	game.HeavyL.push_back(wind.loadTexture("img/pl/left/attack00L.png"));
	game.HeavyL.push_back(wind.loadTexture("img/pl/left/Charge0L.png"));
	game.HeavyL.push_back(wind.loadTexture("img/pl/left/Charge1L.png"));
	game.HeavyL.push_back(wind.loadTexture("img/pl/left/attack01L.png"));
	game.HeavyL.push_back(wind.loadTexture("img/pl/left/attack01L.png"));
	game.HeavyL.push_back(wind.loadTexture("img/pl/left/CHARGE4L.png"));
	game.HeavyL.push_back(wind.loadTexture("img/pl/left/CHARGE4L.png"));
	game.HeavyL.push_back(wind.loadTexture("img/pl/left/CHARGE4L.png"));
	game.HeavyL.push_back(wind.loadTexture("img/pl/left/CHARGE5L.png"));
	game.HeavyL.push_back(wind.loadTexture("img/pl/left/CHARGE5L.png"));
	game.HeavyL.push_back(wind.loadTexture("img/pl/left/CHARGE6L.png"));
//LOAD torch prop
	game.TorchAnim.push_back(wind.loadTexture("img/props/torch0.png"));
	game.TorchAnim.push_back(wind.loadTexture("img/props/torch1.png"));
	game.TorchAnim.push_back(wind.loadTexture("img/props/torch2.png"));
	game.TorchAnim.push_back(wind.loadTexture("img/props/torch3.png"));
	game.TorchAnim.push_back(wind.loadTexture("img/props/torch4.png"));
	game.TorchAnim.push_back(wind.loadTexture("img/props/torch5.png"));
// LOAD varie Props
	game.Props.push_back(wind.loadTexture("img/props/prisoner.png"));
	game.Props.push_back(wind.loadTexture("img/props/table.png"));
	game.Props.push_back(wind.loadTexture("img/props/barrel.png"));



// LOAD Explosion
	game.Explosion.push_back(wind.loadTexture("img/effects/destroy/ex0.png"));
	game.Explosion.push_back(wind.loadTexture("img/effects/destroy/ex1.png"));
	game.Explosion.push_back(wind.loadTexture("img/effects/destroy/ex2.png"));
	game.Explosion.push_back(wind.loadTexture("img/effects/destroy/ex3.png"));
	game.Explosion.push_back(wind.loadTexture("img/effects/destroy/ex4.png"));
	game.Explosion.push_back(wind.loadTexture("img/effects/destroy/ex5.png"));
	game.Explosion.push_back(wind.loadTexture("img/effects/destroy/ex6.png"));
// LOAD flying enemy RIGHT
	game.FlyingEnR.push_back(wind.loadTexture("img/enemy/flying/right/fly0R.png"));
	game.FlyingEnR.push_back(wind.loadTexture("img/enemy/flying/right/fly0R.png"));
	game.FlyingEnR.push_back(wind.loadTexture("img/enemy/flying/right/fly0R.png"));
	game.FlyingEnR.push_back(wind.loadTexture("img/enemy/flying/right/fly0R.png"));
	game.FlyingEnR.push_back(wind.loadTexture("img/enemy/flying/right/fly0R.png"));
	game.FlyingEnR.push_back(wind.loadTexture("img/enemy/flying/right/fly1R.png"));
	game.FlyingEnR.push_back(wind.loadTexture("img/enemy/flying/right/fly1R.png"));
	game.FlyingEnR.push_back(wind.loadTexture("img/enemy/flying/right/fly1R.png"));
	game.FlyingEnR.push_back(wind.loadTexture("img/enemy/flying/right/fly1R.png"));
	game.FlyingEnR.push_back(wind.loadTexture("img/enemy/flying/right/fly1R.png"));	
	game.FlyingEnR.push_back(wind.loadTexture("img/enemy/flying/right/fly2R.png"));
	game.FlyingEnR.push_back(wind.loadTexture("img/enemy/flying/right/fly2R.png"));
	game.FlyingEnR.push_back(wind.loadTexture("img/enemy/flying/right/fly2R.png"));
	game.FlyingEnR.push_back(wind.loadTexture("img/enemy/flying/right/fly2R.png"));		
	game.FlyingEnR.push_back(wind.loadTexture("img/enemy/flying/right/fly2R.png"));
	game.FlyingEnR.push_back(wind.loadTexture("img/enemy/flying/right/fly3R.png"));
	game.FlyingEnR.push_back(wind.loadTexture("img/enemy/flying/right/fly3R.png"));
	game.FlyingEnR.push_back(wind.loadTexture("img/enemy/flying/right/fly3R.png"));
	game.FlyingEnR.push_back(wind.loadTexture("img/enemy/flying/right/fly3R.png"));
	game.FlyingEnR.push_back(wind.loadTexture("img/enemy/flying/right/fly3R.png"));
// LOAD flying enemy LEFT
	game.FlyingEnL.push_back(wind.loadTexture("img/enemy/flying/left/fly0L.png"));
	game.FlyingEnL.push_back(wind.loadTexture("img/enemy/flying/left/fly0L.png"));
	game.FlyingEnL.push_back(wind.loadTexture("img/enemy/flying/left/fly0L.png"));
	game.FlyingEnL.push_back(wind.loadTexture("img/enemy/flying/left/fly0L.png"));
	game.FlyingEnL.push_back(wind.loadTexture("img/enemy/flying/left/fly0L.png"));
	game.FlyingEnL.push_back(wind.loadTexture("img/enemy/flying/left/fly1L.png"));
	game.FlyingEnL.push_back(wind.loadTexture("img/enemy/flying/left/fly1L.png"));
	game.FlyingEnL.push_back(wind.loadTexture("img/enemy/flying/left/fly1L.png"));
	game.FlyingEnL.push_back(wind.loadTexture("img/enemy/flying/left/fly1L.png"));
	game.FlyingEnL.push_back(wind.loadTexture("img/enemy/flying/left/fly1L.png"));
	game.FlyingEnL.push_back(wind.loadTexture("img/enemy/flying/left/fly2L.png"));
	game.FlyingEnL.push_back(wind.loadTexture("img/enemy/flying/left/fly2L.png"));
	game.FlyingEnL.push_back(wind.loadTexture("img/enemy/flying/left/fly2L.png"));	
	game.FlyingEnL.push_back(wind.loadTexture("img/enemy/flying/left/fly2L.png"));	
	game.FlyingEnL.push_back(wind.loadTexture("img/enemy/flying/left/fly2L.png"));
	game.FlyingEnL.push_back(wind.loadTexture("img/enemy/flying/left/fly3L.png"));
	game.FlyingEnL.push_back(wind.loadTexture("img/enemy/flying/left/fly3L.png"));
	game.FlyingEnL.push_back(wind.loadTexture("img/enemy/flying/left/fly3L.png"));
	game.FlyingEnL.push_back(wind.loadTexture("img/enemy/flying/left/fly3L.png"));
	game.FlyingEnL.push_back(wind.loadTexture("img/enemy/flying/left/fly3L.png"));
	// Enemy dying text
	game.FlyDeath.push_back(wind.loadTexture("img/enemy/dying0.png"));
	game.FlyDeath.push_back(wind.loadTexture("img/enemy/dying0.png"));
	game.FlyDeath.push_back(wind.loadTexture("img/enemy/dying0.png"));
	game.FlyDeath.push_back(wind.loadTexture("img/enemy/dying1.png"));
	game.FlyDeath.push_back(wind.loadTexture("img/enemy/dying1.png"));
	game.FlyDeath.push_back(wind.loadTexture("img/enemy/dying1.png"));		
	game.FlyDeath.push_back(wind.loadTexture("img/enemy/dying2.png"));
	game.FlyDeath.push_back(wind.loadTexture("img/enemy/dying2.png"));
	game.FlyDeath.push_back(wind.loadTexture("img/enemy/dying2.png"));	
	//Locked Door
	door1.DoorLocked.push_back(wind.loadTexture("img/bg/door/door0.png"));
	door1.DoorLocked.push_back(wind.loadTexture("img/bg/door/door1.png"));
	door1.DoorLocked.push_back(wind.loadTexture("img/bg/door/door2.png"));
	door1.DoorLocked.push_back(wind.loadTexture("img/bg/door/door3.png"));
	door1.DoorLocked.push_back(wind.loadTexture("img/bg/door/door4.png"));
	door1.DoorLocked.push_back(wind.loadTexture("img/bg/door/door5.png"));
	door1.DoorLocked.push_back(wind.loadTexture("img/bg/door/door6.png"));		

	// STARTING WEAPON TO CHOOSE FROM
	game.base_sword = wind.loadTexture("img/weapons/sword_base.png");	
	game.base_wand = wind.loadTexture("img/weapons/wand_base.png");

	//LOAD CHEST text
	chest1.ChestClosed.push_back(wind.loadTexture("img/props/chest/chest_closed0.png"));									
	chest1.ChestClosed.push_back(wind.loadTexture("img/props/chest/chest_closed1.png"));
	chest1.ChestClosed.push_back(wind.loadTexture("img/props/chest/chest_closed2.png"));
	chest1.ChestClosed.push_back(wind.loadTexture("img/props/chest/chest_closed3.png"));
	chest1.ChestClosed.push_back(wind.loadTexture("img/props/chest/chest_closed4.png"));
	chest1.ChestClosed.push_back(wind.loadTexture("img/props/chest/chest_closed5.png"));
	chest1.ChestClosed.push_back(wind.loadTexture("img/props/chest/chest_closed6.png"));
	chest1.ChestClosed.push_back(wind.loadTexture("img/props/chest/chest_closed7.png"));
	chest1.ChestClosed.push_back(wind.loadTexture("img/props/chest/chest_open.png"));

	// UI TEXT
	game.firstSwordMSG = wind.loadTexture("img/UI/SwordChoice.png");
	game.firstWandMSG = wind.loadTexture("img/UI/WandChoice.png");
	game.AcceptW = wind.loadTexture("img/UI/AcceptWeapon.png");
	game.RefuseW = wind.loadTexture("img/UI/RefuseWeapon.png");


	//SMOKE EFFECT
	game.Smoke.push_back(wind.loadTexture("img/effects/smoke/Smoke1.png"));
	game.Smoke.push_back(wind.loadTexture("img/effects/smoke/Smoke2.png"));
	game.Smoke.push_back(wind.loadTexture("img/effects/smoke/Smoke3.png"));
	game.Smoke.push_back(wind.loadTexture("img/effects/smoke/Smoke4.png"));
	game.Smoke.push_back(wind.loadTexture("img/effects/smoke/Smoke5.png"));
	game.Smoke.push_back(wind.loadTexture("img/effects/smoke/Smoke6.png"));
	game.Smoke.push_back(wind.loadTexture("img/effects/smoke/Smoke7.png"));
	game.Smoke.push_back(wind.loadTexture("img/effects/smoke/Smoke8.png"));
	game.Smoke.push_back(wind.loadTexture("img/effects/smoke/Smoke9.png"));
	game.Smoke.push_back(wind.loadTexture("img/effects/smoke/Smoke10.png"));
	game.Smoke.push_back(wind.loadTexture("img/effects/smoke/Smoke11.png"));
	game.Smoke.push_back(wind.loadTexture("img/effects/smoke/Smoke12.png"));
	game.Smoke.push_back(wind.loadTexture("img/effects/smoke/Smoke13.png"));
	game.Smoke.push_back(wind.loadTexture("img/effects/smoke/Smoke14.png"));
	game.Smoke.push_back(wind.loadTexture("img/effects/smoke/Smoke15.png"));
	game.Smoke.push_back(wind.loadTexture("img/effects/smoke/Smoke16.png"));
	game.Smoke.push_back(wind.loadTexture("img/effects/smoke/Smoke17.png"));
	game.Smoke.push_back(wind.loadTexture("img/effects/smoke/Smoke18.png"));
	game.Smoke.push_back(wind.loadTexture("img/effects/smoke/Smoke19.png"));

	// Green SMoke
	game.SmokeG.push_back(wind.loadTexture("img/effects/smokegreen/Poison Cloud1.png"));
	game.SmokeG.push_back(wind.loadTexture("img/effects/smokegreen/Poison Cloud2.png"));
	game.SmokeG.push_back(wind.loadTexture("img/effects/smokegreen/Poison Cloud3.png"));
	game.SmokeG.push_back(wind.loadTexture("img/effects/smokegreen/Poison Cloud4.png"));
	game.SmokeG.push_back(wind.loadTexture("img/effects/smokegreen/Poison Cloud5.png"));
	game.SmokeG.push_back(wind.loadTexture("img/effects/smokegreen/Poison Cloud6.png"));
	game.SmokeG.push_back(wind.loadTexture("img/effects/smokegreen/Poison Cloud7.png"));
	game.SmokeG.push_back(wind.loadTexture("img/effects/smokegreen/Poison Cloud8.png"));
	game.SmokeG.push_back(wind.loadTexture("img/effects/smokegreen/Poison Cloud9.png"));
	game.SmokeG.push_back(wind.loadTexture("img/effects/smokegreen/Poison Cloud10.png"));
	game.SmokeG.push_back(wind.loadTexture("img/effects/smokegreen/Poison Cloud11.png"));
	game.SmokeG.push_back(wind.loadTexture("img/effects/smokegreen/Poison Cloud12.png"));
	game.SmokeG.push_back(wind.loadTexture("img/effects/smokegreen/Poison Cloud13.png"));
	game.SmokeG.push_back(wind.loadTexture("img/effects/smokegreen/Poison Cloud14.png"));
	game.SmokeG.push_back(wind.loadTexture("img/effects/smokegreen/Poison Cloud15.png"));
	game.SmokeG.push_back(wind.loadTexture("img/effects/smokegreen/Poison Cloud16.png"));
	game.SmokeG.push_back(wind.loadTexture("img/effects/smokegreen/Poison Cloud17.png"));
	game.SmokeG.push_back(wind.loadTexture("img/effects/smokegreen/Poison Cloud18.png"));
	game.SmokeG.push_back(wind.loadTexture("img/effects/smokegreen/Poison Cloud19.png"));

	//SPrinkles
	game.Sprinkle.push_back(wind.loadTexture("img/effects/sprinkle/sprinkle1.png"));
	game.Sprinkle.push_back(wind.loadTexture("img/effects/sprinkle/sprinkle2.png"));
	game.Sprinkle.push_back(wind.loadTexture("img/effects/sprinkle/sprinkle3.png"));
	game.Sprinkle.push_back(wind.loadTexture("img/effects/sprinkle/sprinkle4.png"));

	//Weapons sword notice
	game.RecRustySword = wind.loadTexture("img/UI/RustySwordNotice.png");
	game.RecOldRod = wind.loadTexture("img/UI/OldRodNotice.png");	

	//Rusty sword overhead
	game.RoverR = wind.loadTexture("img/weapons/Rusty_Sword/rusty1.png");
	game.RoverL = wind.loadTexture("img/weapons/Rusty_Sword/rusty2.png");

	// MAgick 1 anim right
	game.MagickAnimR.push_back(wind.loadTexture("img/pl/right/AttackWand0R.png"));
	game.MagickAnimR.push_back(wind.loadTexture("img/pl/right/AttackWand1R.png"));
	game.MagickAnimR.push_back(wind.loadTexture("img/pl/right/AttackWand2R.png"));		
	game.MagickAnimR.push_back(wind.loadTexture("img/pl/right/magick_base/spark-preview1.png"));
	game.MagickAnimR.push_back(wind.loadTexture("img/pl/right/magick_base/spark-preview2.png"));
	game.MagickAnimR.push_back(wind.loadTexture("img/pl/right/magick_base/spark-preview3.png"));
	game.MagickAnimR.push_back(wind.loadTexture("img/pl/right/magick_base/spark-preview4.png"));
	game.MagickAnimR.push_back(wind.loadTexture("img/pl/right/magick_base/hits-3-1.png"));
	game.MagickAnimR.push_back(wind.loadTexture("img/pl/right/magick_base/hits-3-2.png"));
	game.MagickAnimR.push_back(wind.loadTexture("img/pl/right/magick_base/hits-3-3.png"));
	game.MagickAnimR.push_back(wind.loadTexture("img/pl/right/magick_base/hits-3-4.png"));
	game.MagickAnimR.push_back(wind.loadTexture("img/pl/right/magick_base/hits-3-5.png"));
	
	


	int totx = (640 * 2) / 64;
	int toty = (480 * 2) / 64;
	
	Entity p; 
	int z = 0;
	int j = 0;
	int i = 64;
	
	srand((unsigned) time(NULL));
	int random = rand() % 1000;
	int flrnd = rand () % 4;
	bool running = true;
	int x = 64;
	
	//FLOOR TEXT
	for (int x = 64; j < toty - 2 ; j++, x += 64)
	{	
		i = 64;
		if (i == 64)
		p.floor.push_back(Entity(i, x, game.Floor[0]));
		z = 0;
		i = 128;
		for (; z < totx - 3 ; z++, i += 64)
		{	
			if (x == 64 || x == 832 || i == 1152)
			p.floor.push_back(Entity(i, x, game.Floor[0]));
			else
			p.floor.push_back(Entity(i, x, game.Floor[2]));
		}
	}
	int y = 0;
	j = 0;
	//WALL TEXT
	for (int x = 0; x < toty; x++, y += 64)
	{	
		random = rand() % 1000; 
		if (random % 2 == 0)
		p.walls.push_back(Entity(0, y, wallTex));
		else if (random % 5 == 0 && random % 3 == 0)
		p.walls.push_back(Entity(0, y, wall3Tex));
		else
		p.walls.push_back(Entity(0, y, wall2Tex));
	}
	y = 0;
	for (int x = 0; x < totx; x++, y += 64)
	{
		random = rand() % 1000; 
		if (random % 2 == 0)
		p.walls.push_back(Entity(y, 0, wallTex));
		else if (random % 5 == 0 && random % 3 == 0)
		p.walls.push_back(Entity(y, 0, wall3Tex));
		else
		p.walls.push_back(Entity(y, 0, wall2Tex));
	}
	y = 0;
	for (int x = 0; x < toty; x++, y += 64)
	{
		random = rand() % 1000; 
		if (random % 2 == 0)
		p.walls.push_back(Entity(64 * (totx - 1), y, wallTex));
		else if (random % 5 == 0 && random % 3 == 0)
		p.walls.push_back(Entity(64 * (totx - 1), y, wall3Tex));
		else
		p.walls.push_back(Entity(64 * (totx - 1), y, wall2Tex));
	}

	y = 0;
	for (int x = 0; x < totx; x++, y += 64)
	{
		random = rand() % 1000; 
		if (random % 2 == 0)
		p.walls.push_back(Entity(y, 64 * (toty - 1), wallTex));
		else if (random % 5 == 0)
		p.walls.push_back(Entity(y, 64 * (toty - 1), wall3Tex));
		else
		p.walls.push_back(Entity(y, 64 * (toty - 1), wall2Tex));
	}
int currentFrame = 0; // Track the current frame of the animation
int animationSpeed = 80; // Speed of animation (milliseconds per frame)
int animationDelay = animationSpeed; // Initial delay for animation update
int AtFr = 4;
int AtFrl = 2;
int AtFrLL = 5;
int AtFrR = 5;
SDL_Texture * currentTexture;
// STARTING THE GAME
SDL_Event event;
const float timeStep = 0.01f;
float accumulator = 0.0f;
float currentTime = utils::hireTimeInSeconds();
static int TorchFrame;

while (running)
{
int startTicks = SDL_GetTicks();
float newTime = utils::hireTimeInSeconds();
float frameTime = newTime - currentTime;
currentTime = newTime;
accumulator += frameTime;
if (frameTime > 0.25f)
	frameTime = 0.25f;

while (accumulator >= timeStep)
{
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
			running = false;

		if (event.type == SDL_KEYDOWN)
		{
			if (!game.chest2.attack)
				manageKEYBOARD(&event.key, &wind, &game, animationSpeed, chest1, currentTexture);
		}
		if (event.type == SDL_KEYUP)
		{
			manageKEYBOARD(&event.key, &wind, &game, animationSpeed, chest1, currentTexture);
		}
	}
	accumulator -= timeStep;
}

const float alpha = accumulator / timeStep;
wind.clear();
currentTexture = Animations(currentTexture, &game, currentFrame, animationSpeed);
wind.drawMap(p, wind);
renderThings(&game, TorchFrame, currentTexture, &wind, currentFrame, door1, chest1, &event.key);
checkForEvents(&game, &wind, animationSpeed, currentTexture);
	
	// QUICKATTACK & PRESSEDATTACK LOGIC

if (game.Sattack && !game.running && !game.moveDown && !game.moveLeft && !game.moveRight && !game.moveUp && game.W_equipped)
{	
	if (!game.left)
	{
		currentTexture = game.AttackRAnim[AtFr];
		wind.renderTexture(currentTexture, game.pposx + (currentFrame * 2 + 80),  game.pposy + (currentFrame + 3), false);

		vector<int>::iterator i = game.FlyPosx.begin();
		vector<int>::iterator it = game.FlyPosy.begin();
		for (; i != game.FlyPosx.end(); i++, it++)
		{
			game.currFlyingX = *i;
			game.currFlyingY = *it;

		
			if ((game.pposx + (currentFrame * 2 + 80) - game.currFlyingX  >= 0 ) && (game.pposx + (currentFrame * 2 + 80) - game.currFlyingX  <= 180) && (game.currFlyingY - game.pposy + (currentFrame + 3)) >= 0 && ( game.currFlyingY - game.pposy + (currentFrame + 3)) <= 64 )
				{
					game.FlyLightHit = true;
					game.BatD = true;
				}
			else
			game.BatD = false;
		}
		AtFr++;
		if (AtFr == 7)
			AtFr = 4;
	}
	else
	{	
		vector<int>::iterator i = game.FlyPosx.begin();
		vector<int>::iterator it = game.FlyPosy.begin();
		
		currentTexture = game.AttackLAnim[AtFrl];
		for (; i != game.FlyPosx.end(); i++, it++)
		{
			game.currFlyingX = *i;
			game.currFlyingY = *it;
			if ((game.currFlyingX - game.pposx + (currentFrame * 2 + 80) >= 0) && ( game.currFlyingX - game.pposx + (currentFrame * 2 + 80)  <= 180) && (game.pposy + (currentFrame + 3) - game.currFlyingY) >= 0 && (game.pposy + (currentFrame + 3) - game.currFlyingY) <= 64 )
				{
					game.FlyLightHit = true;
					game.BatD =true;
				}
			else
			game.BatD = false;
		}
		wind.renderTexture(currentTexture, game.pposx - (currentFrame * 2 + 66),  game.pposy - (currentFrame - 9), false);
		AtFrl++;
		if (AtFrl == 5)
			AtFrl = 2;
	}
}

//HEAVY ATTACK
if (game.Release && game.idle && game.W_equipped)
{
	static int k;
	k++;
	if (!game.left)
	{
		game.CurrentAttack = true;
		currentTexture = game.Heavy[AtFrR];
		wind.renderTexture(currentTexture, game.pposx + (AtFrR  * 2 + 128), game.pposy + (AtFrR  - 23), false);
		AtFrR++;
		if (AtFrR == 11)
		{
			AtFrR  = 5;
			game.Release = false;
			game.CurrentAttack = false;
		}

	}
	else
	{	
		game.CurrentAttack = true;
		currentTexture = game.HeavyL[AtFrLL];
		wind.renderTexture(currentTexture, game.pposx -(AtFrLL * 2  + 128), game.pposy + (AtFrLL * 2 - 23), false);
		AtFrLL++;
		if (AtFrLL == 11)
		{
			AtFrLL = 5;
			game.Release = false;
			game.CurrentAttack = false;
		}
	}

}


	wind.display();
	currentFrame++;
	TorchFrame++;
	if (currentFrame >= game.playeridleAnimation.size())
		currentFrame = 0;
	if (TorchFrame >= game.TorchAnim.size())
		TorchFrame = 0;

	int frameTicks = SDL_GetTicks() - startTicks;
	int delayTime = animationDelay - frameTicks;
	if (delayTime > 0)
		SDL_Delay(delayTime);

	animationDelay = animationSpeed - frameTicks;
	if (animationDelay < 0)
		animationDelay = 0;
}

wind.cleanUp(&game); 
SDL_Quit();
SDL_QuitSubSystem(SDL_INIT_VIDEO);
return (0);
}