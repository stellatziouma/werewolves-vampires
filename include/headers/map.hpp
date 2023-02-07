#pragma once

#include <iostream>
#include <vector>
#include "sdl2/SDL.h"
#include "sdl2/SDL_image.h"
#include "headers/Textures.hpp"

using std::vector;

// class representing game's active map
class Map {
public:
    Map(int rows, int cols, int screen_width, int screen_height, SDL_Renderer* renderer);
    ~Map();

    void draw();

    bool** get_place_taken() const { return place_taken; }
    int get_rows() const { return map_rows; }
    int get_cols() const { return map_cols; }
    bool is_day_time() const { return is_day; }
    SDL_Rect& get_potion_pos() { return potion_dest; }
private:
    // world's/map's dimentions; given by user
    int map_rows, map_cols;

    // bool array representing taken spots in map by trees, lakes and all Creatures besides Avatar
    bool** place_taken;

    // determines if is day or night
    bool is_day;

    //////////////// Graphics ////////////////

    // day and night packs of tetxures
    Textures *day, *night;
    // game's potion texture
    SDL_Texture* potion;

    // actual destination-coordinates of a map's tile in window
    SDL_Rect dest;
    // stores the (random) destinations of trees in the map
    vector<SDL_Rect*> trees_dest;
    // stores the (random) destinations of lakes in the map
    vector<SDL_Rect*> lakes_dest;
    // stores random potion position
    SDL_Rect potion_dest;

    // game's renderer
    SDL_Renderer* renderer;
};