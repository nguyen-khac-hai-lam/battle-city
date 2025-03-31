#include "Tank.h"
#include <algorithm> // Dùng cho std::remove_if để xóa đạn không còn hoạt động

// Hàm khởi tạo xe tăng
Tank::Tank(int x, int y, SDL_Texture* Up, SDL_Texture* Down, SDL_Texture* Left, SDL_Texture* Right, int move_step)
    : x(x), y(y), width(TILE_SIZE), height(TILE_SIZE), // Gán tọa độ ban đầu và kích thước (bằng TILE_SIZE)
      textureUp(Up), textureDown(Down), textureLeft(Left), textureRight(Right) { // Gán các texture theo hướng
    this->move_step = move_step; // Gán bước di chuyển
    direction = 0; // Mặc định hướng lên (0)
    lives = 3; // Mặc định 3 mạng sống
    spawnX = x; // Lưu tọa độ xuất phát ban đầu
    spawnY = y;
    invincible = false; // Ban đầu không vô địch
    invincibleTime = 0; // Thời gian vô địch ban đầu là 0
    lastShotTime = 0; // Thời gian bắn cuối cùng ban đầu là 0
}

// Di chuyển xe tăng
void Tank::move(int dx, int dy, const std::vector<std::vector<TileType>>& map) {
    int newX = x + dx * move_step; // Tính tọa độ mới theo trục x
    int newY = y + dy * move_step; // Tính tọa độ mới theo trục y

    // Cập nhật hướng dựa trên dx, dy
    if (dx == 1) direction = 3; // Phải
    else if (dx == -1) direction = 2; // Trái
    else if (dy == 1) direction = 1; // Xuống
    else if (dy == -1) direction = 0; // Lên

    // Tính toán các góc của xe tăng trên lưới bản đồ (để kiểm tra va chạm)
    int topLeftX = newX / TILE_SIZE; // Góc trên trái (x)
    int topLeftY = newY / TILE_SIZE; // Góc trên trái (y)
    int topRightX = (newX + width - 1) / TILE_SIZE; // Góc trên phải (x)
    int topRightY = newY / TILE_SIZE; // Góc trên phải (y)
    int bottomLeftX = newX / TILE_SIZE; // Góc dưới trái (x)
    int bottomLeftY = (newY + height - 1) / TILE_SIZE; // Góc dưới trái (y)
    int bottomRightX = (newX + width - 1) / TILE_SIZE; // Góc dưới phải (x)
    int bottomRightY = (newY + height - 1) / TILE_SIZE; // Góc dưới phải (y)

    // Kiểm tra xem xe tăng có thể di chuyển không (chỉ di qua ô trống hoặc cỏ)
    bool canMove = (map[topLeftY][topLeftX] == TileType::EMPTY || map[topLeftY][topLeftX] == TileType::GRASS) &&
                   (map[topRightY][topRightX] == TileType::EMPTY || map[topRightY][topRightX] == TileType::GRASS) &&
                   (map[bottomLeftY][bottomLeftX] == TileType::EMPTY || map[bottomLeftY][bottomLeftX] == TileType::GRASS) &&
                   (map[bottomRightY][bottomRightX] == TileType::EMPTY || map[bottomRightY][bottomRightX] == TileType::GRASS);

    if (canMove) { // Nếu không va chạm
        x = newX; // Cập nhật tọa độ x
        y = newY; // Cập nhật tọa độ y
    }
}

// Bắn đạn
void Tank::shoot(Map* map) {
    Uint32 currentTime = SDL_GetTicks(); // Lấy thời gian hiện tại
    if (currentTime - lastShotTime >= 1000) { // Chỉ bắn nếu đã qua 1 giây kể từ lần bắn trước
        int bulletX = x + width / 2; // Đặt đạn ở giữa xe tăng theo trục x
        int bulletY = y + height / 2; // Đặt đạn ở giữa xe tăng theo trục y
        bullets.push_back(Bullet(bulletX, bulletY, direction, map)); // Thêm đạn mới vào danh sách
        lastShotTime = currentTime; // Cập nhật thời gian bắn cuối cùng
    }
}

// Cập nhật trạng thái các viên đạn
void Tank::updateBullets(Tank& enemyTank, Tank& playerTank, bool& running) {
    for (auto& bullet : bullets) { // Duyệt qua từng viên đạn
        bullet.update(enemyTank, playerTank, running); // Cập nhật trạng thái đạn (va chạm, di chuyển)
    }
    // Xóa các viên đạn không còn hoạt động khỏi danh sách
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
        [](Bullet& b) { return !b.active; }), bullets.end());
}

// Vẽ xe tăng và các thành phần liên quan lên màn hình
void Tank::render(SDL_Renderer* renderer) {
    if (isDestroyed) return; // Không vẽ nếu xe tăng đã bị phá hủy hoàn toàn

    if (invincible) { // Nếu đang vô địch
        Uint32 currentTime = SDL_GetTicks();
        if ((currentTime / 200) % 2 == 0) { // Nhấp nháy mỗi 200ms (bật/tắt)
            return; // Không vẽ trong frame này để tạo hiệu ứng nhấp nháy
        }
    }

    SDL_Rect tankRect = { x, y, width, height }; // Tạo hình chữ nhật cho xe tăng
    // Chọn texture phù hợp với hướng hiện tại
    SDL_Texture* currentTexture = (direction == 0) ? textureUp :
                                  (direction == 1) ? textureDown :
                                  (direction == 2) ? textureLeft :
                                  textureRight;
    SDL_RenderCopy(renderer, currentTexture, nullptr, &tankRect); // Vẽ xe tăng

    if (heartTexture) { // Nếu có texture trái tim (mạng sống)
        const int HEART_SIZE = 16; // Kích thước trái tim
        const int HEART_SPACING = 2; // Khoảng cách giữa các trái tim
        for (int i = 0; i < lives; i++) { // Vẽ số trái tim tương ứng với mạng sống
            SDL_Rect heartRect = { x + i * (HEART_SIZE + HEART_SPACING), y - HEART_SIZE - 5 - 10, HEART_SIZE, HEART_SIZE };
            SDL_RenderCopy(renderer, heartTexture, nullptr, &heartRect); // Vẽ trái tim
        }
    }

    if (hasWon) { // Nếu xe tăng thắng
        SDL_Rect winRect = { x, y - 40, 40, 24 }; // Vị trí hiển thị texture chiến thắng
        SDL_RenderCopy(renderer, winTexture, nullptr, &winRect); // Vẽ texture "Win"
    }

    for (auto& bullet : bullets) { // Vẽ từng viên đạn
        bullet.render(renderer);
    }
}

// Kiểm tra xem xe tăng có bị trúng đạn không
bool Tank::isHitByBullet(int bulletX, int bulletY) {
    if (isDestroyed || invincible) return false; // Không trúng nếu đã bị phá hủy hoặc đang vô địch
    SDL_Rect tankRect = { x, y, width, height }; // Hình chữ nhật của xe tăng
    SDL_Rect bulletRect = { bulletX, bulletY, 5, 5 }; // Hình chữ nhật của đạn (kích thước 5x5)
    return SDL_HasIntersection(&tankRect, &bulletRect); // Trả về true nếu đạn giao với xe tăng
}

// Xử lý khi xe tăng bị phá hủy
void Tank::destroy() {
    if (invincible) return; // Không làm gì nếu đang vô địch
    lives--; // Giảm 1 mạng
    if (lives > 0) { // Nếu vẫn còn mạng
        invincible = true; // Kích hoạt trạng thái vô địch
        invincibleTime = SDL_GetTicks() + 3000; // Vô địch trong 3 giây
        reset2(); // Đặt lại vị trí về điểm xuất phát
    } else { // Nếu hết mạng
        isDestroyed = true; // Đánh dấu xe tăng bị phá hủy hoàn toàn
    }
}

// Cập nhật trạng thái xe tăng
void Tank::update() {
    if (invincible) { // Nếu đang vô địch
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime >= invincibleTime) { // Kiểm tra xem đã hết thời gian vô địch chưa
            invincible = false; // Tắt trạng thái vô địch
        }
    }
}

// Đặt lại xe tăng về trạng thái ban đầu với tọa độ mới
void Tank::reset(int &spawnx, int &spawny) {
    isDestroyed = false; // Đặt lại trạng thái phá hủy
    hasWon = false; // Đặt lại trạng thái thắng
    x = spawnx; // Đặt tọa độ x mới
    y = spawny; // Đặt tọa độ y mới
    lives = 3; // Khôi phục 3 mạng
    lastShotTime = 0; // Đặt lại thời gian bắn
}

// Đặt lại xe tăng về vị trí xuất phát ban đầu
void Tank::reset2() {
    isDestroyed = false; // Đặt lại trạng thái phá hủy
    hasWon = false; // Đặt lại trạng thái thắng
    x = spawnX; // Quay về tọa độ x ban đầu
    y = spawnY; // Quay về tọa độ y ban đầu
}
