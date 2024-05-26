#pragma once
#include <iostream>

struct Config {
    int version = 1;

    struct consoleLog {
        bool JoinMessage  = true;
        bool LeftMessage  = true;
        bool DeathMessage = true;
    } ConsoleLog;

    struct fileLog {
        bool        Enabled = false;
        std::string Path    = "./logs/DeathLog.log";
    } FileLog;

    struct serverSideTranslation {
        bool        Enabled  = false;
        std::string Language = "zh_CN";
    } ServerSideTranslation;
};