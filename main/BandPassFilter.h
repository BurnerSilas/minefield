#pragma once
#include "AudioEffect.h"

class BandPassFilter : public AudioEffect {
public:
    BandPassFilter(float centerFreq, float q, float sampleRate = 48000.0f);

    void setCenterFreq(float freq);
    void setQ(float q);

    float processSample(float input) override;

private:
    float m_sampleRate;
    float m_q;
    float m_centerFreq;
    float m_b0, m_b1, m_b2, m_a1, m_a2;
    float m_x1 = 0, m_x2 = 0, m_y1 = 0, m_y2 = 0;

    void computeCoefficients();
};