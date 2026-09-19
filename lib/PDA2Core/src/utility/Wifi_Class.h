#pragma once

// ════════════════════════════════════════════════════════
//  PDA 2 — Wifi_Class.h
//  Subsystem-обёртка над WiFi: STA (подключение к сети) +
//  AP (раздача точки доступа). Учётки — через Prefs_Class.
// ════════════════════════════════════════════════════════
#include <stdint.h>
#include "PdaStr32.h"

enum class WifiState : uint8_t {
    OFF,
    STA_CONNECTING,
    STA_CONNECTED,
    STA_FAILED,
    AP_ACTIVE
};

class Wifi_Class {
public:
    void begin();
    void update();

    // Совместимость с QuickPanel_Class — управляет STA
    void setEnabled(bool on);

    void connectSTA(const PdaStr32& ssid, const PdaStr32& pass);
    void disconnectSTA();

    void startAP(const PdaStr32& ssid, const PdaStr32& pass);
    void stopAP();

    WifiState state();
    bool      isConnected();
    int       apClientCount();
};