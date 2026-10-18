#pragma once
#include "ModuleBase.h"
namespace jayway {
class VUModule : public ModuleBase {
public:
    const char* getName() const noexcept override { return "VU Meter"; }
    void process(juce::AudioBuffer<float>& b) override {
        if (bypassed) return;
        double sum=0.0; int count=0;
        for(int c=0;c<b.getNumChannels();++c) { const auto* p=b.getReadPointer(c); for(int i=0;i<b.getNumSamples();++i){sum+=p[i]*p[i];++count;} }
        rms = count > 0 ? std::sqrt(sum / count) : 0.0;
    }
    double getRms() const noexcept { return rms; }
private: double rms{0.0};
};
}
