#include "BandPassFilter.h"
#include <cmath>

BandPassFilter::BandPassFilter(float centerFreq, float q, float sampleRate)
    : m_sampleRate(sampleRate), m_q(q), m_centerFreq(centerFreq)
{
    computeCoefficients();
}

void BandPassFilter::setCenterFreq(float freq)
{
    m_centerFreq = freq;
    computeCoefficients();
}

void BandPassFilter::setQ(float q)
{
    m_q = q;
    computeCoefficients();
}

float BandPassFilter::processSample(float input)
{
    float output = m_b0 * input + m_b1 * m_x1 + m_b2 * m_x2 - m_a1 * m_y1 - m_a2 * m_y2;
    m_x2 = m_x1;
    m_x1 = input;
    m_y2 = m_y1;
    m_y1 = output;
    return output;
}

void BandPassFilter::computeCoefficients()
{
    float w0 = 2.0f * M_PI * m_centerFreq / m_sampleRate;
    float cosw0 = cosf(w0);
    float alpha = sinf(w0) / (2.0f * m_q);

    float a0 = 1.0f + alpha;
    m_b0 = alpha / a0;
    m_b1 = 0.0f;
    m_b2 = -alpha / a0;
    m_a1 = -2.0f * cosw0 / a0;
    m_a2 = (1.0f - alpha) / a0;
}