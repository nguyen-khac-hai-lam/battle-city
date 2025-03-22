#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include "Map.h"
#include "Tank.h"
#include "TextureManager.h"
#include "menu.h"
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
    SDL_Texture* emptyTexture = TextureManager::LoadTexture("empty.png", renderer);
    SDL_Texture* BulletlTexture = TextureManager::LoadTexture("Bullet.png", renderer);
    SDL_Texture* textureUp = TextureManager::LoadTexture("tankup.png", renderer);
    SDL_Texture* textureDown = TextureManager::LoadTexture("tankdown.png", renderer);
    SDL_Texture* textureLeft = TextureManager::LoadTexture("tankleft.png", renderer);
    SDL_Texture* textureRight = TextureManager::LoadTexture("tankright.png", renderer);
    SDL_Texture* textureUp2 = TextureManager::LoadTexture("tank2up.png", renderer);
    SDL_Texture* textureDown2 = TextureManager::LoadTexture("tank2down.png", renderer);
    SDL_Texture* textureLeft2 = TextureManager::LoadTexture("tank2left.png", renderer);
    SDL_Texture* textureRight2 = TextureManager::LoadTexture("tank2right.png", renderer);
    SDL_Texture* menuBackground = TextureManager::LoadTexture("menu_bg.png", renderer);
    SDL_Texture* playButtonTexture = TextureManager::LoadTexture("play_button.png", renderer);
    SDL_Texture* titleTexture = TextureManager::LoadTexture("battlecity.png", renderer);


    if (!steelTexture || !brickTexture || !grassTexture || !waterTexture || !emptyTexture) {
        cout << "Failed to load one or more textures!" << endl;
        return 1;
    }

    // Khởi tạo bản đồ
    Map map;

    // Khởi tạo xe tăng
    int spawnX = 40, spawnY = 40;

// Kiểm tra xem vị trí spawn có hợp lệ không
while (map.grid[spawnY / TILE_SIZE][spawnX / TILE_SIZE] != TileType::EMPTY &&
       map.grid[spawnY / TILE_SIZE][spawnX / TILE_SIZE] != TileType::GRASS) {
    spawnX += TILE_SIZE; // Dời sang phải
    if (spawnX >= MAP_WIDTH) {
        spawnX = 40;
        spawnY += TILE_SIZE; // Xuống dưới
    }
}

Tank tank(spawnX, spawnY, textureUp, textureDown, textureLeft, textureRight);

    int spawnX2 = MAP_WIDTH - 40 - TILE_SIZE;
    int spawnY2 = MAP_HEIGHT - 40 - TILE_SIZE;

while (map.grid[spawnY2 / TILE_SIZE][spawnX2 / TILE_SIZE] != TileType::EMPTY &&
       map.grid[spawnY2 / TILE_SIZE][spawnX2 / TILE_SIZE] != TileType::GRASS) {
    spawnX2 -= TILE_SIZE; // Dời sang trái
    if (spawnX2 <= 0) {
        spawnX2 = MAP_WIDTH - 40 - TILE_SIZE;
        spawnY2 -= TILE_SIZE; // Lên trên
    }
}

Tank tank2(spawnX2, spawnY2, textureUp2, textureDown2, textureLeft2, textureRight2);
    Menu menu(renderer);

    bool inMenu = true;
    bool running = false;

    SDL_Event e;
    SDL_Rect playButtonRect = { MAP_WIDTH / 2 - 100, MAP_HEIGHT / 2, 200, 200 };
    SDL_Rect titleRect = { MAP_WIDTH / 2 - 200, 100, 400, 150 }; // Chữ Battle City
while (inMenu) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            inMenu = false;
            running = false; // Đảm bảo thoát game
        }
        if (e.type == SDL_MOUSEBUTTONDOWN) {
            int x = e.button.x, y = e.button.y;
            if (x >= playButtonRect.x && x <= (playButtonRect.x + playButtonRect.w) &&
                y >= playButtonRect.y && y <= (playButtonRect.y + playButtonRect.h)) {
                inMenu = false;
                running = true; // Vào game
            }
        }
    }

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, menuBackground, NULL, NULL); // Hiển thị nền
    SDL_RenderCopy(renderer, titleTexture, NULL, &titleRect); // Hiển thị chữ Battle City
    SDL_RenderCopy(renderer, playButtonTexture, NULL, &playButtonRect); // Hiển thị nút Play
    SDL_RenderPresent(renderer);
}


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
                    case SDLK_KP_3: tank.shoot(&map); break;
                    case SDLK_w:  tank2.move(0, -1, map.grid); break;
                    case SDLK_s:  tank2.move(0, 1, map.grid); break;
                    case SDLK_a:  tank2.move(-1, 0, map.grid); break;
                    case SDLK_d:  tank2.move(1, 0, map.grid); break;
                    case SDLK_SPACE: tank2.shoot(&map); break;
        }
    }
}
        // Kiểm tra đạn va chạm với tank1 và tank2
        for (auto& bullet : tank.bullets) {
            bullet.update(tank2, running); // Kiểm tra đạn tank1 bắn trúng tank2
        }

        for (auto& bullet : tank2.bullets) {
            bullet.update(tank, running); // Kiểm tra đạn tank2 bắn trúng tank1
        }

        tank.updateBullets(tank2, running); // Xe tăng 1 kiểm tra đạn va chạm với xe tăng 2
        tank2.updateBullets(tank, running); // Xe tăng 2 kiểm tra đạn va chạm với xe tăng 1

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Đảm bảo truyền đủ texture
        map.render(renderer, steelTexture, brickTexture, grassTexture, waterTexture, emptyTexture);
        tank.render(renderer);
        tank2.render(renderer);

        SDL_RenderPresent(renderer);
    }

    // Giải phóng bộ nhớ
    SDL_DestroyTexture(steelTexture);
    SDL_DestroyTexture(brickTexture);
    SDL_DestroyTexture(grassTexture);
    SDL_DestroyTexture(waterTexture);
    SDL_DestroyTexture(emptyTexture);
    SDL_DestroyTexture(textureUp2);
    SDL_DestroyTexture(textureDown2);
    SDL_DestroyTexture(textureLeft2);
    SDL_DestroyTexture(textureRight2);
    SDL_DestroyTexture(menuBackground);
    SDL_DestroyTexture(playButtonTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
