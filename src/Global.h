#pragma once
#include <ll/api/io/Logger.h>

#define MOD_NAME "DeathMessages"

extern std::shared_ptr<ll::io::Logger> logger;
extern std::shared_ptr<ll::io::Logger> deathLogger;
extern std::shared_ptr<ll::io::Logger> infoLogger;

/*extern std::string tr(std::string const& key, std::vector<std::string> const& data = {});

extern void RegisterDamageDefinition();
extern void ListenEvents();*/