#include "Entry.h"
#include "Global.h"

ll::Logger logger(PLUGIN_NAME);

namespace DeathMessages {

std::unique_ptr<Entry>& Entry::getInstance() {
    static std::unique_ptr<Entry> instance;
    return instance;
}

bool Entry::load() {
    initPlugin();
    return true;
}

bool Entry::enable() {
    RegisterDamageDefinition();
    ListenEvents();
    return true;
}

bool Entry::disable() { return true; }

} // namespace DeathMessages

LL_REGISTER_PLUGIN(DeathMessages::Entry, DeathMessages::Entry::getInstance());