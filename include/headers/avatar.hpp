#pragma once

#include "sdl2/SDL.h"
#include "sdl2/SDL_image.h"
#include "headers/game_entity.hpp"

class Avatar : public GameEntity {
public:
    Avatar(SDL_Renderer* renderer, Map* map, vector<GameEntity*>* creatures, int scale);
    void update(Map* map);
    void handle_events(SDL_Event event, bool is_day);
    int* get_potions() { return &potions; }
protected:
    bool is_move_legal(Map* map);
private:
    // number of magic potions in avatar's hands
    int potions;
    // team in favor / supporting
    string team;
    // restores health of given team-creatures when a magic potion is used
    void restore_health(bool is_day);
    // checks for potion grab
    bool potion_grab(Map* map, float row, float col);
};