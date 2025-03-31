#ifndef TANK_H
#define TANK_H

#include <SDL2/SDL.h>
#include "Map.h" // Bao gồm tệp Map.h để truy cập bản đồ và TileType

class Bullet; // Khai báo trước lớp Bullet (sẽ định nghĩa trong Bullet.h)
class Tank {
private:
    int lives = 3; // Số mạng sống của xe tăng, mặc định là 3
    int spawnX, spawnY; // Tọa độ xuất phát ban đầu của xe tăng
    bool invincible; // Trạng thái vô địch (không bị bắn trúng)
    Uint32 invincibleTime; // Thời gian hết vô địch (dựa trên SDL_GetTicks)
    Uint32 lastShotTime; // Thời gian bắn đạn cuối cùng (để giới hạn tần suất bắn)
    int x, y; // Tọa độ hiện tại của xe tăng trên bản đồ
    int width, height; // Chiều rộng và cao của xe tăng (thường bằng TILE_SIZE)
    int move_step; // Bước di chuyển mỗi lần nhấn phím (đơn vị pixel)
    int direction; // Hướng hiện tại của xe tăng (0: lên, 1: xuống, 2: trái, 3: phải)
    SDL_Texture* textureUp; // Texture khi xe tăng hướng lên
    SDL_Texture* textureDown; // Texture khi xe tăng hướng xuống
    SDL_Texture* textureLeft; // Texture khi xe tăng hướng trái
    SDL_Texture* textureRight; // Texture khi xe tăng hướng phải

public:
    std::vector<Bullet> bullets; // Danh sách các viên đạn mà xe tăng bắn ra
    int getX() { return x; } // Lấy tọa độ x của xe tăng
    int getY() { return y; } // Lấy tọa độ y của xe tăng

    // Hàm khởi tạo xe tăng với tọa độ, texture theo hướng, và bước di chuyển
    Tank(int x, int y, SDL_Texture* up, SDL_Texture* down, SDL_Texture* left, SDL_Texture* right, int move_step = 10);

    // Di chuyển xe tăng theo hướng (dx, dy) và kiểm tra va chạm với bản đồ
    void move(int dx, int dy, const std::vector<std::vector<TileType>>& map);

    // Bắn một viên đạn mới, sử dụng thông tin từ bản đồ
    void shoot(Map* map);

    // Cập nhật trạng thái các viên đạn, kiểm tra va chạm với xe tăng địch và chính xe tăng này
    void updateBullets(Tank& enemyTank, Tank& playerTank, bool& running);

    // Vẽ xe tăng và các viên đạn lên màn hình
    void render(SDL_Renderer* renderer);

    // Kiểm tra xem xe tăng có bị trúng đạn tại tọa độ (bulletX, bulletY) không
    bool isHitByBullet(int bulletX, int bulletY);

    // Xử lý khi xe tăng bị phá hủy (trừ mạng hoặc hủy hoàn toàn)
    void destroy();

    // Cập nhật trạng thái xe tăng (ví dụ: kiểm tra hết thời gian vô địch)
    void update();

    bool isDestroyed = false; // Trạng thái xe tăng đã bị phá hủy hoàn toàn hay chưa
    SDL_Texture* winTexture = nullptr; // Texture hiển thị khi xe tăng thắng
    SDL_Texture* heartTexture = nullptr; // Texture trái tim để hiển thị mạng sống
    bool hasWon = false; // Trạng thái xe tăng đã thắng hay chưa

    // Đặt lại xe tăng về trạng thái ban đầu với tọa độ mới
    void reset(int &spawnx, int &spawny);

    // Đặt lại xe tăng về vị trí xuất phát ban đầu (không thay đổi spawnx, spawny)
    void reset2();
};

#include "Bullet.h" // Bao gồm tệp Bullet.h để sử dụng lớp Bullet
#include <vector>   // Bao gồm thư viện vector để lưu danh sách đạn

#endif // TANK_H



