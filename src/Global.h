#pragma once
#include <include_all.h>

#define PLUGIN_NAME "DeathMessages"

extern ll::Logger logger;
extern ll::Logger deathLogger;
extern ll::Logger infoLogger;

namespace ConfigData {
extern bool mServerSideLang;
extern bool mJoinMessage;
extern bool mLeftMessage;
extern bool mDeathMessage;
} // namespace ConfigData

extern std::string tr(std::string key, std::vector<std::string> data = {});

extern void initPlugin();
extern void RegisterDamageDefinition();
extern void ListenEvents();