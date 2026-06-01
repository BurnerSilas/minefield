#include "LowPassFilter.h"
#include <cmath>

// Standard M_PI might not be defined on all compilers without extensions
// #ifndef M_PI
// #define M_PI 3.14159265358979323846f
// #endif

LowPassFilter::LowPassFilter(float cutoffFrequency, float sampleRate)
    : m_sampleRate(sampleRate), m_prevOutput(0.0f)
{
    setCutoff(cutoffFrequency);
}

void LowPassFilter::setCutoff(float cutoffFrequency)
{
    // Calculate the time constant RC and the smoothing factor alpha
    float dt = 1.0f / m_sampleRate;
    float rc = 1.0f / (2.0f * static_cast<float>(M_PI) * cutoffFrequency);
    m_alpha = dt / (rc + dt);
}

float LowPassFilter::processSample(float inputSample)
{
    // Pure IIR low-pass filter equation
    float outputSample = (m_alpha * inputSample) + ((1.0f - m_alpha) * m_prevOutput);

    // Store current state for the next sample processing step
    m_prevOutput = outputSample;

    return outputSample;
}