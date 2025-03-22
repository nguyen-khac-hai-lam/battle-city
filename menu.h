#ifndef MENU_H
#define MENU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


class Menu {
public:
    Menu(SDL_Renderer* renderer);
    ~Menu();

    void render();                      // Khai báo hàm render
    bool handleEvent(SDL_Event& event); // Khai báo hàm xử lý sự kiện

private:
    SDL_Renderer* renderer;
    SDL_Texture* background;
    SDL_Texture* playButton;
    SDL_Texture* title;  // Thêm biến chứa chữ "BATTLE CITY"
    SDL_Rect playButtonRect;
    SDL_Rect titleRect; // Vị trí của chữ "BATTLE CITY"
};


#endif // MENU_H

