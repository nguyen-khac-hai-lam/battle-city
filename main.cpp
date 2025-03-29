#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include "Map.h"
#include "Tank.h"
#include "TextureManager.h"
#include "menu.h"
using namespace std;

int main(int argc, char* argv[]) {
    // Khởi tạo SDL và cửa sổ (giữ nguyên code cũ)
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

    // Load các texture (thêm heartTexture)
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
    SDL_Texture* winTexture = TextureManager::LoadTexture("win.png", renderer);
    SDL_Texture* loseTexture = TextureManager::LoadTexture("lose.png", renderer);
    SDL_Texture* heartTexture = TextureManager::LoadTexture("heart.png", renderer); // Load heart.png

    if (!steelTexture || !brickTexture || !grassTexture || !waterTexture || !emptyTexture || !heartTexture) {
        cout << "Failed to load one or more textures!" << endl;
        return 1;
    }

    // Khởi tạo bản đồ
    Map map;

    // Khởi tạo xe tăng
    int spawnX = 40, spawnY = 40;
    Tank tank(spawnX, spawnY, textureUp, textureDown, textureLeft, textureRight, 3);
    tank.winTexture = winTexture;
    tank.loseTexture = loseTexture;
    tank.heartTexture = heartTexture; // Gán heartTexture cho tank1

    int spawnX2 = MAP_WIDTH - 40 - TILE_SIZE;
    int spawnY2 = MAP_HEIGHT - 40 - TILE_SIZE;
    Tank tank2(spawnX2, spawnY2, textureUp2, textureDown2, textureLeft2, textureRight2, 3);
    tank2.winTexture = winTexture;
    tank2.loseTexture = loseTexture;
    tank2.heartTexture = heartTexture; // Gán heartTexture cho tank2

    // Phần còn lại của main.cpp giữ nguyên (menu, game loop, cleanup)
    Menu menu(renderer);
    bool inMenu = true;
    bool running = false;

    while (true) {
        if (inMenu) {
            SDL_Event e;
            SDL_Rect playButtonRect = { MAP_WIDTH / 2 - 100, MAP_HEIGHT / 2, 200, 200 };
            SDL_Rect titleRect = { MAP_WIDTH / 2 - 200, 100, 400, 150 };
            while (inMenu) {
                while (SDL_PollEvent(&e)) {
                    if (e.type == SDL_QUIT) return 0;
                    if (e.type == SDL_MOUSEBUTTONDOWN) {
                        int x = e.button.x, y = e.button.y;
                        if (x >= playButtonRect.x && x <= (playButtonRect.x + playButtonRect.w) &&
                            y >= playButtonRect.y && y <= (playButtonRect.y + playButtonRect.h)) {
                            inMenu = false;
                            running = true;
                            map.reset();
                            spawnX = 40;
                            spawnY = 40;
                            spawnX2 = MAP_WIDTH - 40 - TILE_SIZE;
                            spawnY2 = MAP_HEIGHT - 40 - TILE_SIZE;
                            tank.reset(spawnX, spawnY);
                            tank2.reset(spawnX2, spawnY2);
                        }
                    }
                }
                SDL_RenderClear(renderer);
                SDL_RenderCopy(renderer, menuBackground, NULL, NULL);
                SDL_RenderCopy(renderer, titleTexture, NULL, &titleRect);
                SDL_RenderCopy(renderer, playButtonTexture, NULL, &playButtonRect);
                SDL_RenderPresent(renderer);
            }
        }

        while (running) {
            SDL_Event e;
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) return 0;
            }
            const Uint8* keyStates = SDL_GetKeyboardState(NULL);
            if (keyStates[SDL_SCANCODE_W]) tank.move(0, -1, map.grid);
            if (keyStates[SDL_SCANCODE_S]) tank.move(0, 1, map.grid);
            if (keyStates[SDL_SCANCODE_A]) tank.move(-1, 0, map.grid);
            if (keyStates[SDL_SCANCODE_D]) tank.move(1, 0, map.grid);
            if (keyStates[SDL_SCANCODE_SPACE]) tank.shoot(&map);
            if (keyStates[SDL_SCANCODE_UP]) tank2.move(0, -1, map.grid);
            if (keyStates[SDL_SCANCODE_DOWN]) tank2.move(0, 1, map.grid);
            if (keyStates[SDL_SCANCODE_LEFT]) tank2.move(-1, 0, map.grid);
            if (keyStates[SDL_SCANCODE_RIGHT]) tank2.move(1, 0, map.grid);
            if (keyStates[SDL_SCANCODE_KP_3]) tank2.shoot(&map);

            tank.update();
            tank2.update();
            tank.updateBullets(tank2, tank, running);
            tank2.updateBullets(tank, tank2, running);

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            map.render(renderer, steelTexture, brickTexture, grassTexture, waterTexture, emptyTexture);
            tank.render(renderer);
            tank2.render(renderer);
            SDL_RenderPresent(renderer);

            if (tank.hasWon || tank2.hasWon) {
                SDL_Delay(3000);
                inMenu = true;
                running = false;
            }
            SDL_Delay(16);
        }
    }

    // Giải phóng bộ nhớ
    SDL_DestroyTexture(steelTexture);
    SDL_DestroyTexture(brickTexture);
    SDL_DestroyTexture(grassTexture);
    SDL_DestroyTexture(waterTexture);
    SDL_DestroyTexture(emptyTexture);
    SDL_DestroyTexture(BulletlTexture);
    SDL_DestroyTexture(textureUp);
    SDL_DestroyTexture(textureDown);
    SDL_DestroyTexture(textureLeft);
    SDL_DestroyTexture(textureRight);
    SDL_DestroyTexture(textureUp2);
    SDL_DestroyTexture(textureDown2);
    SDL_DestroyTexture(textureLeft2);
    SDL_DestroyTexture(textureRight2);
    SDL_DestroyTexture(menuBackground);
    SDL_DestroyTexture(playButtonTexture);
    SDL_DestroyTexture(titleTexture);
    SDL_DestroyTexture(winTexture);
    SDL_DestroyTexture(loseTexture);
    SDL_DestroyTexture(heartTexture); // Giải phóng heartTexture
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
