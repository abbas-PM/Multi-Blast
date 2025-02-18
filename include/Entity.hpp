#pragma once
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <vector>

class Entity
{
    private:  
        int type;
        int color;
        float x, y; 
        float velX, velY;    
        SDL_Texture* tex; 

        bool deleted;

    public: 
        Entity(float px, float py, SDL_Texture* ptex);
        ~Entity(); 

        float getX(); 
        float getY();
        float getVelX();
        float getVelY(); 
        int getType();
        int getColor();
        bool getDeleted(); 
        SDL_Texture* getTex(); 
        void setX(float px); 
        void setY(float py);
        void setVelX(float px); 
        void setVelY(float py);
        void setType(int pt);
        void setColor(int pc);
        void setDeleted(bool pd);
        void drawSprite(int sx, int sy, int sw, int sh, int dx, int dy, int dw, int dh, SDL_Texture* tex, SDL_Renderer* renderer);
        float clamp(float value, float min, float max);
        bool intersect(SDL_Rect a, SDL_Rect b);
   
        virtual void update(int wave) = 0;  
        virtual void render(SDL_Renderer* renderer, bool paused) = 0;
        virtual SDL_Rect getBounds() = 0; 
        virtual void Collision(std::vector<Entity*> handler) = 0;

};