#include "menu.h"
#include <iostream>

// Constructor: Khởi tạo menu với các texture
Menu::Menu(SDL_Renderer* renderer) {
    this->renderer = renderer;

    background = IMG_LoadTexture(renderer, "menu_bg.png"); // Tải nền menu
    playButton = IMG_LoadTexture(renderer, "play_button.png"); // Tải nút "Play"
    title = IMG_LoadTexture(renderer, "battlecity.png"); // Tải chữ "Battle City"

    // Kiểm tra lỗi khi tải texture
    if (!background || !playButton || !title) {
        std::cerr << "Failed to load menu textures!" << std::endl;
    }

    playButtonRect = { 300, 350, 250, 100 }; // Vị trí và kích thước nút "Play"
    titleRect = { 200, 100, 400, 150 };      // Vị trí và kích thước chữ "Battle City"
}

// Destructor: Giải phóng các texture
Menu::~Menu() {
    SDL_DestroyTexture(background);
    SDL_DestroyTexture(playButton);
    SDL_DestroyTexture(title);
}

// Vẽ menu lên màn hình
void Menu::render() {
    SDL_RenderCopy(renderer, background, nullptr, nullptr); // Vẽ nền
    SDL_RenderCopy(renderer, title, nullptr, &titleRect);   // Vẽ chữ "Battle City"
    SDL_RenderCopy(renderer, playButton, nullptr, &playButtonRect); // Vẽ nút "Play"
}

// Xử lý sự kiện chuột để bắt đầu game
bool Menu::handleEvent(SDL_Event& e) {
    if (e.type == SDL_MOUSEBUTTONDOWN) {
        int x, y;
        SDL_GetMouseState(&x, &y); // Lấy tọa độ chuột
        // Kiểm tra nếu nhấn vào nút "Play"
        if (x >= playButtonRect.x && x <= (playButtonRect.x + playButtonRect.w) &&
            y >= playButtonRect.y && y <= (playButtonRect.y + playButtonRect.h)) {
            return true; // Trả về true để bắt đầu game
        }
    }
    return false; // Không có hành động nào được thực hiện
}

