
#ifndef ENEMY_H
#define ENEMY_H

#include <SDL2/SDL.h>
#include "Map.h"

class Enemy {
public:
    Enemy(Map* map, int tankX, int tankY, SDL_Texture* Up, SDL_Texture* Down, SDL_Texture* Left, SDL_Texture* Right);
    void update(const std::vector<std::vector<TileType>>& map);
    void render(SDL_Renderer* renderer);
    int getX() { return x; }
    int getY() { return y; }

private:
    int x, y, direction;
    int move_step = 1;   // Giảm tốc độ enemy
    int updateCount = 0; // Điều chỉnh tốc độ di chuyển
    int changeDirectionTimer = 0; // Đếm thời gian để đổi hướng
    SDL_Texture* textureUp, *textureDown, *textureLeft, *textureRight, *currentTexture;
};

#endif
