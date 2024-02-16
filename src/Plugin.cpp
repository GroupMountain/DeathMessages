#include "Plugin.h"
#include "Global.h"

ll::Logger logger(PLUGIN_NAME);

namespace plugin {

Plugin::Plugin(ll::plugin::NativePlugin& self) : mSelf(self) {
    // Code for loading the plugin goes here.
    initPlugin();
}

bool Plugin::enable() {
    // Code for enabling the plugin goes here.
    auto requireLibVersion = SemVersion(0, 8, 2, "", "");
    if (GMLIB::Version::checkLibVersionMatch(requireLibVersion)) {
        RegisterDamageDefinition();
        ListenEvents();
        logger.info("DeathMessages Loaded!");
        logger.info("Author: GroupMountain");
        logger.info("Repository: https://github.com/GroupMountain/DeathMessages");
    } else {
        logger.error("GMLIB Version is outdated! Please update your GMLIB!");
        logger.error(
            "Current GMLIB Version {}, Required Lowest GMLIB Version {}",
            GMLIB::Version::getLibVersionString(),
            requireLibVersion.asString()
        );
    }
    return true;
}

bool Plugin::disable() {
    // Code for disabling the plugin goes here.
    return true;
}

} // namespace plugin