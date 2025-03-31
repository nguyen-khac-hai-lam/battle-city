#ifndef TEXTURE_MANAGER_H // Bảo vệ chống bao gồm lặp (include guard)
#define TEXTURE_MANAGER_H

#include <SDL2/SDL.h> // Bao gồm thư viện SDL để sử dụng SDL_Surface và SDL_Texture
#include <SDL2/SDL_image.h> // Bao gồm SDL_image để tải hình ảnh từ tệp
#include <iostream> // Bao gồm thư viện để in thông báo lỗi qua std::cout

class TextureManager {
public:
    // Phương thức tĩnh để tải texture từ tệp ảnh
    // - filePath: Đường dẫn đến tệp ảnh (ví dụ: "tankup.png")
    // - renderer: Con trỏ tới renderer của SDL để tạo texture
    static SDL_Texture* LoadTexture(const char* filePath, SDL_Renderer* renderer) {
        SDL_Surface* tempSurface = IMG_Load(filePath); // Tải ảnh từ tệp vào một bề mặt tạm (surface)
        if (!tempSurface) { // Kiểm tra xem ảnh có tải thành công không
            std::cout << "Không thể tải ảnh: " << filePath << " SDL_Error: " << SDL_GetError() << std::endl; // In thông báo lỗi nếu thất bại
            return nullptr; // Trả về nullptr nếu không tải được ảnh
        }
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, tempSurface); // Chuyển bề mặt thành texture để sử dụng với renderer
        SDL_FreeSurface(tempSurface); // Giải phóng bề mặt tạm sau khi tạo texture
        return texture; // Trả về texture đã tạo (hoặc nullptr nếu thất bại)
    }
};

#endif // TEXTURE_MANAGER_H // Kết thúc include guard

