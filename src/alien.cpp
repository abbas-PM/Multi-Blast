#include "Alien.hpp"

Alien::Alien(float px, float py, SDL_Texture* ptex, int pcolor, int pstopPoint, Mix_Chunk* paudio1, Mix_Chunk* paudio2, Mix_Chunk* paudio3)
    :Entity{px, py, ptex}
{
	setType(2);
    setColor(pcolor);
    stopPoint = pstopPoint;
    stopCounter = 0;
    angeredState = false;
    bdirection = 1;  

    angle = 4.71238898038469;
    radius = 150.0f; 
    angularSpeed = 0.05f;

    shipHealth = 3;
    destroyed = false;
    destroyedCounter = 0;
    destroyedIndex = 0; 
    destroyedCount = 0;
    destroyedSprite = 0;

    timeUP = 0;
    deathdirection = -1;

    audio1 = paudio1;
    audio2 = paudio2;
    audio3 = paudio3;

    playOnce = true;
}

void Alien::update(int wave)
{
    switch(getColor())
    {
        case 0: 
        case 1: 
            if((stopPoint > 0 && (stopCounter == 0 || stopCounter >= 60)) || (stopPoint == 0))
            {
                setY(getY() + (4 + angeredState*5 + (1 + wave * 0.5)));
            }

            if(stopPoint > 0 && getY() >= stopPoint)
            {
                stopCounter += 1;
            }
                   
            break;
        
        case 2:
            setX(getX() + (4.5 + 0.5*angeredState + 0.05*wave));
            if(getX() >= 751) setDeleted(true);
            break; 

        case 3:
            setX(getX() - (4.5 + 0.5*angeredState + 0.05*wave));
            if(getX() <= -51) setDeleted(true);
            break;
        
        case 4: 
            if(getX() <= -320)
            {
                bdirection = 1; 
                setY(getY() + 60);
            } 
            if(getX() >= 831)
            {
                bdirection = -1;
                setY(getY() + 60);
            }

            setX(getX() + 10*bdirection);
            break;
        
        case 5:
            stopCounter++; 

            if(stopCounter <= 60)
            {
                setY(getY() + 2.5);
            }

            else if(stopCounter > 60 && shipHealth > 0 && timeUP < 1800 - 75 * (wave))
            {
                angle += angularSpeed * (-0.5*shipHealth + 2.5);

                setX(210 + radius * -cos(angle));
                setY(250 + radius * sin(angle));

                timeUP++;
                
            }
            else if(stopCounter > 60 && shipHealth > 0 && timeUP >= 1800 - 75 * (wave))
            {
                if(getY() < 575) setY(getY() + 10);
                else
                {
                    if(getX() <= 0) deathdirection = 1;
                    else if(getX() >= 415) deathdirection = -1;

                    setX(getX() + 10*deathdirection);
                }
                
            }

            if(shipHealth <= 0)
            {
                destroyed = true;
                destroyedCounter++;
                if(playOnce) 
                {
                    Mix_PlayChannel(-1, audio2, 0);
                    playOnce = false;
                }
            }

            if(destroyedCounter == 30)
            {
                setDeleted(true);
            } 
            break;
 
    }
    
    if(getY() >= 700)
    {
        setDeleted(true);
    }

}

void Alien::render(SDL_Renderer* renderer, bool paused)
{
    switch(getColor())
    {
        case 0: 
        case 1: 
            drawSprite(angeredState*128 + 32*getColor(), 0, 32, 32, getX(), getY(), 64, 64, getTex(), renderer); 
            break;

        case 2:
            drawSprite(192, 0, 32, 32, getX(), getY(), 64, 64, getTex(), renderer);
            break;

        case 3:
            drawSprite(224, 0, 32, 32, getX(), getY(), 64, 64, getTex(), renderer); 
            break;
        
        case 4:
            drawSprite(96, 0, 32, 32, getX(), getY(), 64, 64, getTex(), renderer);
            break;

        case 5:
            if(!destroyed && shipHealth == 3) drawSprite(64, 0, 32, 32, getX(), getY(), 160, 160, getTex(), renderer); 
            else if(!destroyed && shipHealth == 2) drawSprite(256, 0, 32, 32, getX(), getY(), 160, 160, getTex(), renderer);
            else if(!destroyed && shipHealth == 1) drawSprite(288, 0, 32, 32, getX(), getY(), 160, 160, getTex(), renderer);
            else if(destroyed)
            {
                if(!paused) destroyedIndex++; 
                if(destroyedIndex > 6)
                {
                    destroyedIndex = 0;
                    destroyedSprite = destroyedCount % 4;
                    destroyedCount++;
                }

                drawSprite(destroyedSprite * 64, 32, 60, 64, getX() + 20, getY(), 160, 160, getTex(), renderer);
            }
            break;
    }
}

SDL_Rect Alien::getBounds()
{
    SDL_Rect rect; 

    switch(getColor())
    {
        case 0:
        case 1: 
        case 2: 
        case 3:
        case 4:
        {
            rect.x = (int)getX(); 
            rect.y = (int)getY(); 
            rect.w = 64; 
            rect.h = 52;
            break;
        }

        case 5:
        {
            rect.x = (int)getX(); 
            rect.y = (int)getY() + 29; 
            rect.w = 155; 
            rect.h = 76;
            break;
        }
    }

    return rect;
}

void Alien::Collision(std::vector<Entity*> handler)
{
    for(int i = 0; i < handler.size(); i++)
    {
        Entity* tempObject = handler[i];

        if(tempObject->getType() == 1 && intersect(getBounds(), tempObject->getBounds()))
        {
            if(tempObject->getColor() == getColor() || (getColor() - 2) == tempObject->getColor() || getColor() == 4)
            {
                setDeleted(true);
                Mix_PlayChannel(-1, audio1, 0);
            }
            else if(getColor() == 5)
            {
                tempObject->setDeleted(true);
            }

            else
            {
                angeredState = true;
                tempObject->setDeleted(true);
                stopCounter = 60;
                if(playOnce)
                {
                    Mix_PlayChannel(-1, audio3, 0);
                    playOnce = false;
                }
            }
        }

        else if(tempObject->getType() == 3 && intersect(getBounds(), tempObject->getBounds()))
        {
            if(getColor() == 5)
            {
                shipHealth -= 1;
                tempObject->setDeleted(true);
                if(shipHealth != 0) Mix_PlayChannel(-1, audio1, 0);
            }
            else
            {
                setDeleted(true);
                Mix_PlayChannel(-1, audio1, 0);
            }
        }

        else if(tempObject->getType() == 0 && intersect(getBounds(), tempObject->getBounds()) && getColor() == 5)
        {
            shipHealth = 0;
        }
    }
}