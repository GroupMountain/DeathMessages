#include "Entry.h"
#include "Global.h"

#include "ll/api/memory/Hook.h"
#include "ll/api/chrono/GameChrono.h"
#include "ll/api/thread/ThreadPoolExecutor.h"

#include "mc/world/actor/Mob.h"
#include "mc/world/actor/monster/EnderCrystal.h"
#include "mc/world/actor/ActorDamageSource.h"
#include "mc/world/actor/ActorDamageByActorSource.h"
#include "mc/world/actor/ActorDamageByBlockSource.h"
#include "mc/world/actor/ActorDamageByChildActorSource.h"
#include "mc/world/actor/ActorHurtResult.h"
#include "mc/world/actor/Actor.h"

#include <chrono>




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
    }
    if (hasCategory(ActorCategory::Player) || isTame()) {
        if (mLastHurtByMobTime != 0) {
            auto hm = getLastHurtByMob();
            mHurtByEntityMap[uid.rawID] = hm->getOrCreateUniqueID();
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

LL_AUTO_TYPE_INSTANCE_HOOK(
    CryatslHurtHook,
    HookPriority::Normal,
    EnderCrystal,
    &EnderCrystal::$_hurt,
    ::ActorHurtResult,
    ActorDamageSource const& a1,
    float              a2,
    bool               a3,
    bool               a4
) {
    auto res  = origin(a1, a2, a3, a4);
    isCrystal = true;
    auto& executor = ll::thread::ThreadPoolExecutor::getDefault();
    auto callback = executor.executeAfter(
        [] {
            isCrystal = false;
        },
        ll::chrono::game::ticks(1)
    );
    return res;
}


LL_AUTO_TYPE_INSTANCE_HOOK(
    ActorDamageHook,
    HookPriority::Lowest,
    ActorDamageSource,
    &ActorDamageSource::$getDeathMessage,
    DeathMessageResult,
    std::string a1,
    Actor*      a2
) {
    auto originl = origin(a1, a2);
    auto res     = translateDeathMessage(originl, a1, a2, this);
    return res;
}
//
/*LL_AUTO_TYPE_INSTANCE_HOOK(
    ActorDamageByActorHook,
    HookPriority::Lowest,
    ActorDamageByActorSource,
    &ActorDamageByActorSource::$getDeathMessage,
    DeathMessageResult,
    std::string a1,
    Actor*      a2
) {
    auto originl = origin(a1, a2);
    auto res     = translateDeathMessage(originl, a1, a2, this);
    return res;
}

LL_AUTO_TYPE_INSTANCE_HOOK(
    ActorDamageByBlockHook,
    HookPriority::Lowest,
    ActorDamageByBlockSource,
    &ActorDamageByBlockSource::$getDeathMessage,
    DeathMessageResult,
    std::string a1,
    Actor*      a2
) {
    auto originl = origin(a1, a2);
    auto res     = translateDeathMessage(originl, a1, a2, this);
    return res;
}

LL_AUTO_TYPE_INSTANCE_HOOK(
    ActorDamageByChildHook,
    HookPriority::Lowest,
    ActorDamageByChildActorSource,
    &ActorDamageByChildActorSource::$getDeathMessage,
    DeathMessageResult,
    std::string a1,
    Actor*      a2
) {
    auto originl = origin(a1, a2);
    auto res     = translateDeathMessage(originl, a1, a2, this);
    return res;
}*/

void uninstallHooks() {
    MobDieHook::unhook();
    CryatslHurtHook::unhook();
    ActorDamageHook::unhook();
    /*ActorDamageByActorHook::unhook();
    ActorDamageByBlockHook::unhook();
    ActorDamageByChildHook::unhook();*/
}
