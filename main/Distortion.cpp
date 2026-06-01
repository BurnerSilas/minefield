#include "Distortion.h"
#include <cstddef>
#include <cmath>

/// @brief
/// @param drive
/// @param level
Distortion::Distortion(float drive, float level)
    : m_drive(drive), m_level(level) {}

float Distortion::processSample(float inputSample)
{

    if (inputSample > 0)
        return 1 - exp(inputSample);
    else
        return -1 + exp(inputSample);

    /*float sample = inputSample * m_drive;
    if (sample > 1.0f)
        return 1.0f;
    else if (sample < -1.0f)
        return -1.0f;
    return sample;*/
}