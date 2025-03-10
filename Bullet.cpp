#include "Bullet.h"

Bullet::Bullet(int startX, int startY, int dir, Map* gameMap) {
    x = startX;
    y = startY;
    direction = dir;
    speed = 5;
    active = true;
    map = gameMap; // Lưu con trỏ bản đồ
}

void Bullet::update() {
    if (!active) return;

    // Di chuyển đạn
    switch (direction) {
        case 0: y -= speed; break; // Lên
        case 1: y += speed; break; // Xuống
        case 2: x -= speed; break; // Trái
        case 3: x += speed; break; // Phải
    }

    // Kiểm tra va chạm với gạch
    int gridX = x / TILE_SIZE;
    int gridY = y / TILE_SIZE;

    if (gridX >= 0 && gridX < MAP_COLS && gridY >= 0 && gridY < MAP_ROWS) {
        if (map->grid[gridY][gridX] == TileType::BRICK) {
            map->grid[gridY][gridX] = TileType::EMPTY; // Xóa tường gạch
            active = false; // Xóa đạn
            return;
        }
    }

    // Kiểm tra nếu viên đạn ra khỏi màn hình thì vô hiệu hóa
    if (x < 0 || x > MAP_COLS * TILE_SIZE || y < 0 || y > MAP_ROWS * TILE_SIZE) {
        active = false;
    }
}

void Bullet::render(SDL_Renderer* renderer) {
    if (!active) return;

    SDL_Rect bulletRect = {x, y, 5, 5}; // Kích thước viên đạn
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // Màu vàng
    SDL_RenderFillRect(renderer, &bulletRect);
}

