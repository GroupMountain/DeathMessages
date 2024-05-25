#include "Entry.h"
#include "Global.h"

void RegisterDamageDefinition() {
    GMLIB::Mod::DamageCause::setCustomDamageCauseEnabled();
    GMLIB::Mod::DamageCause::setVanillaCauseMessage(ActorDamageCause::Anvil, "anvil");
    GMLIB::Mod::DamageCause::setVanillaCauseMessage(ActorDamageCause::EntityAttack, "mob");
    GMLIB::Mod::DamageCause::setVanillaCauseMessage(ActorDamageCause::RamAttack, "mob");
    GMLIB::Mod::DamageCause::setVanillaCauseMessage(ActorDamageCause::Drowning, "drown");
    GMLIB::Mod::DamageCause::setVanillaCauseMessage(ActorDamageCause::FallingBlock, "fallingBlock");
    GMLIB::Mod::DamageCause::setVanillaCauseMessage(ActorDamageCause::Fire, "inFire");
    GMLIB::Mod::DamageCause::setVanillaCauseMessage(ActorDamageCause::FireTick, "onFire");
    GMLIB::Mod::DamageCause::setVanillaCauseMessage(ActorDamageCause::Fireworks, "fireworks");
    GMLIB::Mod::DamageCause::setVanillaCauseMessage(ActorDamageCause::FlyIntoWall, "flyIntoWall");
    GMLIB::Mod::DamageCause::setVanillaCauseMessage(ActorDamageCause::Freezing, "freeze");
    GMLIB::Mod::DamageCause::setVanillaCauseMessage(ActorDamageCause::Lava, "lava");
    GMLIB::Mod::DamageCause::setVanillaCauseMessage(ActorDamageCause::Lightning, "lightningBolt");
    GMLIB::Mod::DamageCause::setVanillaCauseMessage(ActorDamageCause::Wither, "wither");
    GMLIB::Mod::DamageCause::setVanillaCauseMessage(ActorDamageCause::Magic, "indirectMagic");
    GMLIB::Mod::DamageCause::setVanillaCauseMessage(ActorDamageCause::Magma, "hotFloor");
    GMLIB::Mod::DamageCause::setVanillaCauseMessage(ActorDamageCause::Stalactite, "stalactite");
    GMLIB::Mod::DamageCause::setVanillaCauseMessage(ActorDamageCause::Stalagmite, "stalagmite");
    GMLIB::Mod::DamageCause::setVanillaCauseMessage(ActorDamageCause::Starve, "starve");
    GMLIB::Mod::DamageCause::setVanillaCauseMessage(ActorDamageCause::Suffocation, "inWall");
    GMLIB::Mod::DamageCause::setVanillaCauseMessage(ActorDamageCause::SonicBoom, "sonicBoom");
    GMLIB::Mod::DamageCause::setVanillaCauseMessage(ActorDamageCause::Void, "outOfWorld");
    GMLIB::Mod::DamageCause::setVanillaCauseMessage(ActorDamageCause::SelfDestruct, "genericKill");
    GMLIB::Mod::DamageCause::setVanillaCauseMessage(ActorDamageCause::Projectile, "thrown");
    GMLIB::Mod::DamageCause::setVanillaCauseMessage(ActorDamageCause::Thorns, "thorns");
    GMLIB::Mod::DamageCause::registerCustomDeathMessage(
        ActorDamageCause::Projectile,
        "death.attack.fireball.item",
        "minecraft:small_fireball"
    );
    GMLIB::Mod::DamageCause::registerCustomDeathMessage(
        ActorDamageCause::Projectile,
        "death.attack.trident.item",
        "minecraft:thrown_trident"
    );
    GMLIB::Mod::DamageCause::registerCustomDeathMessage(
        ActorDamageCause::Projectile,
        "death.attack.spit",
        "minecraft:llama_spit"
    );
    GMLIB::Mod::DamageCause::registerCustomDeathMessage(
        ActorDamageCause::Projectile,
        "death.attack.bullet",
        "minecraft:shulker_bullet"
    );
    GMLIB::Mod::DamageCause::registerCustomDeathMessage(
        ActorDamageCause::Projectile,
        "death.attack.arrow.item",
        "minecraft:arrow"
    );
    GMLIB::Mod::DamageCause::registerCustomDeathMessage(
        ActorDamageCause::Projectile,
        "death.attack.explosion.player",
        "minecraft:fireball"
    );
    GMLIB::Mod::DamageCause::registerCustomDeathMessage(
        ActorDamageCause::Projectile,
        "death.attack.mob",
        "minecraft:breeze_wind_charge_projectile"
    );
    GMLIB::Mod::DamageCause::registerCustomDeathMessage(ActorDamageCause::Magic, "death.attack.magic", "empty");
    GMLIB::Mod::DamageCause::registerCustomDeathMessage(
        ActorDamageCause::SonicBoom,
        "death.attack.sonicBoom",
        "minecraft:warden"
    );
    GMLIB::Mod::DamageCause::registerCustomDeathMessage(ActorDamageCause::Thorns, "death.attack.generic", "empty");
    GMLIB::Mod::DamageCause::registerCustomDeathMessage(
        ActorDamageCause::EntityExplosion,
        "death.attack.explosion.player",
        "all"
    );
    GMLIB::Mod::DamageCause::registerCustomDeathMessage(
        ActorDamageCause::EntityExplosion,
        "minecraft:wither_skull",
        "death.attack.witherSkull.item"
    );
    GMLIB::Mod::DamageCause::registerCustomDeathMessage(
        ActorDamageCause::EntityExplosion,
        "minecraft:wither_skull_dangerous",
        "death.attack.witherSkull.item"
    );
    GMLIB::Mod::DamageCause::registerCustomDeathMessage(
        ActorDamageCause::EntityAttack,
        "minecraft:player",
        "death.attack.player.item"
    );
    GMLIB::Mod::DamageCause::registerCustomDeathMessage(
        ActorDamageCause::EntityAttack,
        "minecraft:bee",
        "death.attack.sting.item"
    );
    GMLIB::Mod::DamageCause::registerCustomDeathMessage(
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
        eventBus.emplaceListener<GMLIB::Event::EntityEvent::DeathMessageAfterEvent>(
            [](GMLIB::Event::EntityEvent::DeathMessageAfterEvent& ev) {
                auto msg  = ev.getDeathMessage();
                auto info = tr(msg.first, msg.second);
                deathLogger.info(info);
                ev.getDeathMessage() = {info, msg.second};
            }
        );
    }
}