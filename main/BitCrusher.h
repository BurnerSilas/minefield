#pragma once
#include "AudioEffect.h"

class BitCrusher : public AudioEffect
{
public:
    /// @param bits     Ziel-Bittiefe, 1–24. 8 = klassischer 8-bit Sound
    /// @param mix      Mischung 0.0 = clean, 1.0 = nur gecrusht
    BitCrusher(float bits = 4.0f, float mix = .8f);

    void setBits(float bits); // float damit du per Serial smooth steuern kannst
    void setMix(float mix);

    float processSample(float input) override;

private:
    float m_bits;
    float m_mix;
    float m_step; // Quantisierungsschrittweite

    void computeStep();
};