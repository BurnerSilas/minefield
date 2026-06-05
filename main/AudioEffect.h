#pragma once

class AudioEffect
{
public:
    virtual ~AudioEffect() = default;

    virtual float processSample(float input) = 0;

    void setBypass(bool bypass) { m_bypass = bypass; }
    bool isBypassed() const { return m_bypass; }

    float process(float input)
    {
        if (m_bypass)
            return input;
        return processSample(input);
    }

protected:
    bool m_bypass = false;
};