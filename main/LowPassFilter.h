#pragma once

/**
 * @brief A first-order IIR Low-Pass Filter to eliminate high-frequency noise.
 */
class LowPassFilter
{
public:
    /**
     * @brief Constructor to initialize the filter with cutoff and sample rate.
     * @param cutoffFrequency Cutoff frequency in Hz (e.g., 5000.0f for bass).
     * @param sampleRate System sample rate in Hz (e.g., 48000.0f).
     */
    LowPassFilter(float cutoffFrequency = 10000.0f, float sampleRate = 48000.0f);

    /**
     * @brief Processes a single audio sample through the low-pass filter.
     * @param inputSample Normalized input sample in the range [-1.0f to 1.0f].
     * @return Filtered output sample.
     */
    float processSample(float inputSample);

    /**
     * @brief Updates the cutoff frequency dynamically at runtime.
     * @param cutoffFrequency New cutoff frequency in Hz.
     */
    void setCutoff(float cutoffFrequency);

private:
    float m_sampleRate;
    float m_alpha;      ///< Filter smoothing factor calculated from cutoff frequency.
    float m_prevOutput; ///< Memory state storing the previous filtered sample.
};