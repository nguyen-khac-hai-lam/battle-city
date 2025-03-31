#ifndef MENU_H
#define MENU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// Lớp Menu quản lý giao diện menu của game
class Menu {
public:
    Menu(SDL_Renderer* renderer); // Constructor: Khởi tạo menu với renderer
    ~Menu(); // Destructor: Giải phóng tài nguyên

    void render(); // Vẽ menu lên màn hình
    bool handleEvent(SDL_Event& event); // Xử lý sự kiện (nhấn nút "Play")

private:
    SDL_Renderer* renderer; // Renderer để vẽ
    SDL_Texture* background; // Texture nền menu
    SDL_Texture* playButton; // Texture nút "Play"
    SDL_Texture* title;      // Texture chữ "BATTLE CITY"
    SDL_Rect playButtonRect; // Vị trí và kích thước nút "Play"
    SDL_Rect titleRect;      // Vị trí và kích thước chữ "BATTLE CITY"
};

#endif // MENU_H

