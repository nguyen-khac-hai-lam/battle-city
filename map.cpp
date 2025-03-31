#include "Map.h"
#include <cstdlib> // Thư viện cho các hàm như rand() (không dùng trong code này)
#include <ctime>   // Thư viện cho thời gian (không dùng trong code này)

Map::Map() {
    // Hàm khởi tạo bản đồ
    // Định nghĩa một bản đồ tĩnh (15 hàng x 20 cột) bằng mảng 2D với các loại ô (TileType)
    TileType tempMap[MAP_ROWS][MAP_COLS] = {
        {STEEL, STEEL, STEEL, STEEL, STEEL, STEEL, STEEL, STEEL, STEEL, STEEL, STEEL, STEEL, STEEL, STEEL, STEEL, STEEL, STEEL, STEEL, STEEL, STEEL}, // Hàng đầu: toàn tường thép
        {STEEL, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, GRASS, GRASS, EMPTY, EMPTY, STEEL}, // Hàng 2: tường thép 2 đầu, giữa là ô trống và cỏ
        {STEEL, EMPTY, EMPTY, STEEL, GRASS, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, BRICK, BRICK, BRICK, EMPTY, STEEL}, // Hàng 3: thêm tường gạch và cỏ
        {STEEL, EMPTY, STEEL, STEEL, GRASS, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, GRASS, EMPTY, EMPTY, EMPTY, WATER, BRICK, EMPTY, STEEL}, // Hàng 4: thêm nước
        {STEEL, EMPTY, GRASS, GRASS, WATER, EMPTY, GRASS, BRICK, EMPTY, EMPTY, EMPTY, WATER, BRICK, EMPTY, EMPTY, EMPTY, EMPTY, BRICK, EMPTY, STEEL}, // Hàng 5: thêm gạch và nước
        {STEEL, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, WATER, BRICK, WATER, WATER, BRICK, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, STEEL}, // Hàng 6: thêm nước và gạch giữa
        {STEEL, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, GRASS, BRICK, BRICK, GRASS, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, STEEL}, // Hàng 7: gạch và cỏ ở giữa
        {STEEL, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, GRASS, BRICK, BRICK, GRASS, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, STEEL}, // Hàng 8: tương tự hàng 7
        {STEEL, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, GRASS, BRICK, BRICK, GRASS, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, STEEL}, // Hàng 9: tương tự hàng 7
        {STEEL, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, BRICK, WATER, WATER, BRICK, WATER, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, STEEL}, // Hàng 10: nước và gạch ở giữa
        {STEEL, EMPTY, BRICK, EMPTY, EMPTY, EMPTY, EMPTY, BRICK, WATER, EMPTY, EMPTY, EMPTY, BRICK, GRASS, EMPTY, WATER, GRASS, GRASS, EMPTY, STEEL}, // Hàng 11: thêm cỏ và gạch
        {STEEL, EMPTY, BRICK, WATER, EMPTY, EMPTY, EMPTY, GRASS, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, GRASS, STEEL, STEEL, EMPTY, STEEL}, // Hàng 12: thêm nước và tường thép
        {STEEL, EMPTY, BRICK, BRICK, BRICK, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, GRASS, STEEL, EMPTY, EMPTY, STEEL}, // Hàng 13: thêm gạch liên tiếp
        {STEEL, EMPTY, EMPTY, GRASS, GRASS, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, STEEL}, // Hàng 14: thêm cỏ
        {STEEL, STEEL, STEEL, STEEL, STEEL, STEEL, STEEL, STEEL, STEEL, STEEL, STEEL, STEEL, STEEL, STEEL, STEEL, STEEL, STEEL, STEEL, STEEL, STEEL}, // Hàng cuối: toàn tường thép
    };

    // Sao chép mảng tĩnh vào grid (biến thành vector động để dễ xử lý)
    grid.resize(MAP_ROWS, std::vector<TileType>(MAP_COLS, EMPTY)); // Khởi tạo grid với kích thước 15x20, mặc định là ô trống
    for (int i = 0; i < MAP_ROWS; i++) {
        for (int j = 0; j < MAP_COLS; j++) {
            grid[i][j] = tempMap[i][j]; // Gán giá trị từ tempMap vào grid
        }
    }
}

// Hàm vẽ lớp cơ bản của bản đồ (thép, gạch, nước, ô trống)
void Map::renderBase(SDL_Renderer* renderer, SDL_Texture* steelTexture, SDL_Texture* brickTexture, SDL_Texture* waterTexture, SDL_Texture* emptyTexture) {
    for (int i = 0; i < MAP_ROWS; i++) { // Duyệt qua từng hàng
        for (int j = 0; j < MAP_COLS; j++) { // Duyệt qua từng cột
            SDL_Rect tileRect = { j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE }; // Tạo hình chữ nhật cho ô tại (j, i)
            switch (grid[i][j]) { // Kiểm tra loại ô và vẽ texture tương ứng
                case STEEL:
                    SDL_RenderCopy(renderer, steelTexture, nullptr, &tileRect); // Vẽ tường thép
                    break;
                case BRICK:
                    SDL_RenderCopy(renderer, brickTexture, nullptr, &tileRect); // Vẽ tường gạch
                    break;
                case WATER:
                    SDL_RenderCopy(renderer, waterTexture, nullptr, &tileRect); // Vẽ nước
                    break;
                case EMPTY:
                    SDL_RenderCopy(renderer, emptyTexture, nullptr, &tileRect); // Vẽ ô trống
                    break;
                // Lưu ý: GRASS không vẽ ở đây, sẽ vẽ riêng trong renderGrass
                default:
                    break;
            }
        }
    }
}

// Hàm vẽ lớp cỏ (vẽ sau cùng để che xe tăng)
void Map::renderGrass(SDL_Renderer* renderer, SDL_Texture* grassTexture) {
    for (int i = 0; i < MAP_ROWS; i++) { // Duyệt qua từng hàng
        for (int j = 0; j < MAP_COLS; j++) { // Duyệt qua từng cột
            if (grid[i][j] == GRASS) { // Nếu ô là cỏ
                SDL_Rect tileRect = { j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE }; // Tạo hình chữ nhật cho ô cỏ
                SDL_RenderCopy(renderer, grassTexture, nullptr, &tileRect); // Vẽ texture cỏ
            }
        }
    }
}

// Hàm đặt lại bản đồ về trạng thái ban đầu
void Map::reset() {
    grid.clear(); // Xóa toàn bộ dữ liệu hiện tại của grid

    // Định nghĩa lại bản đồ tĩnh giống như trong hàm khởi tạo
    TileType tempMap[MAP_ROWS][MAP_COLS] = {
        {STEEL, STEEL, STEEL, STEEL, STEEL, STEEL, STEEL, STEEL, STEEL, STEEL, STEEL, STEEL, STEEL, STEEL, STEEL, STEEL, STEEL, STEEL, STEEL, STEEL},
        {STEEL, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, GRASS, GRASS, EMPTY, EMPTY, STEEL},
        {STEEL, EMPTY, EMPTY, STEEL, GRASS, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, BRICK, BRICK, BRICK, EMPTY, STEEL},
        {STEEL, EMPTY, STEEL, STEEL, GRASS, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, GRASS, EMPTY, EMPTY, EMPTY, WATER, BRICK, EMPTY, STEEL},
        {STEEL, EMPTY, GRASS, GRASS, WATER, EMPTY, GRASS, BRICK, EMPTY, EMPTY, EMPTY, WATER, BRICK, EMPTY, EMPTY, EMPTY, EMPTY, BRICK, EMPTY, STEEL},
        {STEEL, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, WATER, BRICK, WATER, WATER, BRICK, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, STEEL},
        {STEEL, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, GRASS, BRICK, BRICK, GRASS, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, STEEL},
        {STEEL, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, GRASS, BRICK, BRICK, GRASS, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, STEEL},
        {STEEL, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, GRASS, BRICK, BRICK, GRASS, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, STEEL},
        {STEEL, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, BRICK, WATER, WATER, BRICK, WATER, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, STEEL},
        {STEEL, EMPTY, BRICK, EMPTY, EMPTY, EMPTY, EMPTY, BRICK, WATER, EMPTY, EMPTY, EMPTY, BRICK, GRASS, EMPTY, WATER, GRASS, GRASS, EMPTY, STEEL},
        {STEEL, EMPTY, BRICK, WATER, EMPTY, EMPTY, EMPTY, GRASS, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, GRASS, STEEL, STEEL, EMPTY, STEEL},
        {STEEL, EMPTY, BRICK, BRICK, BRICK, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, GRASS, STEEL, EMPTY, EMPTY, STEEL},
        {STEEL, EMPTY, EMPTY, GRASS, GRASS, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, STEEL},
        {STEEL, STEEL, STEEL, STEEL, STEEL, STEEL, STEEL, STEEL, STEEL, STEEL, STEEL, STEEL, STEEL, STEEL, STEEL, STEEL, STEEL, STEEL, STEEL, STEEL},
    };

    // Sao chép lại bản đồ tĩnh vào grid
    grid.resize(MAP_ROWS, std::vector<TileType>(MAP_COLS, EMPTY)); // Khởi tạo lại grid với kích thước 15x20
    for (int i = 0; i < MAP_ROWS; i++) {
        for (int j = 0; j < MAP_COLS; j++) {
            grid[i][j] = tempMap[i][j]; // Gán giá trị từ tempMap vào grid
        }
    }
}
