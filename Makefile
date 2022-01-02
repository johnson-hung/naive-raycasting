PROGRAM = game

CXXFLAGS += -std=c++17 -Wall -Werror -g
SDL2_LIBS = $(shell sdl2-config --libs) -lSDL2_ttf
SDL2_CFLAGS = $(shell sdl2-config --cflags)
INCLUDES = -I$(INC_DIR) $(SDL2_CFLAGS)
MKDIR_P = mkdir -p

ROOT_DIR = $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
OBJ_DIR = $(ROOT_DIR)/obj
SRC_DIR = $(ROOT_DIR)/src
INC_DIR = $(ROOT_DIR)/include

SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_FILES))
#================

all: $(PROGRAM)

PHONY:
	run

directory:
	$(MKDIR_P) $(OBJ_DIR)

run:
	make
	$(ROOT_DIR)/$(PROGRAM)

clean:
	rm -f $(OBJ_DIR)/*.o
	rm -f $(PROGRAM)
#================

$(PROGRAM): directory $(OBJ_FILES)
	g++ -o $(ROOT_DIR)/$(PROGRAM) $(OBJ_FILES) $(SDL2_LIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	g++ $(CXXFLAGS) $(INCLUDES) -c -o $@ $<