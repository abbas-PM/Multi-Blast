#pragma once 
#include "Entity.hpp"

class Player : public Entity
{
    public: 
        Player(float px, float py, SDL_Texture* ptex, Mix_Chunk* paudio);
        void update(int wave) override;
        void render(SDL_Renderer* renderer, bool paused) override; 
        SDL_Rect getBounds() override;  
        void Collision(std::vector<Entity*> handler) override; 

        int getLives();
        void setLives(int plives);
        void reset();
    
    private:
        int lives;
        int invincibileF;
        bool hit;
        Mix_Chunk* audio;

        int destroyedIndex; 
        int destroyedCount;
        Uint32 destroyedSprite;
        int destroyedTimer;
              
};