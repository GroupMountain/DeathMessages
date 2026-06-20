#pragma once
#include "Config.h"
#include "Global.h"

#include "ll/api/mod/Mod.h"
#include <ll/api/mod/NativeMod.h>

#include <gmlib/gm/i18n/LangI18n.h>

namespace DeathMessages {

class Entry {

public:
    static Entry& getInstance();

    Entry() : mSelf(*ll::mod::NativeMod::current()) {}

    [[nodiscard]] ll::mod::NativeMod& getSelf() const { return mSelf; }

    /// @return True if the mod is loaded successfully.
    bool load();

    /// @return True if the mod is enabled successfully.
    bool enable();

    /// @return True if the mod is disabled successfully.
    bool disable();

    /// @return True if the mod is unloaded successfully.
    bool unload();

    Config& getConfig();

    gmlib::i18n::LangI18n& getI18n();

    void loadI18n();

    void loadResourcePack();


    bool isResourceI18nLoaded;
private:
    ll::mod::NativeMod&                         mSelf;
    std::optional<Config>                       mConfig;
    std::unique_ptr<gmlib::i18n::LangI18n> mI18n;
};

} // namespace DeathMessages
