#include "headers/map.hpp"
#include <cstdlib>

// returns min of two elements a and b
static inline int min(int a, int b) {
    return a < b ? a : b;
}

Map::Map(int rows, int cols, int screen_width, int screen_height, SDL_Renderer* ren) : map_rows(rows), map_cols(cols), renderer(ren) {
    using namespace std;
    srand(time(NULL));
    
    // init width and height of map-tile image
    dest.h = screen_height / map_rows;
    dest.w = (screen_width - 300) / map_cols; // leave space for "stats section" (next to map)

    // init game's day and night textures objects
    day = new Textures(renderer);
    night = new Textures(renderer, false);

    // place taken array init
    // stores true cells which (currently) contain an object or creature
    place_taken = new bool* [map_rows];
    for (int i = 0; i < map_rows; i++)
        place_taken[i] = new bool [map_cols];
    for (int i = 0; i < map_rows; i++)
        for (int j = 0; j < map_cols; j++)
            place_taken[i][j] = false;

    // init game's potion texture
    SDL_Surface* image = IMG_Load("res/potion.png");
    potion = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);
    // find coordinates where there is no tree or lake for potion to be placed
    int row, col;
    do {
        row = rand() % map_rows;
        col = rand() % map_cols;
    }
    while (place_taken[row][col]);
    // mark map-place as visited
    place_taken[row][col] = true;
    // assign values; potion is half the size of a map-tile
    potion_dest.w = dest.w / 2;
    potion_dest.h = dest.h / 2;
    potion_dest.x = col * dest.w + potion_dest.w;
    potion_dest.y = row * dest.h + potion_dest.h;

    // trees in map start from 2 for 8x8 and increace by one for each dimentions' increase
    int number_of_trees = 2 + min(map_rows, map_cols) - 8;
    for (int i = 0; i < number_of_trees; i++) {
        // create a new tree destination with random coordinates in map
        SDL_Rect* tree_dest = new SDL_Rect;
        tree_dest->w = dest.w;
        tree_dest->h = dest.h;
        // don't allow trees in first and last row or col of map and chech if spot is already taken in map
        do {
            tree_dest->x = rand() % map_cols;
            tree_dest->y = rand() % map_rows;
        }
        while (tree_dest->x == 0 || tree_dest->x == map_cols - 1 || 
            tree_dest->y == 0 || tree_dest->y == map_rows - 1 ||
            place_taken[tree_dest->y][tree_dest->x]);
        // inform place taken array
        place_taken[tree_dest->y][tree_dest->x] = true;

        // actual position
        tree_dest->x *= tree_dest->w;
        tree_dest->y *= tree_dest->h;

        // add to vector
        trees_dest.push_back(tree_dest);
    }

    // lakes in map are 2x2 (compared to grass or tree being 1x1) and is one if map's size is 8x8-10x10
    // or are two if map's size is 11x11
    int number_of_lakes = 1;
    if (min(map_rows, map_cols) == 11)
        number_of_lakes++;
    for (int i = 0; i < number_of_lakes; i++) {
        // create a new lake destination with random coordinates in map
        SDL_Rect* lake_dest = new SDL_Rect;
        lake_dest->w = dest.w * 2;
        lake_dest->h = dest.h * 2;
        // don't allow lakes in last row or col of map and avoid place-collisions
        do {
            lake_dest->x = rand() % map_cols;
            lake_dest->y = rand() % map_rows;
        }
        while (lake_dest->x == 0 || lake_dest->x >= map_cols - 2 ||
            lake_dest->y == 0 || lake_dest->y >= map_rows - 2 ||
            place_taken[lake_dest->y][lake_dest->x] ||
            place_taken[lake_dest->y][lake_dest->x + 1] ||
            place_taken[lake_dest->y + 1][lake_dest->x] ||
            place_taken[lake_dest->y + 1][lake_dest->x + 1]);
        // a lake is 2x2 meaning we need to mark all 4 squares as visited
        place_taken[lake_dest->y][lake_dest->x] = true;
        place_taken[lake_dest->y][lake_dest->x + 1] = true;
        place_taken[lake_dest->y + 1][lake_dest->x] = true;
        place_taken[lake_dest->y + 1][lake_dest->x + 1] = true;

        lake_dest->x *= dest.w;
        lake_dest->y *= dest.h;

        // add to vector
        lakes_dest.push_back(lake_dest);
    }
}

Map::~Map() {
    // destroy textures
    delete day;
    delete night;
    
    // destroy tetxure
    SDL_DestroyTexture(potion);

    // free memory of used trees' destinations
    for (auto t : trees_dest)
        delete t;

    // free memory of used lakes' destinations
    for (auto l : lakes_dest)
        delete l;

    // free place taken array
    for (int i = 0; i < map_rows; i++)
        delete place_taken[i];
    delete place_taken;
}

// draws game map
void Map::draw() {
    // determine if day or night, based on a simple counter
    static int counter = 0;
    static Textures* time_is = nullptr;
    static const int textures_change = 700;
    // alternate between day and night every 700-function-calls
    if (counter <= textures_change && time_is != day) {
        time_is = day;
        is_day = true;
    }
    else if (counter > textures_change && counter <= 2 * textures_change && time_is != night) {
        time_is = night;
        is_day = false;
    }
    counter = counter > 2 * textures_change ? 0 : counter + 1;

    // draw background
    for (int i = 0; i < map_cols; i++) {
        dest.x = i * dest.w;
        for (int j = 0; j < map_rows; j++) {
            dest.y = j * dest.h;
            SDL_RenderCopy(renderer, time_is->background, NULL, &dest);
        }
    }
    // draw trees
    for (auto t : trees_dest)
        SDL_RenderCopy(renderer, time_is->tree, NULL, t);

    // draw lakes
    for (auto l : lakes_dest)
        SDL_RenderCopy(renderer, time_is->lake, NULL, l);
    // draw potion
    SDL_RenderCopy(renderer, potion, NULL, &potion_dest);
}