#include "game.hpp"



void init_and_load(game *game, Render *wind, Doors *door1, Chest *chest1, int rand2)
{
    // PROP POSITION INSERT
        game->PropPos.insert(make_pair(game->randProp, false));
        game->PropPos.insert(make_pair(rand2, false));
        game->PropCoord.insert(make_pair(0, 0));
    
    //LOAD Hurt frame
        game->Hurt.push_back(wind->loadTexture("img/pl/right/hurt1.png"));
        game->Hurt.push_back(wind->loadTexture("img/pl/right/hurt.png"));
        game->Hurt.push_back(wind->loadTexture("img/pl/right/hurt2.png"));
        game->Hurt.push_back(wind->loadTexture("img/pl/right/AttackWand2R.png"));
    
        game->HurtL.push_back(wind->loadTexture("img/pl/left/hurt1L.png"));
        game->HurtL.push_back(wind->loadTexture("img/pl/left/hurtL.png"));
        game->HurtL.push_back(wind->loadTexture("img/pl/left/hurt2L.png"));
        game->HurtL.push_back(wind->loadTexture("img/pl/right/AttackWand2L.png"));
    //load Floor
        game->Floor.push_back(wind->loadTexture("img/bg/floor_1.png"));
        game->Floor.push_back(wind->loadTexture("img/bg/floor_2.png"));
        game->Floor.push_back(wind->loadTexture("img/bg/floor_3.png"));
        game->Floor.push_back(wind->loadTexture("img/bg/floor_4.png"));	
    // Load the player idle animation textures
        game->playeridleAnimation.push_back(wind->loadTexture("img/pl/right/pidle0.png"));
        game->playeridleAnimation.push_back(wind->loadTexture("img/pl/right/pidle0.png"));
        game->playeridleAnimation.push_back(wind->loadTexture("img/pl/right/pidle1.png"));
        game->playeridleAnimation.push_back(wind->loadTexture("img/pl/right/pidle1.png"));
        game->playeridleAnimation.push_back(wind->loadTexture("img/pl/right/pidle2.png"));
        game->playeridleAnimation.push_back(wind->loadTexture("img/pl/right/pidle2.png"));	
        game->playeridleAnimation.push_back(wind->loadTexture("img/pl/right/pidle5.png"));
        game->playeridleAnimation.push_back(wind->loadTexture("img/pl/right/pidle5.png"));
        game->playeridleAnimation.push_back(wind->loadTexture("img/pl/right/pidle4.png"));
        game->playeridleAnimation.push_back(wind->loadTexture("img/pl/right/pidle4.png"));
    //Load the player idle left animation textures
    
        game->playeridleAnimationL.push_back(wind->loadTexture("img/pl/left/pidle0l.png"));
        game->playeridleAnimationL.push_back(wind->loadTexture("img/pl/left/pidle0l.png"));
        game->playeridleAnimationL.push_back(wind->loadTexture("img/pl/left/pidle1l.png"));
        game->playeridleAnimationL.push_back(wind->loadTexture("img/pl/left/pidle1l.png"));
        game->playeridleAnimationL.push_back(wind->loadTexture("img/pl/left/pidle2l.png"));
        game->playeridleAnimationL.push_back(wind->loadTexture("img/pl/left/pidle2l.png"));
        game->playeridleAnimationL.push_back(wind->loadTexture("img/pl/left/pidle5l.png"));
        game->playeridleAnimationL.push_back(wind->loadTexture("img/pl/left/pidle5l.png"));
        game->playeridleAnimationL.push_back(wind->loadTexture("img/pl/left/pidle4l.png"));
        game->playeridleAnimationL.push_back(wind->loadTexture("img/pl/left/pidle4l.png"));
    
    // Load the player running right animation textures
        game->RunRAnim.push_back(wind->loadTexture("img/pl/right/runR0.png"));
        game->RunRAnim.push_back(wind->loadTexture("img/pl/right/runR1.png"));
        game->RunRAnim.push_back(wind->loadTexture("img/pl/right/runR1.png"));
        game->RunRAnim.push_back(wind->loadTexture("img/pl/right/runR2.png"));
        game->RunRAnim.push_back(wind->loadTexture("img/pl/right/runR2.png"));
        game->RunRAnim.push_back(wind->loadTexture("img/pl/right/runR3.png"));
        game->RunRAnim.push_back(wind->loadTexture("img/pl/right/runR3.png"));
        game->RunRAnim.push_back(wind->loadTexture("img/pl/right/runR4.png"));
        game->RunRAnim.push_back(wind->loadTexture("img/pl/right/runR4.png"));
        game->RunRAnim.push_back(wind->loadTexture("img/pl/right/runR0.png"));
    // Load the player running left animation textures
        game->RunLAnim.push_back(wind->loadTexture("img/pl/left/runL0.png"));
        game->RunLAnim.push_back(wind->loadTexture("img/pl/left/runL1.png"));
        game->RunLAnim.push_back(wind->loadTexture("img/pl/left/runL1.png"));
        game->RunLAnim.push_back(wind->loadTexture("img/pl/left/runL2.png"));
        game->RunLAnim.push_back(wind->loadTexture("img/pl/left/runL2.png"));
        game->RunLAnim.push_back(wind->loadTexture("img/pl/left/runL3.png"));
        game->RunLAnim.push_back(wind->loadTexture("img/pl/left/runL3.png"));
        game->RunLAnim.push_back(wind->loadTexture("img/pl/left/runL4.png"));
        game->RunLAnim.push_back(wind->loadTexture("img/pl/left/runL4.png"));
        game->RunLAnim.push_back(wind->loadTexture("img/pl/left/runL0.png"));
    // Load the player attacking to the right animation textures
        game->AttackRAnim.push_back(wind->loadTexture("img/pl/right/attack00R.png"));
        game->AttackRAnim.push_back(wind->loadTexture("img/pl/right/attack02R.png"));	
        game->AttackRAnim.push_back(wind->loadTexture("img/pl/right/attack01R.png"));
        game->AttackRAnim.push_back(wind->loadTexture("img/pl/right/attack0R.png"));
        game->AttackRAnim.push_back(wind->loadTexture("img/pl/right/attack1R.png"));
        game->AttackRAnim.push_back(wind->loadTexture("img/pl/right/a2R.png"));
        game->AttackRAnim.push_back(wind->loadTexture("img/pl/right/attack3R.png"));
        game->AttackRAnim.push_back(wind->loadTexture("img/pl/right/attack1R.png"));	
        game->AttackRAnim.push_back(wind->loadTexture("img/pl/right/attack3R.png"));
    // Load the player attacking to the left animation textures
        game->AttackLAnim.push_back(wind->loadTexture("img/pl/left/attack0L.png"));
        game->AttackLAnim.push_back(wind->loadTexture("img/pl/left/attack01L.png"));
        game->AttackLAnim.push_back(wind->loadTexture("img/pl/left/attack02L.png"));
        game->AttackLAnim.push_back(wind->loadTexture("img/pl/left/attack03L.png"));
        game->AttackLAnim.push_back(wind->loadTexture("img/pl/left/attack04L.png"));
    
    // Load the player healthbar
        game->HealthBar.push_back(wind->loadTexture("img/bg/health_ui0H.png"));	
        game->HealthBar.push_back(wind->loadTexture("img/bg/health_ui1H.png"));
        game->HealthBar.push_back(wind->loadTexture("img/bg/health_ui2H.png"));
        game->HealthBar.push_back(wind->loadTexture("img/bg/health_ui3H.png"));
        game->HealthBar.push_back(wind->loadTexture("img/bg/health_ui4H.png"));
        game->HealthBar.push_back(wind->loadTexture("img/bg/health_ui.png"));
    //Load heavy strike
        game->Heavy.push_back(wind->loadTexture("img/pl/right/attack00R.png"));
        game->Heavy.push_back(wind->loadTexture("img/pl/right/Charge0.png"));
        game->Heavy.push_back(wind->loadTexture("img/pl/right/Charge1.png"));
        game->Heavy.push_back(wind->loadTexture("img/pl/right/attack01R.png"));
        game->Heavy.push_back(wind->loadTexture("img/pl/right/attack01R.png"));
        game->Heavy.push_back(wind->loadTexture("img/pl/right/CHARGE4.png"));
        game->Heavy.push_back(wind->loadTexture("img/pl/right/CHARGE4.png"));
        game->Heavy.push_back(wind->loadTexture("img/pl/right/CHARGE4.png"));
        game->Heavy.push_back(wind->loadTexture("img/pl/right/CHARGE5.png"));
        game->Heavy.push_back(wind->loadTexture("img/pl/right/CHARGE5.png"));
        game->Heavy.push_back(wind->loadTexture("img/pl/right/CHARGE6.png"));
    //LOAD heave LEFT strike
        game->HeavyL.push_back(wind->loadTexture("img/pl/left/attack00L.png"));
        game->HeavyL.push_back(wind->loadTexture("img/pl/left/Charge0L.png"));
        game->HeavyL.push_back(wind->loadTexture("img/pl/left/Charge1L.png"));
        game->HeavyL.push_back(wind->loadTexture("img/pl/left/attack01L.png"));
        game->HeavyL.push_back(wind->loadTexture("img/pl/left/attack01L.png"));
        game->HeavyL.push_back(wind->loadTexture("img/pl/left/CHARGE4L.png"));
        game->HeavyL.push_back(wind->loadTexture("img/pl/left/CHARGE4L.png"));
        game->HeavyL.push_back(wind->loadTexture("img/pl/left/CHARGE4L.png"));
        game->HeavyL.push_back(wind->loadTexture("img/pl/left/CHARGE5L.png"));
        game->HeavyL.push_back(wind->loadTexture("img/pl/left/CHARGE5L.png"));
        game->HeavyL.push_back(wind->loadTexture("img/pl/left/CHARGE6L.png"));
    //LOAD torch prop
        game->TorchAnim.push_back(wind->loadTexture("img/props/torch0.png"));
        game->TorchAnim.push_back(wind->loadTexture("img/props/torch1.png"));
        game->TorchAnim.push_back(wind->loadTexture("img/props/torch2.png"));
        game->TorchAnim.push_back(wind->loadTexture("img/props/torch3.png"));
        game->TorchAnim.push_back(wind->loadTexture("img/props/torch4.png"));
        game->TorchAnim.push_back(wind->loadTexture("img/props/torch5.png"));
    // LOAD varie Props
        game->Props.push_back(wind->loadTexture("img/props/prisoner.png"));
        game->Props.push_back(wind->loadTexture("img/props/table.png"));
        game->Props.push_back(wind->loadTexture("img/props/barrel.png"));
    
    // LOAD HEAVY BOLT
        game->Projectile_Heavy.push_back(wind->loadTexture("img/pl/right/magick_base/1_0.png"));
        game->Projectile_Heavy.push_back(wind->loadTexture("img/pl/right/magick_base/1_1.png"));
        game->Projectile_Heavy.push_back(wind->loadTexture("img/pl/right/magick_base/1_2.png"));
        game->Projectile_Heavy.push_back(wind->loadTexture("img/pl/right/magick_base/1_3.png"));
        game->Projectile_Heavy.push_back(wind->loadTexture("img/pl/right/magick_base/1_4.png"));
        game->Projectile_Heavy.push_back(wind->loadTexture("img/pl/right/magick_base/1_5.png"));
        game->Projectile_Heavy.push_back(wind->loadTexture("img/pl/right/magick_base/1_6.png"));
        game->Projectile_Heavy.push_back(wind->loadTexture("img/pl/right/magick_base/1_7.png"));
        game->Projectile_Heavy.push_back(wind->loadTexture("img/pl/right/magick_base/1_8.png"));
        game->Projectile_Heavy.push_back(wind->loadTexture("img/pl/right/magick_base/1_9.png"));
        game->Projectile_Heavy.push_back(wind->loadTexture("img/pl/right/magick_base/1_10.png"));
        game->Projectile_Heavy.push_back(wind->loadTexture("img/pl/right/magick_base/1_11.png"));
        game->Projectile_Heavy.push_back(wind->loadTexture("img/pl/right/magick_base/1_12.png"));
        game->Projectile_Heavy.push_back(wind->loadTexture("img/pl/right/magick_base/1_13.png"));
        game->Projectile_Heavy.push_back(wind->loadTexture("img/pl/right/magick_base/1_14.png"));
        game->Projectile_Heavy.push_back(wind->loadTexture("img/pl/right/magick_base/1_15.png"));
        game->Projectile_Heavy.push_back(wind->loadTexture("img/pl/right/magick_base/1_16.png"));
        game->Projectile_Heavy.push_back(wind->loadTexture("img/pl/right/magick_base/1_17.png"));
        game->Projectile_Heavy.push_back(wind->loadTexture("img/pl/right/magick_base/1_18.png"));
        game->Projectile_Heavy.push_back(wind->loadTexture("img/pl/right/magick_base/1_19.png"));
        game->Projectile_Heavy.push_back(wind->loadTexture("img/pl/right/magick_base/1_20.png"));
        game->Projectile_Heavy.push_back(wind->loadTexture("img/pl/right/magick_base/1_21.png"));
        game->Projectile_Heavy.push_back(wind->loadTexture("img/pl/right/magick_base/1_22.png"));
        game->Projectile_Heavy.push_back(wind->loadTexture("img/pl/right/magick_base/1_23.png"));
        game->Projectile_Heavy.push_back(wind->loadTexture("img/pl/right/magick_base/1_24.png"));
        game->Projectile_Heavy.push_back(wind->loadTexture("img/pl/right/magick_base/1_25.png"));
        game->Projectile_Heavy.push_back(wind->loadTexture("img/pl/right/magick_base/1_26.png"));
        game->Projectile_Heavy.push_back(wind->loadTexture("img/pl/right/magick_base/1_27.png"));
        game->Projectile_Heavy.push_back(wind->loadTexture("img/pl/right/magick_base/1_28.png"));
        game->Projectile_Heavy.push_back(wind->loadTexture("img/pl/right/magick_base/1_29.png"));
        game->Projectile_Heavy.push_back(wind->loadTexture("img/pl/right/magick_base/1_30.png"));
        game->Projectile_Heavy.push_back(wind->loadTexture("img/pl/right/magick_base/1_31.png"));
        game->Projectile_Heavy.push_back(wind->loadTexture("img/pl/right/magick_base/1_32.png"));
        game->Projectile_Heavy.push_back(wind->loadTexture("img/pl/right/magick_base/1_33.png"));
        game->Projectile_Heavy.push_back(wind->loadTexture("img/pl/right/magick_base/1_34.png"));
        game->Projectile_Heavy.push_back(wind->loadTexture("img/pl/right/magick_base/1_35.png"));
        game->Projectile_Heavy.push_back(wind->loadTexture("img/pl/right/magick_base/1_36.png"));
        game->Projectile_Heavy.push_back(wind->loadTexture("img/pl/right/magick_base/1_37.png"));
        game->Projectile_Heavy.push_back(wind->loadTexture("img/pl/right/magick_base/1_38.png"));
        game->Projectile_Heavy.push_back(wind->loadTexture("img/pl/right/magick_base/1_39.png"));
        game->Projectile_Heavy.push_back(wind->loadTexture("img/pl/right/magick_base/1_40.png"));
        game->Projectile_Heavy.push_back(wind->loadTexture("img/pl/right/magick_base/1_41.png"));
        game->Projectile_Heavy.push_back(wind->loadTexture("img/pl/right/magick_base/1_42.png"));
        game->Projectile_Heavy.push_back(wind->loadTexture("img/pl/right/magick_base/1_43.png"));
        game->Projectile_Heavy.push_back(wind->loadTexture("img/pl/right/magick_base/1_44.png"));
        game->Projectile_Heavy.push_back(wind->loadTexture("img/pl/right/magick_base/1_45.png"));
        game->Projectile_Heavy.push_back(wind->loadTexture("img/pl/right/magick_base/1_46.png"));
        game->Projectile_Heavy.push_back(wind->loadTexture("img/pl/right/magick_base/1_47.png"));
        game->Projectile_Heavy.push_back(wind->loadTexture("img/pl/right/magick_base/1_48.png"));
        game->Projectile_Heavy.push_back(wind->loadTexture("img/pl/right/magick_base/1_49.png"));
        game->Projectile_Heavy.push_back(wind->loadTexture("img/pl/right/magick_base/1_50.png"));
        game->Projectile_Heavy.push_back(wind->loadTexture("img/pl/right/magick_base/1_51.png"));
        game->Projectile_Heavy.push_back(wind->loadTexture("img/pl/right/magick_base/1_52.png"));
        game->Projectile_Heavy.push_back(wind->loadTexture("img/pl/right/magick_base/1_53.png"));
        game->Projectile_Heavy.push_back(wind->loadTexture("img/pl/right/magick_base/1_54.png"));
        game->Projectile_Heavy.push_back(wind->loadTexture("img/pl/right/magick_base/1_55.png"));
        game->Projectile_Heavy.push_back(wind->loadTexture("img/pl/right/magick_base/1_56.png"));
        game->Projectile_Heavy.push_back(wind->loadTexture("img/pl/right/magick_base/1_57.png"));
        game->Projectile_Heavy.push_back(wind->loadTexture("img/pl/right/magick_base/1_58.png"));
        game->Projectile_Heavy.push_back(wind->loadTexture("img/pl/right/magick_base/1_59.png"));
        game->Projectile_Heavy.push_back(wind->loadTexture("img/pl/right/magick_base/1_60.png"));
    
    //LOAD HEAVE EXPLOSION
        game->Heavy_Explosion.push_back(wind->loadTexture("img/pl/right/magick_base/explosion-d1.png"));
        game->Heavy_Explosion.push_back(wind->loadTexture("img/pl/right/magick_base/explosion-d2.png"));
        game->Heavy_Explosion.push_back(wind->loadTexture("img/pl/right/magick_base/explosion-d3.png"));
        game->Heavy_Explosion.push_back(wind->loadTexture("img/pl/right/magick_base/explosion-d4.png"));
        game->Heavy_Explosion.push_back(wind->loadTexture("img/pl/right/magick_base/explosion-d5.png"));
        game->Heavy_Explosion.push_back(wind->loadTexture("img/pl/right/magick_base/explosion-d6.png"));
        game->Heavy_Explosion.push_back(wind->loadTexture("img/pl/right/magick_base/explosion-d7.png"));
        game->Heavy_Explosion.push_back(wind->loadTexture("img/pl/right/magick_base/explosion-d8.png"));
        game->Heavy_Explosion.push_back(wind->loadTexture("img/pl/right/magick_base/explosion-d9.png"));
        game->Heavy_Explosion.push_back(wind->loadTexture("img/pl/right/magick_base/explosion-d10.png"));
        game->Heavy_Explosion.push_back(wind->loadTexture("img/pl/right/magick_base/explosion-d11.png"));
        game->Heavy_Explosion.push_back(wind->loadTexture("img/pl/right/magick_base/explosion-d12.png"));
    
    // LOAD Explosion
        game->Explosion.push_back(wind->loadTexture("img/effects/destroy/ex0.png"));
        game->Explosion.push_back(wind->loadTexture("img/effects/destroy/ex1.png"));
        game->Explosion.push_back(wind->loadTexture("img/effects/destroy/ex2.png"));
        game->Explosion.push_back(wind->loadTexture("img/effects/destroy/ex3.png"));
        game->Explosion.push_back(wind->loadTexture("img/effects/destroy/ex4.png"));
        game->Explosion.push_back(wind->loadTexture("img/effects/destroy/ex5.png"));
        game->Explosion.push_back(wind->loadTexture("img/effects/destroy/ex6.png"));
    
    // LOAD flying enemy RIGHT
        game->FlyingEnR.push_back(wind->loadTexture("img/enemy/flying/right/fly0R.png"));
        game->FlyingEnR.push_back(wind->loadTexture("img/enemy/flying/right/fly0R.png"));
        game->FlyingEnR.push_back(wind->loadTexture("img/enemy/flying/right/fly0R.png"));
        game->FlyingEnR.push_back(wind->loadTexture("img/enemy/flying/right/fly0R.png"));
        game->FlyingEnR.push_back(wind->loadTexture("img/enemy/flying/right/fly0R.png"));
        game->FlyingEnR.push_back(wind->loadTexture("img/enemy/flying/right/fly1R.png"));
        game->FlyingEnR.push_back(wind->loadTexture("img/enemy/flying/right/fly1R.png"));
        game->FlyingEnR.push_back(wind->loadTexture("img/enemy/flying/right/fly1R.png"));
        game->FlyingEnR.push_back(wind->loadTexture("img/enemy/flying/right/fly1R.png"));
        game->FlyingEnR.push_back(wind->loadTexture("img/enemy/flying/right/fly1R.png"));	
        game->FlyingEnR.push_back(wind->loadTexture("img/enemy/flying/right/fly2R.png"));
        game->FlyingEnR.push_back(wind->loadTexture("img/enemy/flying/right/fly2R.png"));
        game->FlyingEnR.push_back(wind->loadTexture("img/enemy/flying/right/fly2R.png"));
        game->FlyingEnR.push_back(wind->loadTexture("img/enemy/flying/right/fly2R.png"));		
        game->FlyingEnR.push_back(wind->loadTexture("img/enemy/flying/right/fly2R.png"));
        game->FlyingEnR.push_back(wind->loadTexture("img/enemy/flying/right/fly3R.png"));
        game->FlyingEnR.push_back(wind->loadTexture("img/enemy/flying/right/fly3R.png"));
        game->FlyingEnR.push_back(wind->loadTexture("img/enemy/flying/right/fly3R.png"));
        game->FlyingEnR.push_back(wind->loadTexture("img/enemy/flying/right/fly3R.png"));
        game->FlyingEnR.push_back(wind->loadTexture("img/enemy/flying/right/fly3R.png"));
    // LOAD flying enemy LEFT
        game->FlyingEnL.push_back(wind->loadTexture("img/enemy/flying/left/fly0L.png"));
        game->FlyingEnL.push_back(wind->loadTexture("img/enemy/flying/left/fly0L.png"));
        game->FlyingEnL.push_back(wind->loadTexture("img/enemy/flying/left/fly0L.png"));
        game->FlyingEnL.push_back(wind->loadTexture("img/enemy/flying/left/fly0L.png"));
        game->FlyingEnL.push_back(wind->loadTexture("img/enemy/flying/left/fly0L.png"));
        game->FlyingEnL.push_back(wind->loadTexture("img/enemy/flying/left/fly1L.png"));
        game->FlyingEnL.push_back(wind->loadTexture("img/enemy/flying/left/fly1L.png"));
        game->FlyingEnL.push_back(wind->loadTexture("img/enemy/flying/left/fly1L.png"));
        game->FlyingEnL.push_back(wind->loadTexture("img/enemy/flying/left/fly1L.png"));
        game->FlyingEnL.push_back(wind->loadTexture("img/enemy/flying/left/fly1L.png"));
        game->FlyingEnL.push_back(wind->loadTexture("img/enemy/flying/left/fly2L.png"));
        game->FlyingEnL.push_back(wind->loadTexture("img/enemy/flying/left/fly2L.png"));
        game->FlyingEnL.push_back(wind->loadTexture("img/enemy/flying/left/fly2L.png"));	
        game->FlyingEnL.push_back(wind->loadTexture("img/enemy/flying/left/fly2L.png"));	
        game->FlyingEnL.push_back(wind->loadTexture("img/enemy/flying/left/fly2L.png"));
        game->FlyingEnL.push_back(wind->loadTexture("img/enemy/flying/left/fly3L.png"));
        game->FlyingEnL.push_back(wind->loadTexture("img/enemy/flying/left/fly3L.png"));
        game->FlyingEnL.push_back(wind->loadTexture("img/enemy/flying/left/fly3L.png"));
        game->FlyingEnL.push_back(wind->loadTexture("img/enemy/flying/left/fly3L.png"));
        game->FlyingEnL.push_back(wind->loadTexture("img/enemy/flying/left/fly3L.png"));
        // Enemy dying text
        game->FlyDeath.push_back(wind->loadTexture("img/enemy/dying0.png"));
        game->FlyDeath.push_back(wind->loadTexture("img/enemy/dying0.png"));
        game->FlyDeath.push_back(wind->loadTexture("img/enemy/dying0.png"));
        game->FlyDeath.push_back(wind->loadTexture("img/enemy/dying1.png"));
        game->FlyDeath.push_back(wind->loadTexture("img/enemy/dying1.png"));
        game->FlyDeath.push_back(wind->loadTexture("img/enemy/dying1.png"));		
        game->FlyDeath.push_back(wind->loadTexture("img/enemy/dying2.png"));
        game->FlyDeath.push_back(wind->loadTexture("img/enemy/dying2.png"));
        game->FlyDeath.push_back(wind->loadTexture("img/enemy/dying2.png"));	
        //Locked Door
        door1->DoorLocked.push_back(wind->loadTexture("img/bg/door/door0.png"));
        door1->DoorLocked.push_back(wind->loadTexture("img/bg/door/door1.png"));
        door1->DoorLocked.push_back(wind->loadTexture("img/bg/door/door2.png"));
        door1->DoorLocked.push_back(wind->loadTexture("img/bg/door/door3.png"));
        door1->DoorLocked.push_back(wind->loadTexture("img/bg/door/door4.png"));
        door1->DoorLocked.push_back(wind->loadTexture("img/bg/door/door5.png"));
        door1->DoorLocked.push_back(wind->loadTexture("img/bg/door/door6.png"));
        door1->DoorLocked.push_back(wind->loadTexture("img/bg/door/door7.png"));
        door1->DoorLocked.push_back(wind->loadTexture("img/bg/door/door8.png"));
        door1->DoorLocked.push_back(wind->loadTexture("img/bg/door/door9.png"));
        door1->DoorLocked.push_back(wind->loadTexture("img/bg/door/door10.png"));
        door1->DoorLocked.push_back(wind->loadTexture("img/bg/door/door11.png"));
        door1->DoorLocked.push_back(wind->loadTexture("img/bg/door/door12.png"));
        door1->DoorLocked.push_back(wind->loadTexture("img/bg/door/door13.png"));
        door1->DoorLocked.push_back(wind->loadTexture("img/bg/door/door14.png"));
        door1->DoorLocked.push_back(wind->loadTexture("img/bg/door/door15.png"));		

        //BG
        game->full_black = wind->loadTexture("img/bg/full_black.png"); 

        game->key.push_back(wind->loadTexture("img/props/key00.png"));
        game->key.push_back(wind->loadTexture("img/props/key01.png"));
        game->key.push_back(wind->loadTexture("img/props/key02.png"));
        game->key.push_back(wind->loadTexture("img/props/key03.png"));
        game->key.push_back(wind->loadTexture("img/props/key04.png"));
        game->key.push_back(wind->loadTexture("img/props/key05.png"));
        game->key.push_back(wind->loadTexture("img/props/key06.png"));
        game->key.push_back(wind->loadTexture("img/props/key07.png"));
        game->key.push_back(wind->loadTexture("img/props/key08.png"));
        game->key.push_back(wind->loadTexture("img/props/key09.png"));
        game->key.push_back(wind->loadTexture("img/props/key10.png"));
        game->key.push_back(wind->loadTexture("img/props/key11.png"));

        // STARTING WEAPON TO CHOOSE FROM
        game->base_sword = wind->loadTexture("img/weapons/sword_base.png");	
        game->base_wand = wind->loadTexture("img/weapons/wand_base.png");
    
        //LOAD CHEST text
        chest1->ChestClosed.push_back(wind->loadTexture("img/props/chest/chest_closed0.png"));									
        chest1->ChestClosed.push_back(wind->loadTexture("img/props/chest/chest_closed1.png"));
        chest1->ChestClosed.push_back(wind->loadTexture("img/props/chest/chest_closed2.png"));
        chest1->ChestClosed.push_back(wind->loadTexture("img/props/chest/chest_closed3.png"));
        chest1->ChestClosed.push_back(wind->loadTexture("img/props/chest/chest_closed4.png"));
        chest1->ChestClosed.push_back(wind->loadTexture("img/props/chest/chest_closed5.png"));
        chest1->ChestClosed.push_back(wind->loadTexture("img/props/chest/chest_closed6.png"));
        chest1->ChestClosed.push_back(wind->loadTexture("img/props/chest/chest_closed7.png"));
        chest1->ChestClosed.push_back(wind->loadTexture("img/props/chest/chest_open.png"));
    
        // UI TEXT
        game->firstSwordMSG = wind->loadTexture("img/UI/SwordChoice.png");
        game->firstWandMSG = wind->loadTexture("img/UI/WandChoice.png");
        game->AcceptW = wind->loadTexture("img/UI/AcceptWeapon.png");
        game->RefuseW = wind->loadTexture("img/UI/RefuseWeapon.png");
    
    
        //SMOKE EFFECT
        game->Smoke.push_back(wind->loadTexture("img/effects/smoke/Smoke1.png"));
        game->Smoke.push_back(wind->loadTexture("img/effects/smoke/Smoke2.png"));
        game->Smoke.push_back(wind->loadTexture("img/effects/smoke/Smoke3.png"));
        game->Smoke.push_back(wind->loadTexture("img/effects/smoke/Smoke4.png"));
        game->Smoke.push_back(wind->loadTexture("img/effects/smoke/Smoke5.png"));
        game->Smoke.push_back(wind->loadTexture("img/effects/smoke/Smoke6.png"));
        game->Smoke.push_back(wind->loadTexture("img/effects/smoke/Smoke7.png"));
        game->Smoke.push_back(wind->loadTexture("img/effects/smoke/Smoke8.png"));
        game->Smoke.push_back(wind->loadTexture("img/effects/smoke/Smoke9.png"));
        game->Smoke.push_back(wind->loadTexture("img/effects/smoke/Smoke10.png"));
        game->Smoke.push_back(wind->loadTexture("img/effects/smoke/Smoke11.png"));
        game->Smoke.push_back(wind->loadTexture("img/effects/smoke/Smoke12.png"));
        game->Smoke.push_back(wind->loadTexture("img/effects/smoke/Smoke13.png"));
        game->Smoke.push_back(wind->loadTexture("img/effects/smoke/Smoke14.png"));
        game->Smoke.push_back(wind->loadTexture("img/effects/smoke/Smoke15.png"));
        game->Smoke.push_back(wind->loadTexture("img/effects/smoke/Smoke16.png"));
        game->Smoke.push_back(wind->loadTexture("img/effects/smoke/Smoke17.png"));
        game->Smoke.push_back(wind->loadTexture("img/effects/smoke/Smoke18.png"));
        game->Smoke.push_back(wind->loadTexture("img/effects/smoke/Smoke19.png"));
    
        // Green SMoke
        game->SmokeG.push_back(wind->loadTexture("img/effects/smokegreen/Poison Cloud1.png"));
        game->SmokeG.push_back(wind->loadTexture("img/effects/smokegreen/Poison Cloud2.png"));
        game->SmokeG.push_back(wind->loadTexture("img/effects/smokegreen/Poison Cloud3.png"));
        game->SmokeG.push_back(wind->loadTexture("img/effects/smokegreen/Poison Cloud4.png"));
        game->SmokeG.push_back(wind->loadTexture("img/effects/smokegreen/Poison Cloud5.png"));
        game->SmokeG.push_back(wind->loadTexture("img/effects/smokegreen/Poison Cloud6.png"));
        game->SmokeG.push_back(wind->loadTexture("img/effects/smokegreen/Poison Cloud7.png"));
        game->SmokeG.push_back(wind->loadTexture("img/effects/smokegreen/Poison Cloud8.png"));
        game->SmokeG.push_back(wind->loadTexture("img/effects/smokegreen/Poison Cloud9.png"));
        game->SmokeG.push_back(wind->loadTexture("img/effects/smokegreen/Poison Cloud10.png"));
        game->SmokeG.push_back(wind->loadTexture("img/effects/smokegreen/Poison Cloud11.png"));
        game->SmokeG.push_back(wind->loadTexture("img/effects/smokegreen/Poison Cloud12.png"));
        game->SmokeG.push_back(wind->loadTexture("img/effects/smokegreen/Poison Cloud13.png"));
        game->SmokeG.push_back(wind->loadTexture("img/effects/smokegreen/Poison Cloud14.png"));
        game->SmokeG.push_back(wind->loadTexture("img/effects/smokegreen/Poison Cloud15.png"));
        game->SmokeG.push_back(wind->loadTexture("img/effects/smokegreen/Poison Cloud16.png"));
        game->SmokeG.push_back(wind->loadTexture("img/effects/smokegreen/Poison Cloud17.png"));
        game->SmokeG.push_back(wind->loadTexture("img/effects/smokegreen/Poison Cloud18.png"));
        game->SmokeG.push_back(wind->loadTexture("img/effects/smokegreen/Poison Cloud19.png"));
    
        //SPrinkles
        game->Sprinkle.push_back(wind->loadTexture("img/effects/sprinkle/sprinkle1.png"));
        game->Sprinkle.push_back(wind->loadTexture("img/effects/sprinkle/sprinkle2.png"));
        game->Sprinkle.push_back(wind->loadTexture("img/effects/sprinkle/sprinkle3.png"));
        game->Sprinkle.push_back(wind->loadTexture("img/effects/sprinkle/sprinkle4.png"));
    
        //Weapons sword notice
        game->RecRustySword = wind->loadTexture("img/UI/RustySwordNotice.png");
        game->RecOldRod = wind->loadTexture("img/UI/OldRodNotice.png");	
    
        //Rusty sword overhead
        game->RoverR = wind->loadTexture("img/weapons/Rusty_Sword/rusty1.png");
        game->RoverL = wind->loadTexture("img/weapons/Rusty_Sword/rusty2.png");
    
        // MAgick 1 anim right
        game->MagickAnimR.push_back(wind->loadTexture("img/pl/right/AttackWand0R.png"));
        game->MagickAnimR.push_back(wind->loadTexture("img/pl/right/AttackWand1R.png"));
        game->MagickAnimR.push_back(wind->loadTexture("img/pl/right/AttackWand2R.png"));		
        game->MagickAnimR.push_back(wind->loadTexture("img/pl/right/magick_base/spark-preview1.png"));
        game->MagickAnimR.push_back(wind->loadTexture("img/pl/right/magick_base/spark-preview2.png"));
        game->MagickAnimR.push_back(wind->loadTexture("img/pl/right/magick_base/spark-preview3.png"));
        game->MagickAnimR.push_back(wind->loadTexture("img/pl/right/magick_base/spark-preview4.png"));
        game->MagickAnimR.push_back(wind->loadTexture("img/pl/right/magick_base/hits-3-1.png"));
        game->MagickAnimR.push_back(wind->loadTexture("img/pl/right/magick_base/hits-3-2.png"));
        game->MagickAnimR.push_back(wind->loadTexture("img/pl/right/magick_base/hits-3-3.png"));
        game->MagickAnimR.push_back(wind->loadTexture("img/pl/right/magick_base/hits-3-4.png"));
        game->MagickAnimR.push_back(wind->loadTexture("img/pl/right/magick_base/hits-3-5.png"));
}