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
SRC_FILES = main.cpp game.cpp utils.cpp canvas.cpp map.cpp texture.cpp player.cpp sprite.cpp render.cpp \
			state_waiting.cpp state_running.cpp state_terminate.cpp
OBJ_FILES = main.o game.o utils.o canvas.o map.o texture.o player.o sprite.o render.o \
			state_waiting.o state_running.o state_terminate.o
#================

$(PROGRAM): directory $(OBJ_FILES)
	g++ -o ${OUT_DIR}$(PROGRAM) ${addprefix $(OUT_DIR), $(OBJ_FILES)} $(LINKS) $(LINK_FLAGS)

main.o: main.cpp
	g++ $(CXXFLAGS) -c -o ${OUT_DIR}main.o $(INCLUDES) main.cpp

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

sprite.o: sprite.cpp sprite.h
	g++ $(CXXFLAGS) -c -o ${OUT_DIR}sprite.o sprite.cpp

render.o: render.cpp render.h
	g++ $(CXXFLAGS) -c -o ${OUT_DIR}render.o render.cpp

state_waiting.o: state_waiting.cpp state_waiting.h
	g++ $(CXXFLAGS) -c -o ${OUT_DIR}state_waiting.o $(INCLUDES) state_waiting.cpp

state_running.o: state_running.cpp state_running.h
	g++ $(CXXFLAGS) -c -o ${OUT_DIR}state_running.o $(INCLUDES) state_running.cpp

state_terminate.o: state_terminate.cpp state_terminate.h
	g++ $(CXXFLAGS) -c -o ${OUT_DIR}state_terminate.o $(INCLUDES) state_terminate.cpp