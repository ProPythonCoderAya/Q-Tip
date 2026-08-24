//
// Created by Ayaan on 2026-08-24.
//

#ifndef QTIP_TEXTURE_H
#define QTIP_TEXTURE_H

#include <filesystem>
#include <Q-Tip/Config.h>
#include <Q-Tip/Graphics/Renderer.h>
#include <SDL3_image/SDL_image.h>

QTIP_CODE_BEGIN

namespace fs = std::filesystem;

class Texture {
public:
    Texture(const Renderer& renderer, const fs::path& path);
    ~Texture();

    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    Texture(Texture&& other) noexcept;
    Texture& operator=(Texture&& other) noexcept;

    void destroy();

    [[nodiscard]] bool isValid() const;
    [[nodiscard]] SDL_Texture* nativeHandle() const;

private:
    SDL_Texture* _texture = nullptr;
};

QTIP_CODE_END

#endif //QTIP_TEXTURE_H
