#include "Entity.hpp"

Entity::Entity(float px, float py, SDL_Texture* ptex)
    :x(px), y(py), tex(ptex) 
{
    deleted = false; 
}

Entity::~Entity()
{
    deleted = true;
}

float Entity::getX()
{
    return x; 
}

float Entity::getY()
{
    return y; 
}

float Entity::getVelX()
{
    return velX; 
}

float Entity::getVelY()
{
    return velY; 
}

int Entity::getType()
{
    return type;
}

int Entity::getColor()
{
    return color;
}

bool Entity::getDeleted()
{
    return deleted; 
}

SDL_Texture* Entity::getTex()
{
	return tex;
}

void Entity::setX(float px)
{
    x = px; 
}

void Entity::setY(float py)
{
    y = py; 
}

void Entity::setVelX(float px)
{
    velX = px; 
}

void Entity::setVelY(float py)
{
    velY = py; 
}

void Entity::setType(int pt)
{
    type = pt;
}

void Entity::setColor(int pc)
{
    color = pc;
}

void Entity::setDeleted(bool pd)
{
    deleted = pd;
}

void Entity::drawSprite(int sx, int sy, int sw, int sh, int dx, int dy, int dw, int dh, SDL_Texture* tex, SDL_Renderer* renderer)
{
    SDL_Rect src; 
    src.x = sx; 
    src.y = sy; 
    src.w = sw; 
    src.h = sh; 

    SDL_Rect dst; 
    dst.x = dx; 
    dst.y = dy; 
    dst.w = dw;
    dst.h = dh; 

    SDL_RenderCopy(renderer, tex, &src, &dst);
}

float Entity::clamp(float value, float min, float max)
{
    if (value < min)
    {
        value = min; 
    }

    if(value > max)
    {
        value = max; 
    }

    return value; 
}

bool Entity::intersect(SDL_Rect a, SDL_Rect b)
{
	return !(b.x > (a.x + a.w) || (b.x + b.w) < a.x || b.y > (a.y + a.h) || (b.y + b.h) < a.y);
}