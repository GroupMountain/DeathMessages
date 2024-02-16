#pragma once
#include "Global.h"

std::string defaultConfig = R"({
    "ConsoleLog": {
        "JoinMessage": true,
        "LeftMessage": true,
        "DeathMessage": true
    },
    "FileLog": {
        "Enabled": false,
        "Path": "./logs/DeathLog.log"
    },
    "ServerSideTranslation": {
        "Enabled": false,
        "Language": "en_US"
    }
})";