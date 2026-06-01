#include "Limiter.h"
#include <cstddef>
#include <cmath>

Limiter::Limiter(float threshold, enum LimiterStyle limType)
    : m_threshold(threshold), m_limType(limType) {}

float Limiter::processSample(float inputSample)
{
    if (m_limType == LimiterStyle::HARD)
    {
        // --- HARD LIMITING (Brickwall) ---
        if (inputSample > m_threshold)
        {
            return m_threshold;
        }
        else if (inputSample < -m_threshold)
        {
            return -m_threshold;
        }
        return inputSample;
    }
    else
    {
        // --- SOFT LIMITING (Sanfte Sättigung) ---
        if (inputSample == 0.0f)
            return 0.0f;

        // Formel: threshold * tanh(input / threshold)
        float scaledInput = inputSample / m_threshold;
        float softClipped = std::tanh(scaledInput) * m_threshold;

        return softClipped;
    }
}