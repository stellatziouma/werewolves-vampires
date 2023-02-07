#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "sdl2/SDL.h"

class Map;

using std::string;
using std::vector;

// possible moves
enum class Move {
    LEFT, RIGHT, UP, DOWN, STOP, UL, UR, DR, DL
};

// stores a creature's speed and velocity-factor in each axis
typedef struct {
    int speed;
    int x, y;
} Velocity;

// Graphics-related useful for creatures' movement animation
typedef struct {
    int frames;
    int speed;
    int scale;
} Animation;

// abstract class Creature; base class for all (movable) entities
class Creature {
public:
    Creature(SDL_Renderer* render, Map* map, vector<Creature*>* creatures, int scale);
    virtual ~Creature();

    // pure virtual method making the class abstract
    virtual void update(Map* map) = 0;

    // useful setters-getters
    bool is_ally(Creature* side_creature) const { return type == side_creature->get_type(); }
    int get_strength() const { return strength; }
    void set_health(int new_health);
    int get_health() const { return health; }
    string get_type() const { return type; }
    int get_row() const { return dest.y / dest.h; }
    int get_col() const { return dest.x / dest.w; }

    ///////////// Graphics /////////////
    // basic draw function
    virtual void draw() { SDL_RenderCopy(renderer, texture, &resized, &dest); }
protected:
    void move(Map* map, Move enemy);
    virtual bool is_move_legal(Map* map);
    bool walk_away(Move enemy);
    virtual void interaction(Creature* creature);
    virtual Move check_interaction();
    void update_velocity();

    // creature's type, ex. Werewolf
    string type;
    // creature's attributes
    int health, strength, defense, cure;
    // current move made by creature
    Move curr_move;

    Velocity* velocity;

    // pointer to vector of creatures to allow easy-checking of interactions/collisions
    vector<Creature*>* creatures;

    ///////////////////// Graphics /////////////////////
    // variables determining rounds/loops in which creature is being shown green(taken a cure) or red(taken a hit)
    int is_cured, is_hurt;

    // creature's texture
    SDL_Texture* texture;
    // resized allows us to crop and show a specific part of an image
    // dest determines the position that the image is placed in game's window
    SDL_Rect resized, dest;

    Animation* animation;

    // game's renderer; needed to output a creature on screen
    SDL_Renderer* renderer;
};