#pragma once

class AudioEffect
{
public:
    AudioEffect() {}

    virtual float process(float input) = 0;

    void setBypass(bool shouldBypass)
    {
        _bypassed = shouldBypass;
    }

protected:
    bool _bypassed = false;
};