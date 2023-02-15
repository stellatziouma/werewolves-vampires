#pragma once

#include "sdl2/SDL.h"
#include "sdl2/SDL_image.h"

// a class combining needed textures of the game
// based on if day or night
class Textures {
public:
    Textures(SDL_Renderer* ren, bool is_day = true);
    ~Textures();
    // loads a texture to the game
    SDL_Texture* load_texture(const char* file_name);
    // textures of game for each time period (day/night)
    SDL_Texture *background, *tree, *lake;
private:
    SDL_Renderer* renderer;
};