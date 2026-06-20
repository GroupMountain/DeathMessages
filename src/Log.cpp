
#include "Entry.h"
#include "Global.h"

#include "ll/api/event/player/PlayerConnectEvent.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/event/player/PlayerDieEvent.h"

#include "ll/api/event/player/PlayerDisconnectEvent.h"
void ListenEvents() {
    auto& config   = DeathMessages::Entry::getInstance().getConfig();
    auto& eventBus = ll::event::EventBus::getInstance();
    if (config.ConsoleLog.JoinMessage) {
        eventBus.emplaceListener<ll::event::player::PlayerConnectEvent>([](ll::event::player::PlayerConnectEvent& ev) {
                infoLogger->info(
                    fmt::format(fg(fmt::color::yellow), fmt::runtime(tr("multiplayer.player.joined", {ev.self().getRealName()})))
            );

        });
    }
    if (config.ConsoleLog.LeftMessage) {
        eventBus.emplaceListener<ll::event::player::PlayerDisconnectEvent>(
            [](ll::event::player::PlayerDisconnectEvent& ev) {
                infoLogger->info(
                    fmt::format(fg(fmt::color::yellow), fmt::runtime(tr("multiplayer.player.left", {ev.self().getRealName()})))
                );
            }
        );
    }
    if (config.ConsoleLog.DeathMessage) {
        eventBus.emplaceListener<ll::event::player::PlayerDieEvent>(
            [](ll::event::player::PlayerDieEvent& ev) {
                auto [key, params]  = ev.source().getDeathMessage(ev.self().getRealName(), &ev.self());
                auto info = tr(key, params);
                deathLogger->info(info);
            }
        );
    }
}