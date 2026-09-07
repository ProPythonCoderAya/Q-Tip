//
// Created by Ayaan on 2026-09-07.
//

#ifndef QTIP_RENDERTEXTURE_H
#define QTIP_RENDERTEXTURE_H
#include <Q-Tip/Config.h>

#include "Texture.h"

QTIP_CODE_BEGIN

class RenderTarget : protected Texture {
public:
    RenderTarget(Renderer& renderer, int width, int height);
};

QTIP_CODE_END

#endif //QTIP_RENDERTEXTURE_H
