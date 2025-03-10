#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

class TextureManager {
public:
    // Load texture từ file ảnh
    static SDL_Texture* LoadTexture(const char* filePath, SDL_Renderer* renderer) {
        SDL_Surface* tempSurface = IMG_Load(filePath);
        if (!tempSurface) {
            std::cout << "Failed to load image: " << filePath << " SDL_Error: " << SDL_GetError() << std::endl;
            return nullptr;
        }
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
        SDL_FreeSurface(tempSurface);
        return texture;
    }
};

#endif // TEXTURE_MANAGER_H

