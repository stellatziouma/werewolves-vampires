#include <iostream>
#include <cstdlib>
#include "headers/creature.hpp"
#include "headers/map.hpp"

Creature::Creature(SDL_Renderer* render, Map* map, vector<GameEntity*>* creatures, int scale, bool is_werewolf) : GameEntity(render, map, creatures, scale) {
    // Load creature's images
    SDL_Surface *image, *hurt, *cured;
    if (is_werewolf) {
        type = "Werewolf";
        image = IMG_Load("res/werewolf.png");
        hurt = IMG_Load("res/hurt_werewolf.png");
        cured = IMG_Load("res/healed_werewolf.png");
    }
    else {
        type = "Vampire";
        image = IMG_Load("res/vampire.png");
        hurt = IMG_Load("res/hurt_vampire.png");
        cured = IMG_Load("res/healed_vampire.png");
    }
    resized.h = image->h / 4;
    resized.w = image->w / 3;
    texture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);
    hurt_texture = SDL_CreateTextureFromSurface(renderer, hurt);
    SDL_FreeSurface(hurt);
    cured_texture = SDL_CreateTextureFromSurface(renderer, cured);
    SDL_FreeSurface(cured);

    //init random strength, defense, health and cure
    strength = rand() % 3 + 1;
    defense = rand() % 2 + 1;
    health = 10;
    cure = rand() % 3;

    resized.x = resized.y = 0;

    activated = is_hurt = is_cured = 0;
}

Creature::~Creature() {
    SDL_DestroyTexture(hurt_texture);
    SDL_DestroyTexture(cured_texture);
}

void Creature::update(Map* map) {
    if (activated > 0 && activated < dest.h + 1) { // if the Creature is in motion
        dest.x += velocity->x * velocity->speed;
        dest.y += velocity->y * velocity->speed;

        if (curr_move != Move::STOP)
            resized.x = resized.w * static_cast<int>((SDL_GetTicks64() / animation->speed) % animation->frames);// perform movement's animation
        else
            resized.x = resized.w;
        //increase the variable activated according to the speed so the Creature will move per square 
        activated += velocity->speed;
    }
    else {
        //init activated since Creature will start a new move
        activated = 1;
        velocity->y = velocity->x = 0;
        Move enemy = check_interaction();
        if (get_health() <= 0)
            return;
        move(map, enemy);
        update_velocity();
        //change the row of the image Creature.png so the wolf will look in different directions as it moves
        if (curr_move == Move::LEFT)
            resized.y = 3 * resized.h;
        else if (curr_move == Move::RIGHT)
            resized.y = resized.h;
        else if (curr_move == Move::UP)
            resized.y = 0;
        else if (curr_move == Move::DOWN)
            resized.y = 2 * resized.h;
    }
}

void Creature::draw() { 
    //draw the image of the injured Creature for 15 game loops
    if (is_hurt > 0 && is_hurt <= 15) {
        SDL_RenderCopy(renderer, hurt_texture, &resized, &dest);
        is_hurt++;
    }
    else if (is_cured > 0 && is_cured <= 15) {
        SDL_RenderCopy(renderer, cured_texture, &resized, &dest);
        is_cured++;
    }
    //draw normal image
    else {
        is_hurt = 0;
        is_cured = 0;
        SDL_RenderCopy(renderer, texture, &resized, &dest);
    }
}