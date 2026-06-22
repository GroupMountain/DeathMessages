#include "Entry.h"
#include "Global.h"

#include "ll/api/event/player/PlayerConnectEvent.h"
#include "ll/api/event/player/PlayerDieEvent.h"
#include "ll/api/event/player/PlayerDisconnectEvent.h"

#include "ll/api/event/EventBus.h"

ll::event::ListenerPtr playerConnectEventListener;
ll::event::ListenerPtr playerDisconnectEventListener;
void ListenEvents() {
    auto& config   = DeathMessages::Entry::getInstance().getConfig();
    auto& eventBus = ll::event::EventBus::getInstance();
    if (config.ConsoleLog.JoinMessage) {
        playerConnectEventListener=eventBus.emplaceListener<ll::event::player::PlayerConnectEvent>([](ll::event::player::PlayerConnectEvent& ev) {
                infoLogger->info(
                    fmt::format(fg(fmt::color::yellow), fmt::runtime(tr("multiplayer.player.joined", {ev.self().getRealName()})))
            );

        });
    }
    if (config.ConsoleLog.LeftMessage) {
        playerDisconnectEventListener=eventBus.emplaceListener<ll::event::player::PlayerDisconnectEvent>(
            [](ll::event::player::PlayerDisconnectEvent& ev) {
                infoLogger->info(
                    fmt::format(fg(fmt::color::yellow), fmt::runtime(tr("multiplayer.player.left", {ev.self().getRealName()})))
                );
            }
        );
    }

    //Die Message move to DeathMessage.cpp 93L

}

void uninstallEventListeners() {
    auto& eventBus = ll::event::EventBus::getInstance();
    eventBus.removeListener(playerConnectEventListener);
    eventBus.removeListener(playerDisconnectEventListener);
}