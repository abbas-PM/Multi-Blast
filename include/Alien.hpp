#pragma once 
#include "Entity.hpp"

#include <math.h>
#include <time.h>

class Alien : public Entity
{
    public: 
        Alien(float px, float py, SDL_Texture* ptex, int pcolor, int pstopPoint, Mix_Chunk* paudio1, Mix_Chunk* paudio2, Mix_Chunk* paudio3);
        void update(int wave) override;
        void render(SDL_Renderer* renderer, bool paused) override; 
        SDL_Rect getBounds() override; 
        void Collision(std::vector<Entity*> handler) override;   

    private:
        int stopCounter;
        int stopPoint;
        bool angeredState;   
        int bdirection;

        float angle;
        float radius; 
        float angularSpeed;  

        int shipHealth;
        bool destroyed;
        int destroyedCounter;
        int destroyedIndex; 
        int destroyedCount;
        Uint32 destroyedSprite;

        int timeUP;
        int deathdirection;

        Mix_Chunk* audio1;
        Mix_Chunk* audio2;
        Mix_Chunk* audio3;

        bool playOnce;
};