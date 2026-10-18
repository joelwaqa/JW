#pragma once
#include "ModuleBase.h"
namespace jayway {
class SmootherModule : public ModuleBase {
public:
    const char* getName() const noexcept override { return "Smoother"; }
    void prepare(double sr, int, int) override { coefficient = static_cast<float>(std::exp(-1.0 / (0.01 * sr))); }
    void process(juce::AudioBuffer<float>& b) override {
        if (bypassed) return;
        for(int c=0;c<b.getNumChannels();++c) { auto* p=b.getWritePointer(c); float s=state[static_cast<size_t>(std::min(c,1))]; for(int i=0;i<b.getNumSamples();++i) { const float x=p[i]; s=coefficient*s+(1.0f-coefficient)*x; p[i]=x+blend*(s-x); } state[static_cast<size_t>(std::min(c,1))]=s; }
    }
private:
    float coefficient{0.99f};
    std::array<float,2> state{};
};
}
