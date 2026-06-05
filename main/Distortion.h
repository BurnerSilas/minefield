#pragma once
#include <cstdint>
#include "AudioEffect.h"
#include "LowPassFilter.h"

class Distortion : public AudioEffect
{
public:
    /// @brief a "Boss ODB-3" style overdrive/distortion effect [default init = my preset]
    /// @param gain Factor : amount of distortion < 10.0 not recommended
    /// @param mix % : DRY/WET | blends unprocessed audio with processed audio | 1.0 = WET, 0.0 = DRY, 0,5 = 50% WET + 50% DRY
    /// @param bassBlend Hz : splits the signal at the given frequency in 2 Signals and applies distortion only to the frequencies above the "Highs"
    Distortion(float gain = 2.5f, float mix = 0.9f, float bassBlend = 850.0f);

    float processSample(float inputSample) override;

    void setGain(float gain) { m_gain = gain; }
    void setMix(float mix) { m_mix = mix; }
    void setBassBlend(float freq) { m_bassFilter.setCutoff(freq); }

private:
    float m_gain;
    float m_mix;
    LowPassFilter m_bassFilter;
};