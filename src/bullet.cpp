#include "Bullet.hpp"

Bullet::Bullet(float px, float py, SDL_Texture* ptex, int pcolor)
    :Entity{px, py, ptex}
{
	setType(1);
    setColor(pcolor);
    hit = false;
    hitCounter = 0;
}

void Bullet::update(int wave)
{
    setY(getY() - (((1 - hit) * 10) + (0.1 * wave)));

    if(getY() <= 0 || hitCounter == 5) setDeleted(true);

    hitCounter += hit; 
    
}

void Bullet::render(SDL_Renderer* renderer, bool paused)
{
    drawSprite(32*getColor() + (hit*32*(2 - getColor())), 0, 32, 32, getX(), getY(), 24, 24, getTex(), renderer);  
}

SDL_Rect Bullet::getBounds()
{
    SDL_Rect rect; 

    rect.x = (int)getX(); 
    rect.y = (int)getY(); 
    rect.w = 24; 
    rect.h = 24;
    
    return rect;
}

void Bullet::Collision(std::vector<Entity*> handler)
{
    for(int i = 0; i < handler.size(); i++)
    {
        Entity* tempObject = handler[i];

        if(tempObject->getType() == 2 && intersect(getBounds(), tempObject->getBounds()) && ((tempObject->getColor() == getColor()) || ((tempObject->getColor() - 2) == getColor()) || (tempObject->getColor() == 4)))
        {
            hit = true;
        }
    }
}

bool Bullet::closeCheck(std::vector<Entity*> handler)
{
    for(int i = 0; i < handler.size(); i++)
    {
        Entity* tempObject = handler[i];
        if(tempObject->getType() == 1 && SDL_abs(tempObject->getY() - getY()) < 48 && tempObject->getColor() != getColor())
        {
            return true;
           
        }
    }

    return false;
}
