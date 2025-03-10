
#ifndef BULLET_H
#define BULLET_H

#include <SDL2/SDL.h>

#include "Map.h"

class Bullet {
public:
    Bullet(int startX, int startY, int dir, Map* map);
    void update();
    void render(SDL_Renderer* renderer);
    bool active;
private:
    int x, y, direction, speed;
    Map* map; // Thêm con trỏ đến bản đồ
};

#endif // BULLET_H
