#include "headers/Textures.hpp"

// init correct textures based on if day or night
Textures::Textures(SDL_Renderer* ren, bool is_day) {
    renderer = ren;

    if (is_day) {
        background = load_texture("res/day.png");
        tree = load_texture("res/day_tree.png");
        lake = load_texture("res/lake_day.png");
    }
    else {
        background = load_texture("res/night.png");
        tree = load_texture("res/night_tree.png");
        lake = load_texture("res/lake_night.png");
    }
}

Textures::~Textures() {
    // destroy textures
    SDL_DestroyTexture(background);
    SDL_DestroyTexture(tree);
    SDL_DestroyTexture(lake);
}

// loads an image and returns its texture
SDL_Texture* Textures::load_texture(const char* file_name) {
    SDL_Surface* image = IMG_Load(file_name);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);

    return tex;
}