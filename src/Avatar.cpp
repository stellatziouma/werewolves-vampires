#include "headers/avatar.hpp"
#include "headers/map.hpp"

Avatar::Avatar(SDL_Renderer* ren, Map* map, vector<GameEntity*>* creatures, int scale) : GameEntity(ren, map, creatures, scale) {
    type = "Avatar";

    // init player holding one potion
    potions = 1;

    do {
        std::cout << "Press W to support Werewolves\nPress V to support Vampires" << std::endl;
        std::cin >> team;
    }
    while (team != "V" && team != "W" && team != "v" && team != "w");
        team = team == "v" || team == "V" ? "Vampire" : "Werewolf";

    // load avatar's image
    SDL_Surface* image = IMG_Load("res/avatar.png");
    resized.h = image->h / 4;
    resized.w = image->w / 3;
    texture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);

    // avatar moves faster compared to other creatures
    velocity->speed++;

    // init avatar in map
    dest.x = dest.y = resized.x = 0;
    resized.y = 2 * resized.h;
}

// returns closest int of a float number
static inline int closer_int(float f) {
    return (int)(f + 0.5);
}

// returns true if avatar's current move is legal
bool Avatar::is_move_legal(Map* map) {
    // needed for collision-checking
    static bool** place_taken = map->get_place_taken();
    // number of map columns and rows
    static int col = map->get_cols() - 1;
    static int row = map->get_rows() - 1;

    // avatar's exact current map position
    float row_pos = (float)dest.y / dest.h;
    float col_pos = (float)dest.x / dest.w;

    // check if potion is captured
    if (potion_grab(map, row_pos, col_pos))
        return true;

    // out of bounds or collision with objects; illegal move
    // operations here are picked based on tests and the best visual presentation of avoiding a collision
    if (curr_move == Move::UP && (row_pos <= 0.0 || place_taken[closer_int(row_pos)][closer_int(col_pos)]))
        return false;
    else if (curr_move == Move::DOWN && (row_pos >= row || place_taken[closer_int(row_pos + 0.5)][closer_int(col_pos)]))
        return false;
    else if (curr_move == Move::LEFT && (col_pos <= 0.0 || place_taken[closer_int(row_pos + 0.4)][closer_int(col_pos - 0.25)]))
        return false;
    else if (curr_move == Move::RIGHT && (col_pos >= col || place_taken[closer_int(row_pos + 0.4)][closer_int(col_pos + 0.25)]))
        return false;

    return true;
}

// checks if avatar grabbed map's potion
bool Avatar::potion_grab(Map* map, float row, float col) {
    // potion's position
    static SDL_Rect& potion_pos = map->get_potion_pos();
    static const int potion_row = potion_pos.y / dest.h;
    static const int potion_col = potion_pos.x / dest.w;

    // check if potion was previously grabbed
    if (!potion_pos.w || !potion_pos.h)
        return false;

    // if avatar doesn't collide with potion
    if (curr_move == Move::UP && (closer_int(row) != potion_row || closer_int(col) != potion_col))
        return false;
    else if (curr_move == Move::DOWN && (closer_int(row + 0.5) != potion_row || closer_int(col) != potion_col))
        return false;
    else if (curr_move == Move::LEFT && (closer_int(row + 0.4) != potion_row || closer_int(col - 0.25) != potion_col))
        return false;
    else if (curr_move == Move::RIGHT && (closer_int(row + 0.4) != potion_row || closer_int(col + 0.25) != potion_col))
        return false;

    // take potion
    bool** place_taken = map->get_place_taken();
    place_taken[potion_row][potion_col] = false;
    potion_pos.w = potion_pos.h = 0;
    // increase potions avatar has
    potions++;
    return true;
}

// updates avatar's position and animation
void Avatar::update(Map* map) {
    if (curr_move == Move::STOP) {
        // stop animation, keep it stable
        resized.x = resized.w;
        return;
    }
    else {
        // perform animation
        resized.x = resized.w * static_cast<int>((SDL_GetTicks64() / animation->speed) % animation->frames);
        // check for move applicabillity
        if (!is_move_legal(map))
            return;
    }
    // (possible) change of position of avatar in X or Y axis
    dest.x += velocity->x * velocity->speed;
    dest.y += velocity->y * velocity->speed;
}

// handles user's keyboard input
void Avatar::handle_events(SDL_Event event, bool is_day) {
    // determines if a key is currently down/pressed
    static bool is_key_pressed = false;

    if (event.type == SDL_KEYDOWN) {
        // if another (arrow or h) key is being pressed
        // don't allow another one to be pressed at the same time
        if (is_key_pressed)
            return;
        switch (event.key.keysym.sym)
        {
        // resized.y is modified because of change of animation/change direction avatar points
        case SDLK_UP:
            curr_move = Move::UP;
            is_key_pressed = true;
            resized.y = 0;
            break;
        case SDLK_DOWN:
            curr_move = Move::DOWN;
            is_key_pressed = true;
            resized.y = 2 * resized.h;
            break;
        case SDLK_LEFT:
            curr_move = Move::LEFT;
            is_key_pressed = true;
            resized.y = 3 * resized.h;
            break;
        case SDLK_RIGHT:
            curr_move = Move::RIGHT;
            is_key_pressed = true;
            resized.y = resized.h;
            break;
        case SDLK_h:
            is_key_pressed = true;
            if (!potions) // no potions left
                return;
            // set health of fond team to maximum
            restore_health(is_day);
            break;
        default:
            return;
        }
        // update velocity-factors based on made move
        update_velocity();
    }
    else if (event.type == SDL_KEYUP) {
        // if user releases the pressed key
        SDL_Keycode key = event.key.keysym.sym;
        if (key == SDLK_UP || key == SDLK_DOWN || key == SDLK_LEFT || key == SDLK_RIGHT || key == SDLK_h) {
            curr_move = Move::STOP;
            is_key_pressed = false;
            update_velocity();
        }
    }
}

// restores health of team-in-favor to maximum amount
void Avatar::restore_health(bool is_day) {
    if (team == "Vampire" && !is_day)
        return;
    else if (team == "Werewolf" && is_day)
        return;

    for (auto c : *creatures)
        if (team == c->get_type())
            c->set_health(10);
    potions--;
}