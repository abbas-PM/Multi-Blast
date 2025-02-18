#pragma once
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>

#include "Entity.hpp"

class Handler
{
    public:
        std::vector<Entity*> handler;
        void update(int wave);
        void render(SDL_Renderer* renderer, bool paused); 
        void collision();
        void addObject(Entity* entity);
        void removeObject(int* scoreTracker, int* phaseTracker, int* lifeTracker);
        void clearAll();
        void playerOnly();
};