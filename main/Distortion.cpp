#include "Distortion.h"
#include <cstddef>
#include <cmath>

/// @brief
/// @param gain
/// @param mix
/// @param bassBlendFreq
Distortion::Distortion(float gain, float mix, float bassBlendFreq)
    : m_gain(gain), m_mix(mix), m_bassFilter(bassBlendFreq, 48000.0f) {}

/// @brief Inspiered by the book "DAFX: Digital Audio Effects" (Page 127) Bendiksen, Dutilleux, Zölzer, Dempwolf
/// @param inputSample float : input Sample
/// @return float : outputSample 
float Distortion::processSample(float inputSample)
{
    // only distort highs and mids
    // leave Low end clean
    // -> no mud
    // so split the Signal
    float lowEnd = m_bassFilter.processSample(inputSample); 
    float highEnd = inputSample - lowEnd;                   

    // distort

    float q = highEnd * m_gain;
    float z = (q >= 0 ? 1.0f : -1.0f) * (1.0f - expf(-q));
    float distorted = m_mix * z + (1.0f - m_mix) * highEnd;

    // combine
    return lowEnd + distorted;
}