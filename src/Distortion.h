#pragma once
#include "AudioEffect.h"
#include <algorithm>

class Distortion : public AudioEffect
{
public:
    enum class Mode
    {
        HARD_CLIP, // Hard clipping — aggressive, square-wave-like
        SOFT_CLIP, // Soft clipping — warmer, tube-like
        FUZZ       // Fuzz — extreme, full-wave rectification
    };

    Distortion(Mode mode = Mode::SOFT_CLIP, float drive = 3.0f, float level = 1.0f)
        : _mode(mode), _drive(drive), _level(level) {}

    void setDrive(float drive) { _drive = drive; } // 1.0 = clean, 10.0 = heavy
    void setLevel(float level) { _level = level; } // output volume 0.0–1.0
    void setMode(Mode mode) { _mode = mode; }

    int32_t process(int32_t sample) override
    {
        if (!_enabled)
            return sample;

        // Normalize to -1.0 … 1.0
        float s = sample / 2147483648.0f;

        // Apply drive (pre-gain)
        s *= _drive;

        // Apply waveshaping based on mode
        switch (_mode)
        {
        case Mode::HARD_CLIP:
            s = hardClip(s);
            break;
        case Mode::SOFT_CLIP:
            s = softClip(s);
            break;
        case Mode::FUZZ:
            s = fuzz(s);
            break;
        }

        // Apply output level
        s *= _level;

        int64_t out = static_cast<int64_t>(s * 2147483648.0f);
        out = std::max<int64_t>(-2147483647LL, std::min<int64_t>(2147483647LL, out));
        return static_cast<int32_t>(out);
    }

private:
    Mode _mode;
    float _drive;
    float _level;

    // Hard clip: anything above +1 or -1 is cut flat
    float hardClip(float s)
    {
        return std::max(-1.0f, std::min(1.0f, s));
    }

    // Soft clip: smooth tanh saturation, tube-amp feel
    float softClip(float s)
    {
        // tanh approximation — cheaper than std::tanhf on ESP32
        // accurate enough for audio
        if (s > 3.0f)
            return 1.0f;
        if (s < -3.0f)
            return -1.0f;
        float s2 = s * s;
        return s * (27.0f + s2) / (27.0f + 9.0f * s2);
    }

    // Fuzz: full-wave rectification + hard clip = angry bees
    float fuzz(float s)
    {
        s = s < 0 ? -s : s;  // absolute value (full-wave rectify)
        s = s * 2.0f - 1.0f; // re-center
        return hardClip(s);
    }
};