#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include "Map.h"
#include "Tank.h"
#include "TextureManager.h"
#include "menu.h"
using namespace std;

int main(int argc, char* argv[]) {
    // Khởi tạo SDL cho video và âm thanh, cùng với SDL_image cho định dạng PNG
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0 || !(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
        return 1;
    }
    // Khởi tạo SDL_mixer với tần số 44100 Hz, định dạng mặc định, 2 kênh âm thanh, bộ đệm 2048 byte
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        cout << "SDL_mixer could not initialize! Mixer_Error: " << Mix_GetError() << endl;
        return 1;
    }

    // Tạo cửa sổ game với tiêu đề "Battle City", căn giữa màn hình, kích thước MAP_WIDTH x MAP_HEIGHT
    SDL_Window* window = SDL_CreateWindow("Battle City", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, MAP_WIDTH, MAP_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        cout << "Window could not be created! SDL_Error: " << SDL_GetError() << endl;
        return 1;
    }

    // Tạo renderer tăng tốc phần cứng để vẽ đồ họa
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << endl;
        return 1;
    }

    // Tải các texture cho bản đồ, xe tăng, đạn, menu, và các trạng thái game
    SDL_Texture* steelTexture = TextureManager::LoadTexture("steel.png", renderer);    // Tường thép
    SDL_Texture* brickTexture = TextureManager::LoadTexture("brick.png", renderer);    // Tường gạch
    SDL_Texture* grassTexture = TextureManager::LoadTexture("grass.png", renderer);    // Cỏ
    SDL_Texture* waterTexture = TextureManager::LoadTexture("water.png", renderer);    // Nước
    SDL_Texture* emptyTexture = TextureManager::LoadTexture("empty.png", renderer);    // Ô trống
    SDL_Texture* BulletlTexture = TextureManager::LoadTexture("Bullet.png", renderer); // Đạn
    SDL_Texture* textureUp = TextureManager::LoadTexture("tankup.png", renderer);      // Xe tăng 1 hướng lên
    SDL_Texture* textureDown = TextureManager::LoadTexture("tankdown.png", renderer);  // Xe tăng 1 hướng xuống
    SDL_Texture* textureLeft = TextureManager::LoadTexture("tankleft.png", renderer);  // Xe tăng 1 hướng trái
    SDL_Texture* textureRight = TextureManager::LoadTexture("tankright.png", renderer);// Xe tăng 1 hướng phải
    SDL_Texture* textureUp2 = TextureManager::LoadTexture("tank2up.png", renderer);    // Xe tăng 2 hướng lên
    SDL_Texture* textureDown2 = TextureManager::LoadTexture("tank2down.png", renderer);// Xe tăng 2 hướng xuống
    SDL_Texture* textureLeft2 = TextureManager::LoadTexture("tank2left.png", renderer);// Xe tăng 2 hướng trái
    SDL_Texture* textureRight2 = TextureManager::LoadTexture("tank2right.png", renderer);// Xe tăng 2 hướng phải
    SDL_Texture* menuBackground = TextureManager::LoadTexture("menu_bg.png", renderer);// Nền menu
    SDL_Texture* playButtonTexture = TextureManager::LoadTexture("play_button.png", renderer); // Nút "Play"
    SDL_Texture* titleTexture = TextureManager::LoadTexture("battlecity.png", renderer); // Chữ "Battle City"
    SDL_Texture* winTexture = TextureManager::LoadTexture("win.png", renderer);        // Hình ảnh chiến thắng
    SDL_Texture* heartTexture = TextureManager::LoadTexture("heart.png", renderer);    // Trái tim (mạng sống)

    // Kiểm tra xem các texture quan trọng có được tải thành công không
    if (!steelTexture || !brickTexture || !grassTexture || !waterTexture || !emptyTexture || !heartTexture) {
        cout << "Failed to load one or more textures!" << endl;
        return 1;
    }

    // Tải các hiệu ứng âm thanh
    Mix_Chunk* startSound = Mix_LoadWAV("start.wav");    // Âm thanh khi bắt đầu game
    Mix_Chunk* gameoverSound = Mix_LoadWAV("gameover.wav"); // Âm thanh khi game over
    Mix_Chunk* shootSound = Mix_LoadWAV("shoot.wav");    // Âm thanh khi bắn
    // Kiểm tra xem âm thanh có được tải thành công không
    if (!startSound || !gameoverSound || !shootSound) {
        cout << "Failed to load sound effects! Mixer_Error: " << Mix_GetError() << endl;
        return 1;
    }

    // Khởi tạo bản đồ game
    Map map;

    // Khởi tạo xe tăng 1 tại vị trí (40, 40) với 3 mạng sống
    int spawnX = 40, spawnY = 40;
    Tank tank(spawnX, spawnY, textureUp, textureDown, textureLeft, textureRight, 3);
    tank.winTexture = winTexture;        // Gán texture chiến thắng
    tank.heartTexture = heartTexture;    // Gán texture trái tim (mạng sống)

    // Khởi tạo xe tăng 2 tại góc đối diện bản đồ với 3 mạng sống
    int spawnX2 = MAP_WIDTH - 40 - TILE_SIZE;
    int spawnY2 = MAP_HEIGHT - 40 - TILE_SIZE;
    Tank tank2(spawnX2, spawnY2, textureUp2, textureDown2, textureLeft2, textureRight2, 3);
    tank2.winTexture = winTexture;       // Gán texture chiến thắng
    tank2.heartTexture = heartTexture;   // Gán texture trái tim (mạng sống)

    // Khởi tạo menu với renderer
    Menu menu(renderer);
    bool inMenu = true;  // Trạng thái hiển thị menu
    bool running = false; // Trạng thái game đang chạy

    // Vòng lặp chính của game
    while (true) {
        if (inMenu) {
            SDL_Event e;
            // Vị trí và kích thước của nút "Play" và tiêu đề
            SDL_Rect playButtonRect = { MAP_WIDTH / 2 - 100, MAP_HEIGHT / 2, 200, 200 };
            SDL_Rect titleRect = { MAP_WIDTH / 2 - 200, 100, 400, 150 };
            while (inMenu) {
                while (SDL_PollEvent(&e)) {
                    if (e.type == SDL_QUIT) return 0; // Thoát game nếu nhấn nút đóng cửa sổ
                    if (e.type == SDL_MOUSEBUTTONDOWN) { // Xử lý sự kiện nhấn chuột
                        int x = e.button.x, y = e.button.y;
                        // Kiểm tra nếu nhấn vào nút "Play"
                        if (x >= playButtonRect.x && x <= (playButtonRect.x + playButtonRect.w) &&
                            y >= playButtonRect.y && y <= (playButtonRect.y + playButtonRect.h)) {
                            inMenu = false; // Thoát menu
                            running = true; // Bắt đầu game
                            Mix_PlayChannel(-1, startSound, 0); // Phát âm thanh bắt đầu
                            map.reset(); // Reset bản đồ
                            spawnX = 40; spawnY = 40; // Đặt lại vị trí spawn xe tăng 1
                            spawnX2 = MAP_WIDTH - 40 - TILE_SIZE; // Đặt lại vị trí spawn xe tăng 2
                            spawnY2 = MAP_HEIGHT - 40 - TILE_SIZE;
                            tank.reset(spawnX, spawnY); // Reset xe tăng 1
                            tank2.reset(spawnX2, spawnY2); // Reset xe tăng 2
                        }
                    }
                }
                // Vẽ menu
                SDL_RenderClear(renderer);
                SDL_RenderCopy(renderer, menuBackground, NULL, NULL); // Vẽ nền
                SDL_RenderCopy(renderer, titleTexture, NULL, &titleRect); // Vẽ tiêu đề
                SDL_RenderCopy(renderer, playButtonTexture, NULL, &playButtonRect); // Vẽ nút "Play"
                SDL_RenderPresent(renderer); // Hiển thị lên màn hình
            }
        }

        // Vòng lặp game khi đang chạy
        while (running) {
            SDL_Event e;
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) return 0; // Thoát game nếu nhấn nút đóng cửa sổ
            }
            const Uint8* keyStates = SDL_GetKeyboardState(NULL); // Lấy trạng thái bàn phím
            // Điều khiển xe tăng 1
            if (keyStates[SDL_SCANCODE_W]) tank.move(0, -1, map.grid); // Lên
            if (keyStates[SDL_SCANCODE_S]) tank.move(0, 1, map.grid);  // Xuống
            if (keyStates[SDL_SCANCODE_A]) tank.move(-1, 0, map.grid); // Trái
            if (keyStates[SDL_SCANCODE_D]) tank.move(1, 0, map.grid);  // Phải
            if (keyStates[SDL_SCANCODE_SPACE]) { // Bắn
                tank.shoot(&map);
                Mix_PlayChannel(-1, shootSound, 0); // Phát âm thanh bắn
            }
            // Điều khiển xe tăng 2
            if (keyStates[SDL_SCANCODE_UP]) tank2.move(0, -1, map.grid);    // Lên
            if (keyStates[SDL_SCANCODE_DOWN]) tank2.move(0, 1, map.grid);   // Xuống
            if (keyStates[SDL_SCANCODE_LEFT]) tank2.move(-1, 0, map.grid);  // Trái
            if (keyStates[SDL_SCANCODE_RIGHT]) tank2.move(1, 0, map.grid);  // Phải
            if (keyStates[SDL_SCANCODE_KP_3]) { // Bắn
                tank2.shoot(&map);
                Mix_PlayChannel(-1, shootSound, 0); // Phát âm thanh bắn
            }

            // Cập nhật trạng thái xe tăng và đạn
            tank.update();
            tank2.update();
            tank.updateBullets(tank2, tank, running); // Xe tăng 1 bắn xe tăng 2
            tank2.updateBullets(tank, tank2, running); // Xe tăng 2 bắn xe tăng 1

            // Xóa màn hình với màu đen
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            // Vẽ bản đồ (không bao gồm cỏ)
            map.renderBase(renderer, steelTexture, brickTexture, waterTexture, emptyTexture);
            // Vẽ xe tăng
            tank.render(renderer);
            tank2.render(renderer);
            // Vẽ lớp cỏ (che xe tăng)
            map.renderGrass(renderer, grassTexture);
            SDL_RenderPresent(renderer); // Hiển thị lên màn hình

            // Kiểm tra điều kiện thắng
            if (tank.hasWon || tank2.hasWon) {
                Mix_PlayChannel(-1, gameoverSound, 0); // Phát âm thanh game over
                SDL_Delay(3000); // Chờ 3 giây
                inMenu = true;   // Quay lại menu
                running = false; // Dừng game
            }
            SDL_Delay(16); // Giới hạn FPS (~60 FPS)
        }
    }

    // Giải phóng bộ nhớ trước khi thoát
    Mix_FreeChunk(startSound);
    Mix_FreeChunk(gameoverSound);
    Mix_FreeChunk(shootSound);
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
    SDL_DestroyTexture(heartTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_CloseAudio(); // Đóng SDL_mixer
    SDL_Quit();

    return 0;
}
