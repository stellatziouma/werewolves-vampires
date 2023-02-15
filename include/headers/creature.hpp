#pragma once

#include "sdl2/SDL_image.h"
#include "headers/game_entity.hpp"

// this class refers to werewolves and vampires
class Creature : public GameEntity {
public:
    Creature(SDL_Renderer* renderer, Map* map, vector<GameEntity*>* creatures, int scale, bool w);
    ~Creature();
    void update(Map* map); 
    void draw();
private:
    // variable which increases when the Creature is in motion
    int activated;
    // texture of hurt and cured Creature
    SDL_Texture *hurt_texture, *cured_texture;
};