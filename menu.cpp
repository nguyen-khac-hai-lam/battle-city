#include "menu.h"
#include <iostream>

Menu::Menu(SDL_Renderer* renderer) {
    this->renderer = renderer;

    background = IMG_LoadTexture(renderer, "menu_bg.png");
    playButton = IMG_LoadTexture(renderer, "play_button.png");
    title = IMG_LoadTexture(renderer, "battlecity.png"); // Load chữ Battle City

    if (!background || !playButton || !title) {
        std::cerr << "Failed to load menu textures!" << std::endl;
    }

    playButtonRect = { 300, 350, 250, 100 };  // Kích thước nút Play
    titleRect = { 200, 100, 400, 150 }; // Kích thước chữ Battle City
}

Menu::~Menu() {
    SDL_DestroyTexture(background);
    SDL_DestroyTexture(playButton);
    SDL_DestroyTexture(title);
}

void Menu::render() {
    SDL_RenderCopy(renderer, background, nullptr, nullptr);
    SDL_RenderCopy(renderer, title, nullptr, &titleRect);
    SDL_RenderCopy(renderer, playButton, nullptr, &playButtonRect);
}

bool Menu::handleEvent(SDL_Event& e) {
    if (e.type == SDL_MOUSEBUTTONDOWN) {
        int x, y;
        SDL_GetMouseState(&x, &y);

        if (x >= playButtonRect.x && x <= (playButtonRect.x + playButtonRect.w) &&
            y >= playButtonRect.y && y <= (playButtonRect.y + playButtonRect.h)) {
            return true; // Bắt đầu game
        }
    }
    return false;
}

