#pragma once 
#include "Entity.hpp"

class Bullet : public Entity
{
    public: 
        Bullet(float px, float py, SDL_Texture* ptex, int pcolor);
        void update(int wave) override;
        void render(SDL_Renderer* renderer, bool paused) override; 
        SDL_Rect getBounds() override;  
        void Collision(std::vector<Entity*> handler) override; 
        bool closeCheck(std::vector<Entity*> handler);
        
    private:
        bool hit;
        int hitCounter;          
};