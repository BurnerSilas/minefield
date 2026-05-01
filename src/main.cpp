#include <Arduino.h>
#include "driver/i2s.h" // Die nativen Treiber sind oft performanter
#include <vector>
#include "AudioEffect.h"
#include "Gain.h"
// Task-Handle für den Audio-Prozess
TaskHandle_t AudioTaskHandle;
Gain boost(1.2f);

std::vector<AudioEffect *> effectChain;

// Die Audio-Schleife, die auf Core 1 läuft
void AudioLoop(void *pvParameters)
{
    float testSample = 1.0f;

    while (true)
    {
        // 1. Daten vom ADC lesen (I2S)
        // 2. DSP-Berechnungen (Effekte)
        applyEffects(testSample, effectChain);
        // 3. Daten an den DAC schreiben (I2S)

        // Kleine Pause für den Watchdog (wichtig bei FreeRTOS)
        vTaskDelay(1);
    }
}

void setup()
{
    Serial.begin(115200);

    // I2S Hardware-Konfiguration (Pins, Samplerate etc.)
    // ... hier kommt später dein Setup-Code rein ...

    // build effect chain
    effectChain.push_back(&boost);

    // Wir erstellen den Task auf Core 1 (Core 0 macht System-Kram)
    xTaskCreatePinnedToCore(
        AudioLoop,        /* Funktion */
        "AudioTask",      /* Name */
        10000,            /* Stackgröße */
        NULL,             /* Parameter */
        1,                /* Priorität (hoch!) */
        &AudioTaskHandle, /* Handle */
        1                 /* Core ID */
    );
}

void loop()
{
    // Hier kannst du später Potis abfragen oder LEDs blinken lassen
    // Die loop() läuft auf Core 0 und stört den Sound nicht.
}

float applyEffects(float input, std::vector<AudioEffect *> &Effects)
{
    for (AudioEffect *eff : Effects)
    {
        input = eff->process(input);
    }
    return input;
}