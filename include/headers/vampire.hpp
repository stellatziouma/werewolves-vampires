#pragma once

#include "sdl2/SDL_image.h"
#include "headers/creature.hpp"

class Vampire : public Creature {
public:
    Vampire(SDL_Renderer* renderer, Map* map, vector<Creature*>* creatures, int scale);
    ~Vampire();

    void update(Map* map); 
    
    void draw();
private:
    // variable which increases when the Vampire is in motion
    int activated;
    // texture of hurt vampire
    SDL_Texture* hurt_texture;
    // texture of cured vampire
    SDL_Texture* cured_texture;
};