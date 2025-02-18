#pragma once 
#include "Entity.hpp"

class Rock : public Entity
{
    public:
        Rock(float px, float py, SDL_Texture* ptex, int pdirection);
        void update(int wave) override;
        void render(SDL_Renderer* renderer, bool paused) override; 
        SDL_Rect getBounds() override; 
        void Collision(std::vector<Entity*> handler) override; 

    private:
        int direction;
        bool hit;

        int FrameIndex; 
        int FrameCount;
        Uint32 FrameSprite;
};