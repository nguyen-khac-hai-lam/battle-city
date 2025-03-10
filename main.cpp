#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include "Map.h"
#include "Tank.h"
#include "TextureManager.h"
#include "enemy.h"
using namespace std;

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0 || !(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Battle City", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, MAP_WIDTH, MAP_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        cout << "Window could not be created! SDL_Error: " << SDL_GetError() << endl;
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << endl;
        return 1;
    }

    // Load các texture
    SDL_Texture* steelTexture = TextureManager::LoadTexture("steel.png", renderer);
    SDL_Texture* brickTexture = TextureManager::LoadTexture("brick.png", renderer);
    SDL_Texture* grassTexture = TextureManager::LoadTexture("grass.png", renderer);
    SDL_Texture* waterTexture = TextureManager::LoadTexture("water.png", renderer);
    SDL_Texture* tankTexture = TextureManager::LoadTexture("tank.png", renderer);
    SDL_Texture* emptyTexture = TextureManager::LoadTexture("empty.png", renderer);
    SDL_Texture* BulletlTexture = TextureManager::LoadTexture("Bullet.png", renderer);
    SDL_Texture* textureUp = TextureManager::LoadTexture("tankup.png", renderer);
    SDL_Texture* textureDown = TextureManager::LoadTexture("tankdown.png", renderer);
    SDL_Texture* textureLeft = TextureManager::LoadTexture("tankleft.png", renderer);
    SDL_Texture* textureRight = TextureManager::LoadTexture("tankright.png", renderer);
    SDL_Texture* enemyUp = TextureManager::LoadTexture("enemyup.png", renderer);
    SDL_Texture* enemyDown = TextureManager::LoadTexture("enemydown.png", renderer);
    SDL_Texture* enemyLeft = TextureManager::LoadTexture("enemyleft.png", renderer);
    SDL_Texture* enemyRight = TextureManager::LoadTexture("enemyright.png", renderer);



    if (!steelTexture || !brickTexture || !grassTexture || !waterTexture || !tankTexture || !emptyTexture) {
        cout << "Failed to load one or more textures!" << endl;
        return 1;
    }

    // Khởi tạo bản đồ
    Map map;

    // Khởi tạo xe tăng
    Tank tank(40, 40, textureUp, textureDown, textureLeft, textureRight);
    Enemy enemy(&map, tank.getX() / TILE_SIZE, tank.getY() / TILE_SIZE, enemyUp, enemyDown, enemyLeft, enemyRight);

    enemy.update(map.grid);
    bool running = true;
    SDL_Event e;

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = false;
            }
            else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_UP:    tank.move(0, -1, map.grid); break;
                    case SDLK_DOWN:  tank.move(0, 1, map.grid); break;
                    case SDLK_LEFT:  tank.move(-1, 0, map.grid); break;
                    case SDLK_RIGHT: tank.move(1, 0, map.grid); break;
                    case SDLK_SPACE: tank.shoot(&map); break;
        }
    }
}
        enemy.update(map.grid);
        tank.updateBullets();

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Đảm bảo truyền đủ texture
        map.render(renderer, steelTexture, brickTexture, grassTexture, waterTexture, emptyTexture);
        tank.render(renderer);
        enemy.render(renderer);

        SDL_RenderPresent(renderer);
    }

    // Giải phóng bộ nhớ
    SDL_DestroyTexture(steelTexture);
    SDL_DestroyTexture(brickTexture);
    SDL_DestroyTexture(grassTexture);
    SDL_DestroyTexture(waterTexture);
    SDL_DestroyTexture(tankTexture);
    SDL_DestroyTexture(emptyTexture);
    SDL_DestroyTexture(enemyUp);
    SDL_DestroyTexture(enemyDown);
    SDL_DestroyTexture(enemyLeft);
    SDL_DestroyTexture(enemyRight);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
