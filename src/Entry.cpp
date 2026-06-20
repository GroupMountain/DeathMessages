#include "Entry.h"
#include "Global.h"
#include "Language.h"

#include "ll/api/Config.h"
#include "ll/api/data/IndirectValue.h"
#include "ll/api/io/PatternFormatter.h"
#include "ll/api/io/Logger.h"
#include "ll/api/io/LoggerRegistry.h"
#include "ll/api/io/FileSink.h"
#include "ll/api/mod/Mod.h"
#include "ll/api/mod/ModManagerRegistry.h"
#include "ll/api/mod/RegisterHelper.h"
#include "ll/api/service/Bedrock.h"

#include "gmlib/gm/i18n/LangI18n.h"
#include "gmlib/gm/i18n/ResourceI18n.h"

ll::io::LoggerRegistry&         loggerRegistry = ll::io::LoggerRegistry::getInstance();
std::shared_ptr<ll::io::Logger> logger         = loggerRegistry.getOrCreate(MOD_NAME);
std::shared_ptr<ll::io::Logger> deathLogger    = loggerRegistry.getOrCreate("Death");
std::shared_ptr<ll::io::Logger> infoLogger     = loggerRegistry.getOrCreate("Server");
extern void uninstallHooks();
extern void RegisterDamageDefinition();
extern void uninstallEventListeners();
extern void ListenEvents();

namespace DeathMessages {

Entry& Entry::getInstance() {
    static Entry instance;
    return instance;
}

bool Entry::load() {
        isResourceI18nLoaded=false;

    mConfig.emplace();
    if (!ll::config::loadConfig(*mConfig, getSelf().getConfigDir() / u8"config.json")) {
        ll::config::saveConfig(*mConfig, getSelf().getConfigDir() / u8"config.json");
    }

    if (getConfig().FileLog.Enabled) {
        auto&                                     path = getConfig().FileLog.Path;
        ll::Polymorphic<ll::io::PatternFormatter> polymorphicFormatter =
            ll::makePolymorphic<ll::io::PatternFormatter>("[{3:.3%F %T.} {2}][{1}] {0}", false);
        deathLogger->addSink(std::make_shared<ll::io::FileSink>(path, polymorphicFormatter));
    }
    loadI18n();
    if ((!getConfig().ServerSideTranslation.Enabled)&&ll::mod::ModManagerRegistry::getInstance().hasMod("ModAPI")) {
        loadResourcePack();
    }
    return true;
}

bool Entry::enable() {
    if (!ll::mod::ModManagerRegistry::getInstance().hasMod("ModAPI")) {
        getSelf().getLogger().warn(tr("plugin.deathmessages.modapi.warn"));
    }
    RegisterDamageDefinition();
    ListenEvents();
    return true;
}

bool Entry::disable() { return true; }

bool Entry::unload() {
    mConfig.reset();
    mI18n.reset();
    uninstallHooks();
    loggerRegistry.erase(MOD_NAME);
    loggerRegistry.erase("Death");
    loggerRegistry.erase("Server");
    // getInstance().reset();
    return true;
}

Config& Entry::getConfig() { return mConfig.value(); }

gmlib::i18n::LangI18n& Entry::getI18n() {
    return *mI18n;
}

void Entry::loadI18n() {
    mI18n= std::make_unique<gmlib::i18n::LangI18n>(getSelf().getLangDir(), getConfig().ServerSideTranslation.Language);
    mI18n->updateOrCreateLanguage("en_US", en_US);
    mI18n->updateOrCreateLanguage("zh_CN", zh_CN);
    mI18n->loadAllLanguages();
    mI18n->setDefaultLanguage("zh_CN");
}



void Entry::loadResourcePack() {
    auto mResource = std::make_unique<gmlib::i18n::ResourceI18n>(getSelf().getModDir() / u8"lang", MOD_NAME, 0, 16, 0);
    mResource->addLanguage("en_US", en_US);
    mResource->addLanguage("zh_CN", zh_CN);
    mResource->loadAllLanguages();
    isResourceI18nLoaded=true;
}





} // namespace DeathMessages

LL_REGISTER_MOD(DeathMessages::Entry, DeathMessages::Entry::getInstance());

std::string tr(std::string const& key, std::vector<std::string> const& params) {
    auto& i18n = DeathMessages::Entry::getInstance().getI18n();
        return i18n.get(key, params);


    return key;
}

