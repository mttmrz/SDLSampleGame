#include "Entity.hpp"
#include "utils.hpp"
#include <random>
#include "SDL_ttf.h"
#include <string>
#include "game.hpp"
using namespace std;



void init_and_load(game *game, Render *wind, Doors *door1, Chest *chest1, int rand2);
struct Projectile {
    int x, y;
    int cast = 0;
    bool hit = false;
    int frame = 0;
    int frameDelay = 0;
    bool directionRight;  
};


struct SlowText {
    std::string fullText;
    int currentIndex = 0;
    Uint32 lastUpdate = 0;
    Uint32 delay = 100;
    SDL_Texture* texture = nullptr;
    int width = 0;
    int height = 0;
    
    // Nuovi campi per il fade out
    bool isComplete = false;        // true quando tutto il testo è apparso
    Uint32 completeTime = 0;        // quando il testo è diventato completo
    Uint32 fadeDelay = 2000;        // quanto aspettare prima del fade (2 secondi)
    Uint32 fadeDuration = 1000;     // durata del fade out (1 secondo)
    Uint8 alpha = 255;              // alpha corrente (255 = opaco, 0 = trasparente)
    bool shouldRender = true;       // se false, non renderizzare più
};

void updateSlowText(SDL_Renderer* renderer, TTF_Font* font, SlowText &slowText) {
    Uint32 now = SDL_GetTicks();
    
    // Fase 1: Apparizione del testo lettera per lettera
    if (!slowText.isComplete && now - slowText.lastUpdate > slowText.delay && 
        slowText.currentIndex < (int)slowText.fullText.size()) {
        
        slowText.currentIndex++;
        slowText.lastUpdate = now;
        
        // Controlla se il testo è completo
        if (slowText.currentIndex >= (int)slowText.fullText.size()) {
            slowText.isComplete = true;
            slowText.completeTime = now;
        }
        
        // Ricrea la texture
        if (slowText.texture) {
            SDL_DestroyTexture(slowText.texture);
            slowText.texture = nullptr;
        }
        
        SDL_Color color = {255, 255, 255, 255};
        std::string sub = slowText.fullText.substr(0, slowText.currentIndex);
        SDL_Surface* surface = TTF_RenderText_Blended(font, sub.c_str(), color);
        slowText.texture = SDL_CreateTextureFromSurface(renderer, surface);
        slowText.width = surface->w;
        slowText.height = surface->h;
        SDL_FreeSurface(surface);
    }
    
    // Fase 2: Fade out dopo il delay
    if (slowText.isComplete && now - slowText.completeTime > slowText.fadeDelay) {
        Uint32 fadeElapsed = now - slowText.completeTime - slowText.fadeDelay;
        
        if (fadeElapsed < slowText.fadeDuration) {
            // Calcola alpha: da 255 a 0
            float fadeProgress = (float)fadeElapsed / (float)slowText.fadeDuration;
            slowText.alpha = (Uint8)(255 * (1.0f - fadeProgress));
        } else {
            // Fade completato
            slowText.alpha = 0;
            slowText.shouldRender = false;
        }
    }
}

void renderSlowText(SDL_Renderer* renderer, SlowText &slowText, int x, int y) {
    if (slowText.texture && slowText.shouldRender && slowText.alpha > 0) {
        // Imposta l'alpha della texture
        SDL_SetTextureAlphaMod(slowText.texture, slowText.alpha);
        
        SDL_Rect dst = {x, y, slowText.width, slowText.height};
        SDL_RenderCopy(renderer, slowText.texture, NULL, &dst);
    }
}

void message_to_the_player(SlowText &slowText, game *info)
{	
	 if (slowText.currentIndex == 0 || !slowText.shouldRender) // Reset anche se il precedente è finito
        {
            // Reset completo
            if (slowText.texture) {
                SDL_DestroyTexture(slowText.texture);
                slowText.texture = nullptr;
            }
            if (!info->demo_end)
			{
				slowText.fullText = "Your faith is sealed...";
			}
			else
				slowText.fullText = info->chest2.W_equipped ? "I can see you... MAGE." : "I can see you... WARRIOR.";
            slowText.currentIndex = 0;
            slowText.lastUpdate = SDL_GetTicks();
            slowText.isComplete = false;
            slowText.completeTime = 0;
            slowText.alpha = 255;
            slowText.shouldRender = true;
			
        }
}
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
    auto it = game->PropPos.begin();
    auto k = game->Props.begin();
    k++;

    while (it != game->PropPos.end() && k != game->Props.end())
    {
        if (!it->second)
        {
            wind->renderTexture(*k, it->first, it->first, false, game);
            ++it;
            ++k;
        }
        else
        {
			if (game->Props.size() == 2)
			{
				game->show_key = true;
			}
			it = game->PropPos.erase(it);
			k = game->Props.erase(k);
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
		wind->renderTexture(game->FlyingEnR[i], *it, *ite, false, game);
		else
		wind->renderTexture(game->FlyingEnL[i], *it, *ite, false, game);
		
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
			wind->renderTexture(chest1.ChestClosed[0], 348, 380, false, game);
			else
			wind->renderTexture(chest1.ChestClosed[0], 352, 380, false, game);

		}
		i++;
		if (i >= 30 && i <= 40)
		wind->renderTexture(chest1.ChestClosed[8], 352, 380, false, game);
		if (i >= 41 && i <= 44)
		{
			static int lol;
			wind->renderTexture(game->Sprinkle[lol], 352, 380, false, game);
			wind->renderTexture(game->Sprinkle[lol], 352, 380, false, game);	
			wind->renderTexture(game->Sprinkle[lol], 352, 380, false, game);
			lol++;
		}
		if (i >= 45)
		{
			wind->renderTexture(game->Smoke[k], 342, 360, false, game);
			k++;
			if (k == 19)
				k = 0;

			wind->renderTexture(game->base_sword, 350, 400, false, game);
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
			wind->renderTexture(chest1.ChestClosed[0], 548, 380, false, game);
			else
			wind->renderTexture(chest1.ChestClosed[0], 552, 380, false, game);

		}
		i++;
		if (i >= 30 && i <= 40)
		wind->renderTexture(chest1.ChestClosed[8], 552, 380, false, game);
		if (i >= 41 && i <= 44)
		{
			static int lol;
			wind->renderTexture(game->Sprinkle[lol], 552, 380, false, game);
			wind->renderTexture(game->Sprinkle[lol], 552, 380, false, game);	
			wind->renderTexture(game->Sprinkle[lol], 552, 380, false, game);
			lol++;
		}
		if (i >= 45)
		{
			wind->renderTexture(game->SmokeG[k], 510, 290, false, game);
			k++;
			if (k == 19)
				k = 0;

			wind->renderTexture(game->base_wand, 550, 400, false, game);
			game->chest2.picked = false;
		}
	}

}

void renderThings(game *game, int Tf, SDL_Texture *currentTexture, Render *wind, int fps, Doors &door1, Chest &chest1, SDL_KeyboardEvent *event, SlowText &text)
{
	bool magickAnim = (currentTexture == game->MagickAnimR[0] ||
                   currentTexture == game->MagickAnimR[1] ||
                   currentTexture == game->MagickAnimR[2]);

	if (game->left && magickAnim && !game->Release_Wand)  
	{
		wind->renderTexture(currentTexture, game->pposx, game->pposy, true, game);
	}
	else
	{
		if (!game->Release_Wand)
			wind->renderTexture(currentTexture, game->pposx, game->pposy, false, game);
	}
	wind->renderTexture(game->HealthBar[game->currLife], 0, 0, false, game);
	wind->renderTexture(game->TorchAnim[Tf], 430, 0, false, game);
	wind->renderTexture(game->TorchAnim[Tf], 530, 0, false, game);
	wind->renderTexture(game->TorchAnim[Tf], 530, 898, false, game);
	wind->renderTexture(game->TorchAnim[Tf], 130, 898, false, game);
	wind->renderTexture(game->Props[0], 480, 8, false, game);
	static int index;
	if (game->show_key)
	{
		wind->renderTexture(game->key[index / 3], game->PropToDes, game->PropToDes, false, game);
		index = index == 30 ? 0 : index + 1;
	}	if (!chest1.isOpen())
	{
		static int l;
		static int ll;
		if (ll < 100)
		{
			wind->renderTexture(chest1.ChestClosed[0], 350, 380, false, game);
			ll++;
		}
		else
		{
			wind->renderTexture(chest1.ChestClosed[l], 350, 380, false, game);
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
			wind->renderTexture(chest1.ChestClosed[0], 550, 380, false, game);
			ll++;
		}
		else
		{
			wind->renderTexture(chest1.ChestClosed[l], 550, 380, false, game);
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
		wind->renderTexture(game->Explosion[k], game->PropToDes, game->PropToDes, false, game);
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
		wind->renderTexture(game->firstSwordMSG, 350 , 450, false, game); 
	//CHEST 2
	if (game->chest2.WeaponMSG)
		wind->renderTexture(game->firstWandMSG, 350 , 450, false, game);
	if (game->choice)
	{
		if (game->currchoice == 0)
		wind->renderTexture(game->AcceptW, 350 , 450, false, game);
		else
		wind->renderTexture(game->RefuseW, 350, 450, false, game);
	}
	//CHEST 2
	if (game->chest2.choice)
	{
		if (game->chest2.currchoice == 0)
		wind->renderTexture(game->AcceptW, 350 , 450, false, game);
		else
		wind->renderTexture(game->RefuseW, 350, 450, false, game);
	}
	if (game->YouRecived)
	{
		game->overhead = true;
		wind->renderTexture(game->RecRustySword, 350, 450, false, game);
	}
	//CHEST 2
	if (game->chest2.YouRecived)
	{
		game->chest2.overhead = true;
		wind->renderTexture(game->RecOldRod, 350, 450, false, game);
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
				wind->renderTexture(game->RoverR, game->pposx, game->pposy - 60, false, game);	
		}
		else
		{
			SDL_SetTextureAlphaMod(game->RoverR, o);
			wind->renderTexture(game->RoverL, game->pposx, game->pposy - 60, false, game);
		}

		if (o <= 0)
			game->overhead = false;
	}
	// porta e tc.
	if (door1.getLock())
	{
		wind->renderTexture(door1.DoorLocked[0], 580, 0, false, game);

	}
	else
	{
		static int index;
		if (!game->door_opened)
		{
			game->idle = false;
			wind->renderTexture(door1.DoorLocked[index / 3], 580, 0, false, game);
			index++;
			if (index == 42)
				game->door_opened = true;
		}
		static int cu;
	if (game->door_opened)
	{
		static Uint8 alpha = 0;
		wind->renderTexture(door1.DoorLocked[15], 580, 0, false, game);

		if (!game->demo_end)
		{
			game->demo_end = true;
		}
		else
		{
			game->idle = false;
			game->moveUp = true;
			if (game->pposx < 582)
				game->pposx += 4;
			if (game->pposx > 614)
				game->pposx -= 4;
			if (cu % 5 == 0)
			{
				if (game->pposy > 0)
					game->pposy -= 4;
			}
		}

		if (alpha < 255)
			alpha += 5;

		// Disegna direttamente sul renderer principale (NESSUN render target alternativo)
		SDL_SetTextureBlendMode(game->full_black, SDL_BLENDMODE_BLEND);
		SDL_SetTextureAlphaMod(game->full_black, alpha);
		SDL_RenderCopy(wind->getRenderer(), game->full_black, nullptr, nullptr);

		cu++;
	}
	if (cu == 120)
	{
		message_to_the_player(text, game);
	}
	if (cu == 300)
	{
		SDL_Quit();
		SDL_QuitSubSystem(SDL_INIT_VIDEO);
		wind->cleanUp(game);
		exit(0);

	}
}



}

void Collisions(game *info, std::vector<Projectile>& projectiles)
{

	if (info->demo_end)
	{
		return ;
	}
	if (info->show_key) {
		int key_x = info->PropToDes;
		int key_y = info->PropToDes - 60;
		int radius = 50;
		if (info->pposx >= key_x - radius && info->pposx <= key_x + radius &&
			info->pposy >= key_y - radius && info->pposy <= key_y + radius) 
		{
			info->show_key = false;
			info->key_picked = true;
		}
	}
    // Teletrasporto


	// porta
	if (info->pposx >= 500 && info->pposx <= 630 && info->pposy >= -60 && info->pposy <= 100) {
		info->on_the_door = true;
	} else {
		info->on_the_door = false;
	}
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
		if (!info->Release_Wand)
		{
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
	
	if (!info->demo_end)
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


	if (info->idle && info->Hattk_Wand && info->chest2.W_equipped && !info->Release_Wand)
	{	
		if (!info->left)
		{
			static int j;
			if (!info->Release_Wand)
			{
				currentTexture = info->Hurt[j];
				j++;
				if (j == 3)
					j = 0;
			}
		}
		else
		{
			static int j;
			if (!info->Release_Wand)
			{
				currentTexture = info->HurtL[j];
				j++;
				if (j == 3)
					j = 0;
			}
		}

	}
	if (info->idle && !info->Hattk_Wand && info->Hattak2_Wand && info->chest2.W_equipped && !info->Release_Wand)
	{	
		if (!info->left && (!info->Release_Wand && currentTexture != info->MagickAnimR[2]))
		{
			static int j = 0;
			currentTexture = info->Hurt[j];
			j++;
			if (j == 3)
			{
				j = 0;
				info->Hattak2_Wand = false;
			}
		}
		else
		{
			if (!info->Release_Wand)
			{
				static int j = 0;
				currentTexture = info->HurtL[j];
				j++;
				if (j == 3)
				{
					j = 0;
					info->Hattak2_Wand = false;
				}
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
	if (!info->Release_Wand && info->idle == true && info->left == false && info->attack == false && !info->Hattk && !info->Hattak2 && !info->chest2.attack && !info->Hattak2_Wand &&  !info->Hattk_Wand)
		currentTexture = info->playeridleAnimation[currentFrame];
	if (!info->Release_Wand && info->idle == true && info->left == true && info->attack == false && !info->Hattk && !info->Hattak2 && !info->chest2.attack && !info->Hattak2_Wand &&  !info->Hattk_Wand)
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
	if (info->chest2.W_equipped && !info->Release_Wand)
	{
		wand_anim(info, &currentTexture);
	}

	return (currentTexture);

}


void PopUpMSG(game *info, Render *wind, SDL_KeyboardEvent* event)
{

	info->fSwordMSG = true;

}


void pickWeaponLogickp2(game *info, SDL_KeyboardEvent *event, Render *wind, SlowText &slowText, Chest &chest1)
{
	if (info->chest2.choice)
	{
		if (event->keysym.sym == SDLK_RIGHT || event->keysym.sym == SDLK_d)
		{
			info->chest2.choice = true;
			info->chest2.currchoice = 1;
		}
		if (event->keysym.sym == SDLK_LEFT || event->keysym.sym == SDLK_a)
		{
			info->chest2.choice = true;
			info->chest2.currchoice = 0;
		}
		if (event->keysym.sym == SDLK_RETURN || event->keysym.sym == SDLK_e)
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
		{
			if (!chest1.isOpen())
				info->chest2.WeaponMSG = true;
			else
				message_to_the_player(slowText, info);

		}
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



void pickWeaponLogick(game *info, SDL_KeyboardEvent *event, Chest &chest1, Render *wind, SlowText &slowText)
{

	if (info->choice)
	{
		if (event->keysym.sym == SDLK_RIGHT || event->keysym.sym == SDLK_d)
		{
			info->choice = true;
			info->currchoice = 1;
		}
		if (event->keysym.sym == SDLK_LEFT || event->keysym.sym == SDLK_a)
		{
			info->choice = true;
			info->currchoice = 0;
		}
		if (event->keysym.sym == SDLK_RETURN || event->keysym.sym == SDLK_e)
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
		{
			if (!info->chest2.isOpen())
				PopUpMSG(info, wind, event);
			else
				message_to_the_player(slowText, info);

		}
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
	pickWeaponLogickp2(info, event, wind, slowText, chest1);
}


void WandAttack(game *info, SDL_KeyboardEvent* event, const Uint8 *currentKeyState, Uint32 &spaceKeyDownTime, float elapsedSeconds, Uint32 currentTime, SDL_Texture * currentTexture)
{
	static Cooldown qCooldown(1000);
	static Uint32 qKeyDt = 0;
	float elapseQ = (currentTime - qKeyDt) / 1000.0f;
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
	if (!info->moveDown && !info->moveLeft && !info->moveRight && !info->moveUp && info->chest2.W_equipped && event->keysym.sym == SDLK_q)
	{
		if (qCooldown.isReady())
		{		
			if (!currentKeyState[SDL_SCANCODE_Q])
			{	
				qCooldown.activate();
				qKeyDt = 0;
				info->Release_Wand = true;
			}
			else
			{
				if (elapseQ <= 2.5 && currentTime / 1000.0f > 1)
				{	
					info->Hattk_Wand = true;
					info->Hattak2_Wand = true;
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
}


void manageKEYBOARD(SDL_KeyboardEvent* event, Render* wind, game* info, int& animationSpeed, Chest &chest1, SDL_Texture * currentTexture, SlowText& slowText, Doors &door) 
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
	
	if (info->on_the_door && event->keysym.sym == SDLK_e)
	{
		if (info->key_picked)
		{
			door.setLock(false);
		}

	}
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
	pickWeaponLogick(info, event, chest1, wind, slowText);
	WandAttack(info, event, currentKeyState, spaceKeyDownTime, elapsedSeconds, currentTime, currentTexture);
	if (currentKeyState[SDL_SCANCODE_LSHIFT]) {
		animationSpeed = 60;
		info->running = true;
	} else {
		info->idle = true;
	}
}
if (!door.getLock())
{
	info->running = false;
	info->attack = false;
	animationSpeed = 70;
	info->Sattack = false;
	info->Hattk = false;
	info->Hattk_Wand = false;
	spaceKeyDownTime = 0; // Reset the space key down time

}

if (event->type == SDL_KEYUP) {
	if ((info->chest2.W_equipped || info->W_equipped) && event->keysym.sym == SDLK_q && IdleCooldown.getTime() > 0  && !info->moveDown && !info->moveLeft && !info->moveRight && !info->moveUp)
	{

 		if (info->idle && qCooldown.getTime() >= 2)
		{
			if (info->W_equipped)
				info->Release = true;
			if (info->chest2.W_equipped)
				info->Release_Wand = true;

		}

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
	info->Hattk_Wand = false;
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

            wind->renderTexture(info->MagickAnimR[animFrame], p.x + p.cast, p.y + 10, !p.directionRight, info);

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
						if (info->show_key)
							return;
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
					if (info->show_key)
							return;
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
		wind->renderTexture(info->FlyDeath[k], info->currFlyingX, info->currFlyingY, false, info);
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
	SDL_Renderer* renderer = wind.renderer;
	TTF_Init();
	TTF_Font* font = TTF_OpenFont("whitefont.ttf", 48);  // 24 = dimensione font


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
	game.Release_Wand = false;
	game.Hattak2_Wand = false;
	game.Hattk_Wand = false;
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
	game.launch_heavy = false;
	game.wand_cd = false;
	SlowText slowText;
	init_and_load(&game, &wind, &door1, &chest1, rand2);



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

            if (event.type == SDL_KEYDOWN && !game.demo_end)
            {
                if (!game.chest2.attack)
                    manageKEYBOARD(&event.key, &wind, &game, animationSpeed, chest1, currentTexture, slowText, door1);
            }
            if (event.type == SDL_KEYUP)
            {
                manageKEYBOARD(&event.key, &wind, &game, animationSpeed, chest1, currentTexture, slowText, door1);
            }
        }
        accumulator -= timeStep;
    }


    const float alpha = accumulator / timeStep;
    wind.clear();

    // Aggiorna e disegna il testo lento se c'è testo da mostrare


    currentTexture = Animations(currentTexture, &game, currentFrame, animationSpeed);
    wind.drawMap(p, wind);
    renderThings(&game, TorchFrame, currentTexture, &wind, currentFrame, door1, chest1, &event.key, slowText);
    
	if (!game.demo_end)
		checkForEvents(&game, &wind, animationSpeed, currentTexture);

	
	// QUICKATTACK & PRESSEDATTACK LOGIC

if (game.Sattack && !game.running && !game.moveDown && !game.moveLeft && !game.moveRight && !game.moveUp && game.W_equipped)
{	
	if (!game.left)
	{
		currentTexture = game.AttackRAnim[AtFr];
		wind.renderTexture(currentTexture, game.pposx + (currentFrame * 2 + 80),  game.pposy + (currentFrame + 3), false, &game);

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
		wind.renderTexture(currentTexture, game.pposx - (currentFrame * 2 + 66),  game.pposy - (currentFrame - 9), false, &game);
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
		wind.renderTexture(currentTexture, game.pposx + (AtFrR  * 2 + 128), game.pposy + (AtFrR  - 23), false, &game);
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
		wind.renderTexture(currentTexture, game.pposx -(AtFrLL * 2  + 128), game.pposy + (AtFrLL * 2 - 23), false, &game);
		AtFrLL++;
		if (AtFrLL == 11)
		{
			AtFrLL = 5;
			game.Release = false;
			game.CurrentAttack = false;
		}
	}

}
if (game.Release_Wand && game.idle && game.chest2.W_equipped)
{
	static Projectile p;
	static bool launched = false;
	static int frames = 0;

	frames++;

	if (!launched)
	{
		p.x = game.pposx;
		p.y = game.pposy;
		p.frame = 0;
		p.hit = false;
		p.frameDelay = 0;
		p.cast = 0;
		p.directionRight = !game.left;

		launched = true;
		game.CurrentAttack = true;
	}

	int px = p.x + p.cast;
	int py = p.y + 10;

	if (!p.hit)
	{
		int totx = 640 * 2;
		int toty = 480 * 2;

		bool hitWall = (px < 0 || px >= totx - 120 || py < 0 || py > toty);

		bool chest1Closed = !game.chest1open && (px >= 300 && px <= 400 && py >= 350 && py <= 435);
		bool chest2Closed = !game.chest2.isOpen() && (px >= 500 && px <= 600 && py >= 350 && py <= 435);

		for (auto it = game.PropPos.begin(); it != game.PropPos.end(); ++it)
		{	
			if (!it->second)
			{
				int posx = it->first;
				int posy = posx; 

				int width = 64;
				int height = 128;


				bool hitProp = (px >= posx - width / 2 && px <= posx + width / 2 &&
								py >= posy - height / 2 && py <= posy + height / 2);

				if (hitProp)
				{
					p.hit = true;
					p.frame = 0;
					p.frameDelay = 0;
					game.destroyed = true;
					game.PropToDes = it->first;
					it->second = true;
					break;
				}
			}
		}




		

		bool hitFly = false;
		for (size_t i = 0; i < game.FlyPosx.size(); i++)
		{
			int flyx = game.FlyPosx[i];
			int flyy = game.FlyPosy[i];
			if (abs(px - flyx) <= 20 && abs(py - flyy) <= 20)
			{
				hitFly = true;
				game.FlyLightHit = true;
				break;
			}
		}

		if (hitWall || chest1Closed || chest2Closed || hitFly)
		{
			p.hit = true;
			p.frame = 0;
			p.frameDelay = 0;
		}
	}

	wind.renderTexture(game.MagickAnimR[2], game.pposx, game.pposy, game.left, &game);

	if (!p.hit)
	{
		int animFrame = std::min(p.frame, 24);
		if (!game.left)
			wind.renderTexture(game.Projectile_Heavy[animFrame], px - 30, py - 100, !p.directionRight, &game, 2, 2);
		else
			wind.renderTexture(game.Projectile_Heavy[animFrame], px - 30, py - 100, !p.directionRight, &game, 2, 2);
	}
	else
	{
		int animFrame = std::min(p.frame, 11);
		if (!game.left)
			wind.renderTexture(game.Heavy_Explosion[animFrame], px - 64, py - 170, false, &game, 2, 2);
		else
			wind.renderTexture(game.Heavy_Explosion[animFrame], px - 64, py - 170, false, &game, 2, 2);
	}

	p.frameDelay++;
	if (p.frameDelay >= 2)
	{
		p.frame++;
		p.frameDelay = 0;
	}

	if (!p.hit)
		p.cast += p.directionRight ? 25 : -25;

	if (p.cast > 1600 || p.cast < -1600 || (p.hit && p.frame > 11))
	{
		launched = false;
		game.Release_Wand = false;
		game.CurrentAttack = false;
	}

	game.Hattak2_Wand = false;
}


	if ((!slowText.fullText.empty() && slowText.shouldRender))
    {
        updateSlowText(renderer, font, slowText);
        renderSlowText(renderer, slowText, 400, 500);
    }
    SDL_RenderPresent(renderer);


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