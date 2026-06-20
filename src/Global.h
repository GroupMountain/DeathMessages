#pragma once
#include <ll/api/io/Logger.h>

#include <mc/legacy/ActorUniqueID.h>
#include <mc/world/actor/ActorDamageSource.h>
#include <parallel_hashmap/phmap.h>
#include "ll/api/i18n/I18n.h"

#define MOD_NAME "DeathMessages"

extern std::shared_ptr<ll::io::Logger> logger;
extern std::shared_ptr<ll::io::Logger> deathLogger;
extern std::shared_ptr<ll::io::Logger> infoLogger;
extern phmap::flat_hash_map<int64, float> mFallHeightMap;
extern phmap::flat_hash_map<int64, ActorUniqueID>  mHurtByEntityMap;
extern bool isCrystal;

using DeathMessageResult = std::pair<std::string, std::vector<std::string>>;
using namespace ll::i18n_literals;

extern DeathMessageResult translateDeathMessage(DeathMessageResult origin, std::string name, Actor* dead, ActorDamageSource* ads);
extern void uninstallHooks();
extern void RegisterDamageDefinition();

extern std::string tr(std::string const& key, std::vector<std::string> const& params = {});
extern void ListenEvents();