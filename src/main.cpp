#include "headers/game.hpp"

int main(int argc, char* argv[]) {
    using namespace std;
    // assure map's dimentions are within determined range (8x8 - 11x11)
    int map_rows, map_cols;
    try {
        cout << "Give game's map dimentions:" << endl;

        cin >> map_rows;
        if (map_rows < 8 || map_rows > 11)
            throw 1;
        
        cin >> map_cols;
        if (map_cols < 8 || map_cols > 11)
            throw 2;

        // check if numbers have a max substraction-value of 1
        if (abs(map_rows - map_cols) > 1)
            throw 3;
    }
    catch (int exception) {
        switch (exception)
        {
        case 1:
            cout << "[ERROR] map's rows should be between 8 and 11.";
            break;
        case 2:
            cout << "[ERROR] map's cols should be between 8 and 11.";
            break;
        case 3:
            cout << "[ERROR] map's rows and cols must have at most a difference of 1.";
            break;
        }
        return 0;
    }

    // screen's width and height
    // adjusted based on given map-dimentions given by user
    int width = 960, height = 660;
    if (map_rows < map_cols)
        width += 960 / map_cols;
    else if (map_cols < map_rows)
        height += 660 / map_rows;

    Game* game = new Game("Werewolves VS Vampires", width, height, map_rows, map_cols);
    game->run();
    delete game;

    return 0;
}