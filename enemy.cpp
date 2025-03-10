#include "Enemy.h"
#include <cstdlib>
#include <cmath>

Enemy::Enemy(Map* map, int tankX, int tankY, SDL_Texture* Up, SDL_Texture* Down, SDL_Texture* Left, SDL_Texture* Right) {
    textureUp = Up;
    textureDown = Down;
    textureLeft = Left;
    textureRight = Right;
    currentTexture = textureLeft; // Mặc định hướng trái
    int enemyX, enemyY;

    do {
        enemyX = std::rand() % MAP_COLS;
        enemyY = std::rand() % MAP_ROWS;
    } while (map->grid[enemyY][enemyX] != TileType::EMPTY ||
             std::abs(enemyX - tankX) < 10 ||
             std::abs(enemyY - tankY) < 10);

    x = enemyX * TILE_SIZE;
    y = enemyY * TILE_SIZE;
    direction = std::rand() % 4; // 0: lên, 1: xuống, 2: trái, 3: phải
}
void Enemy::update(const std::vector<std::vector<TileType>>& map) {
    updateCount++;
    if (updateCount < 5) return; // Giảm tốc độ di chuyển
    updateCount = 0;

    int newX = x, newY = y;

    switch (direction) {
        case 0: newY -= move_step; currentTexture = textureUp; break; // Lên
        case 1: newY += move_step; currentTexture = textureDown; break; // Xuống
        case 2: newX -= move_step; currentTexture = textureLeft; break; // Trái
        case 3: newX += move_step; currentTexture = textureRight; break; // Phải
    }

    int topLeftX = newX / TILE_SIZE;
    int topLeftY = newY / TILE_SIZE;
    int topRightX = (newX + TILE_SIZE - 1) / TILE_SIZE;
    int topRightY = newY / TILE_SIZE;
    int bottomLeftX = newX / TILE_SIZE;
    int bottomLeftY = (newY + TILE_SIZE - 1) / TILE_SIZE;
    int bottomRightX = (newX + TILE_SIZE - 1) / TILE_SIZE;
    int bottomRightY = (newY + TILE_SIZE - 1) / TILE_SIZE;

    // Kiểm tra nếu enemy va vào BRICK, STEEL hoặc WATER -> Đổi hướng, không di chuyển
    if (map[topLeftY][topLeftX] == TileType::BRICK || map[topLeftY][topLeftX] == TileType::STEEL || map[topLeftY][topLeftX] == TileType::WATER ||
        map[topRightY][topRightX] == TileType::BRICK || map[topRightY][topRightX] == TileType::STEEL || map[topRightY][topRightX] == TileType::WATER ||
        map[bottomLeftY][bottomLeftX] == TileType::BRICK || map[bottomLeftY][bottomLeftX] == TileType::STEEL || map[bottomLeftY][bottomLeftX] == TileType::WATER ||
        map[bottomRightY][bottomRightX] == TileType::BRICK || map[bottomRightY][bottomRightX] == TileType::STEEL || map[bottomRightY][bottomRightX] == TileType::WATER) {

        direction = std::rand() % 4;  // Đổi hướng ngẫu nhiên
        return;  // Không di chuyển, chỉ đổi hướng
    }

    // Nếu không bị cản, enemy di chuyển
    x = newX;
    y = newY;

    // 🔹 Sau mỗi 0.5 giây, đổi hướng ngẫu nhiên (để tránh đi mãi một hướng)
    changeDirectionTimer++;
    if (changeDirectionTimer >= 600) { // Khoảng 0.5 giây nếu game chạy 60 FPS
        direction = std::rand() % 4;
        changeDirectionTimer = 0;
    }
}

void Enemy::render(SDL_Renderer* renderer) {
    SDL_Rect destRect = { x, y, TILE_SIZE, TILE_SIZE };
    SDL_RenderCopy(renderer, currentTexture, nullptr, &destRect);
}
