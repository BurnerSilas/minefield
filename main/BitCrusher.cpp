#include "BitCrusher.h"
#include <cmath>

BitCrusher::BitCrusher(float bits, float mix)
    : m_bits(bits), m_mix(mix)
{
    computeStep();
}

void BitCrusher::setBits(float bits)
{
    m_bits = bits;
    computeStep();
}

void BitCrusher::setMix(float mix)
{
    m_mix = mix;
}

float BitCrusher::processSample(float input)
{
    // Quantisieren — auf nächsten Schritt runden
    float crushed = roundf(input / m_step) * m_step;

    // Mix zwischen clean und gecrusht
    return m_mix * crushed + (1.0f - m_mix) * input;
}

void BitCrusher::computeStep()
{
    // Schrittweite = 1 / 2^(bits-1)
    // Bei 8 bit → 1/128 = 0.0078
    // Bei 4 bit → 1/8   = 0.125
    // Bei 1 bit → 1/1   = 1.0 (nur +1 und -1)
    m_step = 1.0f / powf(2.0f, m_bits - 1.0f);
}