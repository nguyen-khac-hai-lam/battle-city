
#ifndef BULLET_H
#define BULLET_H

#include <SDL2/SDL.h>
#include "Map.h"
#include "Tank.h"

// Lớp Bullet đại diện cho đạn trong game
class Bullet {
public:
    Bullet(int startX, int startY, int dir, Map* map); // Constructor: Khởi tạo đạn
    void render(SDL_Renderer* renderer); // Vẽ đạn lên màn hình
    bool active; // Trạng thái hoạt động của đạn (true nếu còn tồn tại)
    void update(Tank& enemyTank, Tank& playerTank, bool& running); // Cập nhật vị trí và xử lý va chạm
private:
    int x, y;         // Tọa độ của đạn
    int direction;    // Hướng di chuyển (0: lên, 1: xuống, 2: trái, 3: phải)
    int speed;        // Tốc độ di chuyển của đạn
    Map* map;         // Con trỏ tới bản đồ để kiểm tra va chạm
};

#endif // BULLET_H
