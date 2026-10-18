#pragma once
#include "ModuleBase.h"
namespace jayway {
class PreMicHandler : public ModuleBase {
public:
    const char* getName() const noexcept override { return "Pre-Mic"; }
    void process(juce::AudioBuffer<float>& b) override {
        if (bypassed || b.getNumSamples() == 0) return;
        for (int c=0;c<b.getNumChannels();++c) {
            auto* p=b.getWritePointer(c);
            float peak=0.0f; for(int i=0;i<b.getNumSamples();++i) peak=std::max(peak,std::abs(p[i]));
            const float target=juce::Decibels::decibelsToGain(-15.0f);
            const float gain=peak>1.0e-5f ? juce::jlimit(0.25f,4.0f,target/peak) : 1.0f;
            for(int i=0;i<b.getNumSamples();++i) { const float dry=p[i]; p[i]=dry*(1.0f+blend*(gain-1.0f)); }
        }
    }
};
}
