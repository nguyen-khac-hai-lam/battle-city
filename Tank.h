#ifndef TANK_H
#define TANK_H

#include <SDL2/SDL.h>

#include "Map.h"

class Bullet;
class Tank {
public:
    int x, y;
    int width, height;
    int move_step = 40;  // Tốc độ di chuyển

    int direction; // 0: lên, 1: xuống, 2: trái, 3: phải
    SDL_Texture* textureUp;
    SDL_Texture* textureDown;
    SDL_Texture* textureLeft;
    SDL_Texture* textureRight;
    std::vector<Bullet> bullets; // Danh sách đạn
    int getX() { return x; }
    int getY() { return y; }
    Tank(int x, int y, SDL_Texture* up, SDL_Texture* down, SDL_Texture* left, SDL_Texture* right);

    void move(int dx, int dy, const std::vector<std::vector<TileType>>& map);
    void shoot(Map* map); // Bắn đạn
    void updateBullets(Tank& enemyTank, bool& running);
    void render(SDL_Renderer* renderer);
    bool isHitByBullet(int bulletX, int bulletY);
    void destroy();
    bool isDestroyed = false; // Biến trạng thái xe tăng
};
#include "Bullet.h"
#include <vector>
#endif // TANK_H



