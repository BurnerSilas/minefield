#pragma once
#include <cstdint>
#include "AudioEffect.h"

class Distortion : public AudioEffect
{
public:
    Distortion(float drive, float level);

    float processSample(float inputSample) override;

    void setDrive(float drive) { m_drive = drive; }
    void setLevel(float level) { m_level = level; }

private:
    float m_drive;
    float m_level;
};