#pragma once
#include <stdint.h>
#include <cstddef>

class AudioEffect
{
public:
    virtual ~AudioEffect() = default;
    // Process a single sample, returns the processed sample
    virtual int32_t process(int32_t sample) = 0;
    // Process a buffer in-place
    void processBuffer(int32_t *buffer, size_t sampleCount)
    {
        for (size_t i = 0; i < sampleCount; i++)
        {
            buffer[i] = process(buffer[i]);
        }
    }
    virtual void setEnabled(bool enabled) { _enabled = enabled; }
    virtual bool isEnabled() const { return _enabled; }

protected:
    bool _enabled = true;
};