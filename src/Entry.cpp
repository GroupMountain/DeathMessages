#include "Entry.h"
#include "Global.h"
#include "Language.h"
#include "ll/api/mod/Mod.h"
#include "ll/api/Config.h"
#include "ll/api/io/PatternFormatter.h"
#include "ll/api/io/Logger.h"
#include "ll/api/io/LoggerRegistry.h"
#include "ll/api/io/FileSink.h"
#include "ll/api/mod/RegisterHelper.h"
#include "ll/api/data/IndirectValue.h"
#include "gmlib/gm/i18n/LangI18n.h"
#include "gmlib/gm/i18n/ResourceI18n.h"

ll::io::LoggerRegistry&         loggerRegistry = ll::io::LoggerRegistry::getInstance();
std::shared_ptr<ll::io::Logger> logger         = loggerRegistry.getOrCreate(MOD_NAME);
std::shared_ptr<ll::io::Logger> deathLogger    = loggerRegistry.getOrCreate("Death");
std::shared_ptr<ll::io::Logger> infoLogger     = loggerRegistry.getOrCreate("Server");


namespace DeathMessages {

Entry& Entry::getInstance() {
    static Entry instance;
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
        auto&                                     path = getConfig().FileLog.Path;
        ll::Polymorphic<ll::io::PatternFormatter> polymorphicFormatter =
            ll::makePolymorphic<ll::io::PatternFormatter>("[{3:.3%F %T.} {2}][{1}] {0}", false);
        deathLogger->addSink(std::make_shared<ll::io::FileSink>(path, polymorphicFormatter));
    }
    return true;
}

bool Entry::enable() {
        RegisterDamageDefinition();
    /*    ListenEvents();*/
    return true;
}

bool Entry::disable() { return true; }

bool Entry::unload() {
    mConfig.reset();
    mI18n.reset();
    // getInstance().reset();
    return true;
}

Config& Entry::getConfig() { return mConfig.value(); }

std::unique_ptr<gmlib::i18n::LangI18n> Entry::getI18n() { return std::move(mI18n); }

void Entry::loadI18n() {
    mI18n= std::make_unique<gmlib::i18n::LangI18n>(getSelf().getLangDir(), getConfig().ServerSideTranslation.Language);
    mI18n->updateOrCreateLanguage("en_US", en_US);
    mI18n->updateOrCreateLanguage("zh_CN", zh_CN);
    mI18n->loadAllLanguages();
    mI18n->setDefaultLanguage("zh_CN");
}

void Entry::loadResourcePack() {
    auto resource = gmlib::i18n::ResourceI18n{getSelf().getModDir() / u8"lang", MOD_NAME, 0, 8, 0};
    resource.addLanguage("en_US", en_US);
    resource.addLanguage("zh_CN", zh_CN);
    resource.loadAllLanguages();
}

} // namespace DeathMessages

LL_REGISTER_MOD(DeathMessages::Entry, DeathMessages::Entry::getInstance());

/*std::string tr(std::string const& key, std::vector<std::string> const& params) {
    if (auto i18n = DeathMessages::Entry::getInstance().getI18n()) {
        return i18n->get(key, params);
    }
    auto i18n = DeathMessages::Entry::getI18n();

        return i18n->get(key, params);


}*/