//
// Created by Ayaan on 2026-09-10.
//

#include "Q-Tip/Mods/ModLoader/ModLoader.h"

#include <SDL3/SDL_events.h>

#include "Q-Tip/Mods/Mod.h"

QTIP_CODE_BEGIN

ModLoader ModLoader::instance;

template<std::derived_from<Mod> M>
void ModLoader::load() {
    for (const auto& mod : instance.mods) {
        if (typeid(*mod) == typeid(M)) {
            return; // Already loaded
        }
    }

    auto mod = std::make_unique<M>();

    mod->init();

    instance.mods.push_back(std::move(mod));
}

template <std::derived_from<Mod> M>
M* ModLoader::mod() {
    for (const auto& mod : instance.mods) {
        if (auto* result = dynamic_cast<M*>(mod.get())) {
            return result;
        }
    }

    return nullptr;
}

ModLoader::~ModLoader() {
    for (const auto& mod : mods) {
        mod->shutdown();
    }
}

void ModLoader::handleEvent(const SDL_Event& event) {
    for (const auto& mod : instance.mods) {
        mod->handleEvent(event);
    }
}

QTIP_CODE_END
