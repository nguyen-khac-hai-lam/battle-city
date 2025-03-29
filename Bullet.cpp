#include "Bullet.h"
#include "Tank.h"
#include <iostream>

Bullet::Bullet(int startX, int startY, int dir, Map* gameMap) {
    x = startX;
    y = startY;
    direction = dir;
    speed = 20; // Tốc độ đạn
    active = true;
    map = gameMap; // Lưu con trỏ bản đồ
}

void Bullet::update(Tank& enemyTank, Tank& playerTank, bool& running) {
    if (!active) return;

    // Tính toán vị trí mới của đạn
    int newX = x;
    int newY = y;
    switch (direction) {
        case 0: newY -= speed; break; // Lên
        case 1: newY += speed; break; // Xuống
        case 2: newX -= speed; break; // Trái
        case 3: newX += speed; break; // Phải
    }

    // Kiểm tra va chạm với bản đồ (BRICK hoặc STEEL)
    SDL_Rect bulletRect = { newX, newY, 5, 5 }; // Kích thước đạn (5x5)

    // Chỉ kiểm tra các ô gần vị trí mới của đạn để tối ưu
    int startGridX = (newX - 5) / TILE_SIZE; // Ô bắt đầu (trừ kích thước đạn)
    int endGridX = (newX + 5) / TILE_SIZE;   // Ô kết thúc
    int startGridY = (newY - 5) / TILE_SIZE;
    int endGridY = (newY + 5) / TILE_SIZE;

    // Giới hạn phạm vi kiểm tra trong bản đồ
    startGridX = std::max(0, startGridX);
    endGridX = std::min(MAP_COLS - 1, endGridX);
    startGridY = std::max(0, startGridY);
    endGridY = std::min(MAP_ROWS - 1, endGridY);

    for (int gridY = startGridY; gridY <= endGridY; gridY++) {
        for (int gridX = startGridX; gridX <= endGridX; gridX++) {
            if (map->grid[gridY][gridX] == TileType::BRICK || map->grid[gridY][gridX] == TileType::STEEL) {
                SDL_Rect tileRect = { gridX * TILE_SIZE, gridY * TILE_SIZE, TILE_SIZE, TILE_SIZE };
                if (SDL_HasIntersection(&bulletRect, &tileRect)) {
                    if (map->grid[gridY][gridX] == TileType::BRICK) {
                        map->grid[gridY][gridX] = TileType::EMPTY; // Phá hủy tường gạch
                    }
                    active = false; // Đạn biến mất ngay khi chạm ô đầu tiên
                    return; // Thoát hàm ngay lập tức
                }
            }
        }
    }

    // Cập nhật vị trí nếu không va chạm
    x = newX;
    y = newY;

    // Kiểm tra nếu viên đạn ra khỏi màn hình thì vô hiệu hóa
    if (x < 0 || x > MAP_COLS * TILE_SIZE || y < 0 || y > MAP_ROWS * TILE_SIZE) {
        active = false;
        return;
    }

    // Kiểm tra va chạm với tank địch
    if (!enemyTank.isDestroyed && enemyTank.isHitByBullet(x, y)) {
        enemyTank.destroy(); // Trừ mạng thay vì hủy ngay
        if (enemyTank.isDestroyed) {
            enemyTank.hasLost = true;
            playerTank.hasWon = true;
            running = false; // Kết thúc game nếu hết mạng
        }
        active = false;
    }
}

void Bullet::render(SDL_Renderer* renderer) {
    if (!active) return;

    SDL_Rect bulletRect = {x, y, 5, 5}; // Kích thước viên đạn
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // Màu vàng
    SDL_RenderFillRect(renderer, &bulletRect);
}
