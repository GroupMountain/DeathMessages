#include "Entry.h"
#include "Global.h"

void RegisterDamageDefinition() {
    gmlib::mod::damage::DamageCauseRegistry::setCustomDamageCauseEnabled();
    gmlib::mod::damage::DamageCauseRegistry::setVanillaCauseMessage(ActorDamageCause::Anvil, "anvil");
    gmlib::mod::damage::DamageCauseRegistry::setVanillaCauseMessage(ActorDamageCause::EntityAttack, "mob");
    gmlib::mod::damage::DamageCauseRegistry::setVanillaCauseMessage(ActorDamageCause::RamAttack, "mob");
    gmlib::mod::damage::DamageCauseRegistry::setVanillaCauseMessage(ActorDamageCause::Drowning, "drown");
    gmlib::mod::damage::DamageCauseRegistry::setVanillaCauseMessage(ActorDamageCause::FallingBlock, "fallingBlock");
    gmlib::mod::damage::DamageCauseRegistry::setVanillaCauseMessage(ActorDamageCause::Fire, "inFire");
    gmlib::mod::damage::DamageCauseRegistry::setVanillaCauseMessage(ActorDamageCause::FireTick, "onFire");
    gmlib::mod::damage::DamageCauseRegistry::setVanillaCauseMessage(ActorDamageCause::Fireworks, "fireworks");
    gmlib::mod::damage::DamageCauseRegistry::setVanillaCauseMessage(ActorDamageCause::FlyIntoWall, "flyIntoWall");
    gmlib::mod::damage::DamageCauseRegistry::setVanillaCauseMessage(ActorDamageCause::Freezing, "freeze");
    gmlib::mod::damage::DamageCauseRegistry::setVanillaCauseMessage(ActorDamageCause::Lava, "lava");
    gmlib::mod::damage::DamageCauseRegistry::setVanillaCauseMessage(ActorDamageCause::Lightning, "lightningBolt");
    gmlib::mod::damage::DamageCauseRegistry::setVanillaCauseMessage(ActorDamageCause::Wither, "wither");
    gmlib::mod::damage::DamageCauseRegistry::setVanillaCauseMessage(ActorDamageCause::Magic, "indirectMagic");
    gmlib::mod::damage::DamageCauseRegistry::setVanillaCauseMessage(ActorDamageCause::Magma, "hotFloor");
    gmlib::mod::damage::DamageCauseRegistry::setVanillaCauseMessage(ActorDamageCause::Stalactite, "stalactite");
    gmlib::mod::damage::DamageCauseRegistry::setVanillaCauseMessage(ActorDamageCause::Stalagmite, "stalagmite");
    gmlib::mod::damage::DamageCauseRegistry::setVanillaCauseMessage(ActorDamageCause::Starve, "starve");
    gmlib::mod::damage::DamageCauseRegistry::setVanillaCauseMessage(ActorDamageCause::Suffocation, "inWall");
    gmlib::mod::damage::DamageCauseRegistry::setVanillaCauseMessage(ActorDamageCause::SonicBoom, "sonicBoom");
    gmlib::mod::damage::DamageCauseRegistry::setVanillaCauseMessage(ActorDamageCause::Void, "outOfWorld");
    gmlib::mod::damage::DamageCauseRegistry::setVanillaCauseMessage(ActorDamageCause::SelfDestruct, "genericKill");
    gmlib::mod::damage::DamageCauseRegistry::setVanillaCauseMessage(ActorDamageCause::Projectile, "thrown");
    gmlib::mod::damage::DamageCauseRegistry::setVanillaCauseMessage(ActorDamageCause::Thorns, "thorns");
    gmlib::mod::damage::DamageCauseRegistry::registerCustomDeathMessage(
        ActorDamageCause::Projectile,
        "death.attack.fireball.item",
        "minecraft:small_fireball"
    );
    gmlib::mod::damage::DamageCauseRegistry::registerCustomDeathMessage(
        ActorDamageCause::Projectile,
        "death.attack.trident.item",
        "minecraft:thrown_trident"
    );
    gmlib::mod::damage::DamageCauseRegistry::registerCustomDeathMessage(
        ActorDamageCause::Projectile,
        "death.attack.spit",
        "minecraft:llama_spit"
    );
    gmlib::mod::damage::DamageCauseRegistry::registerCustomDeathMessage(
        ActorDamageCause::Projectile,
        "death.attack.bullet",
        "minecraft:shulker_bullet"
    );
    gmlib::mod::damage::DamageCauseRegistry::registerCustomDeathMessage(
        ActorDamageCause::Projectile,
        "death.attack.arrow.item",
        "minecraft:arrow"
    );
    gmlib::mod::damage::DamageCauseRegistry::registerCustomDeathMessage(
        ActorDamageCause::Projectile,
        "death.attack.explosion.player",
        "minecraft:fireball"
    );
    gmlib::mod::damage::DamageCauseRegistry::registerCustomDeathMessage(
        ActorDamageCause::Projectile,
        "death.attack.mob",
        "minecraft:breeze_wind_charge_projectile"
    );
    gmlib::mod::damage::DamageCauseRegistry::registerCustomDeathMessage(
        ActorDamageCause::Magic,
        "death.attack.magic",
        "empty"
    );
    gmlib::mod::damage::DamageCauseRegistry::registerCustomDeathMessage(
        ActorDamageCause::SonicBoom,
        "death.attack.sonicBoom",
        "minecraft:warden"
    );
    gmlib::mod::damage::DamageCauseRegistry::registerCustomDeathMessage(
        ActorDamageCause::Thorns,
        "death.attack.generic",
        "empty"
    );
    gmlib::mod::damage::DamageCauseRegistry::registerCustomDeathMessage(
        ActorDamageCause::EntityExplosion,
        "death.attack.explosion.player",
        "all"
    );
    gmlib::mod::damage::DamageCauseRegistry::registerCustomDeathMessage(
        ActorDamageCause::EntityExplosion,
        "minecraft:wither_skull",
        "death.attack.witherSkull.item"
    );
    gmlib::mod::damage::DamageCauseRegistry::registerCustomDeathMessage(
        ActorDamageCause::EntityExplosion,
        "minecraft:wither_skull_dangerous",
        "death.attack.witherSkull.item"
    );
    gmlib::mod::damage::DamageCauseRegistry::registerCustomDeathMessage(
        ActorDamageCause::EntityAttack,
        "minecraft:player",
        "death.attack.player.item"
    );
    gmlib::mod::damage::DamageCauseRegistry::registerCustomDeathMessage(
        ActorDamageCause::EntityAttack,
        "minecraft:bee",
        "death.attack.sting.item"
    );
    gmlib::mod::damage::DamageCauseRegistry::registerCustomDeathMessage(
        ActorDamageCause::RamAttack,
        "minecraft:player",
        "death.attack.player.item"
    );
}

void ListenEvents() {
    auto& config   = DeathMessages::Entry::getInstance()->getConfig();
    auto& eventBus = ll::event::EventBus::getInstance();
    if (config.ConsoleLog.JoinMessage) {
        eventBus.emplaceListener<ll::event::player::PlayerConnectEvent>([](ll::event::player::PlayerConnectEvent& ev) {
            infoLogger.info(fmt::format(fg(fmt::color::yellow), tr("multiplayer.player.joined", {ev.self().getName()}))
            );
        });
    }
    if (config.ConsoleLog.LeftMessage) {
        eventBus.emplaceListener<ll::event::player::PlayerLeaveEvent>([](ll::event::player::PlayerLeaveEvent& ev) {
            infoLogger.info(fmt::format(fg(fmt::color::yellow), tr("multiplayer.player.left", {ev.self().getName()})));
        });
    }
    if (config.ConsoleLog.DeathMessage) {
        eventBus.emplaceListener<gmlib::event::entity::DeathMessageAfterEvent>(
            [](gmlib::event::entity::DeathMessageAfterEvent& ev) {
                auto msg  = ev.getDeathMessage();
                auto info = tr(msg.first, msg.second);
                deathLogger.info(info);
                ev.getDeathMessage() = {info, msg.second};
            }
        );
    }
}