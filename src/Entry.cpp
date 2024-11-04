#include "Entry.h"
#include "Global.h"
#include "Language.h"

ll::Logger logger(MOD_NAME);
ll::Logger deathLogger("Death");
ll::Logger infoLogger("Server");

namespace DeathMessages {

std::unique_ptr<Entry>& Entry::getInstance() {
    static std::unique_ptr<Entry> instance;
    return instance;
}

bool Entry::load() {
    mConfig.emplace();
    if (!ll::config::loadConfig(*mConfig, getSelf().getConfigDir() / u8"config.json")) {
        ll::config::saveConfig(*mConfig, getSelf().getConfigDir() / u8"config.json");
    }
    if (getConfig().ServerSideTranslation.Enabled) {
        loadI18n();
    } else {
        loadResourcePack();
    }
    if (getConfig().FileLog.Enabled) {
        auto& path = getConfig().FileLog.Path;
        deathLogger.setFile(path);
    }
    return true;
}

bool Entry::enable() {
    RegisterDamageDefinition();
    ListenEvents();
    return true;
}

bool Entry::disable() { return true; }

bool Entry::unload() {
    mConfig.reset();
    mI18n.reset();
    getInstance().reset();
    return true;
}

Config& Entry::getConfig() { return mConfig.value(); }

std::optional<gmlib::i18n::LangI18n> Entry::getI18n() { return mI18n; }

void Entry::loadI18n() {
    mI18n.emplace(getSelf().getLangDir(), getConfig().ServerSideTranslation.Language);
    mI18n->updateOrCreateLanguage("en_US", en_US);
    mI18n->updateOrCreateLanguage("zh_CN", zh_CN);
    mI18n->loadAllLanguages();
    mI18n->setDefaultLanguage("zh_CN");
}

void Entry::loadResourcePack() {
    gmlib::tools::VanillaFix::setFixI18nEnabled();
    auto resource = gmlib::i18n::ResourceI18n(getSelf().getModDir() / u8"resource", MOD_NAME, 0, 8, 0);
    resource.addLanguage("en_US", en_US);
    resource.addLanguage("zh_CN", zh_CN);
}

} // namespace DeathMessages

LL_REGISTER_MOD(DeathMessages::Entry, DeathMessages::Entry::getInstance());

std::string tr(std::string const& key, std::vector<std::string> const& params) {
    if (auto i18n = DeathMessages::Entry::getInstance()->getI18n()) {
        return i18n->get(key, params);
    }
    return gmlib::world::I18nAPI::get(key, params);
}