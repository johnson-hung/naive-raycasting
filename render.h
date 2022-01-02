#ifndef RENDER_H
#define RENDER_H

#include <vector>
#include "canvas.h"
#include "map.h"
#include "texture.h"
#include "player.h"
#include "sprite.h"


bool sortSpritesByDistance(std::vector<Sprite>& sprites, Player& player);

bool renderWorldSprite(Canvas& canvas, Sprite& sprite, Texture& texture, Player& player, std::vector<float>& buf);
bool renderWorld(Canvas& canvas, Map& map, Texture& textures, Texture& spriteTextures, Player& player, std::vector<Sprite>& sprites);
bool renderMapPlayer(Canvas& canvas, Player& player);
bool renderMapSprites(Canvas& canvas, std::vector<Sprite>& sprites);
bool renderMap(Canvas& canvas, Map& map, Texture& textures);
bool renderHUDPlaceholder(Canvas& canvas);
void render(Canvas& canvas, Map& map, Texture& textures, Texture& spriteTextures, Player& player, std::vector<Sprite>& sprites);

#endif