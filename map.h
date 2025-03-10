#ifndef MAP_H
#define MAP_H

#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// Kích thước bản đồ
const int MAP_WIDTH = 800;
const int MAP_HEIGHT = 600;
const int TILE_SIZE = 40;
const int MAP_ROWS = 15;
const int MAP_COLS = 20;
const float MOVE_STEP = TILE_SIZE / 4.0f; // Bước di chuyển chậm lại

// Các loại ô trên bản đồ
enum TileType {
    EMPTY = 0,
    BRICK = 1,
    STEEL = 2,
    GRASS = 3,
    WATER = 4
};

class Map {
public:
    std::vector<std::vector<TileType>> grid;
    std::pair<int, int> tankPosition; // Lưu vị trí xe tăng

    Map();
    void render(SDL_Renderer* renderer, SDL_Texture* steelTexture, SDL_Texture* brickTexture, SDL_Texture* grassTexture, SDL_Texture* waterTexture, SDL_Texture* tankTexture);
};

#endif // MAP_H

