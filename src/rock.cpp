#include "Rock.hpp"

Rock::Rock(float px, float py, SDL_Texture* ptex, int pdirection)
    :Entity{px, py, ptex}
    {
        setType(3);
        direction = pdirection;
        hit = false;

        FrameIndex = 0; 
        FrameCount = 0;
        FrameSprite = 0;
    }

void Rock::update(int wave)
{
    if (!hit) setX(getX() + ((5 + 0.25 * wave) * direction));
    else setY(getY() - 10);


    if((direction == 1 && getX() >= 625) || (direction == -1 && getX() <= -50) || (getY() <= -50))
    {
        setDeleted(true);
    }
}

void Rock::render(SDL_Renderer* renderer, bool paused)
{
    if(!paused) FrameIndex++; 
    if(FrameIndex > 6)
    {
        FrameIndex = 0; 
        FrameSprite = FrameCount % 4; 
        FrameCount++; 
    }
   
    SDL_Rect src;
    if(direction == 1) src.x = FrameSprite * 64;
    else src.x =  (3 - FrameSprite) * 64;

    src.y = 0;
    src.w = 64;
    src.h = 64;

    SDL_Rect dst;
    dst.x = getX();
    dst.y = getY();
    dst.w = src.w;
    dst.h = src.h;

    SDL_RenderCopy(renderer, getTex(), &src, &dst);
}

SDL_Rect Rock::getBounds()
{
    SDL_Rect rect; 

    rect.x = (int)getX(); 
    rect.y = (int)getY();
    rect.w = 64; 
    rect.h = 64;
    
    return rect;
}

void Rock::Collision(std::vector<Entity*> handler)
{
    for(int i = 0; i < handler.size(); i++)
    {
        Entity* tempObject = handler[i];

        if(tempObject->getType() == 1 && intersect(getBounds(), tempObject->getBounds()))
        {
            hit = true;
            tempObject->setDeleted(true);
        }
    }
}