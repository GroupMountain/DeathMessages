#pragma once
#include <headers/include_all.h>

#define MOD_NAME "DeathMessages"

extern ll::Logger logger;
extern ll::Logger deathLogger;
extern ll::Logger infoLogger;

extern std::string tr(std::string const& key, std::vector<std::string> const& data = {});

extern void RegisterDamageDefinition();
extern void ListenEvents();