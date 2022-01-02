#ifndef SETTINGS_H
#define SETTINGS_H

// Constant
#define PI                  3.14159265

// WORLD_UPDATE_PERIOD: Period of time [ms] for the next world update
#define WORLD_UPDATE_PERIOD 20

// CANVAS: The actual window size
#define CANVAS_WIDTH        512
#define CANVAS_HEIGHT       640

// MAIN: The area where the 3D view would be displayed
#define MAIN_WIDTH          512
#define MAIN_HEIGHT         512

// HUD: The area where the game-related data would be printed out
#define HUD_DISPLAY         1
#define HUD_WIDTH           CANVAS_WIDTH
#define HUD_HEIGHT          CANVAS_HEIGHT / 5
#define HUD_SHIFT_X         0
#define HUD_SHIFT_Y         CANVAS_HEIGHT - 128

// FONT & TEXTFIELD (Field that holds the text)
#define FONT_FILE           "./fonts/font_alagard.ttf"
#define FONT_SIZE           24
#define TEXTFIELD_X         HUD_SHIFT_X + MAP_DISPLAY_WIDTH + 5
#define TEXTFIELD_Y         HUD_SHIFT_Y + 5

// MAP: The area that holds the top-down view of current game state
#define MAP                 "0111111111111111"\
                            "0              1"\
                            "4              1"\
                            "4              1"\
                            "4     22211    1"\
                            "1  0      1    1"\
                            "1         1    0"\
                            "0    1    1    0"\
                            "0    3    1    1"\
                            "1    1         1"\
                            "1    1         5"\
                            "1    11441     5"\
                            "1              5"\
                            "1            311"\
                            "1              1"\
                            "1111111111111111"
#define MAP_WIDTH           16
#define MAP_HEIGHT          16
#define MAP_DISPLAY         1
#define MAP_DISPLAY_WIDTH   128
#define MAP_DISPLAY_HEIGHT  128
#define MAP_RECT_WIDTH      MAP_DISPLAY_WIDTH / MAP_WIDTH
#define MAP_RECT_HEIGHT     MAP_DISPLAY_HEIGHT / MAP_HEIGHT

// TEXTURE: Textures of game objects
#define TEXTURE_FILE_WALL   "./textures/wallTextures.bmp"
#define TEXTURE_FILE_MONS   "./textures/monsterTextures.bmp"

// PLAYER: Viewport controlled by the user
#define PLAYER_DEFAULT_X    5.4
#define PLAYER_DEFAULT_Y    2.1
#define PLAYER_DEFAULT_FOV  PI / 3


#endif