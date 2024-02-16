#include "Config.h"
#include "Global.h"
#include "Language.h"

ll::Logger deathLogger("Death");
ll::Logger infoLogger("Server");

GMLIB::Files::JsonConfig*     Config                 = nullptr;
GMLIB::Files::I18n::LangI18n* Language               = nullptr;
int                           commandPermissionLevel = 4;

namespace ConfigData {
bool mServerSideLang = false;
bool mJoinMessage    = false;
bool mLeftMessage    = false;
bool mDeathMessage   = false;
} // namespace ConfigData


void initServerSideTranslation() {
    std::string langPath     = "./plugins/DeathMessages/language/";
    std::string languageCode = Config->getValue<std::string>({"ServerSideTranslation", "Language"}, "en_US");
    Language                 = new GMLIB::Files::I18n::LangI18n(langPath);
    Language->updateOrCreateLanguage("en_US", en_US);
    Language->updateOrCreateLanguage("zh_CN", zh_CN);
    Language->loadAllLanguages();
    Language->chooseLanguage(languageCode);
}

void initResourcePackTranslation() {
    GMLIB::Mod::VanillaFix::setFixI18nEnabled();
    std::string resourcePath = "./plugins/DeathMessages/resource/";
    auto        resource     = GMLIB::Files::ResourceLanguage::ResourceLanguage(resourcePath, PLUGIN_NAME, 0, 8, 0);
    resource.addLanguage("en_US", en_US);
    resource.addLanguage("zh_CN", zh_CN);
    resource.initLanguage();
}

std::string tr(std::string key, std::vector<std::string> data) {
    if (ConfigData::mServerSideLang) {
        for (auto& info : data) {
            ll::utils::string_utils::replaceAll(info, "%", "");
            info = Language->translate(info);
        }
        return Language->translate(key, data);
    }
    return I18n::get(key, data);
}

void initConfig() {
    Config = new GMLIB::Files::JsonConfig("./plugins/DeathMessages/config/config.json", defaultConfig);
    Config->init();
    ConfigData::mDeathMessage   = Config->getValue<bool>({"ConsoleLog", "DeathMessage"}, true);
    ConfigData::mJoinMessage    = Config->getValue<bool>({"ConsoleLog", "JoinMessage"}, true);
    ConfigData::mLeftMessage    = Config->getValue<bool>({"ConsoleLog", "LeftMessage"}, true);
    ConfigData::mServerSideLang = Config->getValue<bool>({"ServerSideTranslation", "Enabled"}, true);
}

void initPlugin() {
    initConfig();
    if (ConfigData::mServerSideLang) {
        initServerSideTranslation();
    } else {
        initResourcePackTranslation();
    }
    if (Config->getValue<bool>({"FileLog", "Enabled"}, false)) {
        auto path = Config->getValue<std::string>({"FileLog", "Path"}, "./logs/DeathLog.log");
        deathLogger.setFile(path);
    }
}
