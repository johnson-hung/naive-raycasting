PROGRAM = game
CXXFLAGS += -std=c++17 -Wall -Werror -g
INCLUDES = -I/opt/homebrew/include/SDL2 -D_THREAD_SAFE
LINKS = -L/opt/homebrew/lib
LINK_FLAGS = -lSDL2 -lSDL2_ttf
MKDIR_P = mkdir -p
OUT_DIR = ./build/

all: $(PROGRAM)

PHONY:
	run

directory:
	$(MKDIR_P) $(OUT_DIR)

run:
	make
	$(OUT_DIR)$(PROGRAM)

clean:
	rm -f $(OUT_DIR)*.o
	rm -f $(OUT_DIR)$(PROGRAM)

#================
SRC_FILES = game.cpp utils.cpp canvas.cpp map.cpp texture.cpp player.cpp
OBJ_FILES = game.o utils.o canvas.o map.o texture.o player.o
#================

$(PROGRAM): directory $(OBJ_FILES)
	g++ -o ${OUT_DIR}$(PROGRAM) ${addprefix $(OUT_DIR), $(OBJ_FILES)} $(LINKS) $(LINK_FLAGS)

game.o: game.cpp game.h
	g++ $(CXXFLAGS) -c -o ${OUT_DIR}game.o $(INCLUDES) game.cpp

utils.o: utils.cpp utils.h
	g++ $(CXXFLAGS) -c -o ${OUT_DIR}utils.o utils.cpp

canvas.o: canvas.cpp canvas.h
	g++ $(CXXFLAGS) -c -o ${OUT_DIR}canvas.o canvas.cpp

map.o: map.cpp map.h
	g++ $(CXXFLAGS) -c -o ${OUT_DIR}map.o map.cpp

texture.o: texture.cpp texture.h
	g++ $(CXXFLAGS) -c -o ${OUT_DIR}texture.o $(INCLUDES) texture.cpp

player.o: player.cpp player.h
	g++ $(CXXFLAGS) -c -o ${OUT_DIR}player.o player.cpp