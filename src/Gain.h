#pragma once

#include "AudioEffect.h"

class Gain : public AudioEffect
{
private:
    float _gainFactor = 2.0f;

public:
    Gain(float factor) : _gainFactor(factor) {}

    float process(float input) override
    {
        if (_bypassed)
            return input;
        return input * _gainFactor;
    }
};