#pragma once

#include "sdl2/SDL_image.h"
#include "headers/creature.hpp"

class Werewolf : public Creature {
public:
    Werewolf(SDL_Renderer* renderer, Map* map, vector<Creature*>* creatures, int scale);
    ~Werewolf();

    void update(Map* map);
    
    void draw();
private:
    //variable which increases when the werewolf is in motion
    int activated;
    //texture of hurt werewolf
    SDL_Texture* hurt_texture;
    //texture of cured werewolf
    SDL_Texture* cured_texture;
};