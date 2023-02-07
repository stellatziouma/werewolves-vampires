#pragma once

#include <iostream>
#include "sdl2/SDL.h"
#include "sdl2/SDL_image.h"
#include "sdl2/SDL_ttf.h"
#include <vector>

using std::vector;

class Map;
class Avatar;
class Creature;

// main class initializing the game
class Game {
public:
    Game(const char* title, int w, int h, int rows, int cols);
    ~Game();

    void run();
private:
    void update();
    void draw();
    void handle_events();
    bool game_over() const;
    void pause();
    void init_numbers_text();

    ///////////// Graphics /////////////
    SDL_Window* window;
    SDL_Renderer* renderer;

    bool is_game_running;

    // current keyboard-input event
    SDL_Event event;

    // game's map
    Map* map;

    // game's avatar/player
    Avatar* avatar;

    // vector of Creatures (Werewolves and Vampires)
    vector<Creature*> creatures;

    // window's dimetions
    int screen_width, screen_height;

    // number of creatures alive and potions avatar holds
    int werewolves, vampires;
    int* potions;
    
    ////////////// stats-related graphics//////////////

    // images and positions of printed stats-messages
    SDL_Texture *w_text, *v_text, *p_text;
    SDL_Rect destw_text, destv_text, destp_text;

    // stores numbers used in stats 0-... (in text form)
    vector<SDL_Texture*> numbers;
    // dest of each number (ex. number rendered under "Werewolves")
    SDL_Rect destw_num, destv_num, destp_num;
    
    // paused-game image and its window-position
    SDL_Texture* paused;
    SDL_Rect pause_dest;
};