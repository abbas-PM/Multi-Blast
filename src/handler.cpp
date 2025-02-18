#include "Handler.hpp"

//Function to update all entities in handler.
void Handler::update(int wave)
{
    for(int i = 0; i < handler.size(); i++)
    {
        handler[i]->update(wave); 
    }
}

//Function to render all entities in handler.
void Handler::render(SDL_Renderer* renderer, bool paused)
{
    for(int i = 0; i < handler.size(); i++)
    {
        handler[i]->render(renderer, paused); 
    }
}

//Function to check collision between entities in handler.
void Handler::collision()
{
    for(int i = 0; i < handler.size(); i++)
    {
        handler[i]->Collision(handler);
    }
}

//Function to add an entity to handler.
void Handler::addObject(Entity* entity)
{
    handler.push_back(entity);
}

//Function to remove an entity to handler.
void Handler::removeObject(int* scoreTracker, int* phaseTracker, int* lifeTracker)
{
    for(int i = 0; i < handler.size(); i++)
    {
        if (handler[i]->getDeleted())
        {
            if(handler[i]->getType() == 2 && handler[i]->getY() < 700)
            {
                if(handler[i]->getColor() == 0 || handler[i]->getColor() == 1 || handler[i]->getColor() == 4)
                {
                    *scoreTracker = *scoreTracker + 10;
                    *phaseTracker = *phaseTracker + 10;
                }

                else if((handler[i]->getColor() == 2 || handler[i]->getColor() == 3) && handler[i]->getX() >= 0 && handler[i]->getX() <= 700)
                {
                    *lifeTracker = 1;
                }

                else if(handler[i]->getColor() == 5)
                {
                    *scoreTracker += 1000 - *phaseTracker;
                    *phaseTracker += 1000 - *phaseTracker;
                }
            }

            delete handler[i];
            handler.erase(handler.begin() + i);
        }
    }
}

//Function to remove everything from handler 
void Handler::clearAll()
{
    for(int i = 0; i < handler.size(); i++)
    {
        delete handler[i];
    }

    handler.clear();
}

//Function to remove everything from handler except Player
void Handler::playerOnly()
{
    for(int i = 1; i < handler.size(); i++)
    {
        delete handler[i];
    }

    handler.clear();
}

