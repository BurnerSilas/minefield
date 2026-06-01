#pragma once
#include <cstdint>

class Distortion
{
public:
    Distortion(float drive, float level);

    float processSample(float inputSample);

    void setDrive(float drive) { m_drive = drive; }
    void setLevel(float level) { m_level = level; }

private:
    float m_drive;
    float m_level;
};