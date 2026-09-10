//
// Created by Ayaan on 2026-09-10.
//

#ifndef QTIP_MODLOADER_H
#define QTIP_MODLOADER_H
#include <concepts>
#include <Q-Tip/Config.h>
#include <vector>
#include <memory>

#include "Q-Tip/Mods/Mod.h"

union SDL_Event;

QTIP_CODE_BEGIN

class ModLoader {
public:
    ModLoader(const ModLoader&) = delete;
    ModLoader& operator=(const ModLoader&) = delete;

    ModLoader(ModLoader&&) = delete;
    ModLoader& operator=(ModLoader&&) = delete;

    template<std::derived_from<Mod> M>
    static void load();

    template<std::derived_from<Mod> M>
    static M* mod();

private:
    ModLoader() = default;

    ~ModLoader();

    std::vector<std::unique_ptr<Mod>> mods;

    static void handleEvent(const SDL_Event& event);

    static ModLoader instance;

    friend class Window;
};

#include "ModLoader.tpp"

QTIP_CODE_END

#endif //QTIP_MODLOADER_H
