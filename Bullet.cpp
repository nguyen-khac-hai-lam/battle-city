#include "Bullet.h"
#include "Tank.h"
#include <iostream>

// Constructor: Khởi tạo đạn với vị trí ban đầu, hướng, và bản đồ
Bullet::Bullet(int startX, int startY, int dir, Map* gameMap) {
    x = startX;           // Tọa độ x ban đầu
    y = startY;           // Tọa độ y ban đầu
    direction = dir;      // Hướng đạn (0: lên, 1: xuống, 2: trái, 3: phải)
    speed = 20;           // Tốc độ di chuyển của đạn
    active = true;        // Trạng thái hoạt động của đạn
    map = gameMap;        // Con trỏ tới bản đồ để kiểm tra va chạm
}

// Cập nhật vị trí và xử lý va chạm của đạn
void Bullet::update(Tank& enemyTank, Tank& playerTank, bool& running) {
    if (!active) return; // Nếu đạn không hoạt động, thoát hàm

    // Tính toán vị trí mới dựa trên hướng
    int newX = x;
    int newY = y;
    switch (direction) {
        case 0: newY -= speed; break; // Lên
        case 1: newY += speed; break; // Xuống
        case 2: newX -= speed; break; // Trái
        case 3: newX += speed; break; // Phải
    }

    // Hình chữ nhật đại diện cho đạn (kích thước 5x5)
    SDL_Rect bulletRect = { newX, newY, 5, 5 };

    // Xác định phạm vi ô lưới gần vị trí mới của đạn để kiểm tra va chạm
    int startGridX = (newX - 5) / TILE_SIZE; // Ô bắt đầu (trừ kích thước đạn)
    int endGridX = (newX + 5) / TILE_SIZE;   // Ô kết thúc
    int startGridY = (newY - 5) / TILE_SIZE;
    int endGridY = (newY + 5) / TILE_SIZE;

    // Giới hạn phạm vi kiểm tra trong bản đồ
    startGridX = std::max(0, startGridX);
    endGridX = std::min(MAP_COLS - 1, endGridX);
    startGridY = std::max(0, startGridY);
    endGridY = std::min(MAP_ROWS - 1, endGridY);

    // Kiểm tra va chạm với bản đồ (gạch hoặc thép)
    for (int gridY = startGridY; gridY <= endGridY; gridY++) {
        for (int gridX = startGridX; gridX <= endGridX; gridX++) {
            if (map->grid[gridY][gridX] == TileType::BRICK || map->grid[gridY][gridX] == TileType::STEEL) {
                SDL_Rect tileRect = { gridX * TILE_SIZE, gridY * TILE_SIZE, TILE_SIZE, TILE_SIZE };
                if (SDL_HasIntersection(&bulletRect, &tileRect)) { // Nếu đạn chạm gạch hoặc thép
                    if (map->grid[gridY][gridX] == TileType::BRICK) {
                        map->grid[gridY][gridX] = TileType::EMPTY; // Phá hủy tường gạch
                    }
                    active = false; // Đạn biến mất
                    return;         // Thoát hàm ngay
                }
            }
        }
    }

    // Cập nhật vị trí nếu không va chạm với bản đồ
    x = newX;
    y = newY;

    // Kiểm tra nếu đạn ra khỏi màn hình thì vô hiệu hóa
    if (x < 0 || x > MAP_COLS * TILE_SIZE || y < 0 || y > MAP_ROWS * TILE_SIZE) {
        active = false;
        return;
    }

    // Kiểm tra va chạm với xe tăng địch
    if (!enemyTank.isDestroyed && enemyTank.isHitByBullet(x, y)) {
        enemyTank.destroy(); // Trừ mạng của xe tăng địch
        if (enemyTank.isDestroyed) { // Nếu xe tăng địch hết mạng
            playerTank.hasWon = true; // Người chơi thắng
            running = false;          // Kết thúc game
        }
        active = false; // Đạn biến mất
    }
}

// Vẽ đạn lên màn hình
void Bullet::render(SDL_Renderer* renderer) {
    if (!active) return; // Không vẽ nếu đạn không hoạt động

    SDL_Rect bulletRect = {x, y, 5, 5}; // Hình chữ nhật đại diện đạn
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // Màu vàng cho đạn
    SDL_RenderFillRect(renderer, &bulletRect); // Vẽ đạn
}
