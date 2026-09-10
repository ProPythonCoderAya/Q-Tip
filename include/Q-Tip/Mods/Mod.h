//
// Created by Ayaan on 2026-09-10.
//

#ifndef QTIP_MODINIT_H
#define QTIP_MODINIT_H
#include <string>
#include <Q-Tip/Config.h>

union SDL_Event;

QTIP_CODE_BEGIN

class Mod {
public:
    virtual ~Mod() = default;

    [[nodiscard]] virtual std::string_view id() const = 0;
    [[nodiscard]] virtual std::string_view name() const = 0;

    virtual void init() {}
    virtual void shutdown() {}

    virtual void handleEvent(const SDL_Event& event) {}
};

QTIP_CODE_END

#endif //QTIP_MODINIT_H
