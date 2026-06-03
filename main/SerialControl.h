#pragma once
#include "driver/usb_serial_jtag.h"
#include "esp_log.h"
#include "Distortion.h"
#include "Limiter.h"
#include "LowPassFilter.h"
#include <cstring>
#include <cstdlib>

#define SERIAL_BUF_SIZE 256

class SerialControl {
public:
    // Referenzen auf alle Effekte die du steuern willst
    SerialControl(Distortion& distortion, Limiter& limiter, LowPassFilter& lowPass)
        : m_distortion(distortion), m_limiter(limiter), m_lowPass(lowPass) {}

    void init();
    void update();

private:
    Distortion&    m_distortion;
    Limiter&       m_limiter;
    LowPassFilter& m_lowPass;

    char   m_buf[SERIAL_BUF_SIZE];
    size_t m_bufLen = 0;

    void parseCommand(const char* cmd);

    void handleBypass(AudioEffect& effect, const char* sub) {
    if (strcmp(sub, "ON")  == 0) effect.setBypass(true);
    if (strcmp(sub, "OFF") == 0) effect.setBypass(false);}

    // Ein Handler pro Effekt — hier einfach neue hinzufügen
    void handleDistortion(const char* sub);
    void handleLimiter(const char* sub);
    void handleLowPass(const char* sub);

    // Hilfsfunktionen
    void respond(const char* msg);
};