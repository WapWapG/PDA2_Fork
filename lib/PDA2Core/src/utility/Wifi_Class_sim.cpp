// ════════════════════════════════════════════════════════
//  PDA 2 — Wifi_Class_sim.cpp
//  STA: SSID/pass не проверяются (PC не переключает реальную
//  сеть) — connectSTA()/setEnabled(true) делают ОДНОРАЗОВУЮ
//  короткую проверку сети хоста (TCP-коннект на 8.8.8.8:53,
//  таймаут ~400мс) через Winsock2. Результат фиксируется до
//  следующего явного вызова — не поллинг, не фоновый поток.
//  AP: состояние-заглушка, без реального AP хоста (Windows
//  Hosted Network отброшен — deprecated/ненадёжен).
//
//  Компилируется, только если PDA2_SIM определён.
// ════════════════════════════════════════════════════════

#include "Wifi_Class.h"

#ifdef PDA2_SIM

#include <PDA2.h>
#include "../pda2_log.h"

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
#endif

static WifiState _state      = WifiState::OFF;
static int       _ap_clients = 0;

#ifdef _WIN32
static bool _wsa_ready = false;

static bool _ensure_wsa() {
    if (_wsa_ready) return true;
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        PDA_LOGE("wifi", "SIM: WSAStartup failed");
        return false;
    }
    _wsa_ready = true;
    return true;
}
#endif

// Разовая проверка сети хоста. Блокирует вызывающий поток на
// время таймаута — допустимо, т.к. вызывается только по
// явному действию пользователя (тоггл), не каждый тик.
static bool _check_host_network() {
#ifdef _WIN32
    if (!_ensure_wsa()) return false;

    SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (s == INVALID_SOCKET) return false;

    u_long mode = 1;
    ioctlsocket(s, FIONBIO, &mode);

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port   = htons(53);
    inet_pton(AF_INET, "8.8.8.8", &addr.sin_addr);

    connect(s, (sockaddr*)&addr, sizeof(addr));

    fd_set write_set;
    FD_ZERO(&write_set);
    FD_SET(s, &write_set);
    timeval tv{0, 400000}; // 400мс

    int res = select(0, nullptr, &write_set, nullptr, &tv);
    bool ok = (res > 0) && FD_ISSET(s, &write_set);

    closesocket(s);
    return ok;
#else
    return false; // не-Windows sim сейчас не целится
#endif
}

void Wifi_Class::begin() {
    if (PDA.Prefs.getWifi()) {
        connectSTA(PDA.Prefs.getWifiSSID(), PDA.Prefs.getWifiPass());
    }
}

void Wifi_Class::update() {
    // Разовая проверка, не поллинг — намеренно пусто.
}

void Wifi_Class::setEnabled(bool on) {
    if (on) connectSTA(PDA.Prefs.getWifiSSID(), PDA.Prefs.getWifiPass());
    else    disconnectSTA();
}

void Wifi_Class::connectSTA(const PdaStr32& ssid, const PdaStr32& pass) {
    (void)ssid; (void)pass; // SIM: SSID/pass не проверяются, см. заголовок файла
    bool online = _check_host_network();
    _state = online ? WifiState::STA_CONNECTED : WifiState::STA_FAILED;
    PDA_LOGI("wifi", "SIM: host network check -> %s", online ? "online" : "offline");
}

void Wifi_Class::disconnectSTA() {
    _state = WifiState::OFF;
}

void Wifi_Class::startAP(const PdaStr32& ssid, const PdaStr32& pass) {
    (void)ssid; (void)pass;
    _state      = WifiState::AP_ACTIVE;
    _ap_clients = 0; // заглушка, без реального AP хоста
    PDA_LOGI("wifi", "SIM: AP state-only (no real host AP)");
}

void Wifi_Class::stopAP() {
    _state      = WifiState::OFF;
    _ap_clients = 0;
}

WifiState Wifi_Class::state()         { return _state; }
bool      Wifi_Class::isConnected()   { return _state == WifiState::STA_CONNECTED; }
int       Wifi_Class::apClientCount() { return _ap_clients; }

#endif // PDA2_SIM