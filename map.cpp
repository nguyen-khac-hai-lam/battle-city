#include "Map.h"
#include <cstdlib>
#include <ctime>

Map::Map() {
    grid.resize(MAP_ROWS, std::vector<TileType>(MAP_COLS, TileType::EMPTY));
    std::srand(std::time(nullptr)); // Khởi tạo random seed

    // Tạo tường thép bao quanh
    for (int i = 0; i < MAP_ROWS; i++) {
        for (int j = 0; j < MAP_COLS; j++) {
            if (i == 0 || i == MAP_ROWS - 1 || j == 0 || j == MAP_COLS - 1) {
                grid[i][j] = TileType::STEEL;
            } else {
                int randomValue = std::rand() % 100;
                if (randomValue < 10) grid[i][j] = TileType::BRICK;
                else if (randomValue < 20) grid[i][j] = TileType::WATER;
                else if (randomValue < 30) grid[i][j] = TileType::GRASS;
                else grid[i][j] = TileType::EMPTY; // Phần còn lại là EMPTY
            }
        }
    }

    // Đặt xe tăng vào vị trí trống
    while (true) {
        int x = std::rand() % (MAP_ROWS - 2) + 1; // Không chọn viền
        int y = std::rand() % (MAP_COLS - 2) + 1;
        if (grid[x][y] == TileType::EMPTY) {
            tankPosition = {x, y};
            break;
        }
    }
}

void Map::render(SDL_Renderer* renderer, SDL_Texture* steelTexture, SDL_Texture* brickTexture, SDL_Texture* grassTexture, SDL_Texture* waterTexture,SDL_Texture* emptyTexture) {
    for (int i = 0; i < MAP_ROWS; i++) {
        for (int j = 0; j < MAP_COLS; j++) {
            SDL_Rect tileRect = { j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE };
            switch (grid[i][j]) {
                case STEEL: SDL_RenderCopy(renderer, steelTexture, nullptr, &tileRect); break;
                case BRICK: SDL_RenderCopy(renderer, brickTexture, nullptr, &tileRect); break;
                case GRASS: SDL_RenderCopy(renderer, grassTexture, nullptr, &tileRect); break;
                case WATER: SDL_RenderCopy(renderer, waterTexture, nullptr, &tileRect); break;
                case EMPTY: SDL_RenderCopy(renderer, emptyTexture, nullptr, &tileRect); break;
                default: break;
            }
        }
    }

    // Vẽ xe tăng
}
