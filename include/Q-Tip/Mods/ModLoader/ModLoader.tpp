#pragma once

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