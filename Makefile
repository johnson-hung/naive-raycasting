PROGRAM = game
ROOT_DIR = $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
OBJ_DIR = $(ROOT_DIR)/obj
SRC_DIR = $(ROOT_DIR)/src
INC_DIR = $(ROOT_DIR)/include

CXXFLAGS += -std=c++17 -Wall -Werror -g
INCLUDES = -I$(INC_DIR) -I/opt/homebrew/include/SDL2 -D_THREAD_SAFE
LINKS = -L/opt/homebrew/lib
LINK_FLAGS = -lSDL2 -lSDL2_ttf
MKDIR_P = mkdir -p

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
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_FILES))
#================

$(PROGRAM): directory $(OBJ_FILES)
	g++ -o $(ROOT_DIR)/$(PROGRAM) $(OBJ_FILES) $(LINKS) $(LINK_FLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	g++ $(CXXFLAGS) $(INCLUDES) -c -o $@ $<