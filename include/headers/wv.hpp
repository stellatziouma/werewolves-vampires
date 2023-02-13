#pragma once

#include "sdl2/SDL_image.h"
#include "headers/creature.hpp"

class WV : public Creature {
public:
    WV(SDL_Renderer* renderer, Map* map, vector<Creature*>* creatures, int scale, bool w);
    ~WV();

    void update(Map* map); 
    
    void draw();
private:
    // variable which increases when the WV is in motion
    int activated;
    // texture of hurt WV
    SDL_Texture* hurt_texture;
    // texture of cured WV
    SDL_Texture* cured_texture;
};