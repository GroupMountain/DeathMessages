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
#include "ll/api/io/FileUtils.h"
#include <filesystem>

#include "gmlib/gm/i18n/LangI18n.h"
#include "gmlib/gm/i18n/ResourceI18n.h"
#include "gmlib/mc/locale/I18nAPI.h"

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
    if (!ll::file_utils::readFile(getSelf().getLangDir() / u8"en_US.lang").has_value()) {
        ll::file_utils::writeFile(getSelf().getLangDir() / u8"en_US.lang", en_US);
    }
    if (!ll::file_utils::readFile(getSelf().getLangDir() / u8"zh_CN.lang").has_value()) {
        ll::file_utils::writeFile(getSelf().getLangDir() / u8"zh_CN.lang", zh_CN);
    }
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
    if ((!getConfig().ServerSideTranslation.Enabled)&&ll::mod::ModManagerRegistry::getInstance().hasMod("ModAPI")) {
        loadResourcePack();
    }else {
        loadI18n();

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
    const std::string custom_zh_CN = ll::file_utils::readFile(
        getSelf().getLangDir() / u8"zh_CN.lang",
        false
        )
    .value_or(zh_CN);
    const std::string custom_en_US = ll::file_utils::readFile(
        getSelf().getLangDir() / u8"en_US.lang",
        false
        )
    .value_or(en_US);

    mI18n= std::make_unique<gmlib::i18n::LangI18n>(getSelf().getLangDir(), getConfig().ServerSideTranslation.Language);
    mI18n->updateOrCreateLanguage("en_US", custom_en_US);
    mI18n->updateOrCreateLanguage("zh_CN", custom_zh_CN);
    mI18n->loadAllLanguages();
    mI18n->setDefaultLanguage(getConfig().ServerSideTranslation.Language);
}



void Entry::loadResourcePack() {
    if (
        ll::file_utils::readFile(getSelf().getLangDir() / u8"en_US.lang")!=
        ll::file_utils::readFile(getSelf().getModDir() / u8"resource/language_pack/texts/en_US.lang")
        ||ll::file_utils::readFile(getSelf().getLangDir() / u8"zh_CN.lang")!=
        ll::file_utils::readFile(getSelf().getModDir() / u8"resource/language_pack/texts/zh_CN.lang")
        )
        std::filesystem::remove_all(getSelf().getModDir() / u8"resource");

    auto mResource = std::make_unique<gmlib::i18n::ResourceI18n>(getSelf().getModDir() / u8"resource", MOD_NAME, 0, 16, 1);
    mResource->addLanguageFromPath("zh_CN",getSelf().getLangDir() / u8"zh_CN.lang");
    mResource->addLanguageFromPath("en_US",getSelf().getLangDir() / u8"en_US.lang");
    mResource->loadAllLanguages();
    isResourceI18nLoaded=true;
}





} // namespace DeathMessages

LL_REGISTER_MOD(DeathMessages::Entry, DeathMessages::Entry::getInstance());

std::string tr(std::string const& key, std::vector<std::string> params) {
    if (DeathMessages::Entry::getInstance().isResourceI18nLoaded) {
        return gmlib::I18nAPI::get(key, params);
    }
    auto& i18n = DeathMessages::Entry::getInstance().getI18n();
    logger->info(DeathMessages::Entry::getInstance().isResourceI18nLoaded ? "ResourceI18nLoaded" : "LangI18nLoaded");
    if (params.size() > 1)
        params.at(1) = gmlib::I18nAPI::get(
            params.at(1),
            {},
            DeathMessages::Entry::getInstance().getConfig().ServerSideTranslation.Language
        );
    return i18n.get(key, params);
    return key;
}

