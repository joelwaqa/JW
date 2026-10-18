#pragma once
#include <JuceHeader.h>
#include <algorithm>

namespace jayway {
class ModuleBase {
public:
    virtual ~ModuleBase() = default;
    virtual const char* getName() const noexcept = 0;
    virtual void prepare(double sampleRate, int maxBlockSize, int channels) {
        (void) sampleRate; (void) maxBlockSize; (void) channels;
    }
    virtual void process(juce::AudioBuffer<float>& buffer) = 0;
    void setBypassed(bool shouldBypass) noexcept { bypassed = shouldBypass; }
    bool isBypassed() const noexcept { return bypassed; }
    void setBlend(float value) noexcept { blend = std::clamp(value, 0.0f, 1.0f); }
    float getBlend() const noexcept { return blend; }
protected:
    bool bypassed{false};
    float blend{1.0f};
};
}
