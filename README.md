<h1 align="center"><i>Werewolves VS Vampires</i></h1>

<p align="center">
    <b><i>Which team will survive? How will the Avatar impact the final result?</i></b><br><br>
    <img src="res/game.gif" width=700>
</p>

A fun and simple game where a team of werewolves fights against a team of vampires. The last team standing wins. As the Avatar (player), you have the ability to aid the team of your choice by using magical potions that increase each creature's health to the maximum amount.

## Authors
- [Stella Tziouma](https://github.com/stellatziouma)
- [Giorgos Sofronas](https://github.com/gsofron)

## About
This project was created entirely in C++ for the "Object-Oriented Programming" course during our 3rd semester of undergraduate studies (Winter 2022). It received a grade of 9,4/10. A detailed description of the assignment can be found in the root of the repository in Greek.

While the assignment didn't require a GUI, as it suggested using CMD or PS for interaction, we thought it would be more enjoyable to implement a visual UI. Not only would it enhance the gaming experience, but it would also give us the opportunity to explore a UI library in C++ and expand our skills in the process.

## How to Play
To play the game, simply open Command Prompt or Windows PowerShell and execute the following commands:
```cmd
  git clone https://github.com/stellatziouma/werewolves-vampires.git
  cd werewolves-vampires
```
After that, if you have `make` installed execute:
```cmd
  make run
```
Otherwise, execute:
```cmd
  g++ -Wall -Werror -g -O2 -I./include -I./include/sdl2 -I./include/headers -Llib -o GAME ./src/*.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf
  GAME.exe
```
**Please note that this game is designed solely for the Windows operating system.**

## Game Rules
Here are some basic rules to help you play the game:
- Use the *arrow* keys to move the player around the map.
- Use the *h* key to heal the team in favor. Note that werewolves can only be healed at night and vampires at day.
- Use the *p* key to pause/unpause the game.

Have fun playing and good luck!

## Credits
For the UI we used the open source library [SDL](https://www.libsdl.org/).<br>
The following list contains links to all used sprites.
- [werewolf](https://opengameart.org/content/werewolf-lpc)
- [vampire](https://opengameart.org/content/vampires-rework)
- [avatar](https://opengameart.org/content/universal-lpc-sprite-male-01)
- [tree](https://opengameart.org/content/pine-tree-tiles)
- [grass](https://opengameart.org/content/2d-modified-dark-forest-tileset)
- [potion](https://opengameart.org/content/potion-bottles)
- [lake](https://craftpix.net/freebies/free-top-down-military-boats-pixel-art/)

---
 © Stella Tziouma & Giorgos Sofronas
