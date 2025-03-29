#ifndef TANK_H
#define TANK_H

#include <SDL2/SDL.h>
#include "Map.h"

class Bullet;
class Tank {
private:
    int lives = 3;
    int spawnX, spawnY;
    bool invincible;
    Uint32 invincibleTime;
    Uint32 lastShotTime;
    int x, y;
    int width, height;
    int move_step;
    int direction;
    SDL_Texture* textureUp;
    SDL_Texture* textureDown;
    SDL_Texture* textureLeft;
    SDL_Texture* textureRight;
public:
    std::vector<Bullet> bullets;
    int getX() { return x; }
    int getY() { return y; }
    Tank(int x, int y, SDL_Texture* up, SDL_Texture* down, SDL_Texture* left, SDL_Texture* right, int move_step = 10);

    void move(int dx, int dy, const std::vector<std::vector<TileType>>& map);
    void shoot(Map* map);
    void updateBullets(Tank& enemyTank, Tank& playerTank, bool& running);
    void render(SDL_Renderer* renderer);
    bool isHitByBullet(int bulletX, int bulletY);
    void destroy();
    void update();
    bool isDestroyed = false;
    SDL_Texture* winTexture = nullptr;
    SDL_Texture* loseTexture = nullptr;
    SDL_Texture* heartTexture = nullptr; // Thêm texture cho trái tim
    bool hasWon = false;
    bool hasLost = false;
    void reset(int &spawnx, int &spawny);
    void reset2();
};
#include "Bullet.h"
#include <vector>
#endif // TANK_H



