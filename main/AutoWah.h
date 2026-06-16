#pragma once
#include "AudioEffect.h"
#include "BandPassFilter.h"

class AutoWah : public AudioEffect
{
public:
    /// @param minFreq    Tiefste Frequenz in Hz (Heel)
    /// @param maxFreq    Höchste Frequenz in Hz (Toe)
    /// @param q          Schärfe des Filters, 2.0–6.0
    /// @param lfoRate    LFO Geschwindigkeit in Hz, z.B. 1.0f
    /// @param sampleRate Abtastrate in Hz
    AutoWah(float minFreq = 200.0f,
            float maxFreq = 2500.0f,
            float q = 2.5f,
            float lfoRate = 2.3f,
            float sampleRate = 48000.0f);

    void setMinFreq(float freq);
    void setMaxFreq(float freq);
    void setQ(float q);
    void setLfoRate(float rate);   // Hz — wie schnell der Wah schwingt
    void setLfoDepth(float depth); // 0.0–1.0 — wie weit die Frequenz schwingt

    float processSample(float input) override;

private:
    float m_minFreq;
    float m_maxFreq;
    float m_lfoRate;
    float m_lfoDepth;
    float m_sampleRate;
    float m_lfoPhase; // 0.0–1.0

    BandPassFilter m_filter;

    void updateFilter();
};