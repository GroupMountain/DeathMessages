#include "Entry.h"
#include "ll/api/event/player/PlayerConnectEvent.h"
#include "ll/api/event/player/PlayerDisconnectEvent.h"
#include "ll/api/memory/Hook.h"
#include "ll/api/chrono/GameChrono.h"
#include "ll/api/thread/ThreadPoolExecutor.h"
#include <chrono>
#include "mc/world/actor/Mob.h"
#include "mc/world/actor/ActorDamageSource.h"
#include "mc/world/actor/ActorDamageByActorSource.h"
#include "mc/world/actor/ActorDamageByBlockSource.h"
#include "mc/world/actor/ActorDamageByChildActorSource.h"
#include "ll/api/service/Bedrock.h"
#include "mc/world/level/Level.h"


#include "mc/world/actor/Actor.h"
phmap::flat_hash_map<int64, float>            mFallHeightMap;
phmap::flat_hash_map<int64, ActorUniqueID>  mHurtByEntityMap;

LL_AUTO_TYPE_INSTANCE_HOOK(
    MobDieHook,
    HookPriority::Normal,
    Mob,
    &Mob::$die,
    void,
    ActorDamageSource const& ads
) {
    auto uid = getOrCreateUniqueID();

    if (ads.mCause == SharedTypes::Legacy::ActorDamageCause::Fall) {
        mFallHeightMap[uid.rawID] = getFallDistance();
        logger->info("跌落高度为 {}", getFallDistance());
    }
    if (hasCategory(ActorCategory::Player) || isTame()) {
        if (mLastHurtByMobTime != 0) {
            auto hm = getLastHurtByMob();
            mHurtByEntityMap[uid.rawID] = hm->getOrCreateUniqueID();
            if (!getLastHurtByMob()->getNameTag().empty()) logger->info("伤害来源为 {}", getLastHurtByMob()->getNameTag());
            logger->info("伤害来源为 {}", getLastHurtByMob()->getTypeName());


            if (!getLastHurtByMob()->getCarriedItem().getCustomName().empty()) logger->info("伤害来源为 {}", getLastHurtByMob()->getCarriedItem().getCustomName());
        }
    }
    origin(ads);

    auto& executor = ll::thread::ThreadPoolExecutor::getDefault();
    auto callback = executor.executeAfter(
        [] {
            mHurtByEntityMap.clear();
            mFallHeightMap.clear();
        },
        ll::chrono::game::ticks(1)
    );
}
using DeathMessageResult = std::pair<std::string, std::vector<std::string>>;

int                                         mMaxCauseId = 34;
std::vector<std::pair<std::string, int>>    mCustomCauseMap;
phmap::flat_hash_map<int, std::string_view> mVanillaCauseMessage;

phmap::flat_hash_map<SharedTypes::Legacy::ActorDamageCause, phmap::flat_hash_map<std::string_view, std::string_view>> mHardCodedDeathMessage;

bool isCrystal = false;

bool isCustomDefinition(int cause) {
    if (cause >= 35) {
        return true;
    }
    if (mVanillaCauseMessage.count(cause)) {
        return true;
    }
    return false;
}

bool isCustomCauseExist(std::string const& causeName) {
    for (auto& key : mCustomCauseMap) {
        if (key.first == causeName) {
            return true;
        }
    }
    return false;
}

bool isCustomCauseExist(int cause) {
    for (auto& key : mCustomCauseMap) {
        if (key.second == cause) {
            return true;
        }
    }
    return false;
}

SharedTypes::Legacy::ActorDamageCause getCustomCause(std::string const& causeName) {
    for (auto& key : mCustomCauseMap) {
        if (key.first == causeName) {
            return (SharedTypes::Legacy::ActorDamageCause)key.second;
        }
    }
    return SharedTypes::Legacy::ActorDamageCause::None;
}

std::string getCustomCause(SharedTypes::Legacy::ActorDamageCause cause) {
    for (auto& key : mCustomCauseMap) {
        if ((SharedTypes::Legacy::ActorDamageCause)key.second == cause) {
            return key.first;
        }
    }
    return "none";
}

std::string getCause(SharedTypes::Legacy::ActorDamageCause cause) {
    auto id = (int)cause;
    if (isCustomCauseExist(id)) {
        return getCustomCause(cause);
    }
    if (mVanillaCauseMessage.count(id)) {
        return std::string(mVanillaCauseMessage[id]);
    }
    return "";
}



Actor* getDamagingEntity(ActorDamageSource* ads) {
    auto id = ads->getDamagingEntityUniqueID();
    return ll::service::getLevel()->fetchEntity(id,true);
}

// %entity.warden.name
std::string getResourcePackKey(std::string const& name, Actor* ac) {
    if (ac->hasCategory(ActorCategory::Player)) {
        return name;
    }
    auto type = ac->getTypeName();
    ll::utils::string_utils::replaceAll(type, "minecraft:", "");
    std::string res = "%entity." + type + ".name";
    return res;
}

// 构造死亡信息输出
DeathMessageResult makeDeathMessage(
    int            cause,
    DeathMessageResult& deathMessage,
    std::string&   name,
    Actor*         killer,
    std::string&   weaponName,
    bool           isEscaping,
    bool           isHardCodedMessage = false
) {
    // 自定义类型构造
    if (isCustomDefinition(cause) || isHardCodedMessage) {
        std::string msg = "death.attack.damageCause.item";
        ll::utils::string_utils::replaceAll(msg, "damageCause", getCause(SharedTypes::Legacy::ActorDamageCause(cause)));
        if (isHardCodedMessage) {
            msg = deathMessage.first;
        }
        // 没有武器名不使用.item结尾
        if (weaponName.empty()) {
            // 试图逃离 xx 使用.player
            if (killer) {
                if (isEscaping) {
                    ll::utils::string_utils::replaceAll(msg, ".item", ".player");
                }
            } else {
                ll::utils::string_utils::replaceAll(msg, ".item", "");
            }
        }
        DeathMessageResult res = {msg, {name}};
        if (killer) {
            if (killer->isPlayer()) {
                res.second.push_back(((Player*)killer)->getRealName());
            } else {
                res.second.push_back(getResourcePackKey(name, killer));
            }
            auto weaponname = killer->getCarriedItem().getCustomName();
            if (!weaponname.empty()) {
                weaponname = "[" + weaponname + "§r]";
                res.second.push_back(weaponname);
            } else {
                ll::utils::string_utils::replaceAll(res.first, ".item", "");
            }
        } else {
            // 如果没有击杀者和试图逃离，那么不可能使用 .player 结尾
            ll::utils::string_utils::replaceAll(res.first, ".player", "");
        }
        return res;
    }
    // 未定义返回vanilla值
    return deathMessage;
}

DeathMessageResult tryTranslateHardCodedFallingDeathMessage(
    DeathMessageResult& deathMessage,
    std::string&   name,
    Actor*         killer,
    std::string&   directKiller,
    std::string&   weaponName,
    bool           isEscaping,
    int64          deadUniqueId
) {
    if (!directKiller.empty()) {
        deathMessage.first = "death.fell.finish.item";
    }
    // 低处
    else if (deathMessage.first == "death.attack.fall") {
        deathMessage.first = "death.attack.fall.item";
    }
    // 高处
    else if (deathMessage.first == "death.fell.accident.generic") {
        // 试图逃离而亡
        if (killer) {
            std::string weapon = killer->getCarriedItem().getCustomName();
            if (!weapon.empty()) {
                deathMessage.first = "death.fell.assist.item";
            } else {
                deathMessage.first = "death.fell.assist";
            }
        }
        // 超高
        else if (mFallHeightMap[deadUniqueId] >= 100) {
            deathMessage.first = "death.fell.killer";
        } else {
            deathMessage.first = "death.fell.accident.generic";
        }
    } else {
        deathMessage.first = "death.attack.generic";
    }
    return makeDeathMessage((int)SharedTypes::Legacy::ActorDamageCause::Fall, deathMessage, name, killer, weaponName, isEscaping, true);
}

std::optional<DeathMessageResult> tryTranslateHardCodedDeathMessage(
    SharedTypes::Legacy::ActorDamageCause cause,
    DeathMessageResult&   deathMessage,
    std::string&     name,
    Actor*           killer,
    std::string&     directKiller,
    std::string&     weaponName,
    bool             isEscaping,
    int64            deadUniqueId
) {
    switch (cause) {
    // 硬编码摔死信息
    case SharedTypes::Legacy::ActorDamageCause::Fall: {
        return tryTranslateHardCodedFallingDeathMessage(
            deathMessage,
            name,
            killer,
            directKiller,
            weaponName,
            isEscaping,
            deadUniqueId
        );
    }
    // 硬编码爆炸信息
    case SharedTypes::Legacy::ActorDamageCause::BlockExplosion: {
        if (!isCrystal) {
            deathMessage.first = "death.attack.badRespawnPoint.message";
        } else {
            deathMessage.first = "death.attack.explosion.item";
        }
        return makeDeathMessage((int)cause, deathMessage, name, killer, weaponName, isEscaping, true);
    }
    case SharedTypes::Legacy::ActorDamageCause::Contact: {
        switch (ll::hash_utils::doHash(deathMessage.first)) {
        case ll::hash_utils::doHash("death.attack.cactus"):
            deathMessage.first = "death.attack.cactus.item";
            break;
        case ll::hash_utils::doHash("death.attack.sweetBerry"):
            deathMessage.first = "death.attack.sweetBerry.item";
            break;
        default:
            deathMessage.first = "death.attack.generic.item";
            break;
        }
        return makeDeathMessage((int)cause, deathMessage, name, killer, weaponName, isEscaping, true);
    }
    }
    if (mHardCodedDeathMessage.contains(cause)) {
        if (directKiller.empty()) {
            directKiller = "empty";
        }
        if (mHardCodedDeathMessage[cause].contains(directKiller)) {
            deathMessage.first = std::string(mHardCodedDeathMessage[cause][directKiller]);
            return makeDeathMessage((int)cause, deathMessage, name, killer, weaponName, isEscaping, true);
        } else if (mHardCodedDeathMessage[cause].contains("all")) {
            deathMessage.first = std::string(mHardCodedDeathMessage[cause]["all"]);
            return makeDeathMessage((int)cause, deathMessage, name, killer, weaponName, isEscaping, true);
        }
    }
    return {};
}

DeathMessageResult translateDeathMessage(DeathMessageResult origin, std::string name, Actor* dead, ActorDamageSource* ads) {
    name = getResourcePackKey(name, dead);
    if (origin.first.length() > 65565) {
        return {"death.attack.even_more_magic", {name}};
    }
    auto        damageCause = ads->mCause; // 伤害类型
    Actor*      killer      = nullptr;         // 击杀者
    std::string directKiller;                  // 直接击杀实体
    std::string weaponName;                    // 是否持有武器
    bool        isEscaping = false;
    int64       uid        = dead->getOrCreateUniqueID().rawID;
    if (ads->isEntitySource()) {
        directKiller = getDamagingEntity(ads)->getTypeName();
        if (ads->isChildEntitySource()) {
            killer = getDamagingEntity(ads)->getOwner();
        } else {
            killer = getDamagingEntity(ads);
        }
    }
    if (killer == nullptr && mHurtByEntityMap.count(uid)) {
        killer     = ll::service::getLevel()->fetchEntity(mHurtByEntityMap[uid],true);
        isEscaping = true;
    }
    if (killer != nullptr) {
        weaponName = killer->getCarriedItem().getCustomName();
    }
    // Vanilla的硬编码死亡信息类型
    auto result =
        tryTranslateHardCodedDeathMessage(damageCause, origin, name, killer, directKiller, weaponName, isEscaping, uid);
    if (result.has_value()) {
        return result.value();
    }
    return makeDeathMessage((int)damageCause, origin, name, killer, weaponName, isEscaping);
}

LL_AUTO_TYPE_INSTANCE_HOOK(
    DeathMessageHook1,
    HookPriority::Lowest,
    ActorDamageSource,
    &ActorDamageSource::$getDeathMessage,


    DeathMessageResult,

    std::string a1,
    Actor*      a2
) {



    auto originl = origin(a1, a2);
    logger->info("原消息name为 {}", a1);
    logger->info("原消息为 {}", originl.first);
    auto res     = translateDeathMessage(originl, a1, a2, this);
    return res;
}

LL_AUTO_TYPE_INSTANCE_HOOK(
    DeathMessageHook2,
    HookPriority::Lowest,
    ActorDamageByActorSource,
    &ActorDamageByActorSource::$getDeathMessage,
    DeathMessageResult,
    std::string a1,
    Actor*      a2
) {
    auto originl = origin(a1, a2);
    logger->info("原消息name为 {}", a1);
    logger->info("原消息为 {}", originl.first);
    auto res     = translateDeathMessage(originl, a1, a2, this);
    return res;
}

LL_AUTO_TYPE_INSTANCE_HOOK(
    DeathMessageHook3,
    HookPriority::Lowest,
    ActorDamageByBlockSource,
    &ActorDamageByBlockSource::$getDeathMessage,

    DeathMessageResult,
    std::string a1,
    Actor*      a2
) {
    auto originl = origin(a1, a2);
    logger->info("原消息name为 {}", a1);
    logger->info("原消息为 {}", originl.first);
    auto res     = translateDeathMessage(originl, a1, a2, this);
    return res;
}

LL_AUTO_TYPE_INSTANCE_HOOK(
    DeathMessageHook4,
    HookPriority::Lowest,
    ActorDamageByChildActorSource,
    &ActorDamageByChildActorSource::$getDeathMessage,


    DeathMessageResult,
    std::string a1,
    Actor*      a2
) {
    auto originl = origin(a1, a2);
    logger->info("原消息name为 {}", a1);
    logger->info("原消息为 {}", originl.first);
    auto res     = translateDeathMessage(originl, a1, a2, this);
    return res;
}
int getNextCauseId() {
    mMaxCauseId++;
    return mMaxCauseId;
}

bool registerDamageCause(std::string const& causeName) {
    if (!isCustomCauseExist(causeName)) {
        auto id = getNextCauseId();
        mCustomCauseMap.push_back({causeName, id});
        return true;
    }
    return false;
}

bool setVanillaCauseMessage(SharedTypes::Legacy::ActorDamageCause cause, std::string_view msg) {
    auto id = (int)cause;
    if (!mVanillaCauseMessage.count(id)) {
        mVanillaCauseMessage[id] = msg;
        return true;
    }
    return false;
}

bool registerCustomDeathMessage(
    SharedTypes::Legacy::ActorDamageCause cause,
    std::string_view message,
    std::string_view killerType
) {
    if (mHardCodedDeathMessage.contains(cause)) {
        if (mHardCodedDeathMessage[cause].contains(killerType)) {
            return false;
        }
    }
    mHardCodedDeathMessage[cause][killerType] = message;
    return true;
}
void RegisterDamageDefinition() {
    setVanillaCauseMessage(SharedTypes::Legacy::ActorDamageCause::Anvil, "anvil");
    setVanillaCauseMessage(SharedTypes::Legacy::ActorDamageCause::EntityAttack, "mob");
    setVanillaCauseMessage(SharedTypes::Legacy::ActorDamageCause::RamAttack, "mob");
    setVanillaCauseMessage(SharedTypes::Legacy::ActorDamageCause::Drowning, "drown");
    setVanillaCauseMessage(SharedTypes::Legacy::ActorDamageCause::FallingBlock, "fallingBlock");
    setVanillaCauseMessage(SharedTypes::Legacy::ActorDamageCause::Fire, "inFire");
    setVanillaCauseMessage(SharedTypes::Legacy::ActorDamageCause::FireTick, "onFire");
    setVanillaCauseMessage(SharedTypes::Legacy::ActorDamageCause::Fireworks, "fireworks");
    setVanillaCauseMessage(SharedTypes::Legacy::ActorDamageCause::FlyIntoWall, "flyIntoWall");
    setVanillaCauseMessage(SharedTypes::Legacy::ActorDamageCause::Freezing, "freeze");
    setVanillaCauseMessage(SharedTypes::Legacy::ActorDamageCause::Lava, "lava");
    setVanillaCauseMessage(SharedTypes::Legacy::ActorDamageCause::Lightning, "lightningBolt");
    setVanillaCauseMessage(SharedTypes::Legacy::ActorDamageCause::Wither, "wither");
    setVanillaCauseMessage(SharedTypes::Legacy::ActorDamageCause::Magic, "indirectMagic");
    setVanillaCauseMessage(SharedTypes::Legacy::ActorDamageCause::Magma, "hotFloor");
    setVanillaCauseMessage(SharedTypes::Legacy::ActorDamageCause::Stalactite, "stalactite");
    setVanillaCauseMessage(SharedTypes::Legacy::ActorDamageCause::Stalagmite, "stalagmite");
    setVanillaCauseMessage(SharedTypes::Legacy::ActorDamageCause::Starve, "starve");
    setVanillaCauseMessage(SharedTypes::Legacy::ActorDamageCause::Suffocation, "inWall");
    setVanillaCauseMessage(SharedTypes::Legacy::ActorDamageCause::SonicBoom, "sonicBoom");
    setVanillaCauseMessage(SharedTypes::Legacy::ActorDamageCause::Void, "outOfWorld");
    setVanillaCauseMessage(SharedTypes::Legacy::ActorDamageCause::SelfDestruct, "genericKill");
    setVanillaCauseMessage(SharedTypes::Legacy::ActorDamageCause::Projectile, "thrown");
    setVanillaCauseMessage(SharedTypes::Legacy::ActorDamageCause::Thorns, "thorns");
    registerCustomDeathMessage(
        SharedTypes::Legacy::ActorDamageCause::Projectile,
        "death.attack.fireball.item",
        "minecraft:small_fireball"
    );
    registerCustomDeathMessage(
        SharedTypes::Legacy::ActorDamageCause::Projectile,
        "death.attack.trident.item",
        "minecraft:thrown_trident"
    );
    registerCustomDeathMessage(
        SharedTypes::Legacy::ActorDamageCause::Projectile,
        "death.attack.spit",
        "minecraft:llama_spit"
    );
    registerCustomDeathMessage(
        SharedTypes::Legacy::ActorDamageCause::Projectile,
        "death.attack.bullet",
        "minecraft:shulker_bullet"
    );
    registerCustomDeathMessage(
        SharedTypes::Legacy::ActorDamageCause::Projectile,
        "death.attack.arrow.item",
        "minecraft:arrow"
    );
    registerCustomDeathMessage(
        SharedTypes::Legacy::ActorDamageCause::Projectile,
        "death.attack.explosion.player",
        "minecraft:fireball"
    );
    registerCustomDeathMessage(
        SharedTypes::Legacy::ActorDamageCause::Projectile,
        "death.attack.mob",
        "minecraft:breeze_wind_charge_projectile"
    );
    registerCustomDeathMessage(
        SharedTypes::Legacy::ActorDamageCause::Magic,
        "death.attack.magic",
        "empty"
    );
    registerCustomDeathMessage(
        SharedTypes::Legacy::ActorDamageCause::SonicBoom,
        "death.attack.sonicBoom",
        "minecraft:warden"
    );
    registerCustomDeathMessage(
        SharedTypes::Legacy::ActorDamageCause::Thorns,
        "death.attack.generic",
        "empty"
    );
    registerCustomDeathMessage(
        SharedTypes::Legacy::ActorDamageCause::EntityExplosion,
        "death.attack.explosion.player",
        "all"
    );
    registerCustomDeathMessage(
        SharedTypes::Legacy::ActorDamageCause::EntityExplosion,
        "minecraft:wither_skull",
        "death.attack.witherSkull.item"
    );
    registerCustomDeathMessage(
        SharedTypes::Legacy::ActorDamageCause::EntityExplosion,
        "minecraft:wither_skull_dangerous",
        "death.attack.witherSkull.item"
    );
    registerCustomDeathMessage(
        SharedTypes::Legacy::ActorDamageCause::EntityAttack,
        "minecraft:player",
        "death.attack.player.item"
    );
    registerCustomDeathMessage(
        SharedTypes::Legacy::ActorDamageCause::EntityAttack,
        "minecraft:bee",
        "death.attack.sting.item"
    );
    registerCustomDeathMessage(
        SharedTypes::Legacy::ActorDamageCause::RamAttack,
        "minecraft:player",
        "death.attack.player.item"
    );
}
/*
void ListenEvents() {
    auto& config   = DeathMessages::Entry::getInstance().getConfig();
    auto& eventBus = ll::event::EventBus::getInstance();
    if (config.ConsoleLog.JoinMessage) {
        eventBus.emplaceListener<ll::event::player::PlayerConnectEvent>([](ll::event::player::PlayerConnectEvent& ev) {
            infoLogger->info(fmt::format(fg(fmt::color::yellow), tr("multiplayer.player.joined", {ev.self().getRealName()}))
            );
        });
    }
    if (config.ConsoleLog.LeftMessage) {
        eventBus.emplaceListener<ll::event::player::PlayerDisconnectEvent>(
            [](ll::event::player::PlayerDisconnectEvent& ev) {
                infoLogger->info(
                    fmt::format(fg(fmt::color::yellow), tr("multiplayer.player.left", {ev.self().getRealName()}))
                );
            }
        );
    }
    if (config.ConsoleLog.DeathMessage) {
        eventBus.emplaceListener<GMLIB::Event::EntityEvent::DeathMessageAfterEvent>(
            [](GMLIB::Event::EntityEvent::DeathMessageAfterEvent& ev) {
                auto msg  = ev.getDeathMessage();
                auto info = tr(msg.first, msg.second);
                deathLogger->info(info);
                ev.getDeathMessage() = {info, msg.second};
            }
        );
    }
}*/