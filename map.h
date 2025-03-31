#ifndef MAP_H
#define MAP_H

#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// Kích thước bản đồ
const int MAP_WIDTH = 800;  // Chiều rộng bản đồ (pixel)
const int MAP_HEIGHT = 600; // Chiều cao bản đồ (pixel)
const int TILE_SIZE = 40;   // Kích thước mỗi ô (pixel)
const int MAP_ROWS = 15;    // Số hàng của lưới bản đồ
const int MAP_COLS = 20;    // Số cột của lưới bản đồ
const float MOVE_STEP = TILE_SIZE / 4.0f; // Bước di chuyển của xe tăng

// Các loại ô trên bản đồ
enum TileType {
    EMPTY = 0, // Ô trống
    BRICK = 1, // Tường gạch (có thể phá)
    STEEL = 2, // Tường thép (không phá được)
    GRASS = 3, // Cỏ (che xe tăng)
    WATER = 4  // Nước (không đi qua được)
};

// Lớp Map quản lý bản đồ game
class Map {
public:
    std::vector<std::vector<TileType>> grid; // Lưới ô bản đồ
    std::pair<int, int> tankPosition; // Vị trí xe tăng (không sử dụng trong code hiện tại)

    Map(); // Constructor: Khởi tạo bản đồ
    void renderBase(SDL_Renderer* renderer, SDL_Texture* steelTexture, SDL_Texture* brickTexture, SDL_Texture* waterTexture, SDL_Texture* emptyTexture); // Vẽ các ô cơ bản
    void renderGrass(SDL_Renderer* renderer, SDL_Texture* grassTexture); // Vẽ lớp cỏ
    void reset(); // Reset bản đồ về trạng thái ban đầu
};

#endif // MAP_H
