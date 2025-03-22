#include "Tank.h"
#include <algorithm>
Tank::Tank(int x, int y, SDL_Texture* Up, SDL_Texture* Down, SDL_Texture* Left, SDL_Texture* Right)
    : x(x), y(y), width(TILE_SIZE), height(TILE_SIZE),
      textureUp(Up), textureDown(Down), textureLeft(Left), textureRight(Right) {
    move_step = 10;
    direction = 0;
}


void Tank::move(int dx, int dy, const std::vector<std::vector<TileType>>& map) {
    int newX = x + dx * move_step;
    int newY = y + dy * move_step;

    // Cập nhật hướng trước khi kiểm tra va chạm
    if (dx == 1) direction = 3;  // Phải
    else if (dx == -1) direction = 2;  // Trái
    else if (dy == 1) direction = 1;  // Xuống
    else if (dy == -1) direction = 0;  // Lên

    // 🔹 Kiểm tra 4 góc của xe tăng
    int topLeftX = newX / TILE_SIZE;
    int topLeftY = newY / TILE_SIZE;

    int topRightX = (newX + width - 1) / TILE_SIZE;
    int topRightY = newY / TILE_SIZE;

    int bottomLeftX = newX / TILE_SIZE;
    int bottomLeftY = (newY + height - 1) / TILE_SIZE;

    int bottomRightX = (newX + width - 1) / TILE_SIZE;
    int bottomRightY = (newY + height - 1) / TILE_SIZE;

    // Kiểm tra nếu 4 góc không chạm gạch
    bool canMove = (map[topLeftY][topLeftX] == TileType::EMPTY || map[topLeftY][topLeftX] == TileType::GRASS) &&
                   (map[topRightY][topRightX] == TileType::EMPTY || map[topRightY][topRightX] == TileType::GRASS) &&
                   (map[bottomLeftY][bottomLeftX] == TileType::EMPTY || map[bottomLeftY][bottomLeftX] == TileType::GRASS) &&
                   (map[bottomRightY][bottomRightX] == TileType::EMPTY || map[bottomRightY][bottomRightX] == TileType::GRASS);


    if (canMove) {
        x = newX;
        y = newY;
    }
}



void Tank::shoot(Map* map) {
    int bulletX = x + width / 2;
    int bulletY = y + height / 2;
    bullets.push_back(Bullet(bulletX, bulletY, direction, map));
}

void Tank::updateBullets(Tank& enemyTank, bool& running) {
    for (auto& bullet : bullets) {
        bullet.update(enemyTank, running);
    }
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
        [](Bullet& b) { return !b.active; }), bullets.end());
}

void Tank::render(SDL_Renderer* renderer) {
    SDL_Rect destRect = { x, y, width, height };

    SDL_Texture* currentTexture = (direction == 0) ? textureUp :
                                  (direction == 1) ? textureDown :
                                  (direction == 2) ? textureLeft :
                                  textureRight;

    SDL_RenderCopy(renderer, currentTexture, NULL, &destRect);

    for (auto& bullet : bullets) {
        bullet.render(renderer);
    }
}
bool Tank::isHitByBullet(int bulletX, int bulletY) {
    if (isDestroyed) return false; // Nếu xe tăng đã bị hủy, bỏ qua kiểm tra

    SDL_Rect tankRect = { x, y, width, height };
    SDL_Rect bulletRect = { bulletX, bulletY, 5, 5 }; // Kích thước viên đạn

    return SDL_HasIntersection(&tankRect, &bulletRect);
}

void Tank::destroy() {
    isDestroyed = true;
    x = -100; // Di chuyển xe tăng ra khỏi màn hình
    y = -100;
}
