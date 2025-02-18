#include "Player.hpp"

Player::Player(float px, float py, SDL_Texture* ptex, Mix_Chunk* paudio)
    :Entity{px, py, ptex}
{
	 setType(0);
     setColor(-1);
     lives = 3;
     invincibileF = 0;
     hit = false;
     audio = paudio;

     destroyedIndex = 0; 
     destroyedCount = 0;
     destroyedSprite = 0;
     destroyedTimer = 0;
}

void Player::update(int wave)
{
    setX(clamp(getX(), 0, 455));

    invincibileF += hit;

    if(invincibileF == (300 - (20 * wave)))
    {
        hit = false;
        invincibileF = 0;
    }
}

void Player::render(SDL_Renderer* renderer, bool paused)
{
    if(invincibileF % 4 == 0 && lives > 0) drawSprite(0, 0, 80, 32, getX(), getY(), 120, 48, getTex(), renderer);
    else if(lives <= 0 && destroyedTimer < 45)
    {
        destroyedIndex++; 
        destroyedTimer++;
        if(destroyedIndex > 6)
        {
            destroyedIndex = 0;
            destroyedSprite = destroyedCount % 6;
            destroyedCount++;
        }

        drawSprite(destroyedSprite * 80, 0, 80, 32, getX(), getY(), 120, 48, getTex(), renderer);
    }
}

SDL_Rect Player::getBounds()
{
    SDL_Rect rect; 

    rect.x = (int)getX(); 
    rect.y = (int)getY(); 
    rect.w = 96; 
    rect.h = 48;

    return rect;
}

void Player::Collision(std::vector<Entity*> handler)
{
    for(int i = 0; i < handler.size(); i++)
    {
        Entity* tempObject = handler[i];

        if(!hit && tempObject->getType() == 2 && intersect(getBounds(), tempObject->getBounds()) && lives > 0 && tempObject->getColor() != 5)
        {
            lives -= 1; 
            tempObject->setDeleted(true);
            hit = true;
            Mix_PlayChannel(-1, audio, 0);
        }
        else if(!hit && tempObject->getType() == 2 && intersect(getBounds(), tempObject->getBounds()) && lives > 0 && tempObject->getColor() == 5)
        {
            lives -= 2;
            hit = true;
            Mix_PlayChannel(-1, audio, 0);
        }
    }
}

int Player::getLives()
{
    return lives;
}

void Player::setLives(int plives)
{
    lives = plives;
}

void Player::reset()
{
    destroyedIndex = 0; 
    destroyedCount = 0;
    destroyedSprite = 0;
    destroyedTimer = 0;
    invincibileF = 0;
    hit = false;
    lives = 3;
    setX(235);
    setY(650);
}