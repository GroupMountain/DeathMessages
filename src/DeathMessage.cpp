#include "Global.h"

void RegisterDamageDefinition() {
    // GMLIB::Mod::DamageCause::registerDamageCause()
}

void ListenEvents() {
    auto& eventBus = ll::event::EventBus::getInstance();
    if (ConfigData::mJoinMessage) {
        eventBus.emplaceListener<ll::event::player::PlayerConnectEvent>([](ll::event::player::PlayerConnectEvent& ev) {
            infoLogger.info(
                "{}",
                fmt::format(fg(fmt::color::yellow), tr("multiplayer.player.joined", {ev.self().getName()}))
            );
        });
    }
    if (ConfigData::mLeftMessage) {
        eventBus.emplaceListener<ll::event::player::PlayerLeaveEvent>([](ll::event::player::PlayerLeaveEvent& ev) {
            infoLogger.info(
                "{}",
                fmt::format(fg(fmt::color::yellow), tr("multiplayer.player.left", {ev.self().getName()}))
            );
        });
    }
    if (ConfigData::mDeathMessage) {
        eventBus.emplaceListener<GMLIB::Event::EntityEvent::DeathMessageAfterEvent>(
            [](GMLIB::Event::EntityEvent::DeathMessageAfterEvent& ev) {
                auto msg = ev.getDeathMessage();
                deathLogger.info(tr(msg.first, msg.second));
            }
        );
    }
}