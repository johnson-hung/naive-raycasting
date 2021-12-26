PROGRAM = game
INCLUDES = -I/opt/homebrew/include/SDL2 -D_THREAD_SAFE
LINKS = -L/opt/homebrew/lib
LINK_FLAGS = -lSDL2
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
OBJ_FILES = utils.o canvas.o map.o texture.o player.o
#================

$(PROGRAM): directory $(OBJ_FILES)
	g++ -o ${OUT_DIR}$(PROGRAM) ${addprefix $(OUT_DIR), $(OBJ_FILES)} $(INCLUDES) game.cpp $(LINKS) $(LINK_FLAGS)

utils.o: utils.cpp utils.h
	g++ -o ${OUT_DIR}utils.o -c utils.cpp

canvas.o: canvas.cpp canvas.h
	g++ -o ${OUT_DIR}canvas.o -c canvas.cpp

map.o: map.cpp map.h
	g++ -o ${OUT_DIR}map.o -c map.cpp

texture.o: texture.cpp texture.h
	g++ -o ${OUT_DIR}texture.o -c texture.cpp

player.o: player.cpp player.h
	g++ -o ${OUT_DIR}player.o -c player.cpp