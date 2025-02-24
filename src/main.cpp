#include <iostream>
#include <time.h>
#include <cmath>
#include <fstream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include "RenderWindow.hpp"
#include "Entity.hpp"
#include "Handler.hpp"
#include "Player.hpp"
#include "Bullet.hpp"
#include "Alien.hpp"
#include "Rock.hpp"

RenderWindow* window;
Handler* handler; 
Player* player; 

SDL_Texture *playerTex; 
SDL_Texture *bulletTex;
SDL_Texture *aleinTex; 
SDL_Texture *rockTex;
SDL_Texture* heart;

SDL_Texture* backgrounds[4];

TTF_Font* font32;
TTF_Font* font64;

const int WIDTH = 575; 
const int HEIGHT = 700;

int score; 
int wave;

int phaseTrack;
int lifeTrack;

bool bossCounter;

bool updateMovement[4];

bool menuState = true;
bool pickState;
bool playState; 
bool enterNameState; 
bool deleteState;
bool leaderBoardState;
bool pausedState;
int stateOption;
  
SDL_Color white = {255, 255, 255, 255};
SDL_Color black = {0, 0, 0, 255};

SDL_Color green = {31, 252, 2, 255};
SDL_Color red = {141, 1, 4, 255};
SDL_Color blue = {0, 46, 93, 255};

SDL_Color gray = {192, 192, 192, 255};

SDL_Color gold = {255, 215, 0, 255}; 
SDL_Color silver = {192, 192, 192, 255};
SDL_Color bronze = {206, 137, 70, 255};

double dualCheck = 30;
int dualCounterCheck;
int dualStop;

int endPhaseCount;

int gameOverTimer;

Mix_Chunk* laserShot;
Mix_Chunk* alienDeath;
Mix_Chunk* gameOver;
Mix_Chunk* shipExplosion;
Mix_Chunk* playerHit;
Mix_Chunk* wrongColor;
Mix_Chunk* menu;

struct user
{
    /* data */
    char name[4];
    int highScore = 0; 
};

std::vector<user> users; 
int userCounter; 
int nameIndex; 

int userPicked;

std::vector<int> scores = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1};

std::string filename = "save.dat";

void saveData(const std::string& filename)
{
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        std::cout << "Could not save data.\n";
        return;
    }

    size_t size = users.size();
    file.write(reinterpret_cast<const char*>(&size), sizeof(size));
    
    file.write(reinterpret_cast<const char*>(users.data()), size * sizeof(user));

    file.close();
}

void loadData(const std::string& filename)
{
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cout << "No file present.\n";
        return;
    }

    size_t size;
    file.read(reinterpret_cast<char*>(&size), sizeof(size));

    users.clear();
    users.resize(size);

    file.read(reinterpret_cast<char*>(users.data()), size * sizeof(user));

    userCounter = size; 

    for(int i = 0; i < size; i++)
    {
        scores[i] = users[i].highScore;
    }

    file.close();
}

void init()
{
    srand(time(NULL)); 

    window = new RenderWindow("Multi-Blast", WIDTH, HEIGHT);
    handler = new Handler(); 

    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        std::cout << "Audio Error: " << Mix_GetError() << std::endl;
    }

    playerTex = window->loadTexture("res/textures/player.png");
    bulletTex = window->loadTexture("res/textures/bullets.png");
    aleinTex = window->loadTexture("res/textures/aliens.png");
    rockTex = window->loadTexture("res/textures/rock.png");
    heart = window->loadTexture("res/textures/heart.png");
    
    backgrounds[0] = window->loadTexture("res/backgrounds/background1.png");
    backgrounds[1] = window->loadTexture("res/backgrounds/background2.png");
    backgrounds[2] = window->loadTexture("res/backgrounds/background3.png");
    backgrounds[3] = window->loadTexture("res/backgrounds/background4.png");

    font32 = TTF_OpenFont("res/font/font.ttf", 32);
    font64 = TTF_OpenFont("res/font/font.ttf", 64);

    laserShot = Mix_LoadWAV("res/sounds/laserShot.wav");
    alienDeath = Mix_LoadWAV("res/sounds/alienDeath.wav");
    gameOver = Mix_LoadWAV("res/sounds/GameOver.wav");
    shipExplosion = Mix_LoadWAV("res/sounds/shipExplosion.wav");
    playerHit = Mix_LoadWAV("res/sounds/playerHit.wav");
    wrongColor = Mix_LoadWAV("res/sounds/wrongColor.wav");
    menu = Mix_LoadWAV("res/sounds/Menu.wav");

    player = new Player(235, 650, playerTex, playerHit);

    handler->addObject(player);

    loadData(filename);
}

void addBullet(int type)
{
    Bullet *bullet = new Bullet(player->getX() + 13 + 70*type, player->getY() - 15, bulletTex, type);
    if(bullet->closeCheck(handler->handler))
    {
        dualCheck -= (1.75 - 0.05*wave);
        dualCounterCheck = 120;
    } 
    handler->addObject(bullet);
    Mix_PlayChannel(-1, laserShot, 0);
}

void addAliensType(int type)
{
    switch(type)
    {
        case 0: 
        {
            int color = rand() % 2; 
            int x = 20 + rand() % 470;
            Alien *alien = new Alien(x, -50, aleinTex, color, 250, alienDeath, shipExplosion, wrongColor);
            handler->addObject(alien);
            break;
        }
        
        case 1:
        {
            for(int i = 1; i < 9; i++)
            {
                int color = rand() % 2;
                Alien *alien = new Alien(64 * i - 35, -50, aleinTex, color, 250, alienDeath, shipExplosion, wrongColor);
                handler->addObject(alien);
            }
            break; 
        }
        
        case 2:
        {
            int x = rand() % 450;

            Alien *alien1 = new Alien(x, -50, aleinTex, 0, 0, alienDeath, shipExplosion, wrongColor);
            Alien *alien2 = new Alien(x, -102, aleinTex, 0, 0, alienDeath, shipExplosion, wrongColor);
            Alien *alien3 = new Alien(x + 64, -50, aleinTex, 1, 0, alienDeath, shipExplosion, wrongColor);
            Alien *alien4 = new Alien(x + 64, -102, aleinTex, 1, 0, alienDeath, shipExplosion, wrongColor);

            handler->addObject(alien1);
            handler->addObject(alien2);
            handler->addObject(alien3);
            handler->addObject(alien4); 
            break;
        }
        
        case 3:
        {
            int x = rand() % 386;

            Alien *alien1 = new Alien(x + 64, -154, aleinTex, 0, 0, alienDeath, shipExplosion, wrongColor);
            Alien *alien2 = new Alien(x, -102, aleinTex, 0, 0, alienDeath, shipExplosion, wrongColor);
            Alien *alien3 = new Alien(x + 128, -102, aleinTex, 1, 0, alienDeath, shipExplosion, wrongColor);
            Alien *alien4 = new Alien(x + 64, -50, aleinTex, 1, 0, alienDeath, shipExplosion, wrongColor);

            handler->addObject(alien1);
            handler->addObject(alien2);
            handler->addObject(alien3);
            handler->addObject(alien4);
            break; 
        }
        
        case 4:
        {
            int color = rand() % 2; 
            int x = (color * 800) - 50;  
            Alien *alien = new Alien(x, 50, aleinTex, color + 2, 0, alienDeath, shipExplosion, wrongColor);
            handler->addObject(alien);
            break;
        }
        
        case 5:
        {
            for(int i = 0; i < (floor(wave / 2) + 1); i++)
            {
                Alien *alien = new Alien(-(64*i), 50, aleinTex, 4, 0, alienDeath, shipExplosion, wrongColor);
                handler->addObject(alien);
            }
            break;
        }

        case 6:
        {
            int x = rand() % 100;

            Alien *alien1 = new Alien(x, -50, aleinTex, 0, 0, alienDeath, shipExplosion, wrongColor);
            Alien *alien2 = new Alien(x, -102, aleinTex, 0, 0, alienDeath, shipExplosion, wrongColor);

            Alien *alien3 = new Alien(x + 64, -50, aleinTex, 1, 0, alienDeath, shipExplosion, wrongColor);
            Alien *alien4 = new Alien(x + 64, -102, aleinTex, 1, 0, alienDeath, shipExplosion, wrongColor);

            Alien *alien5 = new Alien(x + 32, -152, aleinTex, 0, 0, alienDeath, shipExplosion, wrongColor);
            Alien *alien6 = new Alien(x + 64, -204, aleinTex, 0, 0, alienDeath, shipExplosion, wrongColor);
            Alien *alien7 = new Alien(x + 96, -256, aleinTex, 0, 0, alienDeath, shipExplosion, wrongColor);

            Alien *alien8 = new Alien(x + 96, -152, aleinTex, 1, 0, alienDeath, shipExplosion, wrongColor);
            Alien *alien9 = new Alien(x + 128, -204, aleinTex, 1, 0, alienDeath, shipExplosion, wrongColor);
            Alien *alien10 = new Alien(x + 160, -256, aleinTex, 1, 0, alienDeath, shipExplosion, wrongColor);

            Alien *alien11 = new Alien(x + 96, -308, aleinTex, 0, 0, alienDeath, shipExplosion, wrongColor);
            Alien *alien12 = new Alien(x + 96, -360, aleinTex, 0, 0, alienDeath, shipExplosion, wrongColor);

            Alien *alien13 = new Alien(x + 160, -308, aleinTex, 1, 0, alienDeath, shipExplosion, wrongColor);
            Alien *alien14 = new Alien(x + 160, -360, aleinTex, 1, 0, alienDeath, shipExplosion, wrongColor);

            handler->addObject(alien1);
            handler->addObject(alien2);
            handler->addObject(alien3);
            handler->addObject(alien4);
            handler->addObject(alien5);
            handler->addObject(alien6);
            handler->addObject(alien7);
            handler->addObject(alien8);
            handler->addObject(alien9);
            handler->addObject(alien10);
            handler->addObject(alien11);
            handler->addObject(alien12);
            handler->addObject(alien13);
            handler->addObject(alien14); 
            break; 
        }

        case 7:
        { 
            int x = (rand() % 160) + 351;

            Alien *alien1 = new Alien(x, -50, aleinTex, 1, 0, alienDeath, shipExplosion, wrongColor);
            Alien *alien2 = new Alien(x, -102, aleinTex, 1, 0, alienDeath, shipExplosion, wrongColor);

            Alien *alien3 = new Alien(x - 64, -50, aleinTex, 0, 0, alienDeath, shipExplosion, wrongColor);
            Alien *alien4 = new Alien(x - 64, -102, aleinTex, 0, 0, alienDeath, shipExplosion, wrongColor);

            Alien *alien5 = new Alien(x - 32, -152, aleinTex, 1, 0, alienDeath, shipExplosion, wrongColor);
            Alien *alien6 = new Alien(x - 64, -204, aleinTex, 1, 0, alienDeath, shipExplosion, wrongColor);
            Alien *alien7 = new Alien(x - 96, -256, aleinTex, 1, 0, alienDeath, shipExplosion, wrongColor);

            Alien *alien8 = new Alien(x - 96, -152, aleinTex, 0, 0, alienDeath, shipExplosion, wrongColor);
            Alien *alien9 = new Alien(x - 128, -204, aleinTex, 0, 0, alienDeath, shipExplosion, wrongColor);
            Alien *alien10 = new Alien(x - 160, -256, aleinTex, 0, 0, alienDeath, shipExplosion, wrongColor);

            Alien *alien11 = new Alien(x - 96, -308, aleinTex, 1, 0, alienDeath, shipExplosion, wrongColor);
            Alien *alien12 = new Alien(x - 96, -360, aleinTex, 1, 0, alienDeath, shipExplosion, wrongColor);

            Alien *alien13 = new Alien(x - 160, -308, aleinTex, 0, 0, alienDeath, shipExplosion, wrongColor);
            Alien *alien14 = new Alien(x - 160, -360, aleinTex, 0, 0, alienDeath, shipExplosion, wrongColor);

            handler->addObject(alien1);
            handler->addObject(alien2);
            handler->addObject(alien3);
            handler->addObject(alien4);
            handler->addObject(alien5);
            handler->addObject(alien6);
            handler->addObject(alien7);
            handler->addObject(alien8);
            handler->addObject(alien9);
            handler->addObject(alien10);
            handler->addObject(alien11);
            handler->addObject(alien12);
            handler->addObject(alien13);
            handler->addObject(alien14);
            break;
        } 
        
        case 8:
        {
            int x = 20 + rand() % 470;
            for(int i = 1; i < 9; i++)
            {
                int color = rand() % 2;
                Alien *alien = new Alien(x, -52 * i, aleinTex, color, 0, alienDeath, shipExplosion, wrongColor);
                handler->addObject(alien);
            }
            break;
        } 

        case 9: 
        {
            for(int i = 9; i > 1; i--)
            {
                int color = rand() % 2; 
                Alien *alien = new Alien(64 * (10 - i) - 35, -30 * i, aleinTex, color, 50 * (10 - i), alienDeath, shipExplosion, wrongColor);
                handler->addObject(alien);
            } 
            break; 
        }
        
        case 10: 
        {
            for(int i = 9; i > 1; i--)
            {
                int color = rand() % 2; 
                Alien *alien = new Alien(64 * i - 95, -30 * i, aleinTex, color, 50 * (10 - i), alienDeath, shipExplosion, wrongColor);
                handler->addObject(alien);
            }
            break;
        }
        
        case 11: 
        {
            Alien *alien = new Alien(210, -50, aleinTex, 5, 0, alienDeath, shipExplosion, wrongColor);
            handler->addObject(alien);
            break;
        }
    }
}

void addAliens(int* counter)
{
    if(0 <= phaseTrack && phaseTrack < 100 && *counter % 60 == 0)
    {
        addAliensType(0);
    }
    else if(100 <= phaseTrack && phaseTrack < 250 && *counter % 90 == 0)
    {
        int pattern = rand() % 101;

        if(pattern >= 0 && pattern < 41)
        {
            addAliensType(1);
        }
        
        else if(pattern >= 41 && pattern < 68)
        {
            addAliensType(2);
        }

        else if(pattern >= 68 && pattern < 95)
        {
            addAliensType(3);
        }

        else
        {
            addAliensType(4);
        }
    }

    else if(250 <= phaseTrack && phaseTrack < 350 && *counter % 90 == 0)
    {
        int pattern = rand() % 101;

        if(0 <= pattern && pattern < 40)
        {
            addAliensType(5);
        }

        else if(40 <= pattern && pattern < 65)
        {
            addAliensType(1);
        }

        else if(65 <= pattern && pattern < 80)
        {
            addAliensType(2);
        }
        else if(80 <= pattern && pattern < 95)
        {
            addAliensType(3);
        }
        else
        {
            addAliensType(4);
        }
    }

    else if(350 <= phaseTrack && phaseTrack < 650 && *counter % 180 == 0)
    {
        int pattern = rand() % 101;

        if(0 <= pattern && pattern < 45)
        {
            addAliensType(6);
        }
        else if(45 <= pattern && pattern < 90)
        {
            addAliensType(7);
        }
        else
        {
            addAliensType(4);
        }   
    }

    else if(650 <= phaseTrack && phaseTrack < 900 && *counter % 180 == 0)
    {
        int pattern = rand() % 101;

        if(0 <= pattern && pattern < 30)
        {
            addAliensType(8);
        }
        else if(30 <= pattern && pattern < 60)
        {
            addAliensType(9);
        }
        else if(60 <= pattern && pattern < 90)
        {
            addAliensType(10);
        }
        else
        {
            addAliensType(4);
        }
    }

    else if(phaseTrack >= 900 && *counter % 180 == 0)
    {
        if(!bossCounter)
        {
            addAliensType(11);
            bossCounter = true;
        }

        if(endPhaseCount >= 181 && endPhaseCount <= 1800 - 75 * (wave))
        {
            int pattern = rand() % 101;

            int x;
            int direction;

            if(pattern >= 0 && pattern < 50)
            {
                x = -50;
                direction = 1;
            }
            else
            {
                x = 625;
                direction = -1;
            }

            Rock *rock = new Rock(x, 550, rockTex, direction);
            handler->addObject(rock);
        }
    }
    
}

void HUD()
{
    char s[10]; 
    sprintf(s, "%d", score);
    window->renderText(10, 0, users[userPicked].name, font32, white);
    window->renderText(10, 35, s, font32, white);

    for(int i = 0; i < player->getLives(); i++)
    {
        SDL_Rect src = {0, 0, 20, 17}; 

        SDL_Rect dst = {390 + 60*i, 10, 60, 51}; 

        SDL_RenderCopy(window->getRenderer(), heart, &src, &dst);
    }

    for(int i = 0; i < 3 - player->getLives(); i++)
    {
        SDL_Rect src = {21, 0, 20, 17}; 

        SDL_Rect dst = {338 + 60 * (3 - i), 10, 60, 51}; 

        SDL_RenderCopy(window->getRenderer(), heart, &src, &dst);
    }

    SDL_Rect outer = {10, 70, 150, 10};
    SDL_Rect inner;
    inner.x = 10; 
    inner.y = 70; 
    inner.w = (5*dualCheck);
    inner.h = 10;

    SDL_SetRenderDrawColor(window->getRenderer(), 173, 216, 230, 0);
    SDL_RenderFillRect(window->getRenderer(), &inner);

    SDL_SetRenderDrawColor(window->getRenderer(), 255, 255, 255, 0);
    SDL_RenderDrawRect(window->getRenderer(), &outer);

    SDL_SetRenderDrawColor(window->getRenderer(), 48, 25, 52, 255);
}

void backGround()
{
    Uint32 seconds = SDL_GetTicks() / 200; 
	Uint32 sprite = seconds % 4; 
    
    SDL_Rect src;
    src.x = src.y = 0;
    src.w = 256; 
    src.h = 224; 

    SDL_Rect dst;
    dst.x = 0;
    dst.y = 0;
    dst.w = WIDTH;
    dst.h = HEIGHT;

    SDL_RenderCopy(window->getRenderer(), backgrounds[sprite], &src, &dst);
}

void update()
{
    player->setX(player->getX() - (6 * updateMovement[0]));
    player->setX(player->getX() + (6 * updateMovement[1])); 

    wave = floor(score / 1000);
    
    if(wave >= 8)
    {
        wave = 8;
    } 
    
    if(phaseTrack >= 1000)
    {
        phaseTrack = 0;
        bossCounter = false;
        endPhaseCount = 0;
    }

    if(lifeTrack && player->getLives() < 3)
    {
        player->setLives(player->getLives() + 1);
        lifeTrack = 0;
    } 
    else if (lifeTrack && player->getLives() >= 3)
    {
        score += 30;
        phaseTrack += 30;
        lifeTrack = 0;
    }

    if(phaseTrack >= 900) endPhaseCount++;

    //Used to stop multi-spamming
    dualCounterCheck--;

    if(dualCounterCheck < 0) dualCounterCheck = 0;

    if(dualCounterCheck == 0 && dualCheck > 0)
    {
        dualCheck += 0.20 * (wave + 1);
    }

    if(dualCheck == 0)
    {
        dualStop++; 
    }

    if(dualStop == 600 - (wave * 56))
    {
        dualCheck = 30;
        dualStop = 0;
    }

    if(dualCheck > 30) dualCheck = 30;
    if(dualCheck < 0) dualCheck = 0;
}

void reset()
{
    if(score > users[userPicked].highScore)
    {
        users[userPicked].highScore = score;
        scores[userPicked] = score;
    }

    menuState = true; 
    pickState = false;
    playState = false; 
    enterNameState = false; 
    deleteState = false;
    leaderBoardState = false;
    pausedState = false;
    stateOption = 0;

    score = 0;
    wave = 0;

    player->reset();

    handler->playerOnly();

    handler->addObject(player);

    phaseTrack = 0;
    lifeTrack = 0;

    bossCounter = 0;

    dualCheck = 30;
    dualCounterCheck = 0;
    dualStop = 0;

    endPhaseCount = 0;

    gameOverTimer = 0;
 
    nameIndex = 0; 
    userPicked = 0;
}

void drawTriangle(SDL_Renderer* renderer, int x1, int x2, int x3, int y1, int y2, int y3) {
    // Draw lines between the three points
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
    SDL_RenderDrawLine(renderer, x2, y2, x3, y3);
    SDL_RenderDrawLine(renderer, x3, y3, x1, y1);
}

std::vector<int> rankings()
{
    std::vector<int> userRankings = {-1, -1, -1, -1, -1};

    int first = 0;
    int second = 0; 
    int third = 0; 
    int fourth = 0; 
    int fifth = 0; 

    for(int i = 0; i < userCounter; i++)
    {
        if(scores[i] > first)
        {
            first = scores[i]; 
            userRankings[0] = i;
        }
    }

    for(int i = 0; i < userCounter; i++)
    {
        if(scores[i] > second && i != userRankings[0])
        {
            second = scores[i]; 
            userRankings[1] = i;
        }
    }

    for(int i = 0; i < userCounter; i++)
    {
        if(scores[i] > third && i != userRankings[0] && i != userRankings[1])
        {
            third = scores[i]; 
            userRankings[2] = i;
        }
    }

    for(int i = 0; i < userCounter; i++)
    {
        if(scores[i] > fourth && i != userRankings[0] && i != userRankings[1] && i != userRankings[2])
        {
            fourth = scores[i]; 
            userRankings[3] = i;
        }
    }

    for(int i = 0; i < userCounter; i++)
    {
        if(scores[i] > fifth && i != userRankings[0] && i != userRankings[1] && i != userRankings[2] && i != userRankings[3])
        {
            fifth = scores[i]; 
            userRankings[4] = i;
        }
    }

    return userRankings; 
    
}

void gameLoop()
{
    bool running = true; 
    
    const int FPS = 60; 
    const int frameDelay = 1000 / FPS; 

    Uint32 frameStart;
    int frameTime;

    int counter = 0;

    SDL_StartTextInput();

    while(running)
    {
        frameStart = SDL_GetTicks(); 

        SDL_Event event; 

        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
            {
                running = false; 
            }

            else if(event.type == SDL_KEYDOWN)
            {
                switch(event.key.keysym.sym)
                {
                    case SDLK_a:
                        updateMovement[0] = true; 
                        break;
                    
                    case SDLK_d:
                        updateMovement[1] = true; 
                        break;
                    
                    case SDLK_LEFT: 
                        if(!updateMovement[2] && dualCheck > 0 && playState && !pausedState) addBullet(0);
                        updateMovement[2] = true;
                        break; 
                    
                    case SDLK_RIGHT: 
                        if(!updateMovement[3] && dualCheck > 0 && playState && !pausedState) addBullet(1);
                        updateMovement[3] = true;
                        break;
                    
                }
            }

            else if(event.type == SDL_KEYUP)
            {
                switch(event.key.keysym.sym)
                {
                    case SDLK_a:
                        updateMovement[0] = false; 
                        break;
                    
                    case SDLK_d:
                        updateMovement[1] = false; 
                        break;
                    
                    case SDLK_LEFT: 
                        updateMovement[2] = false;
                        break; 
                    
                    case SDLK_RIGHT: 
                        updateMovement[3] = false;
                        break;
                    
                    case SDLK_UP:
                        if(menuState)
                        {
                          stateOption -= 1;   
                          Mix_PlayChannel(-1, menu, 0);
                        }
                        else if(pickState || deleteState)
                        {
                            userPicked -= 1;
                            Mix_PlayChannel(-1, menu, 0);
                        }
                        break; 
                    
                    case SDLK_DOWN:
                        if(menuState)
                        {
                          stateOption += 1;   
                          Mix_PlayChannel(-1, menu, 0);
                        }
                        else if(pickState || deleteState)
                        {
                            userPicked += 1;
                            Mix_PlayChannel(-1, menu, 0);
                        }
                        break;
                    
                    case SDLK_SPACE:
                        if(playState) pausedState = !pausedState;
                        break;
                    
                    case SDLK_ESCAPE: 
                        if(playState && player->getLives() <= 0)
                        {
                            reset();
                        }
                        else if(pickState || deleteState || leaderBoardState)
                        {
                            pickState = false;
                            deleteState = false; 
                            leaderBoardState = false;
                            menuState = true;
                            userPicked = 0;
                        }
                        break;
                    
                    case SDLK_RETURN:
                        if(menuState)
                        {
                            if(stateOption == 0 && userCounter > 0)
                            {
                                menuState = !menuState;
                                pickState = true;
                            } 
                            if(stateOption == 1 && userCounter < 10)
                            {
                                menuState = !menuState;
                                enterNameState = true; 
                                stateOption = 0;
                                user newUser;
                                users.push_back(newUser);
                                nameIndex = 0;
                            }

                            if(stateOption == 2 && userCounter > 0)
                            {
                                menuState = !menuState;
                                deleteState = true;
                                stateOption = 0;
                            }

                            if(stateOption == 3)
                            {
                                menuState = !menuState;
                                leaderBoardState = true;
                                stateOption = 0;
                            }
                            if(stateOption == 4) running = false;
                        }
                        else if(enterNameState)
                        {
                            if(nameIndex == 3)
                            {
                                enterNameState = !enterNameState;
                                menuState = true;
                                nameIndex = 0;
                                userCounter += 1;
                            }
                        }
                        else if(pickState)
                        {
                            pickState = !pickState;
                            playState = true;
                        }

                        else if(deleteState)
                        {
                            users.erase(users.begin() + userPicked); 
                            scores.erase(scores.begin() + userPicked);
                            userPicked = 0;
                            deleteState = !deleteState;
                            menuState = true;
                            userCounter--;
                        }
                        break;
                }
            }

            else if(event.type == SDL_TEXTINPUT)
            {
                if(nameIndex < 3 && enterNameState)
                {
                    users[userCounter].name[nameIndex++] = event.text.text[0];
                    users[userCounter].name[nameIndex] = '\0';
                }
            }
        }

        if(menuState)
        {
            window->clear(); 

            backGround();

            window->renderText(WIDTH/8 + 65, sin(SDL_GetTicks()/100)*2, "Multi", font64, green);
            window->renderText(WIDTH/8 + 195.5, sin(SDL_GetTicks()/100)*2, "-", font64, blue);
            window->renderText(WIDTH/8 + 230, sin(SDL_GetTicks()/100)*2, "Blast", font64, red);

            window->renderText(WIDTH/8 + 120, 175, "Play", font32, gray);
            window->renderText(WIDTH/8 + 120, 275, "Enter a Name", font32, gray);
            window->renderText(WIDTH/8 + 120, 375, "Delete a Name", font32, gray);
            window->renderText(WIDTH/8 + 120, 475, "LeaderBoard", font32, gray);
            window->renderText(WIDTH/8 + 120, 575, "Quit", font32, gray);

            if(stateOption > 4) stateOption = 0; 
            if(stateOption < 0) stateOption = 4;

            SDL_SetRenderDrawColor(window->getRenderer(), 255, 255, 255, 255);

            drawTriangle(window->getRenderer(), 150, 100, 100, 95 + ((stateOption+1)*100), 70 + ((stateOption+1)*100), 120 + ((stateOption+1)*100));

            SDL_SetRenderDrawColor(window->getRenderer(), 48, 25, 52, 255);

            if(userCounter == 0) window->renderText(WIDTH/8 + 70, 650, "Enter a name to play.", font32, green);
            else if(userCounter == 10) window->renderText(WIDTH/8 + 10, 650, "Reached max amount of players.", font32, green);

            window->display();
        }

        else if(enterNameState)
        {
            window->clear(); 

            backGround();

            window->renderText(WIDTH/8 + 50, 275, "Enter Name:", font32, gray);
            if(nameIndex != 0) window->renderText(WIDTH/8 + 250, 275, users[userCounter].name, font32, green);
            window->renderText(WIDTH/8 + 120, 375, "Return", font32, gray);

            SDL_SetRenderDrawColor(window->getRenderer(), 255, 255, 255, 255);

            if(nameIndex == 3) drawTriangle(window->getRenderer(), 150, 100, 100, 395, 370, 420);

            SDL_SetRenderDrawColor(window->getRenderer(), 48, 25, 52, 255);

            window->display(); 
        }

        else if(deleteState)
        {
            window->clear();

            backGround();

            window->renderText(WIDTH/8 + 90, 0, "Whos Being Deleted?", font32, green);

            for(int i = 0; i < userCounter; i++)
            {
                window->renderText(WIDTH/8 + 120, 60 * (i + 1), users[i].name, font32, red);
            }

            if(userPicked > userCounter-1) userPicked = 0;
            if(userPicked < 0) userPicked = userCounter - 1;

            window->renderText(WIDTH/8 + 20, 650, "Press ESC to return to Menu.", font32, green);

            SDL_SetRenderDrawColor(window->getRenderer(), 255, 255, 255, 255);

            drawTriangle(window->getRenderer(), 150, 100, 100, 80 + 60*(userPicked), 60 + 60*(userPicked), 100 + 60*(userPicked));

            SDL_SetRenderDrawColor(window->getRenderer(), 48, 25, 52, 255);

            window->display(); 
        }

        else if(leaderBoardState)
        {
            window->clear();

            backGround();

            window->renderText(225, 0, "Rankings", font32, green);

            window->renderText(100, 150, "1st:", font32, gold);
            window->renderText(100, 250, "2nd:", font32, silver);
            window->renderText(100, 350, "3rd:", font32, bronze);
            window->renderText(100, 450, "4th:", font32, white);
            window->renderText(100, 550, "5th:", font32, white);

            std::vector<int> ranks = rankings(); 

            for(int i = 0; i < 5; i ++)
            {
                if(ranks[i] != -1)
                {
                    window->renderText(250, 150 + (100*i), users[ranks[i]].name, font32, white);  

                    char s[10]; 
                    sprintf(s, "%d", users[ranks[i]].highScore);

                    window->renderText(400, 150 + (100*i), s, font32, white); 
                }
            }

            window->renderText(WIDTH/8 + 20, 650, "Press ESC to return to Menu.", font32, green);

            window->display(); 
        }

        else if(pickState)
        {
            window->clear();

            backGround();

            window->renderText(WIDTH/8 + 120, 0, "Whos Playing?", font32, green);

            for(int i = 0; i < userCounter; i++)
            {
                window->renderText(WIDTH/8 + 120, 60 * (i + 1), users[i].name, font32, red);
            }

            if(userPicked > userCounter-1) userPicked = 0;
            if(userPicked < 0) userPicked = userCounter - 1;

            window->renderText(WIDTH/8 + 20, 650, "Press ESC to return to Menu.", font32, green);

            SDL_SetRenderDrawColor(window->getRenderer(), 255, 255, 255, 255);

            drawTriangle(window->getRenderer(), 150, 100, 100, 80 + 60*(userPicked), 60 + 60*(userPicked), 100 + 60*(userPicked));

            SDL_SetRenderDrawColor(window->getRenderer(), 48, 25, 52, 255);

            window->display(); 
        }

        else if(playState && !pausedState && player->getLives() > 0)
        {
            update();

            counter++;

            window->clear(); 

            if(counter % 60 == 0)
            {
                addAliens(&counter);
            }

            backGround();
            
            handler->update(wave); 
            handler->render(window->getRenderer(), pausedState);
            handler->collision();

            HUD(); 

            handler->removeObject(&score, &phaseTrack, &lifeTrack);

            window->display(); 
        }

        else if(playState && pausedState && player->getLives() > 0)
        {
            window->clear(); 

            backGround();

            handler->render(window->getRenderer(), pausedState);

            HUD(); 

            window->renderText(215, 320 + sin(SDL_GetTicks()/100)*2, "PAUSED!", font32, green);

            window->display(); 
        }

        else if(playState && player->getLives() <= 0)
        {
            window->clear();

            SDL_SetRenderDrawColor(window->getRenderer(), 0, 0, 0, 255);

            gameOverTimer++; 
            if(gameOverTimer < 5) Mix_PlayChannel(-1, gameOver, 0);

            if(gameOverTimer < 70) handler->render(window->getRenderer(), pausedState);

            if(gameOverTimer >= 100)
            {
                window->renderText(180, 250 + sin(SDL_GetTicks()/100)*2, "GAME OVER!", font32, red);
                window->renderText(100, 320, "FINAL SCORE", font32, blue);

                char s[10]; 
                sprintf(s, "%d", score);
                window->renderText(380, 320, s, font32, white);

                window->renderText(WIDTH/8, 650, "Press ESC to return to Menu.", font32, green); 
            }

            window->display(); 
        }

        frameTime = SDL_GetTicks() - frameStart; 

        if(frameDelay > frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    saveData(filename);

    users.clear();

    handler->clearAll(); 

    TTF_CloseFont(font32);
    TTF_CloseFont(font64);

    Mix_FreeChunk(laserShot);
    Mix_FreeChunk(alienDeath);
    Mix_FreeChunk(gameOver);
    Mix_FreeChunk(shipExplosion);
    Mix_FreeChunk(playerHit);
    Mix_FreeChunk(wrongColor);
    Mix_FreeChunk(menu);

    SDL_DestroyTexture(playerTex); 
    SDL_DestroyTexture(bulletTex);
    SDL_DestroyTexture(aleinTex);
    SDL_DestroyTexture(rockTex);
    SDL_DestroyTexture(heart);

    SDL_DestroyTexture(backgrounds[0]);
    SDL_DestroyTexture(backgrounds[1]);
    SDL_DestroyTexture(backgrounds[2]);
    SDL_DestroyTexture(backgrounds[3]);

    window->cleanUp();

    delete window; 
    delete handler;  
}

int main(int argc, char* argv[])
{
    if(SDL_Init(SDL_INIT_VIDEO) > 0)
    {
        std::cout << "SDL_Init failed: " << SDL_GetError() << std::endl; 
    }

    if(!(IMG_Init(IMG_INIT_PNG)))
    {
        std::cout << "IMG_Init failed: " << SDL_GetError() << std::endl; 

    }

    if(TTF_Init() < 0)
    {
        std::cout << "TTF_Init failed: " << TTF_GetError() << std::endl; 
    }

    if(SDL_Init(SDL_INIT_AUDIO) > 0)
    {
        std::cout << "SDL_Audio failed: " << Mix_GetError() << std::endl; 
    }

    init(); 
    gameLoop();
    

    return 0; 
}