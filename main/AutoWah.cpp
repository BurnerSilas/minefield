#include "AutoWah.h"
#include <cmath>

AutoWah::AutoWah(float minFreq, float maxFreq, float q,
                 float lfoRate, float sampleRate)
    : m_minFreq(minFreq), m_maxFreq(maxFreq),
      m_lfoRate(lfoRate), m_lfoDepth(0.7f),
      m_sampleRate(sampleRate), m_lfoPhase(0.0f),
      m_filter(minFreq, q, sampleRate) {}

void AutoWah::setMinFreq(float freq)
{
    m_minFreq = freq;
}

void AutoWah::setMaxFreq(float freq)
{
    m_maxFreq = freq;
}

void AutoWah::setQ(float q)
{
    m_filter.setQ(q);
}

void AutoWah::setLfoRate(float rate)
{
    m_lfoRate = rate;
}

void AutoWah::setLfoDepth(float depth)
{
    m_lfoDepth = depth;
}

float AutoWah::processSample(float input)
{
    if (!m_bypass)
    {
        // LFO Phase weiterschieben
        m_lfoPhase += m_lfoRate / m_sampleRate;
        if (m_lfoPhase > 1.0f)
            m_lfoPhase -= 1.0f;

        // Sinus LFO → 0.0 bis 1.0
        float lfo = (sinf(2.0f * M_PI * m_lfoPhase) + 1.0f) / 2.0f;

        // Depth anwenden — bei depth=0.5 schwingt nur die halbe Bandbreite
        float position = (1.0f - m_lfoDepth) / 2.0f + lfo * m_lfoDepth;

        // Exponentiell interpolieren — klingt natürlicher als linear
        float freq = m_minFreq * powf(m_maxFreq / m_minFreq, position);
        m_filter.setCenterFreq(freq);
    }

    return m_filter.processSample(input);
}