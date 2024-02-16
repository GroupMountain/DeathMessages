#include "Global.h"

void RegisterDamageDefinition() {
    GMLIB::Mod::DamageCause::setCustomDamageCauseEnabled();
    GMLIB::Mod::DamageCause::setVanillaCauseMessage(ActorDamageCause::Anvil, "anvil");
    GMLIB::Mod::DamageCause::setVanillaCauseMessage(ActorDamageCause::EntityAttack, "mob");
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
}

void ListenEvents() {
    auto& eventBus = ll::event::EventBus::getInstance();
    if (ConfigData::mJoinMessage) {
        eventBus.emplaceListener<ll::event::player::PlayerConnectEvent>([](ll::event::player::PlayerConnectEvent& ev) {
            infoLogger.info(fmt::format(fg(fmt::color::yellow), tr("multiplayer.player.joined", {ev.self().getName()}))
            );
        });
    }
    if (ConfigData::mLeftMessage) {
        eventBus.emplaceListener<ll::event::player::PlayerLeaveEvent>([](ll::event::player::PlayerLeaveEvent& ev) {
            infoLogger.info(fmt::format(fg(fmt::color::yellow), tr("multiplayer.player.left", {ev.self().getName()})));
        });
    }
    if (ConfigData::mDeathMessage) {
        eventBus.emplaceListener<GMLIB::Event::EntityEvent::DeathMessageAfterEvent>(
            [](GMLIB::Event::EntityEvent::DeathMessageAfterEvent& ev) {
                auto msg  = ev.getDeathMessage();
                auto info = tr(msg.first, msg.second);
                deathLogger.info(info);
                ev.setDeathMessage({info, msg.second});
            }
        );
    }
}