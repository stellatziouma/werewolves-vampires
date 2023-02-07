#include <iostream>
#include <cstdlib>
#include "headers/vampire.hpp"
#include "headers/map.hpp"

Vampire::Vampire(SDL_Renderer* render, Map* map, vector<Creature*>* creatures, int scale) : Creature(render, map, creatures, scale) {
    type = "Vampire";

    //init random strength, defense, health and cure
    strength = rand() % 3 + 1;
    defense = rand() % 2 + 1;
    health = 10;
    cure = rand() % 3;


    ///////////Craphics///////////
    //image's loading
    SDL_Surface* image = IMG_Load("res/vampire.png");
    //init croped image dimentions
    resized.h = image->h / 4;
    resized.w = image->w / 3;
    texture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);

    //init vampire-hurt texture
    image = IMG_Load("res/hurt_vampire.png");
    hurt_texture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);

    //init vampire-healed texture
    image = IMG_Load("res/healed_vampire.png");
    cured_texture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);

    resized.x = resized.y = 0;

    //init as 0(since vampire is not in motion)
    activated = 0;

    // init as 0(since the vampire is not hurt)
    is_hurt = 0;
    // init as 0(since the vampire is not cured)
    is_cured = 0;
}

Vampire::~Vampire() {
    SDL_DestroyTexture(hurt_texture);
    SDL_DestroyTexture(cured_texture);
}

void Vampire::update(Map* map) {
    if (activated > 0 && activated < dest.h + 1) {
        // increase vampire coordinates by speed = 1
        dest.x += velocity->x * velocity->speed;
        dest.y += velocity->y * velocity->speed;
        // if current move isn't stop change the image of animation so it will appear to actually move
        if (curr_move != Move::STOP)
            resized.x = resized.w * static_cast<int>((SDL_GetTicks64() / animation->speed) % animation->frames);// perform movement's animation
        else
            resized.x = resized.w;
        // increase the variable activated according to the speed/ so the vampire will move per square 
        activated += velocity->speed;
    }
    else {
        //init activated since vampire will start a new move
        activated = 1;
        velocity->y = velocity->x = 0;
        // check for interactions
        Move enemy = check_interaction();
        //check if vampire is dead
        if (get_health() <= 0)
            return;
        // generate the next move
        move(map, enemy);
        // update velocity's speed, x and y according to new move
        update_velocity();
        //change the row of the image vampire.png so the vampire will look in different directions as it moves
        if (curr_move == Move::LEFT)
            resized.y = 3 * resized.h;
        else if (curr_move == Move::RIGHT)
            resized.y = resized.h;
        else if (curr_move == Move::UP || curr_move == Move::UL || curr_move == Move::UR)
            resized.y = 0;
        else if (curr_move == Move::DOWN || curr_move == Move::DL || curr_move == Move::DR)
            resized.y = 2 * resized.h;
    }
}

void Vampire::draw() { 
    // draw the image of the injured vampire for 15 game loops(number 10 was decided based on tests to ensure the best visual result)
    if (is_hurt > 0 && is_hurt <= 15) {
        SDL_RenderCopy(renderer, hurt_texture, &resized, &dest);
        is_hurt++;
    }
    else if (is_cured > 0 && is_cured <= 15) {
        SDL_RenderCopy(renderer, cured_texture, &resized, &dest);
        is_cured++;
    }
    // draw normal image
    else {
        is_hurt = 0;
        is_cured = 0;
        SDL_RenderCopy(renderer, texture, &resized, &dest);
    }
}