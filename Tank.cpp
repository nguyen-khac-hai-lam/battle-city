#include "Tank.h"
#include <algorithm>

Tank::Tank(int x, int y, SDL_Texture* Up, SDL_Texture* Down, SDL_Texture* Left, SDL_Texture* Right, int move_step)
    : x(x), y(y), width(TILE_SIZE), height(TILE_SIZE),
      textureUp(Up), textureDown(Down), textureLeft(Left), textureRight(Right) {
    this->move_step = move_step;
    direction = 0;
    lives = 3;
    spawnX = x;
    spawnY = y;
    invincible = false;
    invincibleTime = 0;
    lastShotTime = 0;
}

// Các hàm khác giữ nguyên
void Tank::move(int dx, int dy, const std::vector<std::vector<TileType>>& map) {
    int newX = x + dx * move_step;
    int newY = y + dy * move_step;

    if (dx == 1) direction = 3;
    else if (dx == -1) direction = 2;
    else if (dy == 1) direction = 1;
    else if (dy == -1) direction = 0;

    int topLeftX = newX / TILE_SIZE;
    int topLeftY = newY / TILE_SIZE;
    int topRightX = (newX + width - 1) / TILE_SIZE;
    int topRightY = newY / TILE_SIZE;
    int bottomLeftX = newX / TILE_SIZE;
    int bottomLeftY = (newY + height - 1) / TILE_SIZE;
    int bottomRightX = (newX + width - 1) / TILE_SIZE;
    int bottomRightY = (newY + height - 1) / TILE_SIZE;

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
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - lastShotTime >= 1000) {
        int bulletX = x + width / 2;
        int bulletY = y + height / 2;
        bullets.push_back(Bullet(bulletX, bulletY, direction, map));
        lastShotTime = currentTime;
    }
}

void Tank::updateBullets(Tank& enemyTank, Tank& playerTank, bool& running) {
    for (auto& bullet : bullets) {
        bullet.update(enemyTank, playerTank, running);
    }
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
        [](Bullet& b) { return !b.active; }), bullets.end());
}

void Tank::render(SDL_Renderer* renderer) {
    if (isDestroyed) return;
    if (invincible) {
        Uint32 currentTime = SDL_GetTicks();
        // Nhấp nháy mỗi 200ms (bật/tắt)
        if ((currentTime / 200) % 2 == 0) {
            return; // Không vẽ tank trong frame này (tắt)
        }
    }
    SDL_Rect tankRect = { x, y, width, height };
    SDL_Texture* currentTexture = (direction == 0) ? textureUp :
                                  (direction == 1) ? textureDown :
                                  (direction == 2) ? textureLeft :
                                  textureRight;
    SDL_RenderCopy(renderer, currentTexture, nullptr, &tankRect);

    if (heartTexture) {
        const int HEART_SIZE = 16;
        const int HEART_SPACING = 2;
        for (int i = 0; i < lives; i++) {
            SDL_Rect heartRect = { x + i * (HEART_SIZE + HEART_SPACING), y - HEART_SIZE - 5-10, HEART_SIZE, HEART_SIZE };
            SDL_RenderCopy(renderer, heartTexture, nullptr, &heartRect);
        }
    }

    if (hasWon) {
        SDL_Rect winRect = { x, y - 40, 40, 24 };
        SDL_RenderCopy(renderer, winTexture, nullptr, &winRect);
    }
    if (hasLost && loseTexture) {
        SDL_Rect loseRect = { x, y - 40, 50, 30 };
        SDL_RenderCopy(renderer, loseTexture, nullptr, &loseRect);
    }
    for (auto& bullet : bullets) {
        bullet.render(renderer);
    }
}

bool Tank::isHitByBullet(int bulletX, int bulletY) {
    if (isDestroyed || invincible) return false;
    SDL_Rect tankRect = { x, y, width, height };
    SDL_Rect bulletRect = { bulletX, bulletY, 5, 5 };
    return SDL_HasIntersection(&tankRect, &bulletRect);
}

void Tank::destroy() {
    if (invincible) return;
    lives--;
    if (lives > 0) {
        invincible = true;
        invincibleTime = SDL_GetTicks() + 3000;
        reset2();
    } else {
        isDestroyed = true;
    }
}

void Tank::update() {
    if (invincible) {
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime >= invincibleTime) {
            invincible = false;
        }
    }
}

void Tank::reset(int &spawnx, int &spawny) {
    isDestroyed = false;
    hasWon = false;
    hasLost = false;
    x = spawnx;
    y = spawny;
    lives = 3;
    lastShotTime = 0;
}

void Tank::reset2() {
    isDestroyed = false;
    hasWon = false;
    hasLost = false;
    x = spawnX;
    y = spawnY;
}
