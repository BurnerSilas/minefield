#pragma once
#include <Arduino.h>
#include "Distortion.h"

class SerialControl
{
public:
    SerialControl(Distortion &distortion) : _distortion(distortion) {}

    void update()
    {
        if (!Serial.available())
            return;

        String cmd = Serial.readStringUntil('\n');
        cmd.trim();
        if (cmd.length() == 0)
            return;
        Serial.println("CMD: " + cmd);
        parseCommand(cmd);
        Serial.flush();
    }

private:
    Distortion &_distortion;

    void parseCommand(const String &cmd)
    {
        if (cmd.startsWith("DIST:"))
        {
            String sub = cmd.substring(5);

            if (sub.startsWith("DRIVE:"))
            {
                float val = sub.substring(6).toFloat();
                _distortion.setDrive(val);
                Serial.println("OK: drive=" + String(val));
            }
            else if (sub.startsWith("LEVEL:"))
            {
                float val = sub.substring(6).toFloat();
                _distortion.setLevel(val);
                Serial.println("OK: level=" + String(val));
            }
            else if (sub.startsWith("MODE:"))
            {
                String mode = sub.substring(5);
                if (mode == "HARD_CLIP")
                    _distortion.setMode(Distortion::Mode::HARD_CLIP);
                else if (mode == "SOFT_CLIP")
                    _distortion.setMode(Distortion::Mode::SOFT_CLIP);
                else if (mode == "FUZZ")
                    _distortion.setMode(Distortion::Mode::FUZZ);
                Serial.println("OK: mode=" + mode);
            }
            else if (sub == "ON")
            {
                _distortion.setEnabled(true);
                Serial.println("OK: distortion on");
            }
            else if (sub == "OFF")
            {
                _distortion.setEnabled(false);
                Serial.println("OK: distortion off");
            }
            else
            {
                Serial.println("ERR: unknown command");
            }
        }
    }
};