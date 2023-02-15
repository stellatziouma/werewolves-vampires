#include "headers/game_entity.hpp"
#include "headers/map.hpp"

GameEntity::GameEntity(SDL_Renderer* r, Map* map, vector<GameEntity*>* c, int scale) : creatures(c), renderer(r) {
    // find random position for the creature
    int row, col;
    static bool** place_taken = map->get_place_taken();
    // find coordinates where there is no tree or lake or other creature
    do {
        row = rand() % map->get_rows();
        col = rand() % map->get_cols();
    }
    while(place_taken[row][col]);

    curr_move = Move::STOP; 

    // init animation
    animation = new Animation;
    animation->frames = 3;
    animation->scale = scale;
    animation->speed = 180;

    // init movement
    velocity = new Velocity;
    velocity->speed = 1;
    velocity->x = velocity->y = 0;

    // init Creature's coordinates and dimentions
    // scale is the width and height of a tile in the map
    dest.h = dest.w = scale;
    dest.x = col * scale;
    dest.y = row * scale; 
    resized.x = resized.y = 0;
}

GameEntity::~GameEntity() {
    delete velocity;
    delete animation;
    SDL_DestroyTexture(texture);
}

//set new health and start cured animation if need be
void GameEntity::set_health(int new_health) { 
    is_cured = new_health > health ? 1 : is_cured;
    health = new_health; 
}

// if there is an interaction check if it is with an ally or an enemy and act accordingly
void GameEntity::interaction(GameEntity* creature) {
    if (is_ally(creature)) {
        // if interaction's creature health isn't maximum 
        if (creature->get_health() < 10) {
            // choose randomly if it will help the ally
            if (rand() % 2 && cure > 0) {
                cure--;
                creature->set_health(creature->get_health() + 1);
            }
        }
    }
    else if (strength <= creature->get_strength()) {
        health -= abs((creature->get_strength() - defense));
        is_hurt = 1; 
    }
}

//check if there is an interaction and if it is an enemy return the move that will lead to it else return stop
Move GameEntity::check_interaction() {
    int col = get_col();
    int row = get_row();
    // for evey creature check if it is next to it
    for (int i = 0 ; i < (int)creatures->size(); i++) {
        int creature_col = (*creatures)[i]->get_col();
        int creature_row = (*creatures)[i]->get_row();
        if (row == creature_row && col - 1 == creature_col) {
            interaction((*creatures)[i]);
            if (is_hurt)
                return Move::LEFT;
        }
        else if (row == creature_row && col + 1 == creature_col) {
            interaction((*creatures)[i]);
            if (is_hurt)
                return Move::RIGHT;
        }
        else if (row - 1 == creature_row && col == creature_col) {
            interaction((*creatures)[i]);
            if (is_hurt)
                return Move::UP;
        }
        else if (row + 1 == creature_row && col == creature_col) {
            interaction((*creatures)[i]);
            if (is_hurt)
                return Move::DOWN;
        }
    }
    return Move::STOP;
}

void GameEntity::update_velocity() {
    // minus or plus 1 to axis the creature is moving(minus if it moves backward to this axis or plus if it moves forward to it)
    if (curr_move == Move::LEFT)
        velocity->x = -1;
    else if (curr_move == Move::RIGHT)
        velocity->x = 1;
    else if (curr_move == Move::UP)
        velocity->y = -1;
    else if (curr_move == Move::DOWN)
        velocity->y = 1;
    else if (curr_move == Move::STOP)
        velocity->x = velocity->y = 0;
    else {
        if (type != "Vampire")
            return;
        // vampires can also move diagonally
        if (curr_move == Move::UL) {
            velocity->x = -1;
            velocity->y = -1;
        }
        else if (curr_move == Move::UR) {
            velocity->x = 1;
            velocity->y = -1;
        }
        else if (curr_move == Move::DL) {
            velocity->x = -1;
            velocity->y = 1;
        }
        else if (curr_move == Move::DR) {
            velocity->x = 1;
            velocity->y = 1;
        }
    }
}

static Move random_move(int num) {
    // random number from 0 to num-1 according to number of moves of each creature
    int temp_move = rand() % num;

    //according to the random number return the corresponding move
    if (temp_move == 0)
        return Move::LEFT;
    else if (temp_move == 1)
        return Move::RIGHT;
    else if (temp_move == 2)
        return Move::UP;
    else if (temp_move == 3)
        return Move::DOWN;
    else if (temp_move == 4) 
        return Move::STOP;
    else if (temp_move == 5)
        return Move::UL;
    else if (temp_move == 6)
        return Move::UR;
    else if (temp_move == 7)
        return Move::DR;
    else 
        return Move::DL;
}

// find a legal move and change the place_taken array so there are no collisions on the same tile
void GameEntity::move(Map* map, Move enemy_move) {
    int num_of_moves = type == "Vampire" ? 9 : 5;
    //random valid move
    do
        curr_move = random_move(num_of_moves);
    while(!is_move_legal(map) || !walk_away(enemy_move));

    static bool** place_taken = map->get_place_taken();

    int row = get_row();
    int col = get_col();

    //change the value of current tile according to the current move(if the move isn't Stop current tile becomes false since the creature will move)
    place_taken[row][col] = curr_move == Move::STOP;

    //for every move change to true the destination tile
    if (curr_move == Move::LEFT)
       place_taken[row][col - 1] = true;
    else if (curr_move == Move::RIGHT)
        place_taken[row][col + 1] = true;
    else if (curr_move == Move::UP)
        place_taken[row - 1][col] = true;
    else if (curr_move == Move::DOWN)
        place_taken[row + 1][col] = true;
    else {
        if (type != "Vampire")
            return;
        if (curr_move == Move::UL) 
            place_taken[row - 1][col - 1] = true;
        else if (curr_move == Move::UR) 
            place_taken[row - 1][col + 1] = true;
        else if (curr_move == Move::DL) 
            place_taken[row + 1][col - 1] = true;
        else if (curr_move == Move::DR) 
            place_taken[row + 1][col + 1] = true;
    }
}

// check if the current move is legal
bool GameEntity::is_move_legal(Map* map) {
    static bool** place_taken = map->get_place_taken();
    static int map_col = map->get_cols();
    static int map_row = map->get_rows();
    
    int col = get_col();
    int row = get_row();

    if (curr_move == Move::UP && (row - 1 < 0 || place_taken[row - 1][col]))
        return false;
    else if (curr_move == Move::DOWN && (row + 1 == map_row || place_taken[row + 1][col]))
        return false;
    else if (curr_move == Move::LEFT && (col - 1 < 0 || place_taken[row][col - 1]))
        return false;
    else if (curr_move == Move::RIGHT && (col + 1 == map_col || place_taken[row][col + 1]))
        return false;
    else if (curr_move == Move::UR && (row - 1 < 0 || col + 1 == map_col || place_taken[row - 1][col + 1]))
        return false;
    else if (curr_move == Move::UL && (row - 1 < 0 || col - 1 < 0 || place_taken[row - 1][col - 1]))
        return false;
    else if (curr_move == Move::DL && (row + 1 == map_row || col - 1 < 0 || place_taken[row + 1][col - 1]))
        return false;
    else if (curr_move == Move::DR && (row + 1 == map_row || col + 1 == map_col || place_taken[row + 1][col + 1]))
        return false;
    return true;
}

// if the creature has received an attack try to escape
// enemy_move is the move that leads to the attacker
bool GameEntity::walk_away(Move enemy_move) {
    if (enemy_move == Move::STOP)
        return true;
    else {
        // return false if the current move leads to the attacker
        if (curr_move == enemy_move)
            return false;
        // if the creature is vampire check for diagonal moves
        if (get_type() == "Vampire"){
            if (curr_move == Move::UR && (enemy_move == Move::UP || enemy_move == Move::RIGHT))
                return false;
            else if (curr_move == Move::UL && (enemy_move == Move::UP || enemy_move == Move::LEFT))
                return false;
            else if (curr_move == Move::DL && (enemy_move == Move::DOWN || enemy_move == Move::LEFT))
                return false;
            else if (curr_move == Move::DR && (enemy_move == Move::DOWN || enemy_move == Move::RIGHT))
                return false;
        }
    }
    return true;
}