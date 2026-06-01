#pragma once
#include <cstdint>

enum LimiterStyle
{
    HARD,
    SOFT
};

class Limiter
{
public:
    /// @brief Erstellt einen neuen Audio-Limiter zum Schutz vor digitalem Clipping.
    /// @param threshold The threshold in Range of [0.0f - 1.0f]. default = 0.95f.
    /// @param limType Die Clipping-Charakteristik (HARD oder SOFT).
    Limiter(float threshold, LimiterStyle limType);

    float processSample(float inputSample);

    void setLimit(float threshold) { m_threshold = threshold; }
    void setType(LimiterStyle limType) { m_limType = limType; }

private:
    float m_threshold;
    LimiterStyle m_limType;
};
