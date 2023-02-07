#include "headers/game.hpp"
#include "headers/map.hpp"
#include "headers/avatar.hpp"
#include "headers/werewolf.hpp"
#include "headers/vampire.hpp"
#include <string>

Game::Game(const char* title, int w, int h, int map_rows, int map_cols) : screen_width(w), screen_height(h) {
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();

    // init window and renderer
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    is_game_running = true;

    // init map
    map = new Map(map_rows, map_cols, screen_width, screen_height, renderer);

    // number of each creature placed in game map
    srand(time(NULL));
    int num_of_creatures = werewolves = vampires = map_rows * map_cols / 15;
    for (int i = 0; i < num_of_creatures; i++) {
        creatures.push_back(new Werewolf(renderer, map, &creatures, screen_height / map_rows));
        creatures.push_back(new Vampire(renderer, map, &creatures, screen_height / map_rows));
    }

    // init game's avatar
    avatar = new Avatar(renderer, map, &creatures, screen_height / map_rows);

    // avatar's potions
    potions = avatar->get_potions();

    ///////////// Graphics /////////////
    // init stats category images
    SDL_Surface* image = IMG_Load("res/werewolves_stats.png");
    w_text = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);
    image = IMG_Load("res/vampires_stats.png");
    v_text = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);
    image = IMG_Load("res/potions_stats.png");
    p_text = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);
    
    // Calculations of stats' positions were chosen based on tests
    // to provide the best visual experience
    destw_text.x = screen_width - 300;
    destv_text.x = destw_text.x + 25;
    destp_text.x = destv_text.x + 22;

    destw_text.y = 20;
    destv_text.y = screen_height / 2 - 95;
    destp_text.y = screen_height - (destv_text.y - destw_text.y);

    destw_text.h = destv_text.h = destp_text.h = 70;
    destw_text.w = destv_text.w = destp_text.w = 300;

    // init stats' numbers
    init_numbers_text();

    // init paused image
    image = IMG_Load("res/pause.png");
    paused = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);
    pause_dest.w = pause_dest.h = 200;
    pause_dest.x = screen_width / 2 - 250;
    pause_dest.y = screen_height / 2 - 125;
}

Game::~Game() {
    // destroy game's map
    delete map;
    // delete avatar
    delete avatar;
    // delete creatures
    for (auto c : creatures)
        delete c;
    creatures.clear();
    // destroy stats' - all
    SDL_DestroyTexture(w_text);
    SDL_DestroyTexture(v_text);
    SDL_DestroyTexture(p_text);
    for (int i = 0; i <= werewolves; i++)
        SDL_DestroyTexture(numbers[i]);
    numbers.clear();
    // destroy paused image
    SDL_DestroyTexture(paused);
    // destroy window
    TTF_Quit();
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}

void Game::update() {
    // determine if a team won
    if (game_over()) {
        is_game_running = false;
        return;
    }

    // update avatar
    avatar->update(map);

    // maps visited array
    static bool** place_taken = map->get_place_taken();
    // delete objects if need be
    for (int i = 0; i < (int)creatures.size(); i++) {
        if (creatures[i]->get_health() <= 0) {
            // determine type of creature killed
            creatures[i]->get_type() == "Werewolf" ? werewolves-- : vampires--;
            // make creature's old-place false in array
            place_taken[creatures[i]->get_row()][creatures[i]->get_col()] = false;
            // delete it and remove vector node
            delete creatures[i];
            creatures.erase(creatures.begin() + i);
        }
    }
    // update alive creatures
    for (auto c : creatures)
        if (c->get_health() > 0) // make sure creature is alive
            c->update(map);
}

void Game::run() {
    // frame delay for better and constant graphical perfomance
    // source: https://www.youtube.com/watch?v=jzasDqPmtPI&list=PLhfAbcv9cehhkG7ZQK0nfIGJC_C-wSLrx&index=6&ab_channel=Let%27sMakeGames
    const int FPS = 60;
    const int frame_delay = 1000 / FPS;
    // main game loop
    while (is_game_running) {
        Uint32 frame_start = SDL_GetTicks64();

        handle_events();
        update();
        draw();

        int frame_time = SDL_GetTicks64() - frame_start;
        if (frame_delay > frame_time)
            SDL_Delay(frame_delay - frame_time);
    }
}

// handle's user's input
void Game::handle_events() {
    SDL_PollEvent(&event);
    if (event.type == SDL_QUIT) // quit game
        is_game_running = false;
    else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_p) // pause game
        pause();
    else // handle avatar-input
       avatar->handle_events(event, map->is_day_time());
}

// pauses game
void Game::pause() {
    /* pause game by pressing the h key and resume in it
       by pressing and realising h kei again */
    int counter = 1;
    SDL_RenderCopy(renderer, paused, NULL, &pause_dest);
    SDL_RenderPresent(renderer);
    while (counter < 4) {
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT) { // quit game
            is_game_running = false;
            return;
        }
        if (counter % 2) {
            if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_p)
                counter++;
        }
        else {
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_p)
                counter++;
        }
    }
}

void Game::draw() {
    // begin drawing
    SDL_RenderClear(renderer);

    // draw map
    map->draw();
    // draw avatar
    avatar->draw();
    // draw creatures
    for (auto c : creatures)
        c->draw();
    // draw stats section
    SDL_RenderCopy(renderer, w_text, NULL, &destw_text);
    SDL_RenderCopy(renderer, v_text, NULL, &destv_text);
    SDL_RenderCopy(renderer, p_text, NULL, &destp_text);
    SDL_RenderCopy(renderer, numbers[werewolves], NULL, &destw_num);
    SDL_RenderCopy(renderer, numbers[vampires], NULL, &destv_num);
    SDL_RenderCopy(renderer, numbers[*potions], NULL, &destp_num);

    // end drawing
    SDL_RenderPresent(renderer);
}

// determines if game is over aka if a team won
bool Game::game_over() const {
    if (!vampires) {
        std::cout << "Werewolves Win !!!" << std::endl;
        return true;
    }
    if (!werewolves) {
        std::cout << "Vampires Win !!!" << std::endl;
        return true;
    }
    return false;
}

// initializes numbers and stores them in a vector (as text) to be shown in stats category
void Game::init_numbers_text() {
    // used number-font and color
    TTF_Font* font = TTF_OpenFont("res/akashi.ttf", 24);
    SDL_Color white = {255, 255, 255};
    // number of each type of creature is already initialized in werewolves and vampires vars
    int num_of_creatures = werewolves;
    for (int i = 0; i <= num_of_creatures; i++)
        numbers.push_back(SDL_CreateTextureFromSurface(renderer, TTF_RenderText_Solid(font, std::to_string(i).c_str(), white)));

    // next calculations of where numbers are rendered on screen
    // were created based on tests to ensure the best visual experience
    destw_num.x = destp_text.x + destp_text.w / 2 - 75;
    destw_num.y = destw_text.y + destw_text.h + 10;
    destw_num.h = 90;
    destw_num.w = 50;

    destv_num.x = destp_text.x + destp_text.w / 2 - 75;
    destv_num.y = destv_text.y + destv_text.h + 10;
    destv_num.h = 90;
    destv_num.w = 50;

    destp_num.x = destp_text.x + destp_text.w / 2 - 75;
    destp_num.y = destp_text.y + destp_text.h + 10;
    destp_num.h = 90;
    destp_num.w = 50;
}