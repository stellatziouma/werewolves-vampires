# executable's name
EXEC = GAME

# paths
INCLUDE = -I./include -I./include/sdl2 -I./include/headers
SRC = ./src

# compiler
CXX = g++
# compiler options
CXXFLAGS = -Wall -Werror $(INCLUDE)

# compile executable
$(EXEC):
	$(CXX) $(CXXFLAGS) -Llib -o GAME $(SRC)/*.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf

# compile terminal command on Windows:
# g++ -Wall -Werror -I./include -I./include/sdl2 -I./include/headers -Llib -o GAME ./src/*.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf

# run the program
run: $(EXEC)
	./$(EXEC)

.PHONY: run