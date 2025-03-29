
#ifndef BULLET_H
#define BULLET_H

#include <SDL2/SDL.h>

#include "Map.h"
#include "Tank.h"
class Bullet {
public:
    Bullet(int startX, int startY, int dir, Map* map);
    void render(SDL_Renderer* renderer);
    bool active;
    void update(Tank& enemyTank, Tank& playerTank, bool& running);
private:
    int x, y, direction, speed;
    Map* map; // Thêm con trỏ đến bản đồ
};

#endif // BULLET_H
